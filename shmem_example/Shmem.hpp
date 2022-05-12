#pragma once

#include <string>
#include <sys/mman.h>

class Shmem {
protected:
    std::string _shmem_name;
    size_t      _header_size;
    size_t      _body_size;
    int         _fd;
    void        *_header;
    void        *_body;
public:
    Shmem(std::string shmem_name = std::string("shmem_file"), size_t header_size = 1024 * 1024,
          size_t body_size = 10 * 1024 * 1024)
        : _shmem_name(shmem_name), _header_size(page_align(header_size)), _body_size(page_align(body_size)),
          _fd(-1), _header(nullptr), _body(nullptr) {}
    ~Shmem() { detach(); }

    static size_t page_align(size_t size);
    void attach(bool create = false, int body_proto = PROT_READ | PROT_WRITE);
    void detach();
    inline void *get_header_ptr() const {
        return _header;
    }
    inline void *get_body_ptr() const {
        return _body;
    }
};
