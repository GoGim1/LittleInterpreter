#pragma once 

#include <iostream>
#include <string>
#include <memory>

namespace Lexer
{
    using std::string;
    using std::unique_ptr;

 
    class Token
    {
    public:
        enum Type
        {
            INTEGER, FLOAT, IDENTIFIER, 
            OPERATOR, EQUAL, ASSIGN, PLUS, SUB, MUL, DIV, 
            RESERVED, IF, WHILE, ELSE, VAR,
            UNKNOWN
        };
    
    public:
        typedef unique_ptr<Token> Ptr;

        Token(Type t = Type::UNKNOWN, string v = "", int n = -1): type(t), value(v), lineNum(n) {}


        // (DEBUG)
        void Dump()
        {
            std::cout << type << " " << value << " " << lineNum << std::endl;
        }

        Type        type;
        string      value;
        int         lineNum;
    };
};