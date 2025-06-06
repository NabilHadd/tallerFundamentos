/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUM = 258,                     /* NUM  */
    VAR = 259,                     /* VAR  */
    STRING = 260,                  /* STRING  */
    V_BOOL = 261,                  /* V_BOOL  */
    INIT = 262,                    /* INIT  */
    INT = 263,                     /* INT  */
    DOUBLE = 264,                  /* DOUBLE  */
    BOOL = 265,                    /* BOOL  */
    STR = 266,                     /* STR  */
    PRINT = 267,                   /* PRINT  */
    IF = 268,                      /* IF  */
    ELIF = 269,                    /* ELIF  */
    ELSE = 270,                    /* ELSE  */
    WHILE = 271,                   /* WHILE  */
    ADD = 272,                     /* ADD  */
    SUB = 273,                     /* SUB  */
    MUL = 274,                     /* MUL  */
    DIV = 275,                     /* DIV  */
    POW = 276,                     /* POW  */
    INCP = 277,                    /* INCP  */
    POSTINC = 278,                 /* POSTINC  */
    EQ = 279,                      /* EQ  */
    GR = 280,                      /* GR  */
    WR = 281,                      /* WR  */
    EQ_GR = 282,                   /* EQ_GR  */
    EQ_WR = 283,                   /* EQ_WR  */
    LPAREN = 284,                  /* LPAREN  */
    RPAREN = 285,                  /* RPAREN  */
    LBRACE = 286,                  /* LBRACE  */
    RBRACE = 287                   /* RBRACE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 20 "parser.y"

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

#line 112 "parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (Body_node* program);


#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
