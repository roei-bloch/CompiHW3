
#include "gorila.h"
#include "hw3_output.hpp"
#include <assert.h>
#include "symbol_table_dinosaur.h"





void is_bool(Node* node)
{
    if (node->type != "BOOL")
    {
        output::errorMismatch(yylineno);
        exit(0);
    }
}



void is_num(Node* node)
{
    if (node->type != "INT" && node->type != "BYTE")
    {
        output::errorMismatch(yylineno);
        exit(0);
    }
}

bool calc_relop(Node* left, Node* op, Node* right)
{
    try{
        int left_num = std::stoi(left->value);
        int right_num = std::stoi(right->value);
        if (op->value == "<"){
            return left_num < right_num;
        } 
        else if (op->value == ">"){
            return left_num > right_num;
        } 
        else if (op->value == "<="){
            return left_num <= right_num;
        }
        else if (op->value == ">="){
            return left_num >= right_num;
        } 
        else if (op->value == "=="){
            return left_num == right_num;
        }
        else if (op->value == "!="){
            return left_num != right_num;
        }

    } 
    catch (std::exception &e)
    {
        assert(false);
        output::errorMismatch(yylineno);
        exit(0);
    }
}


bool is_id_num(Node* node)
{
    try{
        ID *id_chan = dynamic_cast<ID *>(node);
        return (id_chan->type == "INT" || id_chan->type == "BYTE");
    } catch (std::exception &e) {
        return false;
    }
}
// if return type is true - number
// if return type is false - var
Node* try_number_cast_type(Node* type, Node* cast_me_senpai) // can we cast int to int?
{
    if (type->value !="int" && type->value != "byte")
    {
        output::errorMismatch(yylineno);
        exit(0);
    }
    if (cast_me_senpai->type != "INT" && cast_me_senpai->type != "BYTE")
    {
        if (!(is_id_num(cast_me_senpai)))
        {
            output::errorMismatch(yylineno);
            exit(0);
        } else {
            // search for its value in symbol table, and return a new node of type "type.value" and value from symbo table
            Symbol* s = trex->search_symbol(cast_me_senpai->value);
            if(s == NULL){
                output::errorUndef(yylineno, cast_me_senpai->value);
                exit(0);
            } else {
                return new Node(std::to_string(s->numerical_value), type->type);
            }
        }
    } else {
        if (type->value == "int"){
            return new Node(cast_me_senpai->value, "INT");
        } else if(type->value == "byte"){
            return new Node(cast_me_senpai->value, "BYTE");
        }

    }
}



void is_byte(Node* node)
{
    try
    {
        unsigned int byte_num = std::stoi(node->value);
        if (byte_num > 255)
        {
            output::errorMismatch(yylineno);
            exit(0);
        }
    }
    catch (std::exception &e)
    {
        assert(false);
        output::errorMismatch(yylineno);
        exit(0);
    }
}

Node* call_function(Node* func, Node* argument) {
    Func* current_function = trex->search_func(func->value);
    if (current_function == NULL){
        output::errorUndefFunc(yylineno, func->value);
        exit(0);
    } else if(current_function->argument_type == argument->type){
        if(current_function->output_type != "VOID"){
            return new Node(argument->value, "INT");
        }
        return NULL;
    } else if(current_function->argument_type == "INT" && argument->type == "BYTE"){
        if(current_function->output_type != "VOID"){
            return new Node(argument->value, "INT");
        }
        return NULL;
    }
    output::errorPrototypeMismatch(yylineno, func->value, current_function->argument_type);
    exit(0);

}

