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
    Element element = {&desc, direction, Directions::NO, 0};
    // if priitive can be morfed, morf it
    if (ElementType::Wedge == element.construction->primitiveUID)
    {
        morph(position, element);
    }
    m_pillars.item(position.x, position.z).insert(position.y, element);

    // notify neighbours aboutnew element
    if (updateNeighbours)
    {
        UpdateNeighbourhood(position);
    }

    if (vector3i_t(1,1,1) != (desc.RBB - desc.LFT))
    {
        for (int x = desc.LFT.x; x < desc.RBB.x; ++x)
        {
            for (int z = desc.LFT.z; z < desc.RBB.z; ++z)
            {
                if (x || z)
                {
                    Element ref = {&m_reference, direction, Directions::NO, 0};
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

void Core::UpdateNeighbourhood(const vector3i_t& pos)
{
    auto pillar = m_pillars.get_item_at(pos.x, pos.z);
    assert(nullptr != pillar);

    Element* self = pillar->get_item_at(pos.y);

    for (auto neighbor : self->construction->neighbors)
    {
        vector3i_t relativeDirection = rotate(neighbor.relationPosition, self->direction);

        Element* item = GetElement(relativeDirection + pos);
        if (!item)
            continue;

        const NeighborDesc* itemNeighbour = findRelation(*item, relativeDirection);
        if (!itemNeighbour)
            continue;

        if (itemNeighbour->relationWeight < neighbor.relationWeight)
            item->neighbourhood |= itemNeighbour->relationFlag;
        else if (neighbor.relationWeight == itemNeighbour->relationWeight)
        {
            item->neighbourhood |= itemNeighbour->relationFlag;
            self->neighbourhood |= neighbor.relationFlag;
        }
        else
            self->neighbourhood |= neighbor.relationFlag;
    }
}

void Core::morph(const vector3i_t& position, Element& self)
{
    vector3i_t relativeDirection = rotate(self.construction->neighbors[DirectionIndices::nZ_idx].relationPosition, self.direction);
    Element* item = GetElement(relativeDirection + position);
    if (item && item->construction->primitiveUID == Wedge) 
    {
        vector3i_t itemDirection = rotate(item->construction->neighbors[DirectionIndices::pX_idx].relationPosition, item->direction);
        if (itemDirection.x * relativeDirection.z + itemDirection.z * relativeDirection.x == 0 )
        {
            self.secondaryDirection = self.direction;
            self.construction = &ILibrary::library()->GetConstruction(WedgeOutCorner);
            //if ()
        }
    }
}

const NeighborDesc* Core::findRelation(const Element& item, vector3i_t& direction)
{
    const vector3i_t negative(-direction);
    for (const auto& relations : item.construction->neighbors)
    {
        if (rotate(relations.relationPosition, item.direction) == negative)
            return &relations;
    }
    return nullptr;
}

vector3i_t Core::rotate(const vector3i_t& vec, Directions dst) const
{
    switch(dst)
    {
    case Directions::nX : return vector3i_t(-vec.z, vec.y,  vec.x);
    case Directions::pX : return vector3i_t( vec.z, vec.y, -vec.x);
    case Directions::nZ : return vector3i_t(-vec.x, vec.y, -vec.z);
    }
    return vec;
}

// eof