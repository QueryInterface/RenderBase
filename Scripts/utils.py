import sys
import zipfile
import subprocess
import os
import tarfile

def Unzip(zipFilePath, destDir):
    zfile = zipfile.ZipFile(zipFilePath)
    for name in zfile.namelist():
        (dirName, fileName) = os.path.split(name)
        if fileName == '':
            # directory
            newDir = destDir + '/' + dirName
            if not os.path.exists(newDir):
                os.mkdir(newDir)
        else:
            # file
            fd = open(destDir + '/' + name, 'wb')
            fd.write(zfile.read(name))
            fd.close()
    zfile.close()

def Untar(path, destDir):
    absPath = os.path.abspath(path)
    savedir = os.getcwd()
    os.chdir(destDir)

    tar = tarfile.open(absPath, "r:bz2")
    tar.extractall()
    tar.close()
    
    os.chdir(savedir)

def RunCmd(cmd, timeout = 60):
    cwd = os.path.split(cmd[0])[0]
    if not os.path.exists(cwd):
        cwd = './' 
    p = subprocess.Popen(cmd, stdout=subprocess.STDOUT, stderr=subprocess.STDOUT, cwd=cwd)
    t = time.time()
    strStdOut = []
    retcode = p.poll()
    isSuccess = False
    while retcode == None:
        retcode = p.poll()
        if time.time() - t > timeout and retcode == None:
            print "[ERROR] Application timeout reached for cmd line:"
            print "\tFailed cmd {0}".format(cmd)
            p.kill()
            isSuccess = False
            break

    if retcode == 0: 
        isSuccess = True
    return isSuccess

