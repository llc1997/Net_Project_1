#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct arp_link
{
    unsigned char arp_ip[4];         	//ip地址
    unsigned char arp_mac[6];        	//mac地址
    struct arp_link *front;		//上一个节点
    struct arp_link *next;     	//下一个ARP节点
}ARP_LINK;

ARP_LINK *arp_head=NULL;

//创建插入arp链表节点函数，不用有序
ARP_LINK *arp_link_insert(ARP_LINK *arp_head, ARP_LINK *pnew)
{
	printf("*****arp应答包入表*******\n");
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

//arp链表节点打印
void arp_link_print(ARP_LINK *arp_head)
{
	ARP_LINK *pb = arp_head;
	if(arp_head == NULL)
		return;
	printf("****ARP表****\nip\t\tmac\n");
	while(pb->next != arp_head){
		printf("%d.%d.%d.%d\t%02x.%02x.%02x.%02x.%02x.%02x\n",
			pb->arp_ip[0],pb->arp_ip[1],pb->arp_ip[2],pb->arp_ip[3],
			pb->arp_mac[0],pb->arp_mac[1],pb->arp_mac[2],
			pb->arp_mac[3],pb->arp_mac[4],pb->arp_mac[5]);
		pb = pb->next;		
	}	
	printf("%d.%d.%d.%d\t%02x.%02x.%02x.%02x.%02x.%02x\n",
			pb->arp_ip[0],pb->arp_ip[1],pb->arp_ip[2],pb->arp_ip[3],
			pb->arp_mac[0],pb->arp_mac[1],pb->arp_mac[2],
			pb->arp_mac[3],pb->arp_mac[4],pb->arp_mac[5]);
}

//功能：ARP链表是否有对应IP,即查询
//参数：ARP_LINK *arp_head：arp表头指针；unsigned char *recvip：目标IP
//返回值：有返回arp表节点指针，无返回NULL
ARP_LINK* arp_link_seek(ARP_LINK *arp_head, unsigned char *recv_ip)
{
	ARP_LINK *arp_find = arp_head;
	if(arp_find == NULL)
		return NULL;
	while(arp_find->next != arp_head)	//未到尾节点
	{
		if(strncmp((char *)arp_find->arp_ip,(char *)recv_ip,4) == 0)
			return arp_find;
		arp_find = arp_find->next;
	}
	if(strncmp((char *)arp_find->arp_ip,(char *)recv_ip,4) == 0)
		return arp_find; 
	else
		return NULL;
}


