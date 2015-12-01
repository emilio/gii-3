#ifndef LOGGER_H
#define LOGGER_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

/// A `logger` abstraction could be fine to encapsulate and reuse this.
static struct {
    bool verbose;
    FILE* log_file;
    pthread_mutex_t mutex;
} LOGGER_CONFIG = {false, NULL, PTHREAD_MUTEX_INITIALIZER};

#define LOG(...)                                                               \
    do {                                                                       \
        pthread_mutex_lock(&LOGGER_CONFIG.mutex);                              \
        if (LOGGER_CONFIG.verbose && LOGGER_CONFIG.log_file) {                 \
            fprintf(LOGGER_CONFIG.log_file, "log: " __VA_ARGS__);              \
            fprintf(LOGGER_CONFIG.log_file, "\n");                             \
        }                                                                      \
        pthread_mutex_unlock(&LOGGER_CONFIG.mutex);                            \
    } while (0)

#define WARN(...)                                                              \
    do {                                                                       \
        pthread_mutex_lock(&LOGGER_CONFIG.mutex);                              \
        if (LOGGER_CONFIG.log_file) {                                          \
            fprintf(LOGGER_CONFIG.log_file, "warn: " __VA_ARGS__);             \
            fprintf(LOGGER_CONFIG.log_file, "\n");                             \
        }                                                                      \
        pthread_mutex_unlock(&LOGGER_CONFIG.mutex);                            \
    } while (0)

#define ERROR(...)                                                             \
    do {                                                                       \
        pthread_mutex_lock(&LOGGER_CONFIG.mutex);                              \
        if (LOGGER_CONFIG.log_file) {                                          \
            fprintf(LOGGER_CONFIG.log_file, "error: " __VA_ARGS__);            \
            fprintf(LOGGER_CONFIG.log_file, "\n");                             \
        }                                                                      \
        pthread_mutex_unlock(&LOGGER_CONFIG.mutex);                            \
    } while (0)

#define FATAL(...)                                                             \
    do {                                                                       \
        pthread_mutex_lock(&LOGGER_CONFIG.mutex);                              \
        if (LOGGER_CONFIG.log_file) {                                          \
            fprintf(LOGGER_CONFIG.log_file, "error: " __VA_ARGS__);            \
            fprintf(LOGGER_CONFIG.log_file, "\n");                             \
        }                                                                      \
        pthread_mutex_unlock(&LOGGER_CONFIG.mutex);                            \
        exit(1);                                                               \
    } while (0)

#endif
