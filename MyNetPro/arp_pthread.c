#include "arp_pthread.h"
#include "arp_link.h"

void *arp_pthread(void *arg)
{
	arp_head = arp_link_insert(arp_head,(ARP_LINK *)arg);
	//printf("ARP应答数据包处理线程,插入一个ARP节点\n");
}