#!/usr/bin/env python

import os
import sys
import subprocess
import time
import getpass

import PhysicsLight.PhysicsLightConfig as config

log = config.getLog('subBonn.py', print_time=False)

from optparse import OptionParser
p = OptionParser()

p.add_option('--output', '-o',  type='string',  default=None,              dest='output')
p.add_option('--macro',         type='string',  default=None,              dest='macro')
p.add_option('--file-list',     type='string',  default=None,              dest='file_list')
p.add_option('--input-files',   type='string',  default=None,              dest='input_files')
p.add_option('--arg',           type='string',  default='',                dest='arg')
p.add_option('--queue',         type='string',  default='short',             dest='queue')

p.add_option('--njob',          type='int',     default=0,     dest='njob')
p.add_option('--nfile-per-job', type='int',     default=0,     dest='nfile_per_job')
p.add_option('--mb-per-job',    type='int',     default=1000,  dest='mb_per_job')
p.add_option('--nprint',        type='int',     default=50,    dest='nprint')

p.add_option('-d', '--debug',     action='store_true', default=False, dest='debug')
p.add_option('--overwrite',       action='store_true', default=False, dest='overwrite')
p.add_option('--overwrite-root',  action='store_true', default=False, dest='overwrite_root')
p.add_option('--make-job-dir',    action='store_true', default=False, dest='make_job_dir')
p.add_option('--do-ntuple',       action='store_true', default=False, dest='do_ntuple')
p.add_option('--do-tp',           action='store_true', default=False, dest='do_tp')
p.add_option('--dry-run',         action='store_true', default=False, dest='dry_run')
p.add_option('--submit',          action='store_true', default=False, dest='submit')

(options, args) = p.parse_args()

#-----------------------------------------------------------------------------------------------------
def makeVetoPath():

    user = getpass.getuser()    
    path = '/home/%s/veto' %user

    if not os.path.isdir(path):
        log.info('makeVeto - will create veto directory: %s' %path)
        os.makedirs(path)
        return

    files = os.listdir(path)
    
    if len(files) > 0:
        log.info('Veto directory is not empty: %s' %path)
        log.info('OK if other jobs are running, otherwise please cleanup old veto files')

#-----------------------------------------------------------------------------------------------------        
def checkDSID(dmap):

    all_dsid = set()

    for i, ids in dmap.iteritems():
        print '%d: %s' %(i, ids)

        for id in ids:
            all_dsid.add(id)

    for id in all_dsid:
        count = 0

        for i, ids in dmap.iteritems():
            for id_ in ids:

                if id == id_:
                    count += 1
                    
        print '%2d %s' %(count, id)


#-----------------------------------------------------------------------------------------------------        
def createJobScript(jobExe, jobOpts, outDir,jobKey):
    
    testarea = os.getenv('TestArea')
    setupScript = testarea + '/setup_pro.sh' 
    jobScript  = '%s/job_%s.sh'  % (outDir, jobKey)
    logOutname = '%s/job_%s.out' % (outDir, jobKey)

    file = open(jobScript, 'w') 
    file.write('#!/bin/sh\n')
    file.write('#PBS -j oe\n')
    file.write('#PBS -o %s\n' % logOutname)
    file.write('#PBS -l mem=8GB\n')
    file.write('source ' + setupScript + '\n')
    file.write(' '.join(['python',jobExe,jobOpts]))
    file.close() 

    return jobScript 
#-----------------------------------------------------------------------------------------------------        
def prepareFilesFromTxt(file_list):

    files = []

    if not os.path.isfile(file_list):
        log.warning('Input file path is not valid: %s' %file_list)
        return files

    ifile = open(file_list, 'r')

    log.info('Read file list from: %s' %file_list)

    #
    # Select input files 
    #
    for line in ifile.readlines():
        file   = line.rstrip()
        files += [file]


    return files

#-----------------------------------------------------------------------------------------------------        
if __name__ == "__main__":

    if options.arg.count('-o '):
        log.error('Jobs can use common output directory... arg should not contain "-o": arg="%s"' %options.arg)
        sys.exit(1)
    
    eosCMD = '/afs/cern.ch/project/eos/installation/0.3.4/bin/eos.select' 
    tbeg  = time.time()
    files = []

    if options.input_files != None and os.path.isfile(options.input_files):
        files = prepareFilesFromTxt(options.input_files)
    else:
        log.error('Missing or invalid option: --input-files=%s' %options.input_files)

    if len(files) == 0:
        log.error('Failed to find matching input files... nothing to do')
        sys.exit(1)

    log.info('Number of selected files: %d' %len(files))
    
    #
    # Make input file list for each job
    #
    jobFiles = []
    curFiles = []
    curSize  = 0

    for file in files:
        if type(file.split()[1]) != str:
            log.warning('Invalid file: %s' %file)
            continue
        
        curFiles += [file.split()[1]]
        curSize  += float(file.split()[0])
            
        if (options.nfile_per_job > 0 and len(curFiles) >= options.nfile_per_job) or (options.mb_per_job > 0 and curSize > options.mb_per_job*1048576):
                                                          
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
        jobOut = 'out_%s.root' %jobKey
        jobNtp = 'ntp_%s.root' %jobKey        
        jobTp  = 'tp_%s.root'  %jobKey        
        jobXml = 'run_%s.xml'  %jobKey

        jobPath  = '%s' %(options.output.rstrip('/'))
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

        if options.output == None:
            log.error('Need output option - path to output directory')
            sys.exit(1)

        if not os.path.isdir(options.output):
            os.makedirs(options.output)

        #
        # Prepare submission command 
        #
        testArea = os.environ.get('TestArea')    
        jobExe   = '%s/PhysicsNtuple/PhysicsLight/macros/batch/runCERN.py' %testArea
        jobMacro = options.macro
            
        if not os.path.isfile(jobExe):
            log.error('Invalid job executable: %s' %jobExe)
            sys.exit(1)
            
        if not os.path.isfile(jobMacro):
            log.error('Invalid job macro: %s' %jobMacro)
            sys.exit(1)
 
        #
        # Configure command line options for macro executed by batch system
        #
        runArgs = '%s' % options.arg

        if options.do_ntuple:
            runArgs += ' --ntuple-file=%s' % jobNtp
        
        if options.do_tp:
            runArgs += ' --tp-file=%s' % jobTp


        #
        # Configure job 
        #
        jobArgs  = '--cluster=Bonn --input=%s --outdir=%s --macro=%s --jobid=%d --arg=\'%s\'' %(jobInputPath, jobDir, jobMacro, ijob, runArgs)

        if options.dry_run:
            jobArgs += ' --dry-run'
        
        countJob += 1
        
        if options.njob > 0 and ijob+1 >= options.njob:
            break

        if ijob % options.nprint == 0:
            log.info('processing job %4d: %s' %(ijob, time.asctime()))
 
        #
        # create condor job file
        jobScript = createJobScript(jobExe,jobArgs,options.output,jobKey)
        #    
        # Submit condor job
        #
        commands = ['qsub', '-q', options.queue, jobScript]

        if not options.submit:
            print ' '.join(commands)
            sys.exit(0)

        process  = subprocess.Popen(commands, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        (stdout, stderr) = process.communicate()


