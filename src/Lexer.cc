#include <fstream>
#include <exception>
#include <list>
#include <regex>
#include <iterator>
#include <unordered_map>
#include "Lexer.h"
#include "Error.h"

#include <iostream>

namespace Lexer
{
    using namespace Util;

    using std::fstream;
    using std::exception;
    using std::list;
    
    list<Token> tokenList;      
    list<Token> errorToken;     
    std::unordered_map<string, Token::Type> reservedTable;


    Token NextToken()
    {   
        if (!tokenList.empty())
        {
            Token ret = tokenList.front();
            tokenList.pop_front();
            return ret;
        }
        return Token(Token::Type::_EOF);
    }

    Token PeekToken()
    {
        if (!tokenList.empty())
            return tokenList.front();
        return Token(Token::Type::_EOF);
    }


    void AddToken(string s, int x, int y)
    {
        auto t = reservedTable.find(s);
        if (t != reservedTable.end())   
        {
            tokenList.push_back({t->second, s, x, y});     
            return;
        }

        if (isdigit(s[0]))
        {
            std::size_t found = s.find('.');
            if (found!=std::string::npos)
                tokenList.push_back({Token::Type::FLOAT, s, x, y});
            else
                tokenList.push_back({Token::Type::INTEGER, s, x, y});            
        }
        else if (isalpha(s[0]))
            tokenList.push_back({Token::Type::IDENTIFIER, s, x, y});
        else
            errorToken.push_back({Token::Type::UNKNOWN, s, x, y});
        
    }

    void InitReservedTable()
    {
        reservedTable["if"] = Token::Type::IF;
        reservedTable["else"] = Token::Type::ELSE;
        reservedTable["while"] = Token::Type::WHILE;
        reservedTable["var"] = Token::Type::VAR;
        reservedTable[">="] = Token::Type::GE;
        reservedTable["<="] = Token::Type::LE;
        reservedTable["=="] = Token::Type::EQUAL;
        reservedTable["="] = Token::Type::ASSIGN;
        reservedTable["+"] = Token::Type::PLUS;
        reservedTable["-"] = Token::Type::SUB;
        reservedTable["*"] = Token::Type::MUL;
        reservedTable["/"] = Token::Type::DIV;
        reservedTable[";"] = Token::Type::SEMICOLON;
        reservedTable["{"] = Token::Type::LBRACE;
        reservedTable["}"] = Token::Type::RBRACE;
        reservedTable["("] = Token::Type::LBRACKET;
        reservedTable[")"] = Token::Type::RBRACKET;
    }

    void HandleErrorToken() 
    {
        if (errorToken.empty())
            return;
        else 
        {
            for(auto& i : errorToken)
            {
                std::cerr << "[Line " << i.posY << ", " << i.posX << "]: Unknown token \"" << i.value << "\"." << std::endl;
            }
        }
        throw Error("Unknown token.");
    }

    void RunLexer(string fileName)  
    { 
        std::regex  pattern(R"(([0-9]+.[0-9]+)|([0-9]+)|([A-Z_a-z][A-Z_a-z0-9]*)|==|<=|>=|\S)");
        
        fstream     file(fileName, std::fstream::in);
        int         lineNum = -1;
        string      buf;

        if (!file.is_open()) throw Error("File is not open correctly.");
        
        InitReservedTable();
        while (getline(file, buf))
        {
            lineNum++;
            auto words_begin = std::sregex_iterator(buf.begin(), buf.end(), pattern);
            auto words_end = std::sregex_iterator();
            for (std::sregex_iterator i = words_begin; i != words_end; ++i) 
            {
               //std::cout << i->str() << std::endl;
                AddToken(i->str(), i->position(), lineNum);
            }
        }
        file.close();

        HandleErrorToken();
    }
};