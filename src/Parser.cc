// TODO: 1.职责问题，如1+/2, 应该是parse factor 1+/2时候报错，还是parse primary /2时候报错?
//       2.错误传播问题：孙级parse出错，AddError并返回给子级nullptr， 但是会触发Assert警告。
//          应该在子级防止孙级parse出错，传nullptr给父级；还是。。。

#include <memory>
#include <iostream>
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
        //Print("ParsePrimary");
        auto t = NextToken();
        //Print(t.Dump());
        if (t.getType() == Token::LBRACKET)
        {
            PrimaryPtr ret = MakePrimaryPtr(ParseExpr(), nullptr);
            t = NextToken();
            if (t.getType() != Token::RBRACKET)
            {
                AddError(Error("An error occur when parsing PrimaryNode: missing \")\".", t.getPosX(), t.getPosY()));
            }
            return ret;
        }                     
        else if (t.getType() == Token::IDENTIFIER || t.getType() == Token::FLOAT ||t.getType() == Token::INTEGER)
        {
            auto ret = MakePrimaryPtr(nullptr, MakeTokenPtr(t));
            return ret;
        }    
        else 
        {
            string tmp("An error occur when parsing PrimaryNode: \"");
            tmp = tmp + t.getValue() + "\" can not be a part of PrimaryNode.";
            AddError(Error(tmp, t.getPosX(), t.getPosY()));
            return nullptr;
        }
        
    }

    FactorPtr Parser::ParseFactor()
    {
        //Print("ParseFactor");
        auto t = PeekToken();
        //Print(t.Dump());
        if (t.getType() == Token::SUB || t.getType() == Token::PLUS)
        {
            t = NextToken();
            if (PeekToken().getType() == Token::LBRACKET || PeekToken().getType() == Token::IDENTIFIER || PeekToken().getType() == Token::FLOAT || PeekToken().getType() == Token::INTEGER)
                return MakeFactorPtr(ParsePrimary(), MakeTokenPtr(t));
            AddError(Error("An error occur when parsing FactorNode: PrimaryNode missing.", t.getPosX()+1, t.getPosY()));
            return nullptr;
        }
        else if (t.getType() == Token::LBRACKET || t.getType() == Token::IDENTIFIER || t.getType() == Token::FLOAT ||t.getType() == Token::INTEGER)
            return MakeFactorPtr(ParsePrimary(), nullptr);
        else 
        {
            string tmp("An error occur when parsing FactorNode: \"");
            tmp = tmp + t.getValue() + "\" should be \"+\" or \"-\"";
            AddError(Error(tmp, t.getPosX(), t.getPosY()));
            return nullptr;
        }
    }

    ExprPtr Parser::ParseExpr()
    {
        //Print("ParseExpr");
        auto ret = MakeExprPtr(ParseFactor());
        //Print(ret->Dump());
        while (PeekToken().getType() == Token::PLUS ||
               PeekToken().getType() == Token::MUL ||
               PeekToken().getType() == Token::SUB ||
               PeekToken().getType() == Token::DIV)
        {
            // The function parameters are read in order from right to left.
            auto p = MakeTokenPtr(NextToken());
            //Print(p->Dump());            
            if (PeekToken().getType() != Token::SUB && 
                PeekToken().getType() != Token::PLUS && 
                PeekToken().getType() != Token::IDENTIFIER && 
                PeekToken().getType() != Token::FLOAT &&
                PeekToken().getType() != Token::INTEGER &&
                PeekToken().getType() != Token::LBRACKET)
                {
                    AddError(Error("An error occur when parsing ExprNode: FactorNode missing after operator.", (*p).getPosX(), (*p).getPosY()));
                    return nullptr;
                }
            //Print(p->Dump());
            ret->ListHandler(p, ParseFactor());
            //Print(ret->Dump());
            
        }
        return ret;
    }

    BlockPtr Parser::ParseBlock()
    {
        //Print("ParseBlock");
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
        //Print("ParseSimple");
        return MakeSimplePtr(ParseExpr());
    }

    StatementPtr Parser::ParseStatement()
    {
        //Print("ParseStatement");
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
        //Print("ParseProgram");
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

    void Parser::HandleError()
    {
        if (errorList.empty())  
            return;
        for (auto& i : errorList)
            std::cout << i.what() << std::endl;
        throw errorList.front();
    }

    void Parser::AddError(const Error& e)
    {
        errorList.push_back(e);
    }

    // (DEBUG)
    void Parser::DumpErrorList()
    {
        for (auto& i : errorList)
            std::cout << i.what() << std::endl;
    }

    string Parser::Dump()
    {  
        return program->Dump(); 
    }

    void Parser::RunParser()
    {  
        program = ParseProgram(); 
        HandleError();
    } 
}
