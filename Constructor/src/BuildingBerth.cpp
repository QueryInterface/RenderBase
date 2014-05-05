#include "BuildingBerth.h"
#include "Library.h"

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
// eof