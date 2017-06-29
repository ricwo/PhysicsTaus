#!/usr/bin/env python

import os
import subprocess
import sys
import socket
import time
import logging

from optparse import OptionParser
p = OptionParser()

p.add_option('--input',      type='string',     default=None,   dest='input')
p.add_option('--macro',      type='string',     default=None,   dest='macro')
p.add_option('--outdir',     type='string',     default=None,   dest='outdir')
p.add_option('--eosout',     type='string',     default=None,   dest='eosout')
p.add_option('--arg',        type='string',     default=None,   dest='arg')
p.add_option('--cluster',    type='string',     default='CERN', dest='cluster')

p.add_option('--ntry',       type='int',        default=10,    dest='ntry')
p.add_option('--nstop',      type='int',        default=200,   dest='nstop')
p.add_option('--jobid',      type='int',        default=None,  dest='jobid')
p.add_option('--wait-time',  type='int',        default=30,    dest='wait_time')

p.add_option('--overwrite',  action='store_true', default=False, dest='overwrite')
p.add_option('--dry-run',    action='store_true', default=False, dest='dry_run')

p.add_option('--nfile-per-job',  type='int',        default=0,    dest='nfile_per_job')

(options,args) = p.parse_args()

#======================================================================================================        
def getLog(name, level='INFO', debug=False, print_time=False):

    if print_time:
        f = logging.Formatter('%(asctime)s %(name)s %(levelname)s %(message)s', datefmt='%H:%M:%S')
    else:
        f = logging.Formatter('%(name)s: %(levelname)s - %(message)s')
        
    sh = logging.StreamHandler(sys.stdout)
    sh.setFormatter(f)
    
    fh = logging.FileHandler('job.log')
    fh.setFormatter(f)

    log = logging.getLogger(name)
    log.addHandler(sh)
    log.addHandler(fh)
    
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

#======================================================================================================        
def copyFile(filepath, nfile, nattempt):

    tmpdir = getTmpDir()
    target = '%s/input/%s' %(tmpdir, filepath.replace('/', '_'))

    if os.path.isfile(target):
        log.error('copyFile - target copy file already exists... abort now')
        log.error('copyFile - input:  %s' %filepath)
        log.error('copyFile - target: %s' %target)
        raise Exception('copyFile - problem with destination')

    if filepath.count('/eos/'):
        commands = ['xrdcp', '--nopbar', getFullPathEOS(filepath), target]

    elif os.path.isfile(filepath):
        if options.cluster == 'Bonn':
            commands = ['ln', '-s', filepath, target]
        else:
            commands = ['cp', filepath, target]
    else:
        log.error('copyFile - Input %s is not on EOS and is not a local file.' % filepath)
        raise Exception('copyFile - problem with source')

    if options.dry_run:
        commands = ['echo'] + commands
        
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

#======================================================================================================        
def runCommand(commands, do_exit=True):

    if options.dry_run:
        commands = ['echo'] + commands

    log.info('============================================================================================')
    log.info('runCommand - execute command: "%s"' %(' '.join(commands)))

    process = subprocess.Popen(commands, stdout=subprocess.PIPE, stderr=subprocess.PIPE)    
    (stdout, stderr) = process.communicate()    
        
    sys.stdout.write(stdout)       
    sys.stderr.write(stderr)

    log.info('runCommand: return code=%s' %(process.returncode))
    log.info('============================================================================================')

    if do_exit and process.returncode != 0:
        log.error('runCommand - exit job because bad return code')
        sys.exit(process.returncode)

#======================================================================================================        
def printOut(target, value):
    if len(value.strip()):
        target.write(value)
        target.flush()

#======================================================================================================        
def getFullPathEOS(ipath):

    if ipath.startswith('root://eos'):
        return ipath
    if ipath.count('/eos/escience'):
        return 'root://eospublic/%s' %ipath
    elif ipath.count('/eos/atlas'):
        return 'root://eosatlas/%s' %ipath

    raise Exception('getFullPathEOS - unknown EOS location: ' %ipath)

#======================================================================================================        
def getTmpDir():
    
    if options.cluster == 'Bonn':
        tmpdir = '/tmp/%s' %os.getenv('PBS_JOBID')
        
        if not os.path.isdir(tmpdir):
            os.mkdir(tmpdir)
    else:
        tmpdir = os.getenv('PWD')
        
    return tmpdir

#======================================================================================================        
def copyInputFiles(timeStart, tmpdir):
    ''' Copy input files to $PWD/input sub-directory '''

    ifile  = open(options.input, 'r')
    lines  = ifile.readlines()

    if len(lines) < 1:
        log.error('Input file list is empty: %s' %options.input)
        sys.exit(1)    

    if not os.path.isdir(tmpdir+'/input') and not options.dry_run:
        os.mkdir(tmpdir+'/input')

    lastfile = ''
        
    for iline in range(0, len(lines)):
        line      = lines[iline]
        filepath  = line.rstrip()
        lastfile  = filepath

        log.info('-------------------------------------------------------------------')
        for itry in range(0, options.ntry):
            returncode = copyFile(filepath, iline, itry)
        
            if returncode == 0:
                break
            else:
                log.info('Copy attempt #%d has failed... maximum number of attempts: %d' %(itry, options.ntry))
                time.sleep(options.wait_time)
          
        if returncode != 0:
            log.error('Maximum number of copy attempts reached. Problem copying file %s' % filepath)
            sys.exit(1)

    log.info('Time to copy %d file(s): %.1fs' %(len(lines), time.time()-timeStart))
    return lastfile

#======================================================================================================        
if __name__ == '__main__':

    timeStart = time.time()
    log.info('Start job %s at %s:%s' %(options.jobid, socket.gethostname(), os.getcwd()))
    log.info('Current time: %s' %(time.asctime(time.localtime())))
    log.info('Input files: %s' %options.input)
    
    if not os.path.isfile(options.input):
        log.error('Need valid input file list')
        sys.exit(1)

    if not os.path.isfile(options.macro):
        log.error('Need valid input macro: %s' %options.macro)
        sys.exit(1)

    if not options.outdir or not os.path.isdir(options.outdir):
        log.error('Need valid outdir: %s' %options.outdir)
        sys.exit(1)

    if options.jobid == None:
        log.error('Need valid job id: %s' %options.jobid)
        sys.exit(1)

    jobKey = 'job_%04d'  %options.jobid
    tmpdir = getTmpDir()
    
    #
    # Copy input files
    #
    lastfile = copyInputFiles(timeStart, tmpdir)

    #
    # Execute macro
    #
    commands = ['python',
                options.macro,
                tmpdir+'/input/',
                '--run-xml=run_%s.xml' %jobKey]

    os.chdir(tmpdir) 
    
    if options.arg:
        if options.arg.count('--run-xml'):
            log.error('Invalid argument: %s' %options.arg)
            sys.exit(1)

        for arg in options.arg.split():
            commands += [arg]
    
    if options.dry_run:
        commands = ['echo'] + commands

    log.info('============================================================================================')
    log.info('Execute user command:\n   %s' %(' '.join(commands)))
    log.info('--------------------------------------------------------------------------------------------')

    process = subprocess.Popen(commands, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, bufsize=0)

    #
    # Record output without buffering - ensures output even if job is killed
    #
    for line in process.stdout:
        log.info(line.rstrip('\n'))

    process.wait()

    log.info('--------------------------------------------------------------------------------------------')
    log.info('%s: returncode=%d' %(options.macro, process.returncode))
    log.info('============================================================================================')

    if process.returncode != 0:
        log.error('Macro return code=%d... exit now and do not copy output files' %(process.returncode))
        sys.exit(process.returncode)

    rootfiles = []

    for rootfile in os.listdir('.'):
        if rootfile.count('.root'): 
            rootfiles += [rootfile]

    log.info('Copy %d output ROOT files to %s' %(len(rootfiles), options.outdir))

    for rootfile in rootfiles:
        target = '%s_%s' %(jobKey, rootfile)
        if options.nfile_per_job == 1:
            target = lastfile.split('/')[-1]
            
        if options.eosout:
            commands = ['xrdcp','--nopbar', rootfile, getFullPathEOS('%s/%s' %(options.eosout, target))]
                    
        elif os.path.isdir(options.outdir):
            commands = ['mv', rootfile, '%s/%s' %(options.outdir, target)]

        else:
            log.warning('No output path specified - ignore file: %s' %rootfile)
            continue

        log.info('Copying %s -> %s' %(rootfile, target))            
        runCommand(commands)

    log.info('Job %s - all is done' %jobKey)
    log.info('Total job time: %.1fs' %(time.time()-timeStart))
