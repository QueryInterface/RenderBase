#!/usr/bin/python

import sys
import os
import argparse
import re

#reserved field names
NAME_MARKER         = '$name'
FIELDS_MARKER       = '$fields'
METHOD_MARKER       = '$methods'
TYPE_MARKER         = '$type'
PARAMETERS_MARKER   = '$params'
ENUM_MARKER         = '$is_enum'

#general indices
NAME_ID = 1
TYPE_ID = 0

#keywords
__TABLE = 'table'
__METHOD = 'metamethod'
__ENUM = 'enumeration'

#common script  generated file header
__HEADER = "// This file is generated by python script.\n// Do not perform any manual modifications."

# Functions

def checkTemplates(file, lineNumber, line):
    if re.search('(\w+)\s*<(.+)>', line):
        raise Exception('template detected in file %s @ ln%d: %s' % (file.name, lineNumber, line))

#############################################
#  HEADER FILE GENERATOR
#############################################
def writeHeaderPreDefinitions(file):
    file.write(
"""#include <lua.hpp>

///////////////// PRE DEFINED FUNCTIONS /////////////////

#define parse_int(L, out, index)        out = lua_tointeger(L, index)
#define parse_bool(L, out, index)       out = (bool)lua_tointeger(L, index)
#define parse_double(L, out, index)     out = lua_tonumber(L, index)
#define parse_float(L, out, index)      out = lua_tonumber(L, index)
#define parse_string(L, out, index)     out = lua_tostring(L, index)

#define push_int(L, in)                 lua_pushinteger(L, in)
#define push_bool(L, in)                lua_pushinteger(L, in)
#define push_double(L, in)              lua_pushnumber(L, in)
#define push_float(L, in)               lua_pushnumber(L, in)
#define push_string(L, in)              lua_pushstring(L, in)

inline void riseError(lua_State* L, const std::string& message)
{
    lua_pushstring(L, message.c_str());
    lua_error(L);
}

///////////////// GENERATED FUNCTIONS /////////////////
""")

def generateHeaderFile(tableList, header):
    header.write("%s\n\n" % __HEADER)

    writeHeaderPreDefinitions(header)

    for table in tableList:
        declDictionary = {'$name' : table[NAME_MARKER], '$type' : 'table', '$registrator' : ''}
        if table[ENUM_MARKER]:
            declDictionary.update({'$type' : 'value'})
            declDictionary.update({'$registrator' : 'int register_%($name)s(lua_State* L);\n' % table})

        header.write(
"""
// Accessors for %($name)s enumeration
%($registrator)sint parse_%($name)s(lua_State* L, %($name)s& %($type)s, int index = -1);
int push_%($name)s(lua_State* L, const %($name)s& %($type)s);
""" % declDictionary)
        

#############################################
#  SOURCE FILE GENERATOR
#############################################

# push function
def generateEnumParser(table, sourceFile):
    sourceFile.write("""
int parse_%($name)s(lua_State* L, %($name)s& value, int index)
{
    if (!lua_isinteger(L, index))
        riseError(L, "invalid type for parameter of type %($name)s");

    return parse_int(L, value, index);
} // end of parser %($name)s
""" % table)

# push function
def generateEnumPush(table, sourceFile):
    sourceFile.write("""
int push_%($name)s(lua_State* L, const %($name)s& value)
{
    return push_int(L, value);
}
""" % table)

# register function
def generateEnumRegistrator(table, sourceFile):
    sourceFile.write("""
int register_%($name)s(lua_State* L)
{
    lua_getglobal(L, "%($name)s");
    if (!lua_isnil(L, -1))
        riseError(L, "enumerator %($name)s already registered");
""" % table)

    for record in table[FIELDS_MARKER]:
        sourceFile.write("""
    push_int(L, %s::%s);
    lua_setfield(m_lua, -2, "%s")
""" % (table[NAME_MARKER], record[NAME_ID], record[NAME_ID]))

    sourceFile.write("""
    lua_setglobal(L, "%($name)s");
    return 0;
} // end of register %($name)s
""" % table)
#############################################
# parser
def generateParser(table, sourceFile):
    sourceFile.write("""
int parse_%($name)s(lua_State* L, %($name)s& table, int index)
{
    luaL_checktype(L, -1, LUA_TTABLE);
    lua_pushnil(L);
    while(lua_next(L, -2))
    {
        std::string field(lua_tostring(L, -2));""" % table)

    for record in table[FIELDS_MARKER]:
        sourceFile.write("""
        if (0 == field.compare(\"%s\"))
        {
            parse_%s(L, table.%s, -1);
            lua_pop(L, 1);
            continue;
        }""" % (record[NAME_ID], record[TYPE_ID], record[NAME_ID]))

    sourceFile.write("""
        riseError(L, "unknown field \\"" + field + "\\" found");
        lua_pop(L, 1);
    } /* while */
    return 0;
} // end of parser %($name)s
""" % table)

# push function
def generatePush(table, sourceFile):
    sourceFile.write("""
int push_%($name)s(lua_State* L, const %($name)s& table)
{
    lua_newtable(L);
""" % table)

    for record in table[FIELDS_MARKER]:
        sourceFile.write("""
    push_%s(L, table.%s);
    lua_setfield(m_lua, -2, "%s")
""" % (record[TYPE_ID], record[NAME_ID], record[NAME_ID]))

    sourceFile.write("""
    return 0;
} // end of push %($name)s
""" % table)

# source file itself
def generateSourceFile(tableList, headerName, source):
    source.write(
"""%s

#include "%s"
""" % (__HEADER, headerName))

    for table in tableList:
        if table[ENUM_MARKER]:
            generateEnumParser(table, source)
            generateEnumPush(table, source)
            generateEnumRegistrator(table, source)
        else:
            generateParser(table, source)
            generatePush(table, source)

#############################################
#  TABLE PARSER
#############################################
def main():
    parser = argparse.ArgumentParser(description = 'C++ to Lua converter v0.1')
    parser.add_argument('-fl',  '--file_list',  required = True, type = argparse.FileType('r'), help = 'list of files to be processed', nargs = '+')
    parser.add_argument('-ho',  '--header',     required = True, type = argparse.FileType('w'), help = 'output header file name')
    parser.add_argument('-so',  '--source',     required = True, type = argparse.FileType('w'), help = 'output source file name')
    scriptArgs = parser.parse_args()

    tableList = []

    methodPattern = re.compile("""%s\s+   # 'metamethod' marker is mandatory
        (\w+)\s+(\w+)\s*                  # function return type and name
        \(\s*
        (.*)                              # function parameters
        \s*\)
        \s*=\s*0\s*;                      # PURE marker is mandatory
        """ % __METHOD,
        re.VERBOSE)

    #TODO:
    # allow pointer registration in structure
    fieldPattern = re.compile('(\w+)\s+(\w+)\s*;')
    enumPatter = re.compile("""
        (\w+)                           # enumerated value name
        (?:\s*=\s*.*)?                  # enum assigment is optional, symbol group is not captured
        \,?                             # comma is optional
        """, 
        re.VERBOSE) # enumeration field with optional assignment

    currentTable = {}
    for file in scriptArgs.file_list:
        lineNumber = 0

        tableStarted = False
        enumeration = False
        for line in file:
            lineNumber += 1
            line = re.sub('\s+', ' ', line)       # replace complex spaces by simple one
            line = re.sub('\/\/.*', '', line)     # remove single line comments
            line = re.sub('std\:\:', '', line)    # remove std:: prefixes
            #TODO:
            # undertand how multiple subline comments can be removed.
            # understand multiline comments as well
            line = re.sub('\/\*.*\*\/', '', line) # remove single subline comment
            line = line.strip()

            if line is '':
                continue

            index = 0
            if not tableStarted:
                tableName = re.match('%s\s+(\w+)' % __TABLE, line) or re.match('%s\s+(\w+)' % __ENUM, line)
                if tableName is not None:
                    enumeration = (re.search(__ENUM, line) is not None)
                    currentTable = {NAME_MARKER : tableName.group(1), FIELDS_MARKER : [], METHOD_MARKER : [], ENUM_MARKER : enumeration}
                    tableStarted = True
                    line = line[tableName.span()[1]:].strip()
                else:
                    continue

            parameter = re.search("{",line)
            if parameter is not None:
                line = line[parameter.span()[1]:].strip()

            parameter = fieldPattern.match(line) if not enumeration else enumPatter.match(line)
            # field detected. parse it and save as a tuple (type, name)
            if parameter is not None and len(parameter.groups()) > 0:
                # check template in translated field. non translated fields may be templates
                if enumeration:
                    currentTable[FIELDS_MARKER].append( ('int', parameter.groups(0)[0]) )
                else:
                    checkTemplates(file, lineNumber, line)
                    currentTable[FIELDS_MARKER].append( parameter.groups(0) )
            else:
                # function detected
                # Save function as a name, typoe and list of parameters.
                parameter = methodPattern.match(line)
                if parameter is not None and len(parameter.groups()) > 1:
                    # check template in translated method. non translated methods may be templates
                    checkTemplates(file, lineNumber, line)
                    funcParams = []
                    #TODO:
                    # const parameters are not allowed yet
                    for param in parameter.groups(0)[2].split(','):
                        checkTemplates(file, lineNumber, param.strip())
                        param = re.sub('\s*const\s+', '', param)
                        funcParameter = param.strip().split(' ')
                        funcParams.append( tuple(funcParameter) )

                    currentTable[METHOD_MARKER].append(
                            {
                                TYPE_MARKER: parameter.groups(0)[TYPE_ID],
                                NAME_MARKER: parameter.groups(0)[NAME_ID],
                                PARAMETERS_MARKER: funcParams
                            }
                        )

            if re.search("}", line) is not None:
                tableList.append(currentTable)
                currentTable = {}
                tableStarted = False
                enumeration  = False

    generateHeaderFile(tableList, scriptArgs.header)
    generateSourceFile(tableList, scriptArgs.header.name, scriptArgs.source)


if __name__=="__main__":
    main()

