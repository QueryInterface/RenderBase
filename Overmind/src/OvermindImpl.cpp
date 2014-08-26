#include "OvermindImpl.h"
#include "Constructor.h"
#include "lua.hpp"

using namespace OvermindImpl;

#define INTERFACE_FUNCTION(InterfaceName, FunctionName)                                         \
    lua_pushcfunction(m_lua, LUA_##InterfaceName##FunctionName);                                \
    lua_setfield(m_lua, -2, #FunctionName)

#define BEGIN_REFLECTION_TABLE(_name) static void parse##_name(lua_State* L, _name& table)      \
{                                                                                               \
    luaL_checktype(L, -1, LUA_TTABLE); lua_pushnil(L);                                          \
    while(lua_next(L, -2) != 0)                                                                 \
    {                                                                                           \
        std::string field(lua_tostring(L, -2));

#define STRING_FIELD(_fieldName)                                                                \
        if (0 == field.compare(#_fieldName))                                                    \
            table._fieldName = lua_tostring(L, -1);

#define END_REFLECTION_TABLE()                                                                  \
            lua_pop(L, 1);                                                                      \
    } /* while */                                                                               \
}

#define ALL_DEFINITIONS
    #include "Reflections.h"
#undef ALL_DEFINITIONS

Overmind::Overmind()
    : m_lua(luaL_newstate())
    , m_constructor(Constructor::GetConstructor())
{
    luaL_openlibs(m_lua);

    registerGlobals();
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

///////////////////////////////////////////////////////////////////////////////////////////////
//
// LUA Interfaces: LIBRARY
//
///////////////////////////////////////////////////////////////////////////////////////////////

ILibrary& LUA_getLibrary(lua_State* L)
{
    lua_getfield(L, LUA_REGISTRYINDEX, "_library");
    ILibrary* library = (ILibrary*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    assert(library);
    return *library;
}

int LUA_LibraryNewConstruction(lua_State* L)
{
    return luaL_error(L, "NOT IMPLEMENTED");
}

int LUA_LibraryNewObject(lua_State* L)
{
    if (lua_gettop(L) > 1)
        return luaL_error(L, "invalid arguments count, only 1 table expected");

    ObjectProperties properties = {};
    parseObjectProperties(L, properties);
    IGameObjectPtr obj(new GameObjectBase(properties));

    lua_pushinteger(L, (int)LUA_getLibrary(L).RegisterObject(properties.name, obj));
    return 1;
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

void Overmind::registerLibrary()
{
    lua_newtable(m_lua);
    INTERFACE_FUNCTION(Library, NewConstruction);
    INTERFACE_FUNCTION(Library, NewObject);

    lua_setglobal(m_lua, "Library");
}
// eof