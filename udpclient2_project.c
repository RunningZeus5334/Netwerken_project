#ifdef _WIN32
	#define _WIN32_WINNT _WIN32_WINNT_WIN7 //select minimal legacy support, needed for inet_pton, inet_ntop
	#include <winsock2.h> //for all socket programming
	#include <ws2tcpip.h> //for getaddrinfo, inet_pton, inet_ntop
	#include <stdio.h> //for fprintf
	#include <unistd.h> //for close
	#include <stdlib.h> //for exit
	#include <string.h> //for memset
#else
	#include <sys/socket.h> //for sockaddr, socket, socket
	#include <sys/types.h> //for size_t
	#include <netdb.h> //for getaddrinfo
	#include <netinet/in.h> //for sockaddr_in
	#include <arpa/inet.h> //for htons, htonl, inet_pton, inet_ntop
	#include <errno.h> //for errno
	#include <stdio.h> //for fprintf, perror
	#include <unistd.h> //for close
	#include <stdlib.h> //for exit
	#include <string.h> //for memset
#endif

int main( int argc, char * argv[] )
{
	//////////////////
	//Initialization//
	//////////////////

	//Step 1.0
	WSADATA wsaData;
	WSAStartup( MAKEWORD(2,0), &wsaData );

	//Step 1.1
	struct addrinfo internet_address_setup;
	struct addrinfo *internet_address = NULL;
	memset( &internet_address_setup, 0, sizeof internet_address_setup );
	internet_address_setup.ai_family = AF_UNSPEC;
	internet_address_setup.ai_socktype = SOCK_DGRAM;
	getaddrinfo( "::1", "56013", &internet_address_setup, &internet_address );

	//Step 1.2
	int internet_socket;
	internet_socket = socket( internet_address->ai_family, internet_address->ai_socktype, internet_address->ai_protocol );


	int max_value = -1;
	int number_of_bytes_received = 0;
	char buffer[1000];
	socklen_t internet_address_length = internet_address->ai_addrlen;
	number_of_bytes_received = recvfrom( internet_socket, buffer, ( sizeof buffer ) - 1, 0, internet_address->ai_addr, &internet_address_length );
	buffer[number_of_bytes_received] = '\0';

while (strcmp(buffer, "OK") != 0) {
	  sendto( internet_socket, "GO", 2, 0, internet_address->ai_addr, internet_address->ai_addrlen );
    socklen_t internet_address_length = internet_address->ai_addrlen;
    buffer[number_of_bytes_received] = '\0';
    printf( "%s\n", buffer );
    char max_value_string[10];
    
    number_of_bytes_received = recvfrom( internet_socket, buffer, ( sizeof buffer ) - 1, 0, internet_address->ai_addr, &internet_address_length );
    sprintf(max_value_string, "%d", max_value);
    sendto( internet_socket, max_value_string, strlen(max_value_string), 0, internet_address->ai_addr, internet_address->ai_addrlen );
       int received_value = atoi(buffer);
    if (received_value > max_value) {
        max_value = received_value;
    
}
}
	////////////
	//Clean up//
	////////////

	//Step 3.2
	freeaddrinfo( internet_address );

	//Step 3.1
	close( internet_socket );

	//Step 3.0
	WSACleanup();

	return 0;
	
}
