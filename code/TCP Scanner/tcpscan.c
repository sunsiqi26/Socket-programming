#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>

void usage(char* cmd)
{
    fprintf(stderr,"%s Usage\n",cmd);
    fprintf(stderr,"%s IP port\n",cmd);
    exit(-1);
}

int is_connect(int port, char *addr)
{
	int sockfd;
	struct sockaddr_in serv;
	int ret;
	int stat = 1;

	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if (0>sockfd)
	{
		fprintf(stderr,"error in creating socket\n");
        exit(-1);
	}

	memset(&serv,0,sizeof(struct sockaddr_in));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(addr);
	serv.sin_port = htons(port);
	
	ret=connect(sockfd,(struct sockaddr*)&serv,sizeof(struct sockaddr));
	close(sockfd);

	return ret;
}

int main(int argc,char** argv)
{
	if(4!= argc)
	{	
		usage(argv[0]);
        exit(-1);
	}
	
	struct servent *servent_pointer;
	int start_port = atoi(argv[2]);
	int end_port = atoi(argv[3]);
	char** cp;
	
	printf("servent_pointer:%s\n",argv[1]);
	printf("start_port:%d\n",start_port);
	printf("end_port:%d\n",end_port);

	for(start_port ; start_port<= end_port ; start_port++  )
	{	
		if(is_connect(start_port,argv[1])  >= 0)
		{
			servent_pointer = getservbyport(htons(start_port),NULL);
			if(servent_pointer != NULL)
			{
				printf("%d\t%s\t%s\n",start_port,servent_pointer->s_name,servent_pointer->s_proto);	
				//for(cp=servent_pointer->s_aliases; *cp!=NULL; ++cp)
            		//printf("\tAlias : %s\n",*cp);	
			}else{
				printf("%d\tunkown\n",start_port);
			}
		}
	}

	return 0;

}