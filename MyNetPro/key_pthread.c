#include <pthread.h>
#include <termios.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "ip_link.h"
#include "arp_link.h"
#include "route_link.h"

int fun_ip_write(char *src_file_name,unsigned char *str,int str_len);
int input_ip_isright(char ip_str[16]);
void fun_restart();
extern int fd_fifo;

//函数功能:无回显输入字符
char mygetch()
{
	struct termios oldt, newt;
	char ch;
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
	return ch;
}

//打印帮助提示信息
void help(void)
{
    printf("======================================\n");
    printf("=== 设置路由器请输入对应的编号     ===\n");
    printf("=== 1、set filter dest ip   ==========\n");
    printf("=== 2、set filter src ip    ==========\n");
    printf("=== 3、set filter port      ==========\n");
    printf("=== 4、set filter agree     ==========\n");
	printf("=== 5、look the filter_link ==========\n");
	printf("=== 6、look the arp_link    ==========\n");
	printf("=== 7、look the route_link  ==========\n");
	printf("=== 8、show help            ==========\n");
    printf("======================================\n");
}

void *key_pthread(void *arg)
{
    help();
    while (1)
    {
        char option = mygetch();
		char dest_ip[16]="";
		char src_ip[16]="";
        switch (option)
        {
        case '1':	//printf("选择了操作1\n");
            printf("请输入需要过滤的目的ip：");
            scanf("%s",dest_ip);
			if(input_ip_isright(dest_ip) ==0){	//输入不合法，直接结束
				printf("输入数据不合法，请重新选择操作并输入！\n");
				break;
			}else{//写入文件
				char str[64];
				int len = sprintf((char *)str,"\n%s,+",dest_ip);
				if(fun_ip_write("config/filter_conf",(unsigned char *)str,len) == -1)
				{
					printf("配置信息写入错误，请重新选择操作并输入！\n");
				}else{
					fun_restart();
				}
			}
            break;
        case '2':	//printf("选择了操作2\n");
            printf("请输入需要过滤的目的ip：");
            scanf("%s",src_ip);
			if(input_ip_isright(dest_ip) == 0){	//输入不合法，直接结束
				printf("输入数据不合法，请重新选择操作并输入！\n");
				break;
			}else{//写入文件
				char str[64];
				int len = sprintf(str,"\n ,+%s,+",src_ip);
				if(fun_ip_write("config/filter_conf",(unsigned char *)str,len) == -1)
				{
					printf("配置信息写入错误，请重新选择操作并输入！\n");
				}else{
					fun_restart();
				}
			}
            break;
        case '3':	//printf("选择了操作3\n");
			printf("该功能待升级！\n");
            break;
        case '4':	//printf("选择了操作4\n");
			printf("该功能待升级！\n");
            break;
        case '5':	//printf("选择了操作5\n");
			ip_link_print(ip_head);			//显示过滤表
            break;
		case '6':	//printf("选择了操作6\n");
			arp_link_print(arp_head);		//显示arp表
            break;
		case '7':	//printf("选择了操作7\n");
			route_link_print(route_head);	//显示路由表
            break;
		case '8':	//printf("选择了操作8\n");
			help();							//显示帮助信息
            break;
        default:
            break;
        }
    }
}

//输入的IP合法性判断函数
int input_ip_isright(char ip_str[16])
{
	if(strlen(ip_str) <= 15 && strlen(ip_str) >=7)
		return 1;
	else 
		return 0;
}

//写入文件中
int fun_ip_write(char *src_file_name,unsigned char *str,int str_len)
{
	int fd;//文件指针，指向打开的文件
	if((fd = open(src_file_name,O_WRONLY | O_CREAT | O_APPEND, 0664)) < 0)//fp 指向src_file_name，是原文件
	{
		perror("文件打开错误！");
		return -1;
	}
	if(write(fd,str,str_len) == -1)
    {
        perror("fail to write");
        return -1;
    }
	close(fd);
	return 0;
}

//配置文件更新，重启子进程
void fun_restart()
{
	if(write(fd_fifo, "root_restart", 12) == -1)
    {
        perror("fail to write");
        return;
    }
}






