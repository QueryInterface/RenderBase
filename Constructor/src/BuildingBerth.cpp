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
    m_desc.Dimentions = Vector3D(0,0,0);
    m_desc.TLF = Vector3D(INT32_MAX, INT32_MAX, INT32_MAX);
}

void Compartment::SetElement(ElementType type, const Vector3D& position, Directions direction)
{
    m_desc.direction = direction;
    m_desc.primitiveUID = type;

    const ElementDescription &desc = IObjectLibrary::instance()->GetElementDescription(type);
    m_pillars.item(position.x, position.y).item(position.z) = type;
    if (Vector3D(1,1,1) != desc.Dimentions)
    {
        for (int x = 0; x < desc.Dimentions.x; ++x)
        {
            int X = x + desc.TLF.x;
            for (int y = 0; y < desc.Dimentions.y; ++y)
            {
                int Y = y + desc.TLF.y;
                if (X || Y)
                    m_pillars.item(position.x + X, position.y + Y).item(position.z) = ET_Reference;
            }
        }
    }

    m_desc.TLF = Vector3D(m_pillars.left(), m_pillars.top(), min(m_desc.TLF.z, position.z));
    m_desc.Dimentions = Vector3D(m_pillars.right() - m_desc.TLF.x, m_pillars.bottom() - m_desc.TLF.y,
        max(m_desc.Dimentions.z, position.z + desc.Dimentions.z - m_desc.TLF.z));
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