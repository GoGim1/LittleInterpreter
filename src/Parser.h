#pragma once

#include <memory>
#include <sstream>  
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
        void            RunParser();
        const string    Dump() const;
        void            Eval() const;
    private:
        ProgramPtr      program;
    };

}