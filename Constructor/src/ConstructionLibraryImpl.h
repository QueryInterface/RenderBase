/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//    
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Library.h"
#include "Constructor.h"
#include <vector>
#include <map>
#include <string>

/////////////////////////////////////////////////////////////////////
///
/// object library public interface. Library is a singletone
///
/////////////////////////////////////////////////////////////////////
namespace ConstructorImpl
{
    class ConstructionLibrary
    {
    public:
        const ConstructionDescription* GetConstructionDescription(const std::string& name) const;
        const uint32_t GetConstructionId(std::string& name) const;

        void RegisterPrimitive(std::string name, IConstructablePtr& element);

        // fast access to built in primitives
        const ConstructionDescription* GetConstructionDescription(ElementType type) const;
        void RegisterSimplePrimitive(std::string name, IConstructable* element);

        ConstructionLibrary();
        virtual ~ConstructionLibrary() {};

    private: // arguments
        std::map<std::string, size_t>   m_primitiveNameIdMap;
        std::vector<IConstructablePtr>  m_primitives;
        ConstructionDescription         m_dummy;
    };
}
//end  of namespace

// eof