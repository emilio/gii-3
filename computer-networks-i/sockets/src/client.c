/**
 * client.c
 *
 * @author Emilio Cobos Álvarez (70912324N)
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>

#define SIZE 512
int main() {
    // Simple prototype to see if it's working, this will need to be refactored
    // later, error checking added, tcp...

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(8000);

    connect(sock, (const struct sockaddr*)&serv_addr, sizeof(serv_addr));

    socklen_t size = sizeof(serv_addr);
    char buff[SIZE];
    size_t recv_size;
    size_t len;
    while (true) {
        printf("> ");
        fgets(buff, SIZE, stdin);
        len = strlen(buff);
        if (len)
            buff[--len] = 0;
        sendto(sock, buff, len, 0, (struct sockaddr*)&serv_addr,
               sizeof(serv_addr));
        recv_size =
            recvfrom(sock, buff, SIZE, 0, (struct sockaddr*)&serv_addr, &size);
        buff[recv_size] = 0;
        printf("< %s\n", buff);
    }

    return 0;
}
