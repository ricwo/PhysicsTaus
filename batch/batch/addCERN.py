#!/usr/bin/env python

import os
import re
import sys
import time
import subprocess

import PhysicsLight.PhysicsLightConfig as config

log = config.getLog('addCERN')

from optparse import OptionParser
p = OptionParser()

  
p.add_option('--queue',         type='string',  default='1nh',             dest='queue')
p.add_option('--output', '-o',  type='string',  default=None,              dest='output')
p.add_option('--indir', '-i',  type='string',  default=None,              dest='indir')
p.add_option('-k', '--file-key', type  ='string',   default='out.*root',  dest='file_key')
p.add_option('--overwrite',    action='store_true', default=False, dest='overwrite')
p.add_option('--submit',      action='store_true', default=False, dest='submit')
p.add_option('--print-jobs',      action='store_true', default=False, dest='print_jobs')
(options,args) = p.parse_args()

#-----------------------------------------------------------------------------------------------------        
def printOut(target, value):

    if len(value.strip()):
        target.write(value)
        target.flush()


#-----------------------------------------------------------------------------------------------------        
def procJob(jobExe, jobOpts, outDir, jobId):

    jobKey = '%04d' %jobId
    jobScript=  createJobScript(jobExe,jobOpts,outDir,jobKey)
    jobOutput = 'job_%s.out' % jobKey
    
    commands = ['bsub', '-q', options.queue, 
                '-o', jobOutput,
                '-f', '%s <' %(jobOutput),
                'source', options.output +'/' + jobScript]

    if options.submit:
        
        process  = subprocess.Popen(commands, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        (stdout, stderr) = process.communicate()
        
        if options.print_jobs:
            log.info('Submitted job %d: %s' %(jobId, ' '.join(commands)))
            printOut(sys.stdout, stdout)
            printOut(sys.stderr, stderr)

   
    else:
        if options.print_jobs:
            log.info('Prepared job %d: %s' %(jobId, ' '.join(commands)))

#-----------------------------------------------------------------------------------------------------        
def createJobScript(jobExe, jobOpts, outDir,jobKey):
    
    testarea = os.getenv('TestArea')
    setupScript = testarea + '/setup_pro.sh' 
    jobScript = 'job_'+jobKey+'.sh'
    
    file = open(outDir+ '/' + jobScript, 'w') 
    file.write('#!/bin/sh\n')
    file.write('source ' + setupScript + '\n')
    file.write(' '.join(['python',jobExe,jobOpts]))
    file.write('\n')
    file.close() 

    return jobScript 
#--------------------------------------------------------------------
def getUniqueKeys(files, key = '', veto = ''):
    preKeys = []
    postKeys= []
    for file in files: 
        actualFile = file.split('/')[len(file.split('/'))-1].rstrip(".root")
      
        if not veto == '' and actualFile.count(veto): 
            continue
         
        if not key == '' and not actualFile.count(key):
            continue 
      
        if len(actualFile.split('_')) == 1:
            log.warning('getUniqueKey: filename fails to match template XXX_*.root - %s' %file) 
            continue 
        
        pre  = actualFile.split('_')[0]
        post = ''
       
      
        if not key == '':
           if len(actualFile.split(key)) >1: post=actualFile.split(key)[1]

        if not pre in preKeys: preKeys.append(pre)
        if not post in postKeys: postKeys.append(post)

    return preKeys,postKeys


#--------------------------------------------------------------------
def getRootFiles(path,key,veto=''):
    
    if os.path.isfile(path):
        fname = os.path.basename(path)

        if options.file_key and re.search(key, fname) == None:
            return None
        
        if fname.count('.root'):
            if veto =='' or not fname.split('/')[len(fname.split('/'))-1].count(veto):
                return [path]
           
        return None

    files = []
    
    for file in os.listdir(path):
        if len(file) < 3:
            continue

        fpath = '%s/%s' %(path.rstrip('/'), file)
        flist = getRootFiles(fpath,key)
        
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

    if os.path.isfile(apath):
        log.warning('processDir - output file already exists: %s' %apath)
        return
    
    files = getRootFiles(path, options.file_key)
   
    keysType1 = getUniqueKeys(files,key='-')
    keysType2 = getUniqueKeys(files,veto='-')
    #
    # Prepare submission command 
    #

    testArea = os.environ.get('TestArea')   
    jobExe   = '%s/PhysicsNtuple/PhysicsLight/macros/batch/addFiles.py' %testArea
    outDir = options.output.rstrip('/')
    if not os.path.isdir(outDir):
        os.makedirs(outDir)

    if not os.path.isfile(jobExe):
        log.error('Invalid job executable: %s' %jobExe)
        sys.exit(1)
        
    # submit type 1 jobs
    jobId = 0
    for preKey in keysType1[0]:
        for postKey in keysType1[1]:
              
            #double check that there are actual  files 
            if len(getRootFiles(path,'%s_.*%s\.root' %(preKey,postKey))) < 1:
                log.warning('processDir - did not find any ROOT files for keys: %s,%s' %(preKey,postKey))
                return
            jobOpts = '%s --output %s/%s-%s.root --file-key %s.*%s\.root' %(path,options.output.rstrip('/'),preKey,postKey,preKey,postKey)
            jobId +=1
            procJob(jobExe,jobOpts,outDir,jobId)

    # submit type 2 jobs 

    for preKey in keysType2[0]:
        if len(getRootFiles(path,'%s_.*root' %(preKey),veto='-')) < 1:  
            log.warning('processDir - did not find any ROOT files for keys: %s' %(preKey))
            return

        jobOpts = '%s --output %s/%s.root --file-key %s.*root --file-veto -' %(path,options.output.rstrip('/'),preKey,preKey)
        
        jobId +=1
        procJob(jobExe,jobOpts,outDir,jobId)

    log.info('Directory: %s' %path)
    log.info('Total time: %.1fs' %(time.time()-timeStart))    

#--------------------------------------------------------------------
if __name__ == "__main__":

    # --------------------------------------
    # List files
    #
    
    if options.output == None:
        log.error('Need output option - path to output directory')
        sys.exit(1)
  
    if os.path.isdir(options.indir):
        processDir(options.indir.rstrip('/'))
    else:
        log.info('Skip non-directory: %s' %arg)
         


        
