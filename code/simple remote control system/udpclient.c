#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 2048
int usage(char* cmd)
{
    fprintf(stderr,"%s Usage\n",cmd);
    fprintf(stderr,"%s IP port\n",cmd);
    exit(-1);
}

int main(int argc,char**argv)
{
    int sockfd;
    struct sockaddr_in serv;
    int port;
    char send_buf[BUFSIZ];
    char recv_buf[BUFSIZ];
    int length;
    
    



    if (3!=argc)
    {
        usage(argv[0]);
        exit(-1);
    }

    // 1.create the socket
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if (0>sockfd)
    {
        fprintf(stderr,"error in creating socket\n");
        exit(-1);
    }

    // parse the input from the user
    port=atoi(argv[2]);
    memset(&serv,0,sizeof(serv));
    memset(send_buf,0,sizeof(send_buf));
    memset(recv_buf,0,sizeof(recv_buf));
    serv.sin_family=AF_INET;
    serv.sin_port=htons(port);
    serv.sin_addr.s_addr=inet_addr(argv[1]);


    // communication
    length=sizeof(serv);
    while(1)
    {
        memset(send_buf,0,sizeof(send_buf));
        memset(recv_buf,0,sizeof(recv_buf));

        fprintf(stderr,"%s>",argv[1]);
        fgets(send_buf,sizeof(send_buf),stdin);
        send_buf[strlen(send_buf)-1]='\0';

        sendto(sockfd,send_buf,strlen(send_buf),0,(struct sockaddr*)&serv,sizeof(serv));
        
        if (strcmp(send_buf,"quit")==0)
        {
            fprintf(stderr,"quit the remote control system\n");
            close(sockfd);
            exit(0);
        }
        // pay more attention:the value for the length should be initlized firstly
        recvfrom(sockfd,recv_buf,sizeof(recv_buf),0,(struct sockaddr*)&serv,(socklen_t*)&length);
        fprintf(stderr,"receive message:%s\n",recv_buf);

    }


}