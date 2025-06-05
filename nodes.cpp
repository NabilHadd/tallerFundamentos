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
        
    }, this->v);
}


Mul_node::Mul_node(Type t1, Type t2, Value v1, Value v2){
    this->t1 = t1;
    this->t2 = t2;
    this->v1 = v1;
    this->v2 = v2;
}

Symbol_base* Mul_node::get_Symbol(){return this->aux;}

void Mul_node::execute(){
    if (this->t1 == Type::TYPE_INT && this->t2 == Type::TYPE_INT){
        int a = std::get<int>(this->v1);
        int b = std::get<int>(this->v2);
        int res = a * b;
        this->aux = new Symbol_base(this->t1, res);

    } else if(this->t1 == Type::TYPE_DOUBLE && this->t2 == Type::TYPE_INT){
        double a = std::get<double>(this->v1);
        int b = std::get<int>(this->v2);
        double res = a * b;
        this->aux = new Symbol_base(this->t1, res);

    }else if(this->t2 == Type::TYPE_DOUBLE && this->t1 == Type::TYPE_INT){
        int a = std::get<int>(this->v1);
        double b = std::get<double>(this->v2);
        double res = a * b;
        this->aux = new Symbol_base(this->t2, res);

    }else if(this->t1 == Type::TYPE_DOUBLE && this->t2 == Type::TYPE_DOUBLE){
        double a = std::get<double>(this->v1);
        double b = std::get<double>(this->v2);
        double res = a * b;
        this->aux = new Symbol_base(this->t1, res);
    }else{
        yyerror("tipos incompatibles para multiplicacion");
    }

}



Div_node::Div_node(Type t1, Type t2, Value v1, Value v2){
    this->t1 = t1;
    this->t2 = t2;
    this->v1 = v1;
    this->v2 = v2;
}

Symbol_base* Div_node::get_Symbol(){return this->aux;}

void Div_node::execute(){
    if (this->t1 == Type::TYPE_INT && this->t2 == Type::TYPE_INT){
        int a = std::get<int>(this->v1);
        int b = std::get<int>(this->v2);
        int res = a / b;
        this->aux = new Symbol_base(this->t1, res);

    } else if(this->t1 == Type::TYPE_DOUBLE && this->t2 == Type::TYPE_INT){
        double a = std::get<double>(this->v1);
        int b = std::get<int>(this->v2);
        double res = a / b;
        this->aux = new Symbol_base(this->t1, res);

    }else if(this->t2 == Type::TYPE_DOUBLE && this->t1 == Type::TYPE_INT){
        int a = std::get<int>(this->v1);
        double b = std::get<double>(this->v2);
        double res = a / b;
        this->aux = new Symbol_base(this->t2, res);

    }else if(this->t1 == Type::TYPE_DOUBLE && this->t2 == Type::TYPE_DOUBLE){
        double a = std::get<double>(this->v1);
        double b = std::get<double>(this->v2);
        double res = a / b;
        this->aux = new Symbol_base(this->t1, res);
    }else{
        yyerror("tipos incompatibles para multiplicacion");
    }

}


