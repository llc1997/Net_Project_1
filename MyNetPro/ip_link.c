#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>

//过滤链表结构体
typedef struct ip_link
{
    unsigned char f_dest_ip[4];    	//过滤目的Ip,4字节
    unsigned char f_src_ip[4];    	//过滤源Ip,4字节
    unsigned char f_port[2];		//过滤的port，2字节
    char f_agree[2];				//过滤的协议，2字节,协议代码
    struct ip_link *front;			//链表的上一个节点
    struct ip_link *next;			//链表的下一个节点
}IP_LINK;

IP_LINK *ip_head=NULL;		//过滤链表头节点

IP_LINK* ip_link_insert(IP_LINK *ip_head, IP_LINK *pnew);
char *read_src_file(long unsigned int *file_length,char *src_file_name);
int conf_deal(char *conf_src,char *conf_done[],char *str);
IP_LINK *find_iplink_ip(IP_LINK *ip_head, unsigned char * recv_ip);

//1、读取配置的过滤文件./config/filter_conf//循环读取，每次一行，一行的内容填充到一个链表节点中
//2、切割读取的一行字符串，",+"为间隔符
//3、填充链表节点
//4、插入链表，带创建的插入
//ip过滤表初始化函数
void init_ip_link(void)
{			
	int conf_len;					//保存文件的行数
	unsigned long file_length;		//文件的大小，字节数
	char *conf_buf=NULL;			//保存读取后文件内容的地址
	char *conf_line[100];			//保存每一行的地址，大小？********
	IP_LINK *ip_new;				//节点指针	
	int k;							//循环控制变量
	//打开并读取配置文件
	conf_buf = read_src_file(&file_length,"config/filter_conf");
	//切割文件内容，切割一行为单位
	conf_len = conf_deal(conf_buf,conf_line,"\r\n");
	//循环处理每一行
	for(k=0;k<conf_len;k++)
	{
		char *conf_row[4] = {NULL,NULL,NULL,NULL};
		//切割列，“,+”为单位
		//printf("列切割前：%s\n",conf_line[k]);
		conf_deal(conf_line[k],conf_row,",+");
		ip_new = (IP_LINK *)malloc(sizeof(IP_LINK));
		//printf("列切割后：%s，%s，%s，%s\n",conf_row[0],conf_row[1],conf_row[2],conf_row[3]);
		bzero(ip_new,sizeof(IP_LINK));
		//对每一列的内容保存入链表节点ip_new
		if(conf_row[0] != NULL){
			if(conf_row[0][0] != ' '){
				//点分十进制转换
				inet_pton(AF_INET,conf_row[0],(void *)ip_new->f_dest_ip);
			}
		}
		if(conf_row[1] != NULL){
			if(conf_row[1][0] != ' '){
				inet_pton(AF_INET,conf_row[1],(void *)ip_new->f_src_ip);
			}
		}
		if(conf_row[2] != NULL){
			if(conf_row[2][0] != ' '){
				//端口大小端转换
				memcpy((char *)&(ip_new->f_port),conf_row[2],2);
			}
		}
		if(conf_row[3] != NULL){
			if(conf_row[3][0] != ' '){
				//协议大小端转换
				memcpy(ip_new->f_agree,conf_row[3],strlen(conf_row[3]));
			}
		}
		//插入链表
		ip_head = ip_link_insert(ip_head,ip_new);
	}
}

//带创建的插入
IP_LINK* ip_link_insert(IP_LINK *ip_head, IP_LINK *pnew)
{
	//printf("*****ip_link节点插入*******\n");
	if(ip_head == NULL){ //链表为空
		ip_head = pnew;
		ip_head->next = ip_head;
		ip_head->front = ip_head;
	}else if(ip_head->next == ip_head){ //链表只有一个节点	
		ip_head->next = pnew;
		ip_head->front = pnew;
		pnew->next = ip_head;
		pnew->front = ip_head;
	}else{			//链表有2个及以上的节点
		ip_head->next->front = pnew;
		pnew->next = ip_head->next;
		ip_head->next = pnew;
		pnew->front = ip_head;
	}
	return ip_head;
}

/**************************************************************************/
//函数功能:读出文件内容
//参数：file_length:整型指针，此地址中保存文件字节数。 
// src_file_name:文件名字，从此文件中读取内容。 
// 返回值:读出字符串的首地址
// 在此函数中测文件的大小，并malloc空间，再把文件内容读出返回，读出字符数组的首地址
/**************************************************************************/
char *read_src_file(long unsigned int *file_length,char *src_file_name)
{
	FILE *fp;//文件指针，指向打开的文件
	char *src_file_text;
	int read_sign;
	fp = fopen(src_file_name,"r");//fp 指向src_file_name，是原文件
	if(fp == NULL){
		printf("文件打开错误！");
		return NULL;
	}
	fseek(fp,0L,SEEK_END);
	*file_length = ftell(fp);
	rewind(fp);
	//printf("file_length=%ld！\n",*file_length);
	src_file_text = (char *)malloc(*file_length);
	if(src_file_text==NULL){
		printf("内存申请失败！");
		return NULL;
	}
	read_sign = fread(src_file_text,*file_length,1,fp);
	if(read_sign <= 0){
		printf("文件读入失败！");
		return NULL;
	}
	fclose(fp);
	return src_file_text;
}

//函数功能:切割字符串，“\r\n”为间隔，
//参数：conf_src:要切割的配置串,conf_done:切割后保存的地址，str:切割的标志
//返回值：
int conf_deal(char *conf_src,char *conf_done[],char *str)
{
	int i=0;
	conf_done[i]=strtok(conf_src,str);
	while(conf_done[i]!=NULL)
	{
		i++;
		conf_done[i]=strtok(NULL,str);
	}
	return i;
}

//功能：查找过滤表目的IP的函数，在ip_head所指向的链表
IP_LINK *find_iplink_ip(IP_LINK *ip_head, unsigned char * recv_ip)
{
    IP_LINK *ip_find = ip_head;
	if(ip_find == NULL)
		return NULL;
	while(ip_find->next != ip_head){
		if(memcmp((char *)ip_find->f_dest_ip,(char *)recv_ip,4) == 0)
			return ip_find;
		ip_find = ip_find->next;		
	}	
	if(memcmp((char *)ip_find->f_dest_ip,(char *)recv_ip,4) == 0)
		return ip_find;	
	else
		return NULL;
}

//功能：查找过滤表源IP的函数，在ip_head所指向的链表
IP_LINK *find_iplink_srcip(IP_LINK *ip_head, unsigned char * recv_ip)
{
    IP_LINK *ip_find = ip_head;
	if(ip_find == NULL)
		return NULL;
	while(ip_find->next != ip_head){
		if(memcmp((char *)ip_find->f_src_ip,(char *)recv_ip,4) == 0)
			return ip_find;
		ip_find = ip_find->next;		
	}	
	if(memcmp((char *)ip_find->f_src_ip,(char *)recv_ip,4) == 0)
		return ip_find;	
	else
		return NULL;
}

//ip过滤链表节点打印
void ip_link_print(IP_LINK *ip_head)
{
	IP_LINK *pb = ip_head;
	if(ip_head == NULL)
		return;
	printf("*****过滤表*****\n目的IP\t\t源IP\t\t端口\t协议\n");
	while(pb->next != ip_head){
		printf("%d.%d.%d.%d\t%d.%d.%d.%d\t\t%02x.%02x\t%02x.%02x\n",
			pb->f_dest_ip[0],pb->f_dest_ip[1],pb->f_dest_ip[2],pb->f_dest_ip[3],
			pb->f_src_ip[0],pb->f_src_ip[1],pb->f_src_ip[2],pb->f_src_ip[3],
			pb->f_port[0],pb->f_port[1],
			pb->f_agree[0],pb->f_agree[1]);
		pb = pb->next;		
	}	
	printf("%d.%d.%d.%d\t%d.%d.%d.%d\t\t%02x.%02x\t%02x.%02x\n",
			pb->f_dest_ip[0],pb->f_dest_ip[1],pb->f_dest_ip[2],pb->f_dest_ip[3],
			pb->f_src_ip[0],pb->f_src_ip[1],pb->f_src_ip[2],pb->f_src_ip[3],
			pb->f_port[0],pb->f_port[1],
			pb->f_agree[0],pb->f_agree[1]);
}

#if 0
//删除IP链表节点（基本不用）
IP_LINK *delete_ip(IP_LINK *ip_head,char ip_del[4])
{
	IP_LINK *p = ip_head,*pb;
	if(head == NULL)
		return NULL;
	while(p->next != head){
		if(ip_del[0] == p->ip){//找出要删除的节点   
			if(p == head)//被删除的是头节点
				ip_head = ip_head->next;		
			break;
		}
		pb = p;
		p = p->next;		
	}
	if(p->m_time == m_time){//头、中间、结尾都需要交换
		p->next->front = p->front;
		p->front->next = p->next;	
		free(p);
		if(p == ip_head)//只有一个节点的情况
			return NULL;
		else
			return ip_head;
	}
	return ip_head;
}
#endif
