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
    m_desc.LFT = vector3i_t(INT32_MAX, INT32_MAX, INT32_MAX);
    m_desc.RBB = vector3i_t(0,0,0);
}

void Core::SetElement(const ConstructionDescription& desc, const vector3i_t& position, Directions direction, bool updateNeighbours)
{
    m_isDirty = true;

    m_desc.direction = direction;
    m_desc.primitiveUID = desc.primitiveUID;

    m_desc.LFT = vector3i_t(
        min(position.x + desc.LFT.x, m_desc.LFT.x),
        min(position.y + desc.LFT.y, m_desc.LFT.y),
        min(position.z + desc.LFT.z, m_desc.LFT.z));
    m_desc.RBB = vector3i_t(
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

    if (vector3i_t(1,1,1) != (desc.RBB - desc.LFT))
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

    const vector3i_t nbrs[] = {
        vector3i_t( 1, 0, 0),
        vector3i_t(0,  1, 0),
        vector3i_t(0, 0,  1),
        vector3i_t(0, 0, -1),
        vector3i_t(0, -1, 0),
        vector3i_t(-1, 0, 0),
    };

    for (size_t i = 0; i < 6; ++i)
    {
        auto pillar = m_pillars.get_item_at(x + nbrs[i].x, z + nbrs[i].z);
        if (pillar)
        {
            Element* item = pillar->get_item_at(y + nbrs[i].y);
            if (item)
            {
                uint32_t itemNeighbours = item->construction->neighborRelations[i];
                uint32_t selfNeighbours = self->construction->neighborRelations[5 - i];
                if ( itemNeighbours < selfNeighbours)
                    item->neighbourhood |= (1 << (5 - i));
                else if ( selfNeighbours == itemNeighbours )
                {
                    item->neighbourhood |= (1 << (5 - i));
                    self->neighbourhood |= (1 << i);
                }
                else
                    self->neighbourhood |= (1 << i);
            }
        }
    }
}

uint32_t Core::setNeighbor(Element* item, vector3i_t& direction, uint32_t relationWeight)
{
    uint32_t flag = 0;
    vector3i_t negative(-direction.x, -direction.y, -direction.z);

    for (size_t i = 0; i < item->construction->neighborDirections.size(); ++i)
    {
        if (item->construction->neighborDirections[i] == negative)
        {
            if (item->construction->neighborRelations[i] >= relationWeight)
            {
                //item->neighbourhood |= relation;
            }
        }
    }

    return flag;
}

// eof