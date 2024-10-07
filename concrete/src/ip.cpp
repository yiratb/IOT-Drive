#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h>

#define MAXLINE 1000

int Init(struct sockaddr_in *servaddr, const char *ip, int type, int port)
{
    int listenfd;
    /*sets all bytes in the servaddr structure to zero.
     This ensures that any fields in servaddr that are not
     explicitly set later are initialized to zero.*/ 
	bzero(servaddr, sizeof(*servaddr)); 

    /* socket(IPv4, type, 0) */
	listenfd = socket(AF_INET, type, 0);
    /* This line sets the IP address for the server */
    if (NULL == ip)
    {
	    servaddr->sin_addr.s_addr = htonl(INADDR_ANY); 
    }
    else
    {
        servaddr->sin_addr.s_addr = inet_addr(ip); 
    }
    
    /* Sets the server port number to be 5000*/ 
	servaddr->sin_port = htons(port); 
    /* Specifies the address family as IPv4 */
	servaddr->sin_family = AF_INET; 

    return (listenfd);
}