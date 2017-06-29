#!/usr/bin/env python

import os
import re
import sys
import time
import subprocess
import logging

from optparse import OptionParser
p = OptionParser()

p.add_option('-o', '--output',    type  ='string',     default='add.root',   dest='output')
p.add_option('-k', '--file-key',  type  ='string',     default='out.*root',  dest='file_key')
p.add_option('-v', '--file-veto', type  ='string',     default='',           dest='veto')

p.add_option('--overwrite',    action='store_true', default=False, dest='overwrite')
p.add_option('--dry-run',      action='store_true', default=False, dest='dry_run')

(options,args) = p.parse_args()

#======================================================================================================        
def getLog(name, level='INFO', debug=False, print_time=False):

    if print_time:
        f = logging.Formatter('%(asctime)s %(name)s %(levelname)s %(message)s', datefmt='%H:%M:%S')
    else:
        f = logging.Formatter('%(name)s: %(levelname)s - %(message)s')
        
    sh = logging.StreamHandler(sys.stdout)
    sh.setFormatter(f)
    
    log = logging.getLogger(name)
    log.addHandler(sh)
    
    if debug:
        log.setLevel(logging.DEBUG)
    else:
        if level == 'DEBUG':   log.setLevel(logging.DEBUG)
        if level == 'INFO':    log.setLevel(logging.INFO)
        if level == 'WARNING': log.setLevel(logging.WARNING)    
        if level == 'ERROR':   log.setLevel(logging.ERROR)

    return log

#======================================================================================================        
log = getLog(os.path.basename(__file__), print_time=True)

#--------------------------------------------------------------------
def getRootFiles(path, veto = options.veto):
    
    if os.path.isfile(path):
        fname = os.path.basename(path)

        if options.file_key and re.search(options.file_key, fname) == None:
            return None
                
        if fname.count('.root'):
            if veto =='' or not fname.split('/')[len(fname.split('/'))-1].count(veto):
                return [path]

        return None

    files = []
    
    for f in os.listdir(path):
        if len(f) < 3:
            continue

        fpath = '%s/%s' %(path.rstrip('/'), f)
        flist = getRootFiles(fpath)
        
        if flist != None:
            files += flist

    return files

#--------------------------------------------------------------------
def processDir(path):
    
    timeStart = time.time()

    if not os.path.isdir(path):
        return

    fname = os.path.basename(path)    
    fpath = os.path.dirname(path).rstrip('/')
    apath = '%s/%s' %(path, options.output)

    if options.output.count('/'):
       apath = options.output 

    if os.path.isfile(apath):
        log.warning('processDir - output file already exists: %s' %apath)
        return
    
    files = getRootFiles(path)
    
    if len(files) < 1:
        log.warning('processDir - did not find any ROOT files at: %s' %path)
        return

    log.info('processDir - found %d ROOT file(s) at %s' %(len(files), path))

    for file in files:
        log.info('   %s' %file)
        
    commands = ['hadd', apath] + files

    if options.dry_run:
        commands = ['echo'] + commands
            
    process  = subprocess.Popen(commands, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    (stdout, stderr) = process.communicate()

    log.info('******************************************************************')
    log.info('Print output of hadd command: %s' %str(' ').join(commands))
    log.info('------------------------------------------------------------------')    
    if len(stderr.strip()): sys.stderr.write(stderr)
    if len(stdout.strip()): sys.stdout.write(stdout)
    log.info('------------------------------------------------------------------')    

    log.info('Directory: %s' %path)
    log.info('Total time: %.1fs' %(time.time()-timeStart))    

#--------------------------------------------------------------------
if __name__ == "__main__":

    # --------------------------------------
    # List files
    #
    
    for arg in args:
        if os.path.isdir(arg):
            processDir(arg.rstrip('/'))
        else:
            log.info('Skip non-directory: %s' %arg)
            


        
