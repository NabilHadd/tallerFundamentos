#include "Symbol_base.h"

Symbol_base::Symbol_base(Type t, const Value& v) : type(t), value(v) {}

Symbol_base::~Symbol_base() {}

Type Symbol_base::get_type() const {
    return this->type;
}

Value Symbol_base::get_value() const {
    return this->value;
}

void Symbol_base::set_type(Type t) {
    this->type = t;
}

void Symbol_base::set_value(const Value& v) {
    this->value = v;
}
