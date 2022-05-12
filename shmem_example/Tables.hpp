#pragma once

#include <stddef.h>
#include <inttypes.h>
#include <iostream>

struct TableMetrics {
    int32_t num;
    int32_t max;
    size_t  size;
    size_t  offs;

    friend std::ostream &operator << (std::ostream &, const TableMetrics &);
};

struct Tables {
    TableMetrics orders;
    TableMetrics trades;
};

struct Order {
    int64_t order_no;
    int32_t security;
    int32_t price;
    int32_t qty;
    int64_t volume;
    char    buysell;

    friend std::ostream &operator << (std::ostream &, const Order &);
};

struct Trade {
    int64_t trade_no;
    int32_t order1;
    int32_t order2;
    char    status;

    friend std::ostream &operator << (std::ostream &, const Trade &);
};
