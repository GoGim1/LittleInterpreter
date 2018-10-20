#include "Ast.h"

namespace Ast
{
    /*************************************************************
		PrimaryNode
    *************************************************************/
    const string PrimaryNode::Dump() const
    {
        stringstream ret;
        if (pExpr)
           ret << "(" << pExpr->Dump() << ")";
        else 
            ret << pToken->Dump();
        return ret.str();
    }

    variant<int, double> PrimaryNode::Eval() 
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
                else 
                    throw Error("Runtime error: "+ pToken->getValue() +" is undefined.", pToken->getPosX(), pToken->getPosY());
            }
            default:
                Assert(0, "Eval Primary error");
            }
            
        }
        else if (pExpr)
            return pExpr->Eval(); 
        Assert(0, "Eval Primary error");        
    }
    
    bool PrimaryNode::isIdentifier() const
    {
        if (pToken && pToken->getType() == Token::IDENTIFIER) 
            return true;
        return false;
    }

    const string& PrimaryNode::GetIdentifierName() const
    {
        Assert(pToken->getType() == Token::IDENTIFIER, "GetIdentifierName error, token is not an IDENTIFIER type.");
        return pToken->getValue();
    }


    /*************************************************************
		FactorNode
    *************************************************************/
    const string FactorNode::Dump() const
    {
        stringstream ret;
        if (pToken)
           ret << pToken->Dump();
        ret << pPrimary->Dump();
        return ret.str();
    }

    variant<int, double> FactorNode::Eval()
    {
        
        if (pToken && pToken->getType() == Token::SUB)
            return 0 - GET(pPrimary->Eval());
        else    
            return GET(pPrimary->Eval());
    }

    bool FactorNode::isIdentifier() const
    {
        if (!pToken && pPrimary->isIdentifier()) 
            return true;
        return false;
    }

    const string& FactorNode::GetIdentifierName() const
    {
        return pPrimary->GetIdentifierName();
    }

    /*************************************************************
		ExprNode
    *************************************************************/
    const string ExprNode::Dump() const
    {
        stringstream ret;
        ret << pFactor->Dump();
        for (auto& i : factorList)
            ret << i.first->Dump() << i.second->Dump();
        return ret.str();  
    }

    // TODO 1.未定义标识符 2.定义标识符 3.不能赋值给右值 4.赋值给标识符
    variant<int, double> ExprNode::Eval()
    {
        vector<TokenPtr>                                    opStack;
        vector<variant< variant<int, double>, string> >     valStack;

        auto opTokenToInt = [](const TokenPtr& p) -> int
        {

            switch (p->getType())
            {
            case Token::DIV:    
            case Token::MUL:    
            case Token::MOD: 
                return 4;   
            case Token::PLUS:    
            case Token::SUB:  
                return 3;  
            case Token::GREATER:    
            case Token::GE:    
            case Token::LESS:    
            case Token::LE: 
                return 2;   
            case Token::EQUAL:
                return 1;
            case Token::ASSIGN:
                return 0;    
            default:
                Assert(0, "error type in opTokenToInt.");   
                return -1;                         
            }
        };

        auto stackCalculate = [&]()
        {
            auto tmp = valStack.back();
            auto rhs = std::holds_alternative<variant<int, double>>(tmp) ? 
                        GET((std::get<variant<int, double>>(tmp))) : GET(Env[std::get<string>(tmp)]);
            valStack.pop_back();

            tmp = valStack.back();
            auto lhs = std::holds_alternative<variant<int, double>>(tmp) ? 
                        GET((std::get<variant<int, double>>(tmp))) : GET(Env[std::get<string>(tmp)]);
            valStack.pop_back();

            TokenPtr op = opStack.back();
            opStack.pop_back();

            switch (op->getType())
            {
            case Token::DIV:    
                if (rhs == 0) 
                    throw Error("Runtime error: denominator of div operator can't be zero.", op->getPosX(), op->getPosY());
                valStack.push_back(lhs/rhs);
                break;
            case Token::MUL:      
                valStack.push_back(lhs*rhs);
                break; 
            case Token::MOD:    
                if (typeid(rhs) == typeid(double) || typeid(lhs)== typeid(double))
                    throw Error("Runtime error: parameters of mod operator can't be double.", op->getPosX(), op->getPosY());
                valStack.push_back((int)lhs%(int)rhs);
                break;
            case Token::PLUS:     
                valStack.push_back(lhs+rhs);
                break;  
            case Token::SUB:     
                valStack.push_back(lhs-rhs);
                break;
            case Token::GREATER:   
                valStack.push_back((int)(lhs>rhs));
                break;    
            case Token::GE:       
                valStack.push_back((int)(lhs>=rhs));
                break;
            case Token::LESS:      
                valStack.push_back((int)(lhs<rhs));
                break; 
            case Token::LE:    
                valStack.push_back((int)(lhs<=rhs));
                break;
            case Token::EQUAL:   
                valStack.push_back((int)(lhs==rhs));
                break;
            case Token::ASSIGN:   
                Assert(0, "assign have not been implenment.");
                break;
            default:
                Assert(0, "error type in stackCalculate.");   
            }
        };

        if (pFactor->isIdentifier()) 
            valStack.push_back(pFactor->GetIdentifierName());
        else 
            valStack.push_back(pFactor->Eval());
        opStack.push_back(MakeTokenPtr(Token::ASSIGN));
        
        for (auto& i : factorList)
        {
            if (opTokenToInt(i.first) < opTokenToInt(opStack.back()))
                stackCalculate();
            opStack.push_back(i.first);
            if (i.second->isIdentifier()) 
                valStack.push_back(i.second->GetIdentifierName());
            else 
                valStack.push_back(i.second->Eval());
        }
        
        while (valStack.size() != 1)
            stackCalculate();

        // TODO
        auto ret = valStack.back();
        return std::holds_alternative<variant<int, double>>(ret) ? std::get<variant<int, double>>(ret) : Env[std::get<string>(ret)];
    }
    
    void ExprNode::ListHandler(const TokenPtr& pTokenRhs, const FactorPtr& pFactorRhs)
    {
        factorList.push_back(make_pair((TokenPtr)pTokenRhs, (FactorPtr)pFactorRhs));
    }
   
    /*************************************************************
		BlockNode
    *************************************************************/
    const string BlockNode::Dump() const
    {
        stringstream ret;
        ret << "{" << (pStatement ? pStatement->Dump():"");
        for (auto& i : statementList)
            ret << ";" << (i ? i->Dump() : "");
        ret << "}";
        return ret.str();
    }
    
    variant<int, double> BlockNode::Eval()
    {
        if (pStatement && statementList.empty())
            return pStatement->Eval();
        if (!statementList.empty())
        {
            if (pStatement) pStatement->Eval();
            
            auto ret = variant<int, double>{};
            for (auto& i : statementList)
                if (i)
                    ret = i->Eval();
            return ret;
        }
        // pStatement, statementList all empty
        return variant<int, double>{0};
    }

    void BlockNode::ListHandler(const StatementPtr& pStatementRhs)
    {
        statementList.push_back(pStatementRhs);
    }
    
    /*************************************************************
		SimpleNode
    *************************************************************/
    const string SimpleNode::Dump() const 
    {
        return pExpr->Dump();
    }
    
    variant<int, double> SimpleNode::Eval()
    {
        return pExpr->Eval();
    }

    /*************************************************************
		StatementNode
    *************************************************************/
    const string StatementNode::Dump() const
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

    variant<int, double> StatementNode::Eval()
    {
        switch (type)
        {
        case SIMPLE: 
            return pSimple->Eval();
        case WHILE:
        {
            auto ret = variant<int, double>{0};
            while (GET(pExpr->Eval()))
                ret = pBlock->Eval();
            return ret;
        }
        case IF:
        {
            auto ret = variant<int, double>{0};
            if (GET(pExpr->Eval()))
                ret = pBlock->Eval();
            return ret;
        }
        case IFELSE:
        {
            auto ret = variant<int, double>{0};
            if (GET(pExpr->Eval()))
                ret = pBlock->Eval();
            else 
                ret = pBlockOfElse->Eval();
            return ret;
        }
        default:
            Assert(0, "error type in eval statement.");   
        }
    }

    /*************************************************************
		ProgramNode
    *************************************************************/
    const string ProgramNode::Dump() const
    {
        stringstream ret;
        for (auto& i : statementList)
            ret << (i ? i->Dump() : "") << ";";
        return ret.str();
    }

    variant<int, double> ProgramNode::Eval()
    {
        auto ret = variant<int, double>{0};
        for (auto& i : statementList)
            if (i)
                ret = i->Eval();
        return ret;
    }

    void ProgramNode::ListHandler(const StatementPtr& pStatementRhs)
    {
        statementList.push_back(pStatementRhs);
    }
    
}