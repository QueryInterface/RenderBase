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

        Status CheckObjectStatus(std::string name);
        const IGameObject* GetObject(std::string name);
        Status RegisterObject(std::string name, IGameObjectPtr & primitive);

        ObjectLibrary();
        virtual ~ObjectLibrary() {};

    private: 
        // loaded objects. all dependencies are loaded properly.
        // The objects are fully equiped and can be used in game.
        std::map<std::string, IGameObjectPtr> m_primitives;

        // list of object that have linked resources, that are not loaded yet,
        // so these objects cannot be pushed to object library until all required resources are loaded
        std::map<std::string, IGameObjectPtr> m_pendingObjects;
    };
}
//end  of namespace

// eof