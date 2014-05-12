#include "BuildingBerth.h"
#include "Library.h"
#include "Resources.h"

using namespace ConstructorImpl;

Compartment& BuildingBerth::GetCompartment()
{
    //assert(index < m_compartments.size());
    return m_compartment;
}

bool BuildingBerth::SetElement(ElementType type, const Vector3D& position, Directions direction, bool updateNeighbours)
{
    if (type == ElementType::Space) 
    {
        return false;
    }

    m_compartment.SetElement(ILibrary::library()->GetConstruction(type), position, direction, updateNeighbours);
    return true;
}

IMesh& BuildingBerth::GetHull()
{
    if (m_compartment.IsOutdated())
    {
        m_compartment.ResetCore();
        m_hull.ConstructMesh(m_compartment);
    }
    return m_hull;
}
// eof