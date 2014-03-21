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

ObjectLibrary::ObjectLibrary() 
{
    m_library.resize(ET_SimplePrimitivesCount);
    m_dummy.primitiveUID = ET_SimplePrimitivesCount;
}

void ObjectLibrary::RegisterElement(IElement& element)
{
    m_library[element.GetObjectDescription().primitiveUID] = &element;
}

const ElementDescription& ObjectLibrary::GetElementDescription(ElementType type) const
{
    return (type >= m_library.size()) ? m_dummy : m_library[type]->GetObjectDescription();
}

#include "ObjectLibraryPrimitives.cpp"

// eof