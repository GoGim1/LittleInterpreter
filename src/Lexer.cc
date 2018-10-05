#include <fstream>
#include <exception>
#include <list>
#include <regex>
#include <iterator>
#include <unordered_map>
#include "Lexer.h"

#include <iostream>

namespace Lexer
{
    using std::fstream;
    using std::exception;
    using std::list;
    
    list<Token> tokenList;
    std::unordered_map<string, Token::Type> reservedTable;

    Token NextToken()
    {   
        Token ret = tokenList.front();
        tokenList.pop_front();
        return ret;
    }

    Token PeekToken()
    {
        return tokenList.front();
    }


    void AddToken(string s, int l)
    {
        auto t = reservedTable.find(s);
        if (t != reservedTable.end())   
        {
            tokenList.push_back({t->second, s, l});     
            return;
        }

        if (isdigit(s[0]))
        {
            std::size_t found = s.find('.');
            if (found!=std::string::npos)
                tokenList.push_back({Token::Type::FLOAT, s, l});
            else
                tokenList.push_back({Token::Type::INTEGER, s, l});            
        }
        else if (isalpha(s[0]))
            tokenList.push_back({Token::Type::IDENTIFIER, s, l});
        else
            tokenList.push_back({Token::Type::OPERATOR, s, l});
        
    }

    void InitReservedTable()
    {
        reservedTable["if"] = Token::Type::IF;
        reservedTable["else"] = Token::Type::ELSE;
        reservedTable["while"] = Token::Type::WHILE;
        reservedTable["var"] = Token::Type::VAR;
        reservedTable["=="] = Token::Type::EQUAL;
        reservedTable["="] = Token::Type::ASSIGN;
        reservedTable["+"] = Token::Type::PLUS;
        reservedTable["-"] = Token::Type::SUB;
        reservedTable["*"] = Token::Type::MUL;
        reservedTable["/"] = Token::Type::DIV;
    }


    void RunLexer(string fileName)
    { 
        std::regex  pattern(R"(([0-9]+.[0-9]+)|([0-9]+)|([A-Z_a-z][A-Z_a-z0-9]*)|==|<=|>=|\S)");
        
        fstream     file(fileName, std::fstream::in);
        int         lineNum = -1;
        string      buf;

        if (!file.is_open()) throw "File is not open correctly.";
        InitReservedTable();
        while (getline(file, buf))
        {
            lineNum++;
            auto words_begin = std::sregex_iterator(buf.begin(), buf.end(), pattern);
            auto words_end = std::sregex_iterator();
            for (std::sregex_iterator i = words_begin; i != words_end; ++i) 
            {
                //std::cout << i->str() << std::endl;
                AddToken(i->str(), lineNum);
            }
        }
        file.close();
    }
};