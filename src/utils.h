#ifndef UTILS_H
#define UTILS_H

#include "Symbol_base.h"
#include <iostream>
#include <memory>
#include <stdio.h>
#include <vector>
#include <cmath>


//Clase para enumerar las operaciones logicas.
enum class Logic {
    IS_EQ,
    IS_GR,
    IS_WR,
    IS_EQ_GR,
    IS_EQ_WR,
    OR_,
    AND_
};




//Metodos generales utilizados por diferentes scripts
bool eval(Value v);
std::string to_string_value(Type t, const Value& v);
int yyerror(const char* s);
bool try_parse_s(const std::string& input, Type t);



//Nodo base
class Node {
public:
    virtual void execute() = 0;
    virtual ~Node() = default; 

};




//Nodo base para los statments
class Statment_node : public Node {
};



//Nodo base para las expresiones.
class Expr_node {
public:
    virtual Value get_value() const = 0;
    virtual Type get_type() const = 0;
    virtual ~Expr_node() = default;
     
};



//Nodo acumulador de statments.
class Body_node : public Statment_node {


public:
    Body_node() = default;
    Body_node(std::vector<std::unique_ptr<Statment_node>>&& body);
    std::vector<std::unique_ptr<Statment_node>> body;
    void add_statment(std::unique_ptr<Statment_node> stmt);
    void execute() override;

};



#endif
