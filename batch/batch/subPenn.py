#!/usr/bin/env python

import os
import re
import sys
import subprocess
import time
import getpass

import PhysicsLight.PhysicsLightConfig as config

log = config.getLog('subPenn.py', print_time=False)

from optparse import OptionParser
p = OptionParser()

p.add_option('--output', '-o',  type='string',  default=None,              dest='output')
p.add_option('--macro',         type='string',  default=None,              dest='macro')
p.add_option('--file-list',     type='string',  default=None,              dest='file_list')
p.add_option('--input-files',   type='string',  default=None,              dest='input_files')
p.add_option('--arg',           type='string',  default='',                dest='arg')

p.add_option('--njob', '-n',    type='int',     default=0,     dest='njob')
p.add_option('--nveto',         type='int',     default=10,    dest='nveto')
p.add_option('--nfile-per-job', type='int',     default=0,     dest='nfile_per_job')
p.add_option('--mb-per-job',    type='int',     default=1000,  dest='mb_per_job')
p.add_option('--nprint',        type='int',     default=50,    dest='nprint')

p.add_option('-d', '--debug',     action='store_true', default=False, dest='debug')
p.add_option('--overwrite',       action='store_true', default=False, dest='overwrite')
p.add_option('--overwrite-root',  action='store_true', default=False, dest='overwrite_root')
p.add_option('--make-job-dir',    action='store_true', default=False, dest='make_job_dir')
p.add_option('--do-ntuple',       action='store_true', default=False, dest='do_ntuple')
p.add_option('--dry-run',         action='store_true', default=False, dest='dry_run')
p.add_option('--submit', '-s',    action='store_true', default=False, dest='submit')

(options, args) = p.parse_args()

out_files = []

#-----------------------------------------------------------------------------------------------------
class File:
    
    def __init__(self, path, size):
        
        self.path = path
        self.size = size

        if self.size == None:
            if not os.path.isfile(path):
                raise Exception('Missing file: %s' %path)
      
            if os.path.isfile(path):
                self.size = os.path.getsize(path)
            else:
                raise Exception('Uknown file path and size: %s %s' %(path, size))

#-----------------------------------------------------------------------------------------------------
def makeVetoPath():

    user = getpass.getuser()    
    path = '%s/veto' %os.path.expanduser('~')

    if not os.path.isdir(path):
        log.info('makeVeto - will create veto directory: %s' %path)
        os.makedirs(path)
        return

    files = os.listdir(path)
    
    if len(files) > 0:
        log.info('Veto directory is not empty: %s' %path)
        log.info('OK if other jobs are running, otherwise please cleanup old veto files')

#-----------------------------------------------------------------------------------------------------        
def prepareJobHeader(output):
    
    if output == None:
        log.error('Need output option - path to output directory')
        sys.exit(1)

    if not os.path.isdir(output):
        os.makedirs(output)

    #
    # Prepare condor submission file
    #
    testArea = os.environ.get('TestArea')    
    jobExe   = '%s/PhysicsNtuple/PhysicsLight/macros/batch/runPenn.py' %testArea
    jobMacro = options.macro
        
    if jobMacro == None:
        jobMacro = '%s/PhysicsNtuple/PhysicsLight/macros/makeNtuple.py' %testArea

    if not os.path.isfile(jobExe):
        log.error('Invalid job executable: %s' %jobExe)
        sys.exit(1)
        
    if not os.path.isfile(jobMacro):
        log.error('Invalid job macro: %s' %jobMacro)
        sys.exit(1)
        
    jobPath  = '%s' %(output.rstrip('/'))
    jobDesc  = ''
    jobDesc += 'universe                = vanilla\n'
    jobDesc += 'getenv                  = True\n'
    jobDesc += 'should_transfer_files   = YES\n'
    jobDesc += 'when_to_transfer_output = ON_EXIT_OR_EVICT\n'
    jobDesc += 'executable              = %s\n' %jobExe
    jobDesc += '\n'

    return  jobDesc, jobPath, jobMacro

#-----------------------------------------------------------------------------------------------------        
def prepareFilesFromDir(input_dir):

    files = []

    if os.path.islink(input_dir):
        input_new = os.readlink(input_dir)
        
        log.info('Resolve ')

    if not os.path.isdir(input_dir):
        log.warning('prepareFilesFromDir - input path is not directory: %s' %input_dir)
        return []

    log.info('Process files from: %s' %input_dir)

    #
    # Select input files 
    #
    for entry in os.listdir(input_dir):
        
        if re.search('root', entry) == None:
            continue

        fpath = '%s/%s' %(input_dir.rstrip('/'), entry)

        if os.path.isfile(fpath):
            f = File(fpath, None)
            files += [f]

    return files

#-----------------------------------------------------------------------------------------------------        
def prepareFilesFromTxt(file_list):

    files = []

    if not os.path.isfile(file_list):
        raise Exception('Input file path is not valid: %s' %file_list)

    ifile = open(file_list, 'r')

    log.info('Read file list from: %s' %file_list)

    #
    # Select input files 
    #
    for line in ifile.readlines():
        ifile = line.rstrip()
        
        if re.search('root://hn.at3f', ifile) == None:
            f = File(ifile, None)            
        else:
            parts = ifile.partition(' ')
            f     = File(parts[2], int(parts[0]))

        files += [f]

    return files

#-----------------------------------------------------------------------------------------------------        
def makeNewJob(curFiles, curSize):

    if (options.nfile_per_job > 0 and len(curFiles) >= options.nfile_per_job) or (options.mb_per_job > 0 and curSize > options.mb_per_job*1048576):
        return True

    return False

#-----------------------------------------------------------------------------------------------------        
def getOutFile(jobKey, ifiles):

    if True:
        return 'out_%s.root' %jobKey

    if len(ifiles) != 1:
        raise Exception('getOutFile - syst option requires 1 input file instead of %s' %len(ifiles))

    ifile = os.path.basename(ifiles[0])

    global out_files

    if ifile in out_files:
        raise Exception('getOutFile - duplicate file: %s' %len(ifile))

    out_files += [ifile]

    return 'out_%s' %ifile

#-----------------------------------------------------------------------------------------------------        
def prepareJob(files, output):

    if len(files) == 0:
        log.warning('processJob - zero input files... nothing to do')
        return

    log.info('Number of selected files: %d' %len(files))
    
    #
    # Make input file list for each job
    #
    jobFiles = []
    curFiles = []
    curSize  = 0

    for ifile in files:        
        log.debug('%s: %s' %(ifile.path, ifile.size))

        curFiles += [ifile.path]
        curSize  +=  ifile.size
            
        if makeNewJob(curFiles, curSize):
                                                          
            jobFiles += [curFiles]

            curSize  = 0
            curFiles = []

    if len(curFiles):            
        jobFiles += [curFiles]

    if len(jobFiles) == 0:
        log.info('No valid files available to build jobs... nothing to do')
        sys.exit(0)
        
    #
    # Prepare job description header and path
    #
    log.info('Number of prepared jobs: %d' %len(jobFiles))
    
    jobDesc, jobPath, jobMacro = prepareJobHeader(output)
    jobRun = '%s/condor.run'  %jobPath

    if not options.overwrite and os.path.isfile(jobRun):
        log.error('Job condor file already exists: %s' %jobRun)
        sys.exit(1)

    #
    # Create jobs
    #
    countJob = 0
    
    for ijob in range(0, len(jobFiles)):
        curFiles = jobFiles[ijob]
        
        #
        # Make job output directory and write job's input list of files
        #
        jobKey = '%04d' %ijob
        jobNtp = 'ntp_%s.root' %jobKey        
        jobXml = 'run_%s.xml'  %jobKey
        jobOut = getOutFile(jobKey, curFiles)

        if options.make_job_dir:
            jobDir = '%s/job_%s' %(jobPath, jobKey)
        else:
            jobDir = '%s'        %(jobPath)

        if os.path.isdir(jobDir):
            checkFile = '%s/%s' %(jobDir, jobOut)

            if not options.overwrite_root and os.path.isfile(checkFile):
                log.warning('Skip job with existing output file: %s' %checkFile)
                continue
                
        else:
            os.makedirs(jobDir)

        jobInputPath = '%s/job_%s.input' % (jobDir, jobKey)
        if not options.overwrite and os.path.isfile(jobInputPath):
            log.warning('Job input file already exists: %s' %jobInputPath)
            continue
        
        jobInputFile = open(jobInputPath, 'w')        
            
        for f in curFiles:
            jobInputFile.write('%s\n' %f)

        jobInputFile.close()

        #
        # Configure command line options for macro executed by batch system
        #
        runArgs = '%s -o %s --run-xml=%s' %(options.arg, jobOut, jobXml)

        if options.do_ntuple:
            runArgs += ' --ntuple-file=%s' %(jobNtp)

        #
        # Configure job 
        #
        jobArgs  = '--input=%s --macro=%s --jobid=%d --nveto=%d --arg=\'%s\'' %(jobInputPath, jobMacro, ijob, options.nveto, runArgs)

        if options.dry_run:
            jobArgs += ' --dry-run'

        jobDesc += 'initialdir = %s\n'         %jobDir
        jobDesc += 'arguments  = "%s"\n'       %jobArgs
        jobDesc += 'output     = job_%s.out\n' %jobKey
        jobDesc += 'error      = job_%s.err\n' %jobKey
        jobDesc += 'queue\n\n'

        countJob += 1
        
        if options.njob > 0 and ijob+1 >= options.njob:
            break

        if ijob % options.nprint == 0:
            log.info('processing job %4d: %s' %(ijob, time.asctime()))
 
    #
    # Write condor job file
    #    
    jobFile = open(jobRun, 'w')
    jobFile.write(jobDesc)
    jobFile.close()

    #
    # Make veto directory
    #
    makeVetoPath()

    #
    # Submit condor job
    #
    commands = ['condor_submit', jobRun]

    if not options.submit:
        commands = ['echo'] + commands

    log.info('-------------------------------------------------------------------')
    log.info('Prepared %d job(s) - condor file:\n   %s' %(countJob, jobRun))
    
    process  = subprocess.Popen(commands, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    (stdout, stderr) = process.communicate()

    log.info('Print output of job submission command:')
    log.info('-------------------------------------------------------------------')
    if len(stderr.strip()): sys.stderr.write(stderr)
    if len(stdout.strip()): sys.stdout.write(stdout)    
    log.info('-------------------------------------------------------------------')    

    log.info('Total jobs: %s'   %len(jobFiles))
    log.info('Total time: %.1f' %(time.time()-tbeg))
    log.info('Local time: %s'   %time.asctime())

#-----------------------------------------------------------------------------------------------------        
if __name__ == "__main__":

    if options.arg.count('-o '):
        log.error('Jobs can use common output directory... arg should not contain "-o": arg="%s"' %options.arg)
        sys.exit(1)

    tbeg  = time.time()
    files = []

    if options.input_files != None and os.path.isfile(options.input_files):
        files = prepareFilesFromTxt(options.input_files)
        
        prepareJob(files, options.output)

    elif len(args):
        for arg in args:
            files = prepareFilesFromDir(arg)
            
            prepareJob(files, '%s/%s' %(options.output, os.path.basename(arg.rstrip('/'))))
    else:
        log.error('Missing or invalid option: --input-files=%s' %options.input_files)
