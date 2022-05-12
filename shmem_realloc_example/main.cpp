#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <iostream>

using namespace std;

size_t page_align(size_t size) {
    static const long page_size = sysconf(_SC_PAGE_SIZE);
    if (page_size == -1) {
        throw std::runtime_error("sysconf: " + std::string(strerror(errno)));
    }
    return (size + static_cast<size_t>(page_size) - 1) & ~(static_cast<size_t>(page_size) - 1);
}

void *shmem_allocate(void *addr, size_t &size, int proto = PROT_READ | PROT_WRITE) {
    size = page_align(size);
    if (mprotect(addr, size, proto) == -1)
        throw std::runtime_error("mprotect: " + std::string(strerror(errno)));
    return addr;
}

void inc_pointer_value(void *ptr) {
    int *int_ptr = static_cast<int *>(ptr);
    cout << "Data address is " << hex << int_ptr << dec << endl;
    cout << "Read value before increment: " << *int_ptr << endl;
    *int_ptr = *int_ptr + 1;
    cout << "Read value after increment: " << *int_ptr << endl;
}

int main() {
    const size_t shmem_size = 10ULL * 1024 * 1024 * 1024 * 1024; /* 10Tb - it's unlikely you have that much memory */
    const size_t data_offset = 8ULL * 1024 * 1024 * 1024 * 1024; /* 8Tb - offset of our data in buffer */
    void *addr = nullptr;
    int  fd = -1, res;
    try {
        fd = shm_open("shmem_file", O_RDWR | O_CREAT, 0666);
        if (fd == -1)
            throw std::runtime_error("open: " + std::string(strerror(errno)));
        res = ftruncate(fd, static_cast<__off_t>(shmem_size));
        if (res == -1) {
            throw std::runtime_error("ftruncate: " + std::string(strerror(errno)));
        }
        void *addr = mmap(nullptr, shmem_size, PROT_NONE, MAP_SHARED | MAP_NORESERVE, fd, 0);
        if (addr == MAP_FAILED) {
            throw std::runtime_error("mmap: " + std::string(strerror(errno)));
        }
        cout << "Address is " << hex << addr << dec << endl;
        char *data = static_cast<char *>(addr);
        data += data_offset;

#if 0
        // Will generate SIGSEGV because we didn't allocate memory (didn't change memory protect mode)
        inc_pointer_value(data);
#endif

        size_t size1 = 4096;
        shmem_allocate(data, size1);

        inc_pointer_value(data + size1 - sizeof(int));

#if 0
        // Will generate SIGSEGV because out pointer after allocated buffer
        inc_pointer_value(data + size1);
#endif
        size_t size2 = 8192;
        shmem_allocate(data, size2);
        // Now we allocate 8K memory
        inc_pointer_value(data + size1);

        munmap(addr, shmem_size);
        close(fd);
    } catch (std::exception &e) {
        if (addr)
            munmap(addr, shmem_size);
        if (fd != -1)
            close(fd);
        cout << e.what() << endl;
    }

//    cout << "Press any key..." << endl;
//    getchar();

    return EXIT_SUCCESS;
}
