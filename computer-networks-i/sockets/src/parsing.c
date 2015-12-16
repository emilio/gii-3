/**
 * parsing.c:
 *   Parsing utils implementation
 *
 * Copyright (C) 2015 Emilio Cobos Álvarez <emiliocobos@usal.es>
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
#include "parsing.h"

bool parse_date(const char** cursor, struct tm* tm) {
    static const char* formats_to_try[] = {
        "%d-%m-%Y %H:%M:%S", // day-month-year hours:minutes:seconds
        "%d/%m/%Y %H:%M:%S", // day/month/year hours:minutes:seconds
        "%Y-%m-%d %H:%M:%S", // Year-month-day hours:minutes:seconds
        "%Y/%m/%d %H:%M:%S", // Year/month/day hours:minutes:seconds
        "%d-%m-%Y %H:%M",    // day-month-year hours:minutes
        "%d/%m/%Y %H:%M",    // day/month/year hours:minutes
        "%Y-%m-%d %H:%M",    // Year-month-day hours:minutes
        "%Y/%m/%d %H:%M",    // Year/month/day hours:minutes
        "%d-%m-%Y",          // day-month-year
        "%d/%m/%Y",          // day/month/year
        "%Y-%m-%d",          // ISO date format
        "%Y/%m/%d",          // Year/month/day
        "%c",                // Locale date
        NULL,
    };

    const char* input = *cursor;
    const char* result;

    memset(tm, 0, sizeof(*tm));

    const char** format = formats_to_try;

    while (*format) {
        result = strptime(input, *format, tm);
        if (result) {
            *cursor = result;
            return true;
        }
        format++;
    }

    return false;
}

bool try_consume(const char** in_source, const char* text_to_match) {
    while (*text_to_match) {
        if (!**in_source) // the source string ended
            return false;

        if (*text_to_match++ != **in_source)
            return false;

        (*in_source)++;
    }

    return true;
}

bool try_consume_until(const char** current_cursor, char separator, char* buff,
                       size_t max_len) {
    size_t len = 0;
    while (**current_cursor && **current_cursor != separator) {
        if (len++ == max_len)
            return false;

        *buff++ = *(*current_cursor)++;
    }

    *buff = '\0';

    return len != 0;
}

bool read_long(const char* str, long* result) {
    char* endptr;
    long temp_res = strtol(str, &endptr, 0);

    if (endptr == str || errno == ERANGE)
        return false;

    *result = temp_res;

    return true;
}
