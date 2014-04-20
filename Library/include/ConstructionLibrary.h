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
struct IConstructionLibrary
{
public:
    virtual const Constructor::ConstructionDescription& GetConstructionDescription(Constructor::ElementType type) const = 0;
    virtual void RegisterPrimitive(Constructor::IConstructable& element) = 0;

    virtual ~IConstructionLibrary() {};

public:
    static IConstructionLibrary* instance();
};

// eof