/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//    
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Library.h"
#include "Constructor.h"
#include <vector>

/////////////////////////////////////////////////////////////////////
///
/// object library public interface. Library is a singletone
///
/////////////////////////////////////////////////////////////////////
namespace LibraryImpl
{
    class ConstructionLibrary
    {
    public:
        const ConstructionDescription& GetConstructionDescription(ElementType type) const;

        void RegisterPrimitive(IConstructable& element);

        ConstructionLibrary();
        virtual ~ConstructionLibrary() {};

    private: // arguments
        std::vector<const IConstructable*>  m_primitives;
        ConstructionDescription      m_dummy;
    };
}
//end  of namespace

// eof