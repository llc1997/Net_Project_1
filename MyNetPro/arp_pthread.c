#include "arp_pthread.h"
#include "arp_link.h"
<<<<<<< HEAD
#include "ip_pthread.h"
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/types.h>	
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <netinet/if_ether.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <string.h>
#include "get_interface.h"
#include <unistd.h>
#include <stdlib.h>

int fun_interface(unsigned char* recv_ip);
int fun_arp_reply(int interface,RECV_DATA* recv);

void *arp_pthread(void *arg)  //传的是P指针
=======
#include <unistd.h>

void *arp_pthread(void *arg)
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab
{
	//unsigned char recv_arp_ip[4];
	//unsigned char recv_arp_mac[6];
	ARP_LINK *ret_arp_seek = arp_link_seek(arp_head,((ARP_LINK *)arg)->arp_ip);
	if(ret_arp_seek == NULL)		//没有对应的IP，直接插入arp表
		arp_head = arp_link_insert(arp_head,(ARP_LINK *)arg);
<<<<<<< HEAD
	else if(strncmp((char *)ret_arp_seek->arp_mac,(char *)((ARP_LINK *)arg)->arp_mac,6) != 0)
	{	//有对应的IP，且mac不一致
		arp_head = arp_link_insert(arp_head,(ARP_LINK *)arg);
	}
	return NULL;
	//printf("ARP应答数据包处理线程,插入一个ARP节点\n");
}

void arp_request_pthread(RECV_DATA* recv)	//传递的是数据包结构体
{
	//如果是本机的端口，直接回复应答
	unsigned char recv_ip[5];	//提取目的IP
	memcpy(recv_ip, &(((RECV_DATA *)recv)->data[38]), 4);
	recv_ip[4]='\0';
	int ret_interface =  fun_interface(recv_ip); //返回的值是网口数组的下标，-1为未找到
	if(ret_interface != -1) 
	{  	//找到，是本地网口，直接回复应答
		//if(fun_data_send(ret_interface,(RECV_DATA*)recv,(unsigned char *)(ret_arp_link->arp_mac))))
		if(fun_arp_reply(ret_interface,(RECV_DATA*)recv))
		{
			printf("data发送失败，本次数据未转发！\n");
			//continue;
		}
	}
	else{ //未找到，不是本地网口，要转发
		pthread_t IP_R;
		pthread_create(&IP_R, NULL,ip_pthread, (void*)recv);	//IP数据包转发线程
		pthread_detach(IP_R);
	}
	//否则转发
}


//功能：回复应答ARP数据包
//参数：int interface:接口数组的下标
//		RECV_DATA* recv:接收的数据结构体
//		ARP_LINK* arp_link：ARP链表节点指针，取mac地址
//返回值：1 arp应答包发送失败，0发送成功
int fun_arp_reply(int interface,RECV_DATA* recv)
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
	//构建ARP应答数据包
	unsigned char arp_send_buf[64]={ 	//00-22-68-1A-F3-09
		0x00,0x00,0x00,0x00,0x00,0x00,	//目的mac地址
		//0x00,0x0c,				//源mac地址的前两字节
		//0x29,0xcd,0x23,0x36,	//源mac地址的后四字节
		0,0,0,0,0,0,
		0x08,0x06,				//协议类型2字节
		0x00,0x01,				//硬件地址类型(1表示以太网地址)	
		0x08,0x00,				//要映射的协议类型(0x800表示IP地址)			
		6,4,0x00,0x01,			//位
		//0x00,0x0c,0x29,0xcd,0x23,0x36,
		0,0,0,0,0,0,					//本地mac
		0,0,0,0,						//本地IP，那个网口发送那个就接收
		0x00,0x00,0x00,0x00,0x00,0x00,	//目标mac
		0,0,0,0,						//目的IP
	};	
	memcpy(arp_send_buf,(recv->data)+6,6);			//
	memcpy(arp_send_buf+6,net_interface[interface].mac,6);
	memcpy(arp_send_buf+22,net_interface[interface].mac,6);
	memcpy(arp_send_buf+28,net_interface[interface].ip,4);
	memcpy(arp_send_buf+32,(recv->data)+6,6);
	memcpy(arp_send_buf+38,(recv->data)+28,4);
	//发送
	sendto(sockfd,recv->data,len_data,0,(struct sockaddr *)&sll,sizeof(sll));
	close(sockfd);
	return 0;
=======
	else if(strncmp(ret_arp_seek->arp_mac,((ARP_LINK *)arg)->arp_mac,6) != 0)
	{	//有对应的IP，且mac不一致
		arp_head = arp_link_insert(arp_head,(ARP_LINK *)arg);
	}
	//printf("ARP应答数据包处理线程,插入一个ARP节点\n");
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab
}