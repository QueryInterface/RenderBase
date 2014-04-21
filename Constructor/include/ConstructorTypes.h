/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//    
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "MathBasics.h"

namespace Constructor
{
// type of simple primitive 
    enum ElementType : unsigned int
    {
        ET_Space   = 0,                 // empty space
        ET_Cube,                        // simple cube

        ET_Wedge,                       // canted cube
        ET_Ledder,                      // ledder object

        ET_Cilinder,                    // 1 cell cilinder object
        ET_CilindricPlatform,           // 9 cell cilinder object, 3x3x1. basement for big rotation platform
        ET_Sphere,                      // 1 cell sphere

        ET_SimplePrimitivesCount,       // all primitives must be placed before this line

        ET_UserCreated = 0x0000ffff,    // hope 65536 predefined elements will be enougth

        ET_Reference   = 0xffffffff,    // reference to an object located in different cell.
    };

// enum represents direction of the element,
// also the same enum is used for visible faces definitions
    enum Directions : unsigned char
    {
        ED_NO = 0x00,

        ED_pX = 0x01,
        ED_pY = 0x02,
        ED_pZ = 0x04,

        ED_nX = 0x10,
        ED_nY = 0x20,
        ED_nZ = 0x40,

        ED_All = 0xFF,
    };
    
    typedef Vector3<int> Vector3D;

// description of element
    struct ConstructionDescription
    {
        ElementType primitiveUID;
        Directions  direction; // for reference type it's an object position

        // bounding box
        Vector3D    LFT; // left, front, top
        Vector3D    RBB; // right, bottom, back

        // default element direction is UP (for OpenGL coordinate system it is +Y)
        ConstructionDescription() : primitiveUID(ET_Space), direction(ED_pY) {};
    };

/////////////////////////////////////////////////////////////////////
///
/// Simple construction element interface
///
/////////////////////////////////////////////////////////////////////
    struct IConstructable
    {
        virtual const ConstructionDescription& ConstructionDesc() const = 0;

        virtual ~IConstructable() {};
    };

}//end  of namespace

// eof