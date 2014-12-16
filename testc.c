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

void *connection_handler(void *);

int numsocket = 0,serversocket[10],i;
int main(int argc , char *argv[])
{
    int socket_desc;
    struct sockaddr_in server;
    fd_set rfds;   
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
    if((serversocket[numsocket]=connect(socket_desc , (struct sockaddr *)&server , sizeof(server))) !=-1)
    {
        puts("connected!");
        printf("%d\n",serversocket[numsocket]);
        pthread_t sniffer_thread;
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler ,(void*)&serversocket[numsocket]) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        numsocket ++;
        puts("connection_handler");
    }else{
        perror("connect failed");
        return 1;
    }
       
        for(i = 0 ;i<numsocket;i++){
        if (serversocket[i]<0)
        {
            perror("accept failed");
            return 1;
        }
        }
    
}
void *connection_handler
(void *socket_desc)
    {
        int sock = *(int*)socket_desc;
        int read_size,send_size;
        char message[2000] , server_reply[2000];
        printf("%d\n", sock);
        while(1){
            memset(message,0,sizeof(message));
            fgets(message,sizeof(message),stdin);
            if( (send_size=send(sock , message , sizeof(message) , 0))> 0)
            {
                puts("sended");
            }else
                 perror("send failed");
            
            memset(server_reply,0,sizeof(server_reply)); 
            if( (read_size=recv(sock, server_reply ,sizeof(server_reply) , 0)) > 0)
            {
                puts(server_reply);
                
            }else if(read_size == 0)
            {
                puts("Client disconnected");
                fflush(stdout);
            }
            if(read_size == -1){
                   perror("recv failed");
            }
        }
            
            
            
            close(sock);    
            return 0;
    }

