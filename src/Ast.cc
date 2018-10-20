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
                return std::stoi(pToken->getValue());    
            case Token::FLOAT:
                return std::stod(pToken->getValue());                        
            case Token::IDENTIFIER:
            {
                auto value = Env.find(pToken->getValue()); 
                if (value != Env.end())
                    return value->second;
                else 
                    throw Error("Runtime error: \""+ pToken->getValue() +"\" is undefined.", pToken->getPosX(), pToken->getPosY());
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

    const pair<int, int> PrimaryNode::GetIdentifierPos() const
    {
        Assert(pToken, "pToken is not exist.");
        return { pToken->getPosX(), pToken->getPosY() };
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
        auto ret = pPrimary->Eval();
        bool isInt = std::holds_alternative<int>(ret);
        if (isInt)
        {
            if (pToken && pToken->getType() == Token::SUB)
                return 0 - std::get<int>(pPrimary->Eval());
            else    
                return std::get<int>(pPrimary->Eval());
        }
        else
        {
            if (pToken && pToken->getType() == Token::SUB)
                return 0 - std::get<double>(pPrimary->Eval());
            else    
                return std::get<double>(pPrimary->Eval());
        }
        
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


    const pair<int, int> FactorNode::GetIdentifierPos() const
    {
        return pPrimary->GetIdentifierPos();
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
        using NumValue          = variant<int, double>;
        using IdentifierValue   = tuple<string, int, int>;  // identifier's value, posX and posY
        using valStackItem      = variant<NumValue, IdentifierValue>;

        vector<TokenPtr>        opStack;
        vector<valStackItem>    valStack;

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
            bool isLhsInt, isRhsInt;
            double lhs, rhs;
            /*****************************************************
             * deal with the most top Item
             *****************************************************/
            auto item = valStack.back();
            auto rhsNumValue = NumValue{}; //store the NumValue in Item 
            bool isNumValue = std::holds_alternative<NumValue>(item);
            if (!isNumValue) // rhsItemType is IdentifierValue 
            {
                auto _IdentifierValue = get<IdentifierValue>(item);
                if (Env.find(get<0>(_IdentifierValue)) == Env.end())   
                    throw Error("Runtime error: \"" + get<0>(_IdentifierValue) + "\" undefined.", 
                        get<1>(_IdentifierValue), get<2>(_IdentifierValue));
                else 
                    rhsNumValue = Env[get<0>(_IdentifierValue)];
            }
            else // rhsItemType is NumValue 
                rhsNumValue = get<NumValue>(item);

            isRhsInt = std::holds_alternative<int>(rhsNumValue);
            rhs = GET(rhsNumValue);
            valStack.pop_back();

            /***********************************************************
             * deal with the second top Item
             ***********************************************************/
            TokenPtr op = opStack.back();
            opStack.pop_back();
            bool isAssign = op->getType() == Token::ASSIGN;

            /**************************************************************
             * deal with the second top Item
             **************************************************************/
            item = valStack.back();
            auto lhsNumValue = NumValue{0}; //store the NumValue in Item 
            isNumValue = std::holds_alternative<NumValue>(item);
            if (!isNumValue) // lhsItemType is IdentifierValue 
            {
                auto _IdentifierValue = get<IdentifierValue>(item);
                if (Env.find(get<0>(_IdentifierValue)) == Env.end())  
                {
                    if (isAssign)
                        Env[get<0>(_IdentifierValue)] = NumValue{0};
                    else 
                        throw Error("Runtime error: \"" + get<0>(_IdentifierValue) + "\" undefined.", 
                            get<1>(_IdentifierValue), get<2>(_IdentifierValue));
                } 
                else 
                    lhsNumValue = Env[get<0>(_IdentifierValue)];
            }
            else // lhsItemType is NumValue 
            {
                if (isAssign)
                    throw Error("Runtime error: can not assign to r-value.", op->getPosX(), op->getPosY());
                else
                    lhsNumValue = get<NumValue>(item);
            } 
            isLhsInt = std::holds_alternative<int>(lhsNumValue);
            lhs = GET(lhsNumValue);
            valStack.pop_back();
            
            switch (op->getType())
            {
            case Token::DIV:    
                if (rhs == 0) 
                    throw Error("Runtime error: denominator of div operator can't be zero.", op->getPosX(), op->getPosY());
                if (isLhsInt && isRhsInt)
                    valStack.push_back((int)lhs/(int)rhs);
                else
                    valStack.push_back(lhs/rhs);
                break;
            case Token::MUL:  
                if (isLhsInt && isRhsInt)
                    valStack.push_back((int)lhs*(int)rhs);
                else    
                    valStack.push_back(lhs*rhs);
                break; 
            case Token::MOD:    
                if (isLhsInt && isRhsInt)
                    valStack.push_back((int)lhs%(int)rhs);
                else 
                    throw Error("Runtime error: parameters of mod operator can't be double.", op->getPosX(), op->getPosY());
                break;
            case Token::PLUS:
                if (isLhsInt && isRhsInt)
                    valStack.push_back((int)lhs+(int)rhs);
                else         
                    valStack.push_back(lhs+rhs);
                break;  
            case Token::SUB:  
                if (isLhsInt && isRhsInt)
                    valStack.push_back((int)lhs-(int)rhs);
                else       
                    valStack.push_back(lhs-rhs);
                break;
            case Token::GREATER:   
                valStack.push_back((int)(lhs>rhs));
                break;    
            case Token::GE:       
                valStack.push_back((int)!(lhs<rhs));
                break;
            case Token::LESS:      
                valStack.push_back((int)(lhs<rhs));
                break; 
            case Token::LE:    
                valStack.push_back((int)!(lhs>rhs));
                break;
            case Token::EQUAL:   
                valStack.push_back((int)IsAlmostEqual(lhs, rhs));
                break;
            case Token::ASSIGN:   
                if (isRhsInt)
                {
                    Env[get<0>(get<IdentifierValue>(item))] = (int)rhs;
                    valStack.push_back((int)rhs);
                }
                else 
                {
                    Env[get<0>(get<IdentifierValue>(item))] = rhs;
                    valStack.push_back(rhs); 
                }
                break;
            default:
                Assert(0, "error type in stackCalculate.");   
            }
        };

        if (pFactor->isIdentifier()) 
            valStack.push_back(make_tuple(pFactor->GetIdentifierName(), 
                                    pFactor->GetIdentifierPos().first, pFactor->GetIdentifierPos().second));
        else 
            valStack.push_back(pFactor->Eval());
        opStack.push_back(MakeTokenPtr(Token::ASSIGN));
        
        for (auto& i : factorList)
        {
            if (opTokenToInt(i.first) < opTokenToInt(opStack.back()))
                stackCalculate();
            opStack.push_back(i.first);
            if (i.second->isIdentifier()) 
                valStack.push_back(make_tuple(i.second->GetIdentifierName(),
                                    i.second->GetIdentifierPos().first, i.second->GetIdentifierPos().second));
            else 
                valStack.push_back(i.second->Eval());
        }
        
        while (valStack.size() != 1)
            stackCalculate();

        auto ret = valStack.back();
        return std::holds_alternative<NumValue>(ret) ? get<NumValue>(ret) : Env[get<0>(get<IdentifierValue>(ret))];
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
            if (pStatement) 
                pStatement->Eval();
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
            {
                ret = pBlock->Eval();
            }
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