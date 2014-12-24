#include "OvermindImpl.h"
#include "Constructor.h"
#include "lua.hpp"

using namespace OvermindImpl;

///////////////////////////////////////////////////////////////////////////////////////////////
//
// MACRO MADNESS
//
///////////////////////////////////////////////////////////////////////////////////////////////

#define INTERFACE_FUNCTION(InterfaceName, FunctionName)                                       \
    lua_pushcfunction(m_lua, LUA_##InterfaceName##FunctionName);                              \
    lua_setfield(m_lua, -2, #FunctionName)

#define STRUCTURE_FIELD(value)                                                                \
    lua_pushinteger(m_lua, value);                                                            \
    lua_setfield(m_lua, -2, #value)

#define LUA_CHECK_ARGUMENTS(count)                                                            \
    if (lua_gettop(L) > count)                                                                \
        return luaL_error(L, "invalid arguments count, only " #count " expected")

#define LUA_GETGLOBAL(type, object)                                                           \
    lua_getfield(L, LUA_REGISTRYINDEX, "_" #object);                                          \
    type* object = (type*)lua_touserdata(L, -1);                                              \
    lua_pop(L, 1);                                                                            \
    if (!object)                                                                              \
        return luaL_error(L, "PANIC: " #object " interface not found")

#define LUA_VERIFY(func) if (int err = (func)){ return err;}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// LUA reflections. 
// Generate parsers for all structures defined in reflections.h 
//
///////////////////////////////////////////////////////////////////////////////////////////////

#include "ScriptReflections.h"

///////////////////////////////////////////////////////////////////////////////////////////////
//
// OVERMIND itself
//
///////////////////////////////////////////////////////////////////////////////////////////////

Overmind& Overmind::Get()
{
    static unique_ptr<Overmind> overmind(new OvermindCerebro);
    return *(overmind.get());
}

OvermindCerebro::OvermindCerebro()
    : m_lua(luaL_newstate())
    , m_constructor(*Constructor::Create())
{
    luaL_openlibs(m_lua);

    registerGlobals();
    registerConstructor();
    registerLibrary();
    registerDirections();
}

OvermindCerebro::~OvermindCerebro()
{
    lua_close(m_lua);
	Constructor::Destroy(&m_constructor);
}

Status OvermindCerebro::ExecuteScript(std::string filename)
{
    if (luaL_dofile(m_lua, filename.c_str()))
    {
        m_errorMessages.push_back(lua_tostring(m_lua, -1));
        return Status::ScriptFailure;
    }
    return Status::OK;
}

std::string OvermindCerebro::GetLastError()
{
    if (m_errorMessages.empty())
        return "";

    std::string errorString = m_errorMessages.back();
    m_errorMessages.pop_back();
    return errorString;
}

void OvermindCerebro::registerGlobals()
{
    lua_pushlightuserdata(m_lua, this);
    lua_setfield(m_lua, LUA_REGISTRYINDEX, "_overmind");

    lua_pushlightuserdata(m_lua, &m_constructor);
    lua_setfield(m_lua, LUA_REGISTRYINDEX, "_constructor");

    lua_pushlightuserdata(m_lua, &m_constructor.GetLibrary());
    lua_setfield(m_lua, LUA_REGISTRYINDEX, "_library");
}

void OvermindCerebro::registerDirections()
{
    lua_newtable(m_lua);
    STRUCTURE_FIELD(pX);
    STRUCTURE_FIELD(nX);
    STRUCTURE_FIELD(pY);
    STRUCTURE_FIELD(nY);
    STRUCTURE_FIELD(pZ);
    STRUCTURE_FIELD(nZ);
    lua_setglobal(m_lua, "Directions");
}

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
    LUA_VERIFY( parseObjectProperties(L, properties) );

    IConstructorObjectPtr obj(new ConstructorObjectBase(properties));

    LUA_GETGLOBAL(ILibrary, library);
    lua_pushinteger(L, (int)library->RegisterObject(properties.name, obj));
    return 1;
}

void OvermindCerebro::registerLibrary()
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
    LUA_VERIFY( parsePlacementParameters(L, pp) );

    LUA_GETGLOBAL(Constructor, constructor);
    lua_pushinteger(L, (int)constructor->PlaceObject(pp));
    return 1;
}

void OvermindCerebro::registerConstructor()
{
    lua_newtable(m_lua);
    INTERFACE_FUNCTION(Constructor, Place);

    lua_setglobal(m_lua, "Constructor");
}
// eof