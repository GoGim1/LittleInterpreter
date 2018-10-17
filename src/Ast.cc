#include "Ast.h"

namespace Ast
{
    string PrimaryNode::Dump() const
    {
        stringstream ret;
        if (pExpr)
           ret << "(" << pExpr->Dump() << ")";
        else 
            ret << pToken->Dump();
        return ret.str();
    }

    double PrimaryNode::Eval() 
    {
        if (pToken)
        {
            switch (pToken->getType())
            {
            case Token::INTEGER:
            case Token::FLOAT:
                return std::stod(pToken->getValue());                        
            case Token::IDENTIFIER:
            {
                auto value = Env.find(pToken->getValue()); 
                if (value != Env.end())
                    return value->second;
                // TODO: could not find out the identifier
                else 
                {}
            }
            default:
                Assert(0, "Eval Primary error");
            }
            
        }
        else if (pExpr)
            return pExpr->Eval(); 
        Assert(0, "Eval Primary error");        
    }
    

    string FactorNode::Dump() const
    {
        stringstream ret;
        if (pToken)
           ret << pToken->Dump();
        ret << pPrimary->Dump();
        return ret.str();
    }

    double FactorNode::Eval()
    {
        if (pToken->getType() == Token::SUB)
            return 0 - pPrimary->Eval();
        else    
            return pPrimary->Eval();
    }


    string ExprNode::Dump() const
    {
        stringstream ret;
        ret << pFactor->Dump();
        for (auto& i : factorList)
            ret << i.first->Dump() << i.second->Dump();
        return ret.str();  
    }

    double ExprNode::Eval()
    {
        int ret = pFactor->Eval();
        for (auto& i : factorList)
        {
            switch(i.first->getType())
            {
            case Token::PLUS:
                ret += i.second->Eval();
                break;
            case Token::SUB:
                ret -= i.second->Eval();
                break;
            case Token::ASSIGN:
                ret = i.second->Eval();
            }

        }

    }
    
    void ExprNode::ListHandler(const TokenPtr& pTokenRhs, const FactorPtr& pFactorRhs)
    {
        factorList.push_back(make_pair((TokenPtr)pTokenRhs, (FactorPtr)pFactorRhs));
    }
   

    string BlockNode::Dump() const
    {
        stringstream ret;
        ret << "{" << (pStatement ? pStatement->Dump():"");
        for (auto& i : statementList)
            ret << ";" << (i ? i->Dump() : "");
        ret << "}";
        return ret.str();
    }
    
    double BlockNode::Eval()
    {
    }

    void BlockNode::ListHandler(const StatementPtr& pStatementRhs)
    {
        statementList.push_back(pStatementRhs);
    }
    

    string SimpleNode::Dump() const 
    {
        return pExpr->Dump();
    }
    
    double SimpleNode::Eval()
    {
    }


    string StatementNode::Dump() const
    {
        stringstream ret;
        switch (type)
        {
            case IF: 
                ret << "if " << pExpr->Dump() << pBlock->Dump();
                break;
            case IFELSE: 
                ret << "if " << pExpr->Dump() << pBlock->Dump() << "else" << pBlockOfElse->Dump();
                break;                
            case WHILE:            
                ret << "while " << pExpr->Dump() << pBlock->Dump();
                break;
            case SIMPLE: 
                ret << pSimple->Dump();
                break;
            default: break;
        }
        return ret.str();
    }

    double StatementNode::Eval()
    {
    }


    string ProgramNode::Dump() const
    {
        stringstream ret;
        for (auto& i : statementList)
            ret << (i ? i->Dump() : "") << ";";
        return ret.str();
    }

    double ProgramNode::Eval()
    {
    }

    void ProgramNode::ListHandler(const StatementPtr& pStatementRhs)
    {
        statementList.push_back(pStatementRhs);
    }
    
}