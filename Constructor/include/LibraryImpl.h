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

namespace ConstructorImpl
{
/////////////////////////////////////////////////////////////////////
///
/// object library public interface. object is a singletone
///
/////////////////////////////////////////////////////////////////////
class Library : public ILibrary
{
public:
    Library();
    virtual ~Library() {};

    virtual void Reset();

    // construction library object
    virtual const ConstructionDescription* GetConstructionByName(std::string name);
    virtual void RegisterConstruction(std::string name, IConstructablePtr& element);

    // mesh library object
    // according to flags, mesh may contain different geometry
    virtual const ILibraryMesh& GetMesh(unsigned int id);
    virtual void  RegisterMesh(unsigned int id, ILibraryMeshPtr& mesh);

    // object library primitives
    // object contains information about 
    // - mesh
    // - construction
    // - material
    virtual Status CheckObjectStatus(std::string name);
    virtual const IGameObject* GetObjectByName(std::string name);
    virtual Status RegisterObject(std::string name, IGameObjectPtr & prototype);

    /////////////////////////////////////////////////////////////////////
    ///
    ConstructionLibrary& GetConstructionLibrary() {return m_constructionLibrary;}
    MeshLibrary&         GetMeshLibrary() {return m_meshLibrary;}

private:

    ConstructionLibrary m_constructionLibrary;
    MeshLibrary         m_meshLibrary;
    ObjectLibrary       m_objectLibrary;

    // list of object that have linked resources, not loaded into library,
    // so these objects cannot be pushed into object library until all required resources are loaded
    std::map<std::string, IGameObjectPtr> m_pendingObjects;
};

};
// eof