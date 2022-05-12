#pragma once

#include "ShmemProc.hpp"

#include <future>

class MainProc : public ShmemProc {
public:
    MainProc() : ShmemProc() {};

    int create_new_order(const Order &);
    int create_new_trade(const Trade &);

    void run(std::promise<void> &);
};
