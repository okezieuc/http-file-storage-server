#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h> // contains defintions for the IP family
#include <unistd.h>     // contains definitions for close()
#include <arpa/inet.h>  // for interner address conversions

#define PORT 8000

int send_string(int, char *);

int main()
{
    int server_socket_fd, client_socket_fd, req_body_len, yes = 1;
    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_size;
    char res[1000];
    void *req_body = malloc(1000);
    void *http_req_path, *ptr; // will store the requested http path

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

        // receive messages until the client is done
        req_body_len = recv(client_socket_fd, req_body, 1000, 0);
        if (req_body_len > 0)
        {
            // add string terminator at the end of the message
            strcpy(req_body + req_body_len, "\0");

            // print the received request as a string
            printf("Received: %s\n", (char *)req_body);

            // place a string terminator after the request type
            http_req_path = strstr(req_body, "/");
            if (http_req_path == NULL)
            {
                printf("Invalid HTTP request received\n");
                break;
            }
            strcpy(http_req_path - 1, "\0");

            ptr = strstr(http_req_path, "\r\n");
            if (ptr == NULL)
            {
                printf("Invalid HTTP request received\n");
                break;
            }

            strcpy(ptr, "\0\0");

            printf("A %s request was made to the path %s\n", (char *)req_body, (char *)http_req_path);
            send_string(client_socket_fd, "HTTP/1.0 200 OK\r\n");
            send_string(client_socket_fd, "Server: File Storage Server\r\n\r\n");
        }

        // if request closed gracefully
        if (req_body_len == 0)
        {
            printf("Client has performed an orderly shutdown");
        }

        // if an error occurred when receiving a message
        if (req_body_len == -1)
        {
            printf("An error occurred when receiving a message");
        }

        shutdown(client_socket_fd, SHUT_RDWR);
        printf("Request closed");
    }

    return 0;
}

int send_string(int client_socket_fd, char *str)
{
    return send(client_socket_fd, str, strlen(str), 0);
}
