#include "ObjectConstructor.h"
#include "Library.h"
#include <assert.h>

using namespace ConstructorImpl;

#ifndef min 
#define min(a, b) (a)<(b) ? (a) : (b)
#endif

#ifndef max 
#define max(a, b) (a)>(b) ? (a) : (b)
#endif

Core::Core() 
    : m_pillars(256)
    , m_isDirty(false)
{
    m_desc.LFT = Vector3D(INT32_MAX, INT32_MAX, INT32_MAX);
    m_desc.RBB = Vector3D(0,0,0);
}

void Core::SetElement(const ConstructionDescription& desc, const Vector3D& position, Directions direction, bool updateNeighbours)
{
    m_isDirty = true;

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
    Element element = {&desc, direction, 0};
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
                    Element ref = {&m_reference, direction, 0};
                    m_pillars.item(position.x + x, position.z + z).insert(position.y, ref);
                }
            }
        }
    }
}

void Core::IterrateObject(std::function<void(size_t, size_t, size_t, Element&)> visitor) 
{
    //ACHTUNG: double lambda!!!!
    m_pillars.for_each([&](size_t x, size_t z, Pillar_t& pillar){
        pillar.for_each([&](size_t y, Element& e){
            visitor(x,y,z,e);
        });
    });
}

void Core::UpdateNeighbourhood(size_t x, size_t y, size_t z)
{
    auto pillar = m_pillars.get_item_at(x,z);
    assert(nullptr != pillar);

    Element* self = pillar->get_item_at(y);
    Element* item = nullptr;

    assert(self);

    item = pillar->get_item_at(y - 1);
    if (item)
    {
        if (item->construction->neighborRelations[1] <= self->construction->neighborRelations[1])
            item->neighbourhood |= Directions::pY;
        if (self->construction->neighborRelations[4] <= item->construction->neighborRelations[4])
            self->neighbourhood |= Directions::nY;
    }
    item = pillar->get_item_at(y + 1);
    if (item)
    {
        if (item->construction->neighborRelations[4] <= self->construction->neighborRelations[4])
            item->neighbourhood |= Directions::nY;
        if (self->construction->neighborRelations[1] <= item->construction->neighborRelations[1])
            self->neighbourhood |= Directions::pY;
    }

    pillar = m_pillars.get_item_at(x - 1, z);
    if (pillar)
    {
        item = pillar->get_item_at(y);
        if (item)
        {
            if (item->construction->neighborRelations[0] <= self->construction->neighborRelations[0])
                item->neighbourhood |= Directions::pX;
            if (self->construction->neighborRelations[3] <= item->construction->neighborRelations[3])
                self->neighbourhood |= Directions::nX;
        }
    }

    pillar = m_pillars.get_item_at(x + 1, z);
    if (pillar)
    {
        item = pillar->get_item_at(y);
        if (item)
        {
            if (item->construction->neighborRelations[3] <= self->construction->neighborRelations[3])
                item->neighbourhood |= Directions::nX;
            if (self->construction->neighborRelations[0] <= item->construction->neighborRelations[0])
                self->neighbourhood |= Directions::pX;
        }
    }

    pillar = m_pillars.get_item_at(x, z - 1);
    if (pillar)
    {
        item = pillar->get_item_at(y);
        if (item)
        {
            if (item->construction->neighborRelations[2] <= self->construction->neighborRelations[2])
                item->neighbourhood |= Directions::pZ;
            if (self->construction->neighborRelations[5] <= item->construction->neighborRelations[5])
                self->neighbourhood |= Directions::nZ;
        }
    }

    pillar = m_pillars.get_item_at(x, z + 1);
    if (pillar)
    {
        item = pillar->get_item_at(y);
        if (item)
        {
            if (item->construction->neighborRelations[5] <= self->construction->neighborRelations[5])
                item->neighbourhood |= Directions::nZ;
            if (self->construction->neighborRelations[2] <= item->construction->neighborRelations[2])
                self->neighbourhood |= Directions::pZ;
        }
    }
}

// eof