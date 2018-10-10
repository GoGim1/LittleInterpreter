#include <gtest/gtest.h>
#include <memory>
#include <iostream>
#include "Ast.h"
#include "Token.h"
#include "Helper.h"


using namespace std;
using namespace Lexer;
using namespace Ast;

TEST(Ast, CorrectDump) 
{
    auto lhs = MakeTokenPtr(Token::INTEGER, "3");
    auto pPrimaryLhs = MakePrimaryPtr(nullptr, lhs);
    ASSERT_EQ(pPrimaryLhs->Dump(), "3");

    auto rhs = MakeTokenPtr(Token::IDENTIFIER, "id");
    auto pPrimaryRhs = MakePrimaryPtr(nullptr, rhs);
    ASSERT_EQ(pPrimaryRhs->Dump(), "id");

    auto pFactorLhs = MakeFactorPtr(pPrimaryLhs, nullptr);
    ASSERT_EQ(pFactorLhs->Dump(), "3");

    auto pFactorRhs = MakeFactorPtr(pPrimaryRhs, MakeTokenPtr(Token::SUB, "-"));
    ASSERT_EQ(pFactorRhs->Dump(), "-id");

    auto pExpr1 = MakeExprPtr(pFactorRhs);
    ASSERT_EQ(pExpr1->Dump(), "-id");
    auto pPrimaryWithExpr1 = MakePrimaryPtr(pExpr1, nullptr);
    ASSERT_EQ(pPrimaryWithExpr1->Dump(), "(-id)");

    auto pExpr2 = MakeExprPtr(pFactorLhs);
    pExpr2->ListHandler(MakeTokenPtr(Token::PLUS, "+"), pFactorRhs);
    ASSERT_EQ(pExpr2->Dump(), "3+-id");
    auto pPrimaryWithExpr2 = MakePrimaryPtr(pExpr2, nullptr);
    ASSERT_EQ(pPrimaryWithExpr2->Dump(), "(3+-id)");
    
    auto pSimple = MakeSimplePtr(pExpr2);
    ASSERT_EQ(pSimple->Dump(), "3+-id");
    
    auto pStatementWithSimple = MakeStatementPtr(StatementNode::SIMPLE, nullptr, nullptr, nullptr, pSimple);
    ASSERT_EQ(pStatementWithSimple->Dump(), "3+-id");

    auto pBlock = MakeBlockPtr(pStatementWithSimple);
    ASSERT_EQ(pBlock->Dump(), "{3+-id}");
    pBlock->ListHandler(pStatementWithSimple);
    ASSERT_EQ(pBlock->Dump(), "{3+-id;3+-id}");
    pBlock->ListHandler(nullptr);
    ASSERT_EQ(pBlock->Dump(), "{3+-id;3+-id;}");

    auto pStatementWithIf = MakeStatementPtr(StatementNode::IF, pExpr1, pBlock, nullptr, nullptr);
    ASSERT_EQ(pStatementWithIf->Dump(), "if -id{3+-id;3+-id;}");
    
    auto pStatementWithIfElse = MakeStatementPtr(StatementNode::IFELSE, pExpr2, pBlock, pBlock, nullptr);
    ASSERT_EQ(pStatementWithIfElse->Dump(), "if 3+-id{3+-id;3+-id;}else {3+-id;3+-id;}");

    auto pStatementWithWhile = MakeStatementPtr(StatementNode::WHILE, pExpr2, pBlock, nullptr, nullptr);
    ASSERT_EQ(pStatementWithWhile->Dump(), "while 3+-id{3+-id;3+-id;}");
    
    auto pProgram = MakeProgramPtr();
    pProgram->ListHandler(pStatementWithSimple);
    pProgram->ListHandler(pStatementWithIf);
    pProgram->ListHandler(pStatementWithIfElse);
    pProgram->ListHandler(pStatementWithWhile);
    ASSERT_EQ(pProgram->Dump(), "3+-id;if -id{3+-id;3+-id;};if 3+-id{3+-id;3+-id;}else {3+-id;3+-id;};while 3+-id{3+-id;3+-id;};");



}