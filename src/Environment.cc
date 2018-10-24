#include "Environment.h"
#include "Ast.h"

namespace Environment
{
    using namespace Ast;

    unordered_map<string, variant<int, double>>     Env;
    unordered_map<string, DefPtr>                   defTable;

    void DefiniteFunc(const DefPtr& pDefRhs)
    {
        string index = pDefRhs->GetFunctionName();
        defTable[index] = pDefRhs;
    }
    
    variant<int, double> EvalDef(const string& funName, const vector<variant<int, double>>& args)
    {
        if (defTable.find(funName) == defTable.end())
            assert(0);
        auto pDef = defTable[funName];
        return pDef->Eval(args);
    }
} // Environment
