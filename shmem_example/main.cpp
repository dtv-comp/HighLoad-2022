#include "Tables.hpp"
#include "Shmem.hpp"
#include "ShmemProc.hpp"
#include "MainProc.hpp"
#include "ReaderProc.hpp"

#include <future>
#include <thread>
#include <vector>


int main() {
    std::promise<void>  exitSignal;
    std::future<void>   exitReceiver(exitSignal.get_future());
    std::vector<std::thread> threads;

    threads.push_back(std::thread(&MainProc::run, new MainProc(), std::ref(exitSignal)));
    threads.push_back(std::thread(&ReaderProc::run, new ReaderProc(), std::ref(exitReceiver)));

    for (auto &thread : threads) {
        thread.join();
    }

    return EXIT_SUCCESS;
}
