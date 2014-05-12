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
#include "HullConstructor.h"

struct IMesh;

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

        // Attaches element to Complartment. if new Core was created function returns true;
        bool SetElement(ElementType type, const Vector3D& position, Directions direction, bool updateNeighbours = false);

//        size_t GetCoresCount() const {return m_Cores.size();}
        Core&  GetCore();
        IMesh& GetHull();

    private:
        Core        m_core;
        Hull        m_hull;
    };

}//end  of namespace constructor

// eof