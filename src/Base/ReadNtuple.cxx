// C/C++
#include <algorithm>
#include <iostream>
#include <sstream>

// ROOT
#include "TLeaf.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TSystem.h"

// External
#ifdef ANP_CPU_PROFILER
#include "gperftools/profiler.h"
#endif

// Base
#include "PhysicsBase/Factory.h"
#include "PhysicsBase/HistMan.h"
#include "PhysicsBase/ReadFile.h"

// Local
#include "PhysicsTaus/ReadNtuple.h"

using namespace std;

//==============================================================================
// ReadNtuple
//==============================================================================
unsigned Anp::ReadNtuple::List::GetVarSize() const
{
  bool     init = false;
  unsigned size = 0;

  for(const VecData &data: vars) {
    if(!init) {
      init = true;
      size = data.GetSize();
    }
    else if(size != data.GetSize()) {
      //      cout << "ReadNtuple::List::GetVarSize - size mismatch for " << prefix 
      //	   << ": " << size << "!=" << data.GetSize() << " " << data.branch << endl;
    }
  }
  
  return size;
}

//==============================================================================
void Anp::ReadNtuple::List::Print() const
{
  cout << "List - prefix: " << prefix << " number of vars: " << vars.size() << endl;

  for(unsigned i = 0; i < vars.size(); ++i) {
    vars.at(i).Print();
  }
}

//==============================================================================
Anp::ReadNtuple::ReadNtuple():
  fFile       (0),
  fDebug      (false),
  fDebugVars  (false),
  fCloseFile  (true),
  fPrintFiles (false),
  fNEvent     (0),
  fNPrint     (100),
  fCompression(0),
  fICount     (0)
{
}

//==============================================================================
Anp::ReadNtuple::~ReadNtuple()
{
}


//==============================================================================
void Anp::ReadNtuple::Execute(const string &config_path)
{
  //
  // Main function call: read configuration and execute C++ code
  //
  Registry reg;
  reg.ReadXML(config_path);

  ExecuteRegistry(reg);
}

//==============================================================================
void Anp::ReadNtuple::ExecuteRegistry(const Registry &reg)
{
  //
  // Main function call: read configuration and execute C++ code
  //
  cout << "*****************************************************************" << endl;
  cout << "Executing C++ code..." << endl;

  //
  // First, configure self
  //
  ReadNtuple::Config(reg);   
  ReadNtuple::Init();

  if(fInputFiles.empty()) {
    log() << "Execute - missing input files... skip event loop" << endl;
  }
  else {
    ReadNtuple::Exec();
  }

  ReadNtuple::Done();

  log() << "Execute - all is done." << endl;
  cout << "*****************************************************************" << endl;
}

//==============================================================================
void Anp::ReadNtuple::Config(const Anp::Registry &reg)
{
  //
  // Global job connfiguration
  //
  fReg = reg;

  reg.Get("ReadNtuple::Debug",             fDebug             = false);
  reg.Get("ReadNtuple::DebugVars",         fDebugVars         = false);
  reg.Get("ReadNtuple::CloseFile",         fCloseFile         = true);
  reg.Get("ReadNtuple::PrintFiles",        fPrintFiles        = false);
  reg.Get("ReadNtuple::FillTrueParts",     fFillTrueParts     = false);
  reg.Get("ReadNtuple::NEvent",            fNEvent            = 0);
  reg.Get("ReadNtuple::NEventPerFile",     fNEventPerFile     = 0);
  reg.Get("ReadNtuple::NPrint",            fNPrint            = 100);
  reg.Get("ReadNtuple::Compression",       fCompression       = 0);
  reg.Get("ReadNtuple::EventFracMin",      fEventFracMin      = 0.0);
  reg.Get("ReadNtuple::EventFracMax",      fEventFracMax      = 0.0);

  reg.Get("ReadNtuple::BranchJets",        fBranchJets        = "rantiKt6emjets");
  reg.Get("ReadNtuple::JetVecPrefix",      fJetVecPrefix      = "m_jet4_");
  reg.Get("ReadNtuple::TrackJetVecPrefix", fTrackJetVecPrefix = "");
  reg.Get("ReadNtuple::ElecVecPrefix",     fElecVecPrefix     = "m_elec_");
  reg.Get("ReadNtuple::MuonVecPrefix",     fMuonVecPrefix     = "m_muon_");
  reg.Get("ReadNtuple::TauRVecPrefix",     fTauRVecPrefix     = "m_tau_");
  reg.Get("ReadNtuple::TauVVecPrefix",     fTauVVecPrefix     = "");
  reg.Get("ReadNtuple::TrackVecPrefix",    fTrackVecPrefix    = "m_track_");
  reg.Get("ReadNtuple::HitVecPrefix",      fHitVecPrefix      = "");
  reg.Get("ReadNtuple::TruthVecPrefix",    fTruthVecPrefix    = "m_truth_");
  reg.Get("ReadNtuple::TruthVtxVecPrefix", fTruthVtxVecPrefix = "");
  reg.Get("ReadNtuple::RecoVtxVecPrefix",  fRecoVtxVecPrefix  = "");
  reg.Get("ReadNtuple::TreeName",          fTreeNames);

  //
  // Read self name and type
  // 
  string self_name;
  string self_type;
  
  if(reg.Get(GetType(), "AlgType", self_type) && GetType().empty()) { 
    AlgEvent::SetType(self_type);
  }
  if(reg.Get(GetType(), "AlgName", self_name) && GetName().empty()) {
    AlgEvent::SetName(self_name);
  }

  //
  // Create sub-algorithm
  //
  string alg_name, alg_type;

  reg.Get("ReadNtuple::SubAlgType", alg_type);
  reg.Get("ReadNtuple::SubAlgName", alg_name);
  
  fAlg = Factory<AlgEvent>::Instance().Create(alg_type);

  if(fAlg.valid()) {
    fAlg->SetName(alg_name);
    fAlg->SetType(alg_type);

    log() << "Config - created algorithm: \"" 
	  << alg_name << "\" - \"" << alg_type << "\"" << endl;
  }
  else {
    log() << "Config - failed to create algorithm: \"" << alg_type << "\"" << endl;
  }
  
  //
  // Configure HistMan
  //
  HistMan::Instance().Config(reg);

  //
  // Read input files 
  //
  Registry ireg;
  
  if(reg.Get("ReadNtuple::InputFiles", ireg)) {
    if(fDebug) {
      log() << "Config - reading Registry with input files" << endl;
    }

    while(ireg.KeyExists("File")) {
      //
      // Add individual files
      //
      string data;
      ireg.Get("File", data);
      ireg.RemoveKey("File");

      if(std::find(fInputFiles.begin(), fInputFiles.end(), data) != fInputFiles.end()) {
	log() << "Config - ignore duplicate file: " << data << endl;
	continue;
      }
      
      fInputFiles.push_back(data);
    }
  }
  else {
    log() << "Config - input files are not available at config step" << endl;
  }

  log() << "Config" << endl
	<< "   NEvent:            " << fNEvent            << endl
	<< "   NEventPerFile:     " << fNEventPerFile     << endl
	<< "   Debug:             " << fDebug             << endl
	<< "   CloseFile:         " << fCloseFile         << endl
	<< "   FillTrueParts:     " << fFillTrueParts     << endl
	<< "   ElecVecPrefix:     " << fElecVecPrefix     << endl
	<< "   MuonVecPrefix:     " << fMuonVecPrefix     << endl
	<< "   TauRVecPrefix:     " << fTauRVecPrefix     << endl
	<< "   TauVVecPrefix:     " << fTauVVecPrefix     << endl
	<< "   JetVecPrefix:      " << fJetVecPrefix      << endl
        << "   TrackJetVecPrefix: " << fTrackJetVecPrefix << endl
	<< "   TrackVecPrefix:    " << fTrackVecPrefix    << endl
	<< "   HitVecPrefix:      " << fHitVecPrefix      << endl
	<< "   TruthVecPrefix:    " << fTruthVecPrefix    << endl
	<< "   TruthVtxVecPrefix: " << fTruthVtxVecPrefix << endl;


  //
  // Read branch prefixes
  //
  vector<string> lists;
  reg.Get("ReadNtuple::Lists", lists);

  log() << "Config - process : " << lists.size() << " vector(object) branches " << endl;

  for(const string &l: lists) {
    List ld;
    ld.prefix = l;
    ld.bkeys  = GetKeyBranchSet(ld.prefix);

    fLists.push_back(ld);

    cout << "   branch with prefix: " << ld.prefix << " branch set size=" << ld.bkeys.size() << endl;
  }
  
  //
  // Read veto variables
  //
  if(reg.KeyExists("ReadNtuple::Vetos")) {
    fVetoVars = Cut::ReadVars(reg, "ReadNtuple::Vetos",    GetName());    
    fVetoVecs = Cut::ReadVars(reg, "ReadNtuple::VetoVecs", GetName());
  }
  else {
    fVetoVars = Cut::ReadVars(reg, "ReadNtuple::VetoVars", GetName());
    fVetoVecs = Cut::ReadVars(reg, "ReadNtuple::VetoVecs", GetName());    
  }
  
  //
  // Read variable nicknames
  //
  ReadNick(reg);

  //
  // Book output branches
  //
  fReco = NtupleSvc::Instance("reco").Branch<vector<RecoEvent> >   ("events");
  fJets = NtupleSvc::Instance("reco").Branch<vector<Ptr<RecJet> > >(fBranchJets);
}

//==============================================================================
bool Anp::ReadNtuple::Init()
{
  //
  // Initialiaze self and algorithms
  //
  if(fAlg.valid()) {
    if(fDebug) {
      log() << "Init - configuring algorithm: " << fAlg->GetName() << endl;
    }
    
    Registry areg;
    if(fReg.Get(fAlg->GetName(), areg)) {
      fAlg -> Config(areg);
      log() << "Init - retrieved Registry for top algorithm: " << fAlg->GetName() << endl;
    }
    else {
      log() << "Init - using global Registry for top algorithm: " << fAlg->GetName() << endl;
      fAlg -> Config(fReg);
    }

    if(fDebug) {
      log() << "Init - finished configuring algorithm: " << fAlg->GetName() << endl;
    }
  }

  //
  // Create output ROOT file
  //
  string outfname;

  if(fReg.Get("ReadNtuple::OutputFile", outfname) && outfname.size() > 4) {

    if(fCompression > 0) {
      fFile = TFile::Open(outfname.c_str(), "RECREATE", outfname.c_str(), fCompression);
    }
    else {
      fFile = TFile::Open(outfname.c_str(), "RECREATE");
    }

    log() << "Init - opened output ROOT file: " << outfname << endl;

    if(fAlg.valid()) {
      fAlg -> Save(fFile);
    }
  }
  else {
    if(fDebug) {
      log() << "Init - output file is not set" << endl;
    }
  }

  //
  // Initialize algorithms
  //
  if(fAlg.valid()) {
    if(fDebug) {
      log() << "Init - initializing algorithm: " << fAlg->GetName() << endl;
    }

    fAlg -> Init();

    if(fDebug) {
      log() << "Init - finished initializing algorithm: " << fAlg->GetName() << endl;
    }
  }

  return true;
}

//==============================================================================
void Anp::ReadNtuple::Exec()
{
  //
  // Process input files 
  //

#ifdef ANP_CPU_PROFILER
  string profile_file = "cpu-profile-runmodule-exec";
  fReg.Get("ReadNtuple::CpuProfile", profile_file);

  log() << "Exec - compiled with perftools:" << endl
	<< "   ProfilerStart(" << profile_file << ")" << endl;

  ProfilerStart(profile_file.c_str());
#endif

  fTimerEvent.Reset();
  fTimerEvent.Start();

  if(fAlg.valid()) {
    fAlg -> Signal(fReg, "BEG_EXEC");
  }

  //
  // Iterate over input files and read events
  //
  log() << "Exec - loop over " << fInputFiles.size() << " input file(s)" << endl;
  cout << "-----------------------------------------------------------------" << endl;  

  if(fEventFracMin < fEventFracMax) {
    fNEvent = CountNEvent(fInputFiles) + 1;
  }

  for(const string &file: fInputFiles) {
    //
    // Check event count
    //
    if(StopNow(fICount)) { 
      break;
    }

    ReadFile(file, fICount);
  }
  
  cout << "-----------------------------------------------------------------" << endl;
  log() << "Exec - finished reading input files: event count = " << fICount << endl;

#ifdef ANP_CPU_PROFILER
  log() << "Exec - compiled with perftools: ProfilerStop." << endl;
  ProfilerStop();
#endif
}

//==============================================================================
void Anp::ReadNtuple::Done()
{
  //
  // End of processing...
  //
  log() << "Done - finalize algorithms" << endl;

#ifdef ANP_CPU_PROFILER
  string profile_file = "cpu-profile-runmodule-done";
  fReg.Get("ReadNtuple::CpuProfileDone", profile_file);

  log() << "Done - compiled with perftools:" << endl
	<< "   ProfilerStart(" << profile_file << ")" << endl;

  ProfilerStart(profile_file.c_str());
#endif

  if(fDebug) {
    log() << "Done - calling Done() for sub-algorithm..." << endl;
  }

  if(fAlg.valid()) {
    fAlg -> Done();
  }

  //
  // Erase algorithms (delete them if nobody else holds handle)
  //
  log() << "Done - release algorithm handle(s)..." << endl;
  if(fAlg.valid()) {
    fAlg.release();
  }

  //
  // Erase factory hold list
  //
  /*
  std::vector<Anp::FactoryBase *> &flist = Anp::GetFactoryList();

  log() << "Done - clearing " << flist.size() << " factory object(s)" << endl;

  for(unsigned int i = 0; i < flist.size(); ++i) {
    Anp::FactoryBase *f = flist[i];
    f->Clear();
  }
  */

  //
  // Close output file (if exists)
  //
  if(fFile) {
    const string filename = fFile->GetName();

    log() << "Done - writing ROOT file: " << filename << endl;
    fFile->Write();
    log() << "Done - writing ROOT file: " << filename << " - done!"<< endl;

    if(fCloseFile) {
      log() << "Done - closing ROOT file: " << filename << endl;
      gROOT->GetListOfFiles()->Remove(fFile);
      fFile->Close();
      fFile = 0;      
      log() << "Done - closing ROOT file: " << filename << " - done!" << endl;
    }
  }

#ifdef ANP_CPU_PROFILER
  log() << "Done - compiled with perftools: ProfilerStop." << endl;
  ProfilerStop();
#endif
  
  //
  // Print out job summary
  //
  log() << "Done - processed "<< fICount << " event(s)" << endl; 
  log() << "Done - " << Anp::PrintResetStopWatch(fTimerTotal) << endl;
  log() << "Done - finished." << endl;
}

//==============================================================================
void Anp::ReadNtuple::ReadFile(const std::string &fpath, long &icount)
{
  //
  // Open TFile, get TTree, prepare algorithm and read events
  //
  if(fTreeNames.empty()) {
    log() << "ReadFile - no tree names specified - do nothing" << endl;
    return;
  }

  const pair<string, string> path_name = Anp::SplitPath(fpath);
  
  if(fDebug || fPrintFiles) {
    log() << "ReadFile" << endl
	  << "  path=" << path_name.first  << endl
	  << "  file=" << path_name.second << endl;
  }

  Registry reg;
  reg.Set("SIGNAL::FilePath", fpath);

  if(fAlg.valid()) {
    fAlg -> Signal(reg, "NEW_FILE");
  }

  TFile *file = TFile::Open(fpath.c_str(), "READ");
  if(!file || !file->IsOpen()) {
    log() << "ReadFile - failed to open:\n" << "   " << fpath << endl;

    if(file) {
      delete file;
    }
    return;
  }

  for(const string &tname: fTreeNames) {
    
    TTree *tree = dynamic_cast<TTree *> (file->Get(tname.c_str()));
    if(!tree) {
      log() << "ReadFile - TTree is not found: " << tname << std::endl;
      continue;
    }

    //
    // Check tree size
    //
    const long tsize = tree->GetEntries();

    //
    // Check event count
    //
    if(StopNow(icount)) break;
    
    if(fDebug) {
      log() << "ReadFile - reading " << tsize 
	    << " events from tree=\"" << tname << "\" and file=\"" << fpath << "\"" << endl;
    }

    InitTree(tree);

    for(long i = 0; i < tsize; ++i) {      
      if(fDebug) {
	log() << "ReadFile - " << tname << " entry #" << i << endl;
      }

      //
      // Count events: global counter and dataset counts
      //  
      if(StopNow(icount)) break;

      if(fNEventPerFile > 0 && i >= fNEventPerFile) break;

      ++icount;

      if(SkipEvent(icount)) continue;

      if((icount % fNPrint == 0) || fDebug) {
	cout << "Record #" << setw(8) << setfill(' ') << icount << " "
	     << Anp::PrintResetStopWatch(fTimerEvent) << endl;
      }

      //
      // Read data
      //
      tree -> GetEntry(i);
      
      //
      // Fill event information
      //
      fReco.GetRef().push_back(RecoEvent());

      if(FillReco(fReco.GetRef().back())) {
	fReco.SetEvent(Ntuple::kPASS);
	fReco.SetState(Ntuple::kREAD);      

	fJets.SetEvent(Ntuple::kPASS);
	fJets.SetState(Ntuple::kREAD);
      }
      else {
	fReco.GetRef().clear();
	fJets.GetRef().clear();

	log() << "ReadFile - event " << i << " contains no known variable(s)" << endl;
      }

      //
      // Print debugging information for input tree branch values
      // 
      if(fDebugVars) {
	cout << "---------------------------------------------------------" << endl;
	
	for(const VarData &data: fVars) {
	  cout << "   " << setw(20) << left << data.key << ": val=" << data.GetVal() << endl;
	}
	
	for(List &l: fLists) {	  
	  for(unsigned index = 0; index < l.GetVarSize(); ++index) {
	    for(VecData &data: l.vars) {
	      cout << "   " << setw(25) << data.branch << ": " << data.GetVal(index) << endl;
	    }
	  }
	}
      }

      //
      // Execute algorithm
      //            
      if(fAlg.valid()) fAlg->Exec();

      //
      // Reset branch status
      //
      fReco.SetEvent(Ntuple::kFAIL);
      fReco.SetState(Ntuple::kNONE);
      fReco.GetRef().clear(); 

      fJets.SetEvent(Ntuple::kFAIL);
      fJets.SetState(Ntuple::kNONE);
      fJets.GetRef().clear(); 
    }
  }
  
  delete file;
}

//==============================================================================
void Anp::ReadNtuple::InitTree(TTree *tree)
{
  //
  // Clear previous data
  //
  tree->SetBranchStatus("*", 0);

  for(List &l: fLists) {
    if(fDebug) {
      log() << "InitTree - clear List item with prefix: " << l.prefix << endl;
    }

    for(VecData &data: l.vars) {
      data.Delete();
    }

    l.vars.clear();
  }

  fVars.clear();
  
  //
  // Create a list of branches
  //
  TObjArray *branches = tree->GetListOfBranches();
  
  for(int i = 0; i <= branches->GetLast(); ++i) {
    TBranch *branch = dynamic_cast<TBranch *>(branches->At(i));
    
    if(!branch) {
      continue;
    }

    if(fDebug) {
      log() << "InitTree - branch: " << branch->GetName() << " - start processing..." << endl;    
      
      TObjArray *leaves = branch->GetListOfLeaves();
      
      for(int j = 0; j <= leaves->GetLast(); ++j) {
	TLeaf *leaf = dynamic_cast<TLeaf *>(leaves->At(j));
	if(leaf) {
	  cout << "   leaf type: " << leaf->GetTypeName() << endl;
	}
      }
    }    

    VarData var_data;
    var_data.branch = branch->GetName();
    var_data.key    = var_data.branch;
    var_data.var    = FindVar(var_data.key);
    
    //
    // Check veto list
    //
    const bool veto_var = std::find(fVetoVars.begin(), fVetoVars.end(), var_data.var) != fVetoVars.end();

    if(var_data.var != Cut::NONE && !veto_var) {
      fVars.push_back(var_data);
      continue;
    }   

    for(List &l: fLists) {
      VecData data;
      
      data.debug  = fDebugVars;
      data.branch = branch->GetName();
      
      if(data.branch.size() < l.prefix.size()) {
	if(fDebug) {
	  log() << "InitTree - branch \"" << data.branch << "\" is shorter than List prefix: " << l.prefix << endl;
	}
	
	continue;
      }
      
      const string spref = data.branch.substr(0, l.prefix.size());
      
      if(spref != l.prefix) {
	if(fDebug) {
	  log() << "InitTree - branch \"" << data.branch << "\" does not match List prefix: " << l.prefix << endl;
	}
	
	continue;
      }
      
      if(fDebug) {
	log() << "InitTree - branch \"" << data.branch << "\" matches List prefix: " << l.prefix << endl;
      }

      data.key = data.branch.substr(l.prefix.size(), data.branch.size());
      data.var = FindVar(data.key);
      
      const bool veto_vec = std::find(fVetoVecs.begin(), fVetoVecs.end(), data.var) != fVetoVecs.end();
	
      if(data.var != Cut::NONE && veto_vec) {
	continue;
      }
      
      if(data.var != Cut::NONE) {
	if(fDebug) {
	  log() << "InitTree - branch \"" << data.branch << "\" matches var: " << data.key << endl;
	}
	
	l.vars.push_back(data);
      }
      else {
	if(fDebug) {
	  log() << "InitTree - branch \"" << data.branch << "\" does not match var: " << data.key << endl;
	}	  
      }
    }
  }

  //
  // Book branches
  //
  for(VarData &data: fVars) {
    if(data.SetVarBranch(tree)) {
      if(fDebug) {
	log() << "InitTree - added var branch: " << data.key << endl;
      }
    }
    else {
      log() << "InitTree - failed to set branch: " << data.key << endl;
    }
  }

  for(List &l: fLists) {
    for(VecData &data: l.vars) {
      if(data.SetVarBranch(tree)) {
	if(fDebug) {
	  log() << "InitTree - added vec branch: " << l.prefix << data.key << endl;
	}
      }
      else {
	log() << "InitTree - failed to set branch: " << l.prefix << data.key << endl;
      }
    }    
  }
}

//==============================================================================
long Anp::ReadNtuple::CountNEvent(const std::vector<std::string> &fpaths)
{
  //
  // Open TFile, get TTree, prepare algorithm and read events
  //
  if(fTreeNames.empty()) {
    log() << "CountNEvent - no tree names specified - do nothing" << endl;
    return 0;
  }

  long nevent = 0;

  for(const string &fpath: fpaths) {

    const pair<string, string> path_name = Anp::SplitPath(fpath);
    
    TFile *file = TFile::Open(fpath.c_str(), "READ");
    if(!file || !file->IsOpen()) {
      log() << "CountNEvent - failed to open:\n" << "   " << fpath << endl;
      
      if(file) {
	delete file;
      }

      continue;
    }

    for(const string &tname: fTreeNames) {
    
      TTree *tree = dynamic_cast<TTree *> (file->Get(tname.c_str()));
      if(!tree) {
	log() << "ReadFile - TTree is not found: " << tname << std::endl;
	continue;
      }
      
      nevent += tree->GetEntries();
    }
  }
  
  return nevent;
}

//==============================================================================
template<class T> void Anp::ReadNtuple::FillMom(const VecVec &vecs, unsigned index, T &obj)
{
  //
  // Set object momentum
  //
  pair<bool, double>    p_en (false, 0.0);
  pair<bool, double>    p_m  (false, 0.0);
  pair<bool, double>    p_pt (false, 0.0);
  pair<bool, double>    p_eta(false, 0.0);
  pair<bool, double>    p_phi(false, 0.0);
  pair<bool, uint64_t>  p_bc (false, 0);

  for(const VecData &data: vecs) {
    if     (data.var == Cut::Energy     ) { p_en .first = true; p_en .second = data.GetFullValue<Float_t >(index); }
    else if(data.var == Cut::Mass       ) { p_m  .first = true; p_m  .second = data.GetFullValue<Float_t >(index); }
    else if(data.var == Cut::Pt         ) { p_pt .first = true; p_pt .second = data.GetFullValue<Float_t >(index); }
    else if(data.var == Cut::Eta        ) { p_eta.first = true; p_eta.second = data.GetFullValue<Float_t >(index); }
    else if(data.var == Cut::Phi        ) { p_phi.first = true; p_phi.second = data.GetFullValue<Float_t >(index); }
    else if(data.var == Cut::RecoBarcode) { p_bc .first = true; p_bc .second = data.GetFullValue<uint64_t>(index); }
  }
    
  
  const bool val3 = p_pt.first && p_eta.first && p_phi.first;

  if(fDebugVars) {
    log() << "FillMom - attempt to compute: SetPtEtaPhi: " 
	  << p_pt.second << "," << p_eta.second << "," << p_phi.second << endl;
  }
  
  if(p_pt.first && p_pt.second > 0.0) {
    if     (p_en.first && val3) { obj.GetFourMom().SetPtEtaPhiE(p_pt.second, p_eta.second, p_phi.second, p_en.second); }
    else if(p_m .first && val3) { obj.GetFourMom().SetPtEtaPhiM(p_pt.second, p_eta.second, p_phi.second, p_m .second); }
    else if(val3) {
      
      static TVector3 v;
      v.SetPtEtaPhi(p_pt.second, p_eta.second, p_phi.second);
      
      obj.GetFourMom().SetPx(v.Px());
      obj.GetFourMom().SetPy(v.Py());
      obj.GetFourMom().SetPz(v.Pz());
    }
  }

  if(p_bc.first) {
    obj.SetBarcode(p_bc.second);
  }
  else {
    obj.SetBarcode(Anp::MakeBarcode(obj.GetObjectType()));
  }
}

//==============================================================================
template<class T> unsigned Anp::ReadNtuple::FillMomVec(const List &l, std::vector<Ptr<T> > &vec)
{
  //
  // Fill vector of objects from branches
  //
  if(fDebugVars) {      
    log() << "FillMomVec - process: " << l.prefix << " #var: " << l.GetVarSize() << " type: " << T::GetObjectType() << endl;
  }

  const unsigned size = vec.size();

  for(unsigned index = 0; index < l.GetVarSize(); ++index) {
    Ptr<T> ptr = ObjectFactory<T>::Instance().CreateObject();    
    
    if(fDebugVars) {
      log() << "FillMomVec<" << ptr->GetObjectType() << "> - index=" << index << endl;
    }
    
    //
    // Set momentum
    //
    FillMom<T>(l.vars, index, ptr.ref());

    //
    // Fill vars
    //
    for(const VecData &data: l.vars) {
      if(fDebugVars) {
	log() << "FillMomVec<" << ptr->GetObjectType() << "> - var=" << data.var << " branch=" << data.branch << endl;
      }      

      if(data.var != Cut::RecoBarcode) { 
	data.AddVar(ptr.ref(), index);	
      }
    }
    
    vec.push_back(ptr);
  }
  
  return vec.size()-size;
}

//==============================================================================
template<class T> unsigned Anp::ReadNtuple::FillObjVec(const List &l, std::vector<Ptr<T> > &vec)
{
  //
  // Fill vector of objects from branches
  //
  if(fDebugVars) {
    log() << "FillObjVec - process: " << l.prefix << " #var: " << l.GetVarSize() << " type: " << T::GetObjectType() << endl;
  }

  const unsigned size = vec.size();

  for(unsigned index = 0; index < l.GetVarSize(); ++index) {
    Ptr<T> ptr = ObjectFactory<T>::Instance().CreateObject();    
    
    if(fDebugVars) {
      log() << "FillObjVec<" << ptr->GetObjectType() << "> - index=" << index << endl;
    }
    
    //
    // Fill vars
    //
    for(const VecData &data: l.vars) {
      if(fDebugVars) {
	log() << "FillObjVec<" << ptr->GetObjectType() << "> - var=" << data.var << " branch=" << data.branch << endl;
      }      

      if(data.var != Cut::RecoBarcode) { 
	data.AddVar(ptr.ref(), index);	
      }
    }
    
    vec.push_back(ptr);
  }
  
  return vec.size()-size;
}

//==============================================================================
unsigned Anp::ReadNtuple::FillGenEvent(const List &l, std::vector<Ptr<GenEvent> > &vec)
{
  //
  // Fill vector of objects from branches
  //
  const unsigned size = vec.size();

  for(unsigned index = 0; index < l.GetVarSize(); ++index) {
    Ptr<GenEvent> ptr = ObjectFactory<GenEvent>::Instance().CreateObject();    
    
    if(fDebugVars) {
      log() << "FillGenEvent - index=" << index << endl;
    }
    for(const VecData &data: l.vars) {
      if(fDebugVars) {
	log() << "FillGenEvent - var=" << data.var << " branch=" << data.branch << endl;
      }

      data.AddVar(ptr.ref(), index);

      //if     (data.var == Cut::pdf_id1)      ptr->SetPdfId1      (data.GetFullValue<Float_t>(index));
      //else if(data.var == Cut::pdf_id2)      ptr->SetPdfId2      (data.GetFullValue<Float_t>(index));
      //else if(data.var == Cut::pdf_x1)       ptr->SetPdfX1       (data.GetFullValue<Float_t>(index));
      //else if(data.var == Cut::pdf_x2)       ptr->SetPdfX2       (data.GetFullValue<Float_t>(index));
      //else if(data.var == Cut::pdf_scale)    ptr->SetPdfScale    (data.GetFullValue<Float_t>(index));
      
    }
    
    ptr->SetBarcode(Anp::MakeBarcode(ptr->GetObjectType()));
    vec.push_back(ptr);
  }
  
  return vec.size()-size;
}

//==============================================================================
unsigned Anp::ReadNtuple::FillRecoShw(const List &l, std::vector<Ptr<RecShower> > &vec)
{
  //
  // Fill vector of objects from branches
  //
  const unsigned size = vec.size();

  for(unsigned index = 0; index < l.GetVarSize(); ++index) {
    Ptr<RecShower> ptr = ObjectFactory<RecShower>::Instance().CreateObject();    
    
    if(fDebugVars) {
      log() << "FillRecoShw - index=" << index << endl;
    }

    pair<bool, float> val_et (false, 0.0);
    pair<bool, float> val_eta(false, 0.0);
    pair<bool, float> val_phi(false, 0.0);
    
    for(const VecData &data: l.vars) {
      if(fDebugVars) {
	log() << "FillRecoShw - var=" << data.var << " branch=" << data.branch << endl;
      }

      data.AddVar(ptr.ref(), index);

      if     (data.var == Cut::Et)  { val_et .first = true; val_et .second = data.GetFullValue<Float_t>(index); }
      else if(data.var == Cut::Eta) { val_eta.first = true; val_eta.second = data.GetFullValue<Float_t>(index); }
      else if(data.var == Cut::Phi) { val_phi.first = true; val_phi.second = data.GetFullValue<Float_t>(index); }
    }
    
    ptr->SetBarcode(Anp::MakeBarcode(ptr->GetObjectType()));


    if(val_et.first && val_eta.first && val_phi.first) {
      ptr->SetClusterEtEtaPhi(val_et.second, val_eta.second, val_phi.second);
    }

    vec.push_back(ptr);
  }
  
  return vec.size()-size;
}

//==============================================================================
unsigned Anp::ReadNtuple::FillRecoHit(const List &l, std::vector<Ptr<RecHit> > &vec)
{
  //
  // Fill vector of objects from branches
  //
  const unsigned size = vec.size();

  for(unsigned index = 0; index < l.GetVarSize(); ++index) {
    Ptr<RecHit> ptr = ObjectFactory<RecHit>::Instance().CreateObject();    
    
    if(fDebugVars) {
      log() << "FillRecoHit - index=" << index << endl;
    }

    for(const VecData &data: l.vars) {
      if(fDebugVars) {
	log() << "FillRecoShw - var=" << data.var << " branch=" << data.branch << endl;
      }

      data.AddVar(ptr.ref(), index);
    }
    
    vec.push_back(ptr);
  }
  
  return vec.size()-size;
}

//==============================================================================
unsigned Anp::ReadNtuple::FillTruePar(const List &l, std::vector<Ptr<TruthPart> > &vec)
{
  //
  // Fill vector of objects from branches
  //
  const unsigned size = vec.size();

  if(fDebugVars) {      
    log() << "FillTruePar - process: " << l.prefix << " #var: " << l.GetVarSize() << endl;
  }

  for(unsigned index = 0; index < l.GetVarSize(); ++index) {
    if(fDebugVars) {
      log() << "FillTruePar - index=" << index << endl;
    }

    Ptr<TruthPart> ptr = ObjectFactory<TruthPart>::Instance().CreateObject(); 

    //
    // Set momentum
    //
    FillMom<TruthPart>(l.vars, index, ptr.ref());

    for(const VecData &data: l.vars) {
      if(fDebugVars) {
	log() << "FillTruePar - var=" << data.var << " branch=" << data.branch << endl;
      }

      data.AddVar(ptr.ref(), index);
      
      if(data.var == Cut::BIT)         ptr->SetBitField(data.GetFullValue<UInt_t>(index));
      if(data.var == Cut::PDG)         ptr->SetPdgId   (data.GetFullValue<Int_t> (index));
      if(data.var == Cut::TrueBarcode) ptr->SetBarcode (data.GetFullValue<Int_t> (index));
      if(data.var == Cut::Status)      ptr->SetStatus  (data.GetFullValue<Int_t> (index));
      
      if(data.var == Cut::IndexRangeChild) {
        std::vector<int> children;
        ptr->GetVarVec(Cut::IndexRangeChild,children);

        for(int child_bc : children) {
          if(fDebugVars) {
	    log()<< "Adding child " << child_bc << endl;
	  }

          ptr->AddChild(child_bc);
        }
      }
      
      if(data.var == Cut::IndexRangeParent) {
        std::vector<int> parents;
        ptr->GetVarVec(Cut::IndexRangeParent,parents);

        for(int parent_bc : parents) {
          if(fDebugVars) {
	    log()<< "Adding parent " << parent_bc << endl;
	  }

          ptr->AddParent(parent_bc);
        }
      }
    }
    
    vec.push_back(ptr);
  }
  
  return vec.size()-size;
}

//==============================================================================
unsigned Anp::ReadNtuple::FillReco(RecoEvent &event)
{
  //
  // Create event info and fill event
  //
  event.SetInfo(ObjectFactory<EventInfo>::Instance().CreateObject());

  //
  // Add vars to event
  //
  unsigned count = 0;

  for(const VarData &data: fVars) {
    data.AddVar(event);

    if(fDebugVars) {
      cout << "   var: " << setw(40) << std::left << Cut::AsStr(data.var) << " value: " << data.GetVal() << endl;
    }

    ++count;
  }

  //
  // Fill objects
  //
  for(List &l: fLists) {

    if(l.MatchKey(kTrack))    count += FillMomVec<RecTrack   >(l, event.GetVec<RecTrack >());
    if(l.MatchKey(kElec ))    count += FillMomVec<RecElec    >(l, event.GetVec<RecElec  >());
    if(l.MatchKey(kMuon ))    count += FillMomVec<RecMuon    >(l, event.GetVec<RecMuon  >());
    if(l.MatchKey(kTauR ))    count += FillMomVec<RecTau     >(l, event.GetVec<RecTau   >());
    if(l.MatchKey(kJet4 ))    count += FillMomVec<RecJet     >(l, event.GetVec<RecJet   >());
    if(l.MatchKey(kJet6 ))    count += FillMomVec<RecJet     >(l, fJets.GetRef());
    if(l.MatchKey(kTrackJet)) count += FillMomVec<RecTrackJet>(l, event.GetVec<RecTrackJet>());
    if(l.MatchKey(kRVtx ))    count += FillObjVec<RecVertex>  (l, event.GetVec<RecVertex  >());
    if(l.MatchKey(kGen  ))    count += FillGenEvent           (l, event.GetVec<GenEvent   >());
    if(l.MatchKey(kClust))    count += FillRecoShw            (l, event.GetVec<RecShower  >());
    if(l.MatchKey(kHit))      count += FillRecoHit            (l, event.GetVec<RecHit     >());

    if(fFillTrueParts) {
      if(l.MatchKey(kTauV ))    count += FillMomVec<TrueTau  >(l, event.GetVec<TrueTau  >());
      if(l.MatchKey(kTruth))    count += FillTruePar          (l, event.GetVec<TruthPart>());
      if(l.MatchKey(kTruthVtx)) count += FillObjVec<TruthVtx >(l, event.GetVec<TruthVtx >());
    }
  }

  return count;
}

//==============================================================================
void Anp::ReadNtuple::ReadNick(const Registry &reg)
{
  //
  // Read variable nicknames
  //
  fNicks = ReadNicks("ReadNtuple::VarNicks", reg);
  
  if(!fNicks.empty()) {
    log() << "ReadNick - read " << fNicks.size() << " nickname(s)" << endl;
    
    for(const VarNick &nick: fNicks) {
      cout << "   var: " << setw(30) << std::left << Cut::AsStr(nick.key) 
	   << " nickname: " << nick.nick << endl;
    }
  }
}

//==============================================================================
Anp::Cut::Var Anp::ReadNtuple::FindVar(const std::string &val) const
{ 
  for(const VarNick &nick: fNicks) {
    if(nick.nick == val) {
      return nick.key;
    }
  }
  
  return Cut::Convert2Var(val);
}

//==============================================================================
set<Anp::ReadNtuple::KeyBranch> Anp::ReadNtuple::GetKeyBranchSet(const string &prefix) const
{
  set<KeyBranch> bset;

  if(prefix == "m_clust_")         bset.insert(kClust);
  if(prefix == fElecVecPrefix)     bset.insert(kElec);
  if(prefix == fMuonVecPrefix)     bset.insert(kMuon);
  if(prefix == fTauRVecPrefix)     bset.insert(kTauR);
  if(prefix == fTauVVecPrefix)     bset.insert(kTauV);
  if(prefix == fJetVecPrefix)      bset.insert(kJet4);
  if(prefix == "m_jet6_" )         bset.insert(kJet6);
  if(prefix == fTrackJetVecPrefix) bset.insert(kTrackJet);
  if(prefix == fRecoVtxVecPrefix ) bset.insert(kRVtx);
  if(prefix == "m_mcevt_" )        bset.insert(kGen);
  if(prefix == "m_tvtx_" )         bset.insert(kTVtx);
  if(prefix == fTrackVecPrefix)    bset.insert(kTrack);
  if(prefix == fHitVecPrefix)      bset.insert(kHit);
  if(prefix == fTruthVecPrefix)    bset.insert(kTruth);
  if(prefix == fTruthVtxVecPrefix) bset.insert(kTruthVtx);

  return bset;
}
