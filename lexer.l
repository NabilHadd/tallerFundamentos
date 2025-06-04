%{
#include "Symbol_table.h"
#include "parser.hpp"
#include <stdlib.h>

extern int yylineno;
%}

%option yylineno

%%
-?[0-9]+(\.[0-9]+)?         { yylval.num = atof(yytext); return NUM; }
"int"                       { return INT;}
"bool"                      { return BOOL;}
"double"                    {return DOUBLE;}
"string"                    {return STR;}
"cout"                      {return PRINT;}
"("                         {return LPAREN;}
")"                         {return RPAREN;}
"="                         { return INIT;}
"**"                        { return POW; }
"+="                        { return INCP; }
"=+"                        { return POSTINC; }
"+"                         { return ADD; }
"-"                         { return SUB; }
"*"                         { return MUL; }
"/"                         { return DIV; }
[\n]                        { return '\n'; }
[ \t]+              ;       // ignorar espacios y tabs
\"([^\\\"]|\\.)*\"          {yytext[yyleng - 1] = '\0';
                             yylval.str = strdup(yytext + 1);
                             return STRING;
                            }
[a-zA-Z_][a-zA-Z0-9_]*      { yylval.str = strdup(yytext); return VAR; }
.                           { printf("Token no reconocido %s\n", yytext);}

%%

int yywrap() { return 1; }
