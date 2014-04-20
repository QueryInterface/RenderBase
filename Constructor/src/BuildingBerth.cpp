#include "BuildingBerth.h"
#include "Library.h"
#include <assert.h>

using namespace Constructor;

#ifndef min 
#define min(a, b) (a)<(b) ? (a) : (b)
#endif

#ifndef max 
#define max(a, b) (a)>(b) ? (a) : (b)
#endif

Compartment::Compartment() 
    : m_pillars(256)
{
    m_desc.LFT = Vector3D(INT32_MAX, INT32_MAX, INT32_MAX);
    m_desc.RBB = Vector3D(0,0,0);
}

void Compartment::SetElement(ElementType type, const Vector3D& position, Directions direction)
{
    m_desc.direction = direction;
    m_desc.primitiveUID = type;
    const ConstructionDescription &desc = IConstructionLibrary::instance()->GetConstructionDescription(type);

    m_desc.LFT = Vector3D(
        min(position.x + desc.LFT.x, m_desc.LFT.x),
        min(position.y + desc.LFT.y, m_desc.LFT.y),
        min(position.z + desc.LFT.z, m_desc.LFT.z));
    m_desc.RBB = Vector3D(
        max(position.x + desc.RBB.x, m_desc.RBB.x),
        max(position.y + desc.RBB.y, m_desc.RBB.y),
        max(position.z + desc.RBB.z, m_desc.RBB.z));

    m_pillars.item(position.x, position.y).insert(position.z, type);
    if (Vector3D(1,1,1) != (desc.RBB - desc.LFT))
    {
        for (int x = desc.LFT.x; x < desc.RBB.x; ++x)
        {
            for (int y = desc.LFT.y; y < desc.RBB.y; ++y)
            {
                if (x || y)
                    m_pillars.item(position.x + x, position.y + y).insert(position.z, ET_Reference);
            }
        }
    }
}

const Compartment& BuildingBerth::GetCompartment() const
{
    //assert(index < m_compartments.size());
    return m_compartment;
}

bool BuildingBerth::SetElement(ElementType type, const Vector3D& position, Directions direction)
{
    if (type == ET_Space) 
    {
        return false;
    }

    m_compartment.SetElement(type, position, direction);
    return true;
}
// eof