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

extern int main_child();
//"${workspaceFolder}/**",


int main(int argc, char *argv[])
{
	pid_t pid;
	if((pid = fork()) < 0)
	{
		perror("fail main to fork");
		exit(-1);
	}
	if(pid > 0) //父进程，监视判断子进程运行状态
	{
<<<<<<< HEAD
		//printf("父进程启动\n");
=======
		printf("父进程启动\n");
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab
		while(1)
		{
			;//待完善（在线升级模块预留接口）
		}
	}
	else if(pid == 0) //子进程，运行路由器代码
	{
<<<<<<< HEAD
		//printf("子进程启动\n");
=======
		printf("子进程启动\n");
>>>>>>> 5ef0503a1cde32bc6f28def86654aaa7e72b40ab
		main_child();
	}
	while(1);
	return 0;
}



