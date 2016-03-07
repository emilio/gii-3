#ifndef MYMPI_H
#define MYMPI_H

#include <vector>
#include <mpi.h>
#include "mympi_traits.h"

namespace mympi {
// This is a custom MPI wrapper in order to accomplish the practice more
// easily.
//
// I haven't dived in MPI for C++, but it seems like a *really* lightweight
// wrapper.
template <typename T>
class Channel {
    private:
    int m_to;
    MPI_Comm m_comm;
    int m_my_id;

    public:
    explicit Channel(int to, MPI_Comm comm): m_to(to), m_comm(comm) {
        MPI_Comm_rank(m_comm, &m_my_id);
    }

    explicit Channel(int to): Channel(to, MPI_COMM_WORLD) {}

    void send(const T* data, size_t len, int tag) {
        mympi_traits<T> traits_instance;
        MPI_Send(const_cast<T*>(data),
                 len,
                 traits_instance.mpi_data_type,
                 m_to, tag, m_comm);
    }

    void send(const std::vector<T>& vec, int tag) {
        send(vec.data(), vec.size(), tag);
    }

    template<size_t N>
    void send(const std::array<T, N>& ary, int tag) {
        send(ary.data(), ary.size(), tag);
    }

    void send(const T* data, size_t len) {
        send(data, len, 0);
    }

    void send(const std::vector<T>& vec) {
        send(vec, 0);
    }

    template<size_t N>
    void send(const std::array<T, N>& ary) {
        send(ary, 0);
    }

    void send_one(const T& element, int tag) {
        mympi_traits<T> traits_instance;
        MPI_Send(const_cast<T*>(&element), 1, traits_instance.mpi_data_type, m_to, tag, m_comm);
    }

    void send_one(const T& element) {
        send_one(element, 0);
    }

    std::vector<T> recv(size_t count, int tag, MPI_Status* status) {
        std::vector<T> ret;
        ret.resize(count);

        mympi_traits<T> traits_instance;

        MPI_Recv(ret.data(),
                 count,
                 traits_instance.mpi_data_type,
                 m_to,
                 tag,
                 m_comm,
                 status);

        return ret;
    }

    std::vector<T> recv(size_t count, MPI_Status* status) {
        return recv(count, 0, status);
    }

    std::vector<T> recv(size_t count, int tag) {
        return recv(count, tag, MPI_STATUS_IGNORE);
    }

    std::vector<T> recv(size_t count) {
        return recv(count, 0);
    }

    T recv_one(int tag, MPI_Status* status) {
        T ret;
        mympi_traits<T> traits_instance;

        MPI_Recv(&ret,
                 1,
                 traits_instance.mpi_data_type,
                 m_to, tag,
                 m_comm,
                 status);

        return ret;
    }

    T recv_one(MPI_Status* status) {
        return recv_one(0, status);
    }

    T recv_one(int tag) {
        return recv_one(tag, MPI_STATUS_IGNORE);
    }

    T recv_one() {
        return recv_one(0);
    }
};

} // namespace mympi

#endif
