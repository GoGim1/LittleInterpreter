#include <memory>
#include "Parser.h"
#include "Lexer.h"

namespace Parser
{
    using namespace Lexer;

    using std::make_shared;
    using std::make_unique;
    using std::make_pair;

    PrimaryPtr Parser::ParsePrimary()
    {
        auto t = NextToken();

        if (t.getType() == Token::Type::LBRACKET)
        {
            //TODO: PrimaryPtr
            PrimaryPtr ret = make_shared<PrimaryNode>(ParseExpr(), nullptr);
            NextToken();
            return ret;
        }                     
        else if (t.getType() == Token::Type::IDENTIFIER || t.getType() == Token::Type::FLOAT ||t.getType() == Token::Type::INTEGER)
        {
            auto ret = make_shared<PrimaryNode>(nullptr, make_unique<Token>(t));
            return ret;
        }    

        // TODO error: wrong primary
        else{}
    }

    FactorPtr Parser::ParseFactor()
    {
        auto t = NextToken();
        if (t.getType() == Token::Type::SUB)
            return make_shared<FactorNode>(ParsePrimary(), make_unique<Token>(t));
        else 
            return make_shared<FactorNode>(ParsePrimary(), nullptr);
    }

    ExprPtr Parser::ParseExpr()
    {
        auto ret = make_shared<ExprNode>(ParseFactor(), ExprNode::List());
        while (PeekToken().getType() == Token::Type::PLUS||
                PeekToken().getType() == Token::Type::SUB ||
                PeekToken().getType() == Token::Type::MUL ||
                PeekToken().getType() == Token::Type::DIV)
        {
           ret->ListHandler(make_unique<Token>(NextToken()), ParseFactor());
        }
        return ret;
    }

    BlockPtr Parser::ParseBlock()
    {
        NextToken();
        auto peek = PeekToken();

        // {}
        if (peek.getType() == Token::Type::RBRACE)
        {
            NextToken();
            return make_shared<BlockNode>(nullptr, BlockNode::List());
        }
        // {[statement]}  {[statement]{(;|EOF)[statement]}} {[statement]{(;|EOF)}} {{(;|EOF)[statement]}}  {{(;|EOF)}}
        else 
        {
            // {[statement]}  {[statement]{(;|EOF)[statement]}}  {[statement]{(;|EOF)}}
            if (peek.getType() != Token::Type::SEMICOLON && peek.getType() != Token::Type::_EOF)
            {
                auto ret = make_shared<BlockNode>(ParseStatement(), BlockNode::List());
                auto peek2 = PeekToken();
                
                // {[statement]}  
                if (peek2.getType() == Token::Type::RBRACE)
                {
                    NextToken();
                    return ret;
                }    
                
                // {[statement]{(;|EOF)[statement]}} {[statement]{(;|EOF)}}
                else if (peek2.getType() == Token::Type::SEMICOLON || peek2.getType() == Token::Type::_EOF)
                {
                    auto peek3 = PeekToken();
                    while (peek3.getType() == Token::Type::SEMICOLON || peek3.getType() == Token::Type::_EOF)
                    {
                        NextToken();
                        peek3 = PeekToken();

                        // {[statement]{(;|EOF)}}
                        if (peek3.getType() == Token::Type::RBRACE)  
                        {
                            NextToken();
                            return ret;
                        }
                        if (peek3.getType() == Token::Type::SEMICOLON || peek3.getType() == Token::Type::_EOF) 
                        {
                            ret->ListHandler(nullptr);
                            continue;
                        }
                        ret->ListHandler(ParseStatement());
                        peek3 = PeekToken();
                    }
                    if (peek3.getType() == Token::Type::RBRACE)
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
                auto ret = make_shared<BlockNode>(nullptr, BlockNode::List());
                auto peek2 = PeekToken();
                while (peek2.getType() == Token::Type::SEMICOLON || peek2.getType() == Token::Type::_EOF)
                {
                    NextToken();
                    peek2 = PeekToken();
                    
                    // {{(;|EOF)}}
                    if (peek2.getType() == Token::Type::RBRACE)  
                    {
                        NextToken();
                        return ret;
                    }
                    if (peek2.getType() == Token::Type::SEMICOLON || peek2.getType() == Token::Type::_EOF) 
                    {
                        ret->ListHandler(nullptr);
                        continue;
                    }
                    ret->ListHandler(ParseStatement());
                    peek2 = PeekToken();
                }
                if (peek2.getType() == Token::Type::RBRACE)
                {
                    NextToken();
                    return ret;
                }
                
                //TODO {{(;|EOF)[statement]} error:缺少右括号                
                else{}
                
            }
        }
        

    }

    SimplePtr Parser::ParseSimple()
    {
        return make_shared<SimpleNode>(ParseExpr());
    }

    StatementPtr    Parser::ParseStatement()
    {
        auto peek = PeekToken();

        if (peek.getType() == Token::Type::LBRACKET || peek.getType() == Token::Type::IDENTIFIER || peek.getType() == Token::Type::FLOAT || peek.getType() == Token::Type::INTEGER)
        {
            return make_shared<StatementNode>(StatementNode::Type::SIMPLE, nullptr, nullptr, nullptr, ParseSimple());
        }
        if (peek.getType() == Token::Type::IF)
        {
            NextToken();
            auto pExpr = ParseExpr();
            auto pBlock = ParseBlock();
            if (PeekToken().getType() != Token::Type::ELSE)
                return make_shared<StatementNode>(StatementNode::Type::IF, pExpr, pBlock, nullptr, nullptr);
            else 
            {
                NextToken();
                return make_shared<StatementNode>(StatementNode::Type::IFELSE, pExpr, pBlock, ParseBlock(), nullptr);                
            }
        }
        if (peek.getType() == Token::Type::WHILE)
        {
            NextToken();
            return make_shared<StatementNode>(StatementNode::Type::WHILE, ParseExpr(), ParseBlock(), nullptr, nullptr);
        }
    }

    ProgramPtr Parser::ParseProgram()
    {
        auto peek = PeekToken();
        if (peek.getType() == Token::Type::SEMICOLON || peek.getType() == Token::Type::_EOF)
        {
            NextToken();
            return make_shared<ProgramNode>(nullptr);
        }
        else 
            return make_shared<ProgramNode>(ParseStatement());
    }

}