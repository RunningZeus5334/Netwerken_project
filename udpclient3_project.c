#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

int main(int argc, char *argv[]) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,0), &wsaData);

    struct addrinfo internet_address_setup;
    struct addrinfo *internet_address = NULL;
    memset(&internet_address_setup, 0, sizeof internet_address_setup);
    internet_address_setup.ai_family = AF_UNSPEC;
    internet_address_setup.ai_socktype = SOCK_DGRAM;
    getaddrinfo("::1", "56013", &internet_address_setup, &internet_address);

    int internet_socket;
    internet_socket = socket(internet_address->ai_family, internet_address->ai_socktype, internet_address->ai_protocol);

    sendto(internet_socket, "GO", 2, 0, internet_address->ai_addr, internet_address->ai_addrlen);

    char buffer[1000];
    socklen_t internet_address_length = internet_address->ai_addrlen;
    int number_of_bytes_received = recvfrom(internet_socket, buffer, (sizeof buffer) - 1, 0, internet_address->ai_addr, &internet_address_length);
    buffer[number_of_bytes_received] = '\0';

    int max_value = -2147483647; // set initial value to very low value
    while (strcmp(buffer, "OK") != 0) {
        int received_value = atoi(buffer);
        if (received_value != 0) {
            printf("Received value: %d\n", received_value);
            if (received_value > max_value) {
                max_value = received_value;
            }
        }

        char max_value_string[10];
        sprintf(max_value_string, "%d", max_value);
        
        // check if the message is "OK"
        if (strcmp(buffer, "OK") == 0) {
        printf("Maximum value: %d\n", max_value);
        char max_value_string[10];
        sprintf(max_value_string, "%d", max_value);
        sendto(internet_socket, max_value_string, strlen(max_value_string), 0, internet_address->ai_addr, internet_address->ai_addrlen);
        }

        number_of_bytes_received = recvfrom(internet_socket, buffer, (sizeof buffer) - 1, 0, internet_address->ai_addr, &internet_address_length);
        buffer[number_of_bytes_received] = '\0';
    }

// receive the "OK" message
number_of_bytes_received = recvfrom(internet_socket, buffer, (sizeof buffer) - 1, 0, internet_address->ai_addr, &internet_address_length);
buffer[number_of_bytes_received] = '\0';

    freeaddrinfo(internet_address);
    closesocket(internet_socket);

    WSACleanup();
    return 0;
}

