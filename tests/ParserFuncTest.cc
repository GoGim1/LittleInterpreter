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

TEST(Parser, Build)
{
    {
        RunLexer("3+-id*2; ;3+-id*2;def fun(i, j){}");
        Parser parser;
        parser.RunParser();       
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(parser.Dump(), R"TEST(def fun(i, j){}
3+-id*2;;3+-id*2;)TEST");
    }
}

TEST(Parser, ErrorStep)
{
    // Param
    {
        RunLexer("3");
        Parser parser;
        auto p = parser.ParseParam();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 0]: ParamNode error: param must be IDENTIFIER.
)TEST");
    }  
    // Params
    {
        RunLexer("3, ");
        Parser parser;
        auto p = parser.ParseParams();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 0]: ParamNode error: param must be IDENTIFIER.
[Line 0, 0]: ParamsNode error: parsing ParamNode fail.
)TEST");
    }  
    {
        RunLexer("i,3");
        Parser parser;
        auto p = parser.ParseParams();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 2]: ParamNode error: param must be IDENTIFIER.
[Line 0, 2]: ParamsNode error: parsing ParamNode list fail.
)TEST");
    }  
    // ParamList
    {
        RunLexer("3, ");
        Parser parser;
        auto p = parser.ParseParamList();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 0]: ParamListNode error: param list must start with "(".
)TEST");
    } 
    {
        RunLexer("(i, )");
        Parser parser;
        auto p = parser.ParseParamList();
        EXPECT_EQ(DumpError(), R"TEST([Line 0, 4]: ParamNode error: param must be IDENTIFIER.
[Line 0, 4]: ParamsNode error: parsing ParamNode list fail.
[Line 0, 1]: ParamListNode error: parsing ParamsNode fail.
)TEST");
    } 
    {
        RunLexer("(i");
        Parser parser;
        auto p = parser.ParseParamList();
        EXPECT_EQ(DumpError(), R"TEST([Line 0, 2]: ParamListNode error: param list must end with ")".
)TEST");
    } 
    {
        RunLexer("(");
        Parser parser;
        auto p = parser.ParseParamList();
        EXPECT_EQ(DumpError(), R"TEST([Line 0, 1]: ParamNode error: param must be IDENTIFIER.
[Line 0, 1]: ParamsNode error: parsing ParamNode fail.
[Line 0, 1]: ParamListNode error: parsing ParamsNode fail.
)TEST");
    } 
    // Def
    {
        RunLexer("3, ");
        Parser parser;
        auto p = parser.ParseDef();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 0]: DefNode error: function definition must start with "def".
)TEST");
    } 
    {
        RunLexer("def ");
        Parser parser;
        auto p = parser.ParseDef();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 4]: DefNode error: function name must be IDENTIFIER.
)TEST");
    }
    {
        RunLexer("def 3");
        Parser parser;
        auto p = parser.ParseDef();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 4]: DefNode error: function name must be IDENTIFIER.
)TEST");
    }  
    {
        RunLexer("def fun(3)");
        Parser parser;
        auto p = parser.ParseDef();
        EXPECT_EQ(DumpError(), R"TEST([Line 0, 8]: ParamNode error: param must be IDENTIFIER.
[Line 0, 8]: ParamsNode error: parsing ParamNode fail.
[Line 0, 8]: ParamListNode error: parsing ParamsNode fail.
[Line 0, 7]: DefNode error: parsing function param list fail.
)TEST");
    }
    {
        RunLexer("def fun(){");
        Parser parser;
        auto p = parser.ParseDef();
        EXPECT_EQ(DumpError(), R"TEST([Line 0, 10]: FactorNode error: "EOF" should be "+", "-", indentifier, integer or float.
[Line 0, 10]: ExprNode error: parsing the first Factor error.
[Line 0, 10]: SimpleNode error: parsing Expr error.
[Line 0, 10]: StatementNode error: parsing Simple error.
[Line 0, 10]: BlockNode error: parsing the first Statement error.
[Line 0, 9]: DefNode error: parsing function block fail.
)TEST");
    }
    // Args
    {
        RunLexer("3+");
        Parser parser;
        auto p = parser.ParseArgs();
        EXPECT_EQ(DumpError(), R"TEST([Line 0, 2]: ExprNode error: FactorNode miss after operator.
[Line 0, 0]: ArgsNode error: parsing expr fail.
)TEST");
    }
    // Postfix
    {
        RunLexer("3+");
        Parser parser;
        auto p = parser.ParsePostfix();
        EXPECT_EQ(DumpError(), R"TEST([Line 0, 0]: PostfixNode error: postfix must start with "(".
)TEST");
    }
    {
        RunLexer("(3+3");
        Parser parser;
        auto p = parser.ParsePostfix();
        EXPECT_EQ(DumpError(), R"TEST([Line 0, 4]: PostfixNode error: postfix must start with ")".
)TEST");
    }
    {
        RunLexer("(3+");
        Parser parser;
        auto p = parser.ParsePostfix();
        EXPECT_EQ(DumpError(), R"TEST([Line 0, 3]: ExprNode error: FactorNode miss after operator.
[Line 0, 1]: ArgsNode error: parsing expr fail.
[Line 0, 1]: PostfixNode error: parsing args fail.
)TEST");
    }
    // Primary
    {
        RunLexer("fun(");
        Parser parser;
        auto p = parser.ParsePrimary();
        EXPECT_EQ(DumpError(), R"TEST([Line 0, 4]: FactorNode error: "EOF" should be "+", "-", indentifier, integer or float.
[Line 0, 4]: ExprNode error: parsing the first Factor error.
[Line 0, 4]: ArgsNode error: parsing expr fail.
[Line 0, 4]: PostfixNode error: parsing args fail.
[Line 0, 3]: PrimaryNode error: parsing postfix fail.
)TEST");
    }
    // Simple
    {
        RunLexer("1 1+1, 2+");
        Parser parser;
        auto p = parser.ParseSimple();
        EXPECT_EQ(DumpError(), R"TEST([Line 0, 9]: ExprNode error: FactorNode miss after operator.
[Line 0, 7]: ArgsNode error: parsing expr fail.
[Line 0, 2]: SimpleNode error: parsing Args error.
)TEST");
    }
    // Program
    {
        RunLexer("def fun(3)");
        Parser parser;
        auto p = parser.ParseProgram();
        EXPECT_EQ(DumpError(), R"TEST([Line 0, 8]: ParamNode error: param must be IDENTIFIER.
[Line 0, 8]: ParamsNode error: parsing ParamNode fail.
[Line 0, 8]: ParamListNode error: parsing ParamsNode fail.
[Line 0, 7]: DefNode error: parsing function param list fail.
[Line 0, 0]: ProgramNode error: parsing Def error.
)TEST");
    }
}