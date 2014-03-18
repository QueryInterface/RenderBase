/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//    
/////////////////////////////////////////////////////////////////////////////////
#pragma once

namespace Constructor
{
// type of simple primitive 
    enum PrimitiveType
    {
    PT_Space   = 0,
    PT_Cube,

    PT_Wedge,
    PT_Ledder,

    PT_Cilinder,
    PT_Sphere,
    };

// simple 3D types
    struct Vector3D
    {
        int x;
        int y;
        int z;
        Vector3D() : x(1), y(0), z(0) {};
    };

    struct BoundingBox
    {
        Vector3D TLF;
        Vector3D Dimentions;
    };

/////////////////////////////////////////////////////////////////////
///
/// Simple construction element interface
///
/////////////////////////////////////////////////////////////////////
    struct IElement
    {
        virtual Vector3D&        GetDirection() const = 0;
        virtual Vector3D&        GetPosition() const = 0;
        virtual BoundingBox&     GetBoundingBox() const = 0;

        virtual ~IElement();
    };

}//end  of namespace

// eof