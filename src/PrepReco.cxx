// C/C++
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cstdlib>

// ROOT
#include "TFile.h"
#include "TMath.h"

// Base
#include "PhysicsBase/Factory.h"
#include "PhysicsBase/UtilCore.h"

// Local
#include "PhysicsTaus/CutVars.h"
#include "PhysicsTaus/Utils.h"

// Local
#include "PrepReco.h"

REGISTER_ANP_OBJECT(AlgEvent,PrepReco)

using namespace std;

//======================================================================================================
template<class T> bool Anp::PrepReco::AddVar(T &obj, Cut::Var var, double val)
{
  //
  // Add var if it does not exist, otherwise check values for consitency
  //
  double cur = 0.0;
  
  if(obj.GetVar(var, cur)) {
    static map<Cut::Var, unsigned> counts;
    
    const unsigned icount = counts.insert(map<Cut::Var, unsigned>::value_type(var, 0)).first->second++;

    if(icount < 10) {
      log() << "AddVar - different value already exist for " << Cut::AsStr(var) << ": " 
	    << "cur=" << setprecision(10) << cur << " and new=" << setprecision(10) << val;
      
      if(icount == 9) {
	cout << "... last message" << endl;
      }
      else {
	cout << endl;
      }
    }

    return false;
  }

  return obj.AddVar(var, val);
}

//======================================================================================================
Anp::PrepReco::PrepReco():
  fDir       (0),
  fMassElec  (   0.511),
  fMassMuon  ( 105.658),
  fMassTau   (1776.84)
{
}

//======================================================================================================
Anp::PrepReco::~PrepReco()
{
}

//======================================================================================================
template<class T> void Anp::PrepReco::Filter(vector<Ptr<T> > &objs, int pdg)
{
  typename vector<Ptr<T> >::iterator it = objs.begin();

  while(it != objs.end()) {
    double val = 0.0;
    
    if((*it)->GetVar(Cut::PDG, val)) {

      if(std::abs(static_cast<int>(val)) == pdg) {
	++it;
      }
      else {
	it = objs.erase(it);
      }

      if(fDebugVars) {
	log() << "Filter - remove object with id: " << val << " != " << pdg << endl;
      }
    }
    else { 
      //
      // Add missing PDG ID
      //
      AddVar((*it).ref(), Cut::PDG, pdg);

      //
      // Increment iterator
      //
      ++it;
    }
  }
}

//======================================================================================================
template<class T> void Anp::PrepReco::AddLepMomVars(T &obj, double mass)
{  
  //
  // Add lepton mass and momentum variables
  //
  double mval = 0.0;
  
  if(obj.GetVar(Cut::Mass, mval)) {
    
    if(std::fabs(mval - mass) > 0.001) {
      log() << "AddLepMomVars - mass difference: " << mval << "-" << mass << "=" << mval - mass << endl;
    }
  }
  else { 
    AddVar(obj, Cut::Mass, mass);
  }
  
  pair<bool, double>    p_en (false, 0.0);
  pair<bool, double>    p_m  (false, 0.0);
  pair<bool, double>    p_pt (false, 0.0);
  pair<bool, double>    p_eta(false, 0.0);
  pair<bool, double>    p_phi(false, 0.0);

  p_en .first = obj.GetVar(Cut::Energy, p_en .second);
  p_m  .first = obj.GetVar(Cut::Mass,   p_m  .second);
  p_pt .first = obj.GetVar(Cut::Pt,     p_pt .second);
  p_eta.first = obj.GetVar(Cut::Eta,    p_eta.second);
  p_phi.first = obj.GetVar(Cut::Phi,    p_phi.second);

  const bool val3 = p_pt.first && p_eta.first && p_phi.first;
  
  FourMom save = obj.GetFourMom();

  if(fDebugVars) {
    log() << "AddLepMomVars - attempt to compute: SetPtEtaPhi: " 
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

      log() << "AddLepMomVars - missing energy and mass to build 4-momentum" << endl;
    }
  }

  //
  // Update four momentum
  //
  if(fDebugVars) {
    TLorentzVector lv = obj.GetFourMom().GetLV() - save.GetLV();
    
    log() << "AddLepMomVars - mass=" << mass << endl
	  << "   Pt:  " << obj.GetFourMom().GetLV().Pt () << endl
	  << "   Eta: " << obj.GetFourMom().GetLV().Eta() << endl
	  << "   Phi: " << obj.GetFourMom().GetLV().Phi() << endl
	  << "   E:   " << obj.GetFourMom().GetLV().E  () << endl
	  << "   Mag: " << obj.GetFourMom().GetLV().Mag() << endl
	  << "   Diff Pt:  " << lv.Pt () << endl
	  << "   Diff Eta: " << lv.Eta() << endl
	  << "   Diff Phi: " << lv.Phi() << endl
	  << "   Diff E:   " << lv.E  () << endl
	  << "   Diff Mag: " << lv.Mag() << endl;
  }
}

//======================================================================================================
template<class T> void Anp::PrepReco::SetCharge(vector<Ptr<T> > &objs)
{  
  //
  // Add charge
  //
  for(Ptr<T> &ptr: objs) {
    double val = 0.0;
    
    if(ptr->GetVar(Cut::PDG, val)) {
      if(val > 0.0) {
	ptr->SetCharge(-1.0);
      }
      else {
	ptr->SetCharge(+1.0);	
      }
    }
  }
}

//======================================================================================================
void Anp::PrepReco::Config(const Registry &reg)
{
  //
  // Read configuration
  //
  reg.Get("PrepReco", "Debug",        fDebug          = false);
  reg.Get("PrepReco", "DebugVars",    fDebugVars      = false);
  reg.Get("PrepReco", "Print",        fPrint          = false);
  reg.Get("PrepReco::NoWeight",       fNoWeight       = false);
  reg.Get("PrepReco::DoCopy",         fDoCopy         = false);
  reg.Get("PrepReco::DoCount",        fDoCount        = false);
  reg.Get("PrepReco::DoLepIdFilter",  fDoLepIdFilter  = true);
  reg.Get("PrepReco::DoTrue",         fDoTrue         = true);
  reg.Get("PrepReco::CheckDuplicate", fCheckDuplicate = true);
  reg.Get("PrepReco::AddLeptonMass",  fAddLeptonMass  = true);
  reg.Get("PrepReco::ReadSumWeight",  fReadSumWeight  = true);
  reg.Get("PrepReco::MassElec",       fMassElec       =    0.511);
  reg.Get("PrepReco::MassMuon",       fMassMuon       =  105.658);
  reg.Get("PrepReco::MassTau",        fMassTau        = 1776.84);

  fWeights    = Cut::ReadVars(reg, "PrepReco::Weights",    GetName());
  fWeightsInv = Cut::ReadVars(reg, "PrepReco::WeightsInv", GetName());
  fWeightsDiv = Cut::ReadVars(reg, "PrepReco::WeightsDiv", GetName());

  //
  // Read MC run normalization
  //
  vector<string> str_runs;
  reg.Get("PrepReco::Runs", str_runs);
  
  for(const string &str_run: str_runs) {
    unsigned int_run = 0;
    double    weight = 0.0;

    stringstream str;
    str << str_run;
    str >> int_run;
    
    if(!str.fail() && int_run > 100000 && int_run < 1000000 && reg.Get("PrepReco::"+str_run, weight)) {
      RunData data;
      data.run    = int_run;
      data.weight = weight;
      
      fRuns.push_back(data);
    }
  }

  if(fPrint) {
    log() << "Config - print configuration:"        << endl
	  << "   NoWeight:      " << fNoWeight      << endl
	  << "   DoCopy:        " << fDoCopy        << endl
	  << "   DoCount:       " << fDoCount       << endl
	  << "   DoLepIdFilter: " << fDoLepIdFilter << endl
	  << "   MassElec:      " << fMassElec      << endl
	  << "   MassMuon:      " << fMassMuon      << endl
	  << "   MassTau:       " << fMassTau       << endl
	  << "   AddLeptonMass: " << fAddLeptonMass << endl;

    log() << "   number of   weight var(s): " << fWeights.size() << endl;    
    for(const Cut::Var &var: fWeights) {
      cout << "   var: " << Cut::AsStr(var) << endl;
    }
    
    log() << "   number of 1-weight var(s): " << fWeightsInv.size() << endl;
    for(const Cut::Var &var: fWeightsInv) {
      cout << "   var: " << Cut::AsStr(var) << endl;
    }

    log() << "   number of 1/weight var(s): " << fWeightsDiv.size() << endl;
    for(const Cut::Var &var: fWeightsDiv) {
      cout << "   var: " << Cut::AsStr(var) << endl;
    }
  }

  if(fDebug) {
    log() << "Config - read " << fRuns.size() << " run(s)" << endl;
    
    for(const RunData &run: fRuns) {
      stringstream str;
      str << "   run=" << run.run << " weight=" << setprecision(10) << run.weight;
      cout << str.str() << endl;
    }
  }

  //
  // Copy object
  //
  if(fDoCopy) {
    fCopy = ObjectFactory<CopyHist>::Instance().CreateObject();
    fCopy->Config(reg, GetType());
  }

  //
  // Input events
  //
  fReco = NtupleSvc::Instance("reco").Branch<std::vector<RecoEvent> >("events");
}

//======================================================================================================
void Anp::PrepReco::Save(TDirectory *dir)
{
  fDir = dir;
}

//======================================================================================================
bool Anp::PrepReco::Init()
{
  return true;
}

//======================================================================================================
void Anp::PrepReco::Exec()
{
  //
  // Read new event entry
  //
  if(!fReco.IsValidEvent()) {
    return;
  }  

  for(RecoEvent &event: fReco.GetRef()) {
    ProcessReco(event);
    
    if(fDoTrue) {
      ProcessTrue(event);
    }
  }
}

//======================================================================================================
void Anp::PrepReco::Done()
{
  if(fDir) {
    SaveCutsHists(Anp::GetDir(fDir, GetName()));
  }

  if(fDir && fCopy.valid()) {
    fCopy->SaveObjects(Anp::GetDir(fDir, "Counts"));
  }
  
  for(ChanMap::value_type &chan: fChanMap) {
    stringstream dirname;
    dirname << "Counts/" << chan.first;

    TDirectory *dir = Anp::GetDir(fDir, dirname.str());
    
    if(!dir) {
      log() << "SaveObjects - invalid output TDirectory: " << dirname.str() << endl;
      return;
    }
    
    TH1 *h = new TH1D("event", "event", 2, 0.0, 2.0);
    h->SetDirectory(dir);
    h->GetYaxis()->SetTitle("Events");
    h->GetXaxis()->SetBinLabel(1, "Event counts");
    h->GetXaxis()->SetBinLabel(2, "Event weight");
    
    h->Fill(0.5, chan.second.eventN);
    h->Fill(1.5, chan.second.eventW);
    
    log() << "CopyObjects - save TH1: " << dirname.str() << "/" << h->GetName() << endl;    
  }
}

//======================================================================================================
void Anp::PrepReco::Signal(const Registry &reg, const string &key)
{
  //
  // Callback from top level module
  //
  if(fCopy.valid()) {
    fCopy->Signal(reg, key);
  }
  
  if(fReadSumWeight && key == "NEW_FILE") {
    string fpath;
    
    if(reg.Get("SIGNAL::FilePath", fpath)) {
      ReadSumWeightHist(fpath);
    }
  }
}

//======================================================================================================
void Anp::PrepReco::ProcessReco(RecoEvent &event)
{   
  //
  // Process reco info
  //
  vector<Ptr<RecElec> > &elecs = event.GetVec<RecElec>();
  vector<Ptr<RecMuon> > &muons = event.GetVec<RecMuon>();
  vector<Ptr<RecJet > > &jets  = event.GetVec<RecJet >();
  vector<Ptr<RecTau > > &taus  = event.GetVec<RecTau >();
  
  if(fDoLepIdFilter) {
    Filter(elecs, 11);
    Filter(muons, 13);
  }

  if(fAddLeptonMass) {
    for(Ptr<RecElec> &ptr: elecs) { AddLepMomVars(ptr.ref(), fMassElec); }
    for(Ptr<RecMuon> &ptr: muons) { AddLepMomVars(ptr.ref(), fMassMuon); }
    for(Ptr<RecTau > &ptr: taus)  { AddLepMomVars(ptr.ref(), fMassTau);  }
  }

  //
  // Set charge variable for convience
  //
  SetCharge(elecs);
  SetCharge(muons);

  //
  // Sort reco objects by pt
  //
  std::sort(muons.begin(), muons.end(), SortObjectByPt());
  std::sort(elecs.begin(), elecs.end(), SortObjectByPt());
  std::sort(jets .begin(), jets .end(), SortObjectByPt());
  std::sort(taus .begin(), taus .end(), SortObjectByPt());

  //
  // Check event duplicates
  //
  Ptr<EventInfo> info = event.GetEventInfo();

  uint64_t evt = 0;
  int run      = GetInt(Cut::Run,       event);
  int mcr      = GetInt(Cut::MCChannel, event, 0);

  if(event.HasVar(Cut::Event)) {
    evt = static_cast<uint64_t>(event.GetVar(Cut::Event));
  }

  if(fCheckDuplicate) {
    if(fCheck.insert(RunEvent(run, evt)).second) {
      AddVar(event, Cut::IsDuplicate, 0.0);    
    }
    else if(evt != 0) {
      AddVar(event, Cut::IsDuplicate, 1.0);
      if(fDebug) {
	log() << "ProcessReco - duplicate event: " << run << ", " << evt << endl;
      }
    }
  }

  //
  // Set data/MC flags
  //
  info->SetRun      (run);
  info->SetEvent    (evt);
  info->SetMCChannel(mcr);

  if(event.HasKey(Cut::MCChannel)) { 
    if(mcr == 0) info->AddBit(EventFlags::IsData);
    else         info->AddBit(EventFlags::IsSimulation);
  }
}

//======================================================================================================
void Anp::PrepReco::ProcessTrue(RecoEvent &event)
{     
  //
  // Fill MC info
  //
  Ptr<EventInfo> info = event.GetEventInfo();

  pair<bool, unsigned> val_mcchan(false, 0  );

  for(const VarEntry &var: event.GetVars()) {
  
    if(var.GetKey() == Cut::MCChannel) { 
      val_mcchan.first  = true; 
      val_mcchan.second = static_cast<unsigned>(var.GetVal()) ;
    }    
  }
  
  if(val_mcchan.first) {
    info->SetMCChannel(val_mcchan.second);

    if(fDebug) {
      log() << "ProcessTrue - found MCChannel: " << val_mcchan.second << endl;
    }
  }
  else {
    info->SetMCChannel(0);
    AddVar(event, Cut::MCChannel,   0);
  }

  //
  // Do nothing and return for data
  //
  if(!(info->GetMCChannel() > 0)) {
    if(fDebug) {
      log() << "ProcessTrue - missing MCChannel: " << val_mcchan.second << endl;
    }

    AddVar(event, Cut::EventWeight, 1.0);
    return;
  }

  //
  // Set MC channel based event weight
  //
  double weight = 1.0;

  for(const RunData &run: fRuns) {
    if(run.run == info->GetMCChannel()) {
      weight *= run.weight;
      
      if(fDebug) {
	log() << "FillReco - mcrun=" << run.run << " weight=" << setprecision(9) << run.weight << endl;
      }
    }
  }

  for(const Cut::Var var: fWeights) {
    double wvar = 0.0;
    
    if(event.GetVar(var, wvar) && info->GetMCChannel() > 0) {
      weight *= wvar;
      
      if(fDebug) {
	log() << "FillReco - var=" << Cut::AsStr(var) << " wvar=" << wvar << endl;
      }
    }
  }
  
  for(const Cut::Var var: fWeightsInv) {
    double wvar = 0.0;
    
    if(event.GetVar(var, wvar) && info->GetMCChannel() > 0) {
      weight *= (1.0-wvar);
      
      if(fDebug) {
	log() << "FillReco - var=" << Cut::AsStr(var) << " inv wvar=" << wvar << endl;
      }
    }
  }
  
  for(const Cut::Var var: fWeightsDiv) {
    double wvar = 0.0;
    
    if(event.GetVar(var, wvar) && info->GetMCChannel() > 0 && wvar > 0.0) {
      weight *= 1.0/wvar;
      
      if(fDebug) {
	log() << "FillReco - var=" << Cut::AsStr(var) << " 1/w wvar=" << wvar << endl;
      }
    }
  }

  //
  // Turn off event weight
  //
  if(fNoWeight) {
    event.SetWeight(1.0);
  }
  else {
    event.SetWeight(weight); 
  }

  AddVar(event, Cut::EventWeight, weight);

  if(fDoCount) {
    ProcessCount(info->GetMCChannel(), event.GetWeight());
  }

  if(val_mcchan.first && fCopy.valid()) {
    fCopy->CopyObjects(val_mcchan.second, event.GetWeight());
  }
}

//======================================================================================================
void Anp::PrepReco::ProcessCount(int mc_chan, double weight)
{
  //
  // Count events
  //
  if(mc_chan > 0) {
    ChanMap::iterator cit = fChanMap.insert(ChanMap::value_type(mc_chan, ChanData())).first;
    cit->second.eventN += 1;
    cit->second.eventW += weight;
  }
}

//======================================================================================================
bool Anp::PrepReco::ReadSumWeightHist(const std::string &fpath)
{
  //
  // Read sumWeight tree for counts
  //

  TFile *rfile = TFile::Open(fpath.c_str(), "READ");
  
  if(!rfile || !rfile->IsOpen()) {
    log() << "ReadSumWeightHist - failed to open: " << fpath << endl;
    return false;
  }

  TTree *tree = dynamic_cast<TTree *>(rfile->Get("sumWeights"));
  
  if(!tree) {
    if(fDebug) {
      log() << "ReadSumWeightHist - failed to open: " << fpath << ":sumWeights" << endl;
    }
    return false;
  }

  Int_t     dsid        = 0;
  ULong64_t totalEvents = 0;
  Float_t   totalWeight = 0.0;

  tree->SetBranchAddress("dsid",                &dsid);
  tree->SetBranchAddress("totalEvents",         &totalEvents);
  tree->SetBranchAddress("totalEventsWeighted", &totalWeight);

  int    mc_number  = 0;
  double sum_simple = 0.0;
  double sum_weight = 0.0;
  
  for(int i = 0; i < tree->GetEntries(); ++i) {
    tree->GetEntry(i);

    sum_simple += totalEvents;
    sum_weight += totalWeight;

    if(mc_number == 0) {
      mc_number = dsid;
    }
    else if (mc_number != dsid) {
      log() << "ReadSumWeightHist - DSID mismatch: " << mc_number << "!=" << dsid << endl;
    }
  }
  

  stringstream str;
  str << "sumWeights/" << mc_number;
  
  TDirectory *dir = Anp::GetDir(fDir, str.str());

  TH1 *hc = new TH1D("Count", "Count", 3, 0, 3);
  hc->SetStats(false);
  hc->SetDirectory(dir);
  hc->GetXaxis()->SetBinLabel(1, "totalEventsWeighted");
  hc->GetXaxis()->SetBinLabel(2, "totalEventsWeighted");
  hc->GetXaxis()->SetBinLabel(3, "totalEvents");

  hc->SetBinContent(1, sum_weight);
  hc->SetBinContent(2, sum_weight);
  hc->SetBinContent(3, sum_simple);

  log() << "ReadSumWeightHist - MC channel=" << mc_number << " events=" << sum_simple << " sum weight=" << sum_weight << endl;

  return true;
}
