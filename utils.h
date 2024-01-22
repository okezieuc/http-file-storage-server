#include <sys/socket.h>
#include <string.h>

int send_string(int client_socket_fd, char *str)
{
    return send(client_socket_fd, str, strlen(str), 0);
}
