#include "Symbol_base.h"


Symbol_base::Symbol_base(Type t, Value v): type(t), value(v) {}

Symbol_base::~Symbol_base(){}

Type Symbol_base::get_type() const {return this->type;}
Value Symbol_base::get_value() const {return this->value;}


void Symbol_base::set_type(Type t){
    this->type = t;
}
void Symbol_base::set_value(Value v){
    this->value = v;
}




