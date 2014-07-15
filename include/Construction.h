/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//    
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <Resources.h>
#include "MathBasics.h"
#include <vector>
#include <cstdint>

enum ElementType : unsigned int
{
    Space   = 0,                 // empty space
    Cube,                        // simple cube

    Wedge,                       // canted cube
    WedgeOutCorner,              // joint between two wedges; could not be created by user
    WedgeInCorner,               // joint between two wedges; could not be created by user
    Ledder,                      // ledder object

    Cilinder,                    // 1 cell cilinder object
    CilindricPlatform,           // 9 cell cilinder object, 3x3x1. basement for big rotation platform
    Sphere,                      // 1 cell sphere

    SimplePrimitivesCount,       // all primitives must be placed before this line

    UserCreated = 0x0000ffff,    // hope 65536 predefined elements will be enougth

    Reference   = 0xffffffff,    // reference to an object located in different cell.
};

enum DirectionIndices : unsigned char
{
    pX_idx = 0,
    pY_idx,
    pZ_idx,
    nX_idx,
    nY_idx,
    nZ_idx,

    mirroredX_idx = 8,
    mirroredZ_idx,
};
// enum represents direction of the element,
// also the same enum is used for visible faces definitions
enum Directions : unsigned short
{
    NO = 0x00,

    pX = (1 << DirectionIndices::pX_idx),
    pY = (1 << DirectionIndices::pY_idx),
    pZ = (1 << DirectionIndices::pZ_idx),
    nX = (1 << DirectionIndices::nX_idx),
    nY = (1 << DirectionIndices::nY_idx),
    nZ = (1 << DirectionIndices::nZ_idx),

    FrontToBack = (1 << DirectionIndices::mirroredZ_idx),
    LeftToRight = (1 << DirectionIndices::mirroredX_idx),

    UpSideDown,

    All = 0xFF,
};

#define DIRECTION_MASK   0x00ff
#define MODIFICATOR_MASK 0xff00

enum Influences : unsigned int
{
    NOT_AFFECTED    = 0,
    WEDGE_TRIANGLE  = 5,
    LEDDER_TRIANGLE = 6,
    FULLY_COVERED   = 10, // 10 parts will be enough
};

// struct neighbor description
struct NeighborDesc
{
    uint32_t   relationWeight;
    vector3i_t relationPosition;
    Directions relationFlag;

    NeighborDesc() : relationWeight(NOT_AFFECTED) {}
    NeighborDesc(uint32_t weight, const vector3i_t& position, Directions flag) : relationWeight(weight), relationPosition(position), relationFlag(flag) {}
};

// struct bounding box
struct BBox
{
    vector3i_t LFT;
    vector3i_t RBB;
    BBox() : LFT(0,0,0), RBB(0,0,0) {};
    BBox(const vector3i_t& lft, const vector3i_t& rbb) : LFT(lft), RBB(rbb) {}
};

// description of element
struct ConstructionDescription
{
    ElementType primitiveUID;
    Directions  direction; // for reference type it's an object position

    // bounding box
    BBox        boundingBox;

    // neighbor relations section
    // influence multiplers
    std::vector<NeighborDesc> neighbors;

    // default element direction is UP (for OpenGL coordinate system it is +Y)
    ConstructionDescription() : primitiveUID(ElementType::Space), direction(Directions::pZ) {};
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


// eof