#include "Ast.h"

namespace Ast
{
    /*************************************************************
        ParamNode
    *************************************************************/
    ParamNode::ParamNode(const TokenPtr& pTokenRhs) : pToken(pTokenRhs)
    {
        Assert(pToken, "ParamNode pToken can't be nullptr.");       
        Assert(pToken->getType() == Token::IDENTIFIER, "ParamNode pToken must be IDENTIFIER.");                         
    }

    const string ParamNode::Dump() const
    {
        return pToken->Dump();
    }

    variant<int, double> ParamNode::Eval()
    {

    }  

    /*************************************************************
        ParamsNode
    *************************************************************/
    ParamsNode::ParamsNode(const ParamPtr& pParamRhs) : pParam(pParamRhs)
    {   
        Assert(pParam, "ParamsNode's pParam can't be nullptr");
    }

    const string ParamsNode::Dump() const 
    {
        stringstream ret;
        ret << pParam->Dump();
        for (auto& i : paramList)
            ret << ", " << i->Dump();
        return ret.str();
    }

    variant<int, double> ParamsNode::Eval() {}

    void ParamsNode::ListHandler(const ParamPtr& pParamRhs)
    {
        paramList.push_back(pParamRhs);
    }
    
    /*************************************************************
        ParamListNode
    *************************************************************/
    ParamListNode::ParamListNode(const ParamsPtr& pParamsRhs): pParams(pParamsRhs) {}
    
    const string ParamListNode::Dump() const
    {   
        if (!pParams)   
            return "()";
        else    
            return "(" + pParams->Dump() + ")";
    }

    variant<int, double> ParamListNode::Eval() 
    {}
    /*************************************************************
        DefNode
    *************************************************************/
    DefNode::DefNode(const TokenPtr& pTokenRhs, const ParamListPtr& pParamListRhs, const BlockPtr& pBlockRhs):
                    pToken(pTokenRhs), pParamList(pParamListRhs), pBlock(pBlockRhs) 
    {
        Assert(pToken && pBlock && pParamList, "DefNode's pToken, pBlock, pParamList can't be nullptr.");
        Assert(pToken->getType() == Token::IDENTIFIER, "DefNode's pToken must be IDENTIFIER.");
    }
    
    const string DefNode::Dump() const 
    {
        return "def " + pToken->Dump() + pParamList->Dump() + pBlock->Dump();
    }

    variant<int, double> DefNode::Eval() {}

    /*************************************************************
        ArgsNode
    *************************************************************/
    ArgsNode::ArgsNode(const ExprPtr& pExprRhs) : pExpr(pExprRhs)
    {
        Assert(pExpr, "ArgsNode's pExpr can't be nullptr");
    }
    
    const string ArgsNode::Dump() const 
    {
        stringstream ret;
        ret << pExpr->Dump();
        for (auto& i : exprList)
            ret << ", " << i->Dump();
        return ret.str();
    }
    
    variant<int, double> ArgsNode::Eval() 
    {}

    void ArgsNode::ListHandler(const ExprPtr& pExprRhs)
    {
        exprList.push_back(pExprRhs);
    }
    
    /*************************************************************
        PostfixNode
    *************************************************************/
    PostfixNode::PostfixNode(const ArgsPtr& pArgsRhs): pArgs(pArgsRhs) {}
    
    const string PostfixNode::Dump() const
    {   
        if (!pArgs)   
            return "()";
        else    
            return "(" + pArgs->Dump() + ")";
    }

    variant<int, double> PostfixNode::Eval() 
    {}
}