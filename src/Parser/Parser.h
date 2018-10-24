#pragma once

#include <memory>
#include <sstream>  
#include <iostream>
#include "Error.h"
#include "Ast.h"
#include "Lexer.h"
#include "Helper.h"

namespace Parse
{
    using namespace Ast;
    using namespace Lexer;
    using namespace Util;

    using std::stringstream;
    using std::cout;
    using std::endl;

    class Parser
    {
    public:
        PrimaryPtr      ParsePrimary();
        FactorPtr       ParseFactor();
        ExprPtr         ParseExpr();
        BlockPtr        ParseBlock();
        SimplePtr       ParseSimple();
        StatementPtr    ParseStatement();
        ProgramPtr      ParseProgram();
         
        ParamPtr        ParseParam();
        ParamsPtr       ParseParams();
        ParamListPtr    ParseParamList();
        DefPtr          ParseDef();
        ArgsPtr         ParseArgs();
        PostfixPtr      ParsePostfix();

        void            RunParser();
        const string    Dump() const;
        void            Eval() const;

#ifdef DEBUG
        variant<int, double> EvalValue() 
        {
            return program->Eval();
        }
#endif
    private:
        ProgramPtr      program;
    };

}