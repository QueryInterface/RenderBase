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

struct LayoutItem
{
    unsigned int    size;
    unsigned int    stride;
    unsigned int    offset;
};

struct Mesh
{
    std::vector<LayoutItem> layout;
    std::vector<float>      interleavedBuffer;
    std::vector<short>      indices;
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

    virtual const Mesh& GetMesh(unsigned int id, unsigned int flags) = 0;
};

// eof