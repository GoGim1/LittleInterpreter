#pragma once
#include <vector>
#include "Error.h"
#include "Ast.h"

namespace Parse
{
    using namespace Ast;
    using namespace Util;

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
        
        void            HandleError();
        void            AddError(const Error&);
        void            RunParser();

        // (DEBUG)
        void            DumpErrorList();
        string          Dump();
    private:
        ProgramPtr      program;
        vector<Error>   errorList;
    };

}