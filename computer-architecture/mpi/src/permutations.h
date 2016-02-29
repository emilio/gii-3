#ifndef PERMUTATIONS_H
#define PERMUTATIONS_H

constexpr size_t max_permutations(size_t n, size_t len) {
    return len == 1 ? n : n * max_permutations(n, len - 1);
}

template <size_t N, size_t Offset>
void get_permutation(unsigned char* string,
                     size_t len,
                     size_t permutation) {
    while (len--) {
        string[len] = permutation % N + Offset;
        permutation /= N;
    }
}

template <size_t N, size_t Offset>
void next_permutation(unsigned char* string,
                      size_t len) {
    while (len--) {
        if (string[len] < N - Offset) {
            string[len]++;
            return;
        } else {
            string[len] = Offset;
        }
    }
}

#endif
