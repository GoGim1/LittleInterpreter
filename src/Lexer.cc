#include <fstream>
#include <exception>
#include <list>
#include <regex>
#include <iterator>
#include <unordered_map>
#include <vector>
#include "Lexer.h"
#include "Error.h"

// #include <iostream> 
// using namespace std;

namespace Lexer
{
    using namespace Util;

    using std::vector;
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
        return Token(Token::_EOF);
    }

    Token PeekToken() 
    {
        if (!tokenList.empty())
            return tokenList.front();
        return Token(Token::_EOF);
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
                tokenList.push_back({Token::FLOAT, s, x, y});
            else
                tokenList.push_back({Token::INTEGER, s, x, y});         
        }
        else if (isalpha(s[0]))   
            tokenList.push_back({Token::IDENTIFIER, s, x, y});
        else
            errorToken.push_back({Token::UNKNOWN, s, x, y});
        
    }

    void InitReservedTable()
    {
        reservedTable["if"] = Token::IF;
        reservedTable["else"] = Token::ELSE;
        reservedTable["while"] = Token::WHILE;
        reservedTable["var"] = Token::VAR;
        reservedTable[">="] = Token::GE;
        reservedTable["<="] = Token::LE;
        reservedTable["=="] = Token::EQUAL;
        reservedTable["="] = Token::ASSIGN;
        reservedTable["+"] = Token::PLUS;
        reservedTable["-"] = Token::SUB;
        reservedTable["*"] = Token::MUL;
        reservedTable["/"] = Token::DIV;
        reservedTable[";"] = Token::SEMICOLON;
        reservedTable["{"] = Token::LBRACE;
        reservedTable["}"] = Token::RBRACE;
        reservedTable["("] = Token::LBRACKET;
        reservedTable[")"] = Token::RBRACKET;
    }

    void HandleErrorToken() 
    {
        if (errorToken.empty())
            return;
        else 
        {
            for(auto& i : errorToken)
            {
                std::cerr << "[Line " << i.getPosY() << ", " << i.getPosX() 
                            << "]: Unknown token \"" << i.getValue() << "\"." << std::endl;
            }
        }
        throw Error("Unknown token.");
    }

    // void RunLexer(string fileName)  
    // { 
    //     std::regex  pattern(R"(([0-9]+.[0-9]+)|([0-9]+)|([A-Z_a-z][A-Z_a-z0-9]*)|==|<=|>=|\S)");
        
    //     fstream     file(fileName, std::fstream::in);
    //     int         lineNum = -1;
    //     string      buf;

    //     if (!file.is_open()) throw Error("File is not open correctly.");
        
    //     InitReservedTable();
    //     while (getline(file, buf))
    //     {
    //         lineNum++;
    //         auto words_begin = std::sregex_iterator(buf.begin(), buf.end(), pattern);
    //         auto words_end = std::sregex_iterator();
    //         for (std::sregex_iterator i = words_begin; i != words_end; ++i) 
    //         {
    //            //std::cout << i->str() << std::endl;
    //             AddToken(i->str(), i->position(), lineNum);
    //         }
    //     }
    //     file.close();

    //     HandleErrorToken();
    // }   
    
    void RunLexer(string code)  
    { 
        std::regex          pattern(R"(([0-9]+.[0-9]+)|([0-9]+)|([A-Z_a-z][A-Z_a-z0-9]*)|==|<=|>=|\S)");
        int                 lineNum = -1;
        vector<string>      buf;
        
        InitReservedTable();

        code += "\n";
        size_t pos = code.find("\n");
        size_t size = code.size();
        while (pos != std::string::npos)
        {
            std::string x = code.substr(0,pos);
            buf.push_back(x);
            code = code.substr(pos+1,size);
            pos = code.find("\n");
        }

        for (auto& i : buf)
        {
            lineNum++;
            auto words_begin = std::sregex_iterator(i.begin(), i.end(), pattern);
            auto words_end = std::sregex_iterator();
            for (std::sregex_iterator j = words_begin; j != words_end; ++j) 
            {
               //std::cout << i->str() << std::endl;
                AddToken(j->str(), j->position(), lineNum);
            }
        }

        HandleErrorToken();
    }

    void RunLexerFromFile(string fileName)  
    { 
        fstream                         file(fileName, std::fstream::in);

        if (!file.is_open()) throw Error("File is not open correctly.");

        std::istreambuf_iterator<char>  beg(file), end; 
        std::string                     code(beg, end);

        RunLexer(code); 

        file.close();

    } 
};