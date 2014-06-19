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

Element* Core::GetElement(const vector3i_t& position)
{
    auto pillar = m_pillars.get_item_at(position.x, position.z);
    return pillar ? pillar->get_item_at(position.y) : nullptr;
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

    for (auto neighbor : self->construction->neighbors)
    {
        Element* item = GetElement(
            vector3i_t(x + neighbor.relationPosition.x, y + neighbor.relationPosition.y, z + neighbor.relationPosition.z));
        if (!item)
        {
            continue;
        }

        const NeighborDesc* itemNeighbour = findRelation(*item, *self, neighbor.relationPosition);
        if (!itemNeighbour)
        {
            continue;
        }

        if ( itemNeighbour->relationWeight < neighbor.relationWeight)
            item->neighbourhood |= itemNeighbour->relationFlag;
        else if ( neighbor.relationWeight == itemNeighbour->relationWeight )
        {
            item->neighbourhood |= itemNeighbour->relationFlag;
            self->neighbourhood |= neighbor.relationFlag;
        }
        else
            self->neighbourhood |= neighbor.relationFlag;
    }
}

bool compareRotated(const vector3i_t& arg1, const vector3i_t& arg2, Directions src, Directions dst)
{
    // rotation volumes: 1: 90Deg, 2: 180Deg, -1: -90Deg
    int delta = 0;
    switch(src)
    {
    case Directions::nX : delta = -1; break;
    case Directions::pX : delta =  1; break;
    case Directions::nZ : delta =  2; break;
    }

    switch(dst)
    {
    case Directions::nX : delta -= -1; break;
    case Directions::pX : delta -=  1; break;
    case Directions::nZ : delta -=  2; break;
    }
    switch(delta)
    {
    case -1: 
        return arg1 == vector3i_t(-arg2.z, arg2.y, -arg2.x);
    case  1:
    case -3:
        return arg1 == vector3i_t(arg2.z, arg2.y, arg2.x);
    case  2:
    case -2: 
        return arg1 == vector3i_t(-arg2.x, arg2.y, -arg2.z);
    default: break;
    }
    return arg1 == arg2;
}

const NeighborDesc* Core::findRelation(const Element& item, const Element& self, vector3i_t& direction)
{
    const vector3i_t negative(-direction.x, -direction.y, -direction.z);
    for (const auto& relations : item.construction->neighbors)
    {
        if (compareRotated(negative, relations.relationPosition, self.direction, item.direction))
            return &relations;
    }
    return nullptr;
}

// eof