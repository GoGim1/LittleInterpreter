#pragma once 

#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <exception>


namespace Util
{
    using std::vector;
    using std::string;
    using std::exception;

    class Error : public exception
    {
    public: 
        Error(string s, int x = -1, int y = -1) : msg(s), posX(x) , posY(y)
        {
            if (posY != -1)
            {
                string ret("[Line " + std::to_string(posY) + ", " + std::to_string(posX) + "]: "  + msg.c_str());
                //std::cout << ret.c_str() << std::endl;
                msg = ret.c_str();
            }
        }

        virtual ~Error() noexcept {} 
        const char* what() const noexcept override
        { 
           return msg.c_str();
        }

    private:
        string      msg;
        int         posX;
        int         posY;
        
    };

#ifdef DEBUG
    void     PrintError();   
#endif 

    const string   DumpError(); 
    void     HandleError();
    void     AddError(const Error&);
    
};