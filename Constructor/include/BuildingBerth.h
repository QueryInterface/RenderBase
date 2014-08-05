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
    class BuildingBerth : public Constructor
    {
    public:
        BuildingBerth() {};
        ~BuildingBerth() {};

        ///////////////////////////////////////////////////////////////////////////////////
        // Constructor interface

        ///////////////////////////////////////////////////////////////////////////////////
        // Reset content of all constructor elements
        virtual void        NewConstruction() { m_core.Reset(); };

        ///////////////////////////////////////////////////////////////////////////////////
        // Attaches element to construction
        virtual bool        SetElement(ElementType type, const vector3i_t& position, Directions direction, Directions copySettingsFrom = Directions::nY);

        ///////////////////////////////////////////////////////////////////////////////////
        // Attaches element to construction
        virtual IMesh&      GetMesh();

        ///////////////////////////////////////////////////////////////////////////////////
        // Return bounding box of constructed object
        virtual BBox        GetBoundingBox() const;

        ///////////////////////////////////////////////////////////////////////////////////
        // Return reference to logic constructor object
        Core&  GetCore();

        ///////////////////////////////////////////////////////////////////////////////////
        // Object
        // virtual IMeshPtr        GetMesh() const {return m_mesh;}
        // virtual const GLMeshDescs& GetMeshDescs() const override;

    private:
        Core        m_core;
        Hull        m_hull;

        PREVENT_COPY(BuildingBerth);
    };

}//end  of namespace constructor

// eof