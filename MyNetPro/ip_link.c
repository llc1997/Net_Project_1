//#include "ip_link.h"
#include <stdlib.h>

//过滤链表结构体，可过滤ip/port/协议/
typedef struct ip_link
{
    char filter_ip[4];    			//Ip的IP地址,4字节
    unsigned short filter_port;		//过滤的port，2字节
    char filter_agree[2];			//过滤的协议，2字节
    struct ip_link *front;			//链表的上一个节点
    struct ip_link *next;			//链表的下一个节点
}IP_LINK;

IP_LINK *ip_head=NULL;


//ip初始化函数（主控）
void init_ip_link(void)
{
    printf("init_ip_link初始化Ip链表\n");
#if 0
	ip_head = (IP_LINK *)malloc(sizeof(IP_LINK));
	//需读取文件，提取文件内容初始化
	memcpy(ip_head->filter_ip,"");
	ip_head->filter_port=0;
	memcpy(ip_head->filter_agree,"");
	ip_head->front=ip_head;
	ip_head->next=ip_head;
#endif
}

//查找IP的函数，在ip_head所指向的链表
IP_LINK *find_ip(IP_LINK *ip_head, unsigned char * recv_buff)
{
	return NULL;
#if 0
    IP_LINK *ip_find = ip_head;
    char ip_src[5];
    bzero(ip_src,sizeof(ip_src));
    memcpy(ip_src, recv_buff, 4);//查找过滤ip，来源Ip
    ip_src[7] = '\0';

	if(ip_find == NULL)
		return NULL;
	while(ip_find->next != head){
		if(strncmp(ip_find.filter_ip,ip_src,4) == 0)
			return ip_find;
		ip_find = ip_find->next;		
	}	
	if(strncmp(ip_find.filter_ip,ip_src,4) == 0)
		return ip_find;	
	else
		return ip_find;
#endif 
}

#if 0
//删除IP链表节点（基本不用）
IP_LINK *delete_ip(IP_LINK *ip_head,char ip_del[4])
{
	IP_LINK *p = ip_head,*pb;
	if(head == NULL)
		return NULL;
	while(p->next != head){
		if(ip_del[0] == p->ip){//找出要删除的节点   
			if(p == head)//被删除的是头节点
				ip_head = ip_head->next;		
			break;
		}
		pb = p;
		p = p->next;		
	}
	if(p->m_time == m_time){//头、中间、结尾都需要交换
		p->next->front = p->front;
		p->front->next = p->next;	
		free(p);
		if(p == ip_head)//只有一个节点的情况
			return NULL;
		else
			return ip_head;
	}
	return ip_head;
}
#endif
