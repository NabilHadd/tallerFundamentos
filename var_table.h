#ifndef VAR_TABLE_H
#define VAR_TABLE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_BOOL,
    TYPE_STRING
}Type;


typedef union {

    int i_value;
    double d_value;
    int b_value;
    char *s_value;
}Value;


typedef struct Symbol {
    
    char *name;
    Type type;
    Value value;
    struct Symbol *next;        
} Symbol;

Symbol *seeker(char *name);
void insert(char *name, Type type, Value value);
void update(char *name, Value value);
Value get_value(char *name);
Type get_type(char *name);
void print_value(Value value, Type type);
void free_memory();
int yyerror(const char *s);

#endif



