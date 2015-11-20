#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

/// Converts a string to a long.
/// Returns true on success and false on failure.
///
/// On failure the number pointed to by result is guaranteed to not change
bool read_long(const char* str, long* result);

#endif
