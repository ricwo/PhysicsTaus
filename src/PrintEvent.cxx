
// Base
#include "PhysicsBase/Factory.h"

// Data
#include "PhysicsTaus/Utils.h"

// Local
#include "PrintEvent.h"

REGISTER_ANP_OBJECT(AlgEvent,PrintEvent)

using namespace std;

//======================================================================================================
namespace Anp
{
  std::string PrintMyTruthPart(const TruthPart &tp)
  {
    stringstream str;
    
    str << "(PDG, barcode, status, type, pt [GeV]) = (";
    
    const int pdg = GetInt(Cut::PDG,         tp);
    const int bar = GetInt(Cut::TrueBarcode, tp);
    const int sts = GetInt(Cut::Status,      tp);
    const int ltp = GetInt(Cut::ListType,    tp, -1);
    
    if(pdg >= 0) {
      str << " " << setw(3) << pdg << ", ";
    }
    else {
      str << "-" << setw(3) << std::abs(pdg) << ", ";
    }
    
    const string spt = Anp::Round2Pair(0.001*tp.GetFourMom().GetPt(), 1.0, 5).first;

    str << setw(7) << bar << ", "
	<< setw(2) << sts << ", ";
    
    if(ltp >= 0) {
      str << " " << setw(1) << ltp << ", ";
    }
    else {
      str << "-" << setw(1) << ltp << ", ";
    }
    
    str << spt << ")";

    return str.str();
  }
//======================================================================================================
  std::vector<Anp::Ptr<Anp::TruthPart> > FindMyRangeTrueBarcode(Cut::Var key, 
								const TruthPart &tp, 
								const std::vector<Ptr<TruthPart> > &tvec)
  {
    std::vector<Ptr<TruthPart> > ovec;
    vector<int> ivec;
    
    if(!tp.GetVarVec(key, ivec)) {
      cout << "FindRangeTrueBarcode - missing index vector" << endl;
      return ovec;
    }
    
    for(const Ptr<TruthPart> &ptr: tvec) {
      double value = 0.0;
      int    index = 0;
    
    if(ptr->GetVar(Cut::TrueBarcode, value)) {
      index = static_cast<int>(value);    
      
      if(std::find(ivec.begin(), ivec.end(), index) != ivec.end()) {
	ovec.push_back(ptr);
      }
    }
    else {
      cout << "FindRangeTrueBarcode - number of index value(s) = " << ivec.size() << ": ";      
    }
    }
    
    return ovec;
  }    
}

//==============================================================================
template<class T> inline std::string Anp::PrintEvent::PrintObjects(const std::vector<Ptr<T> > &ptrs, 
								   const std::string &key, 
								   const std::string &pad)
{
  std::stringstream str;
 
  str << pad << "Print variables of " << key << " for " << ptrs.size() << " object(s):" << endl; 
   
  for(const Ptr<T> &ptr: ptrs) {
    str << ptr->GetVarsAsStr(pad + "  ") << endl;
  }

  return str.str();
}

//==============================================================================
std::string Anp::PrintEvent::PrintTruths(const std::vector<Ptr<TruthPart> > &ptrs, 
					 const std::string &key, 
					 const std::string &pad)
{
  std::stringstream str;
 
  str << pad << "Print variables of " << key << " for " << ptrs.size() << " TruthPart object(s):" << endl; 
   
  for(const Ptr<TruthPart> &ptr: ptrs) {
    str << PrintMyTruthPart(ptr.ref()) << endl;

    vector<Ptr<TruthPart> > pvec = FindMyRangeTrueBarcode(Cut::IndexRangeParent, ptr.ref(), ptrs);
    vector<Ptr<TruthPart> > cvec = FindMyRangeTrueBarcode(Cut::IndexRangeChild,  ptr.ref(), ptrs);

    for(const Ptr<TruthPart> &p: pvec) {
      str << "   parent: " << PrintMyTruthPart(p.ref()) << endl;
    }    

    for(const Ptr<TruthPart> &p: cvec) {
      str << "   child: " << PrintMyTruthPart(p.ref()) << endl;
    }  
  }

  return str.str();
}

//======================================================================================================
Anp::PrintEvent::PrintEvent()
{
}

//======================================================================================================
void Anp::PrintEvent::PlotConf(const Registry &reg)
{
  //
  // Read configuration
  //
  reg.Get("PrintEvent::PrintCandEvent", fPrintCandEvent = false);
  reg.Get("PrintEvent::PrintRecoEvent", fPrintRecoEvent = false);

}

//======================================================================================================
bool Anp::PrintEvent::PlotInit()
{
  return true;
}

//======================================================================================================
void Anp::PrintEvent::PlotDone()
{
  SaveCutsHists(GetDir());
}

//======================================================================================================
void Anp::PrintEvent::PlotExec()
{
  /*
    Process raw ntuple event
   */
  
  if(fReco.IsValidEvent()) {
    for(RecoEvent &reco: fReco.GetRef()) {

      if(fPrintRecoEvent) {
	cout << "========================================================================================" << endl;
	PrintRecoEvent(reco);
      }

      if(fPrintCandEvent) {
	cout << "========================================================================================" << endl;
	for(const Ptr<CandEvent> &cand: reco.GetVec<CandEvent>()) {
	  PrintCandEvent(cand.ref());
	}
      }
    }
  }
}

//======================================================================================================
void Anp::PrintEvent::PrintRecoEvent(const RecoEvent &reco)
{
  /*
    Print raw reconstructed event
   */

  log() << "PrintRecoEvent - print RecoEvent variables:" << endl
	<< reco.GetVarsAsStr("  ") << endl
	<< "   number of elecs: " << reco.GetVec<RecElec>().size() << endl
	<< "   number of muons: " << reco.GetVec<RecMuon>().size() << endl
	<< "   number of jets:  " << reco.GetVec<RecJet >().size() << endl
	<< "   number of taus:  " << reco.GetVec<RecTau >().size() << endl
	<< endl
	<< PrintObjects(reco.GetVec<RecElec>(), "RecElec", "  ") << endl
	<< PrintObjects(reco.GetVec<RecMuon>(), "RecMuon", "  ") << endl
	<< PrintObjects(reco.GetVec<RecJet >(), "RecJet",  "  ") << endl
	<< PrintObjects(reco.GetVec<RecTau >(), "RecTau",  "  ") << endl
	<< endl
	<< "---------------------------------------------------" << endl
	<< "Print Truth variables" << endl
	<< PrintTruths(reco.GetVec<TruthPart>(), "TruthPart", " ") << endl;
}

//======================================================================================================
void Anp::PrintEvent::PrintCandEvent(const CandEvent &cand)
{
  /*
    Print selected candidate event
   */

  log() << "PrintCandEvent - print CandEvent variables:" << endl
	<< cand.GetVarsAsStr("  ") << endl
	<< "   number of elecs: " << cand.GetVec<RecElec>().size() << endl
	<< "   number of muons: " << cand.GetVec<RecMuon>().size() << endl
	<< "   number of jets:  " << cand.GetVec<RecJet >().size() << endl
	<< "   number of taus:  " << cand.GetVec<RecTau >().size() << endl
	<< endl
	<< PrintObjects(cand.GetVec<RecElec>(), "CandElec", "  ") << endl
	<< PrintObjects(cand.GetVec<RecMuon>(), "CandMuon", "  ") << endl
	<< PrintObjects(cand.GetVec<RecJet >(), "CandJet",  "  ") << endl
	<< PrintObjects(cand.GetVec<RecTau >(), "CandTau",  "  ") << endl
	<< endl
	<< "----------------------------------------------------" << endl
	<< "Print Truth variables" << endl
	<< PrintTruths(cand.GetVec<TruthPart>(), "TruthPart", " ") << endl;
}
