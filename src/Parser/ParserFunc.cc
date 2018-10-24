#include "Parser.h"

namespace Parse
{
    ParamPtr Parser::ParseParam()
    {   
        auto paramFirst = PeekToken();
        if (paramFirst.getType() != Token::IDENTIFIER)
        {
            AddError(Error("ParamNode error: param must be IDENTIFIER.", paramFirst.getPosX(), paramFirst.getPosY()));
            return nullptr;
        }
        return MakeParamPtr(MakeTokenPtr(NextToken()));
    }
    
    ParamsPtr Parser::ParseParams() 
    {
        auto paramsPeek = PeekToken();

        ParamPtr pParam = ParseParam();
        if (!pParam)    
        {
            AddError(Error("ParamsNode error: parsing ParamNode fail.", paramsPeek.getPosX(), paramsPeek.getPosY()));
            return nullptr;
        }
    
        auto ret = MakeParamsPtr(pParam);
        while (PeekToken().getType() == Token::COMMA)
        {
            NextToken();
            paramsPeek = PeekToken();
            pParam = ParseParam();
            if (!pParam)    
            {
                AddError(Error("ParamsNode error: parsing ParamNode list fail.", paramsPeek.getPosX(), paramsPeek.getPosY()));
                return nullptr;
            }
            ret->ListHandler(pParam);
        }
        return ret;
        
    }

    ParamListPtr Parser::ParseParamList()
    {
        auto paramListPeek = PeekToken();

        if (paramListPeek.getType() != Token::LBRACKET)
        {
            AddError(Error("ParamListNode error: param list must start with \"(\".", paramListPeek.getPosX(), paramListPeek.getPosY()));
            return nullptr;
        }
        NextToken();

        if (PeekToken().getType() == Token::RBRACKET)
        {
            NextToken();
            return MakeParamListPtr();
        }

        paramListPeek = PeekToken();
        auto pParams = ParseParams();
        if (!pParams)
        {
            AddError(Error("ParamListNode error: parsing ParamsNode fail.", paramListPeek.getPosX(), paramListPeek.getPosY()));
            return nullptr;
        }
        paramListPeek = PeekToken();
        if (paramListPeek.getType() != Token::RBRACKET)
        {
            AddError(Error("ParamListNode error: param list must end with \")\".", paramListPeek.getPosX(), paramListPeek.getPosY()));
            return nullptr;
        }
        NextToken();

        return MakeParamListPtr(pParams);
        
    }

    DefPtr Parser::ParseDef()
    {
        auto defPeek = PeekToken();
        if (defPeek.getType() != Token::DEF)
        {
            AddError(Error("DefNode error: function definition must start with \"def\".", defPeek.getPosX(), defPeek.getPosY()));
            return nullptr;
        }
        NextToken();

        auto pToken = MakeTokenPtr(NextToken());
        if (!pToken || pToken->getType() != Token::IDENTIFIER)
        {
            AddError(Error("DefNode error: function name must be IDENTIFIER.", pToken->getPosX(), pToken->getPosY()));
            return nullptr;
        }
        
        defPeek = PeekToken();
        auto pParamList = ParseParamList();
        if (!pParamList)
        {
            AddError(Error("DefNode error: parsing function param list fail.", defPeek.getPosX(), defPeek.getPosY()));
            return nullptr;
        }

        defPeek = PeekToken();
        auto pBlock = ParseBlock();
        if (!pBlock)
        {
            AddError(Error("DefNode error: parsing function block fail.", defPeek.getPosX(), defPeek.getPosY()));
            return nullptr;
        }

        return MakeDefPtr(pToken, pParamList, pBlock);

    }

    ArgsPtr Parser::ParseArgs()
    {
        auto argsPeek = PeekToken();

        ExprPtr pExpr = ParseExpr();
        if (!pExpr)    
        {
            AddError(Error("ArgsNode error: parsing expr fail.", argsPeek.getPosX(), argsPeek.getPosY()));
            return nullptr;
        }
        auto ret = MakeArgsPtr(pExpr);

        while (PeekToken().getType() == Token::COMMA)
        {
            NextToken();
            argsPeek = PeekToken();
            pExpr = ParseExpr();
            if (!pExpr)    
            {
                AddError(Error("ArgsNode error: parsing expr fail.", argsPeek.getPosX(), argsPeek.getPosY()));
                return nullptr;
            }
            ret->ListHandler(pExpr);
        }

        return ret;
    }

    PostfixPtr Parser::ParsePostfix()
    {
        auto postfixPeek = PeekToken();
        if (postfixPeek.getType() != Token::LBRACKET)
        {
            AddError(Error("PostfixNode error: postfix must start with \"(\".", postfixPeek.getPosX(), postfixPeek.getPosY()));
            return nullptr;
        }
        NextToken();

        if (PeekToken().getType() == Token::RBRACKET)
        {
            NextToken();
            return MakePostfixPtr();
        }

        postfixPeek = PeekToken();
        auto pArgs = ParseArgs();
        if (!pArgs)
        {
            AddError(Error("PostfixNode error: parsing args fail.", postfixPeek.getPosX(), postfixPeek.getPosY()));
            return nullptr;
        }

        postfixPeek = PeekToken();
        if (PeekToken().getType() != Token::RBRACKET)
        {
            AddError(Error("PostfixNode error: postfix must start with \")\".", postfixPeek.getPosX(), postfixPeek.getPosY()));
            return nullptr;
        }
        NextToken();
        return MakePostfixPtr(pArgs);
    }
}