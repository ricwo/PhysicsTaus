
// C/C++
#include <cstdlib>
#include <exception>
#include <iostream>
#include <sstream>
#include <sys/stat.h>

// ROOT
#include "TFile.h"

// Local
#include "PhysicsBase/Factory.h"

// Local
#include "RunNexus.h"

REGISTER_ANP_OBJECT(AlgEvent,RunNexus)

using namespace std;

//==============================================================================
Anp::RunNexus::RunNexus():
  fDir(0),
  fVar(Cut::NONE)
{
}

//==============================================================================
Anp::RunNexus::~RunNexus()
{
}

//==============================================================================
void Anp::RunNexus::Config(const Anp::Registry &reg)
{
  //
  // Configure base class - create and configure algorithms
  //
  fRegTop = reg;

  reg.Get(GetType(), "Debug",  fDebug  = false);
  reg.Get(GetType(), "Print",  fPrint  = false);
  reg.Get("RunNexus::DoFile",  fDoFile = false);

  if(fDebug) {
    log() << "Config - begin..." << endl;
  }

  reg.Get("RunNexus::AlgName",     fAlgName);
  reg.Get("RunNexus::DirName",     fDirName);
  reg.Get("RunNexus::VarName",     fVarName);
  reg.Get("RunNexus::FileKey",     fFileKey);

  fVar = Cut::Convert2Var(fVarName);

  if(!reg.Get(fAlgName, fRegAlg)) {
    log() << "MakeAlg - error: missing Registry for: " << fAlgName << endl;
  }

  //
  // Read cuts
  //
  vector<string> chan_cuts;  
  reg.Get("RunNexus::ChannelCuts", chan_cuts);

  if(fDebug) {
    log() << "Config - number of channel cuts: " << chan_cuts.size() << endl;
  }

  for(const string &key: chan_cuts) {
    RunData data(key);

    if(fDebug) {
      log() << "Config - process key: " << key << endl;
    }
    
    if(!reg.Get("RunNexus::" + key + "_runs", data.runs)) {
      log() << "Config - missing runs: " << key << endl;
      continue;
    }

    if(!reg.Get("RunNexus::" + key + "_name", data.name)) {
      log() << "Config - missing name: " << key << endl;
      continue;
    }

    fRuns.push_back(data);

    if(fPrint) {
      log() << "Config - retrieved key: " << key << ", " << data.name << endl;

      for(int run: data.runs) {
	cout << "   run: " << run << endl;
      }
    }
  }

  if(fPrint) {
    log() << "Config - config summary: " << endl
	  << "   AlgName:  " << fAlgName  << endl
	  << "   DirName:  " << fDirName  << endl
	  << "   VarName:  " << fVarName  << endl;
  }

  //
  // Input events
  //
  fReco = NtupleSvc::Instance("reco").Branch<std::vector<RecoEvent> >("events");

  if(fDebug) {
    log() << "Config - finished" << endl;
  }
}

//==============================================================================
void Anp::RunNexus::Signal(const Registry &reg, const string &signal)
{
  //
  // Send signal to children
  //
  for(AlgMap::value_type &alg: fAlgs) {
    for(AlgData &data: alg.second.algs) {
      data.alg->Signal(reg, signal);
    }
  }
}

//==============================================================================
void Anp::RunNexus::Save(TDirectory *dir)
{
  //
  // Set directory
  //
  fDir = dir;
}

//==============================================================================
bool Anp::RunNexus::Init()
{
  return true;
}

//==============================================================================
void Anp::RunNexus::Exec()
{
  //
  // Process event and create a new alg for unique var values
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
  // Select candidates
  //
  double fval = 0.0;
  int    nalg = 0;
  
  for(RecoEvent &reco: fReco.GetRef()) {
    if(reco.GetVar(fVar, fval)) {
      nalg++;
    }
  }

  if(nalg == 0) {
    return;
  }
  else if(nalg > 1) {
    log() << "Exec - multiple matching RecoEvents - do nothing" << endl;
    return;
  }

  //
  // Execute children algs
  //
  RunAlgs(static_cast<int>(fval));

  if(fDebug) {
    log() << "Exec - all is done..." << endl;
  }
}

//==============================================================================
void Anp::RunNexus::Done()
{  
  for(AlgMap::value_type &alg: fAlgs) {
    for(AlgData &data: alg.second.algs) {
      data.alg->Done();
      
      if(data.cut.valid()) {
	data.cut->SaveCutFlow(data.key, data.dir);

	if(fPrint) {
	  log() << "Done - save cut-flow: " << data.key << endl;
	  data.cut->PrintCuts(std::cout);
	}
      }
    }
    
    if(alg.second.file) {
      alg.second.file->Write();
      alg.second.file->Close();
    }
  }
}

//==============================================================================
void Anp::RunNexus::RunAlgs(int ival)
{
  //
  // Process algorithms for this process
  //
  AlgMap::iterator ialg = fAlgs.find(ival);

  if(ialg == fAlgs.end()) {
    //
    // Make algs for new process (DSID)
    //
    AlgList alist;
    alist.file = GetAlgFile(ival);

    TDirectory *dir = alist.file;    
    if(!dir) {
      dir = fDir;
    }

    stringstream sval;
    sval << ival;

    AlgData top_data("");
    MakeAlg(sval.str(), "", top_data, dir);
    
    if(top_data.alg.valid()) {
      alist.algs.push_back(top_data);
    }

    //
    // Special case - allow multiple algs for same ival value
    //
    for(const RunData &data: fRuns) {
      if(!data.MatchRun(ival)) {
	continue;
      }

      AlgData run_data(data.key);

      run_data.cut = BookCut(data.key, fRegTop);
      
      if(!run_data.cut.valid()) {
	log() << "RunAlgs - failed to book cut: " << data.key << endl;
	continue;
      }

      if(fPrint) {
	log() << "RunAlgs - booked cut: " << data.key << endl;
	run_data.cut->PrintConf(std::cout);
      }
      
      MakeAlg(sval.str(), data.name, run_data, dir);
      
      if(run_data.alg.valid()) {
	alist.algs.push_back(run_data);
      }      
    }

    ialg = fAlgs.insert(AlgMap::value_type(ival, alist)).first;
  }

  //
  // Execute algorithms
  //
  for(AlgData &data: ialg->second.algs) {
    bool pass = false;

    if(data.cut.valid()) {
      for(RecoEvent &reco: fReco.GetRef()) {
	if(data.cut->PassCut(reco, reco.GetEventInfo().ref())) {
	  pass = true;
	}
      }
    }
    else {
      pass = true;
    }
    
    if(pass && data.alg.valid()) {

      if(fDebug) {
	log() << "Exec - process sub-algorithms for ival: " << ival << endl;
      }
      
      data.alg->Exec();
    }
  }
}

//==============================================================================
void Anp::RunNexus::MakeAlg(const string &sval, const string &postfix, AlgData &data, TDirectory *dir)
{
  //
  // Make new alg
  //  
  string alg_name = fAlgName + "_" + sval + postfix;
  string alg_type;

  if(!fRegAlg.Get("AlgType", alg_type)) {
    log() << "MakeAlg - missing AlgType" << endl;
    return;
  }

  if(fDebug) {
    log() << "MakeAlg - will try to make new alg: " << alg_name << "/" << alg_type << endl;
  }

  Handle<AlgEvent> alg = Factory<AlgEvent>::Instance().Create(alg_type);

  if(!alg.valid()) {
    log() << "MakeAlg - failed to make: " << alg_type << "/" << alg_name << endl;
    return;
  }
  
  log() << "MakeAlg - created: " << alg_name << "/" << alg_type << endl;

  alg -> SetType(alg_type);
  alg -> SetName(alg_name);
  alg -> Config(fRegAlg);

  string dir_name = sval + postfix;    
  fRegTop.Get("RunNexus::DirName"+sval, dir_name);

  //
  // Save alg and make custom directory in common output file
  //  
  data.alg = alg;  
  data.dir = Anp::GetDir(dir, dir_name);

  alg->Save(data.dir);
  alg->Init(); 

  if(fDebug) {
    log() << "MakeAlg - created and configured new alg: " << alg_name << "/" << alg_type << endl;
  }
}

//==============================================================================
TFile* Anp::RunNexus::GetAlgFile(int ival)
{
  //
  // Open unique file
  //
  if(!fDoFile) {
    return 0;
  }
  
  if(ival < 1) {
    return 0;
  }

  //
  // Check if file already exists
  //
  stringstream fname;
  fname << fFileKey << ival << ".root";
  
  struct stat buffer;   
  if(stat (fname.str().c_str(), &buffer) == 0 && fDebug) {
    log() << "GetAlgFile - output file already exists... will recreate it" << endl;
  }

  //
  // Make unique output file for this var value  
  //
  TFile *file = TFile::Open(fname.str().c_str(), "RECREATE");
  
  if(!file || !file->IsOpen()) {
    log() << "MakeAlg - will abort - failed to open file: " << fname.str() << endl;
    exit(1);
  }

  log() << "GetAlgFile - opened new file: " << fname.str() << endl;
  return file;
}
