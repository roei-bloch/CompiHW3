#ifndef GORILA_H
#define GORILA_H

#include <string>
#include <iostream>

using std::string;
extern int yylineno;
extern int global_while_ctr
Node* is_bool(Node* node);
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
    string get_value() {return value;}
    void set_value(string str) {this->value = str;}
    string get_type() {return type;}
    void set_type(string str) {this->type = str;}
    virtual ~Node() {}
    private:
};

class Type : public Node{
public:
    Type(Node* node, string type) : Node(node->value, type){}
};

class ID : public Node {
public:
    ID(Node* node) : Node(node->value, "") {}
};

class NUM : public Node{
public:

    NUM(Node* node) : Node(node->value, "INT"){}
};

class NUMB : public Node{
public:

    NUMB(Node* node) : Node(node->value, "BYTE"){}
};

class STRING : public Node{
public:

    STRING(Node* node) : Node(node->value, "STRING"){}
};

class BOOL : public Node{
public:

    BOOL(Node* node) : Node(node->value, "BOOL"){}
    BOOL(string val) : Node(val, "BOOL"){}
};

class EXP : public Node{
public:


    EXP(Node* node) : Node(node->value, "EXP"){}
};


#endif /* GORILA_H */