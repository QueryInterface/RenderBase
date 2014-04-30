/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//    
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Constructor.h"
#include <vector>
#include "Resources.h"

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