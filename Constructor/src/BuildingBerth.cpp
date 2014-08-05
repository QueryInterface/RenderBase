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
    if (type == ElementType::Space) 
    {
        return false;
    }

    m_core.SetElement(ILibrary::library()->GetConstruction(type), position, direction, copySettingsFrom);
    return true;
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