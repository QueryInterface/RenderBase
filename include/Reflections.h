
#ifndef BEGIN_REFLECTION_TABLE
#define SELF_DEFINED_BEGIN
#define BEGIN_REFLECTION_TABLE(_name) struct _name {
#endif

#ifndef STRING_FIELD
#define SELF_DEFINED_FIELD
#define STRING_FIELD(fieldName) std::string fieldName;
#endif

#ifndef END_REFLECTION_TABLE
#define SELF_DEFINED_END
#define END_REFLECTION_TABLE() }
#endif

#if defined(DEFINE_OBJECTPROPERTIES) || defined(ALL_DEFINITIONS)

BEGIN_REFLECTION_TABLE(ObjectProperties)
    STRING_FIELD(name)
    STRING_FIELD(meshName)
    STRING_FIELD(materialName)
    STRING_FIELD(elementName)
END_REFLECTION_TABLE();

#endif


#ifdef SELF_DEFINED_BEGIN
#undef BEGIN_REFLECTION_TABLE
#endif

#ifdef SELF_DEFINED_FIELD
#undef STRING_FIELD
#endif

#ifdef SELF_DEFINED_END
#undef END_REFLECTION_TABLE
#endif
