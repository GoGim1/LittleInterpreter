#include <gtest/gtest.h>
#include "tools/FloatCompare.h"
#include "Parser.h"
#include "Lexer.h"
#include "Ast.h"

using namespace Parse;
using namespace Lexer;
using namespace Ast;

TEST(Ast, EnvNormal)
{
    DefineIdentifier("double", 1.3);
    DefineIdentifier("int", 1);
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
    {
        RunLexer("a = 1");
        Parser parser; 
        parser.RunParser();       
        ASSERT_TRUE(IsAlmostEqual(GET(parser.EvalValue()), 1.0));
        IdentifierTableClean();
    }
    {
        RunLexer("a = 1; a");
        Parser parser; 
        parser.RunParser();       
        ASSERT_TRUE(IsAlmostEqual(GET(parser.EvalValue()), 1.0));
        IdentifierTableClean();
    }
    {
        RunLexer("a = 1; a = a + 1; ");
        Parser parser; 
        parser.RunParser();       
        ASSERT_TRUE(IsAlmostEqual(GET(parser.EvalValue()), 2.0));
        IdentifierTableClean();
    }  
    {
        RunLexer("a = 1; b = 1.2; a + b");
        Parser parser; 
        parser.RunParser();       
        ASSERT_TRUE(IsAlmostEqual(GET(parser.EvalValue()), 2.2));
        IdentifierTableClean();
    }    
    {
        RunLexer("a = 1; b = 1.2; c = a + b");
        Parser parser; 
        parser.RunParser();       
        ASSERT_TRUE(IsAlmostEqual(GET(parser.EvalValue()), 2.2));
        IdentifierTableClean();
    }
    {
        RunLexer(R"(
even = 0; 
odd = 0;;
i = 1; 
while i < 10
{
    if i % 2 == 0
    {
        even = even + 1;
    }
    else 
    {
        odd = odd + 1;
    };
    i = i + 1;
};
even + odd;
)");
        Parser parser; 
        parser.RunParser();       
        ASSERT_TRUE(IsAlmostEqual(GET(parser.EvalValue()), 9.0));
        IdentifierTableClean();
    }
}


TEST(AstFunc, EvalFuncStep)
{
    // Primary
    {
        RunLexer(R"TEST(def func(){3};func())TEST");
        Parser parser; 
        parser.RunParser();   
        ASSERT_FLOAT_EQ(GET(parser.EvalValue()), 3);
    }   
    {
        RunLexer(R"TEST(def func(lhs, rhs){lhs+rhs};func(3.2,4.6))TEST");
        Parser parser; 
        parser.RunParser();   
        ASSERT_FLOAT_EQ(GET(parser.EvalValue()), 7.8);
    }
    {
        RunLexer(R"TEST(def func(lhs){if(lhs == 0){0;}else{1 + func(lhs-1);}};
func(1);
)TEST");
        Parser parser; 
        parser.RunParser();   
        ASSERT_FLOAT_EQ(GET(parser.EvalValue()), 1);
    }
    {
        RunLexer(R"TEST(def func(lhs, rhs){if(lhs >= rhs){lhs}else{rhs}};
func(13.5, 2);
)TEST");
        Parser parser; 
        parser.RunParser();   
        ASSERT_FLOAT_EQ(GET(parser.EvalValue()), 13.5);
    }
    // Simple

}

TEST(Env, Namespace)
{
    {
        RunLexer(R"TEST(def fun(lhs) {if (lhs == 0){0}else{if (lhs == 1){1}else{fun(lhs-1)+fun(lhs-2)}}};
fun(10);
)TEST");
        Parser parser; 
        parser.RunParser();   
        ASSERT_FLOAT_EQ(GET(parser.EvalValue()), 55);
    }   
}

TEST(Ast, RuntimeError)
{
    {
        RunLexer("a = b;");
        Parser parser; 
        parser.RunParser();  
        HandleTry
            parser.Eval();     
        HandleCatch
            ASSERT_STREQ(e.what(), R"([Line 0, 4]: Runtime error: "b" undefined.)");
            IdentifierTableClean();
        HandleEnd
    }
    {
        RunLexer("a + 1;");
        Parser parser; 
        parser.RunParser();  
        HandleTry
            parser.Eval();     
        HandleCatch
            ASSERT_STREQ(e.what(), R"([Line 0, 0]: Runtime error: "a" undefined.)");
            IdentifierTableClean();
        HandleEnd
    }
    {
        RunLexer("a + b;");
        Parser parser; 
        parser.RunParser();  
        HandleTry
            parser.Eval();     
        HandleCatch
            ASSERT_STREQ(e.what(), R"([Line 0, 4]: Runtime error: "b" undefined.)");
            IdentifierTableClean();
        HandleEnd
    }
    {
        RunLexer("a;b = a;");
        Parser parser; 
        parser.RunParser();
        HandleTry
            parser.Eval();   
        HandleCatch  
            ASSERT_STREQ(e.what(), R"([Line 0, 0]: Runtime error: "a" undefined.)");
            IdentifierTableClean();
        HandleEnd
    }
    {
        RunLexer("1 = 3");
        Parser parser; 
        parser.RunParser();  
        HandleTry
            parser.Eval();     
        HandleCatch
            ASSERT_STREQ(e.what(), R"([Line 0, 2]: Runtime error: can not assign to r-value.)");
            IdentifierTableClean();
        HandleEnd
    }
    {
        RunLexer("a = 0;1 = a");
        Parser parser; 
        parser.RunParser();  
        HandleTry
            parser.Eval();     
        HandleCatch
            ASSERT_STREQ(e.what(), R"([Line 0, 8]: Runtime error: can not assign to r-value.)");
            IdentifierTableClean();
        HandleEnd
    }
    {
        RunLexer("a = 0;2 + a = a");
        Parser parser; 
        parser.RunParser();  
        HandleTry
            parser.Eval();     
        HandleCatch
            ASSERT_STREQ(e.what(), R"([Line 0, 12]: Runtime error: can not assign to r-value.)");
            IdentifierTableClean();
        HandleEnd
    }
    {
        RunLexer("a = 0;a = 2/a");
        Parser parser; 
        parser.RunParser();  
        HandleTry
            parser.Eval();     
        HandleCatch
            ASSERT_STREQ(e.what(), R"([Line 0, 11]: Runtime error: denominator of div operator can't be zero.)");
            IdentifierTableClean();
        HandleEnd
    }
    {
        RunLexer("a = 1.2; a % 2");
        Parser parser; 
        parser.RunParser();  
        HandleTry
            parser.Eval();     
        HandleCatch
            ASSERT_STREQ(e.what(), R"([Line 0, 11]: Runtime error: parameters of mod operator can't be double.)");
            IdentifierTableClean();
        HandleEnd
    }
}

TEST(Ast, FuncRuntimeError)
{
    {
        RunLexer("def func(lhs){func(lhs)};func(1);");
        Parser parser; 
        parser.RunParser();  
        HandleTry
            parser.Eval();     
        HandleCatch
            ASSERT_STREQ(e.what(), R"([Line 0, 14]: Runtime error: the depth of stack is out of range.)");
            IdentifierTableClean();
            DefTableClean();
        HandleEnd
    }
    {
        RunLexer("def fun(){};fun(1)");
        Parser parser; 
        parser.RunParser();  
        HandleTry
            parser.Eval();     
        HandleCatch
            ASSERT_STREQ(e.what(), R"([Line 0, 12]: Runtime error: the number of args does not match the number of params.)");
            IdentifierTableClean();
            DefTableClean();
        HandleEnd
    }
    {
        RunLexer("fun(1)");
        Parser parser; 
        parser.RunParser();  
        HandleTry
            parser.Eval();     
        HandleCatch
            ASSERT_STREQ(e.what(), R"([Line 0, 0]: Runtime error: undefined function "fun".)");
            IdentifierTableClean();
            DefTableClean();
        HandleEnd
    }
    {
        RunLexer("def func(lhs){lhs};func(p);");
        Parser parser; 
        parser.RunParser();  
        HandleTry
            parser.Eval();     
        HandleCatch
            ASSERT_STREQ(e.what(), R"([Line 0, 24]: Runtime error: "p" undefined.)");
            IdentifierTableClean();
            DefTableClean();
        HandleEnd
    }
   
}
