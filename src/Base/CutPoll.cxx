// C/C++
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

// Base
#include "PhysicsBase/HistMan.h"
#include "PhysicsBase/UtilCore.h"

// Local
#include "PhysicsTaus/CutPoll.h"

using namespace std;

//==============================================================================
Anp::CutPoll::CutPoll():
  fUseRaw(false),
  fHist  (0)
{
}

//==============================================================================
void Anp::CutPoll::Print(const std::string &key) const
{
  //
  // Print event count
  //
  cout << "-----------------------------------------------------------" << endl;
  cout << "   " << key << endl;

  for(SampleMap::const_iterator it = fSamples.begin(); it != fSamples.end(); ++it) {
    cout << "   " << key << " " << it->first << ": " 
	 << Convert2Str(it->second.sumw, std::sqrt(it->second.sumw2)) << endl;
  }
}

//==============================================================================
std::string Anp::CutPoll::Convert2Str(double val, double err) const
{
  //
  // Convert result to string
  //
  const pair<string, string> res = Anp::Round2Pair(val, err);
  
  //
  // Print nice counts
  //
  stringstream str;
  
  str << std::setw(10) << std::left << res.first
      << " +/- " 
      << std::setw(10) << std::left << res.second;

  return str.str();
}

//==============================================================================
bool Anp::CutPoll::GetCountError(double &val, double &err) const
{
  //
  // Read counts and error for sample, return false if sample is missing
  //
  unsigned sumn  = 0;
  double   sumw  = 0.0;
  double   sumw2 = 0.0;

  for(SampleMap::const_iterator cit = fSamples.begin(); cit != fSamples.end(); ++cit) {      
    sumn  += cit->second.sumn;
    sumw  += cit->second.sumw;
    sumw2 += cit->second.sumw2;
  }

  if(sumn == 0) {
    return false;
  }

  if(fUseRaw) {
    val = sumn;
    err = std::sqrt(double(sumn));
  }
  else {
    val = sumw;
    err = std::sqrt(sumw2);   
  }

  return true;
}

//==============================================================================
bool Anp::CutPoll::GetCountError(unsigned sample, double &val, double &err) const
{
  //
  // Read counts and error for sample, return false if sample is missing
  //
  SampleSet samples;
  samples.insert(sample);

  return GetCountError(samples, val, err);
}

//==============================================================================
bool Anp::CutPoll::GetCountError(const SampleSet &samples, double &val, double &err) const
{
  //
  // Read counts and error for sample, return false if sample is missing
  //
  unsigned sumn  = 0;
  double   sumw  = 0.0;
  double   sumw2 = 0.0;

  for(SampleSet::const_iterator sit = samples.begin(); sit != samples.end(); ++sit) {  
    
    const SampleMap::const_iterator cit = fSamples.find(*sit);
    
    if(cit != fSamples.end()) {
      sumn  += cit->second.sumn;
      sumw  += cit->second.sumw;
      sumw2 += cit->second.sumw2;
    }
  }

  if(sumn == 0) {
    return false;
  }

  if(fUseRaw) {
    val = sumn;
    err = std::sqrt(double(sumn));
  }
  else {
    val = sumw;
    err = std::sqrt(sumw2);   
  }

  return true;
}

//==============================================================================
std::pair<std::string, std::string> Anp::CutPoll::GetCountErrorAsPair() const
{
  //
  // Return count and error as 
  //
  double val = 0.0, err = 0.0;
  
  //
  // Special case to select data: do not round off data counts
  //

  if(GetCountError(val, err)) {
    pair<string, string> res = Anp::Round2Pair(val, err);

    if(fUseRaw) {
      //
      // HACK: for data keep exact event counts
      //
      stringstream dstr;
      dstr << static_cast<long>(val);
      
      res.first = dstr.str();
      return res;
    }
    else {
      return res;
    }
  }

  return std::pair<std::string, std::string>();
}

//==============================================================================
std::pair<std::string, std::string> Anp::CutPoll::GetCountErrorAsPair(unsigned sample) const
{
  //
  // Return count and error as 
  //
  SampleSet samples;
  samples.insert(sample);

  return GetCountErrorAsPair(samples);
}

//==============================================================================
std::pair<std::string, std::string> Anp::CutPoll::GetCountErrorAsPair(const SampleSet &sample) const
{
  //
  // Return count and error as 
  //
  double val = 0.0, err = 0.0;
  
  //
  // Special case to select data: do not round off data counts
  //

  if(GetCountError(sample, val, err)) {
    pair<string, string> res = Anp::Round2Pair(val, err);

    if(fUseRaw) {
      //
      // HACK: for data keep exact event counts
      //
      stringstream dstr;
      dstr << static_cast<long>(val);
      
      res.first = dstr.str();
      return res;
    }
    else {
      return res;
    }
  }

  return std::pair<std::string, std::string>();
}

//==============================================================================
void Anp::CutPoll::AddPlotVar(const Ptr<VarEntry> &ptr, const std::string &key, const std::string &hist)
{
  if(fHist) {
    cout << "CutPoll::AddPlotVar - histogram already exists: " << key << endl;
    return;
  }

  fHist   = Anp::HistMan::Instance().CreateTH1(hist, key);
  fVarPtr = ptr;

  /*
  if(fHist && fVarPtr.valid()) {
    cout << "CutPoll::AddPlotVar - add histogram: " << key << endl;
  }
  else {
    cout << "CutPoll::AddPlotVar - failed to add histogram: " << key << endl;
  }
  */
}
