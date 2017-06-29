import os
import sys
import re

import PhysicsBase.PhysicsBaseConfig as physicsBase
import PhysicsTaus.PhysicsTausConfig as physicsConfig

clog = physicsConfig.getLog(os.path.basename(__file__))

#========================================================================================
# Configure main C++ module that reads ntuples and runs algorithms
#========================================================================================
def prepareReadModule(ROOT, options, opt='', var_list=[]):
       
    ''' These list below define mapping between ntuple branches and varibles in C++ code.
        This is only necessery for in cases when branch name does not match variables
        that are defined in CutVar.h and CutVar.cxx
        
        Event variables:
            -- EventNumber -> RecoEvent::AddVar(Cut::Event,       value)
            -- mcWeightOrg -> RecoEvent::AddVar(Cut::MCWeightOrg, value)

        Electron variables:
            -- electron_pt -> RecElec::AddVar(Cut::Pt, value)

        Jet variables:
            -- jet_flavor_weight_MV2c20 -> RecJet::AddVar(Cut:BTagMV2c20, value)
    '''

    var_nicks = ['pt:Pt',
                 'eta:Eta',
                 'phi:Phi',
                 'E:Energy',
                 'e:Energy',
                 'm:Mass',
                 'EventNumber:Event',
                 'RunNumber:Run',
                 'averageIntPerXing:EventMu',
                 'pileupEventWeight_090:PUWeight',
                 'mcWeightOrg:MCWeightOrg',
                 'JVT_EventWeight:JVTWeight',
                 'mc_channel_number:MCChannel',
                 'isBlinded:IsBlinded',
                 'status:Status',
                 'pdgId:PDG',
                 'passEventCleaning:PassEventCleaning',
                 'm_vxpu_n:NRecoPileUp',
                 'children:IndexRangeChild',
                 'parents:IndexRangeParent',
                 'barcode:TrueBarcode',
                 'truthType:TruthType',
                 'truthOrigin:TruthOrigin',    
    ]

    vars_lep = ['author:Author',
                'allAuthor:Authors',
                'ID:PDG',
                'z0SinTheta:Z0Sin',
                'sigd0PV:D0Sig',
                'topoetcone20:EtTopoCone20',
                'topoetcone30:EtTopoCone30',
                'topoetcone40:EtTopoCone40',
                'ptvarcone20:PtVarCone20',
                'ptvarcone30:PtVarCone30',
                'ptvarcone40:PtVarCone40',
                ]

    vars_jet = ['flavor_weight_MV1:BTagMV1',
                'flavor_weight_MV2c00:BTagMV2c00',
                'flavor_weight_MV2c10:BTagMV2c10',
                'flavor_weight_MV2c20:BTagMV2c20',
                'emfrac:EMFrac',
                'sumPtTrk:SumPt',
                'jvtxf:JVTXF',
                'jvt:JVT',
                'etaEM:EtaEM',
                'numTrk:NTrack',
                'flavor_truth_label:JetFlavorTruth',
                'flavor_truth_label_ghost:JetFlavorTruthGhost',
               ]

    vars_tau = ['numTrack:NTrack',
                'numWideTrack:NWideTrack',
                'charge:Charge',
                ]
    
    var_vetos = ['Mll01', 
                 'Mll02',
                 'Mll03',
                 'Mll12',
                 'Mll23',
                 'Ptll01', 
                 'Ptll02',
                 'Ptll03',
                 'Ptll12',
                 'Ptll23',
                 'HT',
                 'numPrecLayers',
                 'muon_numPrecLayers'
             ]

    run = prepareReadNtuple(ROOT, options, hist_config=['config/'])

    run.SetKey('TreeName',          'nominal')
    run.SetKey('JetVecPrefix',      'm_jet_')
    run.SetKey('ElecVecPrefix',     'electron_')
    run.SetKey('MuonVecPrefix',     'muon_')
    run.SetKey('TauRVtxVecPrefix',  'm_tau_')
    run.SetKey('TruthVecPrefix',    'm_truth_')
    run.SetKey('Lists',             'm_jet_, electron_, muon_, m_tau_, m_truth_')

    run.SetKey('VarNicks',    ','.join(var_nicks + vars_lep + vars_jet + vars_tau))
    run.SetKey('Vetos',       ','.join(var_vetos))
    
    run.SetKey('Print',          'yes')
    run.SetKey('NPrint',         10000)
    run.SetKey('Debug',          options.debug_run or options.debug)
    run.SetKey('FillTrueEvent',  'no')
    run.SetKey('FillTrueParts',  'yes')

    return run

#========================================================================================
def prepareReadNtuple(ROOT, options, tree='event', hist_config=None, load_libs=True):

    if load_libs:
        physicsConfig.loadPhysicsTausLib(ROOT, options)
    
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

#========================================================================================
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
        self._log   = physicsConfig.getLog('ReadNtuple')
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
