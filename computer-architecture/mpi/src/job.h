#ifndef JOB_H
#define JOB_H

#include <cstring>
#include "mympi_traits.h"
#include "permutations.h"

typedef char crypt_password_t[13];

enum class JobType { DECRYPT, END, };

class Job {
    JobType m_type;

protected:
    explicit constexpr Job(JobType type): m_type(type) {}

public:
    explicit constexpr Job(): Job(JobType::END) {}
    JobType type() { return m_type; } };

class DecryptJob: public Job {
    uint32_t m_len;
    size_t m_initial_permutation;
    // TODO: check for overflow
    size_t m_permutations;
    crypt_password_t m_encrypted;
public:
    explicit DecryptJob(uint32_t len,
                        size_t initial_permutation,
                        size_t permutations,
                        const char* encrypted)
        : Job(JobType::DECRYPT),
          m_len(len),
          m_initial_permutation(initial_permutation),
          m_permutations(permutations) {
        ::memcpy(m_encrypted, encrypted, sizeof(crypt_password_t));
    }

    uint32_t length()            const { return m_len; }
    size_t initial_permutation() const { return m_initial_permutation; }
    size_t permutations()        const { return m_permutations; }
    void fill_salt(char* salt)   const {
        salt[0] = m_encrypted[0];
        salt[1] = m_encrypted[1];
    }
    bool password_is(const char* pass) const { return strcmp(pass, m_encrypted) == 0; }

    static bool check_password(const char* encrypted) {
        return strlen(encrypted) == sizeof(crypt_password_t);
    }
};

class EndJob: public Job {
    explicit constexpr EndJob(): Job(JobType::END) {}
};

namespace mympi {
template<>
struct mympi_traits<Job> {
    enum { length_multiplier = sizeof(Job) };
    const MPI_Datatype mpi_data_type = MPI_INT8_T;
};
} // namespace mympi

#endif
