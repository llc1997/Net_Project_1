#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "ip_pthread.h"
#include "arp_link.h"
#include "get_interface.h"
#include <sys/ioctl.h>
#include <sys/types.h>	
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <netinet/if_ether.h>
#include <net/if.h>
#include <arpa/inet.h>
#include "route_link.h"
#include <string.h>

typedef struct recv_data
{
    size_t data_len;    //数据长度
    char data[2048];    //数据内容，需修改优化
}RECV_DATA;

//数据包校验和函数
unsigned short checksum(unsigned short *buf, int len)
{
	int nword = len/2;
	unsigned long sum;
	if(len%2 == 1)
		nword++;
	for(sum = 0; nword > 0; nword--)
	{
		sum += *buf;
		buf++;
	}
	sum = (sum>>16) + (sum&0xffff);
	sum += (sum>>16);
	return ~sum;
}

//功能：接口查询函数，
//参数：unsigned char* recv：数据包的IP起始地址
//返回值：找到返回网口数组的下标，-1为未找到
int fun_interface(unsigned char* recv_ip)
{
	int interface = get_interface_num();  //获取接口数量
		//printf("网口查询函数interface = %d\n",interface);
		//printf("\n目的IP：%d.%d.%d.%d\n",recv_ip[0],recv_ip[1],recv_ip[2],recv_ip[3]);
	int i,j,netmask_len;	//i，j为循环变量，netmask_len是掩码长度
	unsigned char network[4];
	for(i=0;i<interface;i++)
	{
			//printf("数组保存的IP：%d.%d.%d.%d\n",
			//(net_interface[i]).ip[0],(net_interface[i]).ip[1],
			//(net_interface[i]).ip[2],(net_interface[i]).ip[3]);
		//获取网段,暂认为网段均为24位
		for(j=0;j<4;j++)
		{
			network[j] = (((net_interface[i]).ip)[j] & ((net_interface[i]).netmask)[j]);
		}
		netmask_len=3;
			//printf("network：%d.%d.%d.%d\n",network[0],network[1],network[2],network[3]);
			//int ret = strncmp(network, recv_ip, netmask_len);
			//printf("strncmp:%d\n",ret);
		if(memcmp((char *)network, (char *)recv_ip, netmask_len) ==0)
		{
			//printf("接口网段是：%d.%d.%d.%d\n",network[0],network[1],network[2],network[3]);
			return i;
		}
	}
	return -1;
}

//功能：判断是否为广播
//参数：unsigned char *recvip：目标IP，interface：接口数组的下标
//返回值：是广播返回1，非广播返回0
int fun_broadcast(unsigned char *recvip, int interface)
{
	if(memcmp((char *)recvip,(char *)net_interface[interface].br_ip,4) ==0)
		return 1;
	else
		return 0;
}

//功能：判断是否为回环地址
//参数：unsigned char *recvip：目标IP，interface：接口数组的下标
//返回值：是回环返回1，非回环返回0
int fun_loopback(unsigned char *recvip, int interface)
{
	char loopback_ip[4] = {127,0,0,1};
		//printf("回环：%d.%d.%d.%d\n",loopback_ip[0],loopback_ip[1],
		//	loopback_ip[2],loopback_ip[3]);
	if(strncmp((char *)recvip,(char *)loopback_ip,4) ==0)
		return 1;
	else
		return 0;
}

//功能：发送ARP请求包
//参数：unsigned char *recv_ip：目标IP,int interface:接口数组的下标
//返回值：1 arp请求包发送失败，0发送成功
int fun_arp_send(unsigned char *recv_ip,int interface)
{
	int sockfd;
	int len = 42;
	struct sockaddr_ll sll;
	struct ifreq ethreq;
	//生成使用ICMP的原始套接字，只有root才能生成
	if((sockfd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL))) < 0)//分别测试各个协议: IPPROTO_ICMP 1 IPPROTO_TCP 6 IPPROTO_UDP 17
	{
		printf("%d\n",sockfd);
		perror("socket error:");
		return 1; 
	}
	strcpy(ethreq.ifr_name,net_interface[interface].name);
	if(-1 == ioctl(sockfd,SIOCGIFINDEX,&ethreq)){
		perror("ioctl");
		close(sockfd);
		return 1;
	}
	bzero(&sll, sizeof(sll));
	sll.sll_ifindex = ethreq.ifr_ifindex;
		//printf("报文处理开始\n");
	unsigned char arp_send_buf[2048]={ 	//00-22-68-1A-F3-09
		0xff,0xff,0xff,0xff,	//目的mac地址前四字节
		0xff,0xff,				//目的mac地址后两字节
		0,0,0,0,0,0,			//源mac地址的后六字节
		0x08,0x06,				//协议类型2字节
		0x00,0x01,				//硬件地址类型(1表示以太网地址)	
		0x08,0x00,				//要映射的协议类型(0x800表示IP地址)			
		6,4,0x00,0x01,			//位
		0,0,0,0,0,0,			//源mac
		0,0,0,0,				//本地IP，那个网口发送那个就接收
		0,0,0,0,0,0,			//目的mac
		0,0,0,0,				//目的ip
	};	
	memcpy(&arp_send_buf[6],net_interface[interface].mac,6);
	memcpy(&arp_send_buf[22],net_interface[interface].mac,6);
	memcpy(&arp_send_buf[28],net_interface[interface].ip,4);
	memcpy(&arp_send_buf[38],recv_ip,4);
	int ret_send = sendto(sockfd,arp_send_buf,len,0,(struct sockaddr *)&sll,sizeof(sll));
	if(ret_send != len)
	{	//发送失败
		return 1;
	}
	close(sockfd);
	return 0;
}

//功能：转发数据包
//参数：int interface:接口数组的下标
//		RECV_DATA* recv:接收的数据结构体
//		ARP_LINK* arp_link：ARP链表节点指针，取mac地址
//返回值：1 arp请求包发送失败，0发送成功
int fun_data_send(int interface,RECV_DATA* recv, unsigned char *arp_mac)
{
	int sockfd;
	int len_data = recv->data_len;
	struct sockaddr_ll sll;
	struct ifreq ethreq;
	//生成使用ICMP的原始套接字，只有root才能生成
	if((sockfd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL))) < 0)//分别测试各个协议: IPPROTO_ICMP 1 IPPROTO_TCP 6 IPPROTO_UDP 17
	{
		printf("%d\n",sockfd);
		perror("socket error:");
		return 1; 
	}
	strcpy(ethreq.ifr_name,net_interface[interface].name);
	if(-1 == ioctl(sockfd,SIOCGIFINDEX,&ethreq)){
		perror("ioctl");
		close(sockfd);
		return 1;
	}
	bzero(&sll, sizeof(sll));
	sll.sll_ifindex = ethreq.ifr_ifindex;
		//printf("报文处理开始\n");
	memcpy(recv->data,arp_mac,6);
#if 0 	//TTL修改并校验                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
	recv->data[22]--;
	printf("TTL:%d\n",recv->data[22]);
	if(recv->data[22] <= 0)
	{
		return 0;
	}
	*((unsigned short *)&(recv->data[24]))=0;
	*((unsigned short *)&(recv->data[24])) = checksum((unsigned short *)((recv->data)+14),20);
#endif
	int ret_send=0;
	ret_send = sendto(sockfd,recv->data,len_data,0,(struct sockaddr *)&sll,sizeof(sll));
	if(ret_send != len_data)
	{
		return 1;
	}
	//printf("data：ret_send=%d,len=%d\n",ret_send,len_data);
	close(sockfd);
	return 0;
}

//功能：查找路由表是否有对应目标网段
//参数：unsigned char * recv_ip
//返回值：null 没有找到目标网段，找到返回节点指针
ROUTE_LINK* fun_insert_route(unsigned char * recv_ip)
{
	ROUTE_LINK *route = route_head;
	int j;
	unsigned char network[4];
	while (route->next != route_head)
	{
		for(j=0;j<4;j++)  //与掩码相与，得到目标网段
		{
			network[j] = recv_ip[j] & (route->mask)[j];
		}
		if(memcmp((char *)network, (char *)route->network, 4) ==0)
		{
			//printf("接口网段是：%d.%d.%d.%d\n",network[0],network[1],network[2],network[3]);
			return route;
		}
	}//判断最后一个
	if(memcmp((char *)network, (char *)route->network, 4) ==0)
	{
		return route;
	}
	else
		return NULL;
}

void *ip_pthread(void *recv)//参数为接收到的数组包结构体
{
    	//printf("\nIP数据包处理函数\n");
	unsigned char recv_ip[5];	//提取目的IP
	unsigned char src_ip[5];
	if (((RECV_DATA *)recv)->data[13]==0x00)  //IP数据包
	{
		memcpy(recv_ip, ((RECV_DATA *)recv)->data+30, 4);
		memcpy(src_ip, ((RECV_DATA *)recv)->data+26, 4);
		recv_ip[4]='\0';
		src_ip[4]='\0';
	}
#if 0
	else if(((RECV_DATA *)recv)->data[13]==0x06) //ARP请求协议包
	{
		//printf("ARP数据包转发！\n");
		memcpy(recv_ip, ((RECV_DATA *)recv)->data+38, 4);
		memcpy(src_ip, ((RECV_DATA *)recv)->data+28, 4);
		recv_ip[4]='\0';
		src_ip[4]='\0';
	}
#endif
	else return NULL;	//不是IP报文，直接退出
	//判断是否是本机发送
	int i;
	int interface = get_interface_num();  //获取接口数量
	for(i=0;i<interface;i++)
	{
		if(memcmp((char *)((net_interface[i]).mac), ((RECV_DATA *)recv)->data+6, 6) ==0)
		{	//是本机网口转发发出的，不再转发
			return NULL;
		}
		if(memcmp((char *)((net_interface[i]).ip), recv_ip, 4) ==0)
		{	//目的是本机的，不再转发
			return NULL;
		}
	}
		//printf("\n进入线程，目的IP：%d.%d.%d.%d\n",recv_ip[0],recv_ip[1],recv_ip[2],recv_ip[3]);
	//1、判断活跃网卡
	int ret_interface =  fun_interface(recv_ip); //返回的值是网口数组的下标，-1为未找到
		//printf("ret_interface=%d $$$$\n",ret_interface);
	if(ret_interface < 0 || ret_interface > 15) 	
	{
		//1、查找路由表是否存在  没有退出
		ROUTE_LINK* ret_route = fun_insert_route(recv_ip); //有返回链表的指针，无返回空
		if(ret_route == NULL)
		{
			return NULL;	//没有目标网段，退出
		}
		//2、替换IP为下一跳的IP
		unsigned char route_ip[5];	//提取目的IP
		memcpy(route_ip,ret_route->nexthop,4);
		//3、查找到达目标网段的本地接口  没有退出
		ret_interface =  fun_interface(route_ip); //返回的值是网口数组的下标，-1为未找到
		if(ret_interface < 0 || ret_interface > 15)
		{
			return NULL;
		}
	}
	if(ret_interface < 0 || ret_interface > 15) //返回值是合法网口的数组下标
	{	//查找路由表之后依然没有找到对应网口
		return NULL;
	}
	//2、判断是否为广播地址，//1为是广播，0为不是广播
		//int ret_broadcast = fun_broadcast(recv_ip,ret_interface); 
	if(fun_broadcast(recv_ip,ret_interface))
	{
		return NULL;	//是广播，退出线程
	}
	//3、判断是否为回环地址,1为回环，0为非回环
		//int ret_loopback = fun_loopback(recv_ip,ret_interface);	
	if(fun_loopback(recv_ip,ret_interface))
	{
		return NULL;	//是回环，退出线程
	}
#if 0
	printf("IP转发线程，目的IP：%d.%d.%d.%d源ip：%d.%d.%d.%d\n",
		recv_ip[0],recv_ip[1],recv_ip[2],recv_ip[3],
		src_ip[0],src_ip[1],src_ip[2],src_ip[3]);
#endif
	//4、查找arp链表
	int num_arp_requst = 0;  //arp请求第几次？
	while(1)
	{
		//找到返回ARP链表的节点指针，没找到返回空
		ARP_LINK *ret_arp_link = arp_link_seek(arp_head,(unsigned char *)recv_ip);	//查询ARP链表，看能否找到IP
		if(ret_arp_link == NULL && num_arp_requst < 3)
		{
			//5、发送arp请求包
				//发送arp请求包，3次，总共9次，延时时间，在判断是否arp中有
			fun_arp_send(recv_ip,ret_interface);
			fun_arp_send(recv_ip,ret_interface);
			fun_arp_send(recv_ip,ret_interface);
			printf("arp requesting ...\n");	
			num_arp_requst++;
			sleep(1);
		}
		else if(ret_arp_link != NULL)
		{
			//6、重组数据包,发送数据包
#if 0
			printf("重组数据包：ip:%d.%d.%d.%d\n", //mac:%02x.%02x.%02x.%02x.%02x.%02x\n
				ret_arp_link->arp_ip[0],ret_arp_link->arp_ip[1],
				ret_arp_link->arp_ip[2],ret_arp_link->arp_ip[3]);
#endif
			memcpy(((RECV_DATA*)recv)->data+6,net_interface[ret_interface].mac,6);
			if(fun_data_send(ret_interface,(RECV_DATA*)recv,(unsigned char *)(ret_arp_link->arp_mac)))
			{
				//printf("data发送失败，本次数据未转发！\n");
				continue;
			}
			else{
				//printf("data发送成功，本次数据转发结束******！\n");
			}
			break;
		}else{
			printf("arp请求失败，本次数据未转发！\n");	
			break;
		}
	}
	return NULL;
}

