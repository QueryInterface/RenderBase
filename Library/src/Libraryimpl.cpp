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

const IMesh& Library::GetMesh(unsigned int id)
{
    return m_meshLibrary.GetMeshObject(id);
}

void Library::RegisterMesh(unsigned int id, const IMesh& mesh)
{
    m_meshLibrary.RegisterMesh(id, mesh);
}
// eof