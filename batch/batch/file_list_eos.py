#
# Invokes the XMLRPC request and parses the output.
# Produces list of files for a given dataset pattern. 
#

import os
import re
import sys
import optparse
import subprocess
import logging

p = optparse.OptionParser()

p.add_option('--datasets',       type=str, default=None)
p.add_option('--ds-name',        type=str, default=None)
p.add_option('--outfile',  '-o', type=str, default='list.txt')
p.add_option('--pattern',        type=str, default='tthNTUP.root')
p.add_option('--disk',           type=str, default='CERN-PROD_PHYS-HIGGS')
p.add_option('--level', '-l',    type=str, default='INFO')

p.add_option('--nfile', '-n',    type=int, default=0)

(options, args) = p.parse_args()

#=====================================================================
def getLog(name):

    f = logging.Formatter('%(name)s: %(levelname)s - %(message)s')        
    h = logging.StreamHandler(sys.stdout)
    h.setFormatter(f)

    log = logging.getLogger(name)
    log.addHandler(h)
    log.setLevel(getattr(logging, options.level))

    return log

log = getLog(os.path.basename(__file__))

#=====================================================================
def getPathCern(efile):

    ecmd  = ['/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select', 'ls', '-l', efile]

    print efile
    
    process = subprocess.Popen(ecmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    (stdout, stderr) = process.communicate()

    if process.returncode != 0:
        raise Exception('getPathCern - eos command failed for %s' %efile)

    lines = stdout.split('\n')
    paths = []

    for line in lines:
        parts = line.split()
        
        if len(parts) < 4:
            continue

        path = parts[-1]

        if int(parts[2]) > 2:
            paths += [path]
            
        log.info('%s' %path)

    return paths

#=====================================================================
def getFileCern(efile):

    ecmd  = ['/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select', 'ls', '-l', efile]
    
    log.info(' - path: %s' %efile)
    
    process = subprocess.Popen(ecmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    (stdout, stderr) = process.communicate()

    lines = stdout.split('\n')
    files = []

    for line in lines:
        parts = line.split()
        
        if len(parts) < 4:
            continue

        fname = parts[-1]
        ftype = int(parts[1])
        fsize = int(parts[4])
        fpath = '%s/%s' %(efile.rstrip('/'), fname)

        if ftype == 2:
            files += [(fsize, fpath)]
        else:
            getFileCern(fpath)

    if len(files) == 0:
        log.info('getFileCern - zero files at: %s' %efile)
        log.info('getFileCern - if path is correct, try setting "EOS_MGM_URL=root://eospublic/"')

    return files

#=====================================================================
def getFileLocal(efile):

    if not os.path.isdir(efile):
        log.warning('getFileLocal - directory does not exist: %s' %efile)
        return []
    
    log.info(' - path: %s' %efile)
    
    files = []

    for line in sorted(os.listdir(efile)):
        fname = line.rstrip('\n')
        
        if fname.count('.root') == 0:
            continue

        fpath = '%s/%s' %(efile.rstrip('/'), fname)
        fsize = os.path.getsize(fpath)

        files += [(fsize, fpath)]

    return files

#=====================================================================
if __name__ == '__main__':

    files = []

    for arg in args:
        #
        # Prepare file list with size
        #
        if os.path.isdir(arg):
            files += getFileLocal(arg)
        else:
            files += getFileCern(arg)

    total_size = 0

    outputFile  = open(options.outfile,  'w')
        
    for (fsize, fpath) in files:
        total_size += fsize        
        log.info('%s  %.1f MB' %(fpath, fsize/1048576.0))
        
        outputFile.write('%12s %s\n' %(fsize, fpath))

    log.info('Total files: %d' %(len(files)))
    log.info('Total size: %.1f MB' %(total_size/1048576.0))
                
    outputFile.close()
