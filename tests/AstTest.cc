#include <gtest/gtest.h>
#include "Ast.h"
#include "tools/FloatCompare.h"

using namespace Ast;

TEST(Ast, CorrectDump) 
{
    // Primary test    
    auto lhs = MakeTokenPtr(Token::INTEGER, "3");
    auto pPrimaryLhs = MakePrimaryPtr(nullptr, lhs);
    ASSERT_EQ(pPrimaryLhs->Dump(), "3");

    auto rhs = MakeTokenPtr(Token::IDENTIFIER, "id");
    auto pPrimaryRhs = MakePrimaryPtr(nullptr, rhs);
    ASSERT_EQ(pPrimaryRhs->Dump(), "id");

    // Factor test
    auto pFactorLhs = MakeFactorPtr(pPrimaryLhs, nullptr);
    ASSERT_EQ(pFactorLhs->Dump(), "3");

    auto pFactorRhs = MakeFactorPtr(pPrimaryRhs, MakeTokenPtr(Token::SUB, "-"));
    ASSERT_EQ(pFactorRhs->Dump(), "-id");

    // Expr test
    auto pExpr1 = MakeExprPtr(pFactorRhs);
    ASSERT_EQ(pExpr1->Dump(), "-id");
    auto pPrimaryWithExpr1 = MakePrimaryPtr(pExpr1, nullptr);
    ASSERT_EQ(pPrimaryWithExpr1->Dump(), "(-id)");

    auto pExpr2 = MakeExprPtr(pFactorLhs);
    pExpr2->ListHandler(MakeTokenPtr(Token::PLUS, "+"), pFactorRhs);
    ASSERT_EQ(pExpr2->Dump(), "3+-id");
    auto pPrimaryWithExpr2 = MakePrimaryPtr(pExpr2, nullptr);
    ASSERT_EQ(pPrimaryWithExpr2->Dump(), "(3+-id)");
    
    // Simple test
    auto pSimple = MakeSimplePtr(pExpr2);
    ASSERT_EQ(pSimple->Dump(), "3+-id");
    
    // Statement test
    auto pStatementWithSimple = MakeStatementPtr(StatementNode::SIMPLE, nullptr, nullptr, nullptr, pSimple);
    ASSERT_EQ(pStatementWithSimple->Dump(), "3+-id");

    // Block test
    auto pBlock = MakeBlockPtr(pStatementWithSimple);
    ASSERT_EQ(pBlock->Dump(), "{3+-id}");
    pBlock->ListHandler(pStatementWithSimple);
    ASSERT_EQ(pBlock->Dump(), "{3+-id;3+-id}");
    pBlock->ListHandler(nullptr);
    ASSERT_EQ(pBlock->Dump(), "{3+-id;3+-id;}");

    // Statement test
    auto pStatementWithIf = MakeStatementPtr(StatementNode::IF, pExpr1, pBlock, nullptr, nullptr);
    ASSERT_EQ(pStatementWithIf->Dump(), "if -id{3+-id;3+-id;}");
    
    auto pStatementWithIfElse = MakeStatementPtr(StatementNode::IFELSE, pExpr2, pBlock, pBlock, nullptr);
    ASSERT_EQ(pStatementWithIfElse->Dump(), "if 3+-id{3+-id;3+-id;}else{3+-id;3+-id;}");

    auto pStatementWithWhile = MakeStatementPtr(StatementNode::WHILE, pExpr2, pBlock, nullptr, nullptr);
    ASSERT_EQ(pStatementWithWhile->Dump(), "while 3+-id{3+-id;3+-id;}");
    
    // Program test
    auto pProgram = MakeProgramPtr();
    pProgram->ListHandler(pStatementWithSimple);
    pProgram->ListHandler(pStatementWithIf);
    pProgram->ListHandler(pStatementWithIfElse);
    pProgram->ListHandler(pStatementWithWhile);
    ASSERT_EQ(pProgram->Dump(), "3+-id;if -id{3+-id;3+-id;};if 3+-id{3+-id;3+-id;}else{3+-id;3+-id;};while 3+-id{3+-id;3+-id;};");
}

TEST(Ast, EvalStep)
{
    // Primary test
    auto pToken1 = MakeTokenPtr(Token::INTEGER, "3");
    auto pPrimary1 = MakePrimaryPtr(nullptr, pToken1);
    auto val1 = GET(pPrimary1->Eval());
    ASSERT_EQ(val1, 3);

    auto pToken2 = MakeTokenPtr(Token::FLOAT, "2.0");
    auto pPrimary2 = MakePrimaryPtr(nullptr, pToken2);
    auto val2 = GET(pPrimary2->Eval());
    ASSERT_TRUE(IsAlmostEqual(val2, 2.0));

    // Factor test
    auto pFactor1 = MakeFactorPtr(pPrimary1, nullptr);
    auto val3 = GET(pFactor1->Eval());
    ASSERT_EQ(val3, 3);

    auto pFactor2 = MakeFactorPtr(pPrimary2, MakeTokenPtr(Token::SUB, "-"));
    auto val4 = GET(pFactor2->Eval());
    ASSERT_TRUE(IsAlmostEqual(val4, -2.0));

    // Expr test
    auto pExpr1 = MakeExprPtr(pFactor1);  // int 3
    pExpr1->ListHandler(MakeTokenPtr(Token::PLUS, "+"), pFactor1); // + int 3
    pExpr1->ListHandler(MakeTokenPtr(Token::PLUS, "+"), pFactor2); // + float -2.0
    auto val5 = GET(pExpr1->Eval());
    ASSERT_TRUE(IsAlmostEqual(val5, 4.0));

    auto pExpr2 = MakeExprPtr(pFactor1);  // int 3
    pExpr2->ListHandler(MakeTokenPtr(Token::PLUS, "+"), pFactor2); // + float -2.0
    pExpr2->ListHandler(MakeTokenPtr(Token::MUL, "*"), pFactor1); // * int 3
    pExpr2->ListHandler(MakeTokenPtr(Token::PLUS, "+"), pFactor1); // + int 3
    auto val6 = GET(pExpr2->Eval());  // 3+-2.0*3+3
    ASSERT_TRUE(IsAlmostEqual(val6, 0.0));

    auto pExpr3 = MakeExprPtr(pFactor1);  // int 3
    pExpr3->ListHandler(MakeTokenPtr(Token::PLUS, "+"), pFactor2); // + float -2.0
    pExpr3->ListHandler(MakeTokenPtr(Token::GE, ">="), pFactor1); // >= int 3
    auto val7 = GET(pExpr3->Eval());  // 3+-2.0>=3
    ASSERT_EQ(val7, 0);

    auto pExpr4 = MakeExprPtr(pFactor1);  // int 3
    pExpr4->ListHandler(MakeTokenPtr(Token::EQUAL, "=="), pFactor2); // == float -2.0
    pExpr4->ListHandler(MakeTokenPtr(Token::GE, ">="), pFactor1); // >= int 3
    auto val8 = GET(pExpr4->Eval());  // 3==2.0>=3
    ASSERT_EQ(val8, 0);
    auto pPrimary3 = MakePrimaryPtr(pExpr4, nullptr);
    auto val9 = GET(pPrimary3->Eval());  // 3==2.0>=3
    ASSERT_EQ(val9, 0);

    // Simple test
    auto pSimple = MakeSimplePtr(pExpr4);
    auto val10 = GET(pSimple->Eval());  // 3==2.0>=3
    ASSERT_EQ(val10, 0);

    // Statement test
    auto pStatementWithSimple = MakeStatementPtr(StatementNode::SIMPLE, nullptr, nullptr, nullptr, pSimple);
    auto val11 = GET(pStatementWithSimple->Eval());  // 3==2.0>=3
    ASSERT_EQ(val11, 0);

     // Block test
    auto pBlock = MakeBlockPtr(pStatementWithSimple);
    auto val12 = GET(pBlock->Eval());  // {3==2.0>=3 }
    ASSERT_EQ(val12, 0);

    pBlock->ListHandler(pStatementWithSimple);
    auto val13 = GET(pBlock->Eval());  // {3==2.0>=3; 3==2.0>=3 }
    ASSERT_EQ(val13, 0);

    pBlock->ListHandler(nullptr);
    auto val14 = GET(pBlock->Eval());  // {3==2.0>=3; 3==2.0>=3; }
    ASSERT_EQ(val14, 0);

    // Statement test
    // pExpr1 = 4, pExpr2 = 0; pBlock = 0, pBlock2 = 4.0
    auto pSimple2 = MakeSimplePtr(pExpr1);
    auto pStatementWithSimple2 = MakeStatementPtr(StatementNode::SIMPLE, nullptr, nullptr, nullptr, pSimple2);
    auto pBlock2 = MakeBlockPtr(pStatementWithSimple2);// { 3 + 3 + -2.0 } = 4.0
    
    auto pStatementWithIf = MakeStatementPtr(StatementNode::IF, pExpr1, pBlock2, nullptr, nullptr);
    auto val15 = GET(pStatementWithIf->Eval());  
    ASSERT_TRUE(IsAlmostEqual(val15, 4.0));

    auto pStatementWithIf2 = MakeStatementPtr(StatementNode::IF, pExpr2, pBlock2, nullptr, nullptr);
    auto val16 = GET(pStatementWithIf2->Eval());  
    ASSERT_EQ(val16, 0);
    
    auto pStatementWithIfElse = MakeStatementPtr(StatementNode::IFELSE, pExpr2, pBlock, pBlock2, nullptr);
    auto val17 = GET(pStatementWithIfElse->Eval());  
    ASSERT_TRUE(IsAlmostEqual(val17, 4.0));

    auto pStatementWithIfElse2 = MakeStatementPtr(StatementNode::IFELSE, pExpr1, pBlock, pBlock2, nullptr);
    auto val18 = GET(pStatementWithIfElse2->Eval());  
    ASSERT_EQ(val18, 0);

    // TODO: solve after implement Env 
    // auto pStatementWithWhile = MakeStatementPtr(StatementNode::WHILE, pExpr2, pBlock, nullptr, nullptr);
    // auto val19 = GET(pStatementWithWhile->Eval());  
    // ASSERT_EQ(val19, 4);

    // Program test
    auto pProgram = MakeProgramPtr();
    auto val24 = GET(pProgram->Eval());  
    ASSERT_EQ(val24, 0);
    
    pProgram->ListHandler(pStatementWithSimple);
    auto val20 = GET(pProgram->Eval());  
    ASSERT_EQ(val20, 0);

    pProgram->ListHandler(pStatementWithIf); 
    auto val21 = GET(pProgram->Eval());  
    ASSERT_TRUE(IsAlmostEqual(val21, 4.0));

    pProgram->ListHandler(nullptr); 
    auto val22 = GET(pProgram->Eval());  
    ASSERT_TRUE(IsAlmostEqual(val22, 4.0));

    pProgram->ListHandler(pStatementWithIfElse2);
    auto val23 = GET(pProgram->Eval());  
    ASSERT_EQ(val23, 0);
}

TEST(Ast, EnvNormal)
{
    
    Env["double"] = 1.3; Env["int"] = 1;

    {
        // Primary Factor test
        auto pToken1 = MakeTokenPtr(Token::IDENTIFIER, "double");
        auto pPrimary1 = MakePrimaryPtr(nullptr, pToken1);
        auto val1 = GET(pPrimary1->Eval());
        ASSERT_TRUE(IsAlmostEqual(val1, 1.3));
        auto pFactor1 = MakeFactorPtr(pPrimary1, MakeTokenPtr(Token::SUB, "-"));
        auto val2 = GET(pFactor1->Eval());
        ASSERT_TRUE(IsAlmostEqual(val2, -1.3));  

        auto pToken2 = MakeTokenPtr(Token::IDENTIFIER, "int");
        auto pPrimary2 = MakePrimaryPtr(nullptr, pToken2);
        auto val3 = GET(pPrimary2->Eval());
        ASSERT_EQ(val3, 1);
        auto pFactor2 = MakeFactorPtr(pPrimary2, nullptr);
        auto val4 = GET(pFactor2->Eval());
        ASSERT_EQ(val4, 1);  

        auto pToken3 = MakeTokenPtr(Token::INTEGER, "2");
        auto pPrimary3 = MakePrimaryPtr(nullptr, pToken3);
        auto pFactor3 = MakeFactorPtr(pPrimary3, nullptr);
        auto val5 = GET(pFactor3->Eval());
        ASSERT_EQ(val5, 2);

        // Expr test
        // pFactor1 = "double" -1.3, pFactor2 = "int" 1, pFactor3 = int 2
        auto pExpr1 = MakeExprPtr(pFactor1);  //"double" -1.3
        auto val6 = GET(pExpr1->Eval());  // "double"
        ASSERT_TRUE(IsAlmostEqual(val6, -1.3));

        pExpr1->ListHandler(MakeTokenPtr(Token::PLUS, "+"), pFactor2); // + "int" 1
        auto val7 = GET(pExpr1->Eval());  // "double" + "int" 
        ASSERT_TRUE(IsAlmostEqual(val7, -0.3));

        pExpr1->ListHandler(MakeTokenPtr(Token::MUL, "*"), pFactor3); // + int 2
        auto val8 = GET(pExpr1->Eval()); // "double" + "int" * 2
        ASSERT_TRUE(IsAlmostEqual(val8, 0.7));
    }
}

TEST(Ast, Env)
{
    
}

TEST(Ast, RuntimeError)
{
    
}