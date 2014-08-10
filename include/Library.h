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

struct ObjectDescription
{
    uint32_t PrimitiveId;
    uint32_t Properties;
};

struct ILibraryObject
{

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

    virtual const ILibraryMesh& GetMesh(uint32_t id) = 0;
    virtual void RegisterMesh(uint32_t id, const ILibraryMesh& mesh) = 0;

    //virtual const ILibraryObject& GetObjectByID(uint32_t id) = 0;
    virtual const ILibraryObject* GetObjectByName(const char* name) = 0;
    virtual void RegisterObject(const char* name, const ILibraryObject& prototype) = 0;
};
// eof