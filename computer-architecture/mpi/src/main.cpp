#include <mpi.h>
#include <vector>
#include <iostream>
#include <cstdlib>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <crypt.h>
#include "mympi.h"
#include "job.h"
#include "reply.h"

bool process_job(const DecryptJob& job, char* result) {
    uint32_t len = job.length();
    char salt[3] = {0};
    job.fill_salt(salt);

    get_permutation<255 - 48, 48>(reinterpret_cast<unsigned char*>(result),
                                  len, job.initial_permutation());

    size_t permutations = job.permutations();
    struct crypt_data data;
    while (permutations--) {
        next_permutation<255 - 48, 48>(reinterpret_cast<unsigned char*>(result), len);
        char* maybe_pass = crypt_r(result, salt, &data);
        if (job.password_is(maybe_pass)) {
            return true;
        }
    }

    return false;
}

int main(int argc, char** argv) {
    int process_count;
    int process_id;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

    // We don't care about the program name
    argv++;
    argc--;

    switch (process_id) {
    case 0:
        if (process_count == 1) {
            std::cout << "Process 0 is alone, brute-forcing"
                         " everything sequentially." << std::endl;
            for (int i = 0; i < argc; ++i) {
                if (!DecryptJob::check_password(argv[i])) {
                    std::cout << "Found invalid password " << argv[i]
                              << std::endl;
                    continue;
                }

                size_t len = 0;
                // Theoretically this *could* overflow
                std::vector<char> result;
                while (++len) {
                    DecryptJob job(len, 0, max_permutations(255 - 48, len), argv[i]);
                    result.resize(len + 1); // TODO: Be smarter about allocs here
                    result.data()[len] = 0;
                    if (process_job(job, result.data())) {
                        std::cout << "Found: " << result.data() << std::endl;
                        break;
                    }
                }
            }

            MPI_Finalize();
            return 0;
        }

        int workers = process_count - 1;
        std::cout << "Process 0 sending data to "
                  << workers << " processes" << std::endl;

        for (int i = 0; i < argc; ++i) {
            size_t len = 0;

            // Theoretically this *could* overflow
            while (++len) {
                size_t perms = max_permutations(255 - 48, len);
                size_t perms_per_worker = perms / workers;
                size_t current = 0;
                for (int j = 1; i < process_count; ++j) {
                    mympi::Channel<Job> chan(j);
                    chan.send_one(DecryptJob(len, current, perms_per_worker, argv[i]));
                    current += perms_per_worker;
                }
            }
        }
        break;
    default:
        mympi::Channel<Job> chan(0);
        while (true) {
            Job job_ = chan.recv();
            if (job_.type() == JobType::End)
                break;

            DecryptJob job = dynamic_cast<DecryptJob>(job_);
            std::vector<char> result;
            size_t len = job.len();
            result.resize(len + 1); // TODO: Be smarter about allocs here
            result.data()[len] = 0;
            if (process_job(job, result.data()))
                chan.send(SuccessReply(result.data()));
            else
                chan.send(FailureReply());
        }
    }

    MPI_Finalize();

    return 0;
}
