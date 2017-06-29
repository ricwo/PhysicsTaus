// C/C++
#include <set>
#include <iostream>
#include <fstream>

// ROOT
#include "TH1.h"

// Local
#include "PhysicsTaus/CutFlow.h"

using namespace std;

//==============================================================================
// CutFlow manager code
//==============================================================================
Anp::CutFlowMan& Anp::CutFlowMan::Instance()
{
  static Anp::CutFlowMan instance;
  return instance;
}

//==============================================================================
void Anp::CutFlowMan::WriteCutFlows(const std::string &path) const
{
  WriteFlows(path, fCutFlows);
}

//==============================================================================
void Anp::CutFlowMan::WriteRawFlows(const std::string &path) const
{
  WriteFlows(path, fRawFlows);
}

//==============================================================================
void Anp::CutFlowMan::WriteFlows(const std::string &path, 
				 const std::map<std::string, std::string> &flows) const
{
  //
  // Write all cut-flows into single text file (to avoid disk IO problems at Penn)
  //
  stringstream str;

  //
  // Write Header of document
  //
  str << "\\documentclass[letterpaper,12pt]{article}\n"
      << "\\usepackage{amsmath}    % need for subequations \n"
      << "\\usepackage{graphicx} \n"
      << "\n"
      << "\\usepackage[hmargin=0.25cm]{geometry}\n"
      << "\n"
      << "\\begin{document}\n"
      << "{\\tiny \n";

  for(std::map<std::string, std::string>::const_iterator cit = flows.begin(); cit != flows.end(); ++cit) {
    //
    // Find and replace the underscores
    //
    const string cutflow_name = ReplaceUnderscore(cit->first);

    str << "%------------------------------------------------------------------------------------------\n"
	<< "% Algorithm: " << cit->first << "\n "
	<< cutflow_name
	<< "\n "
	<< cit->second
	<< " $\\newline$"
	<< "\n";
  }

  //
  // End document
  //
  str << "\n } \n" 
      << "\\end{document}";

  std::ofstream fos(path.c_str());

  if(fos.is_open()) {
    fos << str.str();
    fos.close();
  }
}

//==============================================================================
std::string Anp::CutFlowMan::ReplaceUnderscore(std::string str) const
{
  //
  // Replace "_" with " "
  //
  std::string::iterator sit = str.begin();
  
  while(sit != str.end()) {
    if(*sit == '_') {
      *sit = ' ';
    }
    
    ++sit;
  }

  return str;
}

//-----------------------------------------------------------------------------
Anp::CutFlow::CutFlow():
  fDebug(false)
{
}

//-----------------------------------------------------------------------------
Anp::CutFlow::~CutFlow()
{
}

//-----------------------------------------------------------------------------
void Anp::CutFlow::ConfCut(const std::string &key, const Registry &reg)
{
  //
  // Configure cuts
  //
  vector<string> cuts;
  reg.Get(key, cuts);

  for(unsigned i = 0; i < cuts.size(); ++i) {
    AddCut(key+cuts.at(i), reg);
  }

  if(!fInput.valid()) {
    fInput = ObjectFactory<CutPoll>::Instance().CreateObject();
  }
  else {
    log() << "ConfCut - fInput pointer is already valid" << endl;
  }
}

//-----------------------------------------------------------------------------
void Anp::CutFlow::AddVarPtr(const Ptr<VarEntry> &ptr)
{
  if(ptr.valid()) {
    for(Ptr<CutPair> &p: fCuts) {
      p->icut.AddVarPtr(ptr);
    }
    for(Ptr<CutFlow> &p: fSubs) {
      p->AddVarPtr(ptr);
    }
  }
}

//-----------------------------------------------------------------------------
void Anp::CutFlow::AddPlotVar(const Ptr<VarEntry> &ptr, const string &key, const string &hist)
{
  if(!ptr.valid()) {
    return;
  }
  
  if(fSubs.empty()) {
    if(fDebug) {
      log() << "AddPlotVar - key=" << key << ", hist=" << hist << endl;
    }

    for(Ptr<CutPair> &p: fCuts) {
      p->poll.AddPlotVar(ptr, key, hist);
    }
  } 
  else {
    for(Ptr<CutFlow> &p: fSubs) {
      p->AddPlotVar(ptr, key, hist);
    }
  }
}

//------------------------------------------------------------------------------
std::ostream& Anp::CutFlow::log() const
{
  std::cout << "CutFlow::";
  return std::cout;
}

//-----------------------------------------------------------------------------
void Anp::CutFlow::AddCut(const std::string &key, const Registry &reg)
{
  //
  // Add and configure new cut
  //
  Registry creg;
  if(!reg.Get(key, creg)) {
    log() << "AddCut - missing registry for key: " << key << endl;
    reg.Print();
    return;
  }
  
  //
  // Add and configure new cut
  //
  Ptr<CutPair> cp = ObjectFactory<CutPair>::Instance().CreateObject();
  fCuts.push_back(cp);
  cp->icut.InitCut(creg);

  //
  // Book sub-cuts - used for cut optimization
  //
  vector<string> subs;
  creg.Get("SubCuts", subs);

  for(unsigned i = 0; i < subs.size(); ++i) {
    const string &sub = "SubCut_"+subs.at(i);

    if(fDebug) {
      log() << "AddCut - config subcut: " << sub << endl;
    }

    if(creg.KeyExists(sub)) {
      //
      // Book subcut and save with GLOBAL list of this CutFlow object
      //
      Ptr<CutFlow> cut = ObjectFactory<CutFlow>::Instance().CreateObject();
      cut->ConfCut(sub, creg);
      fSubs.push_back(cut);

      if(fDebug) {
	log() << "AddCut - booked subcut: " << sub << endl;
      }
    }
  }
}

//-----------------------------------------------------------------------------
void Anp::CutFlow::PrintConf(std::ostream &os, const std::string &pad) const
{
  //
  // Print configuration
  //
  os << pad << "   Number of cuts: " << fCuts.size() << endl;
  for(unsigned i = 0; i < fCuts.size(); ++i) { 
    const CutPair &cp = fCuts.at(i).ref();

    cp.icut.PrintConfig(os, "   "+pad);   
  }

  if(!fSubs.empty()) {
    os << pad << "   Number of sub-cuts: " << fSubs.size() << endl;
    
    for(unsigned j = 0; j < fSubs.size(); ++j) { 
      fSubs.at(j)->PrintConf(os, pad+"   ");
    }
  }
}

//==============================================================================
void Anp::CutFlow::SaveCutFlow(const std::string &key, TDirectory *dir) const
{
  //
  // Save histogram with cut-flows
  //
  if(!dir) {
    return;
  }

  const string nameC = key;

  TH1D *hc = new TH1D(nameC.c_str(), nameC.c_str(), fCuts.size()+1, 0, fCuts.size()+1);
  hc->SetDirectory(dir);
  hc->SetStats(false);   
  hc->GetXaxis()->SetBinLabel(1, "Input");

  double ival = 0.0;
  double ierr = 0.0;

  if(fInput.valid() && fInput->GetCountError(ival, ierr)) {
    hc->SetBinContent(1, ival);
    hc->SetBinError  (1, ierr);
  }

  for(unsigned i = 0; i < fCuts.size(); ++i) {
    const CutItem &icut = fCuts.at(i)->icut;
    const CutPoll &poll = fCuts.at(i)->poll;

    const uint bin = i+2;
    double val = 0.0;
    double err = 0.0;
    
    if(poll.GetCountError(val, err)) {
      hc->SetBinContent(bin, val);
      hc->SetBinError  (bin, err);
    }

    hc->GetXaxis()->SetBinLabel(bin, icut.GetName().c_str());
  }
  
  if(fSubs.empty()) {
    if(!fCuts.empty()) {
      SaveVarHist(fCuts.back()->poll, nameC, dir);
    }
  }
  else {
    for(unsigned j = 0; j < fSubs.size(); ++j) { 
      fSubs.at(j)->SaveSubFlow(fInput, "", Anp::GetDir("subcuts_"+key, dir), fCuts);
    }
  }
}

//==============================================================================
void Anp::CutFlow::SaveSubFlow(const Ptr<CutPoll> &input, 
			       const string &key, TDirectory *dir, const CutVec &cuts) const
{
  //
  // Save histogram with cut-flows
  //
  if(!dir) {
    return;
  }

  Ptr<CutPoll> iptr = input;
  if(!iptr.valid()) {
    iptr = fInput;
  }

  if(!fSubs.empty()) {

    CutVec acuts = cuts;
    acuts.insert(acuts.end(), fCuts.begin(), fCuts.end());
      
    //
    // Recursive call for sub-cuts
    //
    for(SubVec::const_iterator sit = fSubs.begin(); sit != fSubs.end(); ++sit) {
      (*sit)->SaveSubFlow(iptr, key, dir, acuts);
    }

    return;
  }

  CutVec acuts = cuts;
  acuts.insert(acuts.end(), fCuts.begin(), fCuts.end());

  //
  // Reach terminal node - save histogram
  //
  string nameC = key;

  for(const Ptr<CutPair> &cut: acuts) {
    if(nameC.empty()) {
      nameC = cut->icut.GetName();
    }
    else {
      nameC += "_" + cut->icut.GetName();
    }
  }

  TH1D *hc = new TH1D(nameC.c_str(), nameC.c_str(), acuts.size()+1, 0, acuts.size()+1);
  hc->SetDirectory(dir);
  hc->SetStats(false);
  hc->GetXaxis()->SetBinLabel(1, "Input");

  double ival = 0.0;
  double ierr = 0.0;

  if(iptr.valid() && iptr->GetCountError(ival, ierr)) {
    hc->SetBinContent(1, ival);
    hc->SetBinError  (1, ierr);
  }

  for(unsigned i = 0; i < acuts.size(); ++i) {
    const CutItem &icut = acuts.at(i)->icut;
    const CutPoll &poll = acuts.at(i)->poll;

    const uint bin = i+2;
    double val = 0.0;
    double err = 0.0;
    
    if(poll.GetCountError(val, err)) {
      hc->SetBinContent(bin, val);
      hc->SetBinError  (bin, err);
    }

    hc->GetXaxis()->SetBinLabel(bin, icut.GetName().c_str());

    //
    // Save histogram from the last cut
    //
    if(i+1 == acuts.size()) {
      SaveVarHist(poll, nameC, dir);
    }
  }
}

//-----------------------------------------------------------------------------
void Anp::CutFlow::PrintCuts(std::ostream &os, const std::string &pad) const
{
  //
  // Print cutflow table for sets of samples, first compute name width
  //
  unsigned namew = string("Input").size();

  for(unsigned i = 0; i < fCuts.size(); ++i) {
    const CutPair &c = fCuts.at(i).ref();

    //
    // Find maximum width for cut name
    //
    namew = std::max<unsigned>(namew, c.icut.GetName().size());
  }

  //
  // Print input events
  //
  pair<string, string> ires;
  if(fInput.valid()) {
    ires = fInput->GetCountErrorAsPair();
  }
  //get value width
  unsigned int valuew = ires.first.size();
  
  os << pad << setw(namew) << left << "Input"
     << " " << ires.first << " +/- " << ires.second << "\n";

  //
  // Print table
  //
  for(unsigned i = 0; i < fCuts.size(); ++i) {
    const CutPair &c = fCuts.at(i).ref();

    const pair<string, string> cres = c.poll.GetCountErrorAsPair();

    os << pad << setw(namew)  << left  << c.icut.GetName()
       << " " << setw(valuew) << right << cres.first << " +/- " << cres.second << "\n";
  }
  
  if(!fSubs.empty()) {
    for(unsigned j = 0; j < fSubs.size(); ++j) { 
      fSubs.at(j)->PrintCuts(os, pad+"   ");
    }
  }
}

//-----------------------------------------------------------------------------
void Anp::CutFlow::UseRawCounts(bool flag)
{
  //
  // Print cutflow table for sets of samples
  //
  if(fInput.valid()) {
    fInput->UseRaw(flag);  }

  for(unsigned i = 0; i < fCuts.size(); ++i) {
    fCuts.at(i)->poll.UseRaw(flag);
  }
}

//-----------------------------------------------------------------------------
void Anp::CutFlow::SaveVarHist(const CutPoll &poll, const std::string &prefix, TDirectory *dir) const
{
  //
  // Save sub-cut histograms
  //
  TH1 *h = poll.GetHist();
  
  if(!h) {
    if(fDebug) {
      log() << "SaveSubFlow - null var histogram" << endl;
    }

    return;
  }

  h = dynamic_cast<TH1 *>(h->Clone());

  const string hname = prefix + "_" + std::string(h->GetName());
  h->SetName(hname.c_str());
  h->SetDirectory(dir);
  
  if(fDebug) {
    log() << "SaveSubFlow - valid histogram: " << hname << endl; 
  }
}
