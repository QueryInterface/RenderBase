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

    virtual ~Library() {};

private:
    friend struct ILibrary;

    Library();

    ConstructionLibrary m_constructionLibrary;
    MeshLibrary         m_meshLibrary;
};

};
// eof