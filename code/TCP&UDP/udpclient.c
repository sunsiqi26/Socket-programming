/**********************************************************\
*                                                         *\
*  program name:tcpclient.c                               *
*  Author:Liang gang & Hu Xiao-qin                        *\
*  Funtion:This program is just a demostrate program      *\
*          to tell students how to use socket to          *\
*          make a tcp communation.                        *\
*  Date: 03-06-2009                                       *\
*                                                         *\
***********************************************************/

//udpclient.c
#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT 9900

void print_usage(char * cmd)
{
	fprintf(stderr," %s usage:\n",cmd);
	fprintf(stderr,"%s IP_Addr [port]\n",cmd);

}


int main(int argc,char** argv)
{
	struct sockaddr_in server;
	int ret;
	int len;
	int size;
	int port;
	int sockfd;
	int sendnum;
	int recvnum;
	char send_buf[2048];
	char recv_buf[2048];

	memset(send_buf,0,2048);
	memset(recv_buf,0,2048);

	if ((2>argc)|| (argc >3))
	{
		print_usage(argv[0]);
		exit(1);

	}

       if (3==argc) 
       {
		port = atoi(argv[2]);
       }

    	if (-1==(sockfd=socket(AF_INET,SOCK_DGRAM,0)))
	{
		perror("can not create socket\n");
		exit(1);
	}

	memset(&server,0,sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_port = htons(port);


	if (0>(ret=connect(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr))))
	{
		perror("connect error");
		close(sockfd);
		exit(1);

	}

	while(1)
	{
		printf("what words do  you want to tell to server:\n");
		gets(send_buf);

        #ifdef DEBUG
	printf("%s\n",send_buf);
  		#endif 

		if (0>(len=send(sockfd,send_buf,strlen(send_buf),0)))
		{
			perror("send data error\n");
			close(sockfd);
			exit(1);
		}		
		recvnum=recvfrom(sockfd,recv_buf,sizeof(recv_buf),0,(struct sockaddr*)&server,sizeof(struct sockaddr_in));
        ecv_buf[recvnum]='\0';
		printf(recv_buf);
		
		if(0==strcmp(send_buf,"quit")){
			printf("quit success!\n");
			break;
		}
		

	}
	
	close(sockfd);

}



