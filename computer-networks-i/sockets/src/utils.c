#include "utils.h"

bool read_long(const char* str, long* result) {
    char* endptr;
    long temp_res = strtol(str, &endptr, 0);

    if (endptr == str || errno == ERANGE)
        return false;

    *result = temp_res;

    return true;
}
