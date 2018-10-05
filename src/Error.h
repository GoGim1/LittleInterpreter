#pragma once 


#include <string>
#include <exception>

namespace Util
{
    using std::string;
    using std::exception;

    class Error : public exception
    {
    public: 
        string      msg;
        int         posX;
        int         posY;
        
        const char* what() const noexcept;
    };
};