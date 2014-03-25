#include "BuildingBerth.h"
#include "ObjectLibrary.h"
#include <assert.h>

using namespace Constructor;

#ifndef min 
#define min(a, b) (a)<(b) ? (a) : (b)
#endif

#ifndef max 
#define max(a, b) (a)>(b) ? (a) : (b)
#endif

Compartment::Compartment()
{
    m_desc.Dimentions = Vector3D(0,0,0);
    m_desc.TLF = Vector3D(INT32_MAX, INT32_MAX, INT32_MAX);
}

void Compartment::SetElement(ElementType type, const Vector3D& position, Directions direction)
{
    m_desc.direction = direction;
    m_desc.primitiveUID = type;

    const ElementDescription &desc = IObjectLibrary::instance()->GetElementDescription(type);
    Vector3D elementPosition = Vector3D(position.x + desc.TLF.x, position.y + desc.TLF.y, position.z + desc.TLF.z);

    m_desc.TLF = Vector3D(
        min(m_desc.TLF.x, elementPosition.x),
        min(m_desc.TLF.y, elementPosition.y),
        min(m_desc.TLF.z, elementPosition.z));

    m_desc.Dimentions = Vector3D(
        max(m_desc.Dimentions.x, elementPosition.x + desc.Dimentions.x - m_desc.TLF.x),
        max(m_desc.Dimentions.y, elementPosition.y + desc.Dimentions.y - m_desc.TLF.y),
        max(m_desc.Dimentions.z, elementPosition.z + desc.Dimentions.z - m_desc.TLF.z));
}

const Compartment& BuildingBerth::GetCompartment(size_t index) const
{
    assert(index < m_compartments.size());
    return m_compartments[index];
}

bool BuildingBerth::SetElement(ElementType type, const Vector3D& position, Directions direction)
{
    if (type == ET_Space) 
    {
        return false;
    }

    bool created = false;
    if (m_compartments.empty())
    {
        m_compartments.push_back(Compartment());
        created = true;
    }

    m_compartments.back().SetElement(type, position, direction);
    return created;
}
// eof