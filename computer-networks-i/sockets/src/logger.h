/**
 * logger.h:
 *   Logger macros and declarations.
 *
 * Copyright (C) 2015 Emilio Cobos Álvarez (70912324N) <emiliocobos@usal.es>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>

/// A `logger` abstraction could be fine to encapsulate and reuse this.
extern struct logger_config {
    bool verbose;
    FILE* log_file;
    pthread_mutex_t mutex;
} LOGGER_CONFIG;

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
