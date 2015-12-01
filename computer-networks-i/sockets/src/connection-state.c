#include "connection-state.h"

void connection_state_init(connection_state_t* state) {
    state->logged_in = false;
    state->uid[0] = '\0';
}
