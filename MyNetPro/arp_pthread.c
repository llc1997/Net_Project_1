#include "arp_pthread.h"
#include "arp_link.h"
#include <unistd.h>

void *arp_pthread(void *arg)
{
	//unsigned char recv_arp_ip[4];
	//unsigned char recv_arp_mac[6];
	ARP_LINK *ret_arp_seek = arp_link_seek(arp_head,((ARP_LINK *)arg)->arp_ip);
	if(ret_arp_seek == NULL)		//没有对应的IP，直接插入arp表
		arp_head = arp_link_insert(arp_head,(ARP_LINK *)arg);
	else if(strncmp(ret_arp_seek->arp_mac,((ARP_LINK *)arg)->arp_mac,6) != 0)
	{	//有对应的IP，且mac不一致
		arp_head = arp_link_insert(arp_head,(ARP_LINK *)arg);
	}
	//printf("ARP应答数据包处理线程,插入一个ARP节点\n");
}