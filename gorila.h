#ifndef GORILA_H
#define GORILA_H

#include <string>
#include <iostream>

using std::string;
extern int yylineno;
void is_bool(Node* node);
void is_num(Node* node);
bool calc_relop(Node* left, Node* op, Node* right);
bool cast_type(Node* node1, Node* node2);
void is_byte(Node* node);

class Node{
public:
    string value; // The actual text (lexema)
    string type; // The inheriting class
    Node(string val) : value(val), type(""){}
    Node(string val, string type): value(val), type(type) {}
};

class Type : Node{
public:

    Type(Node* node) : Node(node->value, "Type"){}
};

class ID : Node {
public:
    string int_byte_bool_func;
    ID(Node* node) : Node(node->value, "ID"), int_byte_bool_func(""){}
};

class NUM : Node{
public:

    NUM(Node* node) : Node(node->value, "NUM"){}
};

class NUMB : Node{
public:

    NUMB(Node* node) : Node(node->value, "NUMB"){}
};

class STRING : Node{
public:

    STRING(Node* node) : Node(node->value, "STRING"){}
};

class BOOL : Node{
public:

    BOOL(Node* node) : Node(node->value, "BOOL"){}
    BOOL(string val) : Node(val, "BOOL"){}
};

class EXP : Node{
public:


    EXP(Node* node) : Node(node->value, "EXP"){}
};


#endif /* GORILA_H */