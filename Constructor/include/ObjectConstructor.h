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
#include "include/QuadTree.h"
#include "include/RangeList.h"
#include <vector>
#include <list>
#include <memory>

namespace ConstructorImpl
{

// on a low level object consists from a set of compartments
    class Compartment : public IConstructable
    {
    public:
        // IElement interface
        const ConstructionDescription& ConstructionDesc() const {return m_desc;};
        Compartment();
        virtual ~Compartment() {};

    public:
        void SetElement(const ConstructionDescription& element, const Vector3D& position, Directions direction);

    private:
        ConstructionDescription                             m_desc;
        Utils::QuadTree< Utils::RangeList<ElementType> >    m_pillars;

        Compartment(const Compartment& arg);
        const Compartment& operator=(const Compartment& arg);
    };

}//end  of namespace constructor

// eof