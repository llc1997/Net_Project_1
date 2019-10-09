#include "main_child.h"

extern void init_route_link(void);

int main_child()
{
	//初始化 配置文件
	init_ip_link();
	//ip_link_print(ip_head);
	//初始化路由表链表
	init_route_link();
	//route_link_print(route_head);
	//获取接口信息
	getinterface();
	//创建键盘处理函数并脱离
	pthread_t KEY_T;
	pthread_create(&KEY_T, NULL,key_pthread, NULL);
	pthread_detach(KEY_T);
	
	//创建原始套接字,接收所有类型的数据包
	raw_sock_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(raw_sock_fd<=0){
		perror("socket");
		_exit(-1);
	}
	//原始套接字数据包大约为1500个字节
	char recv_buff[MY_RECV_SIZE_T]="";
	ssize_t recv_len=0;
	while(1){
		bzero(recv_buff,sizeof(recv_buff));
		recv_len = recvfrom(raw_sock_fd, recv_buff, sizeof(recv_buff), 0, NULL, NULL);
		if(recv_len<=0||recv_len>MY_RECV_SIZE_T){
			perror("recvfrom");
			continue;
		}
		//ARP协议应答包
		if((recv_buff[12]==0x08)&&(recv_buff[13]==0x06)&&(recv_buff[21]==0x02))
		{		//printf("接收到 ARP 应答包&&\n");
			RECV_DATA *recv = (RECV_DATA *)malloc(sizeof(RECV_DATA));
			recv->data_len = recv_len;	//填充结构体的成员data_len为接收数据的长度
			memcpy(recv->data, recv_buff, recv_len);//拷贝数据包到动态地址结构体中
			pthread_t ARP_T;
			pthread_create(&ARP_T, NULL,arp_pthread, (void *)recv);		//创建ARP处理线程，将节点P存入缓存链表ip_link
			pthread_detach(ARP_T);
		}
#if 0		
		//ARP协议请求包
		if((recv_buff[12]==0x08)&&(recv_buff[13]==0x06)&&(recv_buff[21]==0x01)){
				//printf("接收到 ARP 请求包**\n");
			RECV_DATA *recv = (RECV_DATA *)malloc(sizeof(RECV_DATA));
			recv->data_len = recv_len;		//填充结构体的成员data_len为接收数据的长度
			memcpy(recv->data, recv_buff, recv_len);	//拷贝数据包到动态地址结构体中
			arp_request_pthread(recv);
			//pthread_t ARP_R;
			//pthread_create(&ARP_R, NULL,arp_request_pthread, (void*)recv);
			//pthread_detach(ARP_R);
		}
#endif
		//IP协议包
		if((recv_buff[12]==0x08)&&(recv_buff[13]==0x00))
		{		//printf("接收到 IP 数据包&&\n");
				//目的ip过滤和源ip过滤
			//find_iplink_ip查找过滤链表的目的ip，找到不发，进入下一次
			IP_LINK *ip_pb1 = find_iplink_ip(ip_head, (unsigned char*)recv_buff+30); 
			if(ip_pb1!=NULL){
				continue;
			}
			//find_iplink_srcip查找过滤链表的源ip，找到不发，进入下一次
			IP_LINK *ip_pb2 = find_iplink_srcip(ip_head, (unsigned char*)recv_buff+26); 
			if(ip_pb2!=NULL){
				continue;
			}
			RECV_DATA *recv = (RECV_DATA *)malloc(sizeof(RECV_DATA));//申请地址，缓存数据包RECV_DATA（结构体）
			recv->data_len = recv_len;	//填充结构体的成员data_len为接收数据的长度
			memcpy(recv->data, recv_buff, recv_len);//拷贝数据包到动态地址结构体中
			//创建转发数据包处理线程
			pthread_t IP_T;
			pthread_create(&IP_T, NULL,ip_pthread, (void*)recv);	//IP数据包转发线程
			pthread_detach(IP_T);
		}
	}
	return 0;
}