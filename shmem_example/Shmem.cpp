#include "Shmem.hpp"

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
#include <stdexcept>

size_t Shmem::page_align(size_t size) {
    static const long page_size = sysconf(_SC_PAGE_SIZE);
    if (page_size == -1) {
        throw std::runtime_error("sysconf: " + std::string(strerror(errno)));
    }
    return (size + static_cast<size_t>(page_size) - 1) & ~(static_cast<size_t>(page_size) - 1);
}

void Shmem::attach(bool create, int body_proto) {
    int res;
    try {
        if ((_fd = shm_open(_shmem_name.c_str(), O_RDWR | O_CREAT, 0666)) == -1)
            throw std::runtime_error("open: " + std::string(strerror(errno)));
        if (ftruncate(_fd, static_cast<__off_t>(_header_size + _body_size)) == -1)
            throw std::runtime_error("ftruncate: " + std::string(strerror(errno)));
        if ((_header = mmap(nullptr, _header_size, PROT_READ | PROT_WRITE, MAP_SHARED, _fd, 0)) == MAP_FAILED)
            throw std::runtime_error("mmap: " + std::string(strerror(errno)));
        if ((_body = mmap(nullptr, _body_size, body_proto, MAP_SHARED, _fd, _header_size)) == MAP_FAILED)
            throw std::runtime_error("mmap: " + std::string(strerror(errno)));
    } catch (...) {
        detach();
        std::rethrow_exception(std::current_exception());
    }
}

void Shmem::detach() {
    if (_header) {
        munmap(_header, _header_size);
        _header = nullptr;
    }
    if (_body) {
        munmap(_body, _body_size);
        _body = nullptr;
    }
    if (_fd != -1) {
        close(_fd);
        _fd = -1;
    }
}
