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

using Value = std::variant<int, double, bool, std::string>;

class Symbol_base {
private:
    Type type;
    Value value;

public:
    Type get_type() const;
    Value get_value() const;

    void set_type(Type t);
    void set_value(const Value& v); // <-- evitamos copia innecesaria

    Symbol_base(Type t, const Value& v); // <-- evitamos copia innecesaria
    virtual ~Symbol_base(); // <-- solo si planeas heredar
};

#endif
