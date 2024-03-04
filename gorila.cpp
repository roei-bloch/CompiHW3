
#include "gorila.h"
#include "hw3_output.hpp"
#include <assert.h>



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
    if (node->type != "NUM" && node->type != "NUMB")
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


bool is_id_represents_num(Node* node)
{
    if (node->type == "ID")
    {
        ID *id_chan = dynamic_cast<ID *>(node);
        return id_chan->int_byte_bool_func == "NUM" || id_chan->int_byte_bool_func == "NUMB";
    }
    return false;
}
// if return type is true - int
// if return type is false - byte
bool cast_type(Node* type, Node* cast_me_senpai) // can we cast int to int?
{
    if ((type->value !="int" && type->value != "byte") || (cast_me_senpai->type!="NUM" && cast_me_senpai->type!="NUMB" && is_id_represents_num(cast_me_senpai)))
    {
        output::errorMismatch(yylineno);
        exit(0);
    }

    return type->value == "int";
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

