#ifndef PERMUTATIONS_H
#define PERMUTATIONS_H

#include <cassert>

constexpr const char CRYPT_CHARS[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
    'u', 'v', 'w', 'x', 'y', 'z',

    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'
};
const size_t CRYPT_SIZE = sizeof(CRYPT_CHARS);

template <size_t N, typename T, typename IndexType, const T Dict[N]>
class Permutator {
public:
    enum { dict_size = N };
    typedef T value_type;
    typedef IndexType index_type;

    static constexpr size_t max_permutations(size_t len) {
        return len == 1 ? N : N * max_permutations(len - 1);
    }

    static void get_permutation(IndexType* indices, size_t len, size_t permutation) {
        while (len--) {
            indices[len] = permutation % N;
            permutation /= N;
        }
    }

    static void next_permutation(IndexType* indices, size_t len) {
        while (len--) {
            if (indices[len] < N) {
                indices[len]++;
                return;
            }

            indices[len] = 0;
        }
    }

    static void fill_permutation(T* permutation, IndexType* indices, size_t len) {
        for (size_t i = 0; i < len; ++i) {
            assert(indices[i] < N);
            permutation[i] = Dict[indices[i]];
        }
    }

    std::vector<IndexType> m_indices;
    std::vector<T> m_current;
    size_t m_len;

    explicit Permutator(size_t len, size_t initial_permutation): m_len(len) {
        m_indices.resize(len);
        m_current.resize(len + 1);
        m_current[len] = 0; // TODO: This is a hack
        get_permutation(m_indices.data(), len, initial_permutation);
        fill_permutation(m_current.data(), m_indices.data(), len);
    }

    explicit Permutator(size_t len): Permutator(len, 0) {}

    Permutator& operator++() {
        next_permutation(m_indices.data(), m_len);
        fill_permutation(m_current.data(), m_indices.data(), m_len);
        return *this;
    }

    T* operator*() { return m_current.data(); }
};

typedef Permutator<CRYPT_SIZE, char, uint8_t, CRYPT_CHARS> CryptPermutator;

#endif
