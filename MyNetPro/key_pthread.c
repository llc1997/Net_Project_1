#include "key_pthread.h"
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
    printf("========================\n");
    printf("== 1、setip ============\n");
    printf("========================\n");
    printf("========================\n");
    printf("========================\n");
    printf("========================\n");
}

void *key_pthread(void *arg)
{
    printf("创建键盘处理函数并脱离\n");
    help();
    while (1)
    {
        char option = mygetch();
        switch (option)
        {
        case '1':printf("选择了操作1\n");
            break;
        default:
            break;
        }
    }
}





