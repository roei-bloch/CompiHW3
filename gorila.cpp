
#include "gorila.h"
#include "hw3_output.hpp"
#include <assert.h>
#include "symbol_table_dinosaur.h"


using std::string;

int global_while_ctr = 0;

Node* is_bool(Node* node)
{
    string bool_val;
    if (is_id(node) && node->type == "BOOL")
    {
        Symbol *s = trex->search_symbol(node->value);
        if(!s){
            output::errorUndef(yylineno, node->value);
            exit(0);
        }
        bool_val = s->numerical_value == 1 ? "true" : "false";
    }
    else
    {
        bool_val = node->value;
    }
    if (node->type != "BOOL")
    {
        output::errorMismatch(yylineno);
        exit(0);
    }
    return new Node(bool_val, "BOOL");
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
    int left_num, right_num;
    try{
        if (is_id(left))
        {
            Symbol *s = trex->search_symbol(left->value);
            if (!s)
            {
                output::errorUndef(yylineno, left->value);
                exit(0);
            }
            if (s->type != "INT")
            left_num = s->numerical_value;
        }
        else{
            left_num = std::stoi(left->value);
        }
        if (is_id(right))
        {
            Symbol *s = trex->search_symbol(right->value);
            if (!s)
            {
                output::errorUndef(yylineno, right->value);
                exit(0);
            }
            left_num = s->numerical_value;
        }
        else {
            right_num = std::stoi(right->value);
        }

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


bool is_id(Node* node)
{
    try{
        ID *id_chan = dynamic_cast<ID *>(node);
        return true;
    } catch (std::exception &e) {
        return false;
    }
}

bool is_id_num(Node* node)
{
    return is_id(node) && (node->type == "INT" || node->type == "BYTE");
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
            return new Node(cast_me_senpai->value, type->type);
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
        output::errorMismatch(yylineno);
        exit(0);
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

void add_symbol(Node* type, Node* new_symbol, Node* assigned_node = NULL){
    int new_symbol_numerical_value = 0;
    if (assigned_node)
    {
        if (!((type->type == assigned_node->type) || (type->type == "INT" && assigned_node->type == "BYTE")))
        {
            output::errorMismatch(yylineno);
            exit(0);
        } 
        if (type->type == "STRING"|| assigned_node->type == "STRING" || type->type == "VOID"|| assigned_node->type == "VOID")
        {
            output::errorMismatch(yylineno);
            exit(0);
        }
        if(is_id(assigned_node))
        {
            Symbol *s = trex->search_symbol(assigned_node->value); // int a = b;
            if(s)
            {
                new_symbol_numerical_value = s->numerical_value;
            }
            else{
                output::errorUndef(yylineno, assigned_node->value);
                exit(0);
            }
        }
        else // int a = 5; // bool a = true;
        {
            if (assigned_node->type == "BOOL")
            {
                int val = assigned_node->value == "true" ? 1 : 0;
                new_symbol_numerical_value = val;
            }
            try{
                new_symbol_numerical_value = std::stoi(assigned_node->value);
            }
            catch (std::exception& e) {
                output::errorMismatch(yylineno);
                exit(0);
            }
        }
    }
    trex->add_symbol(new_symbol->value, type->type, new_symbol_numerical_value);
}

void assign_symbol(Node* existing_symbol, Node* assigned_node)
{
    int assigned_node_numerical_value = 0;
    if (!((existing_symbol->type == assigned_node->type) || (existing_symbol->type == "INT" && assigned_node->type == "BYTE")))
    {
        output::errorMismatch(yylineno);
        exit(0);
    }
    if (existing_symbol->type == "VOID" || assigned_node->type=="VOID")
    {
        output::errorMismatch(yylineno);
        exit(0);
    }
    Symbol *existing_s = trex->search_symbol(existing_symbol->value);
    if(!existing_s)
    {
        output::errorUndef(yylineno, existing_symbol->value);
        exit(0);
    }
    if(is_id(assigned_node)){
        Symbol *assigned_s = trex->search_symbol(assigned_node->value);
        if(!assigned_s)
        {
            output::errorUndef(yylineno, existing_symbol->value);
            exit(0);
        } else{
            assigned_node_numerical_value = assigned_s->numerical_value;
        }
    } else {
        try
        {
            if (assigned_node->type == "BOOL")
            {
                int val = assigned_node->value == "true" ? 1 : 0;
                assigned_node_numerical_value = val;
            }
            else{
                assigned_node_numerical_value = std::stoi(assigned_node->value);
            }
        }
        catch (std::exception& e) {
            output::errorMismatch(yylineno);
            exit(0);
        }
    } 
}

void add_scope(bool s_is_braces_scope)
{
    trex->add_scope(s_is_braces_scope);
}

