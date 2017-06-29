
import logging
import os
import re
import sys

import PhysicsBase.PhysicsBaseConfig as physicsBase

#-----------------------------------------------------------------------------
def getLog(name, level='INFO', debug=True, print_time=False):
    return physicsBase.getLog(name=name, level=level, debug=debug, print_time=print_time)

#-----------------------------------------------------------------------------
def prepareOptionParser():
    
    from optparse import OptionParser
    
    p = OptionParser()

    p.add_option('--run-xml',           type='string', default='run.xml',          dest='xmlfile',     help='xml job file')
    p.add_option('--dirs-key',          type='string', default=None,               dest='dirskey',     help='filter dirs')
    p.add_option('--file-key',          type='string', default='.root',            dest='filekey',     help='file regex')
    p.add_option('--data-key',          type='string', default='data_[0-9]+.root', dest='datakey',     help='data regex')
    
    p.add_option('--output', '-o',      type='string', default=None,           dest='output',        help='')
    p.add_option('--mc-runs',           type='string', default=None,           dest='mc_runs',       help='')
    p.add_option('--mc-keys',           type='string', default=None,           dest='mc_keys',       help='')
    p.add_option('--samples',           type='string', default=None,           dest='samples',       help='')
    p.add_option('--trees',             type='string', default=None,           dest='trees',         help='')
    p.add_option('--save-files',        type='string', default=None,           dest='save_files',    help='')
    p.add_option('--xsec-list',         type='string', default=None,           dest='xsec_list',     help='')
    p.add_option('--xsec-keys',         type='string', default=None,           dest='xsec_keys',     help='')
    p.add_option('--cf-file',           type='string', default=None,           dest='cf_file')
    p.add_option('--ntuple-file',       type='string', default=None,           dest='ntuple_file')
    p.add_option('--tight-ntuple-file', type='string', default=None,           dest='tight_ntuple_file')
    p.add_option('--tp-file',           type='string', default=None,           dest='tp_file')
    p.add_option('--regex-exp',         type='string', default=False,          dest='regex_exp')
    
    p.add_option('--nevent', '-n',      type='int',    default=0,              dest='nevent',        help='number of events')
    p.add_option('--nprint',            type='int',    default=10000,          dest='nprint')
    p.add_option('--xsec-priority',     type='int',    default=0,              dest='xsec_priority', help='')
    p.add_option('--lumi',              type='float',  default=20280.2,        dest='lumi',          help='luminosity in nb')
    p.add_option('--smear-sigma',       type='float',  default=None,           dest='smear_sigma',   help='')

    p.add_option('--batch', '-b',        action='store_true',  default=False, dest='batch')
    p.add_option('--debug', '-d',        action='store_true',  default=False, dest='debug')
    p.add_option('--debug-fit',          action='store_true',  default=False, dest='debug_fit')
    p.add_option('--debug-run',          action='store_true',  default=False, dest='debug_run')
    p.add_option('--debug-prep',         action='store_true',  default=False, dest='debug_prep')
    p.add_option('--debug-reco',         action='store_true',  default=False, dest='debug_reco')    
    p.add_option('--print-vars',         action='store_true',  default=False, dest='print_vars')
    p.add_option('--print-cuts',         action='store_true',  default=False, dest='print_cuts')    
    p.add_option('--print-true',         action='store_true',  default=False, dest='print_true')
    p.add_option('--test', '-t',         action='store_true',  default=False, dest='test')
    
    p.add_option('--dry-run',            action='store_true',  default=False, dest='dryrun')
    p.add_option('--draw',               action='store_true',  default=False, dest='draw')
    p.add_option('--write',              action='store_true',  default=False, dest='write')
    p.add_option('--write-light',        action='store_true',  default=False, dest='write_light')
    p.add_option('--read-flat',          action='store_true',  default=False, dest='read_flat')
    p.add_option('--read-tth',           action='store_true',  default=False, dest='read_tth')
    p.add_option('--read-blind-data',    action='store_true',  default=False, dest='read_blind_data')    
    
    p.add_option('--match-pv',           action='store_true',  default=False, dest='match_pv')
    p.add_option('--nocut-pv',           action='store_true',  default=False, dest='nocut_pv')
    p.add_option('--track-rob',          action='store_true',  default=False, dest='track_rob')
    p.add_option('--track-new',          action='store_true',  default=False, dest='track_new')
    p.add_option('--track-vtx',          action='store_true',  default=False, dest='track_vtx')
    p.add_option('--track-sil',          action='store_true',  default=False, dest='track_sil')
    p.add_option('--study-recoil',       action='store_true',  default=False, dest='study_recoil')
    p.add_option('--study-stvf',         action='store_true',  default=False, dest='study_stvf')
    p.add_option('--study-hmm',          action='store_true',  default=False, dest='study_hmm')
    p.add_option('--mpt-cluster',        action='store_true',  default=False, dest='mpt_cluster')
    p.add_option('--add-cone',           action='store_true',  default=False, dest='add_cone')
    p.add_option('--do-4l-regions',      action='store_true',  default=False, dest='regions_4l')
    p.add_option('--study-4l-fakes',     action='store_true',  default=False, dest='study_4l_fakes')
    p.add_option('--do-aux-4l',          action='store_true',  default=False, dest='do_aux_4l')
    p.add_option('--do-bucket',          action='store_true',  default=False, dest='do_bucket')

    p.add_option('--opt-vbf',            action='store_true',  default=False, dest='opt_vbf')
    p.add_option('--opt-ggf',            action='store_true',  default=False, dest='opt_ggf')
    p.add_option('--opt-elec',           action='store_true',  default=False, dest='opt_elec')
    p.add_option('--opt-muon',           action='store_true',  default=False, dest='opt_muon')
    p.add_option('--opt-elec-2l',        action='store_true',  default=False, dest='opt_elec_2l')
    p.add_option('--opt-muon-2l',        action='store_true',  default=False, dest='opt_muon_2l')    
    p.add_option('--opt-elec-4l',        action='store_true',  default=False, dest='opt_elec_4l')
    p.add_option('--opt-muon-4l',        action='store_true',  default=False, dest='opt_muon_4l')
    p.add_option('--opt-tau-cr',         action='store_true',  default=False, dest='opt_tau_cr')    
    p.add_option('--opt-calc-mm',        action='store_true',  default=False, dest='opt_calc_mm')    
    p.add_option('--opt-elec-4l-1d',     action='store_true',  default=False, dest='opt_elec_4l_1d')
    p.add_option('--opt-muon-4l-1d',     action='store_true',  default=False, dest='opt_muon_4l_1d')
    p.add_option('--opt-sr-4l',          action='store_true',  default=False, dest='opt_SR_4l')
    
    p.add_option('--do-clust',           action='store_true',  default=False, dest='do_clust')
    p.add_option('--do-light',           action='store_true',  default=False, dest='do_light')
    p.add_option('--do-jetpt',           action='store_true',  default=False, dest='do_jetpt')
    p.add_option('--do-timer',           action='store_true',  default=False, dest='do_timer')
    p.add_option('--do-truth',           action='store_true',  default=False, dest='do_truth')
    p.add_option('--do-hww',             action='store_true',  default=False, dest='do_hww')
    p.add_option('--do-hmm-bjet',        action='store_true',  default=False, dest='do_hmm_bjet')
    p.add_option('--do-split',           action='store_true',  default=False, dest='do_split')
    p.add_option('--do-fit',             action='store_true',  default=False, dest='do_fit')
    p.add_option('--do-minos',           action='store_true',  default=False, dest='do_minos')
    p.add_option('--is-data',            action='store_true',  default=False, dest='is_data')
    
    p.add_option('--no-batch',           action='store_true',  default=False, dest='no_batch')
    p.add_option('--no-track',           action='store_true',  default=False, dest='no_track')
    p.add_option('--no-clust',           action='store_true',  default=False, dest='no_clust')    
    p.add_option('--no-truth',           action='store_true',  default=False, dest='no_truth')
    p.add_option('--no-weight',          action='store_true',  default=False, dest='no_weight')
    p.add_option('--no-opt',             action='store_true',  default=False, dest='no_opt')
    p.add_option('--no-data',            action='store_true',  default=False, dest='no_data')
    p.add_option('--no-mc',              action='store_true',  default=False, dest='no_mc')
    p.add_option('--no-nexus',           action='store_true',  default=False, dest='no_nexus')
    p.add_option('--no-overlap-removal', action='store_true',  default=False, dest='no_overlap_removal')
    p.add_option('--no-mll-cut',         action='store_true',  default=False, dest='no_mll_cut')
    p.add_option('--use-raw-counts',     action='store_true',  default=False, dest='use_raw_counts')

    #---------------------------------------------------------------------------------------------------
    # ttH options
    #
    p.add_option('--is-blinded',         type='int',           default=None,  dest='is_blinded')
    p.add_option('--is-cleaned',         type='int',           default=None,  dest='is_cleaned')

    p.add_option('--do-plot-2l',         action='store_true',  default=False, dest='do_plot_2l')
    p.add_option('--do-plot-3l',         action='store_true',  default=False, dest='do_plot_3l')
    p.add_option('--do-plot-4l',         action='store_true',  default=False, dest='do_plot_4l')
    p.add_option('--do-plot-lep',        action='store_true',  default=False, dest='do_plot_lep')
    p.add_option('--do-plot-tight',      action='store_true',  default=False, dest='do_plot_tight')
    p.add_option('--do-plot-nocut',      action='store_true',  default=False, dest='do_plot_nocut')
    p.add_option('--do-plot-bjet',       action='store_true',  default=False, dest='do_plot_bjet')
    p.add_option('--do-plot-comb',       action='store_true',  default=False, dest='do_plot_comb')
    p.add_option('--do-plot-theo',       action='store_true',  default=False, dest='do_plot_theo')
    p.add_option('--do-plot-obj',        action='store_true',  default=False, dest='do_plot_obj')

    p.add_option('--do-CF',              action='store_true',  default=False, dest='do_CF')
    p.add_option('--do-fake-2ld',        action='store_true',  default=False, dest='do_fake_2ld')
    p.add_option('--do-fake-3ld',        action='store_true',  default=False, dest='do_fake_3ld')
    p.add_option('--do-fake-4l',         action='store_true',  default=False, dest='do_fake_4l')
    p.add_option('--do-1lep-2tau',       action='store_true',  default=False, dest='do_1lep_2tau')
    p.add_option('--do-ntuple-dilepton', action='store_true',  default=False, dest='do_ntuple_dilepton')
    p.add_option('--do-ntuple-fake',     action='store_true',  default=False, dest='do_ntuple_fake')
    p.add_option('--do-ntuple-truth',    action='store_true',  default=False, dest='do_ntuple_truth')
    p.add_option('--do-ntuple-pass-all', action='store_true',  default=False, dest='do_ntuple_pass_all')
    p.add_option('--do-ntuple-ditau',    action='store_true',  default=False, dest='do_ntuple_ditau')
    p.add_option('--do-nexus-file',      action='store_true',  default=False, dest='do_nexus_file')
    p.add_option('--do-count-copy',      action='store_true',  default=False, dest='do_count_copy')
    p.add_option('--do-study-top',       action='store_true',  default=False, dest='do_study_top')
    p.add_option('--do-study-tau',       action='store_true',  default=False, dest='do_study_tau')
    p.add_option('--do-study-new-tau',   action='store_true',  default=False, dest='do_study_new_tau')
    p.add_option('--do-study-proc',      action='store_true',  default=False, dest='do_study_proc')
    p.add_option('--do-study-PS',        action='store_true',  default=False, dest='do_study_PS')
    p.add_option('--do-full-truth',      action='store_true',  default=False, dest='do_full_truth')
    p.add_option('--do-prep-top',        action='store_true',  default=False, dest='do_prep_top')
    p.add_option('--do-flat-ntuple',     action='store_true',  default=False, dest='do_flat_ntuple')
    p.add_option('--do-sys-divide',      action='store_true',  default=False, dest='do_sys_divide')
    p.add_option('--do-btag-weight',     action='store_true',  default=False, dest='do_btag_weight')
    p.add_option('--do-btag-60',         action='store_true',  default=False, dest='do_btag_60')
    p.add_option('--do-btag-80',         action='store_true',  default=False, dest='do_btag_80')
    p.add_option('--do-read-ntp',        action='store_true',  default=False, dest='do_read_ntp')
    p.add_option('--do-test',            action='store_true',  default=False, dest='do_test')

    p.add_option('--do-probe-pt',        type='int',           default=None,  dest='do_probe_pt')
    p.add_option('--do-probe-syst',      type='string',        default=None,  dest='do_probe_syst')
    p.add_option('--flip-fit-file',      type='string',        default=None,  dest='flip_fit_file')
    p.add_option('--flip-fit-opt',       type='string',        default=None,  dest='flip_fit_opt')
    p.add_option('--prep-sys',           type='string',        default=None,  dest='prep_sys')
    p.add_option('--sys-weight',         type='string',        default=None,  dest='sys_weight')
    p.add_option('--add-weight',         type='string',        default=None,  dest='add_weight')
    p.add_option('--set-weight',         type='string',        default=None,  dest='set_weight')

    p.add_option('--do-elec-cut',        type='string',        default=None,  dest='do_elec_cut')    
    p.add_option('--do-muon-cut',        type='string',        default=None,  dest='do_muon_cut')
    p.add_option('--do-tau-cut' ,        type='string',        default=None,  dest='do_tau_cut' )

    p.add_option('--do-OLR' ,            type='string',        default=None,  dest='do_OLR' )

    p.add_option('--no-lep-cuts',        action='store_true',  default=False, dest='no_lep_cuts')

    return p

clog = getLog(os.path.basename(__file__))

#-----------------------------------------------------------------------------
def selectInputFiles(root_files, options):
    """    
    select input files
    """
    import PhysicsTaus.PhysicsTausXSec as XSec
    
    files = []

    #
    # Read xsec file
    #
    if options.xsec_keys and os.path.isfile(options.xsec_keys):
        xfile = open(options.xsec_keys, 'r')
        
        xsecs = []
        testa = os.getenv('TestArea', '')

        for xline in xfile.readlines():
            xpath = xline.replace('$TestArea', testa).strip()

            if os.path.isfile(xpath):
                clog.info('Read xsec file: %s' %xpath)                
                xsecs += XSec.readXSecFile(xpath, options.xsec_priority)
            else:
                clog.warning('Missing xsec file: %s' %xpath)
                
        clog.info('Selected %d xsec entries:' %len(xsecs))

        for xsec in xsecs:
            print xsec.GetLine()
    else:
        xsecs = None
        
    #
    # Select data files and separate MC files
    #
    data_files = []
    notd_files = []

    for f in root_files:
        file = os.path.basename(f)

        if re.search(options.datakey, file) != None:
            if not options.no_data:
                data_files += [f]
                clog.info('data   - %s' %(f))
            continue
        
        if not options.no_mc and re.search('data', file) == None:
            notd_files += [f]

    #
    # Select MC files
    #
    siml_files = []
    
    for f in notd_files:
        file = os.path.basename(f)

        if xsecs == None or (options.mc_keys != None and re.search(options.mc_keys, sample.key) != None):
            clog.info('siml   - %s' %(f))
            siml_files += [f]
            continue
        
        if xsecs:
            match_file = False
            
            for xsec in xsecs:
                key = str(xsec.run)
            
                if re.search(key, file) != None:
                    match_file = True
                    break
            
            if match_file:
                clog.info('%s - %s  %s' %(key, f, xsec.process))
                siml_files += [f]

                if xsec.match:
                    clog.warning('Duplicate files for: %s' %xsec.GetLine())
                else:
                    xsec.match = True
                    
    if xsecs:
        miss_xsecs = []
        
        for xsec in xsecs:
            if not xsec.match:
                miss_xsecs += [xsec]
        
        clog.info('Missing files for following %d processes:' %len(miss_xsecs))

        for xsec in miss_xsecs:
            print '   %s' %xsec.GetLine()

    #
    # Consistency check for input files
    #
    files = data_files + siml_files
    names = set()

    for fpath in files:
        fname = os.path.basename(fpath)
        
        if fname in names:
            clog.error('selectInputFiles - abort: duplicate file: %s' %fpath)
            sys.exit(1)
        else:
            names.add(fname)        

    clog.info('selected data  files: %d' %len(data_files))
    clog.info('selected MC    files: %d' %len(siml_files))
    clog.info('selected total files: %d' %len(names))    

    return files

#-----------------------------------------------------------------------------
def _processLocalDirectory(path, filekey, dirkey):
    
    """    
    Recursive directory search
    """
    
    files = []

    if os.path.isfile(path):
        if type(filekey) == type(''):
            matchFile = (re.search(filekey, path) != None)
        elif type(filekey) == type([]):
            matchFile = False
            
            for fkey in filekey:
                #
                # AND of multiple keys
                #
                parts = fkey.split(',')
                npass = 0
                
                for part in parts:
                    if (re.search(part, path) != None):
                        npass += 1
                if npass == len(parts) and len(parts) > 0:
                    matchFile = True
                    break
        else:
            matchFile = True
            
        if matchFile:
            files += [path]
            
        return files

    if not os.path.isdir(path):
        clog.error('Invalid path: %s' %path)
        sys.exit(1)

    for file in os.listdir(path):
        
        fpath = path.rstrip('/')+'/'+file

        if file in ['.', '..']:
            continue

        if os.path.isdir(fpath):
            
            if type(dirkey) == type(''):
                matchDir = (re.search(dirkey, fpath) != None)
            else:
                matchDir = True
                
            if matchDir:
                files += _processLocalDirectory(fpath, filekey, dirkey)
        else:
            files += _processLocalDirectory(fpath, filekey, dirkey)

    return files

#-----------------------------------------------------------------------------
def findInputLocalFiles(paths, filekey='.root', dirkey=None, save_files=None, print_files=True):

    """    
    Top level function for recursive directory search:
      - include files that match filekey pattern
      - descend directory that matches dirkey pattern
    """

    if type(paths) != type([]) or len(paths) < 1:
        clog.warning('findInputLocalFiles - missing input path(s)')
        return []

    files = []

    for path in paths:

        parts = path.split(',')
        for ipart in parts:
            clog.info('Search input path: %s' %ipart)
        
            if ipart.count('eos'):
                files += [ipart]
            else:
                files += _processLocalDirectory(ipart, filekey, dirkey)

    if print_files:
        for f in sorted(files):
            clog.info('Input file %8d kB: %s' %(os.path.getsize(f)/1024, f))

    if save_files != None:
        sfile = open(save_files, 'w')

        for f in sorted(files):
            sfile.write('%s\n' %f)

        sfile.close()

    return files

#-----------------------------------------------------------------------------
def loadPhysicsTausLib(ROOT, options):

    clog.info('loadPhysicsTausLib - load shared libraries')

    physicsBase.loadPhysicsBase()     
    setPlotDefaults(ROOT)

    if (hasattr(options, 'draw') and options.draw) or (hasattr(options, 'wait') and options.wait):
        clog.info('loadPhysicsTausLib - draw options enabled... set plotting style')
    else:
        ROOT.gROOT.SetBatch(True)
        clog.info('loadPhysicsTausLib - set ROOT batch mode')
    
    ROOT.gSystem.Load('libPhysicsData')
    ROOT.gSystem.Load('libPhysicsDataDict')
    ROOT.gSystem.Load('libPhysicsTaus')
    ROOT.gSystem.Load('libPhysicsTausDict')

#-----------------------------------------------------------------------------
def prepareRunModule(ROOT, options, hist_config=None):
    loadPhysicsTausLib(ROOT, options)
    
    run = physicsBase.RunModule()

    run.SetKey('TreeName',   'sum config event')
    run.SetKey('TreeThread', 'event')
    run.SetKey('PrintFiles', 'no')
    run.SetKey('NPrint',      options.nprint)
    run.SetKey('NEvent',      options.nevent)
    run.SetKey('FileKeys',   '.root')
    run.SetKey('CloseFile',  'yes')
    run.SetPar('HistMan::Debug', 'no')
    run.SetPar('HistMan::Sumw2', 'yes')    

    if options.output:
        run.SetKey('OutputFile', options.output)
        
    testArea = os.environ.get('TestArea')

    #=========================================================================    
    # Set recursively global parameters
    #
    if options.debug:        
        run.SetGlobalPar('Debug', options.debug)
        run.SetKey('Debug', 'yes')
    else:
        run.SetKey('Debug', 'no')
    
    #=========================================================================    
    # Read histogram definitions
    #
    histman = ROOT.Anp.HistMan.Instance()
    
    testArea = os.environ.get('TestArea')

    if hist_config == None:
        hist_config = ['config']

    for hconfig in hist_config:
        histPath = '%s/PhysicsNtuple/PhysicsTaus/%s' %(testArea, hconfig)
    
        if os.path.isdir(histPath):
            for file in os.listdir(histPath):
                if file.count('.xml'):
                    histman.ReadFile(histPath+file)

    top_algs = []
    
    #=========================================================================
    # Configure object algs
    #
    prep_jet   = physicsBase.AlgConfig('prepJet',   'PrepJet')
    prep_met   = physicsBase.AlgConfig('prepMet',   'PrepMet')
    prep_elec  = physicsBase.AlgConfig('prepElec',  'PrepElec')
    prep_muon  = physicsBase.AlgConfig('prepMuon',  'PrepMuon')
    prep_track = physicsBase.AlgConfig('prepTrack', 'PrepTrack')
    prep_vtx   = physicsBase.AlgConfig('prepVtx',   'PrepVtx')
    prep_truth = physicsBase.AlgConfig('prepTruth', 'PrepTruth')

    top_algs += [prep_jet,
                 prep_met,
                 prep_elec,
                 prep_muon,
                 prep_track,
                 prep_vtx,                 
                 prep_truth]

    trig_muon = ['EF_mu18_MG', 'EF_mu18_MG_medium']
    trig_elec = ['EF_e20_medium', 'EF_e22_medium', 'EF_e22vh_medium1']
    
#    for trig in trig_muon:
#        physicsTrig.addTrigCuts(prep_muon, trig)
        
#    for trig in trig_elec:
#        physicsTrig.addTrigCuts(prep_elec, trig)
        
    prep_muon.SetKey('TrigMuon', ','.join(trig_muon))
    prep_elec.SetKey('TrigElec', ','.join(trig_elec))

    if options.smear_sigma > 0.0:
        prep_track.SetKey('SmearSigma', options.smear_sigma)                

    return (run, top_algs)

#-----------------------------------------------------------------------------
def prepareReadNtuple(ROOT, options, tree='HWWTree', hist_config=None, load_libs=True):

    if load_libs:
        loadPhysicsTausLib(ROOT, options)
    
    run = ReadNtuple('readNtuple')
    run.SetKey('TreeName',       tree)
    run.SetKey('PrintFiles',     'no')
    run.SetKey('NPrint',         options.nprint)
    run.SetKey('NEvent',         options.nevent)
    run.SetKey('FileKeys',       '.root')
    run.SetKey('CloseFile',      'yes')
    run.SetKey('Print',          'yes')
    run.SetPar('HistMan::Debug', 'no')
    run.SetPar('HistMan::Sumw2', 'yes')
    
    if options.output:
        run.SetKey('OutputFile', options.output)
        
    testArea = os.environ.get('TestArea')

    #
    # Set recursively global parameters
    #
    if options.debug:        
        run.SetGlobalPar('Debug', options.debug)
        run.SetKey('Debug', 'yes')
    else:
        run.SetKey('Debug', 'no')

    #
    # Read MC run normalization
    #
    if options.mc_runs != None:
        rfile = open(options.mc_runs, 'r')
        runs  = []

        for iline in rfile:
            if iline.count('#'):
                continue
            
            parts = iline.split()
            if len(parts) > 1:
                
                mcrun  = parts[0]
                weight = float(parts[1])

                run.SetKey(mcrun, weight)
                runs += [mcrun]

                clog.debug('mcrun=%s weight=%e' %(mcrun, weight))

        run.SetKey('Runs', ','.join(runs))
        
    #
    # Read histogram definitions
    #
    histman = ROOT.Anp.HistMan.Instance()
    
    testArea = os.environ.get('TestArea')

    if hist_config == None:
        hist_config = ['config']

    for hconfig in hist_config:
        histPath = '%s/PhysicsNtuple/PhysicsTaus/%s/' %(testArea, hconfig.rstrip('/'))
        
        if os.path.isdir(histPath):
            clog.info('prepareReadNtuple - read xml files from: %s' %histPath)
                    
            for file in os.listdir(histPath):
                if file.count('.xml'):
                    histman.ReadFile(histPath+file)
        else:
            clog.warning('prepareReadNtuple - invalid path: %s' %histPath)
            
    return run

#-------------------------------------------------------------------------
class CutItem:
    """CutItem - python wrapper for C++ CutItem class"""
        
    def __init__(self, name, conf='', weight=None, abs=False, hash=None, dummy=False):

        self.cut_name  = name
        self.cut_conf  = ''
        self.raw_conf  = conf
        self.cut_wkey  = weight
        self.cut_reg   = physicsBase.getRegistry()
        self.list_and  = []
        self.list_or   = []
        self.sub_cuts  = {}
        self.hash      = hash
        self.abs       = abs
        self.weight    = None

        if name.count(' '):
            raise Exception('CutItem - name must not include space character: %s' %name)

        if conf.count('(') == 0 and conf.count(')') == 0:
            self.cut_conf = conf
        elif conf.count('(') != conf.count(')'):
            raise Exception('CutItem - bad conf string: %s' %conf)
        else:
            self.AddSubCuts(name, conf)

        self.cut_reg.SetVal('CutName',   self.cut_name)
        self.cut_reg.SetVal('CutConf',   self.cut_conf)

        self.SetHash(self.hash)

        if abs:
            self.cut_reg.SetVal('CutUseAbs', 'yes')
        else:
            self.cut_reg.SetVal('CutUseAbs', 'no')
        
        if type(self.cut_wkey) == type(''):
            self.cut_reg.SetVal('CutWeights', self.cut_wkey)
        elif type(self.cut_wkey) == type([]):
            self.cut_reg.SetVal('CutWeights', ','.join(self.cut_wkey))

        if dummy:
            self.cut_reg.SetVal('CutDummy', 'yes')
        
    def IsValidConf(self, conf):
        
        if type(conf) != type(''):
            raise TypeError('CutItem::IsValidConf - invalid input type: %s' %conf)

        #
        # Lists of known operators
        #        
        oprs = ['>=', '>', '<=', '<', '==', '!=']

        #
        # Split using logical AND=&& or OR==|| and check that expressions are valid
        #
        if conf.count('&&') and conf.count('||'):
            raise TypeError('CutItem - conf="%s" can not contain both && and || operators' %conf)
            
            sys.exit(1)
        elif conf.count('||'):
            confs = conf.split('||')
        else:
            confs = conf.split('&&')

        for v in confs:
            if len(v) == 0:
                continue

            for o in oprs:
                if v.find(o) > 0:
                    break
                o = None
                
            if o == None:
                raise TypeError('CutItem::IsValidConf - "%s" does not contain known operator' %conf)

            parts = v.split(o)

            if len(parts) != 2:
                raise TypeError('CutItem - conf="%s" has invalid logic %s: %s, %s' %(conf, v, o, parts))

            var_name = parts[0]
            var_comp = parts[1]

            try:
                var_comp = float(var_comp)
            except ValueError:
                log.error('CutItem - conf="%s" contains comparison with non-number' %conf)
                sys.exit(1)
                
            #
            # Logical expression is valid and contains only known variables
            #
            log.debug('CutItem - conf="%s": %s %s %f' %(conf, var_name, o, var_comp))         

        return True

    def GetRegistry(self):

        self.cut_reg.RemoveKey('SubCuts')
        self.cut_reg.SetVal   ('SubCuts',  ','.join(self.sub_cuts.keys()))

        for key, cut in self.sub_cuts.iteritems():
            
            key = 'SubCut_%s' %cut.GetCutName()
            self.cut_reg.SetVal(key,                             cut.GetCutName ())
            self.cut_reg.SetVal('%s%s' %(key, cut.GetCutName()), cut.GetRegistry())
        
        return self.cut_reg

    def GetCutName(self):
        return self.cut_name

    def GetCutConf(self):
        return self.cut_conf

    def GetRawConf(self):
        return self.raw_conf

    def AddCut(self, cut, option):

        if cut.GetCutName() in self.list_and + self.list_or:
            raise Exception('AddCut - duplicate cut: %s' %cut.GetCutName())
            
        if   option.lower() == 'and': self.list_and += [cut.GetCutName()]
        elif option.lower() == 'or' : self.list_or  += [cut.GetCutName()]
        else: 
            raise Exception('AddCut - unknown option %s: %s' %(cut.GetCutName(), option))

        self.cut_reg.SetVal(cut.GetCutName(), cut.GetRegistry())
        self.cut_reg.SetVal('ListAND',        ','.join(self.list_and))
        self.cut_reg.SetVal('ListOR',         ','.join(self.list_or))

        if len(self.list_and) > 0 and len(self.list_or) > 0:
            raise Exception('AddCut - both AND and OR are defined - logic error: %s' %cut.GetCutName())

        return True

    def SetHash(self, hash):
        if type(hash) == type(''):
            self.hash = hash
            self.cut_reg.SetVal('CutHash', hash)

    def AddSubCut(self, cut):

        if cut.GetCutName() in self.sub_cuts:
            clog.warning('AddSubCut - duplicate cut: %s' %cut.GetCutName())
            return False
            
        self.sub_cuts[cut.GetCutName()] = cut

        return True

    def AddSubCuts(self, name, conf):

        clog.debug('AddSubCuts - parsing conf str: %s' %conf)

        parts = re.split(r'[()]', conf)

        do_and = 0
        do_or  = 0

        for part in parts:
            if part.strip() == '||':
                do_or += 1
            if part.strip() == '&&':
                do_and += 1
                
        if do_and and do_or:
            raise Exception('CutItem::AddSubCuts - can not use both "||" and "&&": %s' %conf)

        if not do_and and not do_or:
            raise Exception('CutItem::AddSubCuts - missing required "||" or "&&": %s\n   %s' %(conf, parts))

        for i in range(0, len(parts)):
            clog.debug('AddSubCuts - parts[%d] = %s' %(i, part))
            
            part = parts[i].strip()
            
            if part.strip() in ['||', '&&'] or len(part) == 0:
                continue

            cut = CutItem(name='%s_%d' %(name, i), conf=part, weight=self.weight, abs=self.abs)

            if do_and:
                self.AddCut(cut, 'and')
            if do_or:
                self.AddCut(cut, 'or')            

        return True

    def CutAsStr(self):
        text  = '%s: %s' %(self.cut_name, self.cut_conf)

        return text

    def __repr__(self):
        return self.CutAsStr()

#-------------------------------------------------------------------------
def getCutsAsLatex(cuts, outfile=None):

    if cuts == None:
        return ''

    text  = '\\resizebox{1.0\\textwidth}{!}{\n'
    text += '\\begin{tabular}{|l|l|}\n'
    text += '\\hline\n'
    text += 'Name & Definition\\\\\n'
    text += '\\hline\n'

    nwid = 4

    for cut in cuts:
        cut_name = getattr(cut, 'GetCutName', None)

        if cut_name:
            nwid = max([nwid, len(cut_name())])
    
    for cut in cuts:
        cut_name = getattr(cut, 'GetCutName', None)
        cut_conf = getattr(cut, 'GetRawConf', None)

        if cut_name and cut_conf:
            text += '%s & $%s$\\\\\n' %(cut_name().ljust(nwid), cut_conf().replace('&', '\\&'))

    text += '\\hline\n'
    text += '\\end{tabular}}\n'

    for c, n in getCutVarNames().iteritems():
        text = text.replace(c, n)

    if outfile:
        ofile = open(outfile, 'w')
        ofile.write(text)
        ofile.close()

    return text
        
#---------------------------------------------------------------------
#
# Python configurable for Anp::ReadNtuple class
#   - configure input files
#   - configure algorithms
#   - configure histograms
#
class ReadNtuple:
    """ReadNtuple - configures input files and algorithms"""

    def __init__(self, alg_name):
        import ROOT
        
        self._alg   = None
        self._reg   = physicsBase.getRegistry()
        self._run   = ROOT.Anp.ReadNtuple()
        self._files = []
        self._dirs  = []        
        self._log   = getLog('ReadNtuple')        
        self._hist  = physicsBase.getRegistry()
        self._hist.AllowNonUniqueKeys()
        self._gpar  = {}

        self.SetPar('AlgName', alg_name)
        self.SetPar('AlgType', 'ReadNtuple')
        
    def Print(self):
        plog.info('ReadNtuple: print children algorithms:')
        self._alg.Print()

    #
    # Functions for python configuration
    #
    def SetKey(self, key, value):
        physicsBase.setRegistryKey(self._reg, 'ReadNtuple::'+key, value)

    def SetPar(self, key, value):
        physicsBase.setRegistryKey(self._reg, key, value)

    def SetGlobalPar(self, key, value):
        physicsBase.setRegistryKey(self._reg, key, value)
        
        if self._alg:
            self._alg.SetGlobalPar(key, value)
        else:
            self._gpar[key] = value

    def SetTopAlg(self, alg):
        if self._alg != None:
            self._log.warning('SetTopAlg - top algorithm is already configured')
            return
        
        self._alg = alg

        for key, value in self._gpar.iteritems():
            self._alg.SetGlobalPar(key, value)
            
        self._gpar = {}

    def AddTopAlg(self, alg_name, alg_list, Print="yes"):
        if self._alg != None:
            self._log.warning('AddTopAlg - Top algorithm is already configured')
            return
        
        self._alg = physicsBase.AlgConfig(alg_name, 'RunAlgs')
        self._alg.SetKey('Print', Print)
        self._alg.AddAlg(alg_list)

        for key, value in self._gpar.iteritems():
            self._alg.SetGlobalPar(key, value)
        self._gpar = {}

    def AddHistFile(self, hfile):
        self._hist.Set('ReadFile', hfile)

    def StoreInputFile(self, file):
        self._log.debug('StoreInputFile: '+file)
        self._files += [file]
                    
    def GetRegistryConfig(self):
        if type(self._alg) == None:
            self._log.warning('Missing top algorithm')
            return None

        reg = physicsBase.getRegistry()
        
        reg.Merge(self._reg)

        if self._alg != None:
            physicsBase.setRegistryKey(reg, 'ReadNtuple::SubAlgType', self._alg.GetAlgType())
            physicsBase.setRegistryKey(reg, 'ReadNtuple::SubAlgName', self._alg.GetAlgName())
            physicsBase.setRegistryKey(reg, self._alg.GetAlgName(),   self._alg.GetConfigRegistry())
            
        physicsBase.setRegistryKey(reg, 'ReadNtuple::HistMan', self._hist)

        input_reg = physicsBase.getRegistry()
        input_reg.AllowNonUniqueKeys()
        
        for file in self._files:
            physicsBase.setRegistryKey(input_reg, 'File', file)
            
        physicsBase.setRegistryKey(reg, 'ReadNtuple::InputFiles', input_reg)
        return reg
    
    #
    # Wrappers for C++ code: ReadNtuple member functions
    #
    def Execute(self, reg_path):
        self._run.Execute(reg_path)

    def ExecuteRegistry(self):
        self._run.ExecuteRegistry(self.GetRegistryConfig())


#-----------------------------------------------------------------------------
class Pushd:

    init_dir = None
    orig_dir = None

    def __init__(self, dirname):
        self.init_dir = os.path.realpath(dirname)
        self.orig_dir = None
        
    def __enter__(self):
        self.orig_dir = os.getcwd()
        os.chdir(self.init_dir)
        
        clog.debug('Pushd.__enter__ - CWD=%s' %os.getcwd())
        return self
 
    def __exit__(self, type, value, tb):
        os.chdir(self.orig_dir)
        clog.debug('Pushd.__exit__ - CWD=%s' %os.getcwd())

#-----------------------------------------------------------------------------
# Load necessary shared libraries
#
def setPlotDefaults(root, options = None):

    root.gROOT.SetStyle('Plain')

    root.gStyle.SetFillColor(10)           
    root.gStyle.SetFrameFillColor(10)      
    root.gStyle.SetCanvasColor(10)         
    root.gStyle.SetPadColor(10)            
    root.gStyle.SetTitleFillColor(0)       
    root.gStyle.SetStatColor(10)   
    
    root.gStyle.SetCanvasBorderMode(0)
    root.gStyle.SetFrameBorderMode(0) 
    root.gStyle.SetPadBorderMode(0)   
    root.gStyle.SetDrawBorder(0)      
    root.gStyle.SetTitleBorderSize(0)
    
    root.gStyle.SetFuncWidth(2)
    root.gStyle.SetHistLineWidth(2)
    root.gStyle.SetFuncColor(2)
    
    root.gStyle.SetPadTopMargin(0.08)
    root.gStyle.SetPadBottomMargin(0.16)
    root.gStyle.SetPadLeftMargin(0.14)
    root.gStyle.SetPadRightMargin(0.14)
  
    # set axis ticks on top and right
    root.gStyle.SetPadTickX(1)         
    root.gStyle.SetPadTickY(1)         
  
    # Set the background color to white
    root.gStyle.SetFillColor(10)           
    root.gStyle.SetFrameFillColor(10)      
    root.gStyle.SetCanvasColor(10)         
    root.gStyle.SetPadColor(10)            
    root.gStyle.SetTitleFillColor(0)       
    #root.gStyle.SetStatColor(10)     
    root.gStyle.SetStatColor(0)
    root.gStyle.SetStatStyle(4000)      
  
  
    # Turn off all borders
    root.gStyle.SetCanvasBorderMode(0)
    root.gStyle.SetFrameBorderMode(0) 
    root.gStyle.SetPadBorderMode(0)   
    root.gStyle.SetDrawBorder(0)      
    root.gStyle.SetTitleBorderSize(0) 
  
    # Set the size of the default canvas
    root.gStyle.SetCanvasDefH(400)          
    root.gStyle.SetCanvasDefW(650)          
    #gStyle->SetCanvasDefX(10)
    #gStyle->SetCanvasDefY(10)   
  
    # Set fonts
    font = 42
    root.gStyle.SetLabelFont(font,'xyz')
    root.gStyle.SetStatFont(font)       
    root.gStyle.SetTitleFont(font)      
    root.gStyle.SetTitleFont(font,'xyz')
    root.gStyle.SetTextFont(font)       
    root.gStyle.SetTitleX(0.3)        
    root.gStyle.SetTitleW(0.4)        
  
   # Set Line Widths
   #gStyle->SetFrameLineWidth(0)
    root.gStyle.SetFuncWidth(2)
    root.gStyle.SetHistLineWidth(2)
    root.gStyle.SetFuncColor(2)
  
   # Set tick marks and turn off grids
    root.gStyle.SetNdivisions(505,'xyz')
  
   # Set Data/Stat/... and other options
    root.gStyle.SetOptDate(0) 
    root.gStyle.SetDateX(0.1)
    root.gStyle.SetDateY(0.1)
   #gStyle->SetOptFile(0)
    #root.gStyle.SetOptStat(1110)
    root.gStyle.SetOptStat('em') # add m for mean
    root.gStyle.SetOptFit(111)
    root.gStyle.SetStatFormat('6.3f')
    root.gStyle.SetFitFormat('6.3f')
    root.gStyle.SetPaintTextFormat('0.2f')

   #gStyle->SetStatTextColor(1)
   #gStyle->SetStatColor(1)
   #gStyle->SetOptFit(1)
   #gStyle->SetStatH(0.20)
   #gStyle->SetStatStyle(0)
   #gStyle->SetStatW(0.30)
   #gStyle -SetStatLineColor(0)
   #root.gStyle.SetStatX(0.919)
    root.gStyle.SetStatX(0.880) # right
   # root.gStyle.SetStatX(0.470) # left
   #root.gStyle.SetStatY(0.919)
    root.gStyle.SetStatY(0.870) 
   # root.gStyle.SetStatY(0.864)
    root.gStyle.SetOptTitle(0)
   #gStyle->SetStatStyle(0000)    # transparent mode of Stats PaveLabel
    root.gStyle.SetStatBorderSize(0)

    root.gStyle.SetLabelSize(0.065,'xyz')
   #gStyle -> SetLabelOffset(0.005,'xyz')
   #gStyle -> SetTitleY(.98)
    root.gStyle.SetTitleOffset(1.05,'xz')
    root.gStyle.SetTitleOffset(1.20,'y')
    root.gStyle.SetTitleSize(0.065, 'xyz')
    root.gStyle.SetLabelSize(0.065, 'xyz')
    #root.gStyle.SetTextAlign(22)
    #root.gStyle.SetTextSize(0.12)
    root.gStyle.SetTextSize(0.08)
    
    #root.gStyle.SetPaperSize(root.TStyle.kA4)  
    root.gStyle.SetPalette(1)
  
   #root.gStyle.SetHistMinimumZero(True)
   
    root.gROOT.ForceStyle()

#--------------------------------------------------------------------------------
def getCandName(cand):
    
    if hasattr(cand, 'GetAlgName'):
        return cand.GetAlgName()
    elif type(cand) == str:
        return cand
    
    raise Exception('getPlotCand - wrong cand object type="%s" - exit now' %type(cand))

#--------------------------------------------------------------------------------
canvases = {}

def getCanvas(name, x, y, ny=None):

    import ROOT

    global canvases

    if name in canvases:
        c = canvases[name]
        c.cd()
        c.Draw() 
        return c

    c = ROOT.TCanvas(name, name, x, y)
    
    if ny:
        c.Divide(1, ny, 0.01, 0.01, 0)

    c.cd()
    c.Draw()           

    canvases[name] = c
    return c

#--------------------------------------------------------------------------------
def getCutVarNames():
    
    names = {'$Z0Sin':'$|z_{0}sin\\theta|',
             '$D0Sig':'$|\sigma(d_{0})|',
             }
    
    return names

#--------------------------------------------------------------------------------
def getCutValue(cuts, name, default):

    if type(cuts) != dict:
        return default

    try:
        return cuts[name]
    except KeyError:
        return default   

