#include <gtest/gtest.h>
#include "Parser.h"
using namespace Parse;

TEST(Parser, NewBuildStep) 
{
    // ParamNode
    {
        RunLexer("i");
        Parser parser;
        auto p = parser.ParseParam();
        ASSERT_EQ(p->Dump(), "i");
    }
    // ParamsNode
    {
        RunLexer("i");
        Parser parser;
        auto p = parser.ParseParams();
        ASSERT_EQ(p->Dump(), "i");
    }
    {
        RunLexer("i,i");
        Parser parser;
        auto p = parser.ParseParams();
        ASSERT_EQ(p->Dump(), "i, i");
    }
     // ParamListNode
    {
        RunLexer("()");
        Parser parser;
        auto p = parser.ParseParamList();
        ASSERT_EQ(p->Dump(), "()");
    }
    {
        RunLexer("(i)");
        Parser parser;
        auto p = parser.ParseParamList();
        ASSERT_EQ(p->Dump(), "(i)");
    }
    {
        RunLexer("(i,i)");
        Parser parser;
        auto p = parser.ParseParamList();
        ASSERT_EQ(p->Dump(), "(i, i)");
    }
    // DefNode
    {
        RunLexer("def fun(){}");
        Parser parser;
        auto p = parser.ParseDef();
        ASSERT_EQ(p->Dump(), "def fun(){}");
    }
    {
        RunLexer("def fun(i, j){ a = a + 1; }");
        Parser parser;
        auto p = parser.ParseDef();
        ASSERT_EQ(p->Dump(), "def fun(i, j){a=a+1;}");
    }
    // ArgsNode
    {
        RunLexer("1");
        Parser parser;
        auto p = parser.ParseArgs();
        ASSERT_EQ(p->Dump(), "1");
    }
    {
        RunLexer("(1), 1+2");
        Parser parser;
        auto p = parser.ParseArgs();
        ASSERT_EQ(p->Dump(), "(1), 1+2");
    }
    // PostfixNode
    {
        RunLexer("((1))");
        Parser parser;
        auto p = parser.ParsePostfix();
        ASSERT_EQ(p->Dump(), "((1))");
    }    
    {
        RunLexer("(3*4, 5)");
        Parser parser;
        auto p = parser.ParsePostfix();
        ASSERT_EQ(p->Dump(), "(3*4, 5)");
    }   
}

TEST(Parser, OldBuildStep)
{
    // PrimaryNode
    {
        RunLexer("fun(1)");
        Parser parser;
        auto p = parser.ParsePrimary();
        ASSERT_EQ(p->Dump(), "fun(1)");
    }
    {
        RunLexer("(2+3)(1+1, (2))");
        Parser parser;
        auto p = parser.ParsePrimary();
        ASSERT_EQ(p->Dump(), "(2+3)(1+1, (2))");
    }
    // SimpleNode
    {
        RunLexer("1");
        Parser parser;
        auto p = parser.ParseSimple();
        ASSERT_EQ(p->Dump(), "1");
    }
    {
        RunLexer("1 1");
        Parser parser;
        auto p = parser.ParseSimple();
        ASSERT_EQ(p->Dump(), "1 1");
    }
    {
        RunLexer("1 1+1, 2+2");
        Parser parser;
        auto p = parser.ParseSimple();
        ASSERT_EQ(p->Dump(), "1 1+1, 2+2");
    }
    // ProgramNode
    {
        RunLexer("3+-id;def fun(i, j){ a = a + 1; };if 3+(-id){3+-id;3+-id;}else {3+-id;(3+-id);};while 3+-id{(3+-id);3+-id;};def fun(){}");
        Parser parser;
        auto p = parser.ParseProgram();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), R"TEST(def fun(i, j){a=a+1;}
def fun(){}
3+-id;if 3+(-id){3+-id;3+-id;}else{3+-id;(3+-id);};while 3+-id{(3+-id);3+-id;};)TEST");
    }     
}