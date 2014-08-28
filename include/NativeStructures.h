/////////////////////////////////////////////////////////////////////////////////
//
// C++ representation of Scriptable structures
//
/////////////////////////////////////////////////////////////////////////////////
#pragma once

#undef BEGIN_REFLECTION_TABLE
#undef STRING_FIELD
#undef VEC3_FIELD
#undef INT_FIELD
#undef END_REFLECTION_TABLE

#define BEGIN_REFLECTION_TABLE(_name) struct _name {
#define STRING_FIELD(fieldName) std::string fieldName;
#define VEC3_FIELD(fieldName) vector3i_t fieldName;
#define INT_FIELD(fieldName) int fieldName;
#define END_REFLECTION_TABLE() }

///////////////////////////////////////////////////////////////////////////////////
//reflection structures

#include "internal/Reflections.h"

#undef BEGIN_REFLECTION_TABLE
#undef STRING_FIELD
#undef VEC3_FIELD
#undef INT_FIELD
#undef END_REFLECTION_TABLE
