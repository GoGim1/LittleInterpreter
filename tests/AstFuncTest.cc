#include <gtest/gtest.h>
#include "Ast.h"

using namespace Ast;

TEST(AstFunc, Dump) 
{
    // ParamNode
    auto pToken = MakeTokenPtr(Token::IDENTIFIER, "i");
    auto pParam = MakeParamPtr(pToken);
    ASSERT_EQ(pParam->Dump(), "i");
    
     // ParamsNode
    auto pParams = MakeParamsPtr(pParam);
    ASSERT_EQ(pParams->Dump(), "i");
    pParams->ListHandler(pParam);
    ASSERT_EQ(pParams->Dump(), "i, i");

    // ParamListNode
    auto pParmaList = MakeParamListPtr(pParams);
    ASSERT_EQ(pParmaList->Dump(), "(i, i)");
    auto pParmaList1 = MakeParamListPtr();
    ASSERT_EQ(pParmaList1->Dump(), "()");
    
    // DefNode
    //  make block node 
    auto lhs = MakeTokenPtr(Token::INTEGER, "3");
    auto pPrimaryLhs = MakePrimaryPtr(nullptr, lhs);
    auto rhs = MakeTokenPtr(Token::IDENTIFIER, "id");
    auto pPrimaryRhs = MakePrimaryPtr(nullptr, rhs);
    auto pFactorLhs = MakeFactorPtr(pPrimaryLhs, nullptr);
    auto pFactorRhs = MakeFactorPtr(pPrimaryRhs, MakeTokenPtr(Token::SUB, "-"));
    auto pExpr2 = MakeExprPtr(pFactorLhs);
    pExpr2->ListHandler(MakeTokenPtr(Token::PLUS, "+"), pFactorRhs);
    auto pSimple = MakeSimplePtr(pExpr2);
    auto pStatementWithSimple = MakeStatementPtr(StatementNode::SIMPLE, nullptr, nullptr, nullptr, pSimple);
    auto pBlock = MakeBlockPtr(pStatementWithSimple);

    auto pDefToken = MakeTokenPtr(Token::IDENTIFIER, "func");
    auto pDef = MakeDefPtr(pDefToken, pParmaList, pBlock);
    ASSERT_EQ(pDef->Dump(), "def func(i, i){3+-id}");

    // ArgsNode
    auto pArgs = MakeArgsPtr(pExpr2);
    ASSERT_EQ(pArgs->Dump(), "3+-id");
    pArgs->ListHandler(pExpr2);
    ASSERT_EQ(pArgs->Dump(), "3+-id, 3+-id");

    // PostfixNode
    auto pPostfix = MakePostfixPtr(pArgs);
    ASSERT_EQ(pPostfix->Dump(), "(3+-id, 3+-id)");
    auto pPostfix1 = MakePostfixPtr();
    ASSERT_EQ(pPostfix1->Dump(), "()");
    
    
}