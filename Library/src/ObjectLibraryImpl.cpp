#include "ObjectLibraryImpl.h"
#include <memory>

using namespace LibraryImpl;

ObjectLibrary::ObjectLibrary() 
{
}

void ObjectLibrary::Cleanup()
{
    m_primitives.clear();
}

const IGameObject* ObjectLibrary::GetObject(std::string name)
{
    auto object = m_primitives.find(name);
    if (object != m_primitives.end())
        return object->second.get();

    // try to load object from file;

    return nullptr;
}

Errors ObjectLibrary::RegisterObject(std::string name, IGameObjectPtr & primitive)
{
    auto object = m_primitives.find(name);
    if (object != m_primitives.end())
        return Errors::AlreadyExists;

    m_primitives[name] = primitive;
    return Errors::NoError;
}

//#include "ConstructionLibraryPrimitives.cpp"

// eof