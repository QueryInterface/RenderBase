#include "OvermindImpl.h"
#include "Constructor.h"
#include "lua.hpp"

using namespace OvermindImpl;

///////////////////////////////////////////////////////////////////////////////////////////////
//
// MACRO MADNESS
//
///////////////////////////////////////////////////////////////////////////////////////////////

#define INTERFACE_FUNCTION(InterfaceName, FunctionName)                                                         \
    lua_pushcfunction(m_lua, LUA_##InterfaceName##FunctionName);                                                \
    lua_setfield(m_lua, -2, #FunctionName)

#define LUA_ARRAY_ITEM(dst, idx)                                                                                \
    lua_rawgeti(L, -1, idx);                                                                                    \
    if (lua_isnumber(L, -1))                                                                                    \
        dst = lua_tointeger(L, -1);                                                                             \
    lua_pop(L, 1)

#define LUA_GOTO_NEXT_RECORD lua_pop(L, 1); continue

#define BEGIN_REFLECTION_TABLE(_name) static int parse##_name(lua_State* L, _name& table)                       \
{                                                                                                               \
    luaL_checktype(L, -1, LUA_TTABLE); lua_pushnil(L);                                                          \
    while(lua_next(L, -2) != 0)                                                                                 \
    {                                                                                                           \
        std::string field(lua_tostring(L, -2));

#define STRING_FIELD(_fieldName)                                                                                \
        if (0 == field.compare(#_fieldName) && lua_isstring(L, -1)) {                                           \
            table._fieldName = lua_tostring(L, -1); LUA_GOTO_NEXT_RECORD; }

#define INT_FIELD(_fieldName)                                                                                   \
        if (0 == field.compare(#_fieldName) && lua_isnumber(L, -1)) {                                           \
            table._fieldName = lua_tointeger(L, -1); LUA_GOTO_NEXT_RECORD; }

#define VEC3_FIELD(_fieldName)                                                                                  \
        if (0 == field.compare(#_fieldName) && lua_istable(L, -1)) {                                            \
            if ( lua_rawlen(L, -1) != 3)                                                                        \
                return luaL_error(L, "incorrect parameter " #_fieldName " 3 components vector expected");       \
            LUA_ARRAY_ITEM(table._fieldName.x, 1);                                                              \
            LUA_ARRAY_ITEM(table._fieldName.y, 2);                                                              \
            LUA_ARRAY_ITEM(table._fieldName.z, 3);                                                              \
            LUA_GOTO_NEXT_RECORD; }

#define END_REFLECTION_TABLE()                                                                                  \
        lua_pop(L, 1);                                                                                          \
    } /* while */                                                                                               \
    return 0;                                                                                                   \
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// LUA reflections. 
// Generate parsers for all structures defined in reflections.h 
//
///////////////////////////////////////////////////////////////////////////////////////////////

#define CUSTOM_REFLECTION_DEFINITIONS
    #include "Reflections.h"
#undef CUSTOM_REFLECTION_DEFINITIONS

///////////////////////////////////////////////////////////////////////////////////////////////
//
// OVERMIND itself
//
///////////////////////////////////////////////////////////////////////////////////////////////

Overmind::Overmind()
    : m_lua(luaL_newstate())
    , m_constructor(Constructor::GetConstructor())
{
    luaL_openlibs(m_lua);

    registerGlobals();
    registerConstructor();
    registerLibrary();
}

Overmind::~Overmind()
{
    lua_close(m_lua);
}

Status Overmind::ExecuteScript(std::string filename)
{
    if (luaL_dofile(m_lua, filename.c_str()))
    {
        m_errorMessages.push_back(lua_tostring(m_lua, -1));
        return Status::ScriptFailure;
    }
    return Status::OK;
}

std::string Overmind::GetLastError()
{
    if (m_errorMessages.empty())
        return "";

    std::string errorString = m_errorMessages.back();
    m_errorMessages.pop_back();
    return errorString;
}

void Overmind::registerGlobals()
{
    lua_pushlightuserdata(m_lua, this);
    lua_setfield(m_lua, LUA_REGISTRYINDEX, "_overmind");

    lua_pushlightuserdata(m_lua, &m_constructor);
    lua_setfield(m_lua, LUA_REGISTRYINDEX, "_constructor");

    lua_pushlightuserdata(m_lua, &m_constructor.GetLibrary());
    lua_setfield(m_lua, LUA_REGISTRYINDEX, "_library");
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// HELPERS
//
///////////////////////////////////////////////////////////////////////////////////////////////

#define LUA_CHECK_ARGUMENTS(count)                                      \
    if (lua_gettop(L) > count)                                          \
        return luaL_error(L, "invalid arguments count, only " #count " expected")

#define LUA_GETGLOBAL(type, object)                                     \
    lua_getfield(L, LUA_REGISTRYINDEX, "_" #object);                    \
    type* object = (type*)lua_touserdata(L, -1);                        \
    lua_pop(L, 1);                                                      \
    if (!object)                                                        \
        return luaL_error(L, "PANIC: " #object " interface not found")

///////////////////////////////////////////////////////////////////////////////////////////////
//
// LUA Interfaces: LIBRARY
//
///////////////////////////////////////////////////////////////////////////////////////////////

int LUA_LibraryNewConstruction(lua_State* L)
{
    return luaL_error(L, "NOT IMPLEMENTED");
}

int LUA_LibraryNewObject(lua_State* L)
{
    LUA_CHECK_ARGUMENTS(1);

    ObjectProperties properties = {};
    parseObjectProperties(L, properties);
    IConstructorObjectPtr obj(new ConstructorObjectBase(properties));

    LUA_GETGLOBAL(ILibrary, library);

    lua_pushinteger(L, (int)library->RegisterObject(properties.name, obj));
    return 1;
}

void Overmind::registerLibrary()
{
    lua_newtable(m_lua);
    INTERFACE_FUNCTION(Library, NewConstruction);
    INTERFACE_FUNCTION(Library, NewObject);

    lua_setglobal(m_lua, "Library");
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// LUA Interfaces: CONSTRUCTOR
//
///////////////////////////////////////////////////////////////////////////////////////////////

int LUA_ConstructorPlace(lua_State* L)
{
    LUA_CHECK_ARGUMENTS(1);

    PlacementParameters pp = {"", vector3i_t(0,0,0), Directions::pZ, Directions::nY};
    parsePlacementParameters(L, pp);

    LUA_GETGLOBAL(Constructor, constructor);
    lua_pushinteger(L, (int)constructor->PlaceObject(pp));
    return 1;
}

void Overmind::registerConstructor()
{
    lua_newtable(m_lua);
    INTERFACE_FUNCTION(Constructor, Place);

    lua_setglobal(m_lua, "Constructor");
}
// eof