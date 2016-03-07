#ifndef JOB_H
#define JOB_H

#include <cstring>
#include <mutex>
#include "mympi.h"
#include "permutations.h"

typedef char crypt_password_t[13];

enum class JobType: uint8_t {
    DECRYPT,
    END,
};

class DecryptJob;
class EndJob;

class Job {
    JobType m_type;

protected:
    explicit constexpr Job(JobType type): m_type(type) {}

public:
    typedef uint8_t encoded_type;
    static constexpr const size_t DECRYPTION_JOB_ENCODED_SIZE = sizeof(uint8_t) +
                                                                sizeof(uint32_t) +
                                                                sizeof(size_t) * 2 +
                                                                sizeof(crypt_password_t);
    static constexpr const size_t ENCODED_SIZE = DECRYPTION_JOB_ENCODED_SIZE;
    static void encode_and_send_decrypt_job(const DecryptJob& job,
                                            mympi::Channel<encoded_type>& chan);
    static DecryptJob receive_and_decode_decrypt_job(const std::vector<uint8_t>& elements);

    static void encode_and_send(const Job& job, mympi::Channel<encoded_type>& chan);

    static Job receive_and_decode(mympi::Channel<encoded_type>& chan);

    explicit constexpr Job(): Job(JobType::END) {}
    JobType type() const { return m_type; }
};

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

    void fill_password(char* out) const {
        ::memcpy(out, m_encrypted, sizeof(crypt_password_t));
    }

    bool password_is(const char* pass) const { return strcmp(pass, m_encrypted) == 0; }

    static bool check_password(const char* encrypted) {
        return strlen(encrypted) == sizeof(crypt_password_t);
    }
};

class EndJob: public Job {
public:
    explicit constexpr EndJob(): Job(JobType::END) {}
};

#endif
