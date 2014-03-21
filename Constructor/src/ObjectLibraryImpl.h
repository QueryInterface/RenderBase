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
        virtual ElementDescription* GetElementDescription(ElementType type) const;

    private:
        friend struct IObjectLibrary;

        ObjectLibrary() {};
        virtual ~ObjectLibrary() {};

    private: // arguments
        std::vector<IElement*> m_library;
    };

}//end  of namespace

// eof