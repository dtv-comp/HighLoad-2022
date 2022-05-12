#pragma once

#include "ShmemProc.hpp"

#include <future>

class ReaderProc : public ShmemProc {
public:
    ReaderProc() : ShmemProc() {};

    void run(std::future<void> &);
};
