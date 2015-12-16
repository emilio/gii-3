/**
 * protocol.h:
 *   Protocol interface and types
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
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

/**
 * To see the full documentation of the protocol see
 * statement.md
 */
#define MAX_UID_LEN 20
#define MAX_EVENT_ID_LEN 20
#define MAX_EVENT_DESCRIPTION_LEN 140

typedef char protocol_uid_t[MAX_UID_LEN + 1];
typedef char protocol_event_id_t[MAX_EVENT_ID_LEN + 1];
typedef char protocol_description_t[MAX_EVENT_DESCRIPTION_LEN + 1];

#define MESSAGE_TYPE_BEGIN MESSAGE_TYPE_HELLO
typedef enum client_message_type {
    MESSAGE_TYPE_HELLO,
    MESSAGE_TYPE_BYE,
    MESSAGE_TYPE_EVENT_LIST,
    MESSAGE_TYPE_ASSISTANCE_LIST,
    MESSAGE_TYPE_ASSISTANCE,
    MESSAGE_TYPE_END
} client_message_type_t;

typedef struct connection_state {
    protocol_uid_t uid;
    bool logged_in;
} connection_state_t;

typedef struct client_message {
    client_message_type_t type;
    union {
        protocol_uid_t uid; // HELLO/BYE
        struct {            // ASSISTANCE_LIST
            protocol_uid_t uid;
            protocol_event_id_t event_id;
        } assistance_list_info;
        struct { // ASSISTANCE
            protocol_uid_t uid;
            protocol_event_id_t event_id;
            struct tm datetime;
        } assistance_info;
    } content;
} client_message_t;

typedef struct protocol_event {
    protocol_event_id_t id;
    protocol_description_t description;
    struct tm starts_at;
    struct tm ends_at;
} protocol_event_t;

typedef struct protocol_assistance {
    protocol_uid_t uid;
    protocol_event_id_t event_id;
    struct tm at;
} protocol_assistance_t;

typedef struct protocol_invitation {
    protocol_uid_t uid;
    protocol_event_id_t event_id;
} protocol_invitation_t;

typedef struct protocol_user { protocol_uid_t id; } protocol_user_t;

extern const char* PARSE_ERROR_MESSAGES[];

typedef enum parse_error {
    ERROR_NONE = 0,
    ERROR_NO_MESSAGE,
    ERROR_NO_EVENT,
    ERROR_EXPECTED_SEPARATOR,
    ERROR_EXPECTED_VALID_DESCRIPTION,
    ERROR_INVALID_MESSAGE_TYPE,
    // Note that not-found error is not related to the server.
    // It's a message which was expected to have an user id.
    ERROR_EXPECTED_VALID_UID,
    ERROR_EXPECTED_VALID_EVENT_ID,
    ERROR_EXPECTED_VALID_DATE,
    ERROR_UNEXPECTED_CONTENT,
    ERROR_UNKNOWN,
    // Error count for free
    ERROR_COUNT,
} parse_error_t;

static inline const char* parse_error_string(parse_error_t error) {
    if (error < 0 || error >= ERROR_COUNT - 1)
        return PARSE_ERROR_MESSAGES[ERROR_UNKNOWN];

    return PARSE_ERROR_MESSAGES[error];
}

static inline bool is_valid_id_char(char c) {
    return isalnum(c) || c == '-' || c == '_';
}

void connection_state_init(connection_state_t* state);

parse_error_t parse_client_message(const char* in_source,
                                   client_message_t* message);

parse_error_t parse_event(const char* in_source, protocol_event_t* event);

parse_error_t parse_assistance(const char* in_source,
                               protocol_assistance_t* assistance);

parse_error_t parse_invitation(const char* in_source,
                               protocol_invitation_t* invitation);

parse_error_t parse_user(const char* in_source, protocol_user_t* user);
#endif
