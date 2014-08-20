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

void Library::RegisterDefaultConstruction(std::string name, IConstructable* element)
{
    m_constructionLibrary.RegisterSimplePrimitive(name, element);
}

void Library::RegisterConstruction(std::string name, IConstructablePtr& element)
{
    m_constructionLibrary.RegisterPrimitive(name, element);
}

const ConstructionDescription* Library::GetConstruction(ElementType type)
{
    return m_constructionLibrary.GetConstructionDescription(type);
}

const ConstructionDescription* Library::GetConstructionByName(std::string name)
{
    return m_constructionLibrary.GetConstructionDescription(name);
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
    auto pending = m_pendingObjects.find(name);
    if (m_pendingObjects.end() != pending)
    {
        return Status::Pending;
    }
    return m_objectLibrary.CheckObjectStatus(name);
}

const IGameObject* Library::GetObjectByName(std::string name)
{
    return m_objectLibrary.GetObject(name);
}

Status Library::RegisterObject(std::string name, IGameObjectPtr & prototype)
{
    const IGameObject::ObjectProperties& properties = prototype->GetObjectContent();
    if (!properties.elementName.length() || m_constructionLibrary.GetConstructionDescription(properties.elementName))
    {
        return m_objectLibrary.RegisterObject(name, prototype);
    }

    m_pendingObjects[name] = prototype;
    return Status::Pending;
}
// eof