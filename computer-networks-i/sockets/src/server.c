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

#include "logger.h"
#include "utils.h"

struct program_author {
    const char* name;
    const char* email;
};

const struct program_author AUTHORS[] = {
    {"Emilio Cobos Álvarez", "emiliocobos@usal.es"},
    {NULL, NULL}, // End of list
};

/// Shows usage of the program
void show_usage(int argc, char** argv) {
    printf("Usage: %s [options]\n", argv[0]);
    printf("Options:\n");
    printf("  -h, --help\t Display this message and exit\n");
    printf("  -p, --port [port]\t Listen to [port]\n");
    printf("  -v, --verbose\t Be verbose about what is going on\n");
    printf("\n");
    printf("Author(s):\n");

    const struct program_author* current_author = AUTHORS;
    while (current_author->name) {
        printf("  %s (<%s>)\n", current_author->name, current_author->email);
        current_author++;
    }
}

int run_server(long port) {
    // TODO
    return 0;
}

int main(int argc, char** argv) {
    int i;
    long port = 8000;
    LOGGER_CONFIG.log_file = stderr;

    for (i = 0; i < argc; ++i) {
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
        }
    }

    LOG("Starting server on port %ld", port);

    return run_server(port);
}
