/////////////////////////////////////////////////////////////////////////////////
//
// Structs that have script reflection
//
/////////////////////////////////////////////////////////////////////////////////
#include "MathBasics.h"

BEGIN_REFLECTION_TABLE(ObjectProperties)
    STRING_FIELD(name)
    STRING_FIELD(meshName)
    STRING_FIELD(materialName)
    STRING_FIELD(elementName)
END_REFLECTION_TABLE();

BEGIN_REFLECTION_TABLE(PlacementParameters)
    STRING_FIELD(name)
    VEC3_FIELD(position)
    INT_FIELD(orientation)
    INT_FIELD(placeDirection)
END_REFLECTION_TABLE();

