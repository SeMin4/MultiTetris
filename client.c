#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <pthread.h>
#include <ncursesw/curses.h>

#define BUF_SIZE 1024
int otherBoard[20][12];

//void error_handling(char *buf);

void client(char *host, int portnum)
{
	int sock;
	char message[BUF_SIZE];
	char messageout[BUF_SIZE];
	int str_len,fd_max,fd_num,i;
	struct sockaddr_in serv_adr;
	fd_set reads,cpy_reads;
	struct timeval timeout;
	pthread_t thread;

	
	sock=socket(PF_INET,SOCK_STREAM,0);
	if(sock==-1){
		mvprintw(60,70,"error_socket\n");
		refresh();
	}

	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(host);
	serv_adr.sin_port=htons(portnum);
	
	if(connect(sock, (struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1){
		mvprintw(60,70,"error_connect.\n");
		refresh();

	}
	else{
		mvprintw(60,70,"connected......\n");
		refresh();
	}
}	
/*	FD_ZERO(&reads);
	FD_SET(sock,&reads);
	//FD_SET(0,&reads);
	fd_max=sock;
	pthread_create(&thread,NULL,server_read,NULL);
	close(sock);
	return 0;
}
void server_read()
{
	while(1)
	{
		cpy_reads=reads;
		timeout.tv_sec=5;
		timeout.tv_usec=5000;
		
		if((fd_num=select(fd_max+1,&cpy_reads,0,0,NULL))==-1)
		{	
			printf("select error()");
			break;
		}		

		if(fd_num==0)
		{	
			printf("Time out!!\n");
			continue;
		}
			if(FD_ISSET(sock,&cpy_reads))
			{
				str_len=read(sock, message, BUF_SIZE-1);
				message[str_len]=0;
				printf("from server: %s",message);
			}
			/*if(FD_ISSET(0,&cpy_reads))//이거를 빼고 tetris.c파일에 보내기 할때 보낸다.
			{
					
					str_len=read(0,messageout,BUF_SIZE-1);
					messageout[str_len]=0;
					write(sock,messageout,strlen(messageout));
				
			}*/

		/*fputs("input message(Q to quit):", stdout);
		fgets(message,BUF_SIZE,stdin);

		if(!strcmp(message,"q\n")||!strcmp(message,"Q\n"))
			break;
		write(sock,message,strlen(message));
		str_len=read(sock, message, BUF_SIZE-1);
		message[str_len]=0;
		printf("from server: %s",message);*/
				
/*	}
}
void send(int b[][])
{
	//여기서 우리 함수를 구현한다.
}
void error_handling(char *buf)
{
	fputs(buf,stderr);
	fputc('\n',stderr);
	exit(1);
}
*/