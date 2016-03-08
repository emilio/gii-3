#ifndef MYMPI_TRAITS_H
#define MYMPI_TRAITS_H

namespace mympi {

// NOTE: we can't use `const static` because MPI_FLOAT is really a pointer to a
// global struct in OpenMPI :/
//
// That means that we must instance the traits (which is at least weird).
//
// I think the instancing should be optimised away though.

template <typename T>
struct mympi_traits;

template<>
struct mympi_traits<float> {
    const MPI_Datatype mpi_data_type = MPI_FLOAT;
};

template<>
struct mympi_traits<double> {
    const MPI_Datatype mpi_data_type = MPI_DOUBLE;
};

template<>
struct mympi_traits<long double> {
    const MPI_Datatype mpi_data_type = MPI_LONG_DOUBLE;
};

template<>
struct mympi_traits<bool> {
    const MPI_Datatype mpi_data_type = MPI_C_BOOL;
};

template<>
struct mympi_traits<int8_t> {
    const MPI_Datatype mpi_data_type = MPI_INT8_T;
};

template<>
struct mympi_traits<int16_t> {
    const MPI_Datatype mpi_data_type = MPI_INT16_T;
};

template<>
struct mympi_traits<int32_t> {
    const MPI_Datatype mpi_data_type = MPI_INT32_T;
};

template<>
struct mympi_traits<int64_t> {
    const MPI_Datatype mpi_data_type = MPI_INT64_T;
};

template<>
struct mympi_traits<uint8_t> {
    const MPI_Datatype mpi_data_type = MPI_UINT8_T;
};

template<>
struct mympi_traits<uint16_t> {
    const MPI_Datatype mpi_data_type = MPI_UINT16_T;
};

template<>
struct mympi_traits<uint32_t> {
    const MPI_Datatype mpi_data_type = MPI_UINT32_T;
};

template<>
struct mympi_traits<uint64_t> {
    const MPI_Datatype mpi_data_type = MPI_UINT64_T;
};

#pragma GCC diagnostic pop

} // Namespace mympi

#endif
