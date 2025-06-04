%{
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "Symbol_table.h"
#include "utils.h"
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
%token <str> STRING
%type <var> exp

%token ADD SUB MUL DIV POW INCP POSTINC 
%token INIT INT DOUBLE BOOL STR
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
    | INT VAR '\n'                 {table.insert($2, std::make_unique<Symbol_base>(Type::TYPE_INT, 0));}
    | PRINT LPAREN exp RPAREN '\n'            { print_value($3->get_type(), $3->get_value()); }
    | INT VAR INIT exp '\n'        {if($4->get_type() == Type::TYPE_INT){

                                        table.insert($2, std::make_unique<Symbol_base>(Type::TYPE_INT, std::get<int>($4->get_value())));

                                    }else if($4->get_type() == Type::TYPE_DOUBLE){

                                        table.insert($2, std::make_unique<Symbol_base>(Type::TYPE_INT, static_cast<int>(std::get<double>( $4->get_value() ))));

                                    }else{yyerror("Tipo incompatible con int");}
                                    
                                   }

    | DOUBLE VAR INIT exp '\n'        {table.insert($2, std::make_unique<Symbol_base>(Type::TYPE_DOUBLE, std::get<double>($4->get_value())));}
    | BOOL VAR INIT exp '\n'        {table.insert($2, std::make_unique<Symbol_base>(Type::TYPE_BOOL, std::get<bool>($4->get_value())));}
    | STR VAR INIT exp '\n'         {table.insert($2, std::make_unique<Symbol_base>(Type::TYPE_STRING, std::get<std::string>($4->get_value())));}//aplicar casteo a string
    | VAR INIT exp '\n'             {
                                    std::string name = $1;
                                    Type t = $3->get_type();
                                    Value v = $3->get_value();
                                    Symbol_base* var = table.get(name);
                                    if(var->get_type() == t){

                                        table.update(name, std::make_unique<Symbol_base>(t, v));

                                    }else if(var->get_type() == Type::TYPE_INT and t == Type::TYPE_DOUBLE){

                                        table.update(name, std::make_unique<Symbol_base>(t, static_cast<int>(std::get<double>(v))));

                                    }else if(var->get_type() == Type::TYPE_DOUBLE and t == Type::TYPE_INT){

                                        table.update(name, std::make_unique<Symbol_base>(t, static_cast<double>(std::get<int>(v))));

                                    }else if (var->get_type() == Type::TYPE_STRING) {
                                        std::string name = $1;
                                        Symbol_base* val = $3;
                                        std::visit([&](auto&& arg) {
                                            std::string str_val;
                                            using T = std::decay_t<decltype(arg)>;
                                            if constexpr (std::is_same_v<T, std::string>)
                                                str_val = arg;
                                            else if constexpr (std::is_same_v<T, bool>)
                                                str_val = arg ? "true" : "false";
                                            else
                                                str_val = std::to_string(arg);
                                            table.update(name, std::make_unique<Symbol_base>(Type::TYPE_STRING, str_val));
                                        }, val->get_value());
                                    } else {
                                        yyerror("tipos incompatibles");
                                    }
                                    }

    ;



exp:
    NUM                     { 
        $$ = new Symbol_base(Type::TYPE_DOUBLE, $1);                
    }
    |STRING                 {
        $$ = new Symbol_base(Type::TYPE_STRING, $1);
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
            int a = std::get<int>($1->get_value());
            double b = std::get<double>($2->get_value());
            double res = a + b;
            $$ = new Symbol_base(Type::TYPE_DOUBLE, res);
        }else if($1->get_type()==Type::TYPE_DOUBLE && $2->get_type()==Type::TYPE_DOUBLE){
            double a = std::get<double>($1->get_value());
            double b = std::get<double>($2->get_value());
            double res = a + b;
            $$ = new Symbol_base(Type::TYPE_DOUBLE, res);
        }
    }
    | exp exp SUB           {
        if ($1->get_type() == Type::TYPE_INT && $2->get_type() == Type::TYPE_INT){
            int a = std::get<int>($1->get_value());
            int b = std::get<int>($2->get_value());
            int res = a - b;
            $$ = new Symbol_base(Type::TYPE_INT, res);

        } else if($1->get_type() == Type::TYPE_DOUBLE && $2->get_type() == Type::TYPE_INT){
            double a = std::get<double>($1->get_value());
            int b = std::get<int>($2->get_value());
            double res = a - b;
            $$ = new Symbol_base(Type::TYPE_DOUBLE, res);
        }else if($2->get_type() == Type::TYPE_DOUBLE && $1->get_type() == Type::TYPE_INT){
            int a = std::get<int>($1->get_value());
            double b = std::get<double>($2->get_value());
            double res = a - b;
            $$ = new Symbol_base(Type::TYPE_DOUBLE, res);
        }else if($1->get_type()==Type::TYPE_DOUBLE && $2->get_type()==Type::TYPE_DOUBLE){
            double a = std::get<double>($1->get_value());
            double b = std::get<double>($2->get_value());
            double res = a - b;
            $$ = new Symbol_base(Type::TYPE_DOUBLE, res);
        }
    }
    | exp exp MUL           {
        if ($1->get_type() == Type::TYPE_INT && $2->get_type() == Type::TYPE_INT){
            int a = std::get<int>($1->get_value());
            int b = std::get<int>($2->get_value());
            int res = a * b;
            $$ = new Symbol_base(Type::TYPE_INT, res);

        } else if($1->get_type() == Type::TYPE_DOUBLE && $2->get_type() == Type::TYPE_INT){
            double a = std::get<double>($1->get_value());
            int b = std::get<int>($2->get_value());
            double res = a * b;
            $$ = new Symbol_base(Type::TYPE_DOUBLE, res);
        }else if($2->get_type() == Type::TYPE_DOUBLE && $1->get_type() == Type::TYPE_INT){
            int a = std::get<int>($1->get_value());
            double b = std::get<double>($2->get_value());
            double res = a * b;
            $$ = new Symbol_base(Type::TYPE_DOUBLE, res);
        }else if($1->get_type()==Type::TYPE_DOUBLE && $2->get_type()==Type::TYPE_DOUBLE){
            double a = std::get<double>($1->get_value());
            double b = std::get<double>($2->get_value());
            double res = a * b;
            $$ = new Symbol_base(Type::TYPE_DOUBLE, res);
        }
    }
    | exp exp DIV           {
        if ($1->get_type() == Type::TYPE_INT && $2->get_type() == Type::TYPE_INT){
            int a = std::get<int>($1->get_value());
            int b = std::get<int>($2->get_value());
            int res = a / b;
            $$ = new Symbol_base(Type::TYPE_INT, res);

        } else if($1->get_type() == Type::TYPE_DOUBLE && $2->get_type() == Type::TYPE_INT){
            double a = std::get<double>($1->get_value());
            int b = std::get<int>($2->get_value());
            double res = a / b;
            $$ = new Symbol_base(Type::TYPE_DOUBLE, res);
        }else if($2->get_type() == Type::TYPE_DOUBLE && $1->get_type() == Type::TYPE_INT){
            int a = std::get<int>($1->get_value());
            double b = std::get<double>($2->get_value());
            double res = a / b;
            $$ = new Symbol_base(Type::TYPE_DOUBLE, res);
        }else if($1->get_type()==Type::TYPE_DOUBLE && $2->get_type()==Type::TYPE_DOUBLE){
            double a = std::get<double>($1->get_value());
            double b = std::get<double>($2->get_value());
            double res = a / b;
            $$ = new Symbol_base(Type::TYPE_DOUBLE, res);
        }
    }
    //| exp exp POW           { $$ = pow($1, $2); }
    //| exp INCP              { $$ = ++$1; }
    //| exp POSTINC           { $$ = $1++; }
    ;
%%

void print_value(Type t, Value v) {
    Type type = t;
    Value value = v;

    switch(type){
        case Type::TYPE_INT: std::cout << std::get<int>(value) << std::endl; break;
        case Type::TYPE_DOUBLE: std::cout << std::get<double>(value) << std::endl; break;
        case Type::TYPE_BOOL: 
            std::cout << (std::get<bool>(value) ? "true" : "false") << std::endl;
            break;
        case Type::TYPE_STRING: std::cout << std::get<std::string>(value) << std::endl; break;
        default: std::cout << "<16800002x1>" << std::endl;
    }
}

int yyerror(const char*){
    std::cerr << "syntax error: " << std::endl;
    return 0;
}


int main(void) {
    yyparse();
    table.clean_table();
    return 0;
}

