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
    ASSERT_EQ(NextToken().getType(), Token::Type::VAR);
    ASSERT_EQ(NextToken().getType(), Token::Type::IDENTIFIER);
    ASSERT_EQ(NextToken().getType(), Token::Type::ASSIGN);
    ASSERT_EQ(NextToken().getType(), Token::Type::INTEGER);
    ASSERT_EQ(NextToken().getType(), Token::Type::SEMICOLON);
    ASSERT_EQ(NextToken().getType(), Token::Type::IF);
    ASSERT_EQ(NextToken().getType(), Token::Type::LBRACKET);
    ASSERT_EQ(NextToken().getType(), Token::Type::IDENTIFIER);
    ASSERT_EQ(NextToken().getType(), Token::Type::EQUAL);
    ASSERT_EQ(NextToken().getType(), Token::Type::FLOAT);
    ASSERT_EQ(NextToken().getType(), Token::Type::RBRACKET);
    ASSERT_EQ(NextToken().getType(), Token::Type::LBRACE);
    ASSERT_EQ(NextToken().getType(), Token::Type::IDENTIFIER);
    ASSERT_EQ(NextToken().getType(), Token::Type::ASSIGN);
    ASSERT_EQ(NextToken().getType(), Token::Type::INTEGER);
    ASSERT_EQ(NextToken().getType(), Token::Type::SEMICOLON);
    ASSERT_EQ(NextToken().getType(), Token::Type::RBRACE);
    ASSERT_EQ(NextToken().getType(), Token::Type::_EOF);
}