#ifndef IP_LINK_H
#define IP_LINK_H

//过滤链表结构体，可过滤ip/port/协议/
typedef struct ip_link
{
    unsigned char f_dest_ip[4];    	//过滤目的Ip,4字节
    unsigned char f_src_ip[4];    	//过滤目的Ip,4字节
    unsigned short f_port;			//过滤的port，2字节
    char f_agree[2];				//过滤的协议，2字节,协议代码
    struct ip_link *front;			//链表的上一个节点
    struct ip_link *next;			//链表的下一个节点
}IP_LINK;

extern IP_LINK *ip_head;       //ip头指针变量
//功能：初始化IP链表
//参数：无
//返回值：空
 extern void init_ip_link(void);

 //功能：有序插入IP地址进链表IP_LINK
 //参数：IP_LINK*insert_ip_head ：IP链表的头指针
 //返回值：IP_LINK * : 新的头指针
 extern IP_LINK *insert_ip_link(IP_LINK *insert_ip_head);   

//查找IP的函数，在ip_head所指向的链表
//extern IP_LINK *find_ip(IP_LINK *ip_head, unsigned char * recv_buff);

extern IP_LINK* ip_link_insert(IP_LINK *ip_head, IP_LINK *pnew);
extern IP_LINK *find_iplink_ip(IP_LINK *ip_head, unsigned char * recv_ip);
extern IP_LINK *find_iplink_srcip(IP_LINK *ip_head, unsigned char * recv_ip);
extern void ip_link_print(IP_LINK *ip_head);


#endif