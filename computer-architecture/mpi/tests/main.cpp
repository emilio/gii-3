#include <mpi.h>
#include <iostream>
#include <assert.h>

#include "mympi.h"

using mympi::Channel;

int main(int argc, char** argv) {
    int process_count;
    int process_id;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

    assert(process_count > 1 && "Need more than 1 process"
                                "to test channels");


    if (process_id == 0) {
        std::cout << "Testing with process count = " << process_count << std::endl;
        for (int i = 1; i < process_count; ++i) {
            Channel<int32_t> chan(i);
            chan.send_one(9);
            chan.send(std::vector<int32_t>{1, 2, 3, 4});
        }
    } else {
        Channel<int32_t> chan(0);
        int32_t received = chan.recv_one();
        assert(received == 9);

        std::cout << process_id << " received 1 element: " << received << std::endl;

        std::vector<int32_t> vec = chan.recv(4);
        assert(vec.size() == 4);
        assert(vec[0] == 1);

        int size = (int)vec.size();
        for (int i = 0; i < size; ++i)
            assert(vec[i] == i + 1);
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

    if (process_id == 0)
        std::cout << "Everything seems ok!" << std::endl;

    MPI_Finalize();

    return 0;
}
