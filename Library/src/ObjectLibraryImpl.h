/////////////////////////////////////////////////////////////////////////////////
//
// class ObjectConstructor generates object from primitives and modules. 
//    the class doesn't generate mesh
//    
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Library.h"
#include <map>
#include <string>

/////////////////////////////////////////////////////////////////////
///
/// object library public interface. Library is a singletone
///
/////////////////////////////////////////////////////////////////////
namespace LibraryImpl
{
    class ObjectLibrary
    {
    public:
        //const ObjectDescription& GetObjectDescription(uint32_t id) const;

        const ILibraryObject* GetObject(std::string name);
        bool RegisterObject(std::string name, const ILibraryObject& primitive);


        ObjectLibrary();
        virtual ~ObjectLibrary() {};

    private: // arguments
        std::map<std::string, const ILibraryObject*>  m_primitives;
    };
}
//end  of namespace

// eof