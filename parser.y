%{
#include <stdio.h>
#include <math.h>
%}

%union {
    double num;
}

%token <num> NUM
%type <num> exp 

%token ADD SUB MUL DIV POW INCP POSTINC

%nonassoc ADD
%nonassoc SUB
%nonassoc MUL
%nonassoc DIV
%nonassoc POW
%nonassoc INCP
%nonassoc POSTINC

%%
input: 
    | input line
    ;

line:
    '\n'
    | exp '\n' { printf("\t%.10g\n", $1); }
    ;

exp:
    NUM           { $$ = $1; }
    | exp exp ADD { $$ = $1 + $2; }
    | exp exp SUB { $$ = $1 - $2; }
    | exp exp MUL { $$ = $1 * $2; }
    | exp exp DIV { $$ = $1 / $2; }
    | exp exp POW { $$ = pow($1, $2); }
    | exp INCP    { $$ = ++$1; }
    | exp POSTINC { $$ = $1++; }
    ;
%%
int main(void) {
    yyparse();
    return 0;
}

int yyerror(const char *s) {
    printf("syntax error\n");
    return 0;
}
