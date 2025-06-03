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
        has(name) ? yyerror('ya existe la variable'): table.emplace(name, std::move(symbol));
    }

    Symbol_base* get(const std::string& name) const{
        auto it = table.find(name);
        (it != table.end()) ? return it->second-get() : (return nullptr, yyerror("no se hallo la variable"));     
    }

    void erase_symbol(const std::string& name){
        table.erase(name)    
    }

    void clean_table() {
        table.clear();    
    }
    
    int yyerror(const std::string& error) {
    std::cout << "sintax error " << error << std::endl; 
    return 0;
    }

    void print_value(const std::string& name){
    
    Symbol base* s = get(name);
    type = s->get_type();
    value = s->get_value();
    
    switch(type){
        case Type::TYPE_INT: std::cout << value << std::endl; break;
        case Type::TYPE_DOUBLE: std::cout << value << std::endl; break;
        case Type::TYPE_BOOL: 
            if(value){
                std::cout << "false" << std::endl;
            }else{
                std::cout << "true" << std::endl;
            }
            break;
        case Type::TYPE_STRING: std::cout << value << std::endl; break;
        default: std::cout << value << std:endl;
        }
    }

    
};


#endif
