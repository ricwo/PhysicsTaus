#!/usr/bin/env python

import os
import sys
import subprocess
import time
import logging

from optparse import OptionParser
p = OptionParser()

p.add_option('--output', '-o',  type='string',  default=None,              dest='output')
p.add_option('--eosout',        type='string',  default=None,              dest='eosout')
p.add_option('--macro',         type='string',  default=None,              dest='macro')
p.add_option('--file-list',     type='string',  default=None,              dest='file_list')
p.add_option('--input-files',   type='string',  default=None,              dest='input_files')
p.add_option('--arg',           type='string',  default='',                dest='arg')
p.add_option('--queue', '-q',   type='string',  default='8nh',             dest='queue')
p.add_option('--setup-script',  type='string',  default='setup_pro.sh',    dest='setup_script')
p.add_option('--run-macro',     type='string',  default=None,              dest='run_macro')

p.add_option('--njob', '-n',    type='int',     default=0,     dest='njob')
p.add_option('--nfile-per-job', type='int',     default=0,     dest='nfile_per_job')
p.add_option('--mb-per-job',    type='int',     default=1000,  dest='mb_per_job')
p.add_option('--nprint', '-p',  type='int',     default=1,     dest='nprint')

p.add_option('--debug', '-d',     action='store_true', default=False, dest='debug')
p.add_option('--overwrite',       action='store_true', default=False, dest='overwrite')
p.add_option('--make-job-dir',    action='store_true', default=False, dest='make_job_dir')
p.add_option('--copy-log',        action='store_true', default=False, dest='copy_log')
p.add_option('--dry-run',         action='store_true', default=False, dest='dry_run')
p.add_option('--submit', '-s',    action='store_true', default=False, dest='submit')
p.add_option('--local',           action='store_true', default=False, dest='local')
p.add_option('--run-local',       action='store_true', default=False, dest='run_local')
p.add_option('--request-pool',    action='store_true', default=False, dest='request_pool')

(options, args) = p.parse_args()

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

log = getLog('subCERN.py', print_time=False)

#======================================================================================================
class Pushd:

    init_dir = None
    orig_dir = None

    def __init__(self, dirname):
        self.init_dir = os.path.realpath(dirname)
        self.orig_dir = None
        
    def __enter__(self):
        self.orig_dir = os.getcwd()
        os.chdir(self.init_dir)
        
        log.debug('Pushd.__enter__ - CWD=%s' %os.getcwd())
        return self
 
    def __exit__(self, type, value, tb):
        os.chdir(self.orig_dir)
        log.debug('Pushd.__exit__ - CWD=%s' %os.getcwd())

#======================================================================================================
def createJobScript(jobExe, jobOpts, outDir, jobKey):

    testarea = os.getenv('TestArea')
    setupScript = '%s/%s' %(testarea, options.setup_script)
    jobScript = 'job_%s.sh' %jobKey
    jobResult = '%s/job_%s' %(outDir, jobKey)
        
    if not os.path.isdir(outDir):
        raise Exception('createJobScript - invalid job directory: %s' %outDir)

    if not os.path.isdir(testarea):
        raise Exception('createJobScript - invalid test area: %s' %testarea)

    if not os.path.isfile(setupScript):
        raise Exception('createJobScript - invalid release setup script: %s' %setupScript)

    if os.path.isfile('%s.fail' %jobResult):
        log.error('createJobScript - please clean up aborted job: %s.fail' %jobResult)
        sys.exit(1)

    if os.path.isfile('%s.pass' %jobResult):
        log.info('createJobScript - skip successfully completed job: %s' %jobKey)
        return None

    stext = '#!/bin/sh\n\n'

    stext += 'pushd %s\n' %testarea
    stext += 'source %s\n' %setupScript
    stext += 'popd\n\n'

    if options.local:
        lpath = '%s/local/job_%s' %(outDir, jobKey)

        if not os.path.isdir(lpath):
            os.makedirs(lpath)
            
        stext += 'cd %s\n\n' %lpath

    stext += '%s\n\n' %(' '.join(['python', jobExe, jobOpts]))
    stext += 'job_return_code=$?\n\n'

    if options.copy_log:
        stext += 'if [ -e job.log ]\n'
        stext += 'then\n'
        stext += '   cp job.log %s/job_%s.log\n' %(outDir, jobKey)
        stext += 'fi\n\n'

    stext += 'if [ $job_return_code -ne 0 ]\n'
    stext += 'then\n'
    stext += '   touch %s.fail\n' %jobResult
    stext += '   echo Job command has failed with code=$job_return_code - quit job now...\n'
    stext += '   exit $job_return_code\n'
    stext += 'fi\n\n'

    if options.local:
        stext += 'if [ -d input ]\n'
        stext += 'then\n'
        stext += '   rm -r input\n'
        stext += 'fi\n\n'

    stext += 'touch %s.pass\n' %jobResult

    sfile = open('%s/%s' %(outDir, jobScript), 'w')
    sfile.write(stext)
    sfile.close() 

    return jobScript 

#======================================================================================================
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
        files += [line.rstrip().strip()]

    return files

#======================================================================================================
def prepJobFiles(files):

    '''Split input files so that each job accepts files with approximately equal size'''

    jobFiles = []
    curFiles = []
    curSize  = 0
    maxSize  = options.mb_per_job*1048576

    for ifile in files:

        if ifile.count('eos'):
            parts = ifile.split()

            if len(parts) != 2:
                raise Exception('prepJobFiles - invalid line: %s' %ifile)

            if type(parts[1]) != str:
                log.warning('Invalid file: %s' %ifile)
                continue

            part0 = parts[0].strip()
            part1 = parts[1].strip()
            
            ifile_path = part1
            ifile_size = int(part0)
        else: 
            if type(ifile) != str:
                log.warning('Invalid file: %s' %ifile)
                continue

            if not os.path.isfile(ifile):
                log.warning('Skip non existing file: %s' %ifile)
                continue
            
            ifile_path = ifile
            ifile_size = os.path.getsize(ifile)

        pass_count = (options.nfile_per_job > 0 and len(curFiles) + 1    >= options.nfile_per_job)
        pass_size  = (options.mb_per_job    > 0 and curSize + ifile_size >  maxSize)

        if pass_count or pass_size:
            add_curr = False
            
            if options.mb_per_job == 0:
                add_curr = True
            else:
                if curSize <= int(0.1*maxSize):
                    add_curr = True
                elif float(curSize + ifile_size)/max(float(curSize), maxSize) < 1.2:
                    add_curr = True

            if add_curr:
                curFiles += [ifile_path]
                curSize  +=  ifile_size

            jobFiles += [(curFiles, curSize)]
            curSize  = 0
            curFiles = []

            if add_curr:
                curFiles = []
                curSize  = 0
            else:
                curFiles = [ifile_path]
                curSize  =  ifile_size        
        else:
            curFiles += [ifile_path]
            curSize  +=  ifile_size

    if len(curFiles):            
        jobFiles += [(curFiles, curSize)]

    return jobFiles

#======================================================================================================
def printOut(target, value):

    if len(value.strip()):
        target.write(value)
        target.flush()

#======================================================================================================
def procJob(files, file_sizes, jobKey, jobDir, jobMacro):
    #
    # Prepare input file list
    #
    jobInputPath  = '%s/job_%s.input' % (jobDir, jobKey)
    file_sizes_mb = int(float(file_sizes)/1048576.0)

    if not options.overwrite and os.path.isfile(jobInputPath):
        log.warning('Job input file already exists: %s' %jobInputPath)
        
    jobInputFile = open(jobInputPath, 'w')
            
    for f in files:
        jobInputFile.write('%s\n' %f)

    jobInputFile.close()
 
    #
    # Configure job 
    #
    jobArgs = '--input=%s --outdir=%s --macro=%s --jobid=%d --arg=\'%s\'' %(jobInputPath, jobDir, jobMacro, ijob, options.arg)

    if options.dry_run:
        jobArgs += ' --dry-run'
       
    if options.eosout:
        jobArgs += ' --eosout=%s' % options.eosout 

    if options.nfile_per_job:
        jobArgs += ' --nfile-per-job=%s' % options.nfile_per_job
        
    #
    # Create LSF batch script and prepare submission
    #
    jobPrint  = (ijob % options.nprint == 0)
    jobOutput = 'job_%s.out' %jobKey
    jobScript = createJobScript(jobExe, jobArgs, options.output, jobKey)

    if jobScript == None:
        return

    if options.request_pool:
        if os.getenv("LSB_STDOUT_DIRECT") == None:
            commands = ['bsub', '-q', options.queue, 
                        '-R "rusage[pool=%d]"' %max(1, (file_sizes_mb)),  #pool is requested in MB, assumed to be provided in B
                        '-o', jobOutput,
                        '/bin/bash', options.output +'/' + jobScript]
        else:
            raise Exception("LSB_STDOUT_DIRECT is set. No pool resource can be requested.")
    else:
        if file_sizes_mb > 1e4:
            log.warning('Input file >10GB and no pool resource requested. Consider running with --request-pool')

        commands = ['bsub', '-q', options.queue, 
                    '-o', jobOutput,
                    '-f', '%s <' %(jobOutput),
                    '/bin/bash', options.output +'/' + jobScript]
        
    ostr = 'job %3d using %3d files and %6d MB: %s' %(ijob, len(files), file_sizes_mb, ' '.join(commands))

    if options.submit:
        log.info('procJobs - will submit command: %s' %commands)

        if options.request_pool:
            commandsShellTrue = ' '.join(commands)
            process  = subprocess.Popen(commandsShellTrue, stdout=subprocess.PIPE, stderr=subprocess.PIPE,shell=True,cwd=options.output)
        else:
            process  = subprocess.Popen(commands, stdout=subprocess.PIPE, stderr=subprocess.PIPE,)

        (stdout, stderr) = process.communicate()
            
        if jobPrint:
            log.info('Submitted %s' %ostr)
            printOut(sys.stdout, stdout)
            printOut(sys.stderr, stderr)
    else:
        if jobPrint:
            log.info('Prepared %s' %ostr)

    return jobScript

#======================================================================================================        
def prepOutPathEOS(ipath):

    if ipath.startswith('root://eos'):
        log.error('prepOutPathEOS - wrong EOS output path format: %s' %ipath)
        sys.exit(1)

    if ipath.count('/eos/escience'):
        os.environ['EOS_MGM_URL'] = 'root://eospublic/'
    elif ipath.count('/eos/atlas'):
        os.environ['EOS_MGM_URL'] = 'root://eosatlas/'

    eosLS = '/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select ls %s'    %ipath
    eosMK = '/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select mkdir %s' %ipath

    process = subprocess.Popen([eosLS], stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    (stdout, stderr) = process.communicate()

    if process.returncode == 0:
        log.info('prepOutPathEOS - directory already exists: %s - do nothing' %ipath)
        return

    process = subprocess.Popen([eosMK], stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    (stdout, stderr) = process.communicate()

    if len(stdout.strip()):
        sys.stdout.write(stdout)
        sys.stdout.flush()
    if len(stderr.strip()):
        sys.stderr.write(stderr)
        sys.stderr.flush()

    if process.returncode != 0:
        log.error('prepOutPathEOS - failed to mkdir EOS output path: %s' %ipath)
        sys.exit(1)

    if 'EOS_MGM_URL' in os.environ:
        del os.environ['EOS_MGM_URL']

    log.info('prepOutPathEOS - created directory: %s' %ipath)

#======================================================================================================
if __name__ == '__main__':

    if options.output == None:
        log.error('Need output option - path to output directory')
        sys.exit(1)
        
    if not os.path.isdir(options.output):
        os.makedirs(options.output)

    timeStart = time.time()
    files = []

    if options.input_files != None:
        for input_file in options.input_files.split(','):
            if os.path.isfile(input_file):
                files += prepareFilesFromTxt(input_file)
            else:
                log.warning('Skip non-existing input file: %s' %input_file)
                
    elif len(args) > 0:
        for arg in args:
            if os.path.isdir(arg):
                for f in sorted(os.listdir(arg)):
                    if f.count('.root'):
                        files += ['%s/%s' %(arg.rstrip('/'), f)]
    else:
        log.error('Missing or invalid option: --input-files=%s' %options.input_files)

    if len(files) == 0:
        log.error('Failed to find matching input files... nothing to do')
        sys.exit(1)

    #
    # Make input file list for each job
    #
    log.info('Number of selected files: %d' %len(files))
    
    jobFiles = prepJobFiles(files)

    if len(jobFiles) == 0:
        log.info('No valid files available to build jobs... nothing to do')
        sys.exit(0)

    #
    # Prepare submission command 
    #
    testArea = os.environ.get('TestArea')    
    jobMacro = options.macro
    jobPath  = '%s' %(options.output.rstrip('/'))

    if options.run_macro != None:
        jobExe = options.run_macro
    else:
        jobExe = '%s/PhysicsNtuple/PhysicsLight/macros/batch/runCERN.py' %testArea
    
    if not os.path.isfile(jobExe):
        log.error('Invalid job executable: %s' %jobExe)
        sys.exit(1)
        
    if not os.path.isfile(jobMacro):
        log.error('Invalid job macro: %s' %jobMacro)
        sys.exit(1)    

    if options.eosout:
        log.info('EOS out directory: %s' %options.eosout)
        if options.submit:
            prepOutPathEOS(options.eosout)

    #
    # Create jobs
    #
    log.info('Output directory: %s' %options.output)
    log.info('Will prepare %d job(s)' %len(jobFiles))

    countJob   = 0
    countFile  = 0
    jobScripts = []

    for ijob in range(0, len(jobFiles)):
        
        if options.njob > 0 and ijob+1 > options.njob:
            break

        jobKey = '%04d' %ijob
    
        if options.make_job_dir:
            jobDir = '%s/job_%s' %(jobPath, jobKey)
        else:
            jobDir = '%s'        %(jobPath)
            
        if not os.path.isdir(jobDir):
            os.makedirs(jobDir)

        input_file_list = jobFiles[ijob][0]
        input_file_size = jobFiles[ijob][1]

        with Pushd(jobDir):
            jobScripts += [procJob(input_file_list, input_file_size, jobKey, jobDir, jobMacro)]

        countJob  += 1    
        countFile += len(input_file_list)

    if options.local and len(jobScripts):
        spath = '%s/run_local.sh' %options.output
        sfile = open(spath, 'w')
        sfile.write('pushd %s\n\n' %options.output)
        
        for js in jobScripts:
            if js:
                sfile.write('/bin/bash %s &> run_local_%s &\n' %(js, js.replace('.sh', '.log')))
            
        sfile.write('\npopd\n')
        sfile.close()

        if options.run_local:
            log.info('Will execute this script to run jobs locally:')
            log.info('   $ /bin/bash %s' %spath)

            process = subprocess.Popen(['/bin/bash', spath], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            (stdout, stderr) = process.communicate()
            
            print
            printOut(sys.stdout, stdout)
            printOut(sys.stderr, stderr)

            if process.returncode != 0:
                log.error('Failed to run local job script')
                sys.exit(1)
        else:
            log.info('Script to run jobs locally:')
            log.info('   $ source %s' %spath)

    print
    log.info('Processed %d jobs and %d files - all is done' %(countJob, countFile))
    log.info('Local time: %s'    %time.asctime())
    log.info('Total time: %.1fs' %(time.time()-timeStart))
