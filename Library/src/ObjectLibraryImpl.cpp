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

Status ObjectLibrary::CheckObjectStatus(std::string name)
{
    auto pending = m_pendingObjects.find(name);
    if (m_pendingObjects.end() != pending)
    {
        return Status::Pending;
    }
    pending = m_primitives.find(name);
    return m_primitives.end() != pending ? Status::OK : Status::ResourceNotFound;
}

const IGameObject* ObjectLibrary::GetObject(std::string name)
{
    auto object = m_primitives.find(name);
    if (object != m_primitives.end())
        return object->second.get();

    // try to load object from file;

    return nullptr;
}

Status ObjectLibrary::RegisterObject(std::string name, IGameObjectPtr & primitive)
{
    auto object = m_primitives.find(name);
    if (object != m_primitives.end())
        return Status::AlreadyExists;

    m_primitives[name] = primitive;
    return Status::OK;
}

//#include "ConstructionLibraryPrimitives.cpp"

// eof