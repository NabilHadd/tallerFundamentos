#ifndef FUNC_TABLE_H
#define FUNC_TABLE_H

#include <unordered_map>
#include <memory>
#include <string>
#include "utils.h"


class Func_table {
private:
    std::unordered_map<std::string, std::unique_ptr<Body_node>> table;

public:
    bool has(const std::string& name) const;
    void insert(const std::string& name, std::unique_ptr<Body_node> body);
    Body_node* get(const std::string& name) const;
    void update(const std::string& name, std::unique_ptr<Body_node> body);
    void print_table();

    void erase_func(const std::string& name);
    void clean_table();

};


#endif
