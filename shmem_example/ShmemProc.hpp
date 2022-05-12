#pragma once

#include "Tables.hpp"

#include <stddef.h>
#include <vector>

struct TableInfo {
    void   **table_ptr;
    size_t metric_field;
    size_t rec_size;
};
#define TABLE_INFO(tname) { reinterpret_cast<void **>(&tname), offsetof(Tables, tname), sizeof(*tname) }

class ShmemProc {
protected:
    Tables* tables = nullptr;
    Order* orders = nullptr;
    Trade* trades = nullptr;

    std::vector<TableInfo> tables_info = {
        TABLE_INFO(orders),
        TABLE_INFO(trades)
    };
public:
    ShmemProc() {}
    void init_tables(void *body, const Tables *tabs = nullptr);
    void print_tables() noexcept;
};
