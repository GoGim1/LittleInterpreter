#include <gtest/gtest.h>
#include "Parser.h"
#include "Lexer.h"

#include "Helper.h"

using namespace Lexer;
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

TEST(Parser, Error) 
{   
    {
        RunLexer("(3");
        Parser parser;
        auto p = parser.ParsePrimary();
        parser.DumpErrorList();
    }   
    {
        RunLexer("/3");
        Parser parser;
        auto p = parser.ParsePrimary();
        parser.DumpErrorList();
    }    
    {
        RunLexer("-*5");
        Parser parser;
        auto p = parser.ParseFactor();
        parser.DumpErrorList();
    }    
    {
        RunLexer("(3");
        Parser parser;
        auto p = parser.ParseFactor();
        parser.DumpErrorList();
    }  
    {
        RunLexer("+");
        Parser parser;
        auto p = parser.ParseFactor();
        parser.DumpErrorList();
    }   
    {
        RunLexer(" (3 + 2  }");
        Parser parser;
        auto p = parser.ParseFactor();
        parser.DumpErrorList();
    } 
 
    {
        RunLexer("3++");
        Parser parser;
        auto p = parser.ParseExpr();
        parser.DumpErrorList();
    }   
    {
        RunLexer("3+{");
        Parser parser;
        auto p = parser.ParseExpr();
        parser.DumpErrorList();
    }  
    {
        RunLexer("==3+");
        Parser parser;
        auto p = parser.ParseExpr();
        parser.DumpErrorList();
    }   
    {
        RunLexer("58");
        Parser parser;
        auto p = parser.ParseBlock();
        parser.DumpErrorList();
    }   

    {
        RunLexer("{");
        Parser parser;
        auto p = parser.ParseBlock();
        parser.DumpErrorList();
    }   
    {
        RunLexer("{33+");
        Parser parser;
        auto p = parser.ParseBlock();
        parser.DumpErrorList();
    }   
    {
        RunLexer("{33+6");
        Parser parser;
        auto p = parser.ParseBlock();
        parser.DumpErrorList();
    }   
    {
        RunLexer("{33+6;;*35");
        Parser parser;
        auto p = parser.ParseBlock();
        parser.DumpErrorList();
    }   
 
    {
        RunLexer("if ");
        Parser parser;
        auto p = parser.ParseStatement();
        parser.DumpErrorList();
    }  
    {
        RunLexer("if (+2)else");
        Parser parser;
        auto p = parser.ParseStatement();
        parser.DumpErrorList();
    }  
    {
        RunLexer("if (30**2)else");
        Parser parser;
        auto p = parser.ParseStatement();
        parser.DumpErrorList();
    }  
    {
        RunLexer("if (30*2)else {");
        Parser parser;
        auto p = parser.ParseStatement();
        parser.DumpErrorList();
    }  
    {
        RunLexer("if (30*2){30*2)");
        Parser parser;
        auto p = parser.ParseStatement();
        parser.DumpErrorList();
    }  
    {
        RunLexer("if (30*2){30*2; ;}else)");
        Parser parser;
        auto p = parser.ParseStatement();
        parser.DumpErrorList();
    }  
    {
        RunLexer("while(30*2){30*2; ;)");
        Parser parser;
        auto p = parser.ParseStatement();
        parser.DumpErrorList();
    }    
    {
        RunLexer("while(30*2{30*2; ;)");
        Parser parser;
        auto p = parser.ParseStatement();
        parser.DumpErrorList();
    }  
    {
        RunLexer("while");
        Parser parser;
        auto p = parser.ParseStatement();
        parser.DumpErrorList();
    }  
    {
        RunLexer("while(30*2))");
        Parser parser;
        auto p = parser.ParseStatement();
        parser.DumpErrorList();
    }   

    {
        RunLexer("while(30*2))");
        Parser parser;
        auto p = parser.ParseProgram();
        parser.DumpErrorList();
    }   
    {
        RunLexer("while(30*2){}if(){})");
        Parser parser;
        auto p = parser.ParseProgram();
        parser.DumpErrorList();
    }   

    // ExceptionBegin
    //     RunLexer("(3 + 2  }");
    //     Parser parser;
    //     parser.RunParser();  
    //     parser.DumpErrorList();
    // ExceptionEnd  
    // {
    //     RunLexer("/3");
    //     Parser parser;
    //     parser.RunParser();  
    //     parser.DumpErrorList();
    // }   

}





