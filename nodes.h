#include <memory>
#include <stdio.h>
#include <vector>
#include <set>
#include <stack>
#include "Symbol_base.h"
#include "Symbol_table.h"
#include "utils.h"


class Logic_op {//clase para identificar el tipo de operacion logica
private:
    Logic op;

public:
    Logic_op(Logic op);
    
    Logic get_op() const;

    ~Logic_op();
};

class Type_id {
private:
    Type id;
public:
    Type_id(Type id);
    
    Type get_id() const;
    
    ~Type_id();
};



class Node {
public:
    virtual void execute() = 0; //nodo base, de aqui nacen todos los demas.
    virtual ~Node() = default; 

};



class Statment_node : public Node { //nodo statment, almacena una instruccion
};



class Body_node : public Statment_node {
std::vector<std::unique_ptr<Statment_node>> body;

public:
    void add_statment(std::unique_ptr<Statment_node> stmt);
    void execute() override;

};


class Body_holder_node {
public:
    std::vector<std::unique_ptr<Statment_node>> body;
    
    Body_holder_node(std::vector<std::unique_ptr<Statment_node>>&& b);
    
};



class If_node : public Statment_node{

    Symbol_base* cond;
    std::vector<std::unique_ptr<Statment_node>> body;

public:
    If_node(Symbol_base* cond, std::vector<std::unique_ptr<Statment_node>>&& body);//constructor
        
    void execute() override;//en este caso execute, siempre que se cumpla la cond deberia llamar al execute de body, recorriendo cada instruccion
};



class Print_node : public Statment_node{
private:
    Value v;
public: 
    Print_node(Value v);
    
    void execute() override;
};


//Declaracion e instancia-----------------------------------------------------------------------------------------------
class Dec_node : public Statment_node{
private:
    Type t_id;
    std::string name;//usar unique_ptr al guardar en la tabla
    Symbol_table* table;
public:
    Dec_node(Type t_id, const std::string& name, Symbol_table* table);
    
    void execute() override;
};

class Dec_ins_node : public Statment_node{
private:
    Type t_id;
    std::string name;
    Type t;
    Value v; //usar unique si esque se va a guardar en symbol_table
    Symbol_table* table;
public:
    Dec_ins_node(Type t_id, const std::string& name, Symbol_base* exp, Symbol_table* table);
    
    void execute() override;
};

class Ins_node : public Statment_node{
private:
    std::string name;
    Type t;
    Value v;
    Symbol_table* table;
public:
    
    Ins_node(const std::string& name, Symbol_base* exp, Symbol_table* table);

    void execute() override;
    
};
//----------------------------------------------------------------------------------------------------------------------



//aritmetica:------------------------------------------------------------------------------------------------------------
class Add_node : public Statment_node{
private:
    Type t1;
    Type t2;
    Value v1;
    Value v2;
    Symbol_base* aux;
public:
    Add_node(Type t1, Type t2, Value v1, Value v2);
    Symbol_base* get_Symbol();
    void execute() override;
};



class Mul_node : public Statment_node{
private:
    Type t1;
    Type t2;
    Value v1;
    Value v2;
    Symbol_base* aux;
public:
    Mul_node(Type t1, Type t2, Value v1, Value v2);
    Symbol_base* get_Symbol();
    void execute() override;
};

class Div_node : public Statment_node{
private:
    Type t1;
    Type t2;
    Value v1;
    Value v2;
    Symbol_base* aux;
public:
    Div_node(Type t1, Type t2, Value v1, Value v2);
    Symbol_base* get_Symbol();
    void execute() override;
};

class Sub_node : public Statment_node{
private:
    Type t1;
    Type t2;
    Value v1;
    Value v2;
    Symbol_base* aux;
public:
    Sub_node(Type t1, Type t2, Value v1, Value v2);
    Symbol_base* get_Symbol();
    void execute() override;
};

class Logic_node : public Statment_node {
private:
    Type t1;
    Type t2;
    Value v1;
    Value v2;
    Logic op;
    Symbol_base* aux;
public:
    Logic_node(Symbol_base* exp1, Symbol_base* exp2, Logic_op* l_op);
    Symbol_base* get_Symbol();
    void execute() override;
};


