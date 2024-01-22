#include <stdio.h>
#include <string.h>

int handle_get(int client_socket_fd, char *path)
{
    char resource_path[1000];
    FILE *fptr = NULL;

    strcpy(resource_path, "static");
    strcat(resource_path, path); // we assume that the path starts with a "/"

    fptr = fopen(resource_path, "r");

    // check if the file exists
    if (fptr == NULL || strcmp(resource_path, "static/") == 0)
    {
        send_string(client_socket_fd, "HTTP/1.0 404 NOT FOUND\r\n");
        send_string(client_socket_fd, "Server: File Storage Server\r\n\r\n");
        send_string(client_socket_fd, "404: Resource Not Found\r\n");
        return 0;
    }

    // allocate enough memory to contain the file

    // send relevant headers for the file

    // send the contents of the file to the client
    return 0;
}
