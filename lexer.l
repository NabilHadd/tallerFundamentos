%{
#include "Symbol_table.h"
#include "nodes.h"
#include "parser.hpp"
#include <stdlib.h>

extern int yylineno;
%}

%option yylineno

%%
-?[0-9]+(\.[0-9]+)?         { yylval.num_ = atof(yytext); 
                              return NUM; }
(true|false)                {  yylval.bool_ = strdup(yytext);
                              return V_BOOL;}
"if"                        { return IF;}
"int"                       { return INT;}
"bool"                      { return BOOL;}
"double"                    { return DOUBLE;}
"string"                    { return STR;}
"cout"                      { return PRINT;}
"("                         { return LPAREN;}
")"                         { return RPAREN;}
"{"                         { return LBRACE;}
"}"                         { return RBRACE;}
"="                         { return INIT;}
"**"                        { return POW; }
"+="                        { return INCP; }
"=+"                        { return POSTINC; }
"+"                         { return ADD; }
"-"                         { return SUB; }
"*"                         { return MUL; }
"=="                        { return EQ;}
">"                         { return GR;}
"<"                         { return WR;}
">="                        { return EQ_GR;}
"<="                        { return EQ_WR;}
"/"                         { return DIV; }
[\n]                        { return '\n'; }
[ \t]+              ;       // ignorar espacios y tabs
\"([^\\\"]|\\.)*\"          {yytext[yyleng - 1] = '\0';
                             yylval.str_ = strdup(yytext + 1);
                             return STRING;
                            }
[a-zA-Z_][a-zA-Z0-9_]*      { yylval.str_ = strdup(yytext); 
                              return VAR; }
.                           { printf("Token no reconocido %s\n", yytext);}

%%

int yywrap() { return 1; }
