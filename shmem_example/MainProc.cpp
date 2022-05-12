#include "MainProc.hpp"
#include "Shmem.hpp"

#include <iostream>

class record_not_allocated : public std::bad_alloc {
protected:
    std::string _msg;
public:
    record_not_allocated(const std::string &msg) : _msg(msg) {}
    virtual const char* what() const noexcept {
        return _msg.c_str();
    }
};

int MainProc::create_new_order(const Order &data) {
    if (tables->orders.num >= tables->orders.max)
        return -1;
    const int recno = tables->orders.num;
    orders[recno] = data;
    ++tables->orders.num;
    return recno;
}

int MainProc::create_new_trade(const Trade &data) {
    if (tables->trades.num >= tables->trades.max)
        return -1;
    const int recno = tables->trades.num;
    trades[recno] = data;
    ++tables->trades.num;
    return recno;
}

void MainProc::run(std::promise<void> &exitSignal) {
    Shmem   shmem;
    try {
        shmem.attach(true);
        void *body = shmem.get_body_ptr();
        Tables st;
        st.orders.num = 0;
        st.orders.max = 10;
        st.trades.num = 0;
        st.trades.max = 5;
        init_tables(body, &st);
        std::cout << __PRETTY_FUNCTION__ << ":\n";
        print_tables();
        for (int i = 0; i < 10; ++i) {
            if (create_new_order({.order_no = i + 1, .buysell = i % 2 ? 'B' : 'S'}) < 0)
                throw record_not_allocated(std::string(__PRETTY_FUNCTION__) + ": Order was not allocated");
            const auto order1_idx = tables->orders.num - 1;
            std::cout << __PRETTY_FUNCTION__ << ": orders[" << order1_idx << "]: " << orders[order1_idx] << std::endl;
            if (create_new_order({.order_no = i + 2, .buysell = i % 2 ? 'S' : 'B'}) < 0)
                throw record_not_allocated(std::string(__PRETTY_FUNCTION__) + ": Order was not allocated");
            const auto order2_idx = tables->orders.num - 1;
            std::cout << __PRETTY_FUNCTION__ << ": orders[" << order2_idx << "]: " << orders[order2_idx] << std::endl;
            if (create_new_trade({.trade_no = i + 1, .order1 = order1_idx, .order2 = order2_idx, .status = i % 3 ? 'M' : 'C'}) < 0)
                throw record_not_allocated(std::string(__PRETTY_FUNCTION__) + ": Trade was not allocated");
            const auto trade_idx = tables->trades.num - 1;
            std::cout << __PRETTY_FUNCTION__ << ": trades[" << trade_idx << "]: " << trades[trade_idx] << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        std::cout << __PRETTY_FUNCTION__ << ":\n";
        print_tables();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    exitSignal.set_value();
    shmem.detach();
}

