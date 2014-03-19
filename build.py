import sys
import os
import traceback
import glob
from optparse import OptionParser
sys.path.append("./Scripts")
import utils
from subprocess import call
import ntpath
import shutil

class Builder:
    def __init__(self):
        self.__genTypes = set()
        if (sys.platform == 'win32'):
            self.__genTypes.add('Visual Studio 11')
            self.__genTypes.add('Sublime Text 2 - Unix Makefiles')
        if (sys.platform == 'darwin'):
            self.__genTypes.add('Xcode')
            self.__genTypes.add('Unix Makefiles')
            self.__genTypes.add('Sublime Text 2 - Unix Makefiles')
        else:
            # Linux
            self.__genTypes.add('Unix Makefiles')
            self.__genTypes.add('Sublime Text 2 - Unix Makefiles')
        print 

    def ListTypes(self):
        for t in self.__genTypes:
            print t

    def Generate(self, genType):
        # Check if generation type is correct
        if not (genType in self.__genTypes):
            raise Exception('Invalid generation type: ' + genType)
        self.__curGenType = genType
        print "Generating project " + self.__curGenType + "..."
        self.__outPath = './_build/' + self.__curGenType
        if not os.path.exists(self.__outPath) or not os.path.isdir(self.__outPath):
            os.makedirs(self.__outPath)
        oldWorkDir = os.getcwd()
        os.chdir(self.__outPath)
        call(['cmake', '-G', self.__curGenType, '../../'])
        os.chdir(oldWorkDir)
        print "Generating project " + self.__curGenType + "...DONE"
        self.__unpack('qt')

    def Build(self):
        print "INFO: Build is not implemented yet!"

    def Clean(self):
        shutil.rmtree("./_build")
        shutil.rmtree("./3rdParty/_unpack")

    def __unpack(self, module):
        # Unpack qt
        print "Unpacking " + module + "..."
        srcPath = "./3rdParty/qt/"
        dstPath = "./3rdParty/_unpack/" + module + "/"
        if not os.path.exists(dstPath) or not os.path.isdir(dstPath) or not os.listdir(dstPath):
            # Extract zip files
            files = glob.glob(srcPath + "*.zip")
            for f in files:
                basename = ntpath.basename(f)
                basename = os.path.splitext(basename)[0]
                if not os.path.exists(dstPath) or not os.path.isdir(dstPath):
                    os.makedirs(dstPath)
                utils.Unzip(f, dstPath)
            # Extract tar files
            files = glob.glob(srcPath + "*.bz2")
            for f in files:
                basename = ntpath.basename(f)
                basename = os.path.splitext(basename)[0]
                dstPathForBz2 = dstPath + basename
                if not os.path.exists(dstPathForBz2) or not os.path.isdir(dstPathForBz2):
                    os.makedirs(dstPathForBz2)
                utils.Untar(f, dstPathForBz2)

        print "Unpacking " + module + "...DONE"

    __curGenType = ''
    __genTypes = []
    __outPath = ''



def main():
    try:
        parser = OptionParser()
        parser.add_option("-t", "--type", help="type of output for cmake, for a list use -l or --list_types", metavar="TYPE", dest="GenType")
        parser.add_option("-l", "--list_types", help="lists generation type", dest="ListTypes", default=False, action="store_true")        
        parser.add_option("-b", "--build", help="build the prject", dest="Build", default=False, action="store_true")
        parser.add_option("-c", "--clean", help="clean project", dest="Clean", default=False, action="store_true")
        (options, args) = parser.parse_args()
        
        builder = Builder()
        if options.ListTypes:
            builder.ListTypes()
            return

        if not options.GenType:
            parser.print_help()

        if options.Build and options.Clean:
            print "ERROR: Can't use --build and --clean options at the same time"
            parser.print_help()
            return

        if options.GenType:
            builder.Generate(options.GenType)
        if options.Build:
            builder.Build();
        if options.Clean:
            builder.Clean()
    except Exception as ex:
        print traceback.format_exc()

main()