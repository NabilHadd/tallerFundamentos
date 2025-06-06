#include "nodes.h"

//Operador logico------------------------------
Logic_op::Logic_op(Logic op){
    this->op = op;
}

Logic Logic_op::get_op() const { return this->op;}
//----------------------------------------------




//Nodo para cuerpo de un scope----------------------------------------
void Body_node::add_statment(std::unique_ptr<Statment_node> stmt){
    
}

void Body_node::execute(){


}
//--------------------------------------------------------------------



//Nodo para if-------------------------------------------------------------------------------
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
//---------------------------------------------------------------------------------------------





//Nodo para print-----------------------------------------------
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
//---------------------------------------------------------------







//Nodo para la suma------------------------------------------------------------
Add_node::Add_node(Type t1, Type t2, Value v1, Value v2){
    this->t1 = t1;
    this->t2 = t2;
    this->v1 = v1;
    this->v2 = v2;
}

Symbol_base* Add_node::get_Symbol(){return this->aux;}

void Add_node::execute(){
    
    if (this->t1 == this->t2) {
        Value res;
        std::visit([&](auto&& a, auto&& b) {
            using A = std::decay_t<decltype(a)>;
            using B = std::decay_t<decltype(b)>;

            if constexpr (std::is_same_v<A, B>) {
                res = a + b;
            } else {
                yyerror("Error interno: combinación de tipos inválida");
            }
        }, this->v1, this->v2);
        this->aux = new Symbol_base(this->t1, res);

    }else if(this->t1 == Type::TYPE_DOUBLE && this->t2 == Type::TYPE_INT){
        double a = std::get<double>(this->v1);
        int b = std::get<int>(this->v2);
        double res = a + b;
        this->aux = new Symbol_base(this->t1, res);

    }else if(this->t1 == Type::TYPE_INT && this->t2 == Type::TYPE_DOUBLE){
        int a = std::get<int>(this->v1);
        double b = std::get<double>(this->v2);
        double res = a + b;
        this->aux = new Symbol_base(this->t2, res);

    }else if(this->t1 == Type::TYPE_STRING){
        Value res;
        std::string a = std::get<std::string>(this->v1);

        std::visit([&](auto&& b) {//visit prueba con todas las combinaciones posibles para a y b
            std::string str_val;
            using B = std::decay_t<decltype(b)>; 
   
            if constexpr (std::is_same_v<B, std::string>) {
                str_val = b;
            }else if constexpr (std::is_same_v<B, bool>){                   
                str_val = b ? "true" : "false";
            }else{
                str_val = std::to_string(b);
            }
            res = a + str_val;                
        }, this->v2);

        this->aux = new Symbol_base(this->t1, res);

    }else if(this->t2 == Type::TYPE_STRING){
        Value res;
            
    }

}
//-----------------------------------------------------------------------------


//Nodo para multiplicacion------------------------------------------------------
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
        yyerror("tipos incompatibles para división");
    }

}
//---------------------------------------------------------------------------------------





//Nodo para la division-------------------------------------------------------------------
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
        yyerror("tipos incompatibles para división");
    }

}
//-------------------------------------------------------------------------------------




//Nodo para la resta------------------------------------------------------------------
Sub_node::Sub_node(Type t1, Type t2, Value v1, Value v2){
    this->t1 = t1;
    this->t2 = t2;
    this->v1 = v1;
    this->v2 = v2;
}

Symbol_base* Sub_node::get_Symbol(){return this->aux;}

void Sub_node::execute(){
    if (this->t1 == Type::TYPE_INT && this->t2 == Type::TYPE_INT){
        int a = std::get<int>(this->v1);
        int b = std::get<int>(this->v2);
        int res = a - b;
        this->aux = new Symbol_base(this->t1, res);

    } else if(this->t1 == Type::TYPE_DOUBLE && this->t2 == Type::TYPE_INT){
        double a = std::get<double>(this->v1);
        int b = std::get<int>(this->v2);
        double res = a - b;
        this->aux = new Symbol_base(this->t1, res);

    }else if(this->t2 == Type::TYPE_DOUBLE && this->t1 == Type::TYPE_INT){
        int a = std::get<int>(this->v1);
        double b = std::get<double>(this->v2);
        double res = a - b;
        this->aux = new Symbol_base(this->t2, res);

    }else if(this->t1 == Type::TYPE_DOUBLE && this->t2 == Type::TYPE_DOUBLE){
        double a = std::get<double>(this->v1);
        double b = std::get<double>(this->v2);
        double res = a - b;
        this->aux = new Symbol_base(this->t1, res);
    }else{
        yyerror("tipos incompatibles para resta");
    }

}
//-------------------------------------------------------------------------------------



//Nodo para las operaciones logicas--------------------------------------------------------
Logic_node::Logic_node(Symbol_base* exp1, Symbol_base* exp2, Logic_op* l_op){
    this->t1 = exp1->get_type();
    this->t2 = exp2->get_type();
    this->v1 = exp1->get_value();
    this->v2 = exp2->get_value();
    this->op = l_op->get_op();
}

Symbol_base* Logic_node::get_Symbol(){return this->aux;}

void Logic_node::execute(){ //Hacer chequeo de tipos muchisimo mas exhaustivo
    this->aux = new Symbol_base(Type::TYPE_BOOL, true);
    std::visit([&](auto&& a, auto&& b){
        using A = std::decay_t<decltype(a)>; 
        using B = std::decay_t<decltype(b)>; 
        if constexpr (std::is_same_v<A, B>){
            if(this->op == Logic::IS_EQ){
                (a == b) ? this->aux->set_value(true) : this->aux->set_value(false);
            }else if(this->op == Logic::IS_GR){
                (a > b) ?  this->aux->set_value(true)  : this->aux->set_value(false);
            }else if(this->op == Logic::IS_WR){
                (a < b) ?  this->aux->set_value(true)  : this->aux->set_value(false);
            }else if(this->op == Logic::IS_EQ_GR){
                (a >= b) ? this->aux->set_value(true) : this->aux->set_value(false);
            }else if(this->op == Logic::IS_EQ_WR){
                (a <= b) ? this->aux->set_value(true) : this->aux->set_value(false);
            }
        }else{
            yyerror("tipos incompatibles para comparacion");
        }

    }, v1, v2);
}
//-----------------------------------------------------------------------------------------
