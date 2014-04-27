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

const Mesh& Library::GetMesh(unsigned int id, unsigned int flags)
{
    return m_meshLibrary.GetMeshObject(id, flags);
}

void Library::RegisterMesh(unsigned int id, unsigned int flags, const Mesh& mesh)
{
    m_meshLibrary.RegisterMesh(id, flags,mesh);
}
// eof