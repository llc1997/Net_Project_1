#ifndef ARP_LINK_H
#define ARP_LINK_H


typedef struct arp_link
{
    char arp_ip[4];         	//ip地址
    char arp_mac[6];        	//mac地址
    char arp_inter[12];       	//发送的接口及网口，如ens33
    struct arp_link *front;		//上一个节点
    struct arp_link *next;     	//下一个ARP节点
}ARP_LINK;

//ARP链表的头指针
extern ARP_LINK *arp_head;
////创建插入arp链表节点函数，不用有序
extern ARP_LINK *arp_link_insert(ARP_LINK *arp_head, ARP_LINK *pnew);



#endif

