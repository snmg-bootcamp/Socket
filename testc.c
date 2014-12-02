#include <stdio.h>  
#include <stdlib.h>  
#include <errno.h>  
#include <string.h>  
#include <sys/types.h>  
#include <netinet/in.h>  
#include <sys/socket.h>  
#include <sys/wait.h>  
#include <pthread.h>  
#include <sys/time.h>  
#include <arpa/inet.h>  
#include <unistd.h>  
#include <resolv.h>
 
int main(int argc , char *argv[])
{
    int socket_desc;
    int maxfd,retval = -1; 
    struct sockaddr_in server;
    char message[2000] , server_reply[2000];
    fd_set rfds; 
    struct timeval tv;   
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
       
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8889);
 
    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }else 
    puts("Connected\n");
   
    while(1){
    FD_ZERO(&rfds);
    FD_SET(0,&rfds);
    FD_SET(socket_desc,&rfds);    
    maxfd = 0;
    if(socket_desc > maxfd)  
            maxfd = socket_desc; 
    //select
    tv.tv_sec = 1;
    tv.tv_usec= 0;
    retval =select(maxfd,&rfds,NULL,NULL,&tv);
    if(retval == -1){
    printf("select error");
    break;
    }
    else if(retval == 0){
    continue;
    }
    else{
    memset(message,0,sizeof(message));
    fgets(message,sizeof(message),stdin);
    if( send(socket_desc , message , sizeof(message) , 0) < 0)
    {
        puts("Send failed");
        return 1;
    } 
    memset(server_reply,0,sizeof(server_reply)); 
    if( recv(socket_desc, server_reply ,sizeof(server_reply) , 0) < 0)
    {
        puts("recv failed");
    }
    else
        puts(server_reply);
    }
    }
    close(socket_desc);    
    return 0;
}
