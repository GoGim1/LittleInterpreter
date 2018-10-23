#pragma once 

#include <memory>
#define MakeProgramPtr      std::make_shared<ProgramNode>
#define MakeStatementPtr    std::make_shared<StatementNode>
#define MakeSimplePtr       std::make_shared<SimpleNode>
#define MakeBlockPtr        std::make_shared<BlockNode>
#define MakeExprPtr         std::make_shared<ExprNode>
#define MakeFactorPtr       std::make_shared<FactorNode>
#define MakePrimaryPtr      std::make_shared<PrimaryNode>
#define MakeTokenPtr        std::make_shared<Token>

#define MakeParamPtr        std::make_shared<ParamNode>
#define MakeParamsPtr       std::make_shared<ParamsNode>
#define MakeParamListPtr    std::make_shared<ParamListNode>
#define MakeDefPtr          std::make_shared<DefNode>
#define MakeArgsPtr         std::make_shared<ArgsNode>
#define MakePostfixPtr      std::make_shared<PostfixNode>

// VARIANT run two times.
// #define GET(VARIANT) 
// (std::holds_alternative<int>(VARIANT) ? std::get<int>(VARIANT) : std::get<double>(VARIANT))

// WARNING: it always return double.
#define GET(VARIANT)                                                                    \
({                                                                                      \
    auto _tmp = (VARIANT);                                                              \
    (std::holds_alternative<int>(_tmp) ? std::get<int>(_tmp) : std::get<double>(_tmp)); \
})



#ifdef DEBUG
#include <iostream>
#include <cassert>
using std::cout;
using std::endl;
#define Print(str)          cout << (str) << endl
#define Assert(a, b)        assert((a) && (b))

#define HandleTry           \
try                         \
{                               
#define HandleCatch         \
}                           \
catch (Error& e)            \
{
#define HandleEnd           \
}
#else 
#define Print(str)  
#define Assert(a, b)        
#endif