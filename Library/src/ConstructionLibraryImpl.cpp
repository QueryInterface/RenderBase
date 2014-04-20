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
    m_primitives[element.ConstructionDesc().primitiveUID] = &element;
}

const ConstructionDescription& ConstructionLibrary::GetConstructionDescription(ElementType type) const
{
    return (type >= m_primitives.size()) ? m_dummy : m_primitives[type]->ConstructionDesc();
}

#include "ConstructionLibraryPrimitives.cpp"

// eof