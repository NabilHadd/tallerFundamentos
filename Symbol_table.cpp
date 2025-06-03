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

Symbol_base* Symbol_table::get(const std::string& name) const {
    auto it = table.find(name);
    if(it != table.end()) {
        return it->second.get();
    } else {
        return nullptr;
    }
}

void Symbol_table::erase_symbol(const std::string& name){
    table.erase(name);
}

void Symbol_table::clean_table() {
    table.clear();    
}

void Symbol_table::print_value(const std::string& name) {
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
