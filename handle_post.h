#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
    This function handles POST requests to the server. It uploads the file contained in the
    request body to the static folder.

    Parameters:
    - client_socket_fd: The client's socket file descriptor
    - *req_body: a pointer to the portion of the request body immediately following the request path.


    Returns:
    0 on success.
*/
int handle_post(int client_socket_fd, char *req_body)
{
    char *ptr, c_len_str[50], *content;
    int c_len; // content length

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

    // copy content to buffer
    strncpy(content, ptr, c_len);

    // save buffer contents in a file

    return 0;
}