#include <unistd.h>
#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 

using namespace std;

int main(int argc,char **argv)
{
	int pipe1[2],pipe2[2];
	char pstr[]="parent data";
	char cstr[]="child data";
	char buf[100];
	
	if(pipe(pipe1)<0||pipe(pipe2)<0)
		cout<<"pipe error"<<endl;
	pid_t pid=fork();
	if(pid>0)
	{   
        cout<<"parent process:ppid="<<getppid()<<",pid="<<getpid()<<endl;
		//父进程,用管道1写数据,管道2读数据
		close(pipe1[0]);//关闭pipe1读端口
		close(pipe2[1]);//关闭pipe2写端口
		write(pipe1[1],pstr,sizeof(pstr));
		if(read(pipe2[0],buf,100)>0)
			cout<<"parent received:"<<buf<<endl;
	}	
	else if(pid==0)
	{   
		cout<<"child process ppid="<<getppid()<<",pid="<<getpid()<<endl;
		//父进程用管道1读数据,管道2写数据
		close(pipe1[1]);//关闭pipe1写端口
		close(pipe2[0]);//关闭pipe2读端口
		if(read(pipe1[0],buf,100)>0)
			cout<<"child received:"<<buf<<endl;
		write(pipe2[1],cstr,sizeof(cstr));
		exit(0);
	}
	else
		cout<<"fork error"<<endl;
	return 0;
}

