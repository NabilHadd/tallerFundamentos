#include <memory>
#include <stdio.h>
#include <vector>
#include "Symbol_base.h"
#include "utils.h"


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
public:
    Value v;
    
    Print_node(Value v);
    
    void execute() override;
};
