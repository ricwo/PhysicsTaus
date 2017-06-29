

// C/C++
#include <cstdlib>

// ROOT
#include "TDirectory.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"

// Local
#include "PhysicsBase/AlgEvent.h"
#include "PhysicsBase/HistMan.h"
#include "PhysicsBase/Registry.h"

// Local
#include "PhysicsTaus/Utils.h"
#include "PhysicsTaus/HistBase.h"

using namespace std;

//-----------------------------------------------------------------------------
Anp::HistBase::HistBase():
  fSetSumw2    (false),
  fPostProc    (false),
  fShowOverflow(true)
{
}

//-----------------------------------------------------------------------------
Anp::HistBase::~HistBase()
{
}

//-----------------------------------------------------------------------------
void Anp::HistBase::ConfigHistBase(const AlgEvent &alg, const Registry &reg)
{
  ConfigHistBase(alg.GetType(), reg);
}

//-----------------------------------------------------------------------------
void Anp::HistBase::ConfigHistBase(const string &key, const Registry &reg)
{
  fKeyHist = key;

  reg.Get(key, "SetSumw2",     fSetSumw2     = false);
  reg.Get(key, "PostProc",     fPostProc     = false);
  reg.Get(key, "SaveKeyHists", fSaveKeyHists = true);
  reg.Get(key, "ShowOverflow", fShowOverflow = true);
  reg.Get(key, "KeyHist",      fKeyHist);
  reg.Get(key+"::KeyHist",     fKeyHist);
  reg.Get(key+"::HistKey",     fKeyHist);
}

//-----------------------------------------------------------------------------
void Anp::HistBase::SaveHist(TDirectory *dir)
{
  //
  // Post Process Histograms
  //
  for(HistAll::value_type &h: fHistAll) {
    if(fPostProc && h.second) {
      PostProc(h.second);
    }
  }

  //
  // Save histograms
  //
  for(HistAll::value_type &h: fHistAll) {
    if(h.second && !(h.second->GetDirectory())) {
      h.second->SetDirectory(dir);
    }
  }

  for(TH1Vec::value_type &h: fTH1Vec) {
    if(h && !(h->GetDirectory())) {
      h->SetDirectory(dir);
    }
  }

  //
  // Save key histograms
  //
  for(Ptr<HistKey> &ptr: fHists) {
    ptr->SetDirHist(dir);
  }
}

//-----------------------------------------------------------------------------
void Anp::HistBase::ClearHistLists()
{
  //
  // Post Process Histograms
  //
  fHistAll.clear();
  fTH1Vec.clear();
  fHists.clear();
}

//-----------------------------------------------------------------------------
Anp::Ptr<Anp::HistKey> Anp::HistBase::BookHist(const Registry &reg, 
					       const std::string &dir, 
					       const std::string &pref)
{
  Ptr<HistKey> ptr = ObjectFactory<HistKey>::Instance().CreateObject();
  
  string atype;

  if(!reg.Get("AlgType", atype)) {
    if(!reg.Get("KeyHist", atype)) {
      cout << "HistBase::BookHist - missing AlgType/KeyHist key" << endl;    
      reg.Print();
    }
  }

  ptr->SetHistKey   (atype);
  ptr->SetHistPrefix(pref);
  ptr->SetHistDir   (dir);
  ptr->ConfigHist   (atype, reg);

  fHists.push_back(ptr);

  return ptr;
}

//-----------------------------------------------------------------------------
TH1* Anp::HistBase::GetTH1(const std::string &key, const std::string &name_)
{
  //
  // Create TH1 histogram and set directory
  //
  string name = name_;
  if(name.empty()) {
    name = key;
  }

  HistAll::iterator hit = fHistAll.find(name);
  if(hit != fHistAll.end()) {
    if(false) {
      cout << "GetTH1 - return existing histogram: " << key << "/" << name << endl;
    }
    return hit->second;
  }
  
  TH1 *h = Anp::HistMan::Instance().CreateTH1(key, fKeyHist);

  if(h) {
    if(!fHistAll.insert(HistAll::value_type(name, h)).second) {
      cout << "GetTH1 - ignore duplicate histogram: " << key << "/" << name << endl;
    }

    h->SetDirectory(0);

    if(!name.empty()) {
      h->SetName(name.c_str());
    }
    if(fSetSumw2) {
      h->Sumw2();
    }
  }

  return h;
}

//-----------------------------------------------------------------------------
TH2* Anp::HistBase::GetTH2(const std::string &key, const std::string &name_)
{
  //
  // Create TH2 histogram and set directory
  //
  string name = name_;
  if(name.empty()) {
    name = key;
  }

  HistAll::iterator hit = fHistAll.find(name);
  if(hit != fHistAll.end()) {
    return dynamic_cast<TH2 *>(hit->second);
  }

  TH2 *h = Anp::HistMan::Instance().CreateTH2(key, fKeyHist);

  if(h) {
    if(!fHistAll.insert(HistAll::value_type(name, h)).second) {
      cout << "GetTH2 - ignore duplicate histogram: " << key << "/" << name << endl;
    }

    h->SetDirectory(0);

    if(!name.empty()) {
      h->SetName(name.c_str());
    }
    if(fSetSumw2) {
      h->Sumw2();
    }
  }

  return h;
}

//-----------------------------------------------------------------------------
TH3* Anp::HistBase::GetTH3(const std::string &key, const std::string &name_)
{
  //
  // Create TH3 histogram and set directory
  //
  string name = name_;
  if(name.empty()) {
    name = key;
  }

  HistAll::iterator hit = fHistAll.find(name);
  if(hit != fHistAll.end()) {
    return dynamic_cast<TH3 *>(hit->second);
  }

  TH3 *h = Anp::HistMan::Instance().CreateTH3(key, fKeyHist);

  if(h) {
    if(!fHistAll.insert(HistAll::value_type(name, h)).second) {
      cout << "GetTH3 - ignore duplicate histogram: " << key << "/" << name << endl;
    }

    h->SetDirectory(0);

    if(!name.empty()) {
      h->SetName(name.c_str());
    }
    if(fSetSumw2) {
      h->Sumw2();
    }
  }

  return h;
}

//-----------------------------------------------------------------------------
TProfile* Anp::HistBase::GetTProfile(const std::string &name, int nbin, double xmin, double xmax)
{
  //
  // Create TH1 histogram and set directory
  //
  HistAll::iterator hit = fHistAll.find(name);
  if(hit != fHistAll.end()) {
    if(false) {
      cout << "GetTProfile - return existing histogram: " << name << endl;
    }
    return dynamic_cast<TProfile *>(hit->second);
  }
  
  TProfile *h = new TProfile(name.c_str(), name.c_str(), nbin, xmin, xmax);

  if(!fHistAll.insert(HistAll::value_type(name, h)).second) {
    cout << "GetTProfile - ignore duplicate histogram: " << name << endl;
  }
  
  h->SetDirectory(0);

  if(fSetSumw2) {
    h->Sumw2();
  }

  return h;
}
//-----------------------------------------------------------------------------
TH1* Anp::HistBase::MakeTH1(const std::string &key, const std::string &name_)
{
  //
  // Create TH1 histogram and set directory
  //
  string name = name_;
  if(name.empty()) {
    name = key;
  }

  TH1 *h = Anp::HistMan::Instance().CreateTH1(key, fKeyHist);

  if(h) {
    h->SetDirectory(0);

    if(!name.empty()) {
      h->SetName(name.c_str());
    }
    if(fSetSumw2) {
      h->Sumw2();
    }
    
    fTH1Vec.push_back(h);
  }

  return h;
}

//-----------------------------------------------------------------------------
TH2* Anp::HistBase::MakeTH2(const std::string &key, const std::string &name_)
{
  //
  // Create TH2 histogram and set directory
  //
  string name = name_;
  if(name.empty()) {
    name = key;
  }

  TH2 *h = Anp::HistMan::Instance().CreateTH2(key, fKeyHist);

  if(h) {
    h->SetDirectory(0);

    if(!name.empty()) {
      h->SetName(name.c_str());
    }
    if(fSetSumw2) {
      h->Sumw2();
    }

    fTH1Vec.push_back(h);
  }

  return h;
}

//-----------------------------------------------------------------------------
void Anp::HistBase::PostProc(TH1* hist)
{
  //
  // Get Last Bin number
  //
  unsigned int nBins = hist->GetNbinsX();
  
  //
  // Get Overflow
  //
  float overflow    = hist->GetBinContent(nBins+1);
  float overflowErr = hist->GetBinError(nBins+1);

  //
  // Get Last bin content
  //
  float lastBinVal = hist->GetBinContent(nBins);
  float lastBinErr = hist->GetBinError(nBins);
  float newLastBinErr = std::sqrt(lastBinErr*lastBinErr + overflowErr*overflowErr);

  //
  // Set New last Bin
  //
  if(lastBinVal+overflow){
    // Set 1st bin
    hist->SetBinContent(nBins,lastBinVal+overflow);
    hist->SetBinError(nBins,newLastBinErr);

    // Reset N-Entries
    hist->SetEntries(hist->GetEntries()-1);
  }

  //
  // Get Underflow
  //
  float underflow    = hist->GetBinContent(0);
  float underflowErr = hist->GetBinError(0);

  //
  // Get First Bin
  //
  float firstBin    = hist->GetBinContent(1);
  float firstBinErr = hist->GetBinError(1);
  float newFirstBinErr = std::sqrt(firstBinErr*firstBinErr + underflowErr*underflowErr);
  
  //
  // Set New first Bin
  //
  if(firstBin+underflow){
    // Set 1st bin
    hist->SetBinContent(1,firstBin+underflow);
    hist->SetBinError(1,newFirstBinErr);
    
    // Reset N-Entries
    hist->SetEntries(hist->GetEntries()-1);
  }

  return;
}
