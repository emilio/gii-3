#include "protocol.h"
#include <stdbool.h>
#include <ctype.h>

const char* PARSE_ERROR_MESSAGES[] = {
    "No error",                   // ERROR_NONE
    "No message received",        // ERROR_NO_MESSAGE
    "Expected separator",         // ERROR_EXPECTED_SEPARATOR
    "Expected valid description", // ERROR_EXPECTED_VALID_DESCRIPTION
    "Invalid message type",       // ERROR_INVALID_MESSAGE_TYPE
    "Expected valid user id",     // ERROR_EXPECTED_VALID_UID
    "Expected valid event id",    // ERROR_EXPECTED_VALID_EVENT_ID
    "Expected valid date",        // ERROR_EXPECTED_VALID_DATE
    "Unexpected extra content",   // ERROR_UNEXPECTED_CONTENT
    "Unknown error",              // ERROR_UNKNOWN
};

const char* MESSAGE_TYPE_STRINGS[] = {
    "",               // Unused
    "HOLA",           // MESSAGE_TYPE_HELLO
    "ADIOS",          // MESSAGE_TYPE_BYE
    "LISTAR EVENTOS", // MESSAGE_TYPE_EVENT_LIS
    "LISTAR",         // MESSAGE_TYPE_ASSISTANCE_LIST
    "FICHAR",         // MESSAGE_TYPE_ASSISTANCE
};

static inline bool parse_date(const char** cursor, struct tm* tm) {
    static const char* formats_to_try[] = {
        "%d-%m-%Y %H:%M", // day-month-year hours:minutes
        "%d/%m/%Y %H:%M", // day/month/year hours:minutes
        "%Y-%m-%d %H:%M", // Year-month-day hours:minutes
        "%Y/%m/%d %H:%M", // Year/month/day hours:minutes
        "%d-%m-%Y",       // day-month-year
        "%d/%m/%Y",       // day/month/year
        "%Y-%m-%d",       // ISO date format
        "%Y/%m/%d",       // Year/month/day
        "%c",             // Locale date
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

static inline bool try_consume(const char** in_source,
                               const char* text_to_match) {
    while (*text_to_match) {
        if (!**in_source) // the source string ended
            return false;

        if (*text_to_match++ != **in_source)
            return false;

        (*in_source)++;
    }

    return true;
}

/// This is dumb, wut...
static inline bool try_consume_message_type(const char** in_source,
                                            client_message_type_t* type) {
    const char* current_ptr;
    client_message_type_t current = MESSAGE_TYPE_BEGIN;

    for (; current < MESSAGE_TYPE_END; ++current) {
        current_ptr = *in_source;
        if (try_consume(&current_ptr, MESSAGE_TYPE_STRINGS[current])) {
            *in_source = current_ptr;
            *type = current;
            return true;
        }
    }

    return false;
}

static bool try_consume_until(const char** current_cursor, char separator,
                              char* buff, size_t max_len) {
    size_t len = 0;
    while (**current_cursor && **current_cursor != separator) {
        if (len++ == max_len)
            return false;

        *buff++ = *(*current_cursor)++;
    }

    return len != 0;
}

/// We consider an id anything which is not a whitespace
static bool try_consume_id(const char** current_cursor, char* id_buff,
                           size_t max_len) {
    size_t current_len = 0;

    while (**current_cursor && is_valid_id_char(**current_cursor)) {
        // If we have read too much
        if (current_len++ == max_len)
            return false;

        *id_buff++ = *(*current_cursor)++;
    }

    *id_buff = '\0';

    return current_len != 0;
}

/// NOTE: This is the dumbest parser ever...
parse_error_t parse_client_message(const char* in_source,
                                   client_message_t* message) {
    const char* cursor = in_source;

    if (message == NULL)
        return ERROR_UNKNOWN;

    if (in_source == NULL)
        return ERROR_NO_MESSAGE;

    /// First we try to consume the message type
    if (!try_consume_message_type(&cursor, &message->type))
        return ERROR_INVALID_MESSAGE_TYPE;

    /// If the message isn't over it must be followed by a space
    if (*cursor)
        if (!try_consume(&cursor, " "))
            return ERROR_INVALID_MESSAGE_TYPE;

    switch (message->type) {
        case MESSAGE_TYPE_HELLO:
        case MESSAGE_TYPE_BYE:
            if (!try_consume_id(&cursor, message->content.uid, MAX_UID_LEN))
                return ERROR_EXPECTED_VALID_UID;

            if (*cursor != '\0') // If we're not at the end something is wrong
                return ERROR_UNEXPECTED_CONTENT;
            break;

        case MESSAGE_TYPE_EVENT_LIST:
            if (*cursor != '\0')
                return ERROR_UNEXPECTED_CONTENT;
            break;

        case MESSAGE_TYPE_ASSISTANCE_LIST:
            if (!try_consume_id(&cursor,
                                message->content.assistance_list_info.uid,
                                MAX_UID_LEN))
                return ERROR_EXPECTED_VALID_UID;

            if (!try_consume(&cursor, " "))
                return ERROR_EXPECTED_VALID_EVENT_ID;

            if (!try_consume_id(&cursor,
                                message->content.assistance_list_info.event_id,
                                MAX_EVENT_ID_LEN))
                return ERROR_EXPECTED_VALID_EVENT_ID;

            if (*cursor != '\0')
                return ERROR_UNEXPECTED_CONTENT;
            break;

        case MESSAGE_TYPE_ASSISTANCE:
            if (!try_consume_id(&cursor,
                                message->content.assistance_info.event_id,
                                MAX_EVENT_ID_LEN))
                return ERROR_EXPECTED_VALID_EVENT_ID;

            if (!try_consume(&cursor, " "))
                return ERROR_EXPECTED_VALID_UID;

            if (!try_consume_id(&cursor, message->content.assistance_info.uid,
                                MAX_UID_LEN))
                return ERROR_EXPECTED_VALID_UID;

            if (!try_consume(&cursor, " "))
                return ERROR_EXPECTED_VALID_DATE;

            if (!parse_date(&cursor,
                            &message->content.assistance_info.datetime))
                return ERROR_EXPECTED_VALID_DATE;

            if (*cursor != '\0')
                return ERROR_UNEXPECTED_CONTENT;
            break;

        default:
            return ERROR_UNKNOWN;
    }

    return ERROR_NONE;
}

parse_error_t parse_event(const char* in_source, protocol_event_t* event) {
    const char* cursor = in_source;

    if (event == NULL)
        return ERROR_UNKNOWN;

    if (in_source == NULL)
        return ERROR_NO_EVENT;

    if (!try_consume_id(&cursor, event->id, MAX_EVENT_ID_LEN))
        return ERROR_EXPECTED_VALID_EVENT_ID;

    if (!try_consume(&cursor, "#"))
        return ERROR_EXPECTED_SEPARATOR;

    if (!try_consume_until(&cursor, '#', event->description,
                           MAX_EVENT_DESCRIPTION_LEN))
        return ERROR_EXPECTED_VALID_DESCRIPTION;

    if (!try_consume(&cursor, "#"))
        return ERROR_EXPECTED_SEPARATOR;

    if (!parse_date(&cursor, &event->starts_at))
        return ERROR_EXPECTED_VALID_DATE;

    if (!try_consume(&cursor, "#"))
        return ERROR_EXPECTED_SEPARATOR;

    if (!parse_date(&cursor, &event->ends_at))
        return ERROR_EXPECTED_VALID_DATE;

    return ERROR_NONE;
}
