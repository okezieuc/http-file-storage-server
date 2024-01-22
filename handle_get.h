#include <stdio.h>
#include <string.h>

int handle_get(int client_socket_fd, char *path)
{
    char resource_path[1000];
    FILE *fptr;

    strcpy(resource_path, "static");
    strcat(resource_path, path); // we assume that the path starts with a "/"

    fptr = fopen(resource_path, "r");

    // check if the file exists

    // allocate enough memory to contain the file

    // send relevant headers for the file

    // send the contents of the file to the client
    return 0;
}
