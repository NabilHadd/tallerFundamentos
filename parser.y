%{
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "Symbol_table.h"
#include "utils.h"
extern int yylex();
extern int yylineno;
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
    | DOUBLE VAR '\n'              {table.insert($2, std::make_unique<Symbol_base>(Type::TYPE_DOUBLE, 0 ));}
    | BOOL VAR '\n'                {table.insert($2, std::make_unique<Symbol_base>(Type::TYPE_BOOL, true));} 
    | STR VAR '\n'                 {table.insert($2, std::make_unique<Symbol_base>(Type::TYPE_STRING, ""));}
    | INT VAR '\n'                 {table.insert($2, std::make_unique<Symbol_base>(Type::TYPE_INT, 0));}
    | PRINT LPAREN exp RPAREN '\n'            { print_value($3->get_type(), $3->get_value()); }
    | INT VAR INIT exp '\n'        {if($4->get_type() == Type::TYPE_INT){

                                        table.insert($2, std::make_unique<Symbol_base>(Type::TYPE_INT, std::get<int>($4->get_value())));

                                    }else if($4->get_type() == Type::TYPE_DOUBLE){

                                        table.insert($2, std::make_unique<Symbol_base>(Type::TYPE_INT, static_cast<int>(std::get<double>( $4->get_value() ))));

                                    }else{yyerror("Tipo incompatible con int");}
                                    
                                   }

    | DOUBLE VAR INIT exp '\n'     {if($4->get_type() == Type::TYPE_INT){

                                        table.insert($2, std::make_unique<Symbol_base>(Type::TYPE_DOUBLE, static_cast<double>(std::get<int>($4->get_value()))));

                                    }else if($4->get_type() == Type::TYPE_DOUBLE){

                                        table.insert($2, std::make_unique<Symbol_base>(Type::TYPE_DOUBLE, std::get<double>( $4->get_value() )));

                                    }else{yyerror("Tipo incompatible con double");}
                                    
                                   }
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

                                        table.update(name, std::make_unique<Symbol_base>(Type::TYPE_INT, static_cast<int>(std::get<double>(v))));

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
            Type t1 = $1->get_type();
            Type t2 = $2->get_type();
            Value v1 = $1->get_value();
            Value v2 = $2->get_value();
            Value res;

       if (t1 == t2) {
            Value res;
            std::visit([&](auto&& a, auto&& b) {
                using A = std::decay_t<decltype(a)>;
                using B = std::decay_t<decltype(b)>;

                if constexpr (std::is_same_v<A, B>) {
                    res = a + b;
                } else {
                    yyerror("Error interno: combinación de tipos inválida");
                 }
            }, v1, v2);
            $$ = new Symbol_base(t1, res);
        }else if (t1 == Type::TYPE_INT && t2 == Type::TYPE_INT){
            int a = std::get<int>(v1);
            int b = std::get<int>(v2);
            res = a + b;
            $$ = new Symbol_base(Type::TYPE_INT, res);

        } else if(t1 == Type::TYPE_DOUBLE && t2 == Type::TYPE_INT){
            double a = std::get<double>(v1);
            int b = std::get<int>(v2);
            res = a + b;
            $$ = new Symbol_base(Type::TYPE_DOUBLE, res);
        }else if(t2 == Type::TYPE_DOUBLE && t1 == Type::TYPE_INT){
            int a = std::get<int>(v1);
            double b = std::get<double>(v2);
            res = a + b;
            $$ = new Symbol_base(Type::TYPE_DOUBLE, res);

        }else if(t1 == Type::TYPE_DOUBLE && t2 == Type::TYPE_DOUBLE){
            double a = std::get<double>(v1);
            double b = std::get<double>(v2);
            res = a + b;
            $$ = new Symbol_base(Type::TYPE_DOUBLE, res);

        }else if(t1 == Type::TYPE_STRING && t2 == Type::TYPE_STRING){
            //std::string a = std::get<>
            std::cout<<"";        

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
        }else{
            yyerror("tipos incompatibles para resta");
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
        }else{
            yyerror("tipos incompatibles para multiplicacion");
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
        }else{
            yyerror("tipos incompatibles para division");        
        }
    }
    //| exp exp POW           { $$ = pow($1, $2); }
    //| exp INCP              { $$ = ++$1; }
    //| exp POSTINC           { $$ = $1++; }
    ;
%%

void print_value(Type t, Value v) {
    std::visit([t](auto&& arg){
        switch (t){
            case Type::TYPE_INT:
            case Type::TYPE_DOUBLE:
                std::cout << arg << std::endl;
                break;
            case Type::TYPE_BOOL:
                //std::cout << (arg ? "true": "false") << std::endl;
                break;
            case Type::TYPE_STRING:
                std::cout << arg << std::endl;
                break;
            default:
                std::cout<<"no deberia entrar aca nunca"<<std::endl;
        }    
    }, v);
}

int yyerror(const char* s){
    std::cerr << "Error in line "<< yylineno << ": "<< s << std::endl;
    exit(EXIT_FAILURE);
    return 0;
}


int main(void) {
    yyparse();
    table.clean_table();
    return 0;
}

