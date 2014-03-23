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
        Vector3D    Direction; // for reference type it's an object position

        // bounding box
        Vector3D    TLF;
        Vector3D    Dimentions;
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