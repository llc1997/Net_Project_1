#include <stdlib.h>
#include <stdio.h>
<<<<<<< HEAD
#include <unistd.h>
=======
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab
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
<<<<<<< HEAD
#include "route_link.h"
#include <string.h>
=======
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab

typedef struct recv_data
{
    size_t data_len;    //数据长度
    char data[2048];    //数据内容，需修改优化
}RECV_DATA;

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
		//获取网段,暂认为网段均为24位
			//printf("数组保存的IP：%d.%d.%d.%d\n",(net_interface[i]).ip[0],(net_interface[i]).ip[1],(net_interface[i]).ip[2],(net_interface[i]).ip[3]);
		for(j=0;j<4;j++)
		{
			network[j] = (((net_interface[i]).ip)[j] & ((net_interface[i]).netmask)[j]);
		}
		netmask_len=3;
			//printf("network：%d.%d.%d.%d\n",network[0],network[1],network[2],network[3]);
			//int ret = strncmp(network, recv_ip, netmask_len);
			//printf("strncmp:%d\n",ret);
<<<<<<< HEAD
		if(strncmp((char *)network, (char *)recv_ip, netmask_len) ==0)
=======
		if(strncmp(network, recv_ip, netmask_len) ==0)
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab
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
<<<<<<< HEAD
	if(strncmp((char *)recvip,(char *)net_interface[interface].br_ip,4) ==0)
=======
	if(strncmp(recvip,net_interface[interface].br_ip,4) ==0)
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab
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
<<<<<<< HEAD
	if(strncmp((char *)recvip,(char *)loopback_ip,4) ==0)
=======
	if(strncmp(recvip,loopback_ip,4) ==0)
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab
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
	//printf("sockfd=%d\n",sockfd);
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
		//0x00,0x0c,				//源mac地址的前两字节
		//0x29,0xcd,0x23,0x36,	//源mac地址的后四字节
		0,0,0,0,0,0,
		0x08,0x06,				//协议类型2字节
		0x00,0x01,				//硬件地址类型(1表示以太网地址)	
		0x08,0x00,				//要映射的协议类型(0x800表示IP地址)			
		6,4,0x00,0x01,			//位
		//0x00,0x0c,0x29,0xcd,0x23,0x36,
		0,0,0,0,0,0,
		0,0,0,0,				//本地IP，那个网口发送那个就接收
		0x00,0x00,0x00,0x00,0x00,0x00,
		0,0,0,0,
	};	
	memcpy(&arp_send_buf[6],net_interface[interface].mac,6);
	memcpy(&arp_send_buf[22],net_interface[interface].mac,6);
	memcpy(&arp_send_buf[28],net_interface[interface].ip,4);
	memcpy(&arp_send_buf[38],recv_ip,4);
	sendto(sockfd,arp_send_buf,len,0,(struct sockaddr *)&sll,sizeof(sll));
	close(sockfd);
	return 0;
}

//功能：转发数据包
//参数：int interface:接口数组的下标
//		RECV_DATA* recv:接收的数据结构体
//		ARP_LINK* arp_link：ARP链表节点指针，取mac地址
//返回值：1 arp请求包发送失败，0发送成功
<<<<<<< HEAD
int fun_data_send(int interface,RECV_DATA* recv, unsigned char *arp_mac)
=======
int fun_data_send(int interface,RECV_DATA* recv,ARP_LINK* arp_link)
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab
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
	//printf("sockfd=%d\n",sockfd);
	strcpy(ethreq.ifr_name,net_interface[interface].name);
	if(-1 == ioctl(sockfd,SIOCGIFINDEX,&ethreq)){
		perror("ioctl");
		close(sockfd);
		return 1;
	}
	bzero(&sll, sizeof(sll));
	sll.sll_ifindex = ethreq.ifr_ifindex;
		//printf("报文处理开始\n");
<<<<<<< HEAD
	memcpy(recv->data,arp_mac,6);
=======
	memcpy(recv->data,arp_link->arp_mac,6);
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab
	sendto(sockfd,recv->data,len_data,0,(struct sockaddr *)&sll,sizeof(sll));
	close(sockfd);
	return 0;
}

<<<<<<< HEAD
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
		if(strncmp((char *)network, (char *)route->network, 4) ==0)
		{
			//printf("接口网段是：%d.%d.%d.%d\n",network[0],network[1],network[2],network[3]);
			return route;
		}
	}
	return NULL;
}


void *ip_pthread(void *recv)//参数为接收到的数组包结构体
{
    	printf("IP数据包处理函数\n");
	unsigned char recv_ip[5];	//提取目的IP
	if (((RECV_DATA *)recv)->data[13]==0x00)  //IP数据包
	{
		memcpy(recv_ip, ((RECV_DATA *)recv)->data+30, 4);
		recv_ip[4]='\0';
	}
	else if(((RECV_DATA *)recv)->data[13]==0x06) //ARP请求协议包
	{
		memcpy(recv_ip, ((RECV_DATA *)recv)->data+38, 4);
		recv_ip[4]='\0';
	}
=======

void *ip_pthread(void *recv)//参数为接收到的数组包结构体
{
    	//printf("IP数据包处理函数\n");
	unsigned char recv_ip[5];	//提取目的IP
	memcpy(recv_ip, ((RECV_DATA *)recv)->data+30, 4);
	recv_ip[4]='\0';
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab
		//printf("进入线程，目的IP：%02x.%02x.%02x.%02x\n",recv_ip[0],recv_ip[1],recv_ip[2],recv_ip[3]);
	//1、判断活跃网卡???
	int ret_interface =  fun_interface(recv_ip); //返回的值是网口数组的下标，-1为未找到
		//printf("ret_interface=%d $$$$\n",ret_interface);
<<<<<<< HEAD
	if(ret_interface < 0 || ret_interface > 15) 	
	{
		//1、查找是否存在  没有退出
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
		//未找到网段对应的网卡
		//查询路由表，后续完善
		//return NULL;
	}
	//else if(ret_interface >= 0 && ret_interface <= 15) //返回值是合法网口的数组下标
	//{
		//2、判断是否为广播地址，
			//int ret_broadcast = fun_broadcast(recv_ip,ret_interface); //1为是广播，0为不是广播
	if(fun_broadcast(recv_ip,ret_interface))
	{
		return NULL;	//是广播，退出线程
	}
		printf("***********************\n");
	//3、判断是否为回环地址
	int ret_loopback = fun_loopback(recv_ip,ret_interface);	//1为回环，0为非回环
		//printf("ret_loopback=%d *************\n",ret_loopback);
	if(ret_loopback)
	{
		return NULL;	//是回环，退出线程
	}
		//printf("&&&&&&&&&&&&&&&&&&&&&&&&\n");
		printf("\n目的IP：%d.%d.%d.%d\n",recv_ip[0],recv_ip[1],recv_ip[2],recv_ip[3]);
	//4、查找arp链表
	int num_arp_requst = 0;  //arp请求第几次？
	while(1)
	{
		ARP_LINK *ret_arp_link = arp_link_seek(arp_head,(unsigned char *)recv_ip);	//查询ARP链表，看能否找到IP
			//找到返回ARP链表的节点指针，没找到返回空
		//printf("fun_arp_seek_:%p\n",ret_arp_link);
		if(ret_arp_link == NULL && num_arp_requst < 3)
		{
			//5、发送arp请求包
				//发送arp请求包，四次，延时时间，在判断是否arp中有
			fun_arp_send(recv_ip,ret_interface);
			fun_arp_send(recv_ip,ret_interface);
			fun_arp_send(recv_ip,ret_interface);
			if(fun_arp_send(recv_ip,ret_interface))
				continue;	//发送失败不计数，进入下一次发送
			printf("发送arp请求*************************\n");
			num_arp_requst++;
		}
		else if(ret_arp_link != NULL)
		{
			//6、重组数据包,发送数据包
			//printf("转发数据*************************\n");
			//printf("重组数据包：ip:%d.%d.%d.%d\n", //mac:%02x.%02x.%02x.%02x.%02x.%02x\n
			//	ret_arp_link->arp_ip[0],ret_arp_link->arp_ip[1],
			//	ret_arp_link->arp_ip[2],ret_arp_link->arp_ip[3]);
			//arp_link_print(arp_head);
			if(fun_data_send(ret_interface,(RECV_DATA*)recv,(unsigned char *)(ret_arp_link->arp_mac)))
			{
				printf("data发送失败，本次数据未转发！\n");
				continue;
			}
			break;
		}else{
			printf("arp请求失败，本次数据未转发！\n");	
			break;
		}
		sleep(1);
	}
	return NULL;
	//}
}

=======
	if(ret_interface == -1) 	
	{
		//未找到网段对应的网卡
		//查询路由表，后续完善
		return NULL;
	}
	else if(ret_interface >= 0 && ret_interface <= 15) //返回值是合法网口的数组下标
	{
		//2、判断是否为广播地址，
			//int ret_broadcast = fun_broadcast(recv_ip,ret_interface); //1为是广播，0为不是广播
		if(fun_broadcast(recv_ip,ret_interface))
		{
			return NULL;	//是广播，退出线程
		}
			printf("***********************\n");
		//3、判断是否为回环地址
		int ret_loopback = fun_loopback(recv_ip,ret_interface);	//1为回环，0为非回环
			//printf("ret_loopback=%d *************\n",ret_loopback);
		if(ret_loopback)
		{
			return NULL;	//是回环，退出线程
		}
			//printf("&&&&&&&&&&&&&&&&&&&&&&&&\n");
			printf("\n目的IP：%d.%d.%d.%d\n",recv_ip[0],recv_ip[1],recv_ip[2],recv_ip[3]);
		//4、查找arp链表
		int num_arp_requst = 0;  //arp请求第几次？
		while(1)
		{
			ARP_LINK *ret_arp_link = arp_link_seek(arp_head,(unsigned char *)recv_ip);	//查询ARP链表，看能否找到IP
				//找到返回ARP链表的节点指针，没找到返回空
			//printf("fun_arp_seek_:%p\n",ret_arp_link);
			if(ret_arp_link == NULL && num_arp_requst < 3)
			{
				//5、发送arp请求包
					//发送arp请求包，四次，延时时间，在判断是否arp中有
				fun_arp_send(recv_ip,ret_interface);
				fun_arp_send(recv_ip,ret_interface);
				fun_arp_send(recv_ip,ret_interface);
				if(fun_arp_send(recv_ip,ret_interface))
					continue;	//发送失败不计数，进入下一次发送
				printf("发送arp请求*************************\n");
				num_arp_requst++;
			}
			else if(ret_arp_link != NULL)
			{
				//6、重组数据包,发送数据包
				//printf("转发数据*************************\n");
				//printf("重组数据包：ip:%d.%d.%d.%d\n", //mac:%02x.%02x.%02x.%02x.%02x.%02x\n
				//	ret_arp_link->arp_ip[0],ret_arp_link->arp_ip[1],
				//	ret_arp_link->arp_ip[2],ret_arp_link->arp_ip[3]);
				//arp_link_print(arp_head);
				if(fun_data_send(ret_interface,(RECV_DATA*)recv,ret_arp_link))
				{
					printf("data发送失败，本次数据未转发！\n");
					continue;
				}
				break;
			}else{
				printf("arp请求失败，本次数据未转发！\n");	
				break;
			}
			sleep(1);
		}
		
	}
}



>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab
