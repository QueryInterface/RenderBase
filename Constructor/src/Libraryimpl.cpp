#include "LibraryImpl.h"
#include <memory>

using namespace ConstructorImpl;

Library::Library() 
{
}

void Library::Reset()
{
    m_objectLibrary.Cleanup();
    m_constructionLibrary.Cleanup();
}

Status Library::RegisterConstruction(std::string name, IConstructablePtr& element)
{
    auto subscribers = m_constructionSubscribers.find(name);
    if (subscribers != m_constructionSubscribers.end())
    {
        for (auto& subscriber : subscribers->second)
        {
            std::string objName = subscriber->GetName();
            m_objectLibrary.RegisterObject(objName, subscriber);
            m_pendingObjects.erase(objName);
        }
        m_constructionSubscribers.erase(subscribers);
    }
    return m_constructionLibrary.RegisterPrimitive(name, element);
}

const ConstructionDescription* Library::GetConstructionByName(std::string name)
{
    return m_constructionLibrary.GetConstructionDescription(name);
}

const ILibraryMesh& Library::GetMesh(unsigned int id)
{
    return m_meshLibrary.GetMeshObject(id);
}

void Library::RegisterMesh(unsigned int id, ILibraryMeshPtr& mesh)
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
    const ObjectProperties& properties = prototype->GetObjectContent();
    if (!properties.elementName.length() || m_constructionLibrary.GetConstructionDescription(properties.elementName))
    {
        return m_objectLibrary.RegisterObject(name, prototype);
    }

    if (Status::OK == m_objectLibrary.CheckObjectStatus(name))
    {
        return Status::AlreadyExists;
    }
    m_constructionSubscribers[properties.elementName].push_back(prototype);
    m_pendingObjects[name] = prototype;
    return Status::Pending;
}
// eof