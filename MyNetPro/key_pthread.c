<<<<<<< HEAD
#include <pthread.h>
#include <termios.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


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
    printf("=== 如需设置路由器请输入对应的编号 ===\n");
    printf("=== 1、set filter dest ip ============\n");
    printf("=== 2、set filter src ip  ============\n");
    printf("=== 3、set filter port    ============\n");
    printf("=== 4、set filter agree   ============\n");
    printf("======================================\n");
}

void *key_pthread(void *arg)
{
    //printf("创建键盘处理函数并脱离\n");
=======
#include "key_pthread.h"

void *key_pthread(void *arg)
{
    printf("创建键盘处理函数并脱离\n");
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab
    help();
    while (1)
    {
        char option = mygetch();
        switch (option)
        {
        case '1':printf("选择了操作1\n");
<<<<<<< HEAD
            char dest_ip[16];
            printf("请输入需要过滤的目的ip：");
            scanf("%s",dest_ip);
            break;
        case '2':printf("选择了操作2\n");
            break;
        case '3':printf("选择了操作3\n");
            break;
        case '4':printf("选择了操作4\n");
            break;
        case '5':printf("选择了操作5\n");
=======
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab
            break;
        default:
            break;
        }
    }
}

<<<<<<< HEAD
//写入文件中
void fun()
{

}

//





=======
//打印帮助提示信息
void help(void)
{
    printf("========================\n");
    printf("== 1、setip ============\n");
    printf("========================\n");
    printf("========================\n");
    printf("========================\n");
    printf("========================\n");
}

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
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab

