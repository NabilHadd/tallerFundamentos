#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <unordered_map>
#include <memory>
#include <string>
#include "Symbol_base.h"
#include "errors.h"

class Symbol_table {
private:
    std::unordered_map<std::string, std::unique_ptr<Symbol_base>> table;

public:
    bool has(const std::string& name) const;
    void insert(const std::string& name, std::unique_ptr<Symbol_base> symbol);
    Symbol_base* get(const std::string& name) const;

    void erase_symbol(const std::string& name);
    void clean_table();
    void print_value(const std::string& name);

};

#endif
