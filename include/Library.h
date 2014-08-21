/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//    
/////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>
#include "Resources.h"
#include "Construction.h"
#include "LibraryPrimitives.h"

/////////////////////////////////////////////////////////////////////
///
/// Resource library public interface. object is a singletone
///
/////////////////////////////////////////////////////////////////////

struct ILibrary
{
    virtual void Reset() = 0;

// interface available from Overmind
    virtual const ConstructionDescription* GetConstructionByName(std::string name) = 0;
    virtual void RegisterConstruction(std::string name, IConstructablePtr& element) = 0;

    virtual const ILibraryMesh& GetMesh(uint32_t id) = 0;
    virtual void RegisterMesh(uint32_t id, ILibraryMeshPtr& mesh) = 0;

    virtual Status CheckObjectStatus(std::string name) = 0;
    virtual const IGameObject* GetObjectByName(std::string name) = 0;
    virtual Status RegisterObject(std::string name, IGameObjectPtr & prototype) = 0;
};
// eof