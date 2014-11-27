#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
int main(void)
{
	struct sockaddr_in dest;
	char buffer[128];

	/* create socket */
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	
	/* initialize value in dest */
	memset(&dest,0,sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_port = htons(8889);
	inet_aton("127.0.0.1", &dest.sin_addr);

	/* Connecting to server */
	connect(sockfd, (struct sockaddr*)&dest, sizeof(dest));
	
	/* Receive message from the server and print to screen */
	bzero(buffer, 128);
	recv(sockfd, buffer, sizeof(buffer)-1, 0);
	printf("%s", buffer);

	/* Close connection */
	close(sockfd);

	return 0;
}
