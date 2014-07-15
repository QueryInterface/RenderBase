/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//    
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <Resources.h>
#include <Construction.h>
#include "MathBasics.h"
#include <vector>
#include <cstdint>

/////////////////////////////////////////////////////////////////////
///
/// Constructor public interface
///
/////////////////////////////////////////////////////////////////////

struct Constructor //: public ISceneObject
{
    virtual void        NewConstruction() = 0;
    virtual bool        SetElement(ElementType type, const vector3i_t& position, Directions direction, bool updateNeighbours = false) = 0;
    virtual IMesh&      GetMesh() = 0;
    virtual BBox        GetBoundingBox() const = 0;

    static Constructor& GetConstructor();
};

// eof