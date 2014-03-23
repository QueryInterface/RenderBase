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
// simple 3D types
// TODO: make me a template in common header
    struct Vector3D
    {
        int x;
        int y;
        int z;
        Vector3D()                      : x(1), y(0), z(0) {};
        Vector3D(int X, int Y, int Z)   : x(X), y(Y), z(Z) {};
    };

    struct ElementDescription
    {
        ElementType primitiveUID;
        Directions  direction; // for reference type it's an object position

        // bounding box
        Vector3D    TLF;
        Vector3D    Dimentions;

        // default element direction is UP (for OpenGL coordinate system it is +Y)
        ElementDescription() : primitiveUID(ET_Space), direction(ED_pY) {};
    };

/////////////////////////////////////////////////////////////////////
///
/// Simple construction element interface
///
/////////////////////////////////////////////////////////////////////
    struct IElement
    {
        virtual const ElementDescription& GetObjectDescription() const = 0;

        virtual ~IElement() {};
    };

}//end  of namespace

// eof