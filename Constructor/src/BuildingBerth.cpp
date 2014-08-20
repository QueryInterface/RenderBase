#include "BuildingBerth.h"
#include "Library.h"
#include "Resources.h"

using namespace ConstructorImpl;

Constructor& Constructor::GetConstructor()
{
    static unique_ptr<Constructor> constructor(new BuildingBerth);
    return *(constructor.get());
}

Core& BuildingBerth::GetCore()
{
    //assert(index < m_cores.size());
    return m_core;
}

bool BuildingBerth::SetElement(ElementType type, const vector3i_t& position, Directions direction, Directions copySettingsFrom)
{
    if (type == ElementType::Space || type >= ElementType::SimplePrimitivesCount) 
    {
        return false;
    }

    m_core.SetElement(*ILibrary::library()->GetConstruction(type), position, direction, copySettingsFrom);
    return true;
}

bool BuildingBerth::Weld(uint32_t group1, uint32_t group2)
{
    return m_core.Weld(group1, group2);
}

uint32_t BuildingBerth::GetGroup(const vector3i_t& position)
{
    Element *el = m_core.GetElement(position);
    return el ? el->group : ~0x0;
}

IMesh& BuildingBerth::GetMesh()
{
    if (m_core.IsUpdated())
    {
        m_hull.ConstructMesh(m_core);
    }
    return m_hull;
}

BBox BuildingBerth::GetBoundingBox() const
{
    return m_core.ConstructionDesc().boundingBox;
}
// eof