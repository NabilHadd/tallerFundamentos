%{
#include "Symbol_table.h"
#include "parser.h"
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
.                           { printf("Token no reconocido %s\n", yytext);}
[a-zA-Z_][a-zA-Z0-9_]*      { yylval.str = strdup(yytext); return VAR; }
%%

int yywrap() { return 1; }
