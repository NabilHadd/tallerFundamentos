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
"true"                      { yylval.bool_ = strdup(yytext);
                              return T_BOOL;}
"false"                     { yylval.bool_ = strdup(yytext);
                              return F_BOOL;}
"if"                        { return IF;}
"else"                      { return ELSE;}
"while"                     { return WHILE;}
"int"                       { return INT;}
"bool"                      { return BOOL;}
"double"                    { return DOUBLE;}
"string"                    { return STR;}
"->"                        { return PARSE;}
"cout"                      { return PRINT;}
"cin"                       { return SCAN;}
"scan"                      { return SCAN;}
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
"not"                       { return NOT;}
"and"                       { return AND;}
"or"                        { return OR;}
"=="                        { return EQ;}
">"                         { return GR;}
"<"                         { return WR;}
">="                        { return EQ_GR;}
"<="                        { return EQ_WR;}
"/"                         { return DIV; }
";"                         { return ';';}
[\n]                ;
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
