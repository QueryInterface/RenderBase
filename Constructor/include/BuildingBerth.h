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
#include "ObjectConstructor.h"

namespace ConstructorImpl
{
///////////////////////////////////////////////////////////////////////////////////////////////
//
// primary building berth.
//
///////////////////////////////////////////////////////////////////////////////////////////////
    class BuildingBerth
    {
    public:
        BuildingBerth() {};
        ~BuildingBerth() {};

        // Attaches element to Complartment. if new compartment was created function returns true;
        bool SetElement(ElementType type, const Vector3D& position, Directions direction);

//        size_t GetCompartmentsCount() const {return m_compartments.size();}
        const Compartment& GetCompartment() const;
    private:
        Compartment m_compartment;
    };

}//end  of namespace constructor

// eof