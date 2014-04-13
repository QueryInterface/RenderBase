#include "BuildingBerth.h"
#include "ConstructionLibrary.h"
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
    //for (auto chunk = m_chunks.begin(); chunk != m_chunks.end(); ++chunk )
    //{
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
        }
    }/**/
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
    m_desc.LFT = Vector3D(INT32_MAX, INT32_MAX, INT32_MAX);
    m_desc.RBB = Vector3D(0,0,0);
}

void Compartment::SetElement(ElementType type, const Vector3D& position, Directions direction)
{
    m_desc.direction = direction;
    m_desc.primitiveUID = type;
    const ElementDescription &desc = IConstructionLibrary::instance()->GetElementDescription(type);

    m_desc.LFT = Vector3D(
        min(position.x + desc.LFT.x, m_desc.LFT.x),
        min(position.y + desc.LFT.y, m_desc.LFT.y),
        min(position.z + desc.LFT.z, m_desc.LFT.z));
    m_desc.RBB = Vector3D(
        max(position.x + desc.RBB.x, m_desc.RBB.x),
        max(position.y + desc.RBB.y, m_desc.RBB.y),
        max(position.z + desc.RBB.z, m_desc.RBB.z));

    m_pillars.item(position.x, position.y).item(position.z) = type;
    if (Vector3D(1,1,1) != (desc.RBB - desc.LFT))
    {
        for (int x = desc.LFT.x; x < desc.RBB.x; ++x)
        {
            for (int y = desc.LFT.y; y < desc.RBB.y; ++y)
            {
                if (x || y)
                    m_pillars.item(position.x + x, position.y + y).item(position.z) = ET_Reference;
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