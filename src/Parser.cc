// TODO: 后半部分不见，EOF提前出现的错误处理
#ifdef DEBUG
#undef DEBUG

#include <memory>
#include "Parser.h"
#include "Lexer.h"
#include "Helper.h"

namespace Parse
{
    using namespace Lexer;

    using std::make_shared;
    using std::make_unique;
    using std::make_pair;

    PrimaryPtr Parser::ParsePrimary()
    {
        Print("ParsePrimary");
        auto t = NextToken();
        if (t.getType() == Token::LBRACKET)
        {
            //TODO: PrimaryPtr
            PrimaryPtr ret = MakePrimaryPtr(ParseExpr(), nullptr);
            NextToken();
            return ret;
        }                     
        else if (t.getType() == Token::IDENTIFIER || t.getType() == Token::FLOAT ||t.getType() == Token::INTEGER)
        {
            auto ret = MakePrimaryPtr(nullptr, MakeTokenPtr(t));
            return ret;
        }    

        // TODO error: wrong primary
        else 
        {
            return nullptr;
        }
        
    }

    FactorPtr Parser::ParseFactor()
    {
        Print("ParseFactor");
        auto t = PeekToken();
        if (t.getType() == Token::SUB)
        {
            t = NextToken();
            return MakeFactorPtr(ParsePrimary(), MakeTokenPtr(t));
        }
        else 
            return MakeFactorPtr(ParsePrimary(), nullptr);
        
    }

    ExprPtr Parser::ParseExpr()
    {
        Print("ParseExpr");
        auto ret = MakeExprPtr(ParseFactor());
        while (PeekToken().getType() == Token::PLUS||
                PeekToken().getType() == Token::SUB ||
                PeekToken().getType() == Token::MUL ||
                PeekToken().getType() == Token::DIV)
        {
            // The function parameters are read in order from right to left.
            auto p = MakeTokenPtr(NextToken());
            ret->ListHandler(p, ParseFactor());
        }
        return ret;
    }

    BlockPtr Parser::ParseBlock()
    {
        Print("ParseBlock");
        NextToken();
        auto peek = PeekToken();

        // {}
        if (peek.getType() == Token::RBRACE)
        {
            NextToken();
            return MakeBlockPtr(nullptr);
        }
        // {[statement]}  {[statement]{(;|EOF)[statement]}} {[statement]{(;|EOF)}} {{(;|EOF)[statement]}}  {{(;|EOF)}}
        else 
        {
            // {[statement]}  {[statement]{(;|EOF)[statement]}}  {[statement]{(;|EOF)}}
            if (peek.getType() != Token::SEMICOLON && peek.getType() != Token::_EOF)
            {
                auto ret = MakeBlockPtr(ParseStatement());
                auto peek2 = PeekToken();
                
                // {[statement]}  
                if (peek2.getType() == Token::RBRACE)
                {
                    NextToken();
                    return ret;
                }    
                
                // {[statement]{(;|EOF)[statement]}} {[statement]{(;|EOF)}}
                else if (peek2.getType() == Token::SEMICOLON || peek2.getType() == Token::_EOF)
                {
                    auto peek3 = PeekToken();
                    while (peek3.getType() == Token::SEMICOLON || peek3.getType() == Token::_EOF)
                    {
                        NextToken();
                        peek3 = PeekToken();

                        // {[statement]{(;|EOF)}}
                        if (peek3.getType() == Token::RBRACE)  
                        {
                            ret->ListHandler(nullptr);                            
                            NextToken();
                            return ret;
                        }
                        if (peek3.getType() == Token::SEMICOLON || peek3.getType() == Token::_EOF) 
                        {
                            ret->ListHandler(nullptr);
                            continue;
                        }
                        ret->ListHandler(ParseStatement());
                        peek3 = PeekToken();
                    }
                    if (peek3.getType() == Token::RBRACE)
                    {
                        NextToken();
                        return ret;
                    }
                    //TODO {[statement]{(;|EOF)[statement]} error:缺少右括号
                    else{}
                    
                }                    
                //TODO error:{[statement]{^(;|EOF|})[statement]}}
                else{}

            }
            // {{(;|EOF)[statement]}}  {{(;|EOF)}}
            else
            {
                auto ret = MakeBlockPtr(nullptr);
                auto peek2 = PeekToken();
                while (peek2.getType() == Token::SEMICOLON || peek2.getType() == Token::_EOF)
                {
                    NextToken();
                    peek2 = PeekToken();
                    
                    // {{(;|EOF)}}
                    if (peek2.getType() == Token::RBRACE)  
                    {
                        ret->ListHandler(nullptr);                        
                        NextToken();
                        return ret;
                    }
                    if (peek2.getType() == Token::SEMICOLON || peek2.getType() == Token::_EOF) 
                    {
                        ret->ListHandler(nullptr);
                        continue;
                    }
                    ret->ListHandler(ParseStatement());
                    peek2 = PeekToken();
                }
                if (peek2.getType() == Token::RBRACE)
                {
                    NextToken();
                    return ret;
                }
                
                //TODO {{(;|EOF)[statement]} error:缺少右括号                
                else
                {
                    return nullptr;
                }
                
            }
        }
        
        return nullptr;
    }

    SimplePtr Parser::ParseSimple()
    {
        Print("ParseSimple");
        return MakeSimplePtr(ParseExpr());
    }

    StatementPtr Parser::ParseStatement()
    {
        Print("ParseStatement");
        auto peek = PeekToken();

        
        if (peek.getType() == Token::IF)
        {
            NextToken();
            auto pExpr = ParseExpr();
            auto pBlock = ParseBlock();
            if (PeekToken().getType() != Token::ELSE)
                return MakeStatementPtr(StatementNode::IF, pExpr, pBlock, nullptr, nullptr);
            else 
            {
                NextToken();
                return MakeStatementPtr(StatementNode::IFELSE, pExpr, pBlock, ParseBlock(), nullptr);                
            }
        }
        else if (peek.getType() == Token::WHILE)
        {
            NextToken();

            // The function parameters are read in order from right to left.            
            auto p = ParseExpr();
            return MakeStatementPtr(StatementNode::WHILE, p, ParseBlock(), nullptr, nullptr);
        }
        //if (peek.getType() == Token::LBRACKET || peek.getType() == Token::IDENTIFIER || peek.getType() == Token::FLOAT || peek.getType() == Token::INTEGER)
        else
        {
            return MakeStatementPtr(StatementNode::SIMPLE, nullptr, nullptr, nullptr, ParseSimple());
        }
        return nullptr;
    }

    ProgramPtr Parser::ParseProgram()
    {
        Print("ParseProgram");
        auto ret = MakeProgramPtr();
        auto peek = PeekToken();
        while (peek.getType() != Token::_EOF)
        {
            if (peek.getType() == Token::SEMICOLON)
                ret->ListHandler(nullptr);
            else
                ret->ListHandler(ParseStatement());
            NextToken();
            peek = PeekToken();
        }

        return ret;
    }

}


#define DEBUG
#endif