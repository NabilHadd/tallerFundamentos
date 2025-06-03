#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <unordered_map>
#include <memory>
#include <string>
#include "Symbol_base.h"

class Symbol_table {
private:
    std::unordered_map<std::string, std::unique_ptr<Symbol_base>> table;

public:
    bool has(const std::string& name) const {
        return table.find(name) != table.end();    
    }

    void insert(const std::string& name, std::unique_ptr<Symbol_base> symbol){
        if(has(name)){
            yyerror("ya existe la variable");
        } else {
            table.emplace(name, std::move(symbol));
        }
    }

    Symbol_base* get(const std::string& name) const {
        auto it = table.find(name);
        if(it != table.end()) {
            return it->second.get();
        } else {
            return nullptr;
        }
    }

    void erase_symbol(const std::string& name){
        table.erase(name);
    }

    void clean_table() {
        table.clear();    
    }

    void print_value(const std::string& name) {
        Symbol_base* s = get(name);
        if (!s) {
            std::cout << "Variable no encontrada.\n";
            return;
        }

        Type type = s->get_type();
        Value value = s->get_value();

        switch(type){
            case Type::TYPE_INT: std::cout << std::get<int>(value) << std::endl; break;
            case Type::TYPE_DOUBLE: std::cout << std::get<double>(value) << std::endl; break;
            case Type::TYPE_BOOL: 
                std::cout << (std::get<bool>(value) ? "true" : "false") << std::endl;
                break;
            case Type::TYPE_STRING: std::cout << std::get<std::string>(value) << std::endl; break;
            default: std::cout << "<16800002x1>" << std::endl;
        }
    }
};

// Mover esta función fuera de la clase
int yyerror(const std::string& error);

#endif
