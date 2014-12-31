#!/usr/bin/python

import sys 
import os
import argparse

def main():
    parser = argparse.ArgumentParser(description = 'C++ to Lua converter v0.1')
    parser.add_argument('-fl',  '--file_list', nargs = '+', required=True, type = argparse.FileType('r'), help = 'list of files to process')
    scriptArgs = parser.parse_args()

    for file in scriptArgs.file_list:
        for line in file:
            print(line)
    


if __name__=="__main__":
    main()