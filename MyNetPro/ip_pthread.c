#include <stdlib.h>
#include <stdio.h>
#include "ip_pthread.h"
#include "arp_link.h"
#include "get_interface.h"

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
		printf("\n目的IP：%d.%d.%d.%d\n",recv_ip[0],recv_ip[1],recv_ip[2],recv_ip[3]);
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
		if(strncmp(network, recv_ip, netmask_len) ==0)
		{
			printf("接口网段是：%d.%d.%d.%d\n",network[0],network[1],network[2],network[3]);
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
	if(strncmp(recvip,net_interface[interface].br_ip,4) ==0)
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
	if(strncmp(recvip,loopback_ip,4) ==0)
		return 1;
	else
		return 0;
}

//功能：ARP链表是否有对应IP
//参数：ARP_LINK *arp_head：arp表头指针；unsigned char *recvip：目标IP
//返回值：有返回arp表节点指针，无返回NULL
ARP_LINK* fun_arp_seek_(ARP_LINK *arp_head, unsigned char *recv_ip)
{
	ARP_LINK *arp_find = arp_head;
	if(arp_find == NULL)
		return NULL;
	while(arp_find->next != arp_head)	//未到尾节点
	{
		if(strncmp(arp_find->arp_ip,recv_ip,4) == 0)
			return arp_find;
		arp_find = arp_find->next;
	}
	if(strncmp(arp_find->arp_ip,recv_ip,4) == 0)
		return arp_find; 
	else
		return NULL;
}

void *ip_pthread(void *recv)//参数为接收到的数组包结构体
{
    	//printf("IP数据包处理函数\n");
	unsigned char recv_ip[5];	//提取目的IP
	memcpy(recv_ip, ((RECV_DATA *)recv)->data+30, 4);
	recv_ip[4]='\0';
		//printf("进入线程，目的IP：%02x.%02x.%02x.%02x\n",recv_ip[0],recv_ip[1],recv_ip[2],recv_ip[3]);
	//1、判断活跃网卡???
	int ret_interface =  fun_interface(recv_ip); //返回的值是网口数组的下标，-1为未找到
		//printf("ret_interface=%d $$$$\n",ret_interface);
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
		printf("ret_loopback=%d *************\n",ret_loopback);
		if(ret_loopback)
		{
			return NULL;	//是回环，退出线程
		}
			printf("&&&&&&&&&&&&&&&&&&&&&&&&\n");
		//4、查找arp链表
		int num_arp_requst = 0;  //arp请求第几次？
		while(1)
		{
			ARP_LINK *ret_arp_link = fun_arp_seek_(arp_head,(unsigned char *)recv_ip);	//查询ARP链表，看能否找到IP
			//找到返回ARP链表的节点指针，没找到返回空
			printf("fun_arp_seek_:%p\n",ret_arp_link);
			if(ret_arp_link == NULL && num_arp_requst < 3)
			{
				//5、发送arp请求包
				//发送arp请求包，四次，延时时间，在判断是否arp中有
				printf("发送arp请求*************************\n");
				num_arp_requst++;
				
			}
			else 
			{
				//6、重组数据包
				printf("转发数据*************************\n");
				//7、发送数据包
				break;
			}
			sleep(1);
		}
		

		
		
	}
}



