#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<sys/select.h>

#define BUF_SIZE 300
void error_handling(char *buf);

int main(int argc,char *argv[])
{
	char waitstring[]="wait";
	int serv_sock,clnt_sock;
	struct sockaddr_in serv_adr,clnt_adr;
	struct timeval timeout;
	fd_set reads,cpy_reads;
	int p1fd=0;
	int p2fd=0;
	socklen_t adr_sz;
	int fd_max,str_len,fd_num,i;
	char buf[BUF_SIZE];
	if(argc!=2){
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}
	
	serv_sock=socket(PF_INET,SOCK_STREAM,0);
	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);//inet_addr(serv_ip)
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock,5)==-1)
		error_handling("listen() error");

	FD_ZERO(&reads);
	FD_SET(serv_sock,&reads);
	fd_max=serv_sock;

	while(1)
	{
		cpy_reads=reads;
		timeout.tv_sec=5;
		timeout.tv_usec=5000;

		if((fd_num=select(fd_max+1,&cpy_reads,0,0,NULL))==-1)
			break;
		if(fd_num==0)
		{	
			printf("Time out!!");
			continue;
		}

		for(i=0;i<fd_max+1;i++)
		{
			if(FD_ISSET(i,&cpy_reads))
			{
				if(i==serv_sock)
				{
					adr_sz=sizeof(clnt_adr);
					clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&adr_sz);
					FD_SET(clnt_sock,&reads);
					if(fd_max<clnt_sock)
						fd_max=clnt_sock;
					printf("connected clinet: %d \n",clnt_sock);
					if(p1fd>0)
						p2fd=clnt_sock;
					else
						p1fd=clnt_sock;
				}
				else if(p1fd==0||p2fd==0)
				{
					write(p1fd,waitstring,sizeof(waitstring));
				}
				else if(i==p1fd)
				{
					str_len=read(i,buf,BUF_SIZE);
					if(str_len==0)
					{
						FD_CLR(i,&reads);
						close(i);
						printf("closed client: %d\n",i);
					}
					else
					{ 
						//printf("client 1:%s",buf);
						write(p2fd,buf,str_len);
					}
				}
				else if(i==p2fd)
				{
					str_len=read(i,buf,BUF_SIZE);
					if(str_len==0)
					{
						FD_CLR(i,&reads);
						close(i);
						printf("closed client: %d\n",i);
					}
					else
					{ 
						//printf("client 1:%s",buf);
						write(p1fd,buf,str_len);
					}	

				}
			}
		}
	}

	close(serv_sock);
	return 0;
}

void error_handling(char *buf)
{
	fputs(buf,stderr);
	fputc('\n',stderr);
	exit(1);
}

				

