#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdio.h>
int main()
{
	struct sockaddr_in dest;
	char buffer[] = "Hello World!";

	/* create socket , same as client */
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);

	/* initialize structure dest */
	bzero(&dest,sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_port = htons(8889);
	/* this line is different from client */
	dest.sin_addr.s_addr = INADDR_ANY;

	/* Assign a port number to socket */
	bind(sockfd, (struct sockaddr*)&dest, sizeof(dest));

	/* make it listen to socket with max 20 connections */
	listen(sockfd, 20);

	/* infinity loop -- accepting connection from client forever */
	while(1)
	{
		int clientfd;
		struct sockaddr_in client_addr;
		socklen_t addrlen = sizeof(client_addr);

		/* Wait and Accept connection */
		clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);

		/* Send message */
		send(clientfd, buffer, sizeof(buffer)-1, 0);

		/* close(client) */
		close(clientfd);
	}

	/* close(server) , but never get here because of the loop */
	close(sockfd);
	return 0;
}
