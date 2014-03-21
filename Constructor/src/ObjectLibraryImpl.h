/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//    
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "ObjectLibrary.h"
#include <vector>

namespace Constructor
{
/////////////////////////////////////////////////////////////////////
///
/// object library public interface. Library is a singletone
///
/////////////////////////////////////////////////////////////////////
    struct ObjectLibrary : public IObjectLibrary
    {
    public:
        virtual const ElementDescription& GetElementDescription(ElementType type) const;

        virtual void RegisterElement(IElement& element);

    private:
        friend struct IObjectLibrary;

        ObjectLibrary();
        virtual ~ObjectLibrary() {};

    private: // arguments
        std::vector<const IElement*>  m_library;
        ElementDescription      m_dummy;
    };

}//end  of namespace

// eof