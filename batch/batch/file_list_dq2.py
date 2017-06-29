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
p.add_option('--pattern',        type=str, default=None)
p.add_option('--disk',           type=str, default='CERN-PROD_PHYS-HIGGS')
p.add_option('--level', '-l',    type=str, default='INFO')

p.add_option('--nfile', '-n',    type=int, default=0)

p.add_option('--do-dq2',         action='store_true', default=False,   dest='do_dq2')

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

log = getLog('dq2_file_list.py')

#---------------------------------------------------------------------
def getFileRucio(ifile):
    
    parts = ifile.split()
    
    if len(parts) < 7:
        raise Exception('getFileRucio - wrong format: %s' %ifile)

    fsize = int(parts[2])
    efile =     parts[6]

    return (fsize, efile)

#---------------------------------------------------------------------
def getFileCern(ifile):

    efile = '/eos/%s' %ifile.partition('/eos/')[2]
    ecmd  = ['/afs/cern.ch/project/eos/installation/0.3.4/bin/eos.select', 'ls', '-l', efile]
    
    process = subprocess.Popen(ecmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    (stdout, stderr) = process.communicate()

    try:
        fsize = int(stdout.split()[4])
    except:
        raise Exception('Wrong value for %s: %s' %(ifile, stdout))

    return (fsize, efile)

#---------------------------------------------------------------------
def getFilePenn(ifile):

    srm = 'srm://srm.hep.upenn.edu/disk/space00'

    if ifile.count(srm) != 1:
        log.error('Wrong file string: %' %s(ifile))
        sys.exit(1)

    efile = ifile.replace(srm, 'root://hn.at3f/')
    xfile = ifile.replace(srm, '')

    ecmd  = ['xrd', 'hn.at3f', 'stat', xfile]

    log.debug('stat command: %s' %ecmd)
    
    process = subprocess.Popen(ecmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    (stdout, stderr) = process.communicate()

    try:
        fsize = int(stdout.partition('Size:')[2].split()[0])
    except:
        log.error('Wrong value for %s: %s' %(ifile, stdout.split()))
        sys.exit(1)   
        
    return (fsize, efile)

#---------------------------------------------------------------------
def getFileListRucio(ds):

    log.info('getFileListRucio - dataset: %s' %ds)

    if ds.count('.') < 1:
        raise Exception('getFileListRucio - invalid dataset: %s' %ds)

    tag = ds.split('.')[0]
    cmd = ['rucio', 'list-file-replicas', '--protocol=srm', '--rse=%s' %options.disk, '%s:%s' %(tag, ds)]
    
    log.info('Command: %s' %(' '.join(cmd)))

    process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    (stdout, stderr) = process.communicate()

    files = []

    for dline in stdout.split('\n'):
        
        if len(dline.split()) < 7:
            continue
        
        if options.pattern == None or re.search(options.pattern, dline) != None:
            log.debug('%s' %dline)
            files += [dline]

            if options.nfile and len(files) == options.nfile:
                break

    return files

#---------------------------------------------------------------------
def getFileListDQ2(ds):

    cmd = ['dq2-ls', '-L', options.disk, '-fp', '%s/' %ds]
    
    log.info('Dataset: %s' %ds)
    log.info('Command: %s' %(' '.join(cmd)))

    process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    (stdout, stderr) = process.communicate()

    files = []

    for dline in stdout.split():
        if re.search(options.pattern, dline) != None:
            log.debug('%s' %dline)
            files += [dline]

            if options.nfile and len(files) == options.nfile:
                break

    return files

#---------------------------------------------------------------------
def getDatasets():

    dlist = []

    if options.datasets and os.path.isfile(options.datasets):
        for ds in datasetList.readlines():
            dlist += [ds.strip().strip('/')]
    
    elif options.ds_name:
        dlist = [options.ds_name.strip('/')]

    return dlist
            
#---------------------------------------------------------------------
if __name__ == "__main__":

    if options.datasets == None or not os.path.isfile(options.datasets):
        if options.ds_name == None:
            log.error('Missing required --datasets or --ds-name option')
            sys.exit(1)

    outputFile  = open(options.outfile,  'w')

    for ds in getDatasets():
        if len(ds) < 20:
            continue

        ds = ds.strip()

        if options.do_dq2:
            files = getFileListDQ2  (ds)
        else:
            files = getFileListRucio(ds)

        #
        # Prepare file list with size
        #
        tsize = 0

        for ifile in files:

            if ifile.count('eos'):
                (fsize, efile) = getFileCern (ifile)
            elif options.do_dq2:
                (fsize, efile) = getFilePenn (ifile)
            else:
                (fsize, efile) = getFileRucio(ifile)

            tsize += fsize

            outputFile.write('%d %s\n' %(fsize, efile))
            log.info('%10s %s' %('%d' %(fsize), efile))
        
        log.info('found %d file(s) with %.2f MB' %(len(files), tsize/1048576.0))

    outputFile.close()
