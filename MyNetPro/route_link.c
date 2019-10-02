#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//路由表结构体
typedef struct route_link
{
    unsigned char network[4];    	    //目的网段,4字节
    unsigned char mask[4];    	        //目的网段掩码,4字节
    unsigned char nexthop[4];			//下一跳IP，2字节
    struct route_link *front;			//链表的上一个节点
    struct route_link *next;			//链表的下一个节点
}ROUTE_LINK;

ROUTE_LINK* route_head=NULL;		//过滤链表头节点
ROUTE_LINK* route_link_insert(ROUTE_LINK* route_head, ROUTE_LINK* pnew);
extern char *read_src_file(long unsigned int *file_length,char *src_file_name);
extern int conf_deal(char *conf_src,char *conf_done[],char *str);

void init_route_link(void)
{
    int conf_len;					//保存文件的行数
	unsigned long file_length;		//文件的大小，字节数
	char *conf_buf=NULL;			//保存读取后文件内容的地址
	char *conf_line[20];			//保存每一行的地址，大小最大20个路由表
	ROUTE_LINK *route_new;				//节点指针	
	int k;						//循环控制变量
	//char *str_temp;					//临时指针变量
	//打开并读取配置文件
	conf_buf = read_src_file(&file_length,"config/route_tab");
	//切割文件内容，切割一行为单位
	conf_len = conf_deal(conf_buf,conf_line,"\r\n");
	//循环处理每一行
	printf("conf_len=%d\n",conf_len);
	for(k=0;k<conf_len;k++)
	{
		//if(conf_line[k] == NULL)
		//	break;
		//str_temp = conf_line[k];
		//i=0; j=0;
		char *route_row[4] = {NULL,NULL,NULL,NULL};
		//切割列，“,+”为单位
		conf_deal(conf_buf,conf_line,",+");
		route_new = (ROUTE_LINK *)malloc(sizeof(ROUTE_LINK));
		bzero(route_new,sizeof(ROUTE_LINK));
		//对每一列的内容保存入链表节点route_new
		if(route_row[0] != NULL)
			memcpy(route_new->network,route_row[0],4);	
		if(route_row[1] != NULL)
			memcpy(route_new->mask,route_row[1],4);
		if(route_row[2] != NULL)
			memcpy(route_new->nexthop,route_row[2],2);
		//if(route_row[3] != NULL)
		//	memcpy(route_new->f_agree,route_row[3],strlen(route_row[3]));
		//插入链表
		route_head = route_link_insert(route_head,route_new);
	}
}

//带创建的插入
ROUTE_LINK* route_link_insert(ROUTE_LINK* route_head, ROUTE_LINK* pnew)
{
	printf("*****route_link节点插入*******\n");
	if(route_head == NULL){ //链表为空
		route_head = pnew;
		route_head->next = route_head;
		route_head->front = route_head;
	}else if(route_head->next == route_head){ //链表只有一个节点	
		route_head->next = pnew;
		route_head->front = pnew;
		pnew->next = route_head;
		pnew->front = route_head;
	}else{			//链表有2个及以上的节点
		route_head->next->front = pnew;
		pnew->next = route_head->next;
		route_head->next = pnew;
		pnew->front = route_head;
	}
	return route_head;
}

//功能：查找路由表网段，在route_head所指向的链表,recv_ip是目标网段
ROUTE_LINK *find_routelink_ip(ROUTE_LINK *route_head, unsigned char * recv_ip)
{
    ROUTE_LINK *ip_find = route_head;
	if(ip_find == NULL)
		return NULL;
	while(ip_find->next != route_head){
		if(strncmp((char *)ip_find->network,(char *)recv_ip,4) == 0)
			return ip_find;
		ip_find = ip_find->next;		
	}	
	if(strncmp((char *)ip_find->network,(char *)recv_ip,4) == 0)
		return ip_find;	
	else
		return ip_find;
}

