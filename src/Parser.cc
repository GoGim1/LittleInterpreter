// TODO: 1.职责问题，如1+/2, 应该是parse factor 1+/2时候报错，还是parse primary /2时候报错?
//       2.错误传播问题：孙级parse出错，AddError并返回给子级nullptr， 但是会触发Assert警告。
//          应该在子级防止孙级parse出错，传nullptr给父级；还是。。。

#include <memory>
#include <iostream>
#include <sstream>
#include "Parser.h"
#include "Lexer.h"
#include "Helper.h"

namespace Parse
{
    using namespace Lexer;

    using std::make_shared;
    using std::make_unique;
    using std::make_pair;
    using std::stringstream;

    PrimaryPtr Parser::ParsePrimary()
    {
        // Print("ParsePrimary");
        auto t = NextToken();
        //Print(t.Dump());
        if (t.getType() == Token::LBRACKET)
        {
            t = PeekToken();
            auto pParseExpr = ParseExpr();
            if (!pParseExpr)
            {
                AddError(Error("PrimaryNode error: parsing Expr error.", t.getPosX(), t.getPosY()));
                return nullptr;
            }
            PrimaryPtr ret = MakePrimaryPtr(pParseExpr, nullptr);
            //Print(ret->Dump());
            t = NextToken();
            if (t.getType() != Token::RBRACKET)
            {
                AddError(Error("PrimaryNode error: missing \")\".", t.getPosX(), t.getPosY()));
                return nullptr;
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
            stringstream tmp;
            tmp << "PrimaryNode error: \"" << t.getValue() << "\" can not be a part of PrimaryNode.";
            AddError(Error(tmp.str(), t.getPosX(), t.getPosY()));
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
            NextToken();
            auto tempToken = PeekToken();
            auto p = ParsePrimary();
            if (!p) 
            {
                AddError(Error("FactorNode error: missing PrimaryNode.", tempToken.getPosX(), tempToken.getPosY()));
                return nullptr;
            }
            return MakeFactorPtr(p, MakeTokenPtr(t));
        }
        else if (t.getType() == Token::LBRACKET || t.getType() == Token::IDENTIFIER || t.getType() == Token::FLOAT ||t.getType() == Token::INTEGER)
        {
            // Print(t.Dump());
            auto p = ParsePrimary();
            if (!p) 
            {
                //Print(t.Dump());
                AddError(Error("FactorNode error: parsing Primary error.", t.getPosX(), t.getPosY()));
                return nullptr;
            }
            return MakeFactorPtr(p, nullptr);
        }
        else 
        {
            stringstream tmp;
            tmp << "FactorNode error: \"" << t.getValue() + R"(" should be "+", "-", indentifier, integer or float.)";
            AddError(Error(tmp.str(), t.getPosX(), t.getPosY()));
            return nullptr;
        }
    }

    ExprPtr Parser::ParseExpr()
    {
        //Print("ParseExpr");
        auto peekToken = PeekToken();
        auto p = ParseFactor();
        if (!p) 
        {
            AddError(Error("ExprNode error: parsing the first Factor error.", peekToken.getPosX(), peekToken.getPosY()));
            return nullptr;
        }
        auto ret = MakeExprPtr(p);
        //Print(ret->Dump());
        peekToken = PeekToken();
        //Print(peekToken.Dump());
        while (peekToken.getType() == Token::PLUS ||
               peekToken.getType() == Token::MUL ||
               peekToken.getType() == Token::SUB ||
               peekToken.getType() == Token::DIV)
        {
            // The function parameters are read in order from right to left.
            auto p = MakeTokenPtr(NextToken());
            //Print(p->Dump());     
            peekToken = PeekToken(); 
            //Print(peekToken.Dump());
                  
            if (peekToken.getType() != Token::SUB && 
                peekToken.getType() != Token::PLUS && 
                peekToken.getType() != Token::IDENTIFIER && 
                peekToken.getType() != Token::FLOAT &&
                peekToken.getType() != Token::INTEGER &&
                peekToken.getType() != Token::LBRACKET)
                {
                    AddError(Error("ExprNode error: FactorNode miss after operator.", PeekToken().getPosX(), PeekToken().getPosY()));
                    return nullptr;
                }
            //Print(p->Dump());
            auto pParseFactor = ParseFactor();
            if (!pParseFactor)  
            {
                AddError(Error("ExprNode error: parsing Factor list error.", peekToken.getPosX(), peekToken.getPosY()));
                return nullptr;
            }
            ret->ListHandler(p, pParseFactor);
            //Print(ret->Dump());
            peekToken = PeekToken();
            
        }
        return ret;
    }

    BlockPtr Parser::ParseBlock()
    {
        //Print("ParseBlock");
        auto blockFirstToken = NextToken();
        if (blockFirstToken.getType() != Token::LBRACE)
        {
            AddError(Error("BlockNode error: BlockNode must start with \"{\".", blockFirstToken.getPosX(), blockFirstToken.getPosY()));
            return nullptr;
        }

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
            if (peek.getType() != Token::SEMICOLON)
            {
                auto pParseStatement = ParseStatement();
                if (!pParseStatement)
                {
                    AddError(Error("BlockNode error: parsing the first Statement error.", peek.getPosX(), peek.getPosY()));
                    return nullptr;
                }
                auto ret = MakeBlockPtr(pParseStatement);
                
                auto peek2 = PeekToken(); 
                // {[statement]}  
                if (peek2.getType() == Token::RBRACE)
                {
                    NextToken();
                    return ret;
                }    
                
                // {[statement]{(;|EOF)[statement]}} {[statement]{(;|EOF)}}
                else if (peek2.getType() == Token::SEMICOLON)
                {
                    auto peek3 = PeekToken();
                    while (peek3.getType() == Token::SEMICOLON)
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
                        if (peek3.getType() == Token::SEMICOLON) 
                        {
                            ret->ListHandler(nullptr);
                            continue;
                        }
                        auto pParseStatement = ParseStatement();
                        if (!pParseStatement)
                        {
                            AddError(Error("BlockNode error: parsing statement list error.", peek3.getPosX(), peek3.getPosY()));
                            return nullptr;
                        }
                        ret->ListHandler(pParseStatement);
                        peek3 = PeekToken();
                    }
                    if (peek3.getType() == Token::RBRACE)
                    {
                        NextToken();
                        return ret;
                    }
                    else
                    {
                        AddError(Error("BlockNode error: BlockNode must end with \"}\".", peek3.getPosX(), peek3.getPosY()));
                        return nullptr;
                    }
                }                    
                else
                {
                    AddError(Error("BlockNode error: BlockNode must end with \"}\".", peek2.getPosX(), peek2.getPosY()));
                    return nullptr;
                }

            }
            // {{(;|EOF)[statement]}}  {{(;|EOF)}}
            else
            {
                auto ret = MakeBlockPtr(nullptr);
                auto peek2 = PeekToken();
                while (peek2.getType() == Token::SEMICOLON)
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
                    if (peek2.getType() == Token::SEMICOLON) 
                    {
                        ret->ListHandler(nullptr);
                        continue;
                    }
                    auto pParseStatement = ParseStatement();
                    if (!pParseStatement)
                    {
                        AddError(Error("BlockNode error: parsing statement list error.", peek2.getPosX(), peek2.getPosY()));
                        return nullptr;
                    }
                    ret->ListHandler(pParseStatement);
                    peek2 = PeekToken();
                }
                if (peek2.getType() == Token::RBRACE)
                {
                    NextToken();
                    return ret;
                }
                else
                {
                    AddError(Error("BlockNode error: BlockNode must end with \"}\".", peek2.getPosX(), peek2.getPosY()));
                    return nullptr;
                }
                
            }
        }
        
        return nullptr;
    }

    SimplePtr Parser::ParseSimple()
    {
        //Print("ParseSimple");
        auto pParseExpr = ParseExpr();
        auto simpleFirstToken = PeekToken();
        if (!pParseExpr) 
        {
            AddError(Error("SimpleNode error: parsing Expr error.", simpleFirstToken.getPosX(), simpleFirstToken.getPosY()));
            return nullptr;
        }
        return MakeSimplePtr(pParseExpr);
    }

    StatementPtr Parser::ParseStatement()
    {
        //Print("ParseStatement");
        auto peek = PeekToken();
        //Print(peek.Dump());
        
        if (peek.getType() == Token::IF)
        {
            NextToken();

            auto ifFragmentFirst = PeekToken();
            auto pParseExpr = ParseExpr();
            if (!pParseExpr)
            {
                AddError(Error("StatementNode error: parsing Expr error when parsing IF Statement.", ifFragmentFirst.getPosX(), ifFragmentFirst.getPosY()));
                return nullptr;
            }

            ifFragmentFirst = PeekToken();
            auto pParseBlock = ParseBlock();
            if (!pParseBlock)
            {
                AddError(Error("StatementNode error: parsing Block error when parsing IF Statement.", ifFragmentFirst.getPosX(), ifFragmentFirst.getPosY()));
                return nullptr;
            }
            if (PeekToken().getType() != Token::ELSE)
                return MakeStatementPtr(StatementNode::IF, pParseExpr, pParseBlock, nullptr, nullptr);
            else 
            {
                NextToken();
                auto ifElseFragmentFirst = PeekToken();
                auto pParseBlockOfElse = ParseBlock();
                if (!pParseBlockOfElse)
                {
                    AddError(Error("StatementNode error: parsing Block error when parsing IFELSE Statement.", ifElseFragmentFirst.getPosX(), ifElseFragmentFirst.getPosY()));
                    return nullptr;
                }
                return MakeStatementPtr(StatementNode::IFELSE, pParseExpr, pParseBlock, pParseBlockOfElse, nullptr);                
            }
        }
        else if (peek.getType() == Token::WHILE)
        {
            NextToken();
            auto whileFragmentFirst = PeekToken();
            auto pParseExpr = ParseExpr();
            if (!pParseExpr)
            {
                AddError(Error("StatementNode error: parsing Expr error when parsing WHILE Statement.", whileFragmentFirst.getPosX(), whileFragmentFirst.getPosY()));
                return nullptr;
            }
            whileFragmentFirst = PeekToken();
            auto pParseBlock = ParseBlock();
            if (!pParseBlock)
            {
                AddError(Error("StatementNode error: parsing Block error when parsing WHILE Statement.", whileFragmentFirst.getPosX(), whileFragmentFirst.getPosY()));
                return nullptr;
            }
            return MakeStatementPtr(StatementNode::WHILE, pParseExpr, pParseBlock, nullptr, nullptr);
        }
        //if (peek.getType() == Token::LBRACKET || peek.getType() == Token::IDENTIFIER || peek.getType() == Token::FLOAT || peek.getType() == Token::INTEGER)
        else
        {
            auto pParseSimple = ParseSimple();
            if (!pParseSimple)
            {
                AddError(Error("StatementNode error: parsing Simple error.", peek.getPosX(), peek.getPosY()));
                return nullptr;
            }
            return MakeStatementPtr(StatementNode::SIMPLE, nullptr, nullptr, nullptr, pParseSimple);
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
            {
                auto pParseStatement = ParseStatement();
                if (!pParseStatement)
                {
                    AddError(Error("Program error: parsing Statement list error.", peek.getPosX(), peek.getPosY()));
                    return nullptr;
                }
                ret->ListHandler(pParseStatement);
                if (PeekToken().getType() != Token::SEMICOLON && PeekToken().getType() != Token::_EOF)
                {
                    AddError(Error(R"(Program error: Statement list should be separated by ";".)", PeekToken().getPosX(), PeekToken().getPosY()));
                    return nullptr;
                }
            }
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
