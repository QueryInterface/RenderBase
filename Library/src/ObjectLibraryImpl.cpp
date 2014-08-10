#include "ObjectLibraryImpl.h"
#include <memory>

using namespace LibraryImpl;

ObjectLibrary::ObjectLibrary() 
{
}
const ILibraryObject* ObjectLibrary::GetObject(std::string name)
{
    auto object = m_primitives.find(name);
    if (object != m_primitives.end())
        return object->second;

    // try to load object from file;

    return nullptr;
}

bool ObjectLibrary::RegisterObject(std::string name, const ILibraryObject& primitive)
{
    auto object = m_primitives.find(name);
    if (object != m_primitives.end())
        return false;

    m_primitives[name] = &primitive;
    return true;
}

//#include "ConstructionLibraryPrimitives.cpp"

// eof