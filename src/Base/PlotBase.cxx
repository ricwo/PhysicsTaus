
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
#include "PhysicsTaus/PlotBase.h"

using namespace std;

//======================================================================================================
Anp::PlotBase::PlotBase():
  fDir         (0),
  fDirTop      (0),
  hEventWeight (0)
{
}

//======================================================================================================
Anp::PlotBase::~PlotBase()
{
}

//======================================================================================================
void Anp::PlotBase::Config(const Registry &reg)
{
  //
  // Read configuration
  //
  string aname, atype;

  if(GetName().empty() && reg.Get("AlgName", aname)) SetName(aname);
  if(GetType().empty() && reg.Get("AlgType", atype)) SetType(atype);

  fKeyHist = GetType();
  fDirName = GetName();

  reg.Get(GetType()+"::DirName",    fDirName);
  reg.Get(GetType(), "Debug",       fDebug = false);
  reg.Get(GetType(), "Print",       fPrint = false);

  //
  // Configure histogram base class
  //
  ConfigHistBase(*this, reg);

  if(fDebug) {
    log() << "Config() is called" << endl;
  }

  //
  // Reco event
  //
  fReco = NtupleSvc::Instance("reco").Branch<std::vector<RecoEvent> >("events");

  //
  // Execute virtual function for derived class configuration
  //
  PlotConf(reg);

  //
  // Print cut configuration
  //
  if(fPrint) {
    log() << "Config" << endl
	  << "   DirName: " << fDirName << endl
      	  << "   KeyHist: " << fKeyHist << endl;
  }
}

//======================================================================================================
bool Anp::PlotBase::Init()
{
  //
  // Execute virtual function in case derived class needs to do Init()
  //
  if(fDebug) {
    log() << "Init() is called" << endl;
  }

  if(false) {
    hEventWeight = GetTH1("EventWeightSum");
  }

  //
  // Call init for derived algorithm
  //
  const bool result = PlotInit();

  //
  // Process sub-algorithms
  //
  for(Handle<AlgEvent> &alg: fSubAlgs) {
    if(fDebug) {
      log() << "Init() is called for sub-algorithm: " << alg->GetName() << "/" << alg->GetType() << endl;
    }
    alg->Init();
  }

  return result;
}

//======================================================================================================
void Anp::PlotBase::Save(TDirectory *dir)
{
  //
  // Initialize PlotBase
  //
  if(fDebug) {
    log() << "Save() is called: " << fDirName << endl;
  }

  fDirTop = dir;
  fDir    = Anp::GetDir(dir, fDirName);

  //
  // Process sub-algorithms
  //
  for(Handle<AlgEvent> &alg: fSubAlgs) {
    if(fDebug) {
      log() << "Save() is called for sub-algorithm: " << alg->GetName() << "/" << alg->GetType() << endl;
    }

    alg->Save(fDir);
  }
}

//======================================================================================================
void Anp::PlotBase::Done()
{
  if(fDebug) {
    log() << "Done() is called" << endl;
  }

  //
  // Execute virtual function
  //
  PlotDone();

  //
  // Process sub-algorithms
  //
  for(Handle<AlgEvent> &alg: fSubAlgs) {
    if(alg.valid()) {
      alg->Done();
    }
  }

  //
  // Post Process Histograms
  //
  HistBase::SaveHist(fDir);
}
