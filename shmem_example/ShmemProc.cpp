#include "ShmemProc.hpp"
#include "Shmem.hpp"

#include <iostream>

void ShmemProc::init_tables(void *body, const Tables *tabs) {
    if (!body)
        throw std::bad_alloc();
    char *data = static_cast<char *>(body);
    size_t offset = 0;
    char *tabs_char_ptr = data;
    tables = reinterpret_cast<Tables *>(data);
    const size_t tabs_size = Shmem::page_align(sizeof(Tables));
    offset = tabs_size;
    if (tabs) {
        *tables = *tabs;
        for (const auto &table_info : tables_info) {
            TableMetrics &table_metric = *reinterpret_cast<TableMetrics *>(tabs_char_ptr + table_info.metric_field);
            table_metric.offs = offset;
            table_metric.size = Shmem::page_align(table_info.rec_size * table_metric.max);
            offset += table_metric.size;
        }
    }
    offset = tabs_size;
    for (const auto &table_info : tables_info) {
        TableMetrics &table_metric = *reinterpret_cast<TableMetrics *>(tabs_char_ptr + table_info.metric_field);
        *table_info.table_ptr = reinterpret_cast<void *>(data + offset);
        offset += table_metric.size;
    }
}

void ShmemProc::print_tables() noexcept {
    if (tables) {
        std::cout << "orders (0x" << std::hex << orders << std::dec << "): "
             << "sizeof(Order) = " << sizeof(Order) << ", tables->orders: " << tables->orders << std::endl;
        std::cout << "trades (0x" << std::hex << trades << std::dec << "): "
             << "sizeof(Trade) = " << sizeof(Trade) << ", tables->trades: " << tables->trades << std::endl;
    } else {
        std::cout << "Tables is null" << std::endl;
    }
}
