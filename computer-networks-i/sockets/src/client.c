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
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <poll.h>
#include <fcntl.h>

#include "logger.h"
#include "protocol.h"
#include "parsing.h"
#include "socket-utils.h"

#define SIZE 512
#define RECV_TIMEOUT 30

/// Shows usage of the program
void show_usage(int argc, char** argv) {
    printf("Usage: %s [options]\n", argv[0]);
    printf("Options:\n");
    printf("  --help\t Display this message and exit\n");
    printf("  -udp, --use-udp\t Use UDP instead of TCP\n");
    printf("  -f, --file [file]\t Read commands from a file instead of being "
           "interactive\n");
    printf("  -l, --log [file]\t Log to [file]\n");
    printf("  -p, --port [port]\t Connect to [port]\n");
    printf("  -h, --host [host]\t Connect to [host]\n");
    printf("  -v, --verbose\t Be verbose about what is going on\n");
    printf("\n");
    printf("Author(s):\n");
    printf("  Emilio Cobos Álvarez (<emiliocobos@usal.es>)\n");
}

int main(int argc, char** argv) {
    bool use_udp = false;
    const char* port = "8000";
    const char* host = "localhost";

    FILE* src_file = stdin;

    LOGGER_CONFIG.log_file = stderr;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0) {
            show_usage(argc, argv);
            return 1;
        } else if (strcmp(argv[i], "-v") == 0 ||
                   strcmp(argv[i], "--verbose") == 0) {
            LOGGER_CONFIG.verbose = true;
        } else if (strcmp(argv[i], "-l") == 0 ||
                   strcmp(argv[i], "--log") == 0) {
            ++i;
            if (i == argc)
                FATAL("The %s option needs a value", argv[i - 1]);

            FILE* log_file = fopen(argv[i], "w");
            if (log_file)
                LOGGER_CONFIG.log_file = log_file;
            else
                WARN("Could not open \"%s\", using stderr: %s", argv[i],
                     strerror(errno));
        } else if (strcmp(argv[i], "-f") == 0 ||
                   strcmp(argv[i], "--file") == 0) {
            ++i;
            if (i == argc)
                FATAL("The %s option needs a value", argv[i - 1]);

            src_file = fopen(argv[i], "r");
            if (!src_file)
                FATAL("Could not open \"%s\" for reading. Reason: %s", argv[i],
                      strerror(errno));
        } else if (strcmp(argv[i], "-p") == 0 ||
                   strcmp(argv[i], "--port") == 0) {
            ++i;
            if (i == argc)
                FATAL("The %s option needs a value", argv[i - 1]);
            port = argv[i];
        } else if (strcmp(argv[i], "-udp") == 0 ||
                   strcmp(argv[i], "--use-udp") == 0) {
            use_udp = true;
        } else if (strcmp(argv[i], "-h") == 0 ||
                   strcmp(argv[i], "--host") == 0) {
            ++i;
            if (i == argc)
                FATAL("The %s option needs a value", argv[i - 1]);
            host = argv[i];
        } else {
            WARN("Unrecognized option: %s", argv[i]);
        }
    }

    LOG("Using %s, port: %s, host: %s", use_udp ? "UDP" : "TCP", port, host);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = use_udp ? SOCK_DGRAM : SOCK_STREAM;

    int ret;
    struct addrinfo* info;
    ret = getaddrinfo(host, port, &hints, &info);
    if (ret != 0)
        FATAL("getaddrinfo: %s", gai_strerror(ret));

    // We use just the first configuration returned by getaddrinfo
    int sock;
    sock = socket(info->ai_family, info->ai_socktype, 0);
    if (sock == -1)
        FATAL("Error creating socket: %s", strerror(errno));

    if (!use_udp) {
        ret = connect(sock, info->ai_addr, info->ai_addrlen);
        if (ret == -1)
            FATAL("Error connecting to remote: %s", strerror(errno));
    }

    struct sockaddr_in serv_addr;
    memcpy(&serv_addr, info->ai_addr, sizeof(serv_addr));

    freeaddrinfo(info);

    char buff[SIZE];
    size_t len;
    bool interactive = src_file == stdin;
    size_t lines_read = 0;

    // Set non-blocking mode, to use `poll`
    int flags = fcntl(sock, F_GETFL, 0);
    int fcntl_ret = fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    if (fcntl_ret == -1)
        FATAL("fcntl() error: %s", strerror(errno));

    struct pollfd pollfds[1] = {
        { sock, POLLIN, 0 }
    };

    while (true) {
        if (interactive)
            printf("> ");

        if (buff != fgets(buff, SIZE, src_file)) {
            if (feof(src_file)) {
                LOG("EOF reached");
                break;
            }

            if (interactive) {
                LOG("Ignoring fgets() error, err: %s",
                    strerror(ferror(src_file)));
                continue;
            } else {
                WARN("fgets() error, err: %s", strerror(ferror(src_file)));
                break;
            }
        }

        lines_read++;

        len = strlen(buff);

        // Trim trailing newline
        if (len && buff[len - 1] == '\n')
            buff[--len] = 0;

        if (!len) {
            LOG("Ignoring empty line (%zu)", lines_read);
            continue;
        }

        LOG("sending %zu bytes: %s", len, buff);

        if (use_udp)
            ret = sendto(sock, buff, len, 0, (struct sockaddr*)&serv_addr,
                         sizeof(serv_addr));
        else
            ret = send(sock, buff, len, 0);

        if (ret == -1) {
            WARN("Send error: %s", strerror(errno));
            continue;
        }

        struct sockaddr from_addr;
        socklen_t from_size = sizeof(from_addr);

        ssize_t recv_size;
        size_t recv_count = 0;
        int poll_ret = 0;
    retry_recv:
        poll_ret = poll(pollfds, 1, RECV_TIMEOUT * 1000);
        if (poll_ret == -1) {
            WARN("poll() error: %s", strerror(errno));
            break;
        }

        if (poll_ret == 0) {
            WARN("timeout reached");
            break;
        }

        /// The server always replies with and ending null char, that should
        /// not be present in the body response
        recv_size = recvfrom(sock, buff, SIZE, 0, &from_addr, &from_size);

        if (recv_size < 0) {
            WARN("Recv error: %s", strerror(errno));
            break;
        }

        // TODO: try to restore this, since it does not work under some
        // circumstances, for example, if using the local machine name instead
        // of localhost.
        // if (use_udp) {
        //     if (sockaddr_cmp(&from_addr,
        //                      (struct sockaddr*)&serv_addr) != 0) {
        //         WARN("Received message not from server: %s", buff);
        //         goto retry_recv;
        //     }
        // }

        if (recv_size == 0) {
            WARN("Server unexpectedly closed connection");
            break;
        }

        ++recv_count;

        // The server always replies with a trailing zero on end,
        // so maybe we have more data to receive.
        bool is_first_message = recv_count == 1;
        bool is_last_message = buff[recv_size - 1] == '\0';

        if (is_first_message && interactive)
            printf("< ");

        buff[recv_size] = 0;
        LOG("recv(%zu, last: %d): %s", recv_size, is_last_message, buff);

        if (interactive)
            printf("%s", buff);

        if (!is_last_message)
            goto retry_recv;
        else if (interactive)
            printf("\n");
    }

    LOG("Closing client");
    close(sock);

    if (LOGGER_CONFIG.log_file)
        fclose(LOGGER_CONFIG.log_file);

    return 0;
}
