/**
 * protocol.c:
 *   Protocol parsing and connection implementation.
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
#include "protocol.h"
#include "parsing.h"

void connection_state_init(connection_state_t* state) {
    state->logged_in = false;
    state->uid[0] = '\0';
}

const char* PARSE_ERROR_MESSAGES[] = {
    "No error",                   // ERROR_NONE
    "No message received",        // ERROR_NO_MESSAGE
    "No event received",          // ERROR_NO_EVENT
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
    "HOLA",           // MESSAGE_TYPE_HELLO
    "ADIOS",          // MESSAGE_TYPE_BYE
    "LISTAR EVENTOS", // MESSAGE_TYPE_EVENT_LIS
    "LISTAR",         // MESSAGE_TYPE_ASSISTANCE_LIST
    "FICHAR",         // MESSAGE_TYPE_ASSISTANCE
};

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

            if (*cursor != '\0')
                return ERROR_UNEXPECTED_CONTENT;

            // The datetime of the assistance message is always the current one,
            // This was changed during development
            time_t now = time(NULL);
            localtime_r(&now, &message->content.assistance_info.datetime);
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

    if (*cursor)
        return ERROR_UNEXPECTED_CONTENT;

    return ERROR_NONE;
}

parse_error_t parse_assistance(const char* in_source,
                               protocol_assistance_t* assistance) {
    const char* cursor = in_source;

    if (assistance == NULL)
        return ERROR_UNKNOWN;

    if (in_source == NULL)
        return ERROR_UNKNOWN;

    if (!try_consume_id(&cursor, assistance->uid, MAX_UID_LEN))
        return ERROR_EXPECTED_VALID_UID;

    if (!try_consume(&cursor, "#"))
        return ERROR_EXPECTED_SEPARATOR;

    if (!try_consume_id(&cursor, assistance->event_id, MAX_EVENT_ID_LEN))
        return ERROR_EXPECTED_VALID_EVENT_ID;

    if (!try_consume(&cursor, "#"))
        return ERROR_EXPECTED_SEPARATOR;

    if (!parse_date(&cursor, &assistance->at))
        return ERROR_EXPECTED_VALID_DATE;

    if (*cursor)
        return ERROR_UNEXPECTED_CONTENT;

    return ERROR_NONE;
}

parse_error_t parse_invitation(const char* in_source,
                               protocol_invitation_t* invitation) {
    const char* cursor = in_source;

    if (invitation == NULL)
        return ERROR_UNKNOWN;

    if (in_source == NULL)
        return ERROR_UNKNOWN;

    if (!try_consume_id(&cursor, invitation->uid, MAX_UID_LEN))
        return ERROR_EXPECTED_VALID_UID;

    if (!try_consume(&cursor, "#"))
        return ERROR_EXPECTED_SEPARATOR;

    if (!try_consume_id(&cursor, invitation->event_id, MAX_EVENT_ID_LEN))
        return ERROR_EXPECTED_VALID_EVENT_ID;

    if (*cursor)
        return ERROR_UNEXPECTED_CONTENT;

    return ERROR_NONE;
}

parse_error_t parse_user(const char* in_source, protocol_user_t* user) {
    const char* cursor = in_source;

    if (user == NULL)
        return ERROR_UNKNOWN;

    if (in_source == NULL)
        return ERROR_UNKNOWN;

    if (!try_consume_id(&cursor, user->id, MAX_UID_LEN))
        return ERROR_EXPECTED_VALID_UID;

    if (*cursor)
        return ERROR_UNEXPECTED_CONTENT;

    return ERROR_NONE;
}
