#include "Error.h"

namespace Util
{
    vector<Error> errorList;

#ifdef DEBUG
    void PrintError()
    {
        std::cout << DumpError() << std::endl;
    }     
#endif

    const string DumpError()
    {
        std::stringstream s;
        for (auto& i : errorList)
            s << i.what() << std::endl;
        CleanError();
        return s.str();
    }
    
    void HandleError()
    {
        if (errorList.empty())  
            return;
#ifdef NDEBUG
        for (auto& i : errorList)
            std::cout << i.what() << std::endl;
#endif
        throw errorList.front();
    }

    void AddError(const Error& e)
    {
        errorList.push_back(e);
    }

    void CleanError()
    {
        errorList.clear();
    }
}