#!/usr/bin/python

import sys 
import os
import argparse
import re

tableList = {}

def generateOutput():
    global tableList
    noop


def main():
    parser = argparse.ArgumentParser(description = 'C++ to Lua converter v0.1')
    parser.add_argument('-fl',  '--file_list', nargs = '+', required=True, type = argparse.FileType('r'), help = 'list of files to process')
    scriptArgs = parser.parse_args()

    global tableList

    currentTable = {}
    for file in scriptArgs.file_list:
        for line in file:
            index = 0
            tableName = re.match('\s*table\s+(\w+)', line)
            if tableName is not None: 
                currentTable = {"$name" : tableName.group(1)}
            
            parameter = re.search("{",line)
            if parameter is not None:
                line = line[parameter.span()[1]:]

            parameter = re.match("\s*(\w+)\s+(\w+)\s*;", line)
            if parameter is not None and parameter.groups().count > 2:
                currentTable[parameter.group(2)] = parameter.group(1)

            if re.search("}", line) is not None:
                tableList[ currentTable["$name"] ] = currentTable
                currentTable = {}
               
    for table in tableList.items():
        print ("table Name: %s" % table[1]["$name"])
        for item in table[1].items():
            if item[0] is not '$name':
                print('argument "%s" of type %s' % (item[0], item[1]))


if __name__=="__main__":
    main()