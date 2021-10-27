from subprocess import call
import sys

print 'Number of arguments:', len(sys.argv), 'arguments.'
print 'Argument List:', str(sys.argv)
args = (sys.argv[1])
#Or just:
#args = "bin/bar -c somefile.xml -d text.txt -r aString -f anotherString".split()
call(["./executable", args])
