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

#include "Constructor.h"
#include <vector>
#include <memory>

namespace Constructor
{
    class ObjectConstructor
    {
    public:
        ObjectConstructor() {};
        ~ObjectConstructor() {};

        bool SetPrimitive(ElementType type, const Vector3D& position, const Vector3D& direction);
    private:
        std::shared_ptr<IConstructable> m_constructedObject;
    };

}//end  of namespace constructor

// eof