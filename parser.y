%{
#include <stdio.h>
#include <math.h>
#include "var_table.h"
%}

%union {
    Symbol *var;
    double num;
    Value value;
    Type type;
    char *str;
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
    | PRINT LPAREN exp RPAREN '\n'            { print_value($3->value, $3->type); }
    | INT VAR INIT exp '\n'        {insert($2, TYPE_INT, $4->value);}
    | DOUBLE VAR INIT exp '\n'        {insert($2, TYPE_DOUBLE, $4->value);}
    | BOOL VAR INIT exp '\n'        {insert($2, TYPE_BOOL, $4->value);}
    ;

exp:
    NUM                     { 
        $$->type = TYPE_DOUBLE;
        $$->value.d_value = $1;                                          
    }
    | VAR                   { 
        Value v = get_value($1);
        Type t = get_type($1);
        $$->type = t;
        $$->value.i_value = v.i_value; // o d_value, según tipo
        $$->value.d_value = v.d_value;
        $$->value.b_value = v.b_value;
        $$->value.s_value = v.s_value;
    }
    | exp exp ADD           {
        if ($1->type == TYPE_INT && $2->type == TYPE_INT){
            $$->type = TYPE_INT;
            $$->value.i_value = $1->value.i_value + $2->value.i_value;

        } else if($1->type == TYPE_DOUBLE && $2->type == TYPE_INT){
            $$->type = TYPE_DOUBLE;
            $$->value.d_value = $1->value.d_value + $2->value.i_value;

        } else if($2->type == TYPE_DOUBLE && $1->type == TYPE_INT){
            $$->type = TYPE_DOUBLE;
            $$->value.d_value = $2->value.d_value + $1->value.i_value;
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
    printf("todo ben\n");
    free_memory();
    return 0;
}

