#!/usr/bin/env python

import os
import re
import sys
import time
import subprocess

import PhysicsLight.PhysicsLightConfig as config

log = config.getLog('addFiles')

from optparse import OptionParser
p = OptionParser()

p.add_option('-o', '--output',   type  ='string',     default='add.root',   dest='output')
p.add_option('-k', '--file-key', type  ='string',     default='out.*root',  dest='file_key')

p.add_option('--overwrite',    action='store_true', default=False, dest='overwrite')
p.add_option('--dry-run',      action='store_true', default=False, dest='dry_run')

(options,args) = p.parse_args()

#--------------------------------------------------------------------
class Data:
    def __init__(self, dsid):
        self.dsid = dsid
        self.jobs = []

#--------------------------------------------------------------------
def getDataDict(path):
    
    if os.path.isfile(path):
        log.warning('getDataDict - need directory path: %s' %path)
        return None

    dlist = {}
    
    for file in os.listdir(path):
        if len(file) < 3:
            continue

        if file.count('-') != 1 and file.count('.root') != 1:
            continue

        fpath = '%s/%s' %(path.rstrip('/'), file)
        
        fbase = file .partition('.root')[0]
        jobid = fbase.partition('-')[0]
        dsid  = fbase.partition('-')[0]

        print jobid
        print dsid

        if dsid in dlist:
            dlist[dsid] = Data(dsid)

        dlist[dsid].jobs += [jobid]

    log.info('getDataDict - found %d id(s)' %len(dlist))

    for id in sorted(dlist.iterkeys()):
        
    return dlist

#--------------------------------------------------------------------
def processDir(path):
    
    timeStart = time.time()

    if not os.path.isdir(path):
        return

    fname = os.path.basename(path)    
    fpath = os.path.dirname(path).rstrip('/')
    apath = '%s/%s' %(path, options.output)

    if os.path.isfile(apath):
        log.warning('processDir - output file already exists: %s' %apath)
        return
    
    files = getDataDict(path)

    return
    
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
            


        
