import os
import sys
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-p", "--path", help="directory", dest="Path")
parser.add_option("-t", "--type", help="type of files to remove", dest="Type")
(options, args) = parser.parse_args()
if not options.Path or not options.Type:
    parser.print_help()
    sys.exit()

for d, dirs, files in os.walk(options.Path):
    for f in files:
        fileName, fileExtension = os.path.splitext(f)
        if fileExtension == ("." + options.Type):
            filePath = os.path.join(d, f)
            print "Removing file " + filePath
            os.remove(filePath)
