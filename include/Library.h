/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//    
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Construction.h"
#include <vector>
#include "Resources.h"

/////////////////////////////////////////////////////////////////////
///
/// Library implementation of Mesh object
///
/////////////////////////////////////////////////////////////////////

struct MeshProperties
{
    uint32_t    flags;
    vector3f_t  offset;
    uint32_t    orientation;
};

struct ILibraryMesh : public IMesh
{
    virtual void ConstructGeometry(const MeshProperties& properties, IMesh::Shape& out_descriptor) const = 0;
    virtual ~ILibraryMesh() {};
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

    virtual const ILibraryMesh& GetMesh(unsigned int id) = 0;
    virtual void RegisterMesh(unsigned int id, const ILibraryMesh& mesh) = 0;
};
// eof