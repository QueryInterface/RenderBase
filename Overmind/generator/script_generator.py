#!/usr/bin/python

import sys 
import os
import argparse
import re



tableList = {}

def isDirectory(dirName):
  if not os.path.isdir(dirName):
    raise Exception("%s is not a valid directory" % dirName)
  if os.access(dirName, os.R_OK):
    return dirName
  else:
    raise Exception("Access is denied to directory %s" % dirName)

def generateOutput(tableList):
    print """

    THIS IS AWESOME OUTPUT

    """
    for table in tableList.items():
        print ("table Name: %s" % table[1]["$name"])
        for record in table[1].items():
            if not re.search('\$', record[0]): 
                print (record[1] + " of type " + record[0])
            elif re.search("\$function", record[0]): 
                print (record[1])
        print ('\n')


def main():
    parser = argparse.ArgumentParser(description = 'C++ to Lua converter v0.1')
    parser.add_argument('-d',  '--source_directory', default = '.', type = isDirectory, help = 'directory to process files')
    scriptArgs = parser.parse_args()

    tableList = {}

    functionPattern = re.compile("""\s*
        (\w+)\s+(\w+)\s*                 # function return type and name
        \(\s*                            # (
        ([\w\,\s]*)                      # function parameters
        \)                               # )
        \s*=\s*0\s*;                     # PURE marker is optional
        """,
        re.VERBOSE)

    currentTable = {}
    for fileName in os.listdir(scriptArgs.source_directory):
        if not os.path.isfile(os.path.join(scriptArgs.source_directory, fileName)) or re.search('.py', fileName):
            continue

        file = open(fileName, "r")
        if not file:
            continue

        for line in file:
            line = re.sub('\s+', ' ', line)
            line = line.strip()

            if line is '':
                continue

            index = 0
            tableName = re.match('table\s+(\w+)', line)
            if tableName is not None: 
                currentTable = {"$name" : tableName.group(1)}
            
            parameter = re.search("{",line)
            if parameter is not None:
                line = line[parameter.span()[1]:]

            parameter = re.match("(\w+)\s+(\w+)\s*;", line)
            if parameter is not None and len(parameter.groups()) > 1:
                currentTable[parameter.groups(0)[0] ] = parameter.groups(0)[1]
            else:
                parameter = functionPattern.match( line)
                if parameter is not None and len(parameter.groups()) > 1:
                    funcParams = parameter.groups(0)[2].split(',')
                    funcParams.insert(0, parameter.groups(0)[1])
                    funcParams.insert(0, parameter.groups(0)[0])
                    currentTable['$function = ' + parameter.groups(0)[1]] = tuple(funcParams)

            if re.search("}", line) is not None:
                tableList[ currentTable["$name"] ] = currentTable
                currentTable = {}

    generateOutput(tableList)


if __name__=="__main__":
    main()

#eof