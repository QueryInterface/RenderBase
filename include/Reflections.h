
#ifndef CUSTOM_REFLECTION_DEFINITIONS
    #define BEGIN_REFLECTION_TABLE(_name) struct _name {
    #define STRING_FIELD(fieldName) std::string fieldName;
    #define VEC3_FIELD(fieldName) vector3i_t fieldName;
    #define INT_FIELD(fieldName) int fieldName;
    #define END_REFLECTION_TABLE() }
#endif

///////////////////////////////////////////////////////////////////////////////////
//reflection structures

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


#ifndef CUSTOM_REFLECTION_DEFINITIONS
    #undef BEGIN_REFLECTION_TABLE
    #undef STRING_FIELD
    #undef VEC3_FIELD
    #undef INT_FIELD
    #undef END_REFLECTION_TABLE
#endif
