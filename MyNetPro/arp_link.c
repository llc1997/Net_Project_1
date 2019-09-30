#include <stdlib.h>

typedef struct arp_link
{
    char arp_ip[4];         	//ip地址
    char arp_mac[6];        	//mac地址
    char arp_inter[12];       	//发送的接口及网口，如ens33
    struct arp_link *front;		//上一个节点
    struct arp_link *next;     	//下一个ARP节点
}ARP_LINK;

ARP_LINK *arp_head=NULL;

//创建插入arp链表节点函数，不用有序
ARP_LINK *arp_link_insert(ARP_LINK *arp_head, ARP_LINK *pnew)
{
	if(arp_head == NULL){ //链表为空
		arp_head = pnew;
		arp_head->next = arp_head;
		arp_head->front = arp_head;
	}else if(arp_head->next == arp_head){ //链表只有一个节点	
		arp_head->next = pnew;
		arp_head->front = pnew;
		pnew->next = arp_head;
		pnew->front = arp_head;
	}else{			//链表有2个及以上的节点
		arp_head->next->front = pnew;
		pnew->next = arp_head->next;
		arp_head->next = pnew;
		pnew->front = arp_head;
	}
	return arp_head;
}








