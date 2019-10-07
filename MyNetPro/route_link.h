#ifndef ROUTE_LINK_H
#define ROUTE_LINK_H

//过滤链表结构体，可过滤ip/port/协议/
typedef struct route_link
{
    unsigned char network[4];    	    //目的网段,4字节
    unsigned char mask[4];    	        //目的网段掩码,4字节
    unsigned char nexthop[4];			//下一跳IP，2字节
    struct route_link *front;			//链表的上一个节点
    struct route_link *next;			//链表的下一个节点
}ROUTE_LINK;

extern ROUTE_LINK *route_head;          //路由表头指针变量
extern void init_route_link(void);       //路由表链表初始化
extern void route_link_print(ROUTE_LINK *route_head);

#endif