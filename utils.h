#ifndef UTILS_H
#define UTILS_H

#include "Symbol_base.h"

void print_value(Value v);
std::string to_string_value(Type t, const Value& v);
int yyerror(const char* s);

#endif
