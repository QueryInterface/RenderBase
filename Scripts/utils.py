import os
import sys
import subprocess

def RunProcess(command):
    p = subprocess.Popen(command, stderr=subprocess.PIPE, stdout=subprocess.PIPE)
    while True:
        out = p.stdout.read(1)
        err = p.stdout.read(1)
        if out == '' and err == '' and p.poll() != None:
            break
        if out != '':
            sys.stdout.write(out)
            sys.stdout.flush()
        if err != '':
            sys.stdout.write(err)
            sys.stdout.flush()

def DirectoryExists(path):
    return os.path.isdir(path)

def CreateDirectory(path)
    os.makedirs(path)

def Zip():
    return

def Unzip():
    return

