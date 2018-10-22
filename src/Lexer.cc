#include "Lexer.h"

namespace Lexer
{
    list<Token>                             tokenList;      
    std::unordered_map<string, Token::Type> reservedTable;

#ifdef DEBUG
    void PrintTokenList()
    {
        for (auto& i : tokenList)
            Print(i.Dump());
    }
#endif

    const Token NextToken() 
    {   
        Assert(!tokenList.empty(), "NextToken() error: TokenList is empty");
        if (tokenList.size() > 1)
        {
            const Token ret = tokenList.front();
            tokenList.pop_front();    
            return ret;
        }
        return tokenList.front();
    }

    const Token& PeekToken() 
    {
        Assert(!tokenList.empty(), "PeekToken() error: TokenList is empty");
        return tokenList.front();
        
    }


    void AddToken(string s, int x, int y)
    {
        auto t = reservedTable.find(s);
        if (t != reservedTable.end())   
        {
            tokenList.push_back({t->second, s, x, y});     
            return;
        }
        if (s == "EOF")
        {
            tokenList.push_back({Token::_EOF, s, x, y});        
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
            AddError(Error("Unknown token: \""+s + "\".", x, y));
    }

    void InitReservedTable()
    {
        reservedTable["if"] = Token::IF;
        reservedTable["else"] = Token::ELSE;
        reservedTable["while"] = Token::WHILE;
        reservedTable["var"] = Token::VAR;
        reservedTable["def"] = Token::DEF;
        reservedTable[">="] = Token::GE;
        reservedTable["<="] = Token::LE;
        reservedTable["=="] = Token::EQUAL;
        reservedTable[">"] = Token::GREATER;
        reservedTable["<"] = Token::LESS;
        reservedTable["%"] = Token::MOD;
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
        reservedTable[","] = Token::COMMA;
    }
    
    void RunLexer(string code)  
    { 
        CleanError();
        tokenList.clear();
        
        std::regex          pattern(R"(([0-9]+\.[0-9]+)|([0-9]+)|([A-Z_a-z][A-Z_a-z0-9]*)|==|<=|>=|\S)");
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
                AddToken(j->str(), j->position(), lineNum);
        }
        AddToken("EOF", buf.back().size(), lineNum);
        HandleError();
    }

    void RunLexerFromFile(const string& fileName)  
    { 
        fstream                         file(fileName, std::fstream::in);
        if (!file.is_open()) 
            throw Error("File is not open correctly.");

        std::istreambuf_iterator<char>  beg(file), end; 
        std::string                     code(beg, end);

        RunLexer(code); 
        file.close();

    } 
};