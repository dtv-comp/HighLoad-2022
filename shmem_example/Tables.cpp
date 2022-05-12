#include "Tables.hpp"

std::ostream &operator << (std::ostream &o, const TableMetrics &tm) {
    o << "num = " << tm.num << ", max = " << tm.max << ", size = " << tm.size << ", offs = " << tm.offs;
    return o;
}

std::ostream &operator << (std::ostream &o, const Order &ord) {
    o << "order_no = " << ord.order_no << ", security = " << ord.security << ", price = " << ord.price << ", qty = " << ord.qty << ", volume = " << ord.volume<< ", buysell = " << ord.buysell;
    return o;
}

std::ostream &operator << (std::ostream &o, const Trade &t) {
    o << "trade_no = " << t.trade_no << ", order1 = " << t.order1 << ", order2 = " << t.order2 << ", status = " << t.status;
    return o;
}
