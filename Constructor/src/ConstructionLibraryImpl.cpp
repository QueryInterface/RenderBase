#include "ConstructionLibraryImpl.h"
#include <memory>

using namespace Constructor;

IConstructionLibrary* IConstructionLibrary::instance()
{
    static std::unique_ptr<IConstructionLibrary> library;
    if (nullptr == library)
    {
        library.reset(new ConstructionLibrary());
    }
    return library.get();
}

ConstructionLibrary::ConstructionLibrary() 
{
    m_primitives.resize(ET_SimplePrimitivesCount);
    m_dummy.primitiveUID = ET_SimplePrimitivesCount;
}

void ConstructionLibrary::RegisterPrimitive(IConstructable& element)
{
    m_primitives[element.GetObjectDescription().primitiveUID] = &element;
}

const ElementDescription& ConstructionLibrary::GetElementDescription(ElementType type) const
{
    return (type >= m_primitives.size()) ? m_dummy : m_primitives[type]->GetObjectDescription();
}

#include "ConstructionLibraryPrimitives.cpp"

// eof