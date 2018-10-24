#include <gtest/gtest.h>
#include "Ast.h"

using namespace Ast;

//  make block node 
auto lhs = MakeTokenPtr(Token::INTEGER, "3");
auto pPrimaryLhs = MakePrimaryPtr(nullptr, lhs);
auto rhs = MakeTokenPtr(Token::IDENTIFIER, "id");
auto pPrimaryRhs = MakePrimaryPtr(nullptr, rhs);
auto pFactorLhs = MakeFactorPtr(pPrimaryLhs, nullptr);
auto pFactorRhs = MakeFactorPtr(pPrimaryRhs, MakeTokenPtr(Token::SUB, "-"));
auto pExpr2 = MakeExprPtr(pFactorLhs);
auto pSimple = MakeSimplePtr(pExpr2);
auto pStatementWithSimple = MakeStatementPtr(StatementNode::SIMPLE, nullptr, nullptr, nullptr, pSimple);
auto pBlock = MakeBlockPtr(pStatementWithSimple);

TEST(AstFunc, NewNodeDump) 
{
    pExpr2->ListHandler(MakeTokenPtr(Token::PLUS, "+"), pFactorRhs);
    
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

TEST(AstFunc, OldNodeDump) 
{
    // PrimaryNode
    auto pArgs = MakeArgsPtr(pExpr2);
    auto pPostfix = MakePostfixPtr(pArgs);  //(3+-id)
    auto pPrimary = MakePrimaryPtr(nullptr, rhs, pPostfix);
    ASSERT_EQ(pPrimary->Dump(), "id(3+-id)");
    auto pPrimary1 = MakePrimaryPtr(nullptr, lhs, pPostfix);
    ASSERT_EQ(pPrimary1->Dump(), "3(3+-id)");
    auto pPrimary2 = MakePrimaryPtr(pExpr2, nullptr, pPostfix);
    ASSERT_EQ(pPrimary2->Dump(), "(3+-id)(3+-id)");

    // SimpleNode
    auto pSimple1 = MakeSimplePtr(pExpr2, pArgs);
    ASSERT_EQ(pSimple1->Dump(), "3+-id 3+-id");

    // ProgramNode
    auto pToken = MakeTokenPtr(Token::IDENTIFIER, "i");
    auto pParam = MakeParamPtr(pToken);
    auto pParams = MakeParamsPtr(pParam);
    pParams->ListHandler(pParam);
    auto pParmaList = MakeParamListPtr(pParams);
    auto pDefToken = MakeTokenPtr(Token::IDENTIFIER, "func");
    auto pDef = MakeDefPtr(pDefToken, pParmaList, pBlock);
    
    auto pProgram1 = MakeProgramPtr();
    pProgram1->DefListHandler(pDef);
    ASSERT_EQ(pProgram1->Dump(), R"TEST(def func(i, i){3+-id}
)TEST");
    pProgram1->ListHandler(pStatementWithSimple);
    ASSERT_EQ(pProgram1->Dump(), R"TEST(def func(i, i){3+-id}
3+-id;)TEST");
    pProgram1->DefListHandler(pDef);
    ASSERT_EQ(pProgram1->Dump(), R"TEST(def func(i, i){3+-id}
def func(i, i){3+-id}
3+-id;)TEST");
}
