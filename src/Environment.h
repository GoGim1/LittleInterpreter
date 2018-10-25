#pragma once
#include <unordered_map>
#include <string>
#include <variant>
#include <memory>
#include <vector>
#include "Error.h"
namespace Environment
{
    using namespace Util;

    using std::unordered_map;
    using std::string;
    using std::variant;
    using std::shared_ptr;
    using std::vector;

    extern bool                         IsIdentifierDefined(const string&);
    extern const variant<int, double>&  GetIdentifierVal(const string&);
    extern void                         IdentifierTableClean();
    extern void                         DefineIdentifier(const string&, const variant<int, double>&);
    extern void                         NewNamespace();
    extern void                         DeleteNamespace();
    
    extern variant<int, double>         EvalDef(const string&, const vector<variant<int, double>>&, int, int);
    extern void                         DefTableClean();
} // Environment
