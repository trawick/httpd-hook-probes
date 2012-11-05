import os
import string
import subprocess
import sys

source_files = ['*.c', '*.h']
meta_files = ['LICENSE', 'NOTICE', 'README']
bin_files = []

version = '0.1'

dirname = 'mod_hook_ar-%s' % (version)

if os.path.exists(dirname):
    raise Exception("%s should not already exist" % (dirname))

if os.path.exists(dirname + '.zip'):
    raise Exception("zip should not already exist")

os.mkdir(dirname, 0755)

cmd = ['cp', '-pR'] + source_files + meta_files + bin_files + [dirname]

print cmd

cmd = string.join(cmd)
print cmd

try:
    rc = subprocess.call(cmd, shell=True)
except:
    print "couldn't run, error", sys.exc_info()[0]
    raise

if rc != 0:
    print "rc:", rc
    sys.exit(1)

try:
    rc = subprocess.call(['zip', '-r', '%s.zip' % dirname, dirname])
except:
    print "couldn't run, error", sys.exc_info()[0]
    raise

if rc != 0:
    print "rc:", rc
    sys.exit(1)
