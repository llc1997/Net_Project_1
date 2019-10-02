#ifndef ARP_PTHREAD_H
#define ARP_PTHREAD_H

#include <unistd.h>
#include <stdio.h>

typedef struct recv_data
{
    size_t data_len;    //数据长度
    char data[2048];    //数据内容，需修改优化
}RECV_DATA;

extern void *arp_pthread(void *arg);
extern void arp_request_pthread(RECV_DATA* recv);

#endif