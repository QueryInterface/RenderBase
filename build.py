import sys
import traceback
import utils
from optparse import OptionParser

class Builder:
    def __init__(self):
        __genTypes = set()
        if (sys.platform eq 'win32'):
            __genTypes.add('Visual Studio 11')
            __genTypes.add('Sublime Text 2 - Unix Makefiles')
        if (sys.platform eq 'darwin'):
            __getTypes.add('Xcode')
            __getTypes.add('Unix Makefiles')
            __getTypes.add('Sublime Text 2 - Unix Makefiles')
        else:
            # Linux
            __getTypes.add('Unix Makefiles')
            __getTypes.add('Sublime Text 2 - Unix Makefiles')

    def ListTypes(self):
        for t in __genTypes:
            print t

    def Generate(self, genType):
        # Check if generation type is correct
        if not (genType in __genTypes):
            raise Exception('Invalid generation type: ' + genType)
        __curGenType = genType
        __outPath = './_out/' + __curGenType
        if not os.path.exists(__outPath) or not os.path.isdir(__outPath):
            os.makedirs(__outPath)
        os.chdir(__outPath)
        utils.RunProcess('cmake -G ' + __curGenType)

    def Build(self):
        print "INFO: Build is not implemented yet!"

    def Clean(self):
        print "INFO: Clean is not omplemented yet!"

    def __unpack(self):
        pass

    __curGenType
    __genTypes
    __outPath



def main():
    try:
        parser = OptionParser()
        parser.add_option("-t", "--type", help="type of output for cmake, for a list use --help_types", metavar="TYPE", dest="GenType")
        parser.add_option("-ht", "--help_types", help="lists generation type", dest="ListTypes")        
        parser.add_option("-b" "--build", help="build the prject", dest="Build")
        parser.add_option("-c", "--clean", help="clean project", dest="Clean")
        (options, args) = parser.parse_args()
        
        Builder builder
        if options.ListTypes:
            builder.ListTypes()

        if options.Build and options.Clean:
            print "ERROR: Can't use --build and --clean options at the same time"
            parser.print_help()
            return

        builder.Generate(options.GenType)
        if options.Build:
            builder.Build();
        if options.Clean:
            builder.Clean()
    except Exception as ex:
        print traceback.format_exc()

main()