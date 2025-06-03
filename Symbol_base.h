#ifndef SYMBOL_BASE_H
#define SYMBOL_BASE_H


#include <iostream>
#include <string>
#include <variant>

enum class Type {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_BOOL,
    TYPE_STRING
};

using Value = std::variant<int, double, bool, std::string>


class Symbol_base{

private:
    Type type;
    Value value;

public:
    Type get_type() const;
    Value get_value() const;

    void set_type(Type t);
    void set_value(Value v);

    Symbol_base(Type t, Value v);
    virtual ~Symbol_base();

};


#endif
