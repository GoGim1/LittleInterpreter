#include "Ast.h"

namespace Environment
{
    using namespace Ast;
    
    extern void DefiniteFunc(const DefPtr&);
}

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

    void ParamsNode::ListHandler(const ParamPtr& pParamRhs)
    {
        paramList.push_back(pParamRhs);
    }
    
    const vector<string> ParamsNode::GetParams() const
    {
        vector<string> ret{};

        ret.push_back(pParam->Dump());

        for (auto& i : paramList)
            ret.push_back(i->Dump());

        return ret;
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

    const vector<string> ParamListNode::GetParams() const
    {
        if (pParams)
            return pParams->GetParams();
        else 
            return vector<string>{};
    }
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
    void DefNode::Definite()
    {
        DefiniteFunc(shared_from_this());
    }

    variant<int, double> DefNode::Eval(const vector<variant<int, double>>& args) 
    {
        const vector<string>& paramList = pParamList->GetParams();
        if (args.size() != paramList.size())    
            assert(0);
        
        for (size_t i = 0; i < args.size(); i++)
        {
            bool isInt = std::holds_alternative<int>(args[i]);
            if (isInt)
                Env[paramList[i]] = get<int>(args[i]); 
            else 
                Env[paramList[i]] = get<double>(args[i]); 
        }

        return pBlock->Eval();
    }

    const string& DefNode::GetFunctionName() const
    {
        return pToken->getValue();
    }

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
    
    void ArgsNode::ListHandler(const ExprPtr& pExprRhs)
    {
        exprList.push_back(pExprRhs);
    }
    
    const vector<variant<int, double>> ArgsNode::GetArgs() 
    {
        vector<variant<int, double>> ret{};

        ret.push_back(pExpr->Eval());

        for (auto& i : exprList)
            ret.push_back(i->Eval());

        return ret;
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

    const vector<variant<int, double>> PostfixNode::GetArgs() const
    {
        if (pArgs)
            return pArgs->GetArgs();
        else
            return vector<variant<int, double>>{};
    }
}