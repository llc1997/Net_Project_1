/* ************************************************************************
 *       Filename:  main.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2019年09月28日 12时32分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <string.h>


extern int main_child();
int fd_fifo;

int main(int argc, char *argv[])
{
	 //创建一个有名管道，使用mkfifo函数
    if(mkfifo("mynetfifo", 0664) == -1)
    {
        //如果有名管道文件已经存在，没有必要打印错误信息，后期直接用就可以
        if(errno != EEXIST)
        {
            perror("fail to mkfifo");
            exit(1);
        }
    }
	//打开管道
	if((fd_fifo = open("mynetfifo", O_RDWR)) == -1)
    {
        perror("fail to open");
        exit(1);
    }
	while(1)
	{
		pid_t pid;
		if((pid = fork()) < 0)
		{
			perror("fail main to fork");
			continue;
		}
		if(pid > 0) //父进程，监视判断子进程运行状态
		{
			while(1) //循环读管道
			{
				char fifo_buf[128] = "";
    			if(read(fd_fifo, fifo_buf, sizeof(fifo_buf)) == -1)
				{	//接收管道内容失败
    				continue;
    			}else
    			{	//接收成功，处理后结束子进程，父进程进入下一次循环
					if(memcmp(fifo_buf,"root_restart",12) == 0)
					{	//得到重启的指令
						if(kill(pid,SIGINT) == 0)
							break;
					}
    			}
			}
		}
		else if(pid == 0) //子进程，运行路由器代码
		{
			main_child();
			exit(0);	//程序正常结束，退出子进程
		}
	}
	while(1);
	return 0;
}



