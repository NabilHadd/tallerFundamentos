%{
#include <iostream>
#include <stdio.h>
#include "Symbol_table.h"
#include "utils.h"
#include "nodes.h"

extern int          yylex();
extern int          yylineno;
Symbol_table        table;
Body_node           program;//hay que limpiarlo con delete despues para borrar cada nodo vdd????

%}


%union {
    Expr_node* expr_;
    std::vector<Statment_node*>* stmts_;
    Statment_node*  stmt_;
    Body_holder_node*      body_holder_;
    Body_node*  body_;       

    Type_id*        type_;
    Logic_op*       l_op_;

    Symbol_base*    var_;

    double          num_;
    bool            bool_;
    char*           str_;
}

%token  <num_>      NUM
%token  <str_>      VAR
%token  <str_>      STRING
%token  <bool_>     T_BOOL
%token  <bool_>     F_BOOL

%type   <expr_>      exp
%type   <l_op_>     L_op
%type   <type_>     type_id
%type   <stmt_>     line_non_empty
%type   <body_holder_>     scope
%type   <stmts_>    scoped_lines



%token INIT INT DOUBLE BOOL STR
%token PRINT
%token IF ELSE WHILE
//cambio de "token a nonasoc, deberia ser lo mismo"
%nonassoc ADD SUB MUL DIV POW INCP POSTINC
%nonassoc EQ GR WR EQ_GR EQ_WR
%nonassoc LPAREN
%nonassoc RPAREN
%nonassoc LBRACE
%nonassoc RBRACE



%%



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
    | VAR                   { 
        $$ = new Var_node($1, &table);
    }
    | exp exp L_op {
        $$ = new Logic_node($1, $2, $3);
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
    } | WR {
        $$ = new Logic_op(Logic::IS_WR);
    } | EQ_GR {
        $$ = new Logic_op(Logic::IS_EQ_GR);
    } | EQ_WR {
        $$ = new Logic_op(Logic::IS_EQ_WR);
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


int main(void) {
    yyparse();
    program.execute();
    table.clean_table();
    return 0;
}

