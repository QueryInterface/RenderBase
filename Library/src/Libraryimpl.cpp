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

// eof