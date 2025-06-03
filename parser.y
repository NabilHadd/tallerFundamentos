%{
#include <stdio.h>
#include <math.h>
#include "Symbol_table.h"
%}

%union {
    Symbol_base* var;
    double num;
    Value value;
    Type type;
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
    | PRINT LPAREN exp RPAREN '\n'            { std::cout << "print" << std:endl; }
    | INT VAR INIT exp '\n'        {table.insert($2, new Symbol_base(Type::TYPE_INT, $4));}
    | DOUBLE VAR INIT exp '\n'        {table.insert($2,new Symbol_base(Type::TYPE_DOUBLE, $4));}
    | BOOL VAR INIT exp '\n'        {table.insert($2,new Symbol_base(Type::TYPE_BOOL, $4));}
    ;

exp:
    NUM                     { 
        $$ = new Symbol_base;
        $$->type = Type::TYPE_DOUBLE;
        $$->value = $1;                                          
    }
    | VAR                   { 
        Symbol_base* aux = table.get($1);
        Value v = aux->get_value();
        Type t = aux->get_type();

        $$ = new Symbol_base;
        $$->type = t;
        $$->value = v;
    }
    | exp exp ADD           {
        if ($1->type == Type::TYPE_INT && $2->type == Type::TYPE_INT){
            $$->type = Type::TYPE_INT;
            $$->value = $1->value + $2->value;

        } else if(($1->type == Type::TYPE_DOUBLE && $2->type == Type::TYPE_INT) || ($2->type == Type::TYPE_DOUBLE && $1->type == Type::TYPE_INT)){
            $$->type = Type::TYPE_DOUBLE;
            $$->value = $1->value + $2->value;
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
int main(void) {
    yyparse();
    Symbol_table table;
    printf("todo ben\n");
    free_memory();
    return 0;
}

