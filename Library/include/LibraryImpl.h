/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//    
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Library.h"
#include "ConstructionLibraryImpl.h"
#include "MeshLibraryImpl.h"
#include "ObjectLibraryImpl.h"

namespace LibraryImpl
{
/////////////////////////////////////////////////////////////////////
///
/// object library public interface. object is a singletone
///
/////////////////////////////////////////////////////////////////////
class Library : public ILibrary
{
public:
    // construction library object
    virtual const ConstructionDescription& GetConstruction(ElementType et);
    virtual void RegisterConstruction(IConstructable& element);

    // mesh library object
    // according to flags, mesh may contain different geometry
    virtual const ILibraryMesh& GetMesh(unsigned int id);
    virtual void  RegisterMesh(unsigned int id, const ILibraryMesh& mesh);

    // object library primitives
    // object contains information about 
    // - mesh
    // - construction
    // - material
    //virtual const IGameObject& GetObjectByID(uint32_t id);
    virtual const IGameObject* GetObjectByName(const char* name);
    virtual void RegisterObject(const char* name, const IGameObject& prototype);

    virtual ~Library() {};

private:
    friend struct ILibrary;

    Library();

    ConstructionLibrary m_constructionLibrary;
    MeshLibrary         m_meshLibrary;
    ObjectLibrary       m_objectLibrary;
};

};
// eof