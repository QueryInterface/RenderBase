/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//    
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <Resources.h>
#include <Construction.h>
//#include <../Engine/include/ObjectImpl.h>
#include "MathBasics.h"
#include <vector>
#include <cstdint>

/////////////////////////////////////////////////////////////////////
///
/// Constructor public interface
///
/////////////////////////////////////////////////////////////////////
struct Constructor// : public Object
{
    virtual void        NewConstruction() = 0;
    virtual bool        SetElement(ElementType type, const vector3i_t& position, Directions orientation, Directions copySettingsFrom = Directions::nY) = 0;
    virtual bool        Weld(uint32_t group1, uint32_t group2) = 0;
    virtual uint32_t    GetGroup(const vector3i_t& position) = 0;
    virtual IMesh&      GetMesh() = 0;
    virtual BBox        GetBoundingBox() const = 0;

    static Constructor& GetConstructor();
};

// eof