#include "ConstructionLibraryImpl.h"
#include <memory>

using namespace LibraryImpl;

ConstructionLibrary::ConstructionLibrary() 
{
    m_primitives.resize(ElementType::SimplePrimitivesCount);
    m_dummy.primitiveUID = ElementType::SimplePrimitivesCount;
}

void ConstructionLibrary::RegisterPrimitive(std::string name, IConstructablePtr& element)
{
    size_t id = m_primitives.size();
    m_primitives.push_back(element);
    m_primitiveNameIdMap[name] = id;
}

void ConstructionLibrary::RegisterSimplePrimitive(std::string name, IConstructable* element)
{
    size_t id = element->ConstructionDesc().primitiveUID;
    assert(id < ElementType::SimplePrimitivesCount);
    m_primitives[id].reset(element);
    m_primitiveNameIdMap[name] = id;
}

const uint32_t ConstructionLibrary::GetConstructionId(std::string& name) const
{
    auto found = m_primitiveNameIdMap.find(name);
    return found != m_primitiveNameIdMap.end() ? found->second : ~0x0;
}

const ConstructionDescription& ConstructionLibrary::GetConstructionDescription(ElementType type) const
{
    return (type >= m_primitives.size()) ? m_dummy : m_primitives[type]->ConstructionDesc();
}

const ConstructionDescription& ConstructionLibrary::GetConstructionDescription(std::string& name) const
{
    auto found = m_primitiveNameIdMap.find(name);
    return (found != m_primitiveNameIdMap.end()) ? m_primitives[found->second]->ConstructionDesc() : m_dummy;
}

#include "ConstructionLibraryPrimitives.cpp"

// eof