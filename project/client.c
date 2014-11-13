#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdio.h>
int main()
{
int sockfd;
struct sockaddr_in dest;
char buffer[128];
char resp[10]="clientack";

sockfd = socket(PF_INET,SOCK_STREAM,0); 

bzero(&dest,sizeof(dest));
bzero(buffer,128);
recv(sockfd,buffer,sizeof(buffer),0);
printf("receive from server: %s\n",buffer);
send(sockfd,resp,sizeof(resp),0);

close(sockfd);

return 0;

}

