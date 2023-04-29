//https://beej.us/guide/bgnet/html/
/*
** showip.c -- show IP addresses for a host given on the command line
*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>


//This is the call to socket API
/*
It used to be people would hardcode these values, and you can absolutely still do that. 
(domain is PF_INET or PF_INET6, type is SOCK_STREAM or SOCK_DGRAM, and protocol can be set to 0 to choose the proper protocol for the given type. 
Or you can call getprotobyname() to look up the protocol you want, “tcp” or “udp”.)
*/
/*
(This PF_INET thing is a close relative of the AF_INET that you can use when initializing the sin_family field in your struct sockaddr_in.
 In fact, they’re so closely related that they actually have the same value, and many programmers will call socket() and pass AF_INET as the first argument instead of PF_INET. 
 Now, get some milk and cookies, because it’s time for a story. Once upon a time, a long time ago, it was thought that maybe an address family (what the “AF” in “AF_INET” stands for) 
 might support several protocols that were referred to by their protocol family (what the “PF” in “PF_INET” stands for). 
 That didn’t happen. And they all lived happily ever after, The End. So the most correct thing to do is to use AF_INET in your struct sockaddr_in and PF_INET
in your call to socket().)
*/
int socket(int domain, int type, int protocol); 


//sockfd is the socket file descriptor returned by socket(). my_addr is a pointer to a struct sockaddr that contains information about your address, namely, port and IP address. addrlen is the length in bytes of that address.
int bind(int sockfd, struct sockaddr *my_addr, int addrlen);

int main(int argc, char*argv[]){
	struct addrinfo hints, *res, *p; //hints will be passed by us, res is the response returned and p will be used to iterate the linked list
	int status;
	char ipstr[INET6_ADDRSTRLEN]; //Create the chararr wrt to biggest possible value (INET6)

	if(argc<2){
		fprintf(stderr,"usage: showip hostname\n");
		return 1;
	}
	memset(&hints,0,sizeof hints); //Make sure its initialised to 0
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if((status = getaddrinfo(argv[1],NULL,&hints,&res))!=0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
	}

	printf("IP address for %s:\n\n",argv[1]);

	//Iterate the response linked list
	for (p=res;p!=NULL;p=res->ai_next){
		void* addr; // Used to store struct form of extracted IP address
		char* ipver;

		//ai_addr contains generic socket address which can be casted into IP version specific struct based on family
		if(p->ai_family == AF_INET){
			struct sockaddr_in* ipv4=(struct sockaddr_in*)(p->ai_addr);
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		}else{
			struct sockaddr_in6* ipv4=(struct sockaddr_in6*)(p->ai_addr);
			addr = &(ipv4->sin6_addr);
			ipver = "IPv6";	
		}

		//convert the IP to a string (network to presenetation)
		inet_ntop(p->ai_family,addr,ipstr,sizeof ipstr);
		printf("\t%s: %s\n",ipver,ipstr);
	}

	freeaddrinfo(res);
	return 1;
}

/*
Without using getaddrinfo
!!! THIS IS THE OLD WAY !!!

int sockfd;
struct sockaddr_in my_addr;

sockfd = socket(PF_INET, SOCK_STREAM, 0);

my_addr.sin_family = AF_INET;
my_addr.sin_port = htons(MYPORT);     // short, network byte order
my_addr.sin_addr.s_addr = inet_addr("10.12.110.57");
memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr);



*/