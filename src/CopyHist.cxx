// C/C++
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cstdlib>

// ROOT
#include "TFile.h"
#include "TH1.h"

// Boost
#include <boost/foreach.hpp>

// Local
#include "CopyHist.h"

using namespace std;

//======================================================================================================
Anp::CopyHist::CopyHist():
  fResetFile (false),
  fCurrMCChan(0)
{
}

//======================================================================================================
Anp::CopyHist::~CopyHist()
{
}

//======================================================================================================
void Anp::CopyHist::Config(const Registry &reg, const string &key)
{
  //
  // Read configuration
  //
  reg.Get(key, "AlgName",     fName);
  reg.Get(key, "AlgType",     fType);
  reg.Get(key, "Debug",       fDebug         = true);
  reg.Get(key, "Print",       fPrint         = false);
  reg.Get(key + "::CopyName", fCopyNames);

  if(fPrint) {
    log() << "Config - print configuration: " << endl;
  }
}

//======================================================================================================
void Anp::CopyHist::Signal(const Anp::Registry &reg, const std::string &key)
{
  //
  // Callback from top level module
  //
  if(key != "NEW_FILE") {
    return;
  }

  fCurrMCChan = 0;
  fResetFile  = true;

  fInputFilePath.clear();
  
  if(!reg.Get("SIGNAL::FilePath", fInputFilePath)) {
    log() << "Signal - missing FilePath key" << endl;
    return;
  }
}

//======================================================================================================
void Anp::CopyHist::CopyObjects(int mc_chan, double)
{
  //
  // Check that input file is MC
  //
  if(mc_chan < 100000) {
    return;
  }

  //
  // Process each file once
  //
  if(fResetFile) {
    fResetFile  = false;
    fCurrMCChan = mc_chan;
  }
  else if(fCurrMCChan == mc_chan) {
    return;
  }
  else {
    log() << "CopyObjects - logic error: input file contains multiple values of MC channel variable" << endl;
    exit(1);
  }

  //
  // Copy hists from input file to output directory
  //
  TFile *ifile = TFile::Open(fInputFilePath.c_str(), "READ");

  if(!ifile || !ifile->IsOpen()) {
    log() << "CopyObjects - failed to open file:\n" << "   " << fInputFilePath << endl;
    
    if(ifile) {
      delete ifile;
    }
    return;
  }

  log() << "CopyObjects - open: " << fInputFilePath << endl;

  BOOST_FOREACH(const string &cname, fCopyNames) {
    TH1 *h1 = dynamic_cast<TH1 *>(ifile->Get(cname.c_str()));
    
    if(!h1) {
      if(fPrint) {
	log() << "CopyObjects - ignore non-TH1 object: " << mc_chan << " - " << cname << endl;
      }
      continue;
    }
    
    CopyMap::iterator cit = fCopyMap.find(mc_chan);
    if(cit == fCopyMap.end()) {
      cit = fCopyMap.insert(CopyMap::value_type(mc_chan, CopyData())).first;
    }

    HistMap &hists = cit->second.hists;
    
    HistMap::iterator hit = hists.find(cname);
    
    if(hit == hists.end()) {
      TH1 *h2 = dynamic_cast<TH1 *>(h1->Clone());
      h2->SetDirectory(0);
      
      if(h2) {
	hit = hists.insert(HistMap::value_type(cname, h2)).first;

	log() << "CopyObjects - MC channel=" << mc_chan << " - new TH1: " << cname << endl;
      }
      else {
	log() << "CopyObjects - TH1 clone failed: " << mc_chan << " - " << cname << endl;
      }
    }
    else {
      hit->second->Add(h1);

      log() << "CopyObjects - add TH1: " << mc_chan << " - " << cname << endl;
    }
  }

  ifile->Close();
}

//======================================================================================================
void Anp::CopyHist::SaveObjects(TDirectory *topdir)
{
  //
  // Save collected objects from input files to output files
  //
  log() << "SaveObjects - copy map size=" << fCopyMap.size() << endl;

  BOOST_FOREACH(CopyMap::value_type &copy, fCopyMap) {

    stringstream dirname;
    dirname << copy.first;

    TDirectory *dir = Anp::GetDir(topdir, dirname.str());

    if(!dir) {
      log() << "SaveObjects - invalid output TDirectory: " << dirname.str() << endl;
      return;
    }
    
    BOOST_FOREACH(HistMap::value_type &hist, copy.second.hists) {
      TH1 *h = hist.second;
      h->SetDirectory(dir);
      
      log() << "CopyObjects - save TH1: " << dirname.str() << "/" << h->GetName() << endl;
    }  
  }

  log() << "SaveObjects - all is done" << endl;
}

//======================================================================================================
// Simple implementation of log function via std::cout
//
std::ostream& Anp::CopyHist::log() const
{
  std::cout << GetName() << "::";
  return std::cout;
}
