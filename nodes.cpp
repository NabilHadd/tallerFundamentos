#include "nodes.h"



Type type_cheq(Type t1, Type t2, char* msg){
    if(t1 == Type::TYPE_INT && t2 == Type::TYPE_INT){
        return t1;    
    }else if(t1 == Type::TYPE_DOUBLE && t2 == Type::TYPE_INT){
        return t1;    
    }else if(t2 == Type::TYPE_DOUBLE && t1 == Type::TYPE_INT){
        return t2;    
    }else if(t1 == Type::TYPE_DOUBLE && t2 == Type::TYPE_DOUBLE){
        return t1;    
    }else{
        yyerror(msg);
        return Type::TYPE_INT;    
    }
}



//Operador logico------------------------------
Logic_op::Logic_op(Logic op){
    this->op = op;
}

Logic Logic_op::get_op() const { return this->op;}
//----------------------------------------------








//Identificador de tipo------------------------------
Type_id::Type_id(Type id){
    this->id = id;
}

Type Type_id::get_id() const{
    return this->id;
}
//---------------------------------------------------









//Declarar, Declarar e istanciar , instaciar-------------------------
Dec_node::Dec_node(Type t_id, const std::string& name, Symbol_table* table){
    this->t_id = t_id;
    this->name = name;
    this->table = table;
}
    
void Dec_node::execute() {
    if(t_id == Type::TYPE_INT)
        this->table->insert(this->name, std::make_unique<Symbol_base>(t_id, 0 ));
    else if (t_id == Type::TYPE_DOUBLE)
        this->table->insert(this->name, std::make_unique<Symbol_base>(t_id, 0 ));
    else if (t_id == Type::TYPE_BOOL)
        this->table->insert(this->name, std::make_unique<Symbol_base>(t_id, false ));
    else if (t_id == Type::TYPE_STRING)
        this->table->insert(this->name, std::make_unique<Symbol_base>(t_id, "" ));
}


Dec_ins_node::Dec_ins_node(Type t_id, const std::string& name, Expr_node* exp, Symbol_table* table){
    this->t_id = t_id;
    this->name = name;
    this->exp = exp;
    this->table = table;
}
    
void Dec_ins_node::execute(){
    Type t = this->exp->get_type();
    Value v = this->exp->get_value();

    if(this->t_id == Type::TYPE_INT){
        if(t == Type::TYPE_INT){
    
            this->table->insert(this->name, std::make_unique<Symbol_base>(this->t_id, std::get<int>(v)));

        }else if(t == Type::TYPE_DOUBLE){

            this->table->insert(this->name, std::make_unique<Symbol_base>(this->t_id, static_cast<int>(std::get<double>(v))));

        }else{yyerror("Tipo incompatible con int");}

    }else if(this->t_id == Type::TYPE_DOUBLE){
        if(t == Type::TYPE_INT){

            this->table->insert(this->name, std::make_unique<Symbol_base>(this->t_id, static_cast<double>(std::get<int>(v))));

        }else if(t == Type::TYPE_DOUBLE){

            this->table->insert(this->name, std::make_unique<Symbol_base>(this->t_id, std::get<double>(v)));

        }else{yyerror("Tipo incompatible con double");}

    }else if(this->t_id == Type::TYPE_BOOL){

        std::visit([&](auto&& arg) {
            bool bool_val;
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, std::string>){
                if(arg == "true")
                    bool_val = true;
                else
                    bool_val = false;
            }else if constexpr (std::is_same_v<T, bool>){
                bool_val = arg;
            }else{
                if (arg > 0)
                    bool_val = true;
                else
                    bool_val = false;
            }

            this->table->insert(this->name, std::make_unique<Symbol_base>(this->t_id, bool_val));

        }, v);

    }else if(this->t_id == Type::TYPE_STRING){

        std::visit([&](auto&& arg) {
            std::string str_val;
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, std::string>)
                str_val = arg;
            else if constexpr (std::is_same_v<T, bool>)
                str_val = arg ? "true" : "false";
            else
                str_val = std::to_string(arg);

            this->table->insert(this->name, std::make_unique<Symbol_base>(this->t_id, str_val));

        }, v);
    }
}



Ins_node::Ins_node(const std::string& name, Expr_node* exp, Symbol_table* table){
    this->name = name;
    this->exp = exp;
    this->table = table;
}

void Ins_node::execute(){
    Type t = this->exp->get_type();
    Value v = this->exp->get_value();
    Symbol_base* var = this->table->get(this->name);

    if(var->get_type() == t){

        this->table->update(this->name, std::make_unique<Symbol_base>(t, v));

    }else if(var->get_type() == Type::TYPE_INT and t == Type::TYPE_DOUBLE){

        this->table->update(this->name, std::make_unique<Symbol_base>(Type::TYPE_INT, static_cast<int>(std::get<double>(v))));

    }else if(var->get_type() == Type::TYPE_DOUBLE and t == Type::TYPE_INT){

        this->table->update(this->name, std::make_unique<Symbol_base>(t, static_cast<double>(std::get<int>(v))));

    }else if (var->get_type() == Type::TYPE_STRING) {

        std::visit([&](auto&& arg) {
            std::string str_val;
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::string>)
                str_val = arg;
            else if constexpr (std::is_same_v<T, bool>)
                str_val = arg ? "true" : "false";
            else
                str_val = std::to_string(arg);
            this->table->update(this->name, std::make_unique<Symbol_base>(Type::TYPE_STRING, str_val));
        }, v);

    } else {
        yyerror("tipos incompatibles");
    }

}
    
//-------------------------------------------------------------------











//Nodo para cuerpo de un scope----------------------------------------
Body_holder_node::Body_holder_node(std::vector<std::unique_ptr<Statment_node>>&& b)
: body(std::move(b)){}
//--------------------------------------------------------------------










//Nodo para cuerpo de un scope----------------------------------------
void Body_node::add_statment(std::unique_ptr<Statment_node> stmt){
    body.push_back(std::move(stmt));
}

void Body_node::execute(){
    for (auto&& stmt : body)
        stmt->execute();
}
//--------------------------------------------------------------------










//Nodo para if-------------------------------------------------------------------------------
If_node::If_node(Expr_node* cond, std::vector<std::unique_ptr<Statment_node>>&& body)
: cond(cond), body(std::move(body)){}

void If_node::execute(){
    bool condition = eval(this->cond->get_value());

    if (condition){
        for(auto& stmt : this->body)
        stmt->execute();
    }

}
//---------------------------------------------------------------------------------------------







//Nodo para else if statment, no scope--------------------------------------------------------
If_else_node::If_else_node(Expr_node* cond, std::vector<std::unique_ptr<Statment_node>> body, std::unique_ptr<Statment_node> branch){
    this->cond = cond;
    this->body = body;
    this->branch = branch;
}
    
void If_else_node::execute () {
    bool condition = eval(this->cond->get_value());

    if (condition){
        for(auto& stmt : this->body)
        stmt->execute();
    }else{
        this->branch->execute();
    }
}

//---------------------------------------------------------------------------------------------




//Nodo para print-----------------------------------------------
Print_node::Print_node(Expr_node* exp){
    this->exp = exp;
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
        
    }, this->exp->get_value());
}
//---------------------------------------------------------------









//lectura de constantes y variables-----------------------------------
Const_node::Const_node(Type t, Value v){
    this->t = t;
    this->v = v;
}
    
Type Const_node::get_type() const {
    return this->t;
}
Value Const_node::get_value() const {
    return this->v;
}

Var_node::Var_node(std::string name, Symbol_table* table){
    this->name = name;
    this->table = table;
}
    
Type Var_node::get_type() const {
    return this->table->get(this->name)->get_type();
}
Value Var_node::get_value() const {
    return this->table->get(this->name)->get_value();
}
//----------------------------------------------------------------------------







//Nodo para la suma------------------------------------------------------------
Add_node::Add_node(Expr_node* exp1, Expr_node* exp2){
    this->exp1 = exp1;
    this->exp2 = exp2;
}

Type Add_node::get_type()const{
    Type t1 = this->exp1->get_type();
    Type t2 = this->exp2->get_type();
    if(t1 == t2){
        return t1;
    }else if(t1 == Type::TYPE_DOUBLE && t2 == Type::TYPE_INT){
        return t1;
    }else if(t2 == Type::TYPE_DOUBLE && t1 == Type::TYPE_INT){
        return t2;
    }else if(t1 == Type::TYPE_STRING || t2 == Type::TYPE_STRING){
        return Type::TYPE_STRING;
    }else{
        yyerror("tipos incompatibles para suma1");    
        return Type::TYPE_INT;
    }
    
}

Value Add_node::get_value()const{
    Type t1 = this->exp1->get_type();
    Type t2 = this->exp2->get_type();
    Value v1 = this->exp1->get_value();
    Value v2 = this->exp2->get_value();
    
    if (t1 == t2) {
        Value res;
        std::visit([&](auto&& a, auto&& b) {
            using A = std::decay_t<decltype(a)>;
            using B = std::decay_t<decltype(b)>;

            if constexpr (std::is_same_v<A, B>) {
                res = a + b;
            } else {
                yyerror("Error interno: combinación de tipos inválida");
            }
        }, v1, v2);
        return res;

    }else if(t1 == Type::TYPE_DOUBLE && t2 == Type::TYPE_INT){
        double a = std::get<double>(v1);
        int b = std::get<int>(v2);
        double res = a + b;
        return res;

    }else if(t1 == Type::TYPE_INT && t2 == Type::TYPE_DOUBLE){
        int a = std::get<int>(v1);
        double b = std::get<double>(v2);
        double res = a + b;
        return res;

    }else if(t1 == Type::TYPE_STRING){
        Value res;
        std::string a = std::get<std::string>(v1);

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
        }, v2);

        return res;

    }else if(t2 == Type::TYPE_STRING){
        Value res = 0;
        return res;    
    }else{
        yyerror("tipos incompatibles para suma");
        return 0;
    }
}

//-----------------------------------------------------------------------------


//Nodo para multiplicacion------------------------------------------------------
Mul_node::Mul_node(Expr_node* exp1, Expr_node* exp2){
    this->exp1 = exp1;
    this->exp2 = exp2;
}

Type Mul_node::get_type()const{
    Type t1 = this->exp1->get_type();
    Type t2 = this->exp2->get_type();
    return type_cheq(t1, t2, "chequeo de tipos incompatibles para multiplicacion");
}


Value Mul_node::get_value()const{
    Type t1 = this->exp1->get_type();
    Type t2 = this->exp2->get_type();
    Value v1 = this->exp1->get_value();
    Value v2 = this->exp2->get_value();

    if (t1 == Type::TYPE_INT && t2 == Type::TYPE_INT){
        int a = std::get<int>(v1);
        int b = std::get<int>(v2);
        int res = a * b;
        return res;

    } else if(t1 == Type::TYPE_DOUBLE && t2 == Type::TYPE_INT){
        double a = std::get<double>(v1);
        int b = std::get<int>(v2);
        double res = a * b;
        return res;

    }else if(t2 == Type::TYPE_DOUBLE && t1 == Type::TYPE_INT){
        int a = std::get<int>(v1);
        double b = std::get<double>(v2);
        double res = a * b;
        return res;

    }else if(t1 == Type::TYPE_DOUBLE && t2 == Type::TYPE_DOUBLE){
        double a = std::get<double>(v1);
        double b = std::get<double>(v2);
        double res = a * b;
        return res;
    }else{
        yyerror("tipos incompatibles para multiplicacion");
        return 0;
    }
}
//---------------------------------------------------------------------------------------





//Nodo para la division-------------------------------------------------------------------
Div_node::Div_node(Expr_node* exp1, Expr_node* exp2){
    this->exp1 = exp1;
    this->exp2 = exp2;
}

Type Div_node::get_type()const{
    Type t1 = this->exp1->get_type();
    Type t2 = this->exp2->get_type();
    
    return type_cheq(t1, t2, "chequeo de tipos incompatibles para division");
}
Value Div_node::get_value()const{
    Type t1 = this->exp1->get_type();
    Type t2 = this->exp2->get_type();
    Value v1 = this->exp1->get_value();
    Value v2 = this->exp2->get_value();

    if (t1 == Type::TYPE_INT && t2 == Type::TYPE_INT){
        int a = std::get<int>(v1);
        int b = std::get<int>(v2);
        int res = a / b;
        return res;

    } else if(t1 == Type::TYPE_DOUBLE && t2 == Type::TYPE_INT){
        double a = std::get<double>(v1);
        int b = std::get<int>(v2);
        double res = a / b;
        return res;

    }else if(t2 == Type::TYPE_DOUBLE && t1 == Type::TYPE_INT){
        int a = std::get<int>(v1);
        double b = std::get<double>(v2);
        double res = a / b;
        return res;

    }else if(t1 == Type::TYPE_DOUBLE && t2 == Type::TYPE_DOUBLE){
        double a = std::get<double>(v1);
        double b = std::get<double>(v2);
        double res = a / b;
        return res;
    }else{
        yyerror("tipos incompatibles para división");
        return 0;
    }
}

//-------------------------------------------------------------------------------------




//Nodo para la resta------------------------------------------------------------------
Sub_node::Sub_node(Expr_node* exp1, Expr_node* exp2){
    this->exp1 = exp1;
    this->exp2 = exp2;
}

Type Sub_node::get_type()const{
    Type t1 = this->exp1->get_type();
    Type t2 = this->exp2->get_type();
    
    return type_cheq(t1, t2, "chequeo de tipos incompatibles para resta");
}

Value Sub_node::get_value()const{
    Type t1 = this->exp1->get_type();
    Type t2 = this->exp2->get_type();
    Value v1 = this->exp1->get_value();
    Value v2 = this->exp2->get_value();

    if (t1 == Type::TYPE_INT && t2 == Type::TYPE_INT){
        int a = std::get<int>(v1);
        int b = std::get<int>(v2);
        int res = a - b;
        return res;

    } else if(t1 == Type::TYPE_DOUBLE && t2 == Type::TYPE_INT){
        double a = std::get<double>(v1);
        int b = std::get<int>(v2);
        double res = a - b;
        return res;

    }else if(t2 == Type::TYPE_DOUBLE && t1 == Type::TYPE_INT){
        int a = std::get<int>(v1);
        double b = std::get<double>(v2);
        double res = a - b;
        return res;

    }else if(t1 == Type::TYPE_DOUBLE && t2 == Type::TYPE_DOUBLE){
        double a = std::get<double>(v1);
        double b = std::get<double>(v2);
        double res = a - b;
        return res;
    }else{
        yyerror("tipos incompatibles para resta");
        return 0;
    }
}

//-------------------------------------------------------------------------------------



//Nodo para las operaciones logicas--------------------------------------------------------
Logic_node::Logic_node(Expr_node* exp1, Expr_node* exp2, Logic_op* l_op){
    this->exp1 = exp1;
    this->exp2 = exp2;
    this->op = l_op->get_op();
}

Type Logic_node::get_type()const{return Type::TYPE_BOOL;}


Value Logic_node::get_value()const{//Hacer chequeo de tipos muchisimo mas exhaustivo, para permitir double e int
    Value v1 = this->exp1->get_value();
    Value v2 = this->exp2->get_value();
    Value v;

    std::visit([&](auto&& a, auto&& b){
        using A = std::decay_t<decltype(a)>; 
        using B = std::decay_t<decltype(b)>; 
        if constexpr (std::is_same_v<A, B>){
            if(this->op == Logic::IS_EQ){
                (a == b) ? v = true : v = false;
            }else if(this->op == Logic::IS_GR){
                (a > b) ?  v = true : v = false;
            }else if(this->op == Logic::IS_WR){
                (a < b) ?  v = true : v = false;
            }else if(this->op == Logic::IS_EQ_GR){
                (a >= b) ? v = true : v = false;
            }else if(this->op == Logic::IS_EQ_WR){
                (a <= b) ? v = true : v = false;
            }
        }else{
            yyerror("tipos incompatibles para comparacion");
        }

    }, v1, v2);
    return v;
}

//-----------------------------------------------------------------------------------------
