#include "Parser.h"

namespace Parse
{
    ParamPtr Parser::ParseParam()
    {   
        if (PeekToken().getType() != Token::IDENTIFIER)
            Assert(0, "parse token error, must be identifier.");
        return MakeParamPtr(MakeTokenPtr(NextToken()));
    }
    
    ParamsPtr Parser::ParseParams() 
    {
        ParamPtr pParam = ParseParam();
        if (!pParam)    
            assert(0);
        auto ret = MakeParamsPtr(pParam);
        while (PeekToken().getType() == Token::COMMA)
        {
            NextToken();
            pParam = ParseParam();
            if (!pParam)    
                assert(0);
            ret->ListHandler(pParam);
        }
        return ret;
        
    }

    ParamListPtr Parser::ParseParamList()
    {
        if (PeekToken().getType() != Token::LBRACKET)
            assert(0);
        NextToken();

        if (PeekToken().getType() == Token::RBRACKET)
        {
            NextToken();
            return MakeParamListPtr();
        }

        auto pParams = ParseParams();
        if (!pParams)
            assert(0);
        if (PeekToken().getType() != Token::RBRACKET)
            assert(0);
        NextToken();
        return MakeParamListPtr(pParams);
        
    }

    DefPtr Parser::ParseDef()
    {
        if (PeekToken().getType() != Token::DEF)
            assert(0);
        NextToken();

        auto pToken = MakeTokenPtr(NextToken());
        if (!pToken || pToken->getType() != Token::IDENTIFIER)
            assert(0);
        
        auto pParamList = ParseParamList();
        if (!pParamList)
            assert(0);

        auto pBlock = ParseBlock();
        if (!pBlock)
            assert(0);

        return MakeDefPtr(pToken, pParamList, pBlock);

    }

    ArgsPtr Parser::ParseArgs()
    {
        ExprPtr pExpr = ParseExpr();
        if (!pExpr)    
            assert(0);
        auto ret = MakeArgsPtr(pExpr);
        while (PeekToken().getType() == Token::COMMA)
        {
            NextToken();
            pExpr = ParseExpr();
            if (!pExpr)    
                assert(0);
            ret->ListHandler(pExpr);
        }
        return ret;
    }

    PostfixPtr Parser::ParsePostfix()
    {
        if (PeekToken().getType() != Token::LBRACKET)
            assert(0);
        NextToken();

        if (PeekToken().getType() == Token::RBRACKET)
        {
            NextToken();
            return MakePostfixPtr();
        }

        auto pArgs = ParseArgs();
        if (!pArgs)
            assert(0);
        if (PeekToken().getType() != Token::RBRACKET)
            assert(0);
        NextToken();
        return MakePostfixPtr(pArgs);
    }
}