#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h> // contains defintions for the IP family
#include <unistd.h>     // contains definitions for close()
#include <arpa/inet.h>  // for interner address conversions
#include <time.h>
#include "handle_request.h"

#define PORT 8000

int main()
{
    int server_socket_fd, client_socket_fd, yes = 1;
    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_size;

    srand(time(NULL));

    // create a new socket where the server will run
    if ((server_socket_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("An error occured while creating a socket");
        return -1;
    }

    if (setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        printf("An error occured when setting socket options SO_REUSEADDR");
    }

    // construct the server's address object
    server_addr.sin_port = htons(PORT); // convert variable endianness
    server_addr.sin_addr.s_addr = 0;
    server_addr.sin_family = PF_INET;
    memset(&(server_addr.sin_zero), '\0', 8);

    // bind the server to a local address
    if (bind(server_socket_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        printf("An error occured while binding the server to a local port.\n");
        return -1;
    }

    // start listening for connections
    if (listen(server_socket_fd, 30) == -1)
    {
        printf("An error occured while opening the socket to listen for connections.");
        return -1;
    }

    while (1)
    {
        client_socket_fd = accept(server_socket_fd, (struct sockaddr *)&client_addr, &sin_size);
        if (client_socket_fd == -1)
        {
            printf("An error occured while receiving a connection");
            return -1;
        }

        // log request info to console
        printf("Received a request from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        handle_request(client_socket_fd);
    }

    return 0;
}
