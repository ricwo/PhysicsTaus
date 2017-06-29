#!/usr/bin/env python

import os
import subprocess
import sys
import socket
import time
import datetime
import getpass
import numpy

import PhysicsLight.PhysicsLightConfig as config

log = config.getLog('runPenn.py', print_time=True)

from optparse import OptionParser
p = OptionParser()

p.add_option('--input',      type='string',     default=None,  dest='input')
p.add_option('--macro',      type='string',     default=None,  dest='macro')
p.add_option('--arg',        type='string',     default=None,  dest='arg')
p.add_option('--ntry',       type='int',        default=10,    dest='ntry')
p.add_option('--nveto',      type='int',        default=10,    dest='nveto')
p.add_option('--nstop',      type='int',        default=200,   dest='nstop')
p.add_option('--jobid',      type='int',        default=None,  dest='jobid')
p.add_option('--wait-time',  type='int',        default=40,    dest='wait_time')

p.add_option('--overwrite',  action='store_true', default=False, dest='overwrite')
p.add_option('--dry-run',    action='store_true', default=False, dest='dry_run')

(options, args) = p.parse_args()

numpy.random.seed([hash('%d_%s_%s' %(options.jobid, socket.gethostname(), time.time()))])

#-----------------------------------------------------------------------------------------------------
def touch(fname, times=None):
    fhandle = open(fname, 'a')
    try:
        os.utime(fname, times)
    finally:
        fhandle.close()

#-----------------------------------------------------------------------------------------------------
def getVetoPath():

    keys = {}
    user = getpass.getuser()
    
    if user in keys:
        return keys[user]

    return ('/home/%s/veto' %user)
        
#-----------------------------------------------------------------------------------------------------
def getVeto():
    
    if not options.nveto:
        return None

    vpath = getVetoPath()

    if not os.path.isdir(vpath):
        log.error('getVeto - need to create veto directory: %s' %vpath)
        sys.exit(1)

    nattempt  = 0
    timeStart = time.time()
    
    while(True):
        nattempt += 1                        
        dfiles = os.listdir(vpath)
        
        if len(dfiles) < options.nveto:
            break
        else:
            log.info('getVeto - attempt #%d after waiting %.1fs' %(nattempt, time.time()-timeStart))
            time.sleep(0.1*numpy.random.poisson(10*options.wait_time))

        if nattempt >= options.nstop:
            log.error('getVeto - failed to obtain file lock after %d attempt(s)... stop job now' %nattempt)
            sys.exit(1)

    ctime = datetime.datetime.now()
    vtime = ctime.strftime('%Y-%m-%d_%H-%M-%S.%f')
    tpath = '%s/job_%03d_%s_%s.log' %(vpath.rstrip(), options.jobid, socket.gethostname(), vtime)
    
    log.info('Touch veto file: %s' %tpath)
    touch(tpath)

    return tpath
    
#-----------------------------------------------------------------------------------------------------
def copyFile(filepath, nfile, nattempt):

    filename = os.path.basename(filepath)

    if os.path.isfile('input/%s' %filename):
        raise Exception('copyFile - target file already exists %s - abort now' %filepath)

    if filepath.count('root://hn.at3f//srm'):
        commands = ['xrdcp', filepath, 'input/']        

    elif filepath.count('/srm/'):
        commands = ['xrdcp', 'root://hn.at3f/%s' %filepath, 'input/']  

    elif os.path.isfile(filepath):
        commands = ['cp', filepath, 'input/']
    else:
        raise Exception('copyFile - unknown file location: %s' %filepath)
    
    if options.dry_run:
        commands = ['echo'] + commands
    else:
        filename = 'input/%s' %os.path.basename(filepath)
        
        if os.path.isfile(filename):
            log.info('Delete local file: %s' %filename)
            os.remove(filename)
        
    copyStart = time.time()
    log.info('Copy command #%d:\n   %s' %(nattempt, str(' ').join(commands)))
    
    process = subprocess.Popen(commands, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    (stdout, stderr) = process.communicate()        
    
    if len(stderr.strip()):
        sys.stderr.write(stderr)
        sys.stderr.flush()
    if len(stdout.strip()):
        sys.stdout.write(stdout)
        sys.stdout.flush()

    log.info('Copy command #%d: return code=%d, timer=%.1fs' %(nattempt, process.returncode, time.time()-copyStart))
    
    return process.returncode

#-----------------------------------------------------------------------------------------------------        
if __name__ == "__main__":

    if not os.path.isfile(options.input):
        log.error('Need valid input file list')
        sys.exit(1)

    if not os.path.isfile(options.macro):
        log.error('Need valid input macro')
        sys.exit(1)

    timeStart = time.time()
    log.info('Start job at host: %s' %socket.gethostname())

    ifile = open(options.input, 'r')
    log.info('Input file: %s' %options.input)

    #
    # Copy input files to input sub-directory
    #
    vetoFile = getVeto()
    lines    = ifile.readlines()

    if len(lines) < 1:
        log.error('Input file list is empty')
        sys.exit(1)    

    if not os.path.isdir('input') and not options.dry_run:
        os.mkdir('input')
    
    for iline in range(0, len(lines)):
        line      = lines[iline]
        filepath  = line.rstrip()

        log.info('-------------------------------------------------------------------')
        for itry in range(0, options.ntry):
            returncode = copyFile(filepath, iline, itry)
        
            if returncode == 0:
                break
            else:
                log.info('Copy attempt #%d has failed... maximum number of attempts: %d' %(itry, options.ntry))
                time.sleep(0.1*numpy.random.poisson(10*options.wait_time))

    log.info('Time to copy %d file(s): %.1fs' %(len(lines), time.time()-timeStart))

    if vetoFile != None and os.path.isfile(vetoFile):
        log.info('Delete veto file: %s' %vetoFile)
        os.remove(vetoFile)
    
    #
    # Execute macro
    #
    commands = ['python',
                options.macro,
                'input/']

    if options.arg:
        for arg in options.arg.split():
            commands += [arg]
    
    if options.dry_run:
        commands = ['echo'] + commands

    log.info('----------------------------------------------------------------------')
    log.info('Execute macro:\n   %s' %(' '.join(commands)))
        
    process  = subprocess.Popen(commands, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    (stdout, stderr) = process.communicate()

    if process.returncode != 0:
        log.error('Macro existed with returncode=%d... delete local ROOT files' %process.returncode)

        for file in os.listdir('.'):
            if file.count('root'):
                log.info('Delete file: %s' %file)
                os.remove(file)

    log.info('-------------------------------------------------------------------')
    log.info('Print output of macro execution: returncode=%d' %process.returncode)
    log.info('-------------------------------------------------------------------')
    if len(stderr.strip()):
        sys.stderr.write(stderr)
        sys.stderr.flush()
    if len(stdout.strip()):
        sys.stdout.write(stdout)
        sys.stdout.flush()
    log.info('-------------------------------------------------------------------')
    log.info('Total job time: %.1fs' %(time.time()-timeStart))
