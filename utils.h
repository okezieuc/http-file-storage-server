#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#define STRLEN 17

int send_string(int client_socket_fd, char *str)
{
    return send(client_socket_fd, str, strlen(str), 0);
}

char random_char(int index)
{
    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    return charset[index];
}

char *random_file_name()
{
    char str[STRLEN], *new_file_name;
    int i, index;
    new_file_name = malloc(STRLEN + 10); // we use 10 for potential \r\n additions

    for (i = 0; i < STRLEN - 1; i++)
    {
        index = rand() % 62;
        str[i] = random_char(index);
    }

    strcpy(new_file_name, "static/");
    strncat(new_file_name, str, 17);

    return new_file_name;
}

#endif