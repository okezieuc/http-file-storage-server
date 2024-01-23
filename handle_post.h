#ifndef HANDLE_POST_H
#define HANDLE_POST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

/*
    This function handles POST requests to the server. It uploads the file contained in the
    request body to the static folder.

    Parameters:
    - client_socket_fd: The client's socket file descriptor
    - *req_body: a pointer to the portion of the request body immediately following the request path.


    Returns:
    0 on success.
*/
int handle_post(int client_socket_fd, char *req_body, int received_bytes, int ptr_offset)
{
    char *ptr, c_len_str[50], *content, *new_file_name, *buffer, new_req_body[1024];
    int c_len, new_received_bytes_length; // content length
    FILE *fptr;

    // get the expected length of the file to upload
    // and allocate memory for the file to be received
    ptr = strstr(req_body, "Content-Length: ");
    if (ptr == NULL)
    {
        printf("Content-Length header not included in request");
        return -1;
    }

    ptr += 16;
    strncpy(c_len_str, ptr, strstr(ptr, "\r\n") - ptr);
    strncpy(c_len_str + (strstr(ptr, "\r\n") - ptr), "\0", 1); // add null pointer after length
    c_len = atoi(ptr);                                         // set c_len to the length of the content
    content = (char *)malloc(c_len);

    // TODO: Handle case where full request body has not been received

    // set ptr to point to the start of the content
    ptr = strstr(ptr + strlen(c_len_str) + 1, "\r\n\r\n");

    // skip ptr through any newlines
    while (strncmp(ptr, "\r\n", 2) == 0)
    {
        ptr += 2;
    }

    // find for the number of bytes we have received
    // we need this to only copy the number of bytes we have received
    // and to confirm that we have received the whole file
    received_bytes = received_bytes - (ptr - req_body) - ptr_offset;

    // copy received content to buffer
    memcpy(content, ptr, received_bytes);

    // if and until we received enough bytes, receive more data from the client
    while (received_bytes < c_len)
    {
        // get new data
        new_received_bytes_length = recv(client_socket_fd, new_req_body, 1023, 0);

        // store the data in our buffer
        memcpy(content + received_bytes, new_req_body, new_received_bytes_length);

        // update the length of received bytes
        received_bytes += new_received_bytes_length;
    }

    // save buffer contents in a file
    new_file_name = random_file_name();

    if ((fptr = fopen(new_file_name, "wb")) == NULL)
    {
        printf("Error creating a new file");
        return -1;
    }

    fwrite(content, 1, c_len, fptr);
    printf("\tUploaded file to /%s\n", new_file_name);
    fclose(fptr);

    // send response to client
    strncat(new_file_name, "\r\n", 2);
    send_string(client_socket_fd, "HTTP/1.0 200 OK\r\n");
    send_string(client_socket_fd, "Server: File Storage Server\r\n\r\n");
    send_string(client_socket_fd, "File uploaded\r\n");
    send_string(client_socket_fd, new_file_name + 7);

    free(new_file_name);
    free(content);
    return 0;
}

#endif