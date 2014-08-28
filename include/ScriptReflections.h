/////////////////////////////////////////////////////////////////////////////////
//
// list of parser functions to translate LUA tables to native structures
//
/////////////////////////////////////////////////////////////////////////////////
#pragma once

#undef BEGIN_REFLECTION_TABLE
#undef STRING_FIELD
#undef VEC3_FIELD
#undef INT_FIELD
#undef END_REFLECTION_TABLE

#define LUA_ARRAY_ITEM(dst, idx)                                                                                \
    lua_rawgeti(L, -1, idx);                                                                                    \
    if (lua_isnumber(L, -1))                                                                                    \
        dst = lua_tointeger(L, -1);                                                                             \
    lua_pop(L, 1)

#define LUA_GOTO_NEXT_RECORD lua_pop(L, 1); continue

#define BEGIN_REFLECTION_TABLE(_name) inline int parse##_name(lua_State* L, _name& table)                       \
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


///////////////////////////////////////////////////////////////////////////////////
//reflection structures

#include "internal/Reflections.h"


#undef BEGIN_REFLECTION_TABLE
#undef STRING_FIELD
#undef VEC3_FIELD
#undef INT_FIELD
#undef END_REFLECTION_TABLE
