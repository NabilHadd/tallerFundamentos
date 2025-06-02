#include "var_table.h"

Symbol *var_table = NULL;


Symbol *seeker(char *name){

    Symbol *var = var_table;
    while(var != NULL){
         if(!strcmp(var->name, name)){
            return var;
         }
         var = var->next;
    }
    return NULL;

}


void insert(char *name, Type type, Value value) {
    Symbol *new_var = (Symbol *)malloc(sizeof(Symbol));
    if (!new_var) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el símbolo.\n");
        exit(1);
    }
    new_var->name = strdup(name); // Copia el nombre
    if (!new_var->name) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el nombre del símbolo.\n");
        free(new_var);
        exit(1);
    }
    new_var->type = type;
    new_var->value = value;
    new_var->next = var_table; // Agregar al inicio de la lista
    var_table = new_var;
}


void update(char *name, Value value){
    Symbol *var = seeker(name);
    if(var == NULL){
        yyerror("Variable no definida");    
    }else{
        var->value = value;    
    }

}



Value get_value(char *name){
    Symbol *var = seeker(name);
    if(var == NULL){
        yyerror("Variable no definida");
    }else{
        return var->value;    
    }
    return var->value; // This line is unreachable, but added to avoid compiler warning
}

Type get_type(char *name){
    Symbol *var = seeker(name);
    if(var == NULL){
        yyerror("Variable no definida");
    }else{
        return var->type;
    }
    return  TYPE_INT;
}


void print_value(Value value, Type type){
    switch(type){
        case TYPE_INT: printf("%d\n", value.i_value); break;
        case TYPE_DOUBLE: printf("%f\n", value.d_value); break;
        case TYPE_BOOL: 
            if(value.b_value){
                printf("true\n");            
            }else{
                printf("false\n");            
            }
            break;
        case TYPE_STRING: printf("%s\n", value.s_value); break;
        default: printf("Tipo no reconocido\n");
    }
}


void free_memory(){
    Symbol *var = var_table;
    while(var != NULL){
        free(var->name);
        Symbol *tmp = var;
        var = var->next;
        free(var);  
    }
}

int yyerror(const char *s) {
    printf("syntax error\n");
    return 0;
}

