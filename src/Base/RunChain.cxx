
// C/C++
#include <cstdlib>
#include <exception>
#include <iostream>
#include <sstream>

// Local
#include "PhysicsBase/Factory.h"
#include "PhysicsBase/UtilCore.h"

// Local
#include "RunChain.h"

REGISTER_ANP_OBJECT(AlgEvent,RunChain)

using namespace std;

//==============================================================================
Anp::RunChain::RunChain():
  fDir   (0),
  fCandId(0),
  fRecoN (0.0),
  fRecoW (0.0),
  fCandN (0.0),
  fCandW (0.0)
{
}

//==============================================================================
Anp::RunChain::~RunChain()
{
}

//==============================================================================
void Anp::RunChain::Config(const Anp::Registry &reg)
{
  //
  // Configure base class - create and configure algorithms
  //
  reg.Get(GetType(), "Debug",     fDebug     = false);
  reg.Get(GetType(), "DebugVars", fDebugVars = false);
  reg.Get(GetType(), "Print",     fPrint     = false);

  if(fDebug) {
    log() << "Config - begin..." << endl;
  }

  fReg = reg;

  //
  // Configure cut-flow
  //
  fCutCand = BookCut("CutCand", reg);
  
  reg.Get("RunChain::PrintCounts",    fPrintCounts    = false);
  reg.Get("RunChain::PrintConfig",    fPrintConfig    = false);
  reg.Get("RunChain::FirstEventInit", fFirstEventInit = false);
  reg.Get("RunChain::DoTimer",        fDoTimer        = false); 
  reg.Get("RunChain::NoWeight",       fNoWeight       = false);
  reg.Get("RunChain::DoScaleWeight",  fDoScaleWeight  = false);
  reg.Get("RunChain::ScaleWeight",    fScaleWeight); 
  reg.Get("RunChain::DirName",        fDirName);

  if(reg.Get("RunChain::CandName", fCandName)) {
    fCandId = Anp::String2Hash(fCandName);
  }

  fCandWeightsSiml = Cut::ReadVars(reg, "RunChain::CandWeightsSiml", GetName());
  fCandWeightsData = Cut::ReadVars(reg, "RunChain::CandWeightsData", GetName());

  if(fPrint || fPrintConfig) {
    log() << "Config - config summary: "  << endl
	  << "   DirName:           " << fDirName                << endl
	  << "   CandName:          " << fCandName               << endl
	  << "   FirstEventInit:    " << fFirstEventInit         << endl
	  << "   DoTimer:           " << fDoTimer                << endl;
    
    cout << "   Number of siml weights: " << fCandWeightsSiml.size() << endl;

    for(const Cut::Var var: fCandWeightsSiml) {
      cout << "      " << Cut::AsStr(var) << endl;
    }

    cout << "   Number of data weights: " << fCandWeightsData.size() << endl;

    for(const Cut::Var var: fCandWeightsData) {
      cout << "      " << Cut::AsStr(var) << endl;
    }

    PrintCutConfig();
  }

  if(!fFirstEventInit) {
    if(fDebug) {
      log() << "Config - book alg..." << endl;
    }

    fAlg = BookAlg<RunAlgs>("RunAlgs", GetName(), fReg);

    if(fDebug) {
      log() << "Config - booked: " << fAlg->GetName() << endl;
    }
  }

  //
  // Input events
  //
  fReco = NtupleSvc::Instance("reco").Branch<std::vector<RecoEvent> >("events");

  fTimerExec.Reset();
  fTimerPrep.Reset();
}

//==============================================================================
void Anp::RunChain::Signal(const Registry &reg, const string &signal)
{
  //
  // Send signal to children
  //
  if(fAlg.valid()) {
    fAlg->Signal(reg, signal);
  }
}

//==============================================================================
void Anp::RunChain::Save(TDirectory *dir)
{
  //
  // Set directory
  //
  fDir = dir;
  
  //
  // Set directory for children
  //
  if(!fFirstEventInit && fAlg.valid()) {
    fAlg->Save(Anp::GetDir(fDir, fDirName));
  }
}

//==============================================================================
bool Anp::RunChain::Init()
{
  //
  // Initialize sub-algorithms
  //
  if(!fFirstEventInit && fAlg.valid()) {
    fAlg->Init();
  }
  
  return true;
}

//==============================================================================
void Anp::RunChain::Exec()
{
  //
  // Invalid reco branch - call children and exit
  //
  if(fDebug) {
    log() << "Exec - begin..." << endl;
  }

  if(!fReco.IsValidEvent()) {
    if(fDebug) {
      log() << "Exec - RecoEvent branch is invalid - do nothing" << endl;
    }

    return;
  }

  //
  // Process candidates
  //
  for(RecoEvent &reco: fReco.GetRef()) {
    Process(reco);
  }

  if(fDebug) {
    log() << "Exec - all is done..." << endl;
  }
}

//==============================================================================
void Anp::RunChain::Done()
{  
  if(fPrint || fPrintCounts) {
    log() << "Done - event summary: " << endl
	  << "   RecoN: " << fRecoN << endl
	  << "   RecoW: " << fRecoW << endl
	  << "   CandN: " << fCandN << endl
	  << "   CandW: " << fCandW << endl;

    PrintCutFlows();
  }

  if(fDoTimer) {
    log() << "Done - timer summary:" << endl
	  << "   Exec:  " << Anp::PrintResetStopWatch(fTimerExec) << endl
	  << "   Prep:  " << Anp::PrintResetStopWatch(fTimerPrep) << endl;   
  }

  if(fAlg.valid()) {
    //
    // Save cut-flow only if algs exist
    //
    SaveCutFlows(Anp::GetDir(fDir, fDirName));
  
    //
    // Call end for sub-algorithms
    //
    fAlg->Done();
  }
}

//==============================================================================
void Anp::RunChain::Process(RecoEvent &reco)
{
  //
  // Invalid reco branch - call children and exit
  //
  fInputCands.clear();
  fEventCands.clear();
  fWeights.clear();

  if(fDoTimer) {
    fTimerPrep.Start(false);
  }

  fRecoN += 1;
  fRecoW += reco.GetWeight();

  if(fDebug) {
    log() << "Process - begin - number of CandEvent: " << reco.GetVec<CandEvent>().size() << endl;
  }

  //
  // Select candidates
  //
  fInputCands = reco.GetVec<CandEvent>();

  for(Ptr<CandEvent> &cand: reco.GetVec<CandEvent>()) {
    //
    // Save all candidates - to be restored later
    //
    if(!fWeights.insert(map<uint64_t, double>::value_type(cand->GetBarcode(), cand->GetWeight())).second) {
     log() << "Process - duplicate barcode: " << cand->GetBarcode() << endl;
    }

    if(fCandId != 0 && cand->GetCandId() != fCandId) {
      if(fDebug) {
	log() << "Process - skip not matching candidate: " << cand->GetCandName() << "!=" << fCandName << endl;
      }
      continue;
    }

    //
    // Update event weight after cust (b-tagging weights, scale factors, etc)
    //
    if(cand->GetInfo()->IsMC() && !fCandWeightsSiml.empty()) {
      cand->SetWeight(GetEventWeight(cand.ref(), fCandWeightsSiml));
    }
    else if(cand->GetInfo()->IsData() && !fCandWeightsData.empty()) {
      cand->SetWeight(GetEventWeight(cand.ref(), fCandWeightsData));
    }
    else if(fNoWeight) {
      cand->SetWeight(1.0);
    }

    if(fDoScaleWeight) {
      cand->SetWeight(fScaleWeight*cand->GetWeight());
    }

    if(fCutCand->PassCut(cand.ref(), cand->GetEventInfo().ref())) {
      if(fDebug) {
	log() << "Process - pass candidate: " << cand->GetCandName() << "=" << fCandName 
	      << " weight=" << cand->GetWeight() << endl;
      }
    
      if(fCandWeightsSiml.empty() && fCandWeightsData.empty() && !fNoWeight) {
	cand->SetWeight(fCutCand->GetCutWeight());
      }

      fCandN += 1;
      fCandW += cand->GetWeight();

      //
      // Save selected candidates
      //
      fEventCands.push_back(cand);
    }
    else {
      if(fDebug) {
	log() << "Process - fail candindate: " << cand->GetCandName() << "!=" << fCandName << endl;
      }
    }

    if(fDebugVars) {
      for(const VarEntry &var: cand->GetVars()) {
	log() << "Process - key[" << var.GetKey() << "] = " << var.GetData() << std::endl;
      }
    }
  }
  
  //
  // Replace event candidates with selected candidates
  //
  reco.GetVec<CandEvent>() = fEventCands;

  if(fDoTimer) {
    fTimerPrep.Stop();
    fTimerExec.Start(false);
  }

  //
  // Execute sub-algorithms
  // 
  if(fFirstEventInit) {

    if(fDebug) {
      log() << "Exec - doing FirstEventInit..." << endl;
    }

    if(!fEventCands.empty()) {

      if(!fAlg.valid()) {
	if(fDebug) {
	  log() << "Exec - create RunAlgs..." << endl;
	}
	
	fAlg = BookAlg<RunAlgs>("RunAlgs", GetName(), fReg);
	fAlg->Save(Anp::GetDir(fDir, fDirName));
	fAlg->Init();
	
	if(fDebug) {
	  fReg.Print();
	}
      }
      
      if(fAlg.valid()) {
	if(fDebug) {
	  log() << "Exec - process sub-algorithms..." << endl;
	}
	
	fAlg->Exec();
      }
    }
  }
  else {
    if(fAlg.valid()) {
      if(fDebug) {
	log() << "Exec - process sub-algorithms..." << endl;
      }
      
      fAlg->Exec();
    }    
  }

  //
  // Restore candidates
  //
  if(fDoTimer) {
    fTimerExec.Stop();
    fTimerPrep.Start(false);
  }

  //
  // Reset event candidates
  //
  reco.GetVec<CandEvent>() = fInputCands;

  for(Ptr<CandEvent> &cand: fInputCands) {
    //
    // Restore candidate weights: !!!ASSUME ONLY WEIGHTS CHANGED!!!
    //
    const map<uint64_t, double>::const_iterator wit = fWeights.find(cand->GetBarcode());

    if(wit != fWeights.end()) {
      cand->SetWeight(wit->second);
    }
    else {
      log() << "Process - missing barcode: " << cand->GetBarcode() << endl;      
    }
  }

  if(fDoTimer) {
    fTimerPrep.Stop();
  }

  if(fDebug) {
    log() << "Process - all is done..." << endl;
  }
}

//==============================================================================
double Anp::RunChain::GetEventWeight(const CandEvent &event, const std::vector<Cut::Var> &weights) const
{
  if(fDebug) {
    log() << "GetEventWeight - number of weights: " << weights.size() << endl;
  }

  long double weight = 1.0;

  for(const Cut::Var var: weights) {
    double wvar = 0.0;
    
    if(event.GetVar(var, wvar)) {
      weight *= wvar;
      
      if(fDebug) {
	log() << "GetEventWeight - var=" << Cut::AsStr(var) << " value=" << wvar << endl;
      }
    }
    else {
      if(fDebug) {
	log() << "GetEventWeight - missing var=" << Cut::AsStr(var) << endl;
      }
    }
  }

  if(fDebug) {
    log() << "GetEventWeight - total weight=" << weight << endl;
  }

  return weight;
}
