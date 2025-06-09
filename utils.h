#ifndef UTILS_H
#define UTILS_H

#include "Symbol_base.h"

enum class Logic {
    IS_EQ,
    IS_GR,
    IS_WR,
    IS_EQ_GR,
    IS_EQ_WR,
    OR_,
    AND_
};

//void print_value(Value v);
bool eval(Value v);
std::string to_string_value(Type t, const Value& v);
int yyerror(const char* s);



#endif
