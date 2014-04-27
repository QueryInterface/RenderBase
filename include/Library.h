/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//    
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Constructor.h"
#include <vector>

//TODO: draft structures. to be movedto appropriate place



struct IMesh
{
    struct LayoutItem
    {
        unsigned int    size;
        unsigned int    stride;
        unsigned int    offset;
    };

    typedef std::vector<LayoutItem> LayoutData_t;
    typedef std::vector<float>      VertexData_t;
    typedef std::vector<short>      IndexData_t;

    virtual const LayoutData_t& GetLayout()     const = 0;
    virtual const VertexData_t& GetMeshBuffer() const = 0;
    virtual const IndexData_t&  GetIndexData(unsigned int flags) const = 0;

    virtual ~IMesh() {};
};

/////////////////////////////////////////////////////////////////////
///
/// Resource library public interface. object is a singletone
///
/////////////////////////////////////////////////////////////////////
struct ILibrary
{
    static ILibrary* library();

// access to constructions
    virtual const ConstructionDescription& GetConstruction(ElementType et) = 0;
    virtual void RegisterConstruction(IConstructable& element) = 0;

    virtual const IMesh& GetMesh(unsigned int id) = 0;
    virtual void RegisterMesh(unsigned int id, const IMesh& mesh) = 0;
};

// eof