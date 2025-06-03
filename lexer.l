%{
#include "Symbol_table.h"
#include "parser.hpp"
#include <stdlib.h>
%}

%%
[0-9]+(\.[0-9]+)?           { yylval.num = atof(yytext); return NUM; }
"int"                       { return INT;}
"bool"                      { return BOOL;}
"double"                    {return DOUBLE;}
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
[a-zA-Z_][a-zA-Z0-9_]*      { yylval.str = strdup(yytext); return VAR; }
.                           { printf("Token no reconocido %s\n", yytext);}

%%

int yywrap() { return 1; }
