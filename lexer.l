%{
#include "parser.tab.h"
#include <stdlib.h>
%}

%%
[0-9]+(\.[0-9]+)?   { yylval.num = atof(yytext); return NUM; }
"**"                { return POW; }
"+="                { return INCP; }
"=+"                { return POSTINC; }
"+"                 { return ADD; }
"-"                 { return SUB; }
"*"                 { return MUL; }
"/"                 { return DIV; }
[\n]                { return '\n'; }
[ \t]+              ; // ignorar espacios y tabs
.                   { return yytext[0]; }
%%

int yywrap() { return 1; }
