%{
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "Symbol_table.h"
#include "utils.h"
#include "nodes.h"

extern int yylex();
extern int yylineno;
Symbol_table table;

%}

%union {
    Type_id* type_;
    Logic_op* l_op_;
    Symbol_base* var_;
    double num_;
    bool bool_;
    char* str_;
}

%token <num_> NUM
%token <str_> VAR
%token <str_> STRING
%token <bool_> V_BOOL

%type <var_> exp
%type <l_op_> L_op;
%type <type_> type_id;

%token INIT INT DOUBLE BOOL STR
%token PRINT
%token IF ELIF ELSE WHILE
//cambio de "token a nonasoc, deberia ser lo mismo"
%nonassoc ADD SUB MUL DIV POW INCP POSTINC
%nonassoc EQ GR WR EQ_GR EQ_WR
%nonassoc LPAREN
%nonassoc RPAREN
%nonassoc LBRACE
%nonassoc RBRACE



%%



input: 
    | input line
    ;

line_non_empty:
    exp '\n'
    | type_id VAR '\n'      {
        Dec_node node($1->get_id(), $2, &table);
        node.execute();
        
    }
    | type_id VAR INIT exp  {
        Dec_ins_node node($1->get_id(), $2, $4, &table);
        node.execute();
    }
    | VAR INIT exp          {
        Ins_node node($1, $3, &table);
        node.execute();
    }
    | IF LPAREN exp RPAREN scope'\n'    {
        std::cout << "en construccion";
    }     
    | PRINT LPAREN exp RPAREN '\n'     {
        Print_node node($3->get_value());
        node.execute();
    }
    ;


line:
    '\n'
    | line_non_empty 
    ;
    



exp:
    NUM                     { 
        $$ = new Symbol_base(Type::TYPE_DOUBLE, $1);                
    }
    |V_BOOL                 {
        $$ = new Symbol_base(Type::TYPE_BOOL, $1);    
    }
    |STRING                 {
        $$ = new Symbol_base(Type::TYPE_STRING, $1);
    }
    | VAR                   { 
        Symbol_base* aux = table.get($1);
        Value v = aux->get_value();
        Type t = aux->get_type();

        $$ = new Symbol_base(t, v);
    }
    | exp exp L_op {
        Logic_node node($1, $2, $3);
        node.execute();
        $$ = node.get_Symbol();
    }
    | exp exp ADD           {
        
        Type t1 = $1->get_type();
        Type t2 = $2->get_type();
        Value v1 = $1->get_value();
        Value v2 = $2->get_value();
        Add_node node(t1, t2, v1, v2);
        node.execute();
        $$ = node.get_Symbol();
    }
    | exp exp SUB           {
        Type t1 = $1->get_type();
        Type t2 = $2->get_type();
        Value v1 = $1->get_value();
        Value v2 = $2->get_value();
        Sub_node node(t1, t2, v1, v2);
        node.execute();
        $$ = node.get_Symbol();
    }
    | exp exp MUL           {
            
        Type t1 = $1->get_type();
        Type t2 = $2->get_type();
        Value v1 = $1->get_value();
        Value v2 = $2->get_value();
        Mul_node node(t1, t2, v1, v2);
        node.execute();
        $$ = node.get_Symbol();
    }

    | exp exp DIV           {
        Type t1 = $1->get_type();
        Type t2 = $2->get_type();
        Value v1 = $1->get_value();
        Value v2 = $2->get_value();
        Div_node node(t1, t2, v1, v2);
        node.execute();
        $$ = node.get_Symbol();
    }
    //| exp exp POW           { $$ = pow($1, $2); }
    //| exp INCP              { $$ = ++$1; }
    //| exp POSTINC           { $$ = $1++; }
    ;

L_op: 
    EQ {
        $$ = new Logic_op(Logic::IS_EQ);
    } 
    | GR {
        $$ = new Logic_op(Logic::IS_GR);
    } | WR {
        $$ = new Logic_op(Logic::IS_WR);
    } | EQ_GR {
        $$ = new Logic_op(Logic::IS_EQ_GR);
    } | EQ_WR {
        $$ = new Logic_op(Logic::IS_EQ_WR);
    }
    ;

type_id: 
    INT         {
        $$ = new Type_id(Type::TYPE_INT);
    } | DOUBLE  {
        $$ = new Type_id(Type::TYPE_DOUBLE);
    } | BOOL    {
        $$ = new Type_id(Type::TYPE_BOOL);
    } | STR     {
        $$ = new Type_id(Type::TYPE_STRING);
    }
    ;

scoped_lines:
    line_non_empty
    |scoped_lines line_non_empty
    ;
scope:
    '{'scoped_lines'}'
    ;

%%

bool eval(Value v){

    bool bool_val;
    std::visit([&](auto&& arg) {
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


    }, v);
    return bool_val;
}

int yyerror(const char* s){
    std::cerr << "Error in line "<< yylineno << ": "<< s << std::endl;
    exit(EXIT_FAILURE);
    return 0;
}


int main(void) {
    yyparse();
    table.clean_table();
    return 0;
}

