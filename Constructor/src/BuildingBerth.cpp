#include "BuildingBerth.h"
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
}

void Compartment::SetElement(ElementType type, const Vector3D& position, Directions direction)
{
    position;
    m_desc.direction = direction;
    m_desc.primitiveUID = type;

    m_desc.Dimentions = Vector3D(
        max(m_desc.Dimentions.x, position.x + 1),
        max(m_desc.Dimentions.y, position.y + 1),
        max(m_desc.Dimentions.z, position.z + 1));
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