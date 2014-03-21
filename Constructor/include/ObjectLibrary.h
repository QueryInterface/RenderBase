/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//    
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "ConstructorTypes.h"

namespace Constructor
{

/////////////////////////////////////////////////////////////////////
///
/// object library public interface. object is a singletone
///
/////////////////////////////////////////////////////////////////////
    struct IObjectLibrary
    {
    public:
        virtual ElementDescription* GetElementDescription(ElementType type) const = 0;

        virtual ~IObjectLibrary() {};

    public:
        static IObjectLibrary* instance();
    };

}//end  of namespace

// eof