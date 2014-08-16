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
        void Cleanup();

        const IGameObject* GetObject(std::string name);
        Errors RegisterObject(std::string name, IGameObjectPtr & primitive);


        ObjectLibrary();
        virtual ~ObjectLibrary() {};

    private: // arguments
        std::map<std::string, IGameObjectPtr>  m_primitives;
    };
}
//end  of namespace

// eof