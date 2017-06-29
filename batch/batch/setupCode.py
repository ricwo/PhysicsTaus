
"""

Examples:

  python code/setupCode.py --trunk

"""

import os, sys, re
import time
import subprocess
import socket
import logging
import optparse

p = optparse.OptionParser()

p.add_option('--trunk',   default=False, action='store_true', dest='trunk')
p.add_option('--debug',   default=False, action='store_true', dest='debug')
p.add_option('--level',   default=False, action='store_true', dest='level')
p.add_option('--dry-run', default=False, action='store_true', dest='dry_run')

(options, args) = p.parse_args()

#---------------------------------------------------------------------
def getLog(name, level=options.level, debug=False, print_time=False):

    f = logging.Formatter('%(name)s: %(levelname)s - %(message)s')        
    h = logging.StreamHandler(sys.stdout)
    h.setFormatter(f)

    log = logging.getLogger(name)
    log.addHandler(h)
    log.setLevel(logging.INFO)    

    return log

log = getLog('setupCode.py')

#--------------------------------------------------------------------------------------------
def executeCommand(commands):

    log.info('executeCommand - "%s"' %(' '.join(commands)))

    process = subprocess.Popen(commands, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    (stdout, stderr) = process.communicate()        
    
    if len(stderr.strip()):
        sys.stderr.write(stderr)
        sys.stderr.flush()
    if len(stdout.strip()):
        sys.stdout.write(stdout)
        sys.stdout.flush()
        
    if process.returncode != 0:
        raise Exception('executeCommand - failed command: %s' %commands)

    return process.returncode
        
#--------------------------------------------------------------------------------------------
def CheckOutTrunk(package, force=False):

    command = 'checkout'
    package = package.rstrip('/')
    pacpath = 'PhysicsNtuple/%s' %package
    user    = ''

    if os.path.isdir(pacpath):
        raise Exception('Package already exists: %s' %pacpath)

    log.info('CheckOutTrunk - %s package: %s' %(command, package))

    commands = ['svn', 
                command,
                'svn+ssh://%ssvn.cern.ch/reps/penn/PhysicsNtuple/%s/trunk' %(user, package),
                pacpath]

    if not options.dry_run:
        return executeCommand(commands)

#--------------------------------------------------------------------------------------------
def makeAtlasSetupScript():

    fname  = 'setup_atlas_release.sh'    
    config = GetReleaseConfig()
    curdir = os.getcwd()
    
    ofile = open(fname, 'w')
    ofile.write('ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase\n')
    ofile.write('source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh\n')
    ofile.write('asetup %s --testarea=`pwd`\n' %config)
    ofile.close()

    return fname

#--------------------------------------------------------------------------------------------
def makeCompileScript(afile):

    fname  = 'compile_code.sh'

    ofile = open(fname, 'w')
    ofile.write('source %s\n' %afile)
    ofile.write('pushd `pwd`\n\n')
    ofile.write('cd PhysicsNtuple/PhysicsLight/cmt\n')
    ofile.write('cmt bro cmt config\n')
    ofile.write('cmt bro `source setup.sh`\n')
    ofile.write('cmt bro make -j4\n\n')
    ofile.write('popd\n')
    ofile.close()

    return fname

#--------------------------------------------------------------------------------------------
def GetReleaseConfig():

    return '18.9.0,slc6,here,save'

#-----------------------------------------------------------------------------------------------------        
if __name__ == "__main__":

    timeStart = time.time()
    log.info('Start job at host %s:%s' %(socket.gethostname(), os.getcwd()))
    
    afile = makeAtlasSetupScript()
    
    CheckOutTrunk('PhysicsBase')
    CheckOutTrunk('PhysicsData')
    CheckOutTrunk('PhysicsLight')

    cfile = makeCompileScript(afile)

    if not options.dry_run:
        executeCommand(['bash', cfile])

    log.info('All is done')
    log.info('Total job time: %.1fs' %(time.time()-timeStart))
