#include "Environment.h"
#include "Ast.h"

namespace Environment
{
    using namespace Ast;

    using idTlbType = unordered_map<string, variant<int, double>>;
    using defTlbType = unordered_map<string, DefPtr>;
    
    vector<idTlbType>       identifierTable;
    defTlbType              defTable;

    static int depth = 0;

    bool IsIdentifierDefined(const string& name)
    {
        for (int i = identifierTable.size()-1; i>=0; i--)
        {
            if (identifierTable[i].find(name) != identifierTable[i].end())
                return true;
        }
        return false;
    }
    const variant<int, double>& GetIdentifierVal(const string& name)
    {
        for (int i = identifierTable.size()-1; i>=0; i--)
        {
            if (identifierTable[i].find(name) != identifierTable[i].end())
                return identifierTable[i][name];
        }
        assert(0);
        return 0;
    }

    void DefineIdentifier(const string& name, const variant<int, double>& val)
    {
        if (identifierTable.empty())
            NewNamespace();
        identifierTable[identifierTable.size()-1][name] = val;
    }

    void IdentifierTableClean()
    {
        identifierTable.clear();
        NewNamespace();
    }

    void NewNamespace()
    {
        identifierTable.push_back(idTlbType{});
    }

    void DeleteNamespace()
    {
        identifierTable.pop_back();
    }

    void DefiniteFunc(const DefPtr& pDefRhs)
    {
        string index = pDefRhs->GetFunctionName();
        defTable[index] = pDefRhs;
    }
    
    variant<int, double> EvalDef(const string& funName, const vector<variant<int, double>>& args, int posX, int posY)
    {
        depth++;
        if (depth == 50)
        {
            depth = 0;
            throw Error("Runtime error: the depth of stack is out of range.", posX, posY);
        }
        if (defTable.find(funName) == defTable.end())
        {
            depth = 0;
            throw Error("Runtime error: undefined function \"" + funName + "\".", posX, posY);
        }
        auto pDef = defTable[funName];
        try
        {
            auto ret = pDef->Eval(args, posX, posY);
            depth--;
            return ret;
        }
        catch (Error& e)
        {
            depth = 0;
            throw;
        }
        
    }

    void DefTableClean()
    {
        defTable.clear();
    }
} // Environment
