#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *buf);

int main(int argc,char *argv[])
{
	int sock;
	char message[BUF_SIZE];
	char messageout[BUF_SIZE];
	int str_len,fd_max,fd_num,i;
	struct sockaddr_in serv_adr;
	fd_set reads,cpy_reads;
	struct timeval timeout;

	if(argc!=3){
		printf("Usage: %s <IP><port>\n",argv[0]);
		exit(1);
	}	
	sock=socket(PF_INET,SOCK_STREAM,0);
	if(sock==-1)
		error_handling("socket() error");

	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		printf("connected......\n");
	
        FD_ZERO(&reads);
	FD_SET(sock,&reads);
	FD_SET(0,&reads);
	fd_max=sock;

	while(1)
	{

		cpy_reads=reads;
		timeout.tv_sec=5;
		timeout.tv_usec=5000;
		
		if((fd_num=select(fd_max+1,&cpy_reads,0,0,NULL))==-1)
			{	printf("select error()");
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
			if(FD_ISSET(0,&cpy_reads))
			{
					if(!strcmp(message,"q\n")||!strcmp(message,"Q\n"))
						break;
					str_len=read(0,messageout,BUF_SIZE-1);
					messageout[str_len]=0;
					write(sock,messageout,strlen(messageout));
				
			}

		/*fputs("input message(Q to quit):", stdout);
		fgets(message,BUF_SIZE,stdin);

		if(!strcmp(message,"q\n")||!strcmp(message,"Q\n"))
			break;
		write(sock,message,strlen(message));
		str_len=read(sock, message, BUF_SIZE-1);
		message[str_len]=0;
		printf("from server: %s",message);*/
				
	}

	close(sock);
	return 0;
}

void error_handling(char *buf)
{
	fputs(buf,stderr);
	fputc('\n',stderr);
	exit(1);
}

				

