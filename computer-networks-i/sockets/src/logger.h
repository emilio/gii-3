#ifndef LOGGER_H
#define LOGGER_H

/// This structs represents global-level config
/// It should not be touched out of the main thread
///
/// A `logger` abstraction could be fine to encapsulate
/// this.
struct {
    bool verbose;
    FILE* log_file;
} LOGGER_CONFIG = {false, NULL};

#define LOG(fmt, ...)                                                          \
    do {                                                                       \
        if (LOGGER_CONFIG.verbose && LOGGER_CONFIG.log_file)                   \
            fprintf(LOGGER_CONFIG.log_file, "log: " fmt "\n", __VA_ARGS__);    \
    } while (0)

#define WARN(fmt, ...)                                                         \
    do {                                                                       \
        if (LOGGER_CONFIG.log_file)                                            \
            fprintf(LOGGER_CONFIG.log_file, "warn: " fmt "\n", __VA_ARGS__);   \
    } while (0)

#define FATAL(fmt, ...)                                                        \
    do {                                                                       \
        if (LOGGER_CONFIG.log_file)                                            \
            fprintf(LOGGER_CONFIG.log_file, "fatal: " fmt "\n", __VA_ARGS__);  \
        exit(1);                                                               \
    } while (0)

#endif
