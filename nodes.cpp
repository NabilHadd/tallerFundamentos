#include "nodes.h"
void Body_node::add_statment(std::unique_ptr<Statment_node> stmt){
    
}

void Body_node::execute(){


}


If_node::If_node(Symbol_base* cond, Body_node* body_node){
    this->cond = cond;
    this->body_node = body_node;
}

void If_node::execute(){
    if (eval(cond->get_value())){
        //hacer que ejecute cada linea de body y almacene la info en una table.
        //haciendo uso de body->execute(); dentro de un for algo asi.  
        //hay que pensar bien lo de la memoria para que se elimine apenas salga de cad scope.
    }else{
        return;//si eval no cumple que no haga nada.
    }
}

Print_node::Print_node(Value v){
    this->v = v;
}

void Print_node::execute() {
    std::string s_val;

    std::visit([&](auto&& arg){
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::string>){
            s_val = arg;
        }else if constexpr (std::is_same_v<T, bool>){
            s_val = (arg ? "true" : "false");
        }else{
            s_val = std::to_string(arg);
        }
        std::cout << s_val << std::endl;
        
    }, v);
}
