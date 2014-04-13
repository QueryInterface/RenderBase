/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//    
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "ConstructionLibrary.h"
#include <vector>

namespace Constructor
{
/////////////////////////////////////////////////////////////////////
///
/// object library public interface. Library is a singletone
///
/////////////////////////////////////////////////////////////////////
    struct ConstructionLibrary : public IConstructionLibrary
    {
    public:
        virtual const ElementDescription& GetElementDescription(ElementType type) const;

        virtual void RegisterPrimitive(IConstructable& element);

    private:
        friend struct IConstructionLibrary;

        ConstructionLibrary();
        virtual ~ConstructionLibrary() {};

    private: // arguments
        std::vector<const IConstructable*>  m_primitives;
        ElementDescription      m_dummy;
    };

}//end  of namespace

// eof