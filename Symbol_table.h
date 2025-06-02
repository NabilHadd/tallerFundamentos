#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <unordered_map>
#include <memory>
#include <string>
#include "Symbol_base.h"

struct Symbol_table {
    std::unordered_map<std::string, std::unique_ptr<Symbol_base>> table;

    
    bool has(const std::string& name) const {
        return table.find(name) != table.end();    
    }

    void insert(const std::string& name, std::unique_ptr<Symbol_base> symbol){
        (table.find(name) == table.end()) ? table.emplace(name, std::move(symbol)) : yyerror('ya existe la variable');
    }

    Symbol_base* get(const std::string& name) const{
        auto it = table.find(name);
        return (it != table.end()) ? it->second-get() : nullptr    
    }

    void erase_symbol(const std::string& name){
        table.erase(name)    
    }

    void clean_table() {
        table.clear();    
    }
    
    int yyerror(const std::string& error) {
    cout << "sintax error " << error << endl; 
    return 0;
    }
    
};


#endif
