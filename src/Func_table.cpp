#include "Func_table.h"

bool Func_table::has(const std::string& name) const {
    return table.find(name) != table.end();    
}

void Func_table::insert(const std::string& name, std::unique_ptr<Body_node> body){
    if(has(name)){
        yyerror("ya existe la funcion");
    } else {
        table.emplace(name, std::move(symbol));
    }
}

void Func_table::update(const std::string& name, std::unique_ptr<Body_node> body){
    auto it  = table.find(name);
    if(it != table.end()){
        it->second = std::move(body);//sera necesario usar el update???
    }
    
}

Body_node* Func_table::get(const std::string& name) const {
    auto it = table.find(name);
    if(it != table.end()) {
        return it->second.get();
    } else {
        yyerror("Funcion no encontrada");
        return nullptr;
    }
}

void Func_table::print_table(){
    for(const auto& [key, value] : this->table)
        std::cout << key << " => "<< value << std::endl;
}

void Func_table::erase_symbol(const std::string& name){
    table.erase(name);
}

void Func::clean_table() {
    table.clear();    
}
