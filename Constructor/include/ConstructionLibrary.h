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
    struct IConstructionLibrary
    {
    public:
        virtual const ElementDescription& GetElementDescription(ElementType type) const = 0;
        virtual void RegisterPrimitive(IConstructable& element) = 0;

        virtual ~IConstructionLibrary() {};

    public:
        static IConstructionLibrary* instance();
    };

}//end  of namespace

// eof