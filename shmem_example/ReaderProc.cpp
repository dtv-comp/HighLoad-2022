#include "ReaderProc.hpp"
#include "Shmem.hpp"

#include <iostream>

void ReaderProc::run(std::future<void> &exitReceiver) {
    Shmem   shmem;
    try {
        shmem.attach(false, PROT_READ);
        void *body = shmem.get_body_ptr();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        int32_t last_order_num = 0, last_trade_num = 0;
        init_tables(body);
        std::cout << __PRETTY_FUNCTION__ << ":\n";
        print_tables();
        while (exitReceiver.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout) {
            while (last_order_num < tables->orders.num) {
                std::cout << __PRETTY_FUNCTION__ << ": orders[" << last_order_num << "]: " << orders[last_order_num] << std::endl;
                ++last_order_num;
            }
            while (last_trade_num < tables->trades.num) {
                std::cout << __PRETTY_FUNCTION__ << ": trades[" << last_trade_num << "]: " << trades[last_trade_num] << std::endl;
                ++last_trade_num;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout << __PRETTY_FUNCTION__ << ":\n";
        print_tables();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    shmem.detach();
}
