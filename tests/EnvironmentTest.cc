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
    {
        RunLexer("a = 1");
        Parser parser; 
        parser.RunParser();       
        ASSERT_TRUE(IsAlmostEqual(GET(parser.EvalValue()), 1.0));
        Env.clear();
    }
    {
        RunLexer("a = 1; a");
        Parser parser; 
        parser.RunParser();       
        ASSERT_TRUE(IsAlmostEqual(GET(parser.EvalValue()), 1.0));
        Env.clear();
    }
    {
        RunLexer("a = 1; a = a + 1; ");
        Parser parser; 
        parser.RunParser();       
        ASSERT_TRUE(IsAlmostEqual(GET(parser.EvalValue()), 2.0));
        Env.clear();
    }  
    {
        RunLexer("a = 1; b = 1.2; a + b");
        Parser parser; 
        parser.RunParser();       
        ASSERT_TRUE(IsAlmostEqual(GET(parser.EvalValue()), 2.2));
        Env.clear();
    }    
    {
        RunLexer("a = 1; b = 1.2; c = a + b");
        Parser parser; 
        parser.RunParser();       
        ASSERT_TRUE(IsAlmostEqual(GET(parser.EvalValue()), 2.2));
        Env.clear();
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
        Env.clear();
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
        Env.clear();
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
            Env.clear();
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
            Env.clear();
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
            Env.clear();
        HandleEnd
    }
    {
        RunLexer("a;b = a;");
        Parser parser; 
        parser.RunParser();
        ASSERT_TRUE(IsAlmostEqual(GET(parser.EvalValue()), 0.0));
        Env.clear();
    }
    {
        RunLexer("1 = 3");
        Parser parser; 
        parser.RunParser();  
        HandleTry
            parser.Eval();     
        HandleCatch
            ASSERT_STREQ(e.what(), R"([Line 0, 2]: Runtime error: can not assign to r-value.)");
            Env.clear();
        HandleEnd
    }
    {
        RunLexer("a;1 = a");
        Parser parser; 
        parser.RunParser();  
        HandleTry
            parser.Eval();     
        HandleCatch
            ASSERT_STREQ(e.what(), R"([Line 0, 4]: Runtime error: can not assign to r-value.)");
            Env.clear();
        HandleEnd
    }
    {
        RunLexer("a;2 + a = a");
        Parser parser; 
        parser.RunParser();  
        HandleTry
            parser.Eval();     
        HandleCatch
            ASSERT_STREQ(e.what(), R"([Line 0, 8]: Runtime error: can not assign to r-value.)");
            Env.clear();
        HandleEnd
    }
    {
        RunLexer("a;a = 2/a");
        Parser parser; 
        parser.RunParser();  
        HandleTry
            parser.Eval();     
        HandleCatch
            ASSERT_STREQ(e.what(), R"([Line 0, 7]: Runtime error: denominator of div operator can't be zero.)");
            Env.clear();
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
            Env.clear();
        HandleEnd
    }

}

