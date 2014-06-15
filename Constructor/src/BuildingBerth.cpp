#include "BuildingBerth.h"
#include "Library.h"
#include "Resources.h"

using namespace ConstructorImpl;

Core& BuildingBerth::GetCore()
{
    //assert(index < m_cores.size());
    return m_core;
}

bool BuildingBerth::SetElement(ElementType type, const vector3i_t& position, Directions direction, bool updateNeighbours)
{
    if (type == ElementType::Space) 
    {
        return false;
    }

    m_core.SetElement(ILibrary::library()->GetConstruction(type), position, direction, updateNeighbours);
    return true;
}

IMesh& BuildingBerth::GetHull()
{
    if (m_core.IsOutdated())
    {
        m_core.ResetCore();
        m_hull.ConstructMesh(m_core);
    }
    return m_hull;
}
// eof