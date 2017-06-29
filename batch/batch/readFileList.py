import os
import re
import subprocess

import PhysicsLight.PhysicsLightConfig as config

log = config.getLog('readFileList.py')

from optparse import OptionParser
p = OptionParser()

p.add_option('--output',    type  ='string',     default=None,  dest='output')
p.add_option('--fkey',      type  ='string',     default=None,  dest='fkey')

p.add_option('--check-dq2',  action='store_true', default=False, dest='check_dq2')
p.add_option('--print-tid',  action='store_true', default=False, dest='print_tid')
p.add_option('--print-file', action='store_true', default=False, dest='print_file')
p.add_option('--dry-run',    action='store_true', default=False, dest='dry_run')
p.add_option('--debug',      action='store_true', default=False, dest='debug')

(options, args) = p.parse_args()

#--------------------------------------------------------------------
def getDQ2Status(dataset):

    if options.print_tid:
        commands = ['dq2-ls', '-r', '%s'  %dataset.rstrip('/')]
    else:
        commands = ['dq2-ls', '-r', '%s/' %dataset.rstrip('/')]
    
    if options.dry_run:
        commands = ['echo'] + commands

    log.debug('------------------------------------------------------------')
    log.debug('getDQ2Status - execute command:\n   %s' %(' '.join(commands)))
    
    process  = subprocess.Popen(commands, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    (stdout, stderr) = process.communicate()
    
    log.debug('stdout:\n%s' %stdout)

    for line in stdout.splitlines():
        if line.count('UPENN_LOCALGROUPDISK') > 0:
            return line.partition(':')[0].strip()

    return 'UNKNOWN'
    
#--------------------------------------------------------------------
if __name__ == '__main__':

    ifiles = []

    #
    # Read file list from previous scans
    #
    for arg in args:
        ifile = open(arg, 'r')

        for line in ifile.readlines():
            ifiles += [line.rstrip('\n').strip()]
    
    #
    # Add new files to the list
    #
    ds_full = {}
    ds_tids = {}
    ds_prod = {}
    
    for ifile in ifiles:
        if options.fkey != None and re.search(options.fkey, ifile) == None:
            continue
        
        file = os.path.basename(ifile)
        path = os.path.dirname (ifile)
        
        ds0 = os.path.basename(path)
        ds1 = os.path.basename(path).rpartition('.')[0]

        try:
            ds_full[ds1] += [file]
        except KeyError:
            ds_full[ds1]  = [file]

        try:
            ds_tids[ds0] += [file]
        except KeyError:
            ds_tids[ds0]  = [file]

        parts = ds1.split('.')
        if len(parts) > 6:
            if ds1.count('AOD'):
                prod = parts[8]
            else:
                prod = parts[6]
            
            try:
                ds_prod[prod].add(ds1)
            except KeyError:
                ds_prod[prod] = set()
                ds_prod[prod].add(ds1)                

    #
    # Print dataset summary
    #
    if options.print_tid:
        ds_print = ds_tids
    else:
        ds_print = ds_full
    
    for ds in sorted(ds_print.keys()):
        
        if options.check_dq2:
            print 'NFILE=%4d %s: %s' %(len(ds_print[ds]), getDQ2Status(ds), ds)
        else:
            print 'NFILE=%4d %s' %(len(ds_print[ds]), ds)
    
        if options.print_file:
            for file in sorted(ds_print[ds]):
                print '   %s' %file

    #
    # Print production keys
    #
    for prod in sorted(list(ds_prod.keys())):
        print prod
        for ds in sorted(list(ds_prod[prod])):
            print '   %s' %ds
