#!/usr/bin/env python

import os
import re
import sys
import time

#-----------------------------------------------------------------------------
# Configure python and C++ objects, add input files and execute C++ code
#    
def main():

    timeStart = time.time()
    
    #
    # Parse command line options
    #
    import PhysicsProject.PhysicsProjectConfig as config

    p = config.prepareOptionParser()
    (options, args) = p.parse_args()

    log = config.getLog(__name__, debug=options.debug)
    log.info('Start time: %s' %time.asctime())    

    #
    # Recursively search for ROOT files in input paths
    #
    root_files = sorted(config.findInputLocalFiles(args, '.root', options.dirskey, save_files=options.save_files, print_files=False))

    if options.xsec_list:
        files = config.selectInputFiles(root_files, options)
    else:
        files = root_files

    log.info('%4d file(s) selected from %d input ROOT files' %(len(files), len(root_files)))

    #
    # Filter files
    #
    if options.filekey:
        store = files
        files = []
        
        for fpath in store:
            if re.search(options.filekey, os.path.basename(fpath)):
                files += [fpath]

        log.info('%4d file(s) passed filter: --file-key=\"%s\"' %(len(files), options.filekey))

    if options.save_files:
        log.info('Save input files to: %s' %options.save_files)
        
        ofile = open(options.save_files, 'w')
        
        for file in files:
            ofile.write('%s\n' %file)

        ofile.close()

    log.info('Process %d input file(s)' %len(files))
    for file in files:
        print '   %s' %file
        
    #
    # Configure python RunConfig wrapper around C++ RunModule
    #
    import ROOT
    import PhysicsProject.PhysicsProjectRun2Base as Base
    import PhysicsProject.PhysicsProjectRun2Plot as tth
    
    if options.batch:
        ROOT.gROOT.SetBatch(True)

    run = tth.prepareJobConfig(ROOT, options)

    for file in files:
        run.StoreInputFile(file)

    if options.print_vars:
        ROOT.Anp.Cut.PrintCutNames()        

    run.ExecuteRegistry()
    
    log.info('Run python post processing code...')

    if options.ntuple_file and options.do_flat_ntuple:
        Base.copySimpleCountsFromInputFiles(files, options.ntuple_file)
    
    log.info('Local time: %s'    %time.asctime())
    log.info('Total time: %.1fs' %(time.time()-timeStart))

if __name__ == "__main__":
    main()
