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

ElementType&        Pillar::item(size_t z)
{
    for (auto chunk = m_chunks.begin(); chunk != m_chunks.end(); ++chunk )
    {
        /*if ( chunk->from - z == 1)
        {
            chunk->elements.push_front(ET_Space);
            return chunk->elements.front();
        }
        else if (chunk->from < z && chunk->elements.size() + chunk->from > z)
        {
            return chunk->elements[z - chunk->from];
        }
        else if (z - chunk->from == chunk->elements.size())
        {
            chunk->elements.push_back(ET_Space);
            if ((chunk + 1) != m_chunks.end() && (chunk+1)->from == chunk->from + chunk->elements.size())
            {
                chunk->elements.insert(chunk->elements.end(), (chunk+1)->elements.begin(), (chunk+1)->elements.end());
                m_chunks.erase(chunk + 1);
            }
            return chunk->elements.back();
        }*/
    }
    Chunk chunk;
    chunk.from = z;
    chunk.elements.push_back(ET_Space);
    return chunk.elements.back();
}

const ElementType*  Pillar::get_item_at(size_t z) const
{
    z;
    return nullptr;
}

Compartment::Compartment() 
    : m_pillars(256)
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

/*    bool created = false;
    if (m_compartments.empty())
    {
        m_compartments.push_back(Compartment());
        created = true;
    }*/

    m_compartment.SetElement(type, position, direction);
    return true;
}
// eof