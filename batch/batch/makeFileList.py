import os
import re
import subprocess

import PhysicsLight.PhysicsLightConfig as config

log = config.getLog('makeFileList.py')

from optparse import OptionParser
p = OptionParser()

p.add_option('--input',     type  ='string',     default=None,  dest='input')
p.add_option('--output',    type  ='string',     default=None,  dest='output')

p.add_option('--overwrite',   action='store_true', default=False, dest='overwrite')
p.add_option('--print-files', action='store_true', default=False, dest='print_files')
p.add_option('--all',         action='store_true', default=False, dest='all')
p.add_option('--dry-run',     action='store_true', default=False, dest='dry_run')
p.add_option('--merge',       action='store_true', default=False, dest='merge')

(options, args) = p.parse_args()

#--------------------------------------------------------------------
def getFileList(path, addall):

    commands = ['xrd', 'hn.at3f', 'dirlist', path]

    if options.dry_run:
        commands = ['echo'] + commands

    log.info('getFileList - execute command:\n   %s' %(' '.join(commands)))
    
    process  = subprocess.Popen(commands, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    (stdout, stderr) = process.communicate()
    
    lines = stdout.rstrip('\n').split('\n')
    files = []
    
    for line in lines:
        file = line.rstrip('\n').rpartition('/srm/')[2]
        
        if len(file) and (addall or file.count('root')):
            files += ['/srm/%s' %file]

    log.info('getFileList - found %d file(s)\n' %len(files))
    
    return files

#--------------------------------------------------------------------
def getDirs(files):

    odict = {}
    
    for afile in files:
        file = os.path.basename(afile)
        path = os.path.dirname (afile).rpartition('.')[0]
        
        try:
            odict[path] += [file]
        except KeyError:
            odict[path]  = [file]

    return odict
                
#--------------------------------------------------------------------
def checkList(ofiles, ifiles):

    odict = getDirs(ofiles)
    idict = getDirs(ifiles)
    
    for dir, files in odict.iteritems():
        log.info('New dir: %s' %os.path.basename(dir))

        imiss  = 0
        imatch = 0

        if dir in idict:
            
            for file in files:
                if file in idict[dir]:
                    imatch += 1
                else:
                    imiss += 1

        log.info('   number of files - total: %s' %len(files))
        log.info('   number of files - new:   %s' %imiss)
        log.info('   number of files - exist: %s' %imatch)

    for dir, files in idict.iteritems():

        imiss  = 0
        imatch = 0

        if dir in odict:
            
            for file in files:
                if file in odict[dir]:
                    imatch += 1
                else:
                    imiss += 1

        if imatch == len(files):
            continue
        
        log.info('Old dir: %s' %os.path.basename(dir))
        log.info('   number of files - total: %s' %len(files))
        log.info('   number of files - miss:  %s' %imiss)
        log.info('   number of files - match: %s' %imatch)
        
#--------------------------------------------------------------------
if __name__ == '__main__':

    # --------------------------------------
    # List files
    #
    ofiles = []
    ifiles = []

    #
    # Scane xrootd directory and add selected ROOT files
    #
    for arg in args:
        dirs = getFileList(arg, addall=True)

        for dir in dirs:
            ofiles += getFileList(dir, options.all)

    #
    # Read file list from previous scans
    #
    if options.input:
        ifile = open(options.input, 'r')

        for line in ifile.readlines():
            ifiles += [line.rstrip('\n').strip()]

    #
    # Compare two lists of files
    #
    checkList(ofiles, ifiles)    
    
    #
    # Add new files to the list
    #
    if options.print_files:
        for file in ofiles:
            
            if file in ifiles:
                log.info('Inp file: %s' %file)
            else:
                log.info('New file: %s' %file)

    #
    # Save new list
    #
    if options.output:
        ofile = open(options.output, 'w')
        
        for file in ofiles:
            ofile.write('%s\n' %file)

        ofile.close()

    log.info('All is done - found %d file(s)' %len(ofiles))
