/**
 * parsing.h:
 *   Parsing utils
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
#ifndef PARSING_H
#define PARSING_H

#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include <time.h>
#include <string.h>
#include <ctype.h>

bool parse_date(const char** cursor, struct tm* tm);

bool try_consume(const char** in_source, const char* text_to_match);

bool try_consume_until(const char** current_cursor, char separator, char* buff,
                       size_t max_len);

/// Converts a string to a long.
/// Returns true on success and false on failure.
///
/// On failure the number pointed to by result is guaranteed to not change
bool read_long(const char* str, long* result);

#endif
