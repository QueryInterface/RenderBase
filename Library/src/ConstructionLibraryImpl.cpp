#include "ConstructionLibraryImpl.h"
#include <memory>

using namespace LibraryImpl;

ConstructionLibrary::ConstructionLibrary() 
{
    m_primitives.resize(ElementType::SimplePrimitivesCount);
    m_dummy.primitiveUID = ElementType::SimplePrimitivesCount;
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