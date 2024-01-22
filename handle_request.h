#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include "utils.h"
#include "handle_get.h"

int handle_request(int client_socket_fd)
{
    int req_body_len;
    char *ptr, req_body[1024];

    // receive messages until the client is done
    req_body_len = recv(client_socket_fd, req_body, 1024, 0);

    // although an if statement seems like the more appropriate thing to use below, we use
    // while below for more control over the control flow of the application
    while (req_body_len > 0)
    {
        // add string terminator at the end of the message
        strcpy(req_body + req_body_len, "\0");

        // check that we received a valid HTTP request
        ptr = strstr(req_body, " HTTP");
        if (ptr == NULL)
        {
            printf("Not a HTTP request.\n");
            break;
        }
        else
        {
            // add a string terminator after the request path
            strcpy(ptr, "\0");

            // print the received request as a string
            printf("\t%s\n", req_body);

            // check the request type
            ptr = strstr(req_body, " /");

            // handle POST requests
            ptr = strstr(req_body, "POST");
            if (ptr != NULL)
            {
                ptr += 5; // move ptr to the start of the path
                // write POST request handler here
                break;
            }

            // point ptr to the request address and do nothing if request is not a GET request
            if ((ptr = strstr(req_body, "GET")) != NULL)
            {
                ptr += 4;
                handle_get(client_socket_fd, ptr);
            }
            else
            {
                printf("Request cannot be handled.\n");
                break;
            }

            send_string(client_socket_fd, "HTTP/1.0 200 OK\r\n");
            send_string(client_socket_fd, "Server: File Storage Server\r\n\r\n");
        } // end of valid HTTP request

        break; // leave this loop by force
    }

    shutdown(client_socket_fd, SHUT_RDWR);
    return 0;
}