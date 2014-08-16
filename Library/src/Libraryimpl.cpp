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

void Library::Reset()
{
    m_objectLibrary.Cleanup();
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

Status Library::CheckObjectStatus(std::string name)
{
    return m_objectLibrary.CheckObjectStatus(name);
}

const IGameObject* Library::GetObjectByName(std::string name)
{
    return m_objectLibrary.GetObject(name);
}

Status Library::RegisterObject(std::string name, IGameObjectPtr & prototype)
{
    return m_objectLibrary.RegisterObject(name, prototype);
}
// eof