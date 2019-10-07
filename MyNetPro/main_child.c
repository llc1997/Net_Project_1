#include "main_child.h"

<<<<<<< HEAD
extern void init_route_link(void);

=======
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab
/******************************************************************
函	数:	int main(int argc, char *argv[])
功	能:	主函数
参	数:	无
返回值: 无
*******************************************************************/
int main_child(){
<<<<<<< HEAD
		//printf("main_child初始化Ip链表\n");
	//初始化 配置文件
// **********文件：ip_link************************
	init_ip_link();
	ip_link_print(ip_head);

	//初始化路由表链表
// **********文件：route_link************************
	init_route_link();
	route_link_print(route_head);
=======
	printf("main_child初始化Ip链表\n");
	//初始化 配置文件
// **********文件：ip_link************************
	init_ip_link();
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab
	
	//获取接口信息
	//getinterface()填充了数组：net_interface[MAXINTERFACES];接口数据结构体数组
// **********文件：get_interface************************
	getinterface();
<<<<<<< HEAD
	//printf("getinterface初始化接口数据\n");
=======
	printf("getinterface初始化接口数据\n");
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab
	//创建键盘处理函数并脱离
	pthread_t KEY_T;
// **********文件：key_pthread************************
	pthread_create(&KEY_T, NULL,key_pthread, NULL);
	pthread_detach(KEY_T);
	
	//创建原始套接字,接收所有类型的数据包
	raw_sock_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(raw_sock_fd<=0){
		perror("socket");
		_exit(-1);
	}
	char recv_buff[MY_RECV_SIZE_T]="";//原始套接字数据包大约为1500个字节
	ssize_t recv_len=0;
	while(1){
		bzero(recv_buff,sizeof(recv_buff));
		recv_len = recvfrom(raw_sock_fd, recv_buff, sizeof(recv_buff), 0, NULL, NULL);
		if(recv_len<=0||recv_len>MY_RECV_SIZE_T){
			perror("recvfrom");
			continue;
		}
<<<<<<< HEAD
		//ARP请求包未做处理
		if((recv_buff[12]==0x08)&&(recv_buff[13]==0x06)&&(recv_buff[20]==0x02)){//ARP协议应答包
			printf("接收到 ARP 应答包\n");
=======
		if((recv_buff[12]==0x08)&&(recv_buff[13]==0x06)){//ARP协议应答包
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab
			ARP_LINK *p = (ARP_LINK *)malloc(sizeof(ARP_LINK));//arp_link链表声明定义，申请一个节点的空间
			if(p==NULL){
				perror("malloc");
				continue;
			}
			memcpy(p->arp_mac, recv_buff+22, 6);//mac，填充amc
			memcpy(p->arp_ip , recv_buff+28, 4);//ip ，填充ip
			//printf("%d.%d.%d.%d-->",p->ip[0],p->ip[1],p->ip[2],p->ip[3]);
			pthread_t ARP_T;
// **********文件：arp_pthread************************
			pthread_create(&ARP_T, NULL,arp_pthread, (void*)p);		//创建ARP处理线程，将节点P存入缓存链表ip_link
			pthread_detach(ARP_T);
		}
<<<<<<< HEAD
		if((recv_buff[12]==0x08)&&(recv_buff[13]==0x06)&&(recv_buff[20]==0x01)){//ARP协议请求包
			printf("接收到 ARP 请求包**\n");
			RECV_DATA *recv = (RECV_DATA *)malloc(sizeof(RECV_DATA));
			recv->data_len = recv_len;		//填充结构体的成员data_len为接收数据的长度
			memcpy(recv->data, recv_buff, recv_len);	//拷贝数据包到动态地址结构体中
			arp_request_pthread(recv);
			//pthread_t ARP_R;
			//pthread_create(&ARP_R, NULL,arp_request_pthread, (void*)recv);
			//pthread_detach(ARP_R);
		}
		if((recv_buff[12]==0x08)&&(recv_buff[13]==0x00)){//IP协议包
				//printf("接收到 IP 数据包&&\n");
			//目的ip过滤
			IP_LINK *ip_pb = find_iplink_ip(ip_head, (unsigned char*)recv_buff+30); //find_ip查找过滤链表，找到不发，进入下一次
=======
		if((recv_buff[12]==0x08)&&(recv_buff[13]==0x00)){//IP协议包
			//目的ip过滤
			IP_LINK *ip_pb = find_ip(ip_head, (unsigned char*)recv_buff+30); //find_ip查找过滤链表，找到不发，进入下一次
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab
// **********函数：find_ip************************
			if(ip_pb!=NULL){
				continue;
			}
			RECV_DATA *recv = (RECV_DATA *)malloc(sizeof(RECV_DATA));	//申请地址，缓存数据包RECV_DATA（结构体）
			recv->data_len = recv_len;		//填充结构体的成员data_len为接收数据的长度
			memcpy(recv->data, recv_buff, recv_len);	//拷贝数据包到动态地址结构体中
			//创建转发数据包处理线程
			pthread_t IP_T;
// **********文件：ip_pthread************************
			pthread_create(&IP_T, NULL,ip_pthread, (void*)recv);	//IP数据包转发线程
			pthread_detach(IP_T);
		}
	}
	
	return 0;
}