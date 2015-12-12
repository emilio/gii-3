#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include <time.h>
#include <string.h>
#include <ctype.h>

bool parse_date(const char** cursor, struct tm* tm);

bool try_consume(const char** in_source,
                               const char* text_to_match);

bool try_consume_until(const char** current_cursor, char separator,
                              char* buff, size_t max_len);

/// Converts a string to a long.
/// Returns true on success and false on failure.
///
/// On failure the number pointed to by result is guaranteed to not change
bool read_long(const char* str, long* result);

#endif
