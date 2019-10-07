#ifndef ARP_PTHREAD_H
#define ARP_PTHREAD_H

<<<<<<< HEAD
#include <unistd.h>
#include <stdio.h>

typedef struct recv_data
{
    size_t data_len;    //数据长度
    char data[2048];    //数据内容，需修改优化
}RECV_DATA;

extern void *arp_pthread(void *arg);
extern void arp_request_pthread(RECV_DATA* recv);
=======
extern void *arp_pthread(void *arg);
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab

#endif