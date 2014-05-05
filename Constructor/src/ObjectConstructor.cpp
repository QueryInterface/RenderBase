#include "ObjectConstructor.h"
#include <assert.h>

using namespace ConstructorImpl;

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

void Compartment::SetElement(const ConstructionDescription& desc, const Vector3D& position, Directions direction, bool updateNeighbours)
{
    m_desc.direction = direction;
    m_desc.primitiveUID = desc.primitiveUID;

    m_desc.LFT = Vector3D(
        min(position.x + desc.LFT.x, m_desc.LFT.x),
        min(position.y + desc.LFT.y, m_desc.LFT.y),
        min(position.z + desc.LFT.z, m_desc.LFT.z));
    m_desc.RBB = Vector3D(
        max(position.x + desc.RBB.x, m_desc.RBB.x),
        max(position.y + desc.RBB.y, m_desc.RBB.y),
        max(position.z + desc.RBB.z, m_desc.RBB.z));

    // Y is UP direction
    Element element = {desc.primitiveUID, direction, 0};
    m_pillars.item(position.x, position.z).insert(position.y, element);

    // notify neighbours aboutnew element
    if (updateNeighbours)
    {
        UpdateNeighbourhood(position.x, position.y, position.z);
    }

    if (Vector3D(1,1,1) != (desc.RBB - desc.LFT))
    {
        for (int x = desc.LFT.x; x < desc.RBB.x; ++x)
        {
            for (int z = desc.LFT.z; z < desc.RBB.z; ++z)
            {
                if (x || z)
                {
                    Element ref = {ElementType::Reference, direction, 0};
                    m_pillars.item(position.x + x, position.z + z).insert(position.y, ref);
                }
            }
        }
    }
}

void Compartment::UpdateNeighbourhood(size_t x, size_t y, size_t z)
{
    auto pillar = m_pillars.get_item_at(x,z);
    assert(nullptr != pillar);

    Element* self = pillar->get_item_at(y);
    Element* item = nullptr;

    assert(self);

    item = pillar->get_item_at(y - 1);
    if (item)
    {
        item->neighbourhood |= Directions::nY;
        self->neighbourhood |= Directions::pY;
    }
    item = pillar->get_item_at(y + 1);
    if (item)
    {
        item->neighbourhood |= Directions::pY;
        self->neighbourhood |= Directions::nY;
    }

    pillar = m_pillars.get_item_at(x - 1, z);
    if (pillar)
    {
        item = pillar->get_item_at(y);
        if (item)
        {
            item->neighbourhood |= Directions::nX;
            self->neighbourhood |= Directions::pX;
        }
    }

    pillar = m_pillars.get_item_at(x + 1, z);
    if (pillar)
    {
        item = pillar->get_item_at(y);
        if (item)
        {
            item->neighbourhood |= Directions::pX;
            self->neighbourhood |= Directions::nX;
        }
    }

    pillar = m_pillars.get_item_at(x, z - 1);
    if (pillar)
    {
        item = pillar->get_item_at(y);
        if (item)
        {
            item->neighbourhood |= Directions::nZ;
            self->neighbourhood |= Directions::pZ;
        }
    }

    pillar = m_pillars.get_item_at(x, z + 1);
    if (pillar)
    {
        item = pillar->get_item_at(y);
        if (item)
        {
            item->neighbourhood |= Directions::pZ;
            self->neighbourhood |= Directions::nZ;
        }
    }
}
// eof