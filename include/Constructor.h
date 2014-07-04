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

    UpSideDown,

    All = 0xFF,
};

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

// description of element
struct ConstructionDescription
{
    ElementType primitiveUID;
    Directions  direction; // for reference type it's an object position

    // bounding box
    vector3i_t    LFT; // left, front, top
    vector3i_t    RBB; // right, bottom, back

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

/////////////////////////////////////////////////////////////////////
///
/// Library implementation of Mesh object
///
/////////////////////////////////////////////////////////////////////
struct ILibraryMesh : public IMesh
{
    enum class LayoutType : unsigned int
    {
        Vertices = 0,
        Normals,
        Texcoord0,
        Texcoord1,
        Tangent,
        Binormal,
        Triangle,
        Strip,
        Fan,
        Points
    };

    struct LayoutItem
    {
        LayoutType                  layoutType;
        size_t                      itemSize;
        size_t                      itemsCount;
        float*                      items;
    };

    struct IndexGroup
    {
        const uint32_t*         indices;  // might be NULL
        uint32_t                count;
    };

    struct GeometryDesc
    {
        std::vector<LayoutItem>     layout;
        std::vector<IndexGroup>     groups; // can be empty
    };
    virtual void GetGeometryDesc(uint32_t flags, GeometryDesc& out_descriptor) const = 0;
    virtual ~ILibraryMesh() {};
};

/////////////////////////////////////////////////////////////////////
///
/// Constructor public interface
///
/////////////////////////////////////////////////////////////////////
struct Constructor //: public ISceneObject
{
    virtual void                    NewConstruction() = 0;
    virtual bool                    AddElement(ElementType type, const vector3i_t& position, Directions direction) = 0;
    virtual const IConstructable&   GetBoundConstruction() = 0;
    
};

// eof