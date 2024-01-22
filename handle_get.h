#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int handle_get(int client_socket_fd, char *path)
{
    long int resource_size;
    char resource_path[1000];
    FILE *fptr = NULL;
    void *resource;

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
    fseek(fptr, 0, SEEK_END);
    resource_size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    // send relevant headers for the file
    send_string(client_socket_fd, "HTTP/1.0 200 OK\r\n");
    send_string(client_socket_fd, "Server: File Storage Server\r\n\r\n");

    // send the contents of the file to the client
    resource = malloc(resource_size);
    fread((char *)resource, 1, resource_size, fptr);
    fclose(fptr);
    send(client_socket_fd, resource, resource_size, 0);

    free(resource);
    return 0;
}
