#include "LibraryImpl.h"
#include <memory>

using namespace LibraryImpl;

ILibrary* ILibrary::library()
{
    static std::unique_ptr<ILibrary> library;
    if (nullptr == library)
    {
        library.reset(new Library());
    }
    return library.get();
}

Library::Library() 
{
}

void Library::RegisterConstruction(IConstructable& element)
{
    m_constructionLibrary.RegisterPrimitive(element);
}

const ConstructionDescription& Library::GetConstruction(ElementType type)
{
    return m_constructionLibrary.GetConstructionDescription(type);
}

const ILibraryMesh& Library::GetMesh(unsigned int id)
{
    return m_meshLibrary.GetMeshObject(id);
}

void Library::RegisterMesh(unsigned int id, const ILibraryMesh& mesh)
{
    m_meshLibrary.RegisterMesh(id, mesh);
}

const IGameObject* Library::GetObjectByName(const char* name)
{
    return m_objectLibrary.GetObject(name);
}

void Library::RegisterObject(const char* name, const IGameObject& prototype)
{
    m_objectLibrary.RegisterObject(name, prototype);
}
// eof