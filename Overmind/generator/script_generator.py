#!/usr/bin/python

import sys 
import os
import argparse
import re

#reserved field names
NAME_MARKER = '$name'
FILEDS_MARKER = '$fields'
METHOD_MARKER = '$methods'
TYPE_MARKER = '$type'
PARAMETERS_MARKER = '$params'

#general indices
NAME_ID = 0
TYPE_ID = 1

#keywords
__TABLE = 'table'
__METHOD = 'metamethod'

#common script  generated file header
__HEADER = "// This file is generated by python script.\n// Do not perform any manual modifications."

# Functions
def isDirectory(dirName):
  if not os.path.isdir(dirName):
    raise Exception("%s is not a valid directory" % dirName)
  if os.access(dirName, os.R_OK):
    return dirName
  else:
    raise Exception("Access is denied to directory %s" % dirName)

def writePreDefinitions(file):
    file.write("""#include <lua.hpp>

#define parse_int(L, out, index)        out = lua_tointeger(L, index)
#define parse_double(L, out, index)     out = lua_tonumber(L, index)
#define parse_float(L, out, index)      out = lua_tonumber(L, index)
#define parse_string(L, out, index)     out = lua_tostring(L, index)

""")

def generateHeaderFile(tableList, header):
    header.write("%s\n\n" % __HEADER)

    writePreDefinitions(header)

    for table in tableList.items():
        header.write("int parse_%s(lua_State* L, %s& table, int index = -1);\n" % (table[1][NAME_MARKER], table[1][NAME_MARKER]))


def generateSourceFile(tableList, headerName, source):
    source.write(
"""%s

#include "%s"
""" % (__HEADER, headerName))

    for table in tableList.items():
        source.write(
"""
inline int parse_%s(lua_State* L, %s& table, int index)
{
    luaL_checktype(L, -1, LUA_TTABLE);
    lua_pushnil(L);
    while(lua_next(L, -2))
    {
        std::string field(lua_tostring(L, -2));""" % (table[1][NAME_MARKER], table[1][NAME_MARKER]))
        for record in table[1][FILEDS_MARKER].items():
            source.write(
"""
        if (0 == field.compare(\"%s\"))
        {
            parse_%s(L, table.%s, -1);
            lua_pop(L, 1);
            continue;
        }""" % (record[NAME_ID], record[TYPE_ID], record[NAME_ID]))
        source.write(
"""
        riseError(L, "unknown field \\"" + field + "\\" found");
        lua_pop(L, 1);
    } /* while */
    return 0;
} // end of parser %s
""" % table[1][NAME_MARKER])


def main():
    parser = argparse.ArgumentParser(description = 'C++ to Lua converter v0.1')
    parser.add_argument('-d',  '--source_directory', default = '.', type = isDirectory, help = 'directory to process files')
    parser.add_argument('-ho',  '--header', required = True, type = argparse.FileType('w'), help = 'output header file name')
    parser.add_argument('-so',  '--source', required = True, type = argparse.FileType('w'), help = 'output source file name')
    scriptArgs = parser.parse_args()

    tableList = {}

    functionPattern = re.compile("""%s\s+ # 'metamethod' marker is mandatory
        (\w+)\s+(\w+)\s*                  # function return type and name
        \(\s*
        ([\w\,\s]*)                       # function parameters
        \s*\)
        \s*=\s*0\s*;                      # PURE marker is mandatory
        """ % __METHOD,
        re.VERBOSE)

    currentTable = {}
    for fileName in os.listdir(scriptArgs.source_directory):
        if not os.path.isfile(os.path.join(scriptArgs.source_directory, fileName)) or re.search('.py', fileName):
            continue

        file = open(fileName, "r")
        if not file:
            continue

        for line in file:
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
            tableName = re.match('%s\s+(\w+)' % __TABLE, line)
            if tableName is not None: 
                currentTable = {NAME_MARKER : tableName.group(1), FILEDS_MARKER : {}, METHOD_MARKER : {} }

            parameter = re.search("{",line)
            if parameter is not None:
                line = line[parameter.span()[1]:].strip()

            parameter = re.match('(\w+)\s+(\w+)\s*;', line)
            if parameter is not None and len(parameter.groups()) > 1:
                currentTable[FILEDS_MARKER].update( {parameter.groups(0)[1]: parameter.groups(0)[0] } )
            else:
                parameter = functionPattern.match( line)
                if parameter is not None and len(parameter.groups()) > 1:
                    funcParams = {}
                    for param in parameter.groups(0)[2].split(','):
                        fincParameter = param.strip().split(' ')
                        funcParams.update({fincParameter[1]: fincParameter[0]})
                    currentTable[METHOD_MARKER].update(
                        { parameter.groups(0)[1]:
                            {
                             TYPE_MARKER: parameter.groups(0)[0],
                             NAME_MARKER: parameter.groups(0)[1],
                             PARAMETERS_MARKER: funcParams
                            }
                        } )

            if re.search("}", line) is not None:
                tableList[ currentTable[NAME_MARKER] ] = currentTable
                currentTable = {}

    generateHeaderFile(tableList, scriptArgs.header)
    generateSourceFile(tableList, scriptArgs.header.name, scriptArgs.source)


if __name__=="__main__":
    main()

#eof