%{
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "Symbol_table.h"
#include "errors.h"
extern int yylex();
Symbol_table table;

%}

%union {
    Symbol_base* var;
    double num;
    char* str;
}

%token <num> NUM
%token <str> VAR
%type <var> exp

%token ADD SUB MUL DIV POW INCP POSTINC 
%token INIT INT DOUBLE BOOL 
%token PRINT

%nonassoc ADD
%nonassoc SUB
%nonassoc MUL
%nonassoc DIV
%nonassoc POW
%nonassoc INCP
%nonassoc POSTINC
%nonassoc PRINT
%nonassoc INIT
%nonassoc LPAREN
%nonassoc RPAREN



%%



input: 
    | input line
    ;

line:
    '\n'
    | exp '\n'
    | PRINT LPAREN exp RPAREN '\n'            { std::cout << "print" << std::endl; }
    | INT VAR INIT exp '\n'        {table.insert($2, std::make_unique<Symbol_base>(Type::TYPE_INT, std::get<int>($4->get_value())));}
    | DOUBLE VAR INIT exp '\n'        {table.insert($2, std::make_unique<Symbol_base>(Type::TYPE_DOUBLE, std::get<double>($4->get_value())));}
    | BOOL VAR INIT exp '\n'        {table.insert($2, std::make_unique<Symbol_base>(Type::TYPE_BOOL, std::get<bool>($4->get_value())));}
    ;

exp:
    NUM                     { 
        $$ = new Symbol_base(Type::TYPE_INT, $1);                
    }
    | VAR                   { 
        Symbol_base* aux = table.get($1);
        Value v = aux->get_value();
        Type t = aux->get_type();

        $$ = new Symbol_base(t, v);
    }
    | exp exp ADD           {
        if ($1->get_type() == Type::TYPE_INT && $2->get_type() == Type::TYPE_INT){
            int a = std::get<int>($1->get_value());
            int b = std::get<int>($2->get_value());
            int res = a + b;
            $$ = new Symbol_base(Type::TYPE_INT, res);

        } else if($1->get_type() == Type::TYPE_DOUBLE && $2->get_type() == Type::TYPE_INT){
            double a = std::get<double>($1->get_value());
            int b = std::get<int>($2->get_value());
            double res = a + b;
            $$ = new Symbol_base(Type::TYPE_DOUBLE, res);
        }else if($2->get_type() == Type::TYPE_DOUBLE && $1->get_type() == Type::TYPE_INT){
            int a = std::get<double>($1->get_value());
            double b = std::get<int>($2->get_value());
            double res = a + b;
            $$ = new Symbol_base(Type::TYPE_DOUBLE, res);
        }else if($1->get_type()==Type::TYPE_DOUBLE && $2->get_type()==Type::TYPE_DOUBLE){
            int a = std::get<double>($1->get_value());
            double b = std::get<int>($2->get_value());
            double res = a + b;
            $$ = new Symbol_base(Type::TYPE_DOUBLE, res);
        }
    }
    //| exp exp SUB           { $$ = $1 - $2; }
    //| exp exp MUL           { $$ = $1 * $2; }
    //| exp exp DIV           { $$ = $1 / $2; }
    //| exp exp POW           { $$ = pow($1, $2); }
    //| exp INCP              { $$ = ++$1; }
    //| exp POSTINC           { $$ = $1++; }
    ;
%%

int yyerror(const char*){
    std::cerr << "syntax error: " << std::endl;
    return 0;
}


int main(void) {
    yyparse();
    table.clean_table();
    return 0;
}

