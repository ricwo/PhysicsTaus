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
    import PhysicsTaus.PhysicsTausConfig as config

    p = config.prepareOptionParser()
    (options, args) = p.parse_args()

    log = config.getLog(__name__, debug=options.debug)
    log.info('Start time: %s' %time.asctime())    

    #
    # Recursively search for ROOT files in input paths
    #
    root_files = sorted(config.findInputLocalFiles(args, options.filekey, options.dirskey, print_files=False))

    if options.xsec_list:
        files = config.selectInputFiles(root_files, options)
    else:
        files = root_files


    log.info('Selected %4d ROOT files' %(len(files)))

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

    log.info('Process %d input file(s)' %len(files))
    for file in files:
        print '   %s' %file
            
    #
    # Configure python RunConfig wrapper around C++ RunModule
    #
    import ROOT
    from PhysicsTaus.PhysicsTausProcReco import prepareJobConfig
    
    if not options.no_batch:
        ROOT.gROOT.SetBatch(True)

    isData = False
    if 'Data' in file:
        isData = True
    runNumber = file[-11:-5]
    run = prepareJobConfig(ROOT, options, runNumber, isData)
    
    for file in files:
        run.StoreInputFile(file)

    if options.print_vars:
        ROOT.Anp.Cut.PrintCutNames()        
        
    run.ExecuteRegistry()

    log.info('Run python post processing code...')

    log.info('Local time: %s'    %time.asctime())
    log.info('Total time: %.1fs' %(time.time()-timeStart))

if __name__ == "__main__":
    main()
