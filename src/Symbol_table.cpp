#include "Symbol_table.h"

bool Symbol_table::has(const std::string& name) const {
    return table.find(name) != table.end();    
}

void Symbol_table::insert(const std::string& name, std::unique_ptr<Symbol_base> symbol){
    if(has(name)){
        yyerror("ya existe la variable");
    } else {
        table.emplace(name, std::move(symbol));
    }
}

void Symbol_table::update(const std::string& name, std::unique_ptr<Symbol_base> symbol){
    auto it  = table.find(name);
    if(it != table.end()){
        it->second = std::move(symbol);//sera necesario usar variant?
    }
    
}

Symbol_base* Symbol_table::get(const std::string& name) const {
    auto it = table.find(name);
    if(it != table.end()) {
        return it->second.get();
    } else {
        yyerror("variable no encontrada");
        return nullptr;
    }
}

void Symbol_table::print_table(){
    for(const auto& [key, value] : this->table)
        std::cout << key << " => "<< value << std::endl;
}

void Symbol_table::erase_symbol(const std::string& name){
    table.erase(name);
}

void Symbol_table::clean_table() {
    table.clear();    
}
