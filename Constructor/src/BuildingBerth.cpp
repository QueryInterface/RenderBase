#include "BuildingBerth.h"
#include "Library.h"

using namespace ConstructorImpl;

const Compartment& BuildingBerth::GetCompartment() const
{
    //assert(index < m_compartments.size());
    return m_compartment;
}

bool BuildingBerth::SetElement(ElementType type, const Vector3D& position, Directions direction)
{
    if (type == ET_Space) 
    {
        return false;
    }

    m_compartment.SetElement(ILibrary::library()->GetConstruction(type), position, direction);
    return true;
}
// eof