#ifndef CONNECTION_STATE_H
#define CONNECTION_STATE_H

#include <stdbool.h>
#include "protocol.h"

typedef struct connection_state {
    protocol_uid_t uid;
    bool logged_in;
} connection_state_t;

void connection_state_init(connection_state_t* state);

#endif
