/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//    
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "MathBasics.h"

enum ElementType : unsigned int
{
    Space   = 0,                 // empty space
    Cube,                        // simple cube

    Wedge,                       // canted cube
    Ledder,                      // ledder object

    Cilinder,                    // 1 cell cilinder object
    CilindricPlatform,           // 9 cell cilinder object, 3x3x1. basement for big rotation platform
    Sphere,                      // 1 cell sphere

    SimplePrimitivesCount,       // all primitives must be placed before this line

    UserCreated = 0x0000ffff,    // hope 65536 predefined elements will be enougth

    Reference   = 0xffffffff,    // reference to an object located in different cell.
};

// enum represents direction of the element,
// also the same enum is used for visible faces definitions
enum Directions : unsigned char
{
    NO = 0x00,

    pX = (1 << 0),
    pY = (1 << 1),
    pZ = (1 << 2),

    nX = (1 << 3),
    nY = (1 << 4),
    nZ = (1 << 5),

    All = 0xFF,
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
    ConstructionDescription() : primitiveUID(ElementType::Space), direction(Directions::pY) {};
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

/////////////////////////////////////////////////////////////////////
///
/// Constructor public interface
///
/////////////////////////////////////////////////////////////////////
struct Constructor //: public ISceneObject
{
    virtual void                    NewConstruction() = 0;
    virtual bool                    AddElement(ElementType type, const Vector3D& position, Directions direction) = 0;
    virtual const IConstructable&   GetBoundConstruction() = 0;
    
};

// eof