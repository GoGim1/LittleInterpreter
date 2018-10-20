#include <gtest/gtest.h>
#include "Parser.h"
#include "../src/tools/FloatCompare.h"

using namespace Parse;

TEST(Parser, BuildStep) 
{
    //PrimaryNode
    {
        RunLexer("3");
        Parser parser;
        auto p = parser.ParsePrimary();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);        
        ASSERT_EQ(p->Dump(), "3");
    }
    {
        RunLexer("id");
        Parser parser;
        auto p = parser.ParsePrimary();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "id");
    }
    {
        RunLexer("(3+-id*2)");
        Parser parser;
        auto p = parser.ParsePrimary();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "(3+-id*2)");
    }

    // FactorNode
    {
        RunLexer("3");
        Parser parser;
        auto p = parser.ParseFactor();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "3");
    }
    {
        RunLexer("-id");
        Parser parser;
        auto p = parser.ParseFactor();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "-id");
    }

   // ExprNode
    {
        RunLexer("(3+-id)*2");
        Parser parser;
        auto p = parser.ParseExpr();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "(3+-id)*2");
    }
    {
        RunLexer("3+-id*2");
        Parser parser;
        auto p = parser.ParseExpr();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "3+-id*2");
    }
    {
        RunLexer("3+(-id)*2");
        Parser parser;
        auto p = parser.ParseExpr();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "3+(-id)*2");
    }
    // SimpleNode
    {
        RunLexer("3+-id*2");
        Parser parser;
        auto p = parser.ParseSimple();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "3+-id*2");
    }

    // BlockNode
    // {}
    {
        RunLexer("{}");
        Parser parser;
        auto p = parser.ParseBlock();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "{}");
    }
    // {[statement]{(;|EOF)[statement]}}
    {
        RunLexer("{3+-id*2;3+-id*2;3+-id*2}");
        Parser parser;
        auto p = parser.ParseBlock();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "{3+-id*2;3+-id*2;3+-id*2}");
    }
    {
        RunLexer("{3+-id*2;;3+-id*2}");
        Parser parser;
        auto p = parser.ParseBlock();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "{3+-id*2;;3+-id*2}");
    }
    {
        RunLexer("{3+-id*2;;3+-id*2;}");
        Parser parser;
        auto p = parser.ParseBlock();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "{3+-id*2;;3+-id*2;}");
    }
    // {[statement]}
    {
        RunLexer("{3+-id*2}");
        Parser parser;
        auto p = parser.ParseBlock();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "{3+-id*2}");
    }
    // {[statement]{(;|EOF)}}
    {
        RunLexer("{3+-id*2;}");
        Parser parser;
        auto p = parser.ParseBlock();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "{3+-id*2;}");
    }
    {
        RunLexer("{3+-id*2;;}");
        Parser parser;
        auto p = parser.ParseBlock();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "{3+-id*2;;}");
    }
    // {{(;|EOF)[statement]}}  
    {
        RunLexer("{;3+-id*2;3+-id*2}");
        Parser parser;
        auto p = parser.ParseBlock();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "{;3+-id*2;3+-id*2}");
    }
    // {{(;|EOF)}}
    {
        RunLexer("{;;;}");
        Parser parser;
        auto p = parser.ParseBlock();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "{;;;}");
    }
    {
        RunLexer("{;3+-id*2;3+-id*2;;}");
        Parser parser;
        auto p = parser.ParseBlock();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "{;3+-id*2;3+-id*2;;}");
    }

    // StatementNode
    {
        RunLexer("3+-id*2");
        Parser parser;
        auto p = parser.ParseStatement();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "3+-id*2");
    }    
    {
        RunLexer("if 3+-id*2 {3+-id*2}");
        Parser parser;
        auto p = parser.ParseStatement();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "if 3+-id*2{3+-id*2}");
    }   
    {
        RunLexer("if 3+-id*2 {3+-id*2}else{;3+-id*2;3+-id*2;;}");
        Parser parser;
        auto p = parser.ParseStatement();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "if 3+-id*2{3+-id*2}else{;3+-id*2;3+-id*2;;}");
    }     
    {
        RunLexer("while 3+-id*2 {3+-id*2;;}");
        Parser parser;
        auto p = parser.ParseStatement();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "while 3+-id*2{3+-id*2;;}");
    } 
    
    // ProgramNode    
    {
        RunLexer("3+-id;if -id{3+-id;3+-id;};if 3+(-id){3+-id;3+-id;}else {3+-id;(3+-id);};while 3+-id{(3+-id);3+-id;}");
        Parser parser;
        auto p = parser.ParseProgram();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(p->Dump(), "3+-id;if -id{3+-id;3+-id;};if 3+(-id){3+-id;3+-id;}else{3+-id;(3+-id);};while 3+-id{(3+-id);3+-id;};");
    }     
}

TEST(Parser, Build) 
{
    {
        RunLexer("3;");
        Parser parser;
        parser.RunParser();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(parser.Dump(), "3;");
    }
    {
        RunLexer("(3+-id*2)");
        Parser parser;
        parser.RunParser();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(parser.Dump(), "(3+-id*2);");
    }
    {
        RunLexer("-id");
        Parser parser;
        parser.RunParser();
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(parser.Dump(), "-id;");
    }
    {
        RunLexer("3+(-id*2)");
        Parser parser;
        parser.RunParser();       
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(parser.Dump(), "3+(-id*2);");
    }
    {
        RunLexer("3+-id*2; ;3+-id*2");
        Parser parser;
        parser.RunParser();       
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(parser.Dump(), "3+-id*2;;3+-id*2;");
    }
    {
        RunLexer("if 3+-id*2{3+-id*2; ;3+-id*2;}");
        Parser parser; 
        parser.RunParser();       
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(parser.Dump(), "if 3+-id*2{3+-id*2;;3+-id*2;};");
    }
    {
        RunLexer("if 3+-id*2{3+-id*2; ;3+-id*2}else{; ;3+-id*2}");
        Parser parser; 
        parser.RunParser();       
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(parser.Dump(), "if 3+-id*2{3+-id*2;;3+-id*2}else{;;3+-id*2};");
    }
    {
        RunLexer("if 3+-id*2{}else{;}");
        Parser parser; 
        parser.RunParser();       
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(parser.Dump(), "if 3+-id*2{}else{;};");
    }
    {
        RunLexer("while 3+-id*2 {3+-id*2;;}");
        Parser parser;
        parser.RunParser();       
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(parser.Dump(), "while 3+-id*2{3+-id*2;;};");
    } 
    {
        RunLexer("3+-id;if -2{3+-id;3+-id;};if 3+-id{3+-id;3+-id;}else {3+-id;3+-id;};while 3+-id{3+-id;3+-id;}");
        Parser parser;
        parser.RunParser();  
        ASSERT_EQ(NextToken().getType(), Token::_EOF);
        ASSERT_EQ(parser.Dump(), "3+-id;if -2{3+-id;3+-id;};if 3+-id{3+-id;3+-id;}else{3+-id;3+-id;};while 3+-id{3+-id;3+-id;};");
    }     
}

TEST(Parser, ErrorStep) 
 {   
    {
        RunLexer("(3");
        Parser parser;
        auto p = parser.ParsePrimary();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 2]: PrimaryNode error: missing ")".
)TEST");
    }  
 
    {
        RunLexer("/3");
        Parser parser;
        auto p = parser.ParsePrimary();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 0]: PrimaryNode error: "/" can not be a part of PrimaryNode.
)TEST");
    }    
    {
        RunLexer("-*5");
        Parser parser;
        auto p = parser.ParseFactor();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 1]: PrimaryNode error: "*" can not be a part of PrimaryNode.
[Line 0, 1]: FactorNode error: missing PrimaryNode.
)TEST");
    }    
    {
        RunLexer("(3");
        Parser parser;
        auto p = parser.ParseFactor();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 2]: PrimaryNode error: missing ")".
[Line 0, 0]: FactorNode error: parsing Primary error.
)TEST");
    }  
    {
        RunLexer("+");
        Parser parser;
        auto p = parser.ParseFactor();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 1]: PrimaryNode error: "EOF" can not be a part of PrimaryNode.
[Line 0, 1]: FactorNode error: missing PrimaryNode.
)TEST");
    }   
    {
        RunLexer(" (3 + 2  }");
        Parser parser;
        auto p = parser.ParseFactor();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 9]: PrimaryNode error: missing ")".
[Line 0, 1]: FactorNode error: parsing Primary error.
)TEST");
    } 
 
    {
        RunLexer("3++");
        Parser parser;
        auto p = parser.ParseExpr();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 3]: PrimaryNode error: "EOF" can not be a part of PrimaryNode.
[Line 0, 3]: FactorNode error: missing PrimaryNode.
[Line 0, 2]: ExprNode error: parsing Factor list error.
)TEST");
    }   
    {
        RunLexer("3+{");
        Parser parser;
        auto p = parser.ParseExpr();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 2]: ExprNode error: FactorNode miss after operator.
)TEST");
    }  
    {
        RunLexer("==3+");
        Parser parser;
        auto p = parser.ParseExpr();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 0]: FactorNode error: "==" should be "+", "-", indentifier, integer or float.
[Line 0, 0]: ExprNode error: parsing the first Factor error.
)TEST");
    }   
    {
        RunLexer("58");
        Parser parser;
        auto p = parser.ParseBlock();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 0]: BlockNode error: BlockNode must start with "{".
)TEST");
    }   

    {
        RunLexer("{");
        Parser parser;
        auto p = parser.ParseBlock();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 1]: FactorNode error: "EOF" should be "+", "-", indentifier, integer or float.
[Line 0, 1]: ExprNode error: parsing the first Factor error.
[Line 0, 1]: SimpleNode error: parsing Expr error.
[Line 0, 1]: StatementNode error: parsing Simple error.
[Line 0, 1]: BlockNode error: parsing the first Statement error.
)TEST");
    }   
    {
        RunLexer("{33+");
        Parser parser;
        auto p = parser.ParseBlock();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 4]: ExprNode error: FactorNode miss after operator.
[Line 0, 1]: SimpleNode error: parsing Expr error.
[Line 0, 1]: StatementNode error: parsing Simple error.
[Line 0, 1]: BlockNode error: parsing the first Statement error.
)TEST");
    }   
    {
        RunLexer("{33+6");
        Parser parser;
        auto p = parser.ParseBlock();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 5]: BlockNode error: BlockNode must end with "}".
)TEST");
    }   
    {
        RunLexer("{33+6;;*35");
        Parser parser;
        auto p = parser.ParseBlock();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 7]: FactorNode error: "*" should be "+", "-", indentifier, integer or float.
[Line 0, 7]: ExprNode error: parsing the first Factor error.
[Line 0, 7]: SimpleNode error: parsing Expr error.
[Line 0, 7]: StatementNode error: parsing Simple error.
[Line 0, 7]: BlockNode error: parsing statement list error.
)TEST");
    }   
 
    {
        RunLexer("if ");
        Parser parser;
        auto p = parser.ParseStatement();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 3]: FactorNode error: "EOF" should be "+", "-", indentifier, integer or float.
[Line 0, 3]: ExprNode error: parsing the first Factor error.
[Line 0, 3]: StatementNode error: parsing Expr error when parsing IF Statement.
)TEST");
    }  
    {
        RunLexer("if (+2)else");
        Parser parser;
        auto p = parser.ParseStatement();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 7]: BlockNode error: BlockNode must start with "{".
[Line 0, 7]: StatementNode error: parsing Block error when parsing IF Statement.
)TEST");
    }  
    {
        RunLexer("if (30**2)else");
        Parser parser;
        auto p = parser.ParseStatement();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 7]: ExprNode error: FactorNode miss after operator.
[Line 0, 4]: PrimaryNode error: parsing Expr error.
[Line 0, 3]: FactorNode error: parsing Primary error.
[Line 0, 3]: ExprNode error: parsing the first Factor error.
[Line 0, 3]: StatementNode error: parsing Expr error when parsing IF Statement.
)TEST");
    }  
    {
        RunLexer("if (30*2)else {");
        Parser parser;
        auto p = parser.ParseStatement();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 9]: BlockNode error: BlockNode must start with "{".
[Line 0, 9]: StatementNode error: parsing Block error when parsing IF Statement.
)TEST");
    }  
    {
        RunLexer("if (30*2){30*2)");
        Parser parser;
        auto p = parser.ParseStatement();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 14]: BlockNode error: BlockNode must end with "}".
[Line 0, 9]: StatementNode error: parsing Block error when parsing IF Statement.
)TEST");
    }  
    {
        RunLexer("if (30*2){30*2; ;}else)");
        Parser parser;
        auto p = parser.ParseStatement();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 22]: BlockNode error: BlockNode must start with "{".
[Line 0, 22]: StatementNode error: parsing Block error when parsing IFELSE Statement.
)TEST");
    }  
    {
        RunLexer("while(30*2){30*2; ;)");
        Parser parser;
        auto p = parser.ParseStatement();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 19]: FactorNode error: ")" should be "+", "-", indentifier, integer or float.
[Line 0, 19]: ExprNode error: parsing the first Factor error.
[Line 0, 19]: SimpleNode error: parsing Expr error.
[Line 0, 19]: StatementNode error: parsing Simple error.
[Line 0, 19]: BlockNode error: parsing statement list error.
[Line 0, 11]: StatementNode error: parsing Block error when parsing WHILE Statement.
)TEST");
    }    
    {
        RunLexer("while(30*2{30*2; ;)");
        Parser parser;
        auto p = parser.ParseStatement();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 10]: PrimaryNode error: missing ")".
[Line 0, 5]: FactorNode error: parsing Primary error.
[Line 0, 5]: ExprNode error: parsing the first Factor error.
[Line 0, 5]: StatementNode error: parsing Expr error when parsing WHILE Statement.
)TEST");
    }  
    {
        RunLexer("while");
        Parser parser;
        auto p = parser.ParseStatement();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 5]: FactorNode error: "EOF" should be "+", "-", indentifier, integer or float.
[Line 0, 5]: ExprNode error: parsing the first Factor error.
[Line 0, 5]: StatementNode error: parsing Expr error when parsing WHILE Statement.
)TEST");
    }  
    {
        RunLexer("while(30*2))");
        Parser parser;
        auto p = parser.ParseStatement();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 11]: BlockNode error: BlockNode must start with "{".
[Line 0, 11]: StatementNode error: parsing Block error when parsing WHILE Statement.
)TEST");
    }   

    {
        RunLexer("while(30*2))");
        Parser parser;
        auto p = parser.ParseProgram();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 11]: BlockNode error: BlockNode must start with "{".
[Line 0, 11]: StatementNode error: parsing Block error when parsing WHILE Statement.
[Line 0, 0]: ProgramNode error: parsing Statement list error.
)TEST");
    }   
    {
        RunLexer("while(30*2){}if(){})");
        Parser parser;
        auto p = parser.ParseProgram();
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 13]: ProgramNode error: Statement list should be separated by ";".
)TEST");
    }   
}



TEST(Parser, Error) 
{
    {    
        Parser parser;
        HandleTry
            RunLexer("(3 + 2  }");
            parser.RunParser();  
        HandleCatch
            ASSERT_EQ(DumpError(), R"TEST([Line 0, 8]: PrimaryNode error: missing ")".
[Line 0, 0]: FactorNode error: parsing Primary error.
[Line 0, 0]: ExprNode error: parsing the first Factor error.
[Line 0, 0]: SimpleNode error: parsing Expr error.
[Line 0, 0]: StatementNode error: parsing Simple error.
[Line 0, 0]: ProgramNode error: parsing Statement list error.
)TEST");
        HandleEnd  
    }
    {    
        Parser parser;
        HandleTry
            RunLexer("/3");
            parser.RunParser();  
        HandleCatch
            ASSERT_EQ(DumpError(), R"TEST([Line 0, 0]: FactorNode error: "/" should be "+", "-", indentifier, integer or float.
[Line 0, 0]: ExprNode error: parsing the first Factor error.
[Line 0, 0]: SimpleNode error: parsing Expr error.
[Line 0, 0]: StatementNode error: parsing Simple error.
[Line 0, 0]: ProgramNode error: parsing Statement list error.
)TEST");
        HandleEnd  
    }

    {    
        Parser parser;
        HandleTry
            RunLexer("+");
            parser.RunParser();  
        HandleCatch
            ASSERT_EQ(DumpError(), R"TEST([Line 0, 1]: PrimaryNode error: "EOF" can not be a part of PrimaryNode.
[Line 0, 1]: FactorNode error: missing PrimaryNode.
[Line 0, 0]: ExprNode error: parsing the first Factor error.
[Line 0, 0]: SimpleNode error: parsing Expr error.
[Line 0, 0]: StatementNode error: parsing Simple error.
[Line 0, 0]: ProgramNode error: parsing Statement list error.
)TEST");
        HandleEnd  
    }


    {    
        Parser parser;
        HandleTry
            RunLexer("{ 1 + 2; }");
            parser.RunParser();  
        HandleCatch
            //parser.PrintError();
            ASSERT_EQ(DumpError(), R"TEST([Line 0, 0]: FactorNode error: "{" should be "+", "-", indentifier, integer or float.
[Line 0, 0]: ExprNode error: parsing the first Factor error.
[Line 0, 0]: SimpleNode error: parsing Expr error.
[Line 0, 0]: StatementNode error: parsing Simple error.
[Line 0, 0]: ProgramNode error: parsing Statement list error.
)TEST");
        HandleEnd  
    }
    {    
        Parser parser;
        HandleTry
            RunLexer("while(30*2){}if(){})");
            parser.RunParser();  
        HandleCatch
            ASSERT_EQ(DumpError(), R"TEST([Line 0, 13]: ProgramNode error: Statement list should be separated by ";".
)TEST");
        HandleEnd  
    }
}

TEST(Parser, Eval)
{
    Env["id"] = 3.2;
    {
        RunLexer("3;");
        Parser parser;
        parser.RunParser();
        ASSERT_TRUE(GET(parser.EvalValue()) == 3);
    }
    {
        RunLexer("(3+-id*2)");
        Parser parser;
        parser.RunParser();
        ASSERT_TRUE(IsAlmostEqual(GET(parser.EvalValue()), -3.4));
    }
    {
        RunLexer("-id");
        Parser parser;
        parser.RunParser();
        ASSERT_TRUE(IsAlmostEqual(GET(parser.EvalValue()), -3.2));
    }
    {
        RunLexer("3+(-id*2)");
        Parser parser;
        parser.RunParser();       
        ASSERT_TRUE(IsAlmostEqual(GET(parser.EvalValue()), -3.4));
    }
    {
        RunLexer("3+-id*2; ;3+-id*2");
        Parser parser;
        parser.RunParser();       
        ASSERT_TRUE(IsAlmostEqual(GET(parser.EvalValue()), -3.4));
    }
    {
        RunLexer("if 3+-id*2{3+-id*2; ;3+-id*2;}");
        Parser parser; 
        parser.RunParser();       
        ASSERT_TRUE(IsAlmostEqual(GET(parser.EvalValue()), -3.4));
    }
    {
        RunLexer("if 3+-id*2{3+-id*2; ;3+-id*2}else{; ;3+-id*2}");
        Parser parser; 
        parser.RunParser();       
        ASSERT_TRUE(IsAlmostEqual(GET(parser.EvalValue()), -3.4));
    }
    {
        RunLexer("if 3+-id*2{}else{;}");
        Parser parser; 
        parser.RunParser();       
        ASSERT_TRUE(GET(parser.EvalValue()) == 0);
    }
    // {
    //     RunLexer("while 3+-id*2 {3+-id*2;;}");
    //     Parser parser;
    //     parser.RunParser();       
    //     ASSERT_TRUE(IsAlmostEqual(GET(parser.EvalValue()),  ));
    // } 
    // {
    //     RunLexer("3+-id;if -2{3+-id;3+-id;};if 3+-id{3+-id;3+-id;}else {3+-id;3+-id;};while 3+-id{3+-id;3+-id;}");
    //     Parser parser;
    //     parser.RunParser();  
    //     ASSERT_TRUE(IsAlmostEqual(GET(parser.EvalValue()),  ));
    // }     
}

TEST(Parser, File)
{
    {
        RunLexerFromFile("../tests/testcases/ParserCase.txt");
        Parser parser;
        parser.RunParser();
     //   cout << parser.Dump();
        
    }
}


