#include <iostream>
#include <memory>
#include <stdio.h>
#include <vector>
#include <cmath>
#include "Symbol_base.h"
#include "Symbol_table.h"
#include "utils.h"

//Clases base-------------------------------------------------------------
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


class Expr_node {
public:
    virtual Value get_value() const = 0;
    virtual Type get_type() const = 0;
    virtual ~Expr_node() = default;
     
};

//------------------------------------------------------------------------------------------















//Clases statment--------------------------------------------------------------------------

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

    Expr_node* cond;
    std::vector<std::unique_ptr<Statment_node>> body;

public:
    If_node(Expr_node* cond, std::vector<std::unique_ptr<Statment_node>>&& body);//constructor
        
    void execute() override;//en este caso execute, siempre que se cumpla la cond deberia llamar al execute de body, recorriendo cada instruccion
};

class If_else_node : public Statment_node{
    Expr_node* cond;
    std::vector<std::unique_ptr<Statment_node>> body;
    std::unique_ptr<Statment_node> branch;
public:
    If_else_node(Expr_node* cond, std::vector<std::unique_ptr<Statment_node>>&& body, std::unique_ptr<Statment_node> branch);
    
    void execute () override;
};


class If_else_scope_node : public Statment_node{
    Expr_node* cond;
    std::vector<std::unique_ptr<Statment_node>> body;
    std::vector<std::unique_ptr<Statment_node>> branch;
public:
    If_else_scope_node(Expr_node* cond, std::vector<std::unique_ptr<Statment_node>>&& body, std::vector<std::unique_ptr<Statment_node>>&& branch);
    
    void execute () override;
};

class While_node : public Statment_node{

    Expr_node* cond;
    std::vector<std::unique_ptr<Statment_node>> body;

public:
    While_node(Expr_node* cond, std::vector<std::unique_ptr<Statment_node>>&& body);
        
    void execute() override;
};

class Print_node : public Statment_node{
private:
    Expr_node* exp;
public: 
    Print_node(Expr_node* exp);
    
    void execute() override;
};

class Scan_node : public Statment_node{
private:
    std::string name;
    Symbol_table* table;
public:
    Scan_node(const std::string& name, Symbol_table* table);

    void execute() override;

};
//---------------------------------------------------------------------------------------------------------------------------
















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
    Expr_node* exp;
    Symbol_table* table;
public:
    Dec_ins_node(Type t_id, const std::string& name, Expr_node* exp, Symbol_table* table);
    
    void execute() override;
};

class Ins_node : public Statment_node{
private:
    std::string name;
    Expr_node* exp;
    Symbol_table* table;
public:
    
    Ins_node(const std::string& name, Expr_node* exp, Symbol_table* table);

    void execute() override;
    
};
//----------------------------------------------------------------------------------------------------------------------






//Clases para expresiones---------------------------------------------------------------------------------------------

class Const_node : public Expr_node {
private:
    Type t;
    Value v;
public:
    Const_node(Type t, Value v);
    
    Type get_type() const override;
    Value get_value() const override;
};

class Var_node : public Expr_node {
private:
    std::string name;
    Symbol_table* table;
public:
    Var_node(std::string name, Symbol_table* table);
    
    Type get_type() const override;
    Value get_value() const override;   
};

class Parse_node : public Expr_node {
private:
    Type_id* targ_t;
    Expr_node* exp;
public: 
    Parse_node(Type_id* t, Expr_node* exp);
    
    Type get_type() const override;
    Value get_value() const override;
    
};



//--------------------------------------------------------------------------------------------------------------------





//aritmetica:------------------------------------------------------------------------------------------------------------
class Add_node : public Expr_node {
private:
    Expr_node* exp1;
    Expr_node* exp2;

public:
    Add_node(Expr_node* exp1, Expr_node* exp2);
    Type get_type() const override;
    Value get_value() const override;
};



class Mul_node : public Expr_node{
private:
    Expr_node* exp1;
    Expr_node* exp2;
    
    
public:
    Mul_node(Expr_node* exp1, Expr_node* exp2);
    Type get_type() const override;
    Value get_value() const override;
};

class Div_node : public Expr_node{
private:
    Expr_node* exp1;
    Expr_node* exp2;    


public:
    Div_node(Expr_node* exp1, Expr_node* exp2);
    Type get_type() const override;
    Value get_value() const override;
};

class Sub_node : public Expr_node{
private:
    Expr_node* exp1;
    Expr_node* exp2;

public:
    Sub_node(Expr_node* exp1, Expr_node* exp2);
    Type get_type() const override;
    Value get_value() const override;
};

class Pow_node : public Expr_node{
private:
    Expr_node* exp1;
    Expr_node* exp2;

public:
    Pow_node(Expr_node* exp1, Expr_node* exp2);
    Type get_type() const override;
    Value get_value() const override;
};

class Not_node: public Expr_node{
private:
    Expr_node* exp1;

public:
    Not_node(Expr_node* exp1);
    
    Type get_type() const override;
    
    Value get_value() const override;
    
};
class Logic_node : public Expr_node {
private:
    Expr_node* exp1;
    Expr_node* exp2;
    Logic op;

public:
    Logic_node(Expr_node* exp1, Expr_node* exp2, Logic_op* l_op);
    Type get_type() const override;
    Value get_value() const override;
};


