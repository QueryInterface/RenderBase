/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//
// the class is responcible for:
//    - create element based structure
//    - split structure on separate solid modules
//    - create skeleton for object
//
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "common.h"
#include <vector>
#include <string>

struct lua_State;
struct Constructor;

namespace OvermindImpl
{

    class Overmind
    {
    public:
        Overmind();
        ~Overmind();

        ///////////////////////////////////////////////////////////////////////////////////
        // executes given script
        Status ExecuteScript(std::string script);

        ///////////////////////////////////////////////////////////////////////////////////
        // return last error message
        // if string is empty, no errors happened
        std::string GetLastError();

    private:
        void registerGlobals();
        void registerLibrary();

        lua_State *m_lua;
        Constructor& m_constructor;

        std::vector<std::string> m_errorMessages;

        PREVENT_COPY(Overmind);
    };

}//end  of namespace constructor

// eof