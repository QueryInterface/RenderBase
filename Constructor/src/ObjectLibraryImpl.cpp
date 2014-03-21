#include "ObjectLibraryImpl.h"
#include <memory>

using namespace Constructor;

IObjectLibrary* IObjectLibrary::instance()
{
    static std::unique_ptr<IObjectLibrary> library;
    if (nullptr == library)
    {
        library.reset(new ObjectLibrary());
    }
    return library.get();
}

ElementDescription* ObjectLibrary::GetElementDescription(ElementType type) const
{
    return (type >= m_library.size()) ? nullptr : m_library[type]->GetObjectDescription();
}

// eof