#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<stdlib.h>
#include<stdio.h>

int getaddrinfo(const char *node,     // e.g. "www.example.com" or IP
                const char *service,  // e.g. "http" or port number
                const struct addrinfo *hints,
                struct addrinfo **res);
int main(){
	int status;
	struct addrinfo hints;  //Using this we will give it what we want to listen on
	struct addrinfo *servinfo; //will point to results: Linked list with one or more valid addrinfo

	memset(&hints,0,sizeof hints); //Make sure that the struct is empty
	hints.ai_family = AF_UNSPEC; // Leave unspecified to be agnostic of IPV6 or IPV7
	hints.ai_socktype = SOCK_STREAM; //TCP stream sockets
	hints.ai_flags = AI_PASSIVE; // Fill in my IP for me

	if((status = getaddrinfo(NULL,"8080",&hints,&servinfo)) != 0){
		fprintf(stderr, "getaddrinfo error %s\n",gai_strerror(status));
		exit(1);
	}

	// servinfo now points to a linked list of 1 or more struct addrinfos

	// ... do everything until you don't need servinfo anymore ....

	freeaddrinfo(servinfo); // free the linked-list
}