#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SRC_LEN 3
#define DEST_LEN 6

// This makes the table for the block code that converts words:
//
// abc -> abcxyz
//
// where:
//   x = a + b
//   y = a + c
//   z = b + c

void print_bin(uint8_t what) {
    size_t bits = 8 * sizeof(what);
    for (size_t i = 0; i < bits; ++i) {
        printf("%d", (what & (1 << (bits - i - 1))) ? 1 : 0);
    }
}

int main() {
    uint8_t max = 1 << SRC_LEN;

    for (uint8_t i = 0; i < max; ++i) {
        print_bin(i);
        uint8_t a = i & (1 << 2) ? 1 : 0;
        uint8_t b = i & (1 << 1) ? 1 : 0;
        uint8_t c = i & 1;
        printf(" -> ");
        uint8_t converted = i << 3 | ((a ^ b) << 2) | ((a ^ c) << 1) | (b ^ c);
        print_bin(converted);
        printf("\n");
    }

    return 0;
}
