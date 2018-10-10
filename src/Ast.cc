//TODO:  异常状态，如同时非nullptr


#include <iostream>
#include <sstream>
#include <utility>
#include "Ast.h"

namespace Ast
{
    using std::stringstream;
    using std::make_pair;

    string PrimaryNode::Dump() const
    {
        stringstream ret;
        if (pExpr)
           ret << "(" << pExpr->Dump() << ")";
        else 
            ret << pToken->Dump();
        return ret.str();
    }

    string FactorNode::Dump() const
    {
        stringstream ret;
        if (pToken)
           ret << pToken->Dump();
        ret << pPrimary->Dump();
        return ret.str();
    }

    string ExprNode::Dump() const
    {
        stringstream ret;
        ret << pFactor->Dump();
        for (auto& i : factorList)
            ret << i.first->Dump() << i.second->Dump();
        return ret.str();  
    }

    void ExprNode::ListHandler(TokenPtr pTokenRhs, FactorPtr pFactorRhs)
    {
        factorList.push_back(make_pair((TokenPtr)std::move(pTokenRhs), (FactorPtr)pFactor));
    }
        
    string BlockNode::Dump() const
    {
        stringstream ret;
        ret << "{" << pStatement ? "" : pStatement->Dump();
        for (auto& i : statementList)
            ret << ";" << (i ? "" : i->Dump());
        ret << "}";
        return ret.str();
    }

    void BlockNode::ListHandler(StatementPtr pStatementRhs)
    {
        statementList.push_back(pStatementRhs);
    }
    

    string SimpleNode::Dump() const 
    {
        return pExpr->Dump();
    }

    string StatementNode::Dump() const
    {
        stringstream ret;
        switch (type)
        {
            case IF: 
                ret << "if" << pExpr->Dump() << pBlock->Dump();
                break;
            case IFELSE: 
                ret << "if" << pExpr->Dump() << pBlock->Dump() << "else" << pBlockOfElse->Dump();
                break;                
            case WHILE:            
                ret << "while" << pExpr->Dump() << pBlock->Dump();
                break;
            case SIMPLE: 
                ret << pSimple->Dump();
                break;
            default: break;
        }
        return ret.str();
    }

    string ProgramNode::Dump() const
    {
        stringstream ret;
        ret << (pStatement ? "" : pStatement->Dump());
        return ret.str();
    }
}