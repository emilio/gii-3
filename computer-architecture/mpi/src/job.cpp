
#include "job.h"

template<typename T>
using Channel = mympi::Channel<T>;

Job Job::receive_and_decode(mympi::Channel<encoded_type>& chan) {
    std::vector<uint8_t> result = chan.recv(ENCODED_SIZE);
    switch (static_cast<JobType>(result[0])) {
    case JobType::DECRYPT:
        return receive_and_decode_decrypt_job(result);
    case JobType::END:
        return EndJob();
    default:
        assert(0 && "Invalid type received!");
    }
}

void Job::encode_and_send(const Job& job, Channel<Job::encoded_type>& chan) {
    switch (job.type()) {
    case JobType::DECRYPT:
        return encode_and_send_decrypt_job(static_cast<const DecryptJob&>(job), chan);
    case JobType::END:
        return chan.send_one(static_cast<encoded_type>(JobType::END));
    }
}

void Job::encode_and_send_decrypt_job(const DecryptJob& job, Channel<Job::encoded_type>& chan) {
    std::vector<uint8_t> elements(DECRYPTION_JOB_ENCODED_SIZE);

    uint8_t* data = elements.data();

    *data++ = static_cast<uint8_t>(job.type());

    *reinterpret_cast<uint32_t*>(data) = job.length();
    data += sizeof(uint32_t);

    *reinterpret_cast<size_t*>(data) = job.initial_permutation();
    data += sizeof(size_t);

    *reinterpret_cast<size_t*>(data) = job.permutations();
    data += sizeof(size_t);

    job.fill_password(reinterpret_cast<char*>(data));

    std::cout << "Enc, ";
    for (const uint8_t& el: elements)
        std::cout << (int)el << ", ";
    std::cout << std::endl;


    chan.send(elements);
}

DecryptJob Job::receive_and_decode_decrypt_job(const std::vector<uint8_t>& elements) {
    const uint8_t* data = elements.data();

    std::cout << "Dec, ";
    for (const uint8_t& el: elements)
        std::cout << (int) el << ", ";
    std::cout << std::endl;

    // One byte was already read
    data++;

    uint32_t len = *reinterpret_cast<const uint32_t*>(data);
    data += sizeof(uint32_t);

    size_t initial = *reinterpret_cast<const size_t*>(data);
    data += sizeof(size_t);

    size_t perms = *reinterpret_cast<const size_t*>(data);
    data += sizeof(size_t);

    crypt_password_t pass;
    ::memcpy(pass, data, sizeof(crypt_password_t));

    return DecryptJob(len, initial, perms, pass);
}
