import os
import sys
import re

import PhysicsBase.PhysicsBaseConfig as physicsBase
import PhysicsTaus.PhysicsTausConfig as physicsConfig
import PhysicsTaus.PhysicsTausCuts   as physicsCuts
import PhysicsTaus.PhysicsTausBase   as physicsTauBase

from PhysicsTaus.PhysicsTausConfig  import CutItem
from PhysicsTaus.PhysicsTausConfig  import getCandName

clog = physicsConfig.getLog(os.path.basename(__file__))
#========================================================================================
# Configuration for algorithms
#========================================================================================
def prepareJobConfig(ROOT, options, runNumber, isData):

    clog.info('Prepare python configuration...')

    prep_algs = [getPrepReco('prepReco', options)]
    plot_algs = []
    
    #-------------------------------------------------------------------------
    # Configure event plotting algorithm
    #
    plot_algs += [getPlotEventAlgs('plotEventAlgs', options, runNumber, isData)]

    #-------------------------------------------------------------------------
    # Configure top level alg
    #
    top_algs = prep_algs
        
    top_algs += [getRunNexus('runNexus', options, plot_algs)]

    run = physicsTauBase.prepareReadModule(ROOT, options)
    run.AddTopAlg('topAlg', top_algs)
    clog.info('Finished python configuration: process up to %d event(s)' %options.nevent)
    return run

#========================================================================================
def getPlotEventAlgs(name, options, runNumber, isData):

    print_event  = physicsBase.AlgConfig('printEvent', 'PrintEvent')

    plot_event  = physicsBase.AlgConfig('plotEvent', 'PlotEvent')
    plot_event.SetKey('DirName',     'taus')
    plot_event.SetKey('Debug',       options.debug)
    plot_event.SetKey('KeyHist',     'PlotEventHistKey')

    plot_SigSS  = physicsBase.AlgConfig('plotSigSS', 'PlotEvent')
    plot_SigSS.SetKey('DirName', 'SigSS')
    plot_SigSS.SetKey('Debug',       options.debug)
    plot_SigSS.SetKey('KeyHist',     'PlotEventHistKey')

    plot_SigOS  = physicsBase.AlgConfig('plotSigOS', 'PlotEvent')
    plot_SigOS.SetKey('DirName', 'SigOS')
    plot_SigOS.SetKey('Debug',       options.debug)
    plot_SigOS.SetKey('KeyHist',     'PlotEventHistKey')

    plot_BkgOS  = physicsBase.AlgConfig('plotBkgOS', 'PlotEvent')
    plot_BkgOS.SetKey('DirName', 'BkgOS')
    plot_BkgOS.SetKey('Debug',       options.debug)
    plot_BkgOS.SetKey('KeyHist',     'PlotEventHistKey')

    plot_BkgSS  = physicsBase.AlgConfig('plotBkgSS', 'PlotEvent')
    plot_BkgSS.SetKey('DirName', 'BkgSS')
    plot_BkgSS.SetKey('Debug',       options.debug)
    plot_BkgSS.SetKey('KeyHist',     'PlotEventHistKey')

    event_cuts = getEventCuts(runNumber, isData)
    event_cuts_OS = getEventCutsOS(runNumber, isData)
    event_cuts_SS = getEventCutsSS(runNumber, isData)
    object_tau_cuts = getTauCuts()
    object_truth_cuts = getTruthCuts()
    object_muon_cuts = getMuonCuts()
    invert_muon_cuts = getInvertMuonCuts()
    object_elec_cuts = getElecCuts()
    object_jet_cuts = getJetCuts()
    object_truth_tau_cuts = getTruthCuts();
    object_truth_tau_cuts = getTruthTauCuts()
    object_truth_elec_cuts = getTruthElecCuts()
    object_truth_muon_cuts = getTruthMuonCuts()
    object_elec_trig_cuts = getNewElecTriggers()
    object_muon_trig_cuts = getNewMuonTriggers()

    physicsCuts.addCuts(plot_event, 'CutEvent', event_cuts)
    physicsCuts.addCuts(plot_event, 'CutElec', object_elec_cuts)
    physicsCuts.addCuts(plot_event, 'CutMuon', object_muon_cuts)
    physicsCuts.addCuts(plot_event, 'CutTau', object_tau_cuts)

    physicsCuts.addCuts(plot_SigOS, 'CutEvent', event_cuts_OS )
    physicsCuts.addCuts(plot_SigOS, 'CutMuon', object_muon_cuts)
    physicsCuts.addCuts(plot_SigOS, 'CutElec', object_elec_cuts)
    physicsCuts.addCuts(plot_SigOS, 'CutTau', object_tau_cuts)

    physicsCuts.addCuts(plot_SigSS, 'CutEvent', event_cuts_SS )
    physicsCuts.addCuts(plot_SigSS, 'CutMuon', object_muon_cuts)
    physicsCuts.addCuts(plot_SigSS, 'CutElec', object_elec_cuts)
    physicsCuts.addCuts(plot_SigSS, 'CutTau', object_tau_cuts)

    physicsCuts.addCuts(plot_BkgOS, 'CutEvent', event_cuts_OS )
    physicsCuts.addCuts(plot_BkgOS, 'CutMuon', invert_muon_cuts)
    physicsCuts.addCuts(plot_BkgOS, 'CutElec', object_elec_cuts)
    physicsCuts.addCuts(plot_BkgOS, 'CutTau', object_tau_cuts)

    physicsCuts.addCuts(plot_BkgSS, 'CutEvent', event_cuts_SS )
    physicsCuts.addCuts(plot_BkgSS, 'CutMuon', invert_muon_cuts)
    physicsCuts.addCuts(plot_BkgSS, 'CutElec', object_elec_cuts)
    physicsCuts.addCuts(plot_BkgSS, 'CutTau', object_tau_cuts)

    write_ntuple = physicsBase.AlgConfig('WriteNtuple', 'WriteNtuple')

    physicsCuts.addCuts(write_ntuple, 'CutEvent', event_cuts)
    physicsCuts.addCuts(write_ntuple, 'CutElec', object_elec_cuts)

    algs = [plot_SigOS]#, plot_SigSS]#, plot_BkgOS, plot_BkgSS ]
    
    chain = physicsCuts.getRunChain(name, algs=algs)
    chain.SetKey('DirName',     name)
    chain.SetKey('Debug',       options.debug)
    chain.SetKey('Print',       'no')
    chain.SetKey('DoTimer',     options.do_timer)

    return chain

#========================================================================================
def getPrintEventAlg(name, options):


    return print_event

#========================================================================================
def getPrepReco(name, options):

    var_weights = ['MCWeightOrg']
    
    clog.info('getPrepReco - list of weights: %s' %var_weights)

    alg = physicsBase.AlgConfig(name, 'PrepReco')
    alg.SetKey('Debug',         options.debug)
    alg.SetKey('Print',         'yes')
    alg.SetKey('DoLepIdFilter', 'yes')
    alg.SetKey('AddLeptonMass', 'yes')
    alg.SetKey('Weights',       ','.join(var_weights))
    alg.SetKey('CopyName',      'loose/Count')
    alg.SetKey('DoCopy',        'yes')
        
    if options.no_weight:
        alg.SetKey('NoWeight', 'yes')

    return alg

#========================================================================================
def getRunNexus(name, options, algs=None, do_nexus_file=False):

    chain = physicsCuts.getRunChain(name, algs=algs, cuts=None)
    chain.SetKey('DirName',     '')
    chain.SetKey('Debug',       options.debug)
    chain.SetKey('Print',       options.debug)
    chain.SetKey('DoTimer',     options.do_timer)

    alg = physicsBase.AlgConfig(name, 'RunNexus')    
    alg.SetKey('Print',    options.debug)
    alg.SetKey('Debug',    options.debug)
    alg.SetKey('DirName',  '')    
    alg.SetKey('DirName0', 'data')
    alg.SetKey('VarName',  'MCChannel')
    alg.SetKey('AlgName',   chain.GetAlgName())
    
    alg.AddAlg(chain)

    if do_nexus_file and options.output and options.output.count('.root'):
        alg.SetKey('FileKey', '%s-' %options.output.partition('.root')[0])
        alg.SetKey('DoFile',  'yes')

    return alg

#========================================================================================
# Cut configurations
#========================================================================================
def getCutValue(cuts, name, default):

    if type(cuts) != dict:
        return default

    try:
        return cuts[name]
    except KeyError:
        return default    

#========================================================================================
##################################################################################
def getTriggers():
    trigger_list = ['match_HLT_mu26_imedium',
                    'match_HLT_mu50',
                    'match_HLT_e26_lhtight_iloose',
                    'match_HLT_e60_lhmedium',
                    'match_HLT_e140_lhloose']

    return trigger_list
##################################################################################
def getTriggerCuts():
    trigger_cuts = CutItem('CutTrigger')
    for trigger in getTriggers():

        cut = CutItem('Cut%s' %trigger, '%s == 1' %(trigger))
        trigger_cuts.AddCut(cut, 'OR')

    return [trigger_cuts]

##################################################################################
def getEventCuts(runNumber, isData):

    cuts  = []
    cuts += [CutItem('PassEventCleaning', 'PassEventCleaning > 0 || passEventCleaning > 0')]
    cuts += [CutItem('CutOneMuonZeroElecs', 'NMuon == 1')]
    cuts += [CutItem('CutOneTau', 'NTau > 0')]
    cuts += [CutItem('CutOppositeCharge', 'OppositeCharge == -1')]
    cuts += [CutItem('CutTrigMuon', 'NTrigMuon > 0')]

    return cuts
##################################################################################
def getEventCutsOS(runNumber, isData):

    cuts  = []
    cuts += [CutItem('PassEventCleaning', 'PassEventCleaning > 0 || passEventCleaning > 0')]
    cuts += [CutItem('CutOneMuonZeroElecs', 'NMuon == 1')]
    cuts += [CutItem('CutOneTau', 'NTau > 0')]
    cuts += [CutItem('CutOppositeSign', 'OppositeCharge == -1')]
    cuts += [CutItem('CutTrigMuon', 'NTrigMuon > 0')]
    return cuts
##################################################################################
def getEventCutsSS(runNumber, isData):

    cuts  = []
    cuts += [CutItem('PassEventCleaning', 'PassEventCleaning > 0 || passEventCleaning > 0')]
    cuts += [CutItem('CutOneMuonZeroElecs', 'NMuon == 1')]
    cuts += [CutItem('CutOneTau', 'NTau > 0')]
    cuts += [CutItem('CutSameSign', 'OppositeCharge == 1')]
    cuts += [CutItem('CutTrigMuon', 'NTrigMuon > 0')]
    return cuts

#================================================================================                                                                                    

def getNewMuonTriggers():
    
    cuts = []
    cuts += [CutItem('MuonTrig', 'match_HLT_mu20_iloose_L1MU15 > 0 || match_HLT_mu50 > 0 || match_HLT_mu26_ivarmedium > 0 || match_HLT_mu50 > 0' )]
    return cuts

#================================================================================                                                                                         
def getNewElecTriggers():
    
    cuts = []
    cuts += [CutItem('ElecTrig', 'match_HLT_e24_lhmedium_L1EM18VH > 0 || match_HLT_e24_lhmedium_L1EM20VH > 0 || match_HLT_e60_lhmedium > 0 || match_HLT_e120_lhloose > 0 || match_HLT_e26_lhtight_nod0_ivarloose > 0 || match_HLT_e60_lhmedium_nod0 > 0 || match_HLT_e140_lhloose_nod0 > 0' )]
    return cuts


#========================================================================================
def getTruthCuts():

    cuts  = []
    
    return cuts
#========================================================================================
def getElecCuts():

    cuts  = []
    cuts += [CutItem('CutEta',      'Eta < 2.47',                 abs=True)]
    cuts += [CutItem('CutEtaCrack', 'Eta < 1.37 || Eta > 1.52',   abs=True)]
    cuts += [CutItem('CutPt',       'Pt   > 27.0e3')]
    cuts += [CutItem('CutIDStrength', 'isTightLH > 0')]
    cuts += [CutItem('CutIso', 'isolationLoose > 0')]
#    cuts += [CutItem('CutChargeIDBDTTight', 'ChargeIDBDTTight >= .99')]
    
    return cuts
#========================================================================================
def getTruthElecCuts():

    cuts  = []
    cuts += [CutItem('CutPDG',       'PDG == 11', abs=True)]
    
    return cuts


#========================================================================================
def getMuonCuts():

    cuts  = []
    cuts += [CutItem('CutEta',      'Eta  < 2.5', abs=True)]
    cuts += [CutItem('CutPt',       'Pt   > 27.3e3')]
    cuts += [CutItem('CutPtVarCone30', 'PtVarCone30OverPt < 0.01')]
    cuts += [CutItem('CutEtTopoconeCone20', 'EtCone20OverPt < 0.05')]#formerly < 20 
    cuts += [CutItem('CutIDStrength', 'isMedium > 0')]

    return cuts

#========================================================================================
def getInvertMuonCuts():

    cuts  = []
    cuts += [CutItem('CutEta',      'Eta  < 2.5', abs=True)]
    cuts += [CutItem('CutPt',       'Pt   > 27.3e3')]
    cuts += [CutItem('CutPtVarCone30', 'PtVarCone30OverPt >= 0.01')]
    cuts += [CutItem('CutEtTopoconeCone20', 'EtCone20OverPt >= 0.05')]#formerly < 20 
    cuts += [CutItem('CutIDStrength', 'isMedium > 0')]

    return cuts
#========================================================================================
def getTruthMuonCuts():

    cuts  = []
    cuts += [CutItem('CutPDG',       'PDG == 13',abs=True)]
    
    return cuts


#========================================================================================
def getTauCuts():

    cuts  = [] 
    cuts += [CutItem('CutEta', 'Eta < 2.47', abs=True)]    
    cuts += [CutItem('CutEtaCrack', 'Eta < 1.37 || Eta > 1.52', abs=True)]
    cuts += [CutItem('CutPt',  'Pt  > 2.e4')]
    cuts += [CutItem('CutNTrack',  'NTrack == 1 || NTrack == 3')]
    cuts += [CutItem('CutOverlap', 'TauOverlap > 0')]
    cuts += [CutItem('CutJetBDTScore',  'BDTJetScore > 0.3')]
    cuts += [CutItem('CutPassEleOLR',      'passEleOLR > 0')]

 
    return cuts
#========================================================================================
def getTruthTauCuts():

    cuts  = [] 
    cuts += [CutItem('CutPDG', 'PDG == 15', abs=True)]    
    
    return cuts
#========================================================================================
def getJetCuts():

    cuts  = [] 
    cuts += [CutItem('CutEta', 'Eta < 4.5', abs=True)]    
 #   cuts += [CutItem('CutPt',  'Pt  > 2.5e4')]
 
    return cuts

#========================================================================================
def getBJetCuts():

    cuts  = []
    cuts += [CutItem('CutBTagEta', 'Eta < 2.4', abs=True)]
    cuts += [CutItem('CutBTagMV2', 'BTagMV2c20 > -0.0436')]
    
    return cuts

#========================================================================================
def getTrigElecCuts(runNumber, isData):

    cuts = []
#    cuts += [CutItem('CutTrigPt',   'Pt > 27.0e3')]
#    if isData == True:
 #       if runNumber ==  298687:
  #          cuts += [CutItem('Trigger', 'match_HLT_e24_lhtight_nod0_ivarloose == 1 || match_HLT_e24_lhmedium_nod0_L1EM20VH == 1 || match_HLT_e60_lhmedium_nod0 == 1 || match_HLT_e60_medium == 1 || match_HLT_e140_lhloose_nod0 == 1 || match_HLT_e300_etcut == 1')]
   #     elif runNumber >=  296939 and runNumber <= 300287: # data16 period A Trigger
    #        cuts += [CutItem('Trigger', 'match_HLT_e24_lhtight_nod0_ivarloose == 1 || match_HLT_e60_lhmedium_nod0 == 1 || match_HLT_e60_medium == 1 || match_HLT_e140_lhloose_nod0 == 1 || match_HLT_e300_etcut == 1')]
     #   elif runNumber <= 300918: # data16 period B to D3 trigger
    #        cuts += [CutItem('Trigger', 'match_HLT_e24_lhtight_nod0_ivarloose == 1 || match_HLT_e60_lhmedium_nod0 == 1 || match_HLT_e60_medium == 1 || match_HLT_e140_lhloose_nod0 == 1 || match_HLT_e300_etcut == 1')]
     #   elif runNumber <= 303892: # data16 period D4 to E trigger
       #     cuts += [CutItem('Trigger', 'match_HLT_e24_lhtight_nod0_ivarloose == 1 || match_HLT_e60_lhmedium_nod0 == 1 || match_HLT_e60_medium == 1 || match_HLT_e140_lhloose_nod0 == 1 || match_HLT_e300_etcut == 1')]
    #    elif runNumber <= 311481: # data16 rest trigger
     #       cuts += [CutItem('Trigger', 'match_HLT_e24_lhtight_nod0_ivarloose == 1 || match_HLT_e60_lhmedium_nod0 == 1 || match_HLT_e60_medium == 1 || match_HLT_e140_lhloose_nod0 == 1 || match_HLT_e300_etcut == 1')]
     #   else: # data15 trigger
     #       cuts += [CutItem('Trigger', 'match_HLT_e24_lhmedium_L1EM20VH == 1 || match_HLT_e60_lhmedium == 1 || match_HLT_e120_lhloose == 1')]
   # else: # mc trigger
#        cuts += [CutItem('Trigger', 'match_HLT_e24_lhmedium_L1EM20VH == 1 || match_HLT_e60_lhmedium == 1 || match_HLT_e120_lhloose == 1')]

    return cuts
#========================================================================================
def getTrigMuonCuts(runNumber, isData):
    
    cuts = []
#    cuts += [CutItem('CutTrigPt',   'Pt > 27.0e3')]
 #   if isData == True:
  #      if runNumber ==  298687:
   #         cuts += [CutItem('Trigger', 'match_HLT_mu24_iloose == 1 || match_HLT_mu24_ivarloose == 1 || match_HLT_mu40 == 1 || match_HLT_mu50 == 1')]
    #    elif runNumber >=  296939 and runNumber <= 300287: # data16 period A Trigger
   #         cuts += [CutItem('Trigger', 'match_HLT_mu24_iloose == 1 || match_HLT_mu24_ivarloose == 1 || match_HLT_mu40 == 1 || match_HLT_mu50 == 1')]
    #    elif runNumber <= 300918: # data16 period B to D3 trigger
   #         cuts += [CutItem('Trigger', 'match_HLT_mu24_imedium == 1 || match_HLT_mu24_ivarmedium == 1 || match_HLT_mu50 == 1')]
    #    elif runNumber <= 303892: # data16 period D4 to E trigger
     #       cuts += [CutItem('Trigger', 'match_HLT_mu24_imedium == 1 || match_HLT_mu24_ivarmedium == 1 || match_HLT_mu26_imedium == 1 || match_HLT_mu26_ivarmedium == 1 || match_HLT_mu50 == 1')]
      #  elif runNumber <= 311481: # data16 rest trigger
       #     cuts += [CutItem('Trigger', 'match_HLT_mu24_imedium == 1 || match_HLT_mu24_ivarmedium == 1 || match_HLT_mu26_imedium == 1 || match_HLT_mu26_ivarmedium == 1 || match_HLT_mu50 == 1')]
        #else: # data15 trigger
       #     cuts += [CutItem('Trigger', 'match_HLT_mu20_iloose_L1MU15 == 1 || match_HLT_mu40 == 1 || match_HLT_mu60_0eta105_msonly == 1')]
    #else: # mc trigger
    #    cuts += [CutItem('Trigger', 'match_HLT_mu20_iloose_L1MU15 == 1 || match_HLT_mu40 == 1 || match_HLT_mu60_0eta105_msonly == 1')]
  

    return cuts
#========================================================================================
