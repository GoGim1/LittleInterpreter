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
            UNKNOWN, _EOF,
        };
    
    public:
        Token(Type t = UNKNOWN, string v = "", int x = -1, int y = -1): type(t), value(v), posX(x) ,posY(y) {}
       
        Token(const Token& rhs): type(rhs.type), value(rhs.value), posX(rhs.posX), posY(rhs.posY) {}
       
        Token& operator=(const Token& rhs) 
        {
            type = rhs.type;
            value = rhs.value;
            posX = rhs.posX;
            posY = rhs.posY;

            return *this;
        }

        // (DEBUG)
        string Dump()
        {
           // std::cout << type << " " << value << " " << posY << ", "<< posX << std::endl;
            return value;
        }

        Token::Type getType() const
        {
            return type;
        }

        const string& getValue() const
        {
            return value;
        }

        const int getPosX() const
        {
            return posX;
        }

        const int getPosY() const
        {
            return posY;
        }
    private:
        Type        type;
        string      value;
        int         posX;
        int         posY;
    };
};