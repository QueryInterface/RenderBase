import zipfile
import subprocess
import os
import tarfile
import sys

class TarModes:
    bz2 = 0
    tar_gz = 1

def Unzip(zipFilePath, destDir):
    zfile = zipfile.ZipFile(zipFilePath)
    for name in zfile.namelist():
        (dirName, fileName) = os.path.split(name)
        # directory
        newDir = os.path.join(destDir, dirName)
        if not os.path.exists(newDir):
            os.makedirs(newDir)
        if fileName:
            # file
            fd = open(os.path.join(destDir, name), 'wb')
            fd.write(zfile.read(name))
            fd.close()
    zfile.close()
    
def Untar(path, destDir, mode):
    mode_string = ""
    if mode == TarModes.bz2:
        mode_string = "r:bz2"
    if mode == TarModes.tar_gz:
        mode_string = "r:gz"
    absPath = os.path.abspath(path)

    tar = tarfile.open(absPath, mode_string)
    tar.extractall(destDir)
    tar.close()

def RunCmd(cmd, timeout=60):
    cwd = os.path.split(cmd[0])[0]
    if not os.path.exists(cwd):
        cwd = './'
    p = subprocess.Popen(cmd, stdout=subprocess.STDOUT, stderr=subprocess.STDOUT, cwd=cwd)
    t = sys.time.time()
    retcode = p.poll()
    isSuccess = False
    while retcode is None:
        retcode = p.poll()
        if sys.time.time() - t > timeout and retcode == None:
            print "[ERROR] Application timeout reached for cmd line:"
            print "\tFailed cmd {0}".format(cmd)
            p.kill()
            isSuccess = False
            break

    if retcode == 0:
        isSuccess = True
    return isSuccess
