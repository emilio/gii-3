#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#define __USE_XOPEN // For strptime
#include <time.h>
#include <string.h>

/// It is mandatory that the protocol must be text based.
///
/// The client orders are:
/// HOLA user_id
///
/// TODO document the rest
///
///

/// There are some constraints like:
#define MAX_UID_LEN 20
#define MAX_EVENT_ID_LEN 20
#define MAX_DESCRIPTION_LEN 140

typedef char uid_t[MAX_UID_LEN + 1];
typedef char event_id_t[MAX_EVENT_ID_LEN + 1];

typedef uint8_t client_message_type_t;

/// Using defines we loose type checking but we gain the same
/// message structure, so if we want to send raw binary data
/// from the client it will work regardless of the architecture.
#define MESSAGE_TYPE_BEGIN MESSAGE_TYPE_HELLO
#define MESSAGE_TYPE_HELLO 1
#define MESSAGE_TYPE_BYE 2
#define MESSAGE_TYPE_EVENT_LIST 3
#define MESSAGE_TYPE_ASSISTANCE_LIST 4
#define MESSAGE_TYPE_ASSISTANCE 5
#define MESSAGE_TYPE_END 6

typedef struct client_message {
    client_message_type_t type;
    union {
        uid_t uid; // HELLO/BYE
        struct {   // ASSISTANCE_LIST
            uid_t uid;
            event_id_t event_id;
        } assistance_list_info;
        struct { // ASSISTANCE
            uid_t uid;
            event_id_t event_id;
            struct tm datetime;
        } assistance_info;
    } content;
} client_message_t;

extern const char* PARSE_ERROR_MESSAGES[];

typedef enum parse_error {
    ERROR_NONE = 0,
    ERROR_NO_MESSAGE,
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

/// Here we rely in non-portable behaviour (that time_t is an integer value).
/// It's really this way in every sane platform, but if it was not this way,
/// we should make the multiplication by hand from the fields on `struct tm`.
static inline int64_t tm_to_seconds(struct tm* tm) {
    return (int64_t)mktime(tm);
}

static inline const char* parse_error_string(parse_error_t error) {
    if (error < 0 || error >= ERROR_COUNT - 1)
        return PARSE_ERROR_MESSAGES[ERROR_UNKNOWN];

    return PARSE_ERROR_MESSAGES[error];
}

parse_error_t parse_client_message(const char* in_source,
                                   client_message_t* message);

#endif
