/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//    
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Constructor.h"
/////////////////////////////////////////////////////////////////////
///
/// object library public interface. object is a singletone
///
/////////////////////////////////////////////////////////////////////
struct IObjectLibrary;
struct ILibrary
{
    static ILibrary* library();

// access to constructions
    virtual const ConstructionDescription& GetConstruction(ElementType et) = 0;
    virtual void RegisterConstruction(IConstructable& element) = 0;

// access to meshes
//  virtual const ConstructionDescription& GetConstruction(ElementType et) = 0;
};

// eof