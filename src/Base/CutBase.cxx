// C/C++
#include <iomanip>
#include <iostream>
#include <sstream>

// Boost
#include <boost/foreach.hpp>

// Local
#include "PhysicsTaus/CutBase.h"

using namespace std;

//-----------------------------------------------------------------------------
Anp::Ptr<Anp::CutFlow> Anp::CutBase::BookCut(const std::string &key, const Registry &reg)
{
  //
  // Create, configure and save CutFlow object
  //
  CutData data;

  data.key = key;
  data.cut = ObjectFactory<CutFlow>::Instance().CreateObject();
  data.cut->ConfCut(key, reg);

  fCuts.push_back(data);

  return data.cut;
}

//-----------------------------------------------------------------------------
void Anp::CutBase::PrintCutConfig(std::ostream &os) const
{
  //
  // Print configuration
  //
  BOOST_FOREACH(const CutData &cut, fCuts) {
    os << "Print cut configuration: " << cut.key << endl;
    cut.cut->PrintConf(os);
  }
}

//-----------------------------------------------------------------------------
void Anp::CutBase::PrintCutFlows(std::ostream &os) const
{
  //
  // Print configuration
  //
  BOOST_FOREACH(const CutData &cut, fCuts) {
    os << "Print cut-flow: " << cut.key << endl;
    cut.cut->PrintCuts(os);
  }
}

//-----------------------------------------------------------------------------
void Anp::CutBase::SaveCutFlows(TDirectory *dir)
{
  //
  // Print cut-flows
  //
  BOOST_FOREACH(const CutData &cut, fCuts) {
    cut.cut->SaveCutFlow(cut.key, dir);
  }
}

//-----------------------------------------------------------------------------
void Anp::CutBase::SaveCutsHists(TDirectory *dir)
{
  //
  // Save cut-flows and hists
  //
  BOOST_FOREACH(CutData &cut, fCuts) {
    cut.cut->SaveCutFlow(cut.key, dir);
  }

  HistBase::SaveHist(dir);
}

//-----------------------------------------------------------------------------
void Anp::CutBase::UseRawCounts(bool flag)
{
  BOOST_FOREACH(CutData &cut, fCuts) {
    cut.cut->UseRawCounts(flag);
  }
}
