%{
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "Symbol_table.h"
#include "utils.h"
#include "nodes.h"
#include "Func_table.h"

extern int          yylex();
extern int          yylineno;
Symbol_table        table;
Func_table          f_table;
Body_node           program;//hay que limpiarlo con delete despues para borrar cada nodo vdd????

%}


%union {
    //Retorno de producciones.
    std::vector<Statment_node*>*    stmts_;
    Statment_node*                  stmt_;      
    Expr_node*                      expr_;

    //Generalizaciones.
    Type_id*                        type_;
    Logic_op*                       l_op_;

    //Auxiliares.
    Body_holder_node*               body_holder_;
    Symbol_base*                    var_;
    Body_node*                      body_; 

    //Tipos de datos.
    double                          num_;
    bool                            bool_;
    char*                           str_;
}

//Tokens para lectura de valores crudos.
%token  <num_>      NUM
%token  <str_>      VAR
%token  <str_>      STRING
%token  <bool_>     T_BOOL
%token  <bool_>     F_BOOL

//Tipos de dato en producciones.
%type   <expr_>      exp
%type   <l_op_>     L_op
%type   <type_>     type_id
%type   <stmt_>     line_non_empty
%type   <body_holder_>     scope
%type   <stmts_>    scoped_lines


//Tokens para declaracion e inicializacion.
%token INIT INT DOUBLE BOOL STR

//Tokens para statments
%token IF ELSE WHILE PRINT SCAN

//Operadores aritmeticos
%nonassoc ADD SUB MUL DIV POW INCP POSTINC 

//Operadores logicos
%nonassoc EQ GR WR EQ_GR EQ_WR OR AND NOT ARROW

//parentesis.
%nonassoc LPAREN RPAREN

//llaves.
%nonassoc LBRACE RBRACE



%%
//Producciones


input: 
    /*vacio*/
    | input line_non_empty {
        program.add_statment(std::unique_ptr<Statment_node>($2));
    }
    ;

line_non_empty:
    type_id VAR ';'                  {
        $$ = new Dec_node($1->get_id(), $2, &table);
        
    }
    | type_id VAR INIT exp ';'         {
        $$ = new Dec_ins_node($1->get_id(), $2, $4, &table);
        
    }
    | VAR INIT exp ';'                 {
        $$ = new Ins_node($1, $3, &table);

    }
    | IF LPAREN exp RPAREN scope    {
        auto body_holder = dynamic_cast<Body_holder_node*>($5);
        $$ = new If_node($3, std::move(body_holder->body));
        delete body_holder;

    }
    | IF LPAREN exp RPAREN scope ELSE line_non_empty    {
        auto body_holder = dynamic_cast<Body_holder_node*>($5);
        $$ = new If_else_node($3, std::move(body_holder->body), std::unique_ptr<Statment_node>($7));
        delete body_holder;
    }
    | IF LPAREN exp RPAREN scope ELSE scope  {
        auto body_holder = dynamic_cast<Body_holder_node*>($5);
        auto branch_holder = dynamic_cast<Body_holder_node*>($7);
        $$ = new If_else_scope_node($3, std::move(body_holder->body), std::move(branch_holder->body));
        delete body_holder;
        delete branch_holder;
    }
    | WHILE LPAREN exp RPAREN scope  {
        auto body_holder = dynamic_cast<Body_holder_node*>($5);
        $$ = new While_node($3, std::move(body_holder->body));
        delete body_holder;
    }
    | PRINT LPAREN exp RPAREN ';'      {
        $$ = new Print_node($3);

    }
    | SCAN LPAREN VAR RPAREN ';'        {
        $$ = new Scan_node($3, &table);    
    }
    | ARROW VAR LPAREN RPAREN scope {
        auto body_holder = dynamic_cast<Body_holder_node*>($5);
        $$ = new Dec_func_node($2, &f_table, std::move(body_holder->body));
    }
    | VAR LPAREN RPAREN{
        $$ = new Execute_node($1, &f_table);
    }
    ;


/*line:
    '\n'
    | line_non_empty {
        $1->execute();        
    }
    ;*/
    



exp:
    NUM                     { 
        $$ = new Const_node(Type::TYPE_DOUBLE, $1);                
    }
    |T_BOOL                 {
        $$ = new Const_node(Type::TYPE_BOOL, true);    
    }
    |F_BOOL                 {
        $$ = new Const_node(Type::TYPE_BOOL, false);
    }
    |STRING                 {
        $$ = new Const_node(Type::TYPE_STRING, $1);
    }
    | exp ARROW type_id     {
        $$ = new Parse_node($3, $1);    
    }
    | VAR                   { 
        $$ = new Var_node($1, &table);
    }
    | exp exp L_op {
        $$ = new Logic_node($1, $2, $3);
    }
    | exp NOT {
        $$ = new Not_node($1);    
    }
    | exp exp ADD           {
        $$  = new Add_node($1, $2);
    }
    | exp exp SUB           {
        $$ = new Sub_node($1, $2);
    }
    | exp exp MUL           {
            
        $$ = new Mul_node($1, $2);
    }

    | exp exp DIV           {

        $$ = new Div_node($1, $2);
    }
    | exp exp POW           { 

        $$ = new Pow_node($1, $2); 
    }
    //| exp INCP              { $$ = ++$1; }
    //| exp POSTINC           { $$ = $1++; }
    ;

L_op: 
    EQ {
        $$ = new Logic_op(Logic::IS_EQ);
    } 
    | GR {
        $$ = new Logic_op(Logic::IS_GR);
    } 
    | WR {
        $$ = new Logic_op(Logic::IS_WR);
    } 
    | EQ_GR {
        $$ = new Logic_op(Logic::IS_EQ_GR);
    } 
    | EQ_WR {
        $$ = new Logic_op(Logic::IS_EQ_WR);
    }
    | AND {
        $$ = new Logic_op(Logic::AND_);
    }
    | OR    {
        $$ = new Logic_op(Logic::OR_);
    }
    ;

type_id: 
    INT         {
        $$ = new Type_id(Type::TYPE_INT);
    } | DOUBLE  {
        $$ = new Type_id(Type::TYPE_DOUBLE);
    } | BOOL    {
        $$ = new Type_id(Type::TYPE_BOOL);
    } | STR     {
        $$ = new Type_id(Type::TYPE_STRING);
    }
    ;

scoped_lines:
    line_non_empty          {
        $$ = new std::vector<Statment_node*>();
        $$->push_back($1);
    }
    |scoped_lines line_non_empty {
        $1->push_back($2);
        $$ = $1;
    }
    ;
scope:
    LBRACE scoped_lines RBRACE {
        std::vector<std::unique_ptr<Statment_node>> body;
        for (auto stmt : *$2)
            body.push_back(std::unique_ptr<Statment_node>(stmt));
        delete $2;
        $$ = new Body_holder_node(std::move(body));    
        }
    ;

%%

bool try_parse_s(const std::string& input, Type t){
    Value out_val;
    try{
        if(t ==Type::TYPE_INT){
            out_val = std::stoi(input);
        }else if(t==Type::TYPE_DOUBLE){
            out_val = std::stod(input);
        }else if(t==Type::TYPE_BOOL){
            return true;
        }else if(t==Type::TYPE_STRING){
            out_val = input;
        }else{
            return false;
        }
        return true;
    }catch(const std::invalid_argument& e){
        return false;
    }catch(const std::out_of_range& e){
        return false;    
    }
    

}

bool eval(Value v){

    bool bool_val;
    std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, std::string>){
            if(arg == "true")
                bool_val = true;
            else
                bool_val = false;
        }else if constexpr (std::is_same_v<T, bool>){
            bool_val = arg;
        }else{
            if (arg > 0)
                bool_val = true;
            else
                bool_val = false;
        }


    }, v);
    return bool_val;
}

int yyerror(const char* s){
    std::cerr << "Error in line "<< yylineno << ": "<< s << std::endl;
    exit(EXIT_FAILURE);
    return 0;
}


int main(int argc, char* argv[]){
    if(argc < 2){
        std::cerr << "Uso: " << argv[0] << " archivo.pol" << std::endl;
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if(!file){
        std::cerr << "No se pudo abrir el archivo: " << argv[1] << std::endl;
        return 1;   
    }

    extern FILE* yyin;
    yyin = file;

    yyparse();
    program.execute();
    table.clean_table();
    fclose(file);
    return 0;
}

