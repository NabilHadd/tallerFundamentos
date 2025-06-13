#ifndef UTILS_H
#define UTILS_H

#include "Symbol_base.h"
#include <iostream>
#include <memory>
#include <stdio.h>
#include <vector>
#include <cmath>

enum class Logic {
    IS_EQ,
    IS_GR,
    IS_WR,
    IS_EQ_GR,
    IS_EQ_WR,
    OR_,
    AND_
};

//void print_value(Value v);
bool eval(Value v);
std::string to_string_value(Type t, const Value& v);
int yyerror(const char* s);
bool try_parse_s(const std::string& input, Type t);

class Node {
public:
    virtual void execute() = 0; //nodo base, de aqui nacen todos los demas.
    virtual ~Node() = default; 

};



class Statment_node : public Node { //nodo statment, almacena una instruccion
};


class Expr_node {
public:
    virtual Value get_value() const = 0;
    virtual Type get_type() const = 0;
    virtual ~Expr_node() = default;
     
};


class Body_node : public Statment_node {
std::vector<std::unique_ptr<Statment_node>> body;

public:
    std::vector<std::unique_ptr<Statment_node>> get();
    void add_statment(std::unique_ptr<Statment_node> stmt);
    void execute() override;

};



#endif
