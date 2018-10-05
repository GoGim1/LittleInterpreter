#include <gtest/gtest.h>
#include "Lexer.h"

using namespace Lexer;

// var i = 2;
// if (i == 2.0)
// {
//   i = 3;
// }
TEST(Lexer, Basic) 
{
    RunLexer("../tests/data/test.txt");
    ASSERT_EQ(NextToken().type, Token::Type::VAR);
    ASSERT_EQ(NextToken().type, Token::Type::IDENTIFIER);
    ASSERT_EQ(NextToken().type, Token::Type::ASSIGN);
    ASSERT_EQ(NextToken().type, Token::Type::INTEGER);
    ASSERT_EQ(NextToken().type, Token::Type::OPERATOR);
    ASSERT_EQ(NextToken().type, Token::Type::IF);
    ASSERT_EQ(NextToken().type, Token::Type::OPERATOR);
    ASSERT_EQ(NextToken().type, Token::Type::IDENTIFIER);
    ASSERT_EQ(NextToken().type, Token::Type::EQUAL);
    ASSERT_EQ(NextToken().type, Token::Type::FLOAT);
    
    
    
}