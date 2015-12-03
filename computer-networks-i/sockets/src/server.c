/**
 * server.c
 *
 * @author Emilio Cobos Álvarez (70912324N)
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <signal.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <unistd.h>

#include "logger.h"
#include "protocol.h"
#include "connection-state.h"
#include "utils.h"
#include "vector.h"

#define TCP_MAX_CONNECTIONS 1024
#define UDP_MAX_CONNECTIONS 1024
#define MAX_MESSAGE_SIZE 512

struct server_data {
    pthread_mutex_t mutex;
    vector_t events;
    vector_t users;
    vector_t assistances;
    vector_t invitations;
} GLOBAL_DATA = {PTHREAD_MUTEX_INITIALIZER,
                 VECTOR_INITIALIZER(sizeof(protocol_event_t)),
                 VECTOR_INITIALIZER(sizeof(protocol_user_t)),
                 VECTOR_INITIALIZER(sizeof(protocol_assistance_t)),
                 VECTOR_INITIALIZER(sizeof(protocol_invitation_t))};

bool event_exists(protocol_event_id_t id, protocol_event_t* event) {
    // NOTE: no locking here since it's supposed to be inmutable, fixme if it
    // changes
    //
    // Wohoo linear search FTW.
    for (size_t i = 0; i < vector_size(&GLOBAL_DATA.events); ++i) {
        vector_get(&GLOBAL_DATA.events, i, event);
        if (strcmp(event->id, id) == 0)
            return true;
    }

    return false;
}

bool user_exists(protocol_uid_t id, protocol_user_t* user) {
    // NOTE: no locking here since it's supposed to be inmutable, fixme if it
    // changes
    for (size_t i = 0; i < vector_size(&GLOBAL_DATA.users); ++i) {
        vector_get(&GLOBAL_DATA.users, i, user);
        if (strcmp(user->id, id) == 0)
            return true;
    }

    return false;
}

bool invitation_exists(protocol_uid_t uid, protocol_event_id_t event_id) {
    protocol_invitation_t invitation;
    // NOTE: no locking here since it's supposed to be inmutable, fixme if it
    // changes
    for (size_t i = 0; i < vector_size(&GLOBAL_DATA.invitations); ++i) {
        vector_get(&GLOBAL_DATA.invitations, i, &invitation);
        if (strcmp(invitation.uid, uid) == 0 &&
            strcmp(invitation.event_id, event_id) == 0)
            return true;
    }

    return false;
}

// Adds an assistance to the global data
// returns false if the assistance was previously registered
bool add_assistance(protocol_assistance_t* assistance) {
    pthread_mutex_lock(&GLOBAL_DATA.mutex);
    protocol_assistance_t existing_assistance;

    for (size_t i = 0; i < vector_size(&GLOBAL_DATA.assistances); ++i) {
        vector_get(&GLOBAL_DATA.assistances, i, &existing_assistance);
        if (strcmp(assistance->uid, existing_assistance.uid) == 0 &&
            strcmp(assistance->event_id, existing_assistance.event_id) == 0) {
            pthread_mutex_unlock(&GLOBAL_DATA.mutex);
            return false;
        }
    }

    vector_push(&GLOBAL_DATA.assistances, assistance);
    pthread_mutex_unlock(&GLOBAL_DATA.mutex);

    return true;
}

bool get_events_from(const char* filename) {
    FILE* f;
    char line[512];
    protocol_event_t tmp_event;

    f = fopen(filename, "r");
    if (!f) {
        WARN("Couldn't open \"%s\": %s", filename, strerror(errno));
        return false;
    }

    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);

        // Trim last newline
        if (line[len - 1] == '\n')
            line[len - 1] = '\0';

        LOG("event_parse: %s", line);

        parse_error_t error = parse_event(line, &tmp_event);
        if (error != ERROR_NONE) {
            WARN("Parse error: %s", parse_error_string(error));
            continue;
        }

        if (mktime(&tmp_event.starts_at) > mktime(&tmp_event.ends_at)) {
            WARN("Event can't start after it ends");
            continue;
        }

        vector_push(&GLOBAL_DATA.events, &tmp_event);
    }

    LOG("event_parse: Found %zu events", vector_size(&GLOBAL_DATA.events));
    fclose(f);

    return true;
}

bool get_users_from(const char* filename) {
    FILE* f;
    char line[512];
    protocol_user_t tmp_user;

    f = fopen(filename, "r");
    if (!f) {
        WARN("Couldn't open \"%s\": %s", filename, strerror(errno));
        return false;
    }

    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);

        // Trim last newline
        if (line[len - 1] == '\n')
            line[len - 1] = '\0';

        LOG("user_parse: %s", line);

        parse_error_t error = parse_user(line, &tmp_user);
        if (error != ERROR_NONE) {
            WARN("Parse error: %s", parse_error_string(error));
            continue;
        }

        vector_push(&GLOBAL_DATA.users, &tmp_user);
    }

    LOG("user_parse: Found %zu users", vector_size(&GLOBAL_DATA.users));
    fclose(f);

    return true;
}

bool get_invitations_from(const char* filename) {
    FILE* f;
    char line[512];
    protocol_invitation_t tmp_invitation;
    protocol_event_t event;
    protocol_user_t user;

    f = fopen(filename, "r");
    if (!f) {
        WARN("Couldn't open \"%s\": %s", filename, strerror(errno));
        return false;
    }

    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);

        // Trim last newline
        if (line[len - 1] == '\n')
            line[len - 1] = '\0';

        LOG("invitation_parse: %s", line);

        parse_error_t error = parse_invitation(line, &tmp_invitation);
        if (error != ERROR_NONE) {
            WARN("Parse error: %s", parse_error_string(error));
            continue;
        }

        if (!event_exists(tmp_invitation.event_id, &event)) {
            WARN("Event \"%s\" does not exist, ignoring",
                 tmp_invitation.event_id);
            continue;
        }

        if (!user_exists(tmp_invitation.uid, &user)) {
            WARN("User \"%s\" does not exist, ignoring", tmp_invitation.uid);
            continue;
        }

        // if (!invitation_exists(..))
        // There's no practical issue with a duplicated invitation

        vector_push(&GLOBAL_DATA.invitations, &tmp_invitation);
    }

    LOG("invitation_parse: Found %zu invitations",
        vector_size(&GLOBAL_DATA.invitations));
    fclose(f);

    return true;
}

bool get_assistances_from(const char* filename) {
    FILE* f;
    char line[512];
    protocol_assistance_t tmp_assistance;
    protocol_event_t event;
    protocol_user_t user;

    f = fopen(filename, "r");
    if (!f) {
        WARN("Couldn't open \"%s\": %s", filename, strerror(errno));
        return false;
    }

    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);

        // Trim last newline
        if (line[len - 1] == '\n')
            line[len - 1] = '\0';

        LOG("assistance_parse: %s", line);

        parse_error_t error = parse_assistance(line, &tmp_assistance);
        if (error != ERROR_NONE) {
            WARN("Parse error: %s", parse_error_string(error));
            continue;
        }

        if (!event_exists(tmp_assistance.event_id, &event)) {
            WARN("Event \"%s\" does not exist, ignoring",
                 tmp_assistance.event_id);
            continue;
        }

        time_t assistance_time = mktime(&tmp_assistance.at);

        if (mktime(&event.starts_at) > assistance_time ||
            mktime(&event.ends_at) < assistance_time) {
            WARN("Invalid time for assistance to event \"%s\"", event.id);
            continue;
        }

        if (!user_exists(tmp_assistance.uid, &user)) {
            WARN("User \"%s\" does not exist, ignoring", tmp_assistance.uid);
            continue;
        }

        if (!invitation_exists(tmp_assistance.uid, tmp_assistance.event_id)) {
            WARN("Invalid assistance from \"%s\" to \"%s\", not invited",
                 tmp_assistance.uid, tmp_assistance.event_id);
            continue;
        }

        if (!add_assistance(&tmp_assistance)) {
            WARN("Assistance already registered for \"%s\" -> \"%s\"",
                 tmp_assistance.uid, tmp_assistance.event_id);
            continue;
        }
    }

    LOG("assistance_parse: Found %zu assistances",
        vector_size(&GLOBAL_DATA.assistances));
    fclose(f);

    return true;
}

bool send_event_list(int sock, struct sockaddr* target_addr,
                     socklen_t target_addr_len) {
    protocol_event_t event;
    char response[512];
    char starts_at[20];
    char ends_at[20];
    time_t now = time(NULL);
    size_t found = 0;
    int ret;

    // No need to lock here, events are inmutable
    for (size_t i = 0; i < vector_size(&GLOBAL_DATA.events); ++i) {
        vector_get(&GLOBAL_DATA.events, i, &event);
        if (mktime(&event.starts_at) < now && mktime(&event.ends_at) > now) {
            ++found;

            strftime(starts_at, sizeof(starts_at), "%d/%m/%Y %H:%M:%S",
                     &event.starts_at);
            strftime(ends_at, sizeof(ends_at), "%d/%m/%Y %H:%M:%S",
                     &event.ends_at);

            // If we're not the firsts we add the newline of the previous
            if (found == 1) {
                snprintf(response, sizeof(response), "%s#%s#%s#%s", event.id,
                         event.description, starts_at, ends_at);
            } else {
                snprintf(response, sizeof(response), "\n%s#%s#%s#%s", event.id,
                         event.description, starts_at, ends_at);
            }

            ret = sendto(sock, response, strlen(response), 0, target_addr,
                         target_addr_len);
            if (ret == -1) {
                WARN("Ignoring sendto() error (socket: %d, response: %s)", sock,
                     response);
                return false;
            }
        }
    }

    // Send the trailing byte
    response[0] = '\0';
    ret = sendto(sock, response, 1, 0, target_addr, target_addr_len);
    if (ret == -1) {
        WARN("Ignoring sendto() error (socket: %d, response: %s)", sock,
             response);
        return false;
    }

    return true;
}

bool send_assistance_list(int sock, struct sockaddr* target_addr,
                          socklen_t target_addr_len,
                          client_message_t* message) {
    assert(message->type == MESSAGE_TYPE_ASSISTANCE_LIST);
    protocol_assistance_t assistance;
    char at[20];
    char response[512];
    int ret;
    size_t found = 0;

    pthread_mutex_lock(&GLOBAL_DATA.mutex);

    // TODO: We probably just want to send one, since we don't allow duplicates.
    //
    // We initially did so it can remain this way for now.
    for (size_t i = 0; i < vector_size(&GLOBAL_DATA.assistances); ++i) {
        vector_get(&GLOBAL_DATA.assistances, i, &assistance);
        if (strcmp(assistance.uid, message->content.assistance_list_info.uid) ==
                0 &&
            strcmp(assistance.event_id,
                   message->content.assistance_list_info.event_id) == 0) {
            ++found;

            strftime(at, sizeof(at), "%d/%m/%Y %H:%M:%S", &assistance.at);
            if (found == 1) {
                snprintf(response, sizeof(response), "%s#%s#%s", assistance.uid,
                         assistance.event_id, at);
            } else {
                snprintf(response, sizeof(response), "\n%s#%s#%s",
                         assistance.uid, assistance.event_id, at);
            }

            ret = sendto(sock, response, strlen(response), 0, target_addr,
                         target_addr_len);
            if (ret == -1) {
                WARN("Ignoring sendto() error (socket: %d, response: %s)", sock,
                     response);
                pthread_mutex_unlock(&GLOBAL_DATA.mutex);
                return false;
            }
        }
    }

    pthread_mutex_unlock(&GLOBAL_DATA.mutex);

    response[0] = '\0';
    ret = sendto(sock, response, 1, 0, target_addr, target_addr_len);
    if (ret == -1) {
        WARN("Ignoring sendto() error (socket: %d, response: %s)", sock,
             response);
        return false;
    }

    return true;
}

bool fill_assistance(int sock, struct sockaddr* target_addr,
                     socklen_t target_addr_len, client_message_t* message) {
    protocol_assistance_t assistance;
    protocol_event_t event;
    char response[512];
    int ret;

    assert(message->type == MESSAGE_TYPE_ASSISTANCE);

    time_t assistance_time = mktime(&message->content.assistance_info.datetime);
    strcpy(assistance.uid, message->content.assistance_info.uid);
    strcpy(assistance.event_id, message->content.assistance_info.event_id);
    memcpy(&assistance.at, &message->content.assistance_info.datetime,
           sizeof(struct tm));

    if (!event_exists(assistance.event_id, &event)) {
        snprintf(response, sizeof(response), "ERROR invalid event_id");
    } else if (mktime(&event.starts_at) > assistance_time ||
               mktime(&event.ends_at) < assistance_time) {
        snprintf(response, sizeof(response),
                 "ERROR event expired or not yet started");
    } else if (!invitation_exists(assistance.uid, assistance.event_id)) {
        snprintf(response, sizeof(response), "ERROR not invited");
    } else if (!add_assistance(&assistance)) {
        snprintf(response, sizeof(response),
                 "ERROR assistance already registered");
    } else {
        snprintf(response, sizeof(response), "OK");
    }

    ret = sendto(sock, response, strlen(response) + 1, 0, target_addr,
                 target_addr_len);
    if (ret == -1) {
        WARN("Ignoring sendto() error (socket: %d, response: %s)", sock,
             response);
        return false;
    }

    return true;
}

/// Signal handler used to daemonize the program
void daemonize_sig_handler(int signal) {
    switch (signal) {
        case SIGCHLD: // The daemon died
            FATAL("Daemon has died on startup");
        case SIGALRM:
            LOG("Daemon timed out waiting for children, they're probably fine");
            exit(0);
    }
}

/// Shows usage of the program
void show_usage(int argc, char** argv) {
    printf("Usage: %s [options]\n", argv[0]);
    printf("Options:\n");
    printf("  -h, --help\t Display this message and exit\n");
    printf("  -p, --port [port]\t Listen to [port]\n");
    printf("  -v, --verbose\t Be verbose about what is going on\n");
    printf("  -d, --daemonize\t Start server as a daemon\n");
    printf("  -e, --events [filename]\t Use [file] as event data source\n");
    printf("  -u, --users [filename]\t Use [file] as user data source\n");
    printf("  -a, --assistances [filename]\t Use [file] as assistance data "
           "source\n");
    printf("  -i, --invitations [filename]\t Use [file] as invitation data "
           "source\n");
    printf("\n");
    printf("Author(s):\n");
    printf("  Emilio Cobos Álvarez (<emiliocobos@usal.es>)\n");
}

/// Parses the message stored in buff, and sends the correct reply to `sock`,
/// using `sendto()` and the given `target_addr`, and `target_addr_len`.
///
/// This function expects `buff` to be zero-terminated.
///
/// We exploit the fact that `sento()` can be used instead of `write()` in TCP,
/// given that `target_addr` is null and `target_addr_size` is zero.
///
/// The return value is a boolean indicating if the connection (in the tcp
/// case) should continue
bool parse_message_and_reply(int sock, connection_state_t* state,
                             const char* buff, struct sockaddr* target_addr,
                             socklen_t target_addr_len) {
#define RESPOND(...) snprintf(response, sizeof(response), __VA_ARGS__)
#define RESPOND_ERROR(...) RESPOND("ERROR " __VA_ARGS__)

    // If the first character of the response is null at the end of it, we
    // assume success and reply "OK"
    //
    // TODO: We allocate too much space in the stack here if we end up calling
    // to another function
    char response[512];
    ssize_t ret;

    client_message_t message;
    parse_error_t error = parse_client_message(buff, &message);

    response[0] = '\0';

    if (error != ERROR_NONE) {
        LOG("Parse error: %s", parse_error_string(error));
        RESPOND_ERROR("Parse error: %s", parse_error_string(error));
        ret = sendto(sock, response, strlen(response) + 1, 0, target_addr,
                     target_addr_len);

        if (ret == -1) {
            WARN("Ignoring sendto() error (socket: %d, response: %s)", sock,
                 response);
            return false;
        }

        return true;
    }

    protocol_user_t user;
    switch (message.type) {
        case MESSAGE_TYPE_HELLO:
            if (state->logged_in) {
                RESPOND_ERROR("Already logged in");
                break;
            }

            if (!user_exists(message.content.uid, &user)) {
                RESPOND_ERROR("User not registered");
                break;
            }
            strncpy(state->uid, message.content.uid, sizeof(protocol_uid_t));
            state->logged_in = true;
            break;

        case MESSAGE_TYPE_BYE:
            if (!state->logged_in) {
                RESPOND_ERROR("Not logged in");
                break;
            }

            if (strcmp(state->uid, message.content.uid) != 0) {
                RESPOND_ERROR("Invalid user");
                break;
            }

            state->logged_in = false;
            break;

        case MESSAGE_TYPE_EVENT_LIST:
            return send_event_list(sock, target_addr, target_addr_len);

        case MESSAGE_TYPE_ASSISTANCE_LIST:
            return send_assistance_list(sock, target_addr, target_addr_len,
                                        &message);

        case MESSAGE_TYPE_ASSISTANCE:
            return fill_assistance(sock, target_addr, target_addr_len,
                                   &message);

        default:
            WARN("Unexpected message type %d", message.type);
            RESPOND_ERROR("Unknown error");
    }

    if (!response[0])
        RESPOND("OK");

    // The strlen(response) + 1 is to always add a trailing null byte to the
    // response
    //
    // This is useful over all because of randomly large responses like
    // `send_event_list`.
    ret = sendto(sock, response, strlen(response) + 1, 0, target_addr,
                 target_addr_len);
    if (ret == -1) {
        WARN("Ignoring sendto() error (socket: %d, response: %s)", sock,
             response);
        return false;
    }

    return true;
#undef RESPOND
#undef RESPOND_ERROR
}

void* handle_tcp_connection(void* sent_socket) {
    int socket = *((int*)sent_socket);
    free(sent_socket);
    connection_state_t state;
    connection_state_init(&state);

    /// Set a recv timeout, to allow closing connections
    struct timeval tv;
    memset(&tv, 0, sizeof(struct timeval));
    tv.tv_sec = 30;
    setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv,
               sizeof(struct timeval));

    LOG("tcp: Connection handling routine started (fd: %d)", socket);

    char buff[MAX_MESSAGE_SIZE] = {0};
    while (true) {
        // TODO: We don't handle chunked messages because we know the message
        // size is fixed, and that messages longer than `MAX_MESSAGE_SIZE`
        // can't be valid. We should have a growing buffer otherwise.
        ssize_t len = recv(socket, buff, sizeof(buff) - 1, 0);
        if (len == -1) {
            WARN("tcp: (fd: %d) read() error: %s", socket, strerror(errno));
            break;
        }

        if (len == 0) {
            LOG("tcp: (fd: %d) received empty message", socket);
            break;
        }

        buff[len] = 0;

        LOG("tcp: (fd: %d) received %zu bytes: %s", socket, len, buff);
        if (!parse_message_and_reply(socket, &state, buff, NULL, 0))
            break;
    }

    LOG("tcp: Closing connection (fd: %d)", socket);
    close(socket);

    return NULL;
}

void* start_tcp_server(void* info) {
    long port = *((long*)info);
    struct sockaddr_in serv_addr;
    int sock;
    int ret;

    LOG("start_tcp_server(%p); port: %ld", info, port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        ERROR("Couldn't open TCP socket: %s", strerror(errno));
        return NULL;
    }

    LOG("Opened TCP socket (fd: %d)", sock);

    int yes = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        WARN("tcp: Couldn't set SO_REUSEADDR. Reason: %s", strerror(errno));

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    ret = bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret == -1) {
        ERROR("Couldn't bind TCP socket: %s", strerror(errno));
        goto cleanup_and_return;
    }

    LOG("TCP socket bound to port: %ld", port);

    ret = listen(sock, TCP_MAX_CONNECTIONS);
    if (ret == -1) {
        ERROR("Listen: %s", strerror(errno));
        goto cleanup_and_return;
    }

    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_size = sizeof(client_addr);

        int new_socket =
            accept(sock, (struct sockaddr*)&client_addr, &client_size);

        if (new_socket == -1) {
            WARN("tcp: Ignoring accept() error: %s", strerror(errno));
            continue;
        }

        LOG("tcp: New connection (fd: %d)", new_socket);
        int* sent_socket = malloc(sizeof(int));
        *sent_socket = new_socket;

        pthread_t new_connection_thread;
        pthread_create(&new_connection_thread, NULL, handle_tcp_connection,
                       sent_socket);
        pthread_detach(new_connection_thread);
    }

cleanup_and_return:
    LOG("TCP server closing");
    close(sock);
    return NULL;
}

struct udp_cache_data {
    struct sockaddr_in address;
    time_t last_time_accessed;
} udp_cache_data_t;

void* start_udp_server(void* info) {
    long port = *((long*)info);
    struct sockaddr_in serv_addr;
    int sock;
    int ret;
    connection_state_t fake_connection_state;

    LOG("start_udp_server(%p); port: %ld", info, port);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        ERROR("Couldn't open UDP socket: %s", strerror(errno));
        return NULL;
    }

    LOG("Opened UDP socket (fd: %d)", sock);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    ret = bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret == -1) {
        ERROR("Couldn't bind UDP socket: %s", strerror(errno));
        close(sock);
        return NULL;
    }

    LOG("UDP socket bound to port: %ld", port);

    // TODO: handle chunked datagrams?
    // It's arguably difficult (if not impossible) with UDP.
    while (true) {
        memset(&fake_connection_state, 0, sizeof(connection_state_t));

        struct sockaddr_in src_addr;
        socklen_t src_addr_len = sizeof(src_addr);
        ssize_t len;
        char buff[MAX_MESSAGE_SIZE];

        len = recvfrom(sock, buff, sizeof(buff) - 1, 0,
                       (struct sockaddr*)&src_addr, &src_addr_len);
        if (len == -1) {
            WARN("Ignoring UDP datagram due to error: %s", strerror(errno));
            continue;
        }

        buff[len] = 0;

        LOG("udp: Received %zu bytes: \"%s\"", len, buff);

        parse_message_and_reply(sock, &fake_connection_state, buff,
                                (struct sockaddr*)&src_addr, src_addr_len);

        if (len == -1) {
            WARN("UDP response lost: %s", strerror(errno));
            continue;
        }
    }

    LOG("UDP server closing");
    close(sock);
    return NULL;
}

void cleanup_global_data() {
    vector_destroy(&GLOBAL_DATA.events);
    vector_destroy(&GLOBAL_DATA.users);
    vector_destroy(&GLOBAL_DATA.assistances);
    vector_destroy(&GLOBAL_DATA.invitations);
}

int main(int argc, char** argv) {
    int i;
    long port = 8000;
    int thread_creation_status;
    bool daemonize = false;
    const char* events_src_filename = "etc/events.txt";
    const char* assistances_src_filename = "etc/assistances.txt";
    const char* invitations_src_filename = "etc/invitations.txt";
    const char* users_src_filename = "etc/users.txt";

    pthread_t tcp_thread;
    pthread_t udp_thread;

    LOGGER_CONFIG.log_file = stderr;

    for (i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            show_usage(argc, argv);
            return 1;
        } else if (strcmp(argv[i], "-v") == 0 ||
                   strcmp(argv[i], "--verbose") == 0) {
            LOGGER_CONFIG.verbose = true;
        } else if (strcmp(argv[i], "-p") == 0 ||
                   strcmp(argv[i], "--port") == 0) {
            ++i;
            if (i == argc)
                FATAL("The %s option needs a value", argv[i - 1]);
            if (!read_long(argv[i], &port))
                WARN("Using default port %ld", port);
        } else if (strcmp(argv[i], "-d") == 0 ||
                   strcmp(argv[i], "--daemonize") == 0) {
            daemonize = true;
        } else if (strcmp(argv[i], "-e") == 0 ||
                   strcmp(argv[i], "--events") == 0) {
            ++i;
            if (i == argc)
                FATAL("The %s option needs a value", argv[i - 1]);
            events_src_filename = argv[i];
        } else if (strcmp(argv[i], "-a") == 0 ||
                   strcmp(argv[i], "--assistances") == 0) {
            ++i;
            if (i == argc)
                FATAL("The %s option needs a value", argv[i - 1]);
            assistances_src_filename = argv[i];
        } else if (strcmp(argv[i], "-u") == 0 ||
                   strcmp(argv[i], "--users") == 0) {
            ++i;
            if (i == argc)
                FATAL("The %s option needs a value", argv[i - 1]);
            users_src_filename = argv[i];
        } else if (strcmp(argv[i], "-i") == 0 ||
                   strcmp(argv[i], "--invitations") == 0) {
            ++i;
            if (i == argc)
                FATAL("The %s option needs a value", argv[i - 1]);
            invitations_src_filename = argv[i];
        } else {
            WARN("Unrecognized option: %s", argv[i]);
        }
    }

    LOG("Events source: %s", events_src_filename);
    if (!get_events_from(events_src_filename))
        FATAL("Couldn't get event data");

    LOG("User source: %s", invitations_src_filename);
    if (!get_users_from(users_src_filename))
        FATAL("Couldn't get user data");

    LOG("Invitation source: %s", invitations_src_filename);
    if (!get_invitations_from(invitations_src_filename))
        FATAL("Couldn't get invitation data");

    LOG("Assistances source: %s", assistances_src_filename);
    if (!get_assistances_from(assistances_src_filename))
        FATAL("Couldn't get assistance data");

    LOG("Starting server on port %ld, daemon: %d", port, daemonize ? 1 : 0);

    // TODO: Some kind of lockfile?
    if (daemonize) {
        if (signal(SIGCHLD, daemonize_sig_handler) == SIG_ERR)
            FATAL("Error registering SIGCHLD: %s", strerror(errno));

        if (signal(SIGALRM, daemonize_sig_handler) == SIG_ERR)
            FATAL("Error registering SIGALRM: %s", strerror(errno));

        pid_t child_pid;
        switch ((child_pid = fork())) {
            case 0:
                if (signal(SIGHUP, SIG_IGN) == SIG_ERR)
                    FATAL("Error ignoring SIGHUP: %s", strerror(errno));

                if (signal(SIGCHLD, SIG_IGN) == SIG_ERR)
                    FATAL("Error ignoring SIGCHLD: %s", strerror(errno));

                break;
            case -1:
                FATAL("Fork error: %s", strerror(errno));
                break;
            default:
                // NOTE: this is an educated guess, and does not guarantee
                // that the server is fine. We assume that if after two seconds
                // it's fine, it will be fine forever, but this does not have to
                // be true.
                alarm(2);
                waitpid(child_pid, NULL, 0); // Either SIGCHLD (if the daemon
                                             // dies) or SIGALRM will arrive
                return 0;
        }
    }

    // NOTE: Passing a pointer to a variable on the stack is unsafe if `main()`
    // does not live long enough. It does though, so...
    thread_creation_status =
        pthread_create(&tcp_thread, NULL, start_tcp_server, &port);
    if (thread_creation_status != 0)
        FATAL("Couldn't create TCP server thread: %s",
              strerror(thread_creation_status));

    LOG("TCP server thread created: %ld", tcp_thread);

    thread_creation_status =
        pthread_create(&udp_thread, NULL, start_udp_server, &port);
    if (thread_creation_status != 0)
        FATAL("Couldn't create UDP server thread: %s",
              strerror(thread_creation_status));

    LOG("UDP server thread created: %ld", udp_thread);

    atexit(cleanup_global_data);

    pthread_join(tcp_thread, NULL);
    pthread_join(udp_thread, NULL);

    LOG("Seems like all servers went down, exiting...");
    return 0;
}
