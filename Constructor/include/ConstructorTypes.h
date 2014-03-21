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
        ET_Space   = 0,
        ET_Cube,

        ET_Wedge,
        ET_Ledder,

        ET_Cilinder,
        ET_Sphere,

        ET_SimplePrimitivesCount,

        ET_UserCreated = 0x0000ffff, // hope 65536 predefined elements will be enougth


        ET_Reference   = 0xffffffff, // reference to an object located in different cell.
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