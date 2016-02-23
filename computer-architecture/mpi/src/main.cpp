#include <mpi.h>
#include <vector>
#include <iostream>

#include "mympi.h"

int main(int argc, char** argv) {
    int process_count;
    int process_id;
    char name[MPI_MAX_PROCESSOR_NAME];
    int name_length;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
    MPI_Get_processor_name(name, &name_length);

    switch (process_id) {
    case 0:
        std::cout << "Process 0 sending data to "
                  << process_count - 1 << " processes" << std::endl;
        for (int i = 1; i < process_count; ++i) {
            mympi::Channel<int32_t> chan(i);
            chan.send_one(4);
        }
        break;
    default:
        mympi::Channel<int32_t> chan(0);
        int32_t result = chan.recv_one();
        std::cout << "Process " << process_id << " received " << result
                  << std::endl;
    }

    MPI_Finalize();

    return 0;
}
