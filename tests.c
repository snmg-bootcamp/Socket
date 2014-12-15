#include<stdio.h>
#include<string.h>   
#include<stdlib.h>   
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>   
#include<pthread.h> 

void *connection_handler(void *);

int numsocket = 0,clientsocket[10];
int main
(int argc , char *argv[])
{
    int socket_desc, c,read_size,i;
    struct sockaddr_in server , client;
 
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8889);
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("bind failed");
        return 1;
    }else
    puts("bind done");
     
    //Listen
    listen(socket_desc , 10);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (clientsocket[numsocket]= accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
         
        pthread_t sniffer_thread;
        numsocket ++;
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler ,NULL) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        puts("Handler assigned");
    }
    for(i = 0 ;i<numsocket;i++){
	    if (clientsocket[i]<0)
	    {
	        perror("accept failed");
	        return 1;
	    }
    }

   void *connection_handler
   (void *socket_desc)
	{
	  //Receive a message from client
	        char client_message[2000];
		    read_size = recv(clientsocket[numsocket], client_message , 2000 , 0);
			   while(read_size> 0 )
			   {
			       //Send the message back to client
			       for(i = 0 ;i<numsocket;i++)
			       write(clientsocket[i], client_message , strlen(client_message));
			   }   
		    if(read_size == 0)
		    {
		        puts("Client disconnected");
		        fflush(stdout);
		    }
		    else if(read_size == -1)
		    {
		        perror("recv failed");
		    }
		   	
		    
		    free(socket_desc);
		     
		    return 0;
		    
	}
}
                                                                    

