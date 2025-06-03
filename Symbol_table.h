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
        if(has(name)){
            yyerror("ya existe la variable");
        }else{
            table.emplace(name, std::move(symbol));
        }
    }

    Symbol_base* get(const std::string& name) const{
        auto it = table.find(name);
        if(it!= table.end()){
            return it->second.get();
        }else{
            return nullptr;
        }
             
    }

    void erase_symbol(const std::string& name){
        table.erase(name);
    }

    void clean_table() {
        table.clear();    
    }
    
    int yyerror(const std::string& error) {
    std::cout << "sintax error " << error << std::endl; 
    return 0;
    }

    void print_value(const std::string& name){
    
    Symbol_base* s = get(name);
    Type type = s->get_type();
    Value value = s->get_value();
    
    switch(type){
        case Type::TYPE_INT: std::cout << std::get<int>(value) << std::endl; break;
        case Type::TYPE_DOUBLE: std::cout << std::get<double>(value) << std::endl; break;
        case Type::TYPE_BOOL: 
            if(std::get<bool>(value)){
                std::cout << "false" << std::endl;
            }else{
                std::cout << "true" << std::endl;
            }
            break;
        case Type::TYPE_STRING: std::cout << std::get<std::string>(value) << std::endl; break;
        default: std::cout << "<16800002x1>" << std::endl;
        }
    }

    
};


#endif
