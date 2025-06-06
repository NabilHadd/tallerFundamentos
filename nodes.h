#include <memory>
#include <stdio.h>
#include <vector>
#include "Symbol_base.h"
#include "utils.h"


class Logic_op {//clase para identificar el tipo de operacion logica
private:
    Logic op;

public:
    Logic_op(Logic op);
    
    Logic get_op() const;

    ~Logic_op();
};



class Node {
public:
    virtual void execute() = 0; //nodo base, de aqui nacen todos los demas.
    virtual ~Node() = default; 

};



class Statment_node : public Node { //nodo statment, almacena una instruccion
};



class Body_node : public Statment_node { //un nodo que almacena una lista de instrucciones, osea Statment_node
public:
    std::vector<std::unique_ptr<Statment_node>> statments;
    
    void add_statment(std::unique_ptr<Statment_node> stmt);//permite añadir una instruccion a la cola de instrucciones
    void execute () override;//debe ejecutar la instruccion.
    
};



class If_node : public Statment_node{
public:
    Symbol_base* cond;//es la condicion que debe cumplirse, para eso hay que generar un metodo "eval" que evalue la condicion (con un visit seguramente)
    Body_node* body_node;//almacena la lista de instrucciones, aunque no se si deberia declararse como: Body_node* body_node; o asi esta bien.

    If_node(Symbol_base* cond, Body_node* body_node);//constructor
        
    void execute() override;//en este caso execute, siempre que se cumpla la cond deberia llamar al execute de body, recorriendo cada instruccion
};



class Print_node : public Statment_node{
private:
    Value v;
public: 
    Print_node(Value v);
    
    void execute() override;
};




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


