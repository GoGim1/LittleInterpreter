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
            OPERATOR, EQUAL, ASSIGN, PLUS, SUB, MUL, DIV, GE, LE, LBRACE, RBRACE, SEMICOLON, LBRACKET, RBRACKET,
            RESERVED, IF, WHILE, ELSE, VAR,
            UNKNOWN
        };
    
    public:
        typedef unique_ptr<Token> Ptr;

        Token(Type t = Type::UNKNOWN, string v = "", int x = -1, int y = -1): type(t), value(v), posX(x) ,posY(y) {}


        // (DEBUG)
        void Dump()
        {
            std::cout << type << " " << value << " " << posY << ", "<< posX << std::endl;
        }

        Type        type;
        string      value;
        int         posX;
        int         posY;
    };
};