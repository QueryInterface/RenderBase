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

struct lua_State;

namespace OvermindImpl
{
    class Overmind// : public Constructor
    {
    public:
        Overmind();
        ~Overmind();

    private:
        lua_State *m_lua;
    };

}//end  of namespace constructor

// eof