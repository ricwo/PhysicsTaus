// This is -*- c++ -*-
#ifndef ANP_LIGHTUTILS_H
#define ANP_LIGHTUTILS_H

// C/C++
#include <vector>
#include <utility>

// ROOT
#include "TButton.h"
#include "TDialogCanvas.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TTree.h"

// Base
#include "PhysicsBase/AlgEvent.h"
#include "PhysicsBase/Ptr.h"

// Data
#include "PhysicsData/EventInfo.h"
#include "PhysicsData/TruthPart.h"

// Local
#include "PhysicsTaus/CutFlow.h"

class RooAbsReal;

void ANP_button_called_function();

namespace Anp
{  
  class TruthVtx;

  TDirectory* GetRecursiveDir(TDirectory *dir, std::string path);

  double GetDeltaPhi(double phi1, double phi2);

  double GetDeltaPhi(double x1, double y1, double x2, double y2);

  double GetDeltaR(double eta1, double phi1, double eta2, double phi2);

  TH1 * GetTH1(const std::string &key, const std::string & dirbase, TDirectory * dir);

  std::vector<Ptr<TruthPart> > FindMothers(const Ptr<TruthPart> &, const std::vector<Ptr<TruthPart> >&);

  void FindMothersRecursive(const Ptr<TruthPart> &t,
			    const std::vector<Ptr<TruthPart> > &truths,
			    std::vector<Ptr<TruthPart> > &mothers);

  std::set<int> ReadIntSet(const Registry &reg, const std::string key);

  void PrintRooAbsReal(const RooAbsReal &pdf, int contents);

  std::vector<Registry> GetRegistryVec(const std::string &key, const Registry &reg);

  std::vector<std::pair<double, double> > DivideByEqualBins(std::vector<double> &vars, int nbin, double vmin=1.0, double vmax=0.0);

  std::string PrintPart(const TruthPart &tp);

  std::string PrintVars(const VarHolder &vh);

  double GetDPt(const FourMom &tp, const FourMom &lv);

  std::vector<Anp::Ptr<Anp::TruthPart> > FindRangeTrueBarcode(Cut::Var key, const TruthPart &tp, 
							      const std::vector<Ptr<TruthPart> > &tvec); 

  int FindRecursiveRangeTrueBarcode(Cut::Var key, 
				    std::vector<Anp::Ptr<Anp::TruthPart> > &result,
				    const TruthPart &tp, 
				    const std::vector<Ptr<TruthPart> > &tvec,
				    bool debug=false); 

  bool IsGenerator(const TruthPart &tp);

  Anp::Ptr<Anp::TruthVtx> FindDecayVtx(const TruthPart &p, const std::vector<Ptr<TruthVtx> > &vtxs);
  Anp::Ptr<Anp::TruthVtx> FindProdVtx (const TruthPart &p, const std::vector<Ptr<TruthVtx> > &vtxs);

  bool IsSameChargeSign(const VarHolder &l, const VarHolder &r);

  Ptr<TruthPart> FindTruthPart(Cut::Var var, const VarHolder &rec_obj, const std::vector<Ptr<TruthPart> > &truths);

  int WaitForButton(const std::string &canvas);

  std::string RemoveSpaces(const std::string &input);

  //-----------------------------------------------------------------------------
  template<class T> bool FillLV(const T &tp, TLorentzVector &lv) {

    double eta = 0.0;
    double phi = 0.0;
    double pt  = 0.0;
    double m   = 0.0;

    if(tp.GetVar(Cut::Eta,  eta) &&
       tp.GetVar(Cut::Phi,  phi) &&
       tp.GetVar(Cut::Pt,    pt) &&
       tp.GetVar(Cut::Mass,   m)) {

      lv.SetPtEtaPhiM(pt, eta, phi, m);
      return true;
    }

    return false;
  }

  //-----------------------------------------------------------------------------
  struct SortObjectByPt
  {
    template<class T> bool operator()(const Ptr<T> &lhs, const Ptr<T> &rhs) 
    { 
      return lhs->GetFourMom().GetPt() > rhs->GetFourMom().GetPt();
    }
  };
  struct SortObjectByEnergy
  {
    template<class T> bool operator()(const Ptr<T> &lhs, const Ptr<T> &rhs) 
    { 
      return lhs->GetFourMom().GetE() > rhs->GetFourMom().GetE();
    }
  };
  struct SortObjectByMass
  {
    template<class T> bool operator()(const Ptr<T> &lhs, const Ptr<T> &rhs) 
    { 
      return lhs->GetFourMom().GetM() > rhs->GetFourMom().GetM();
    }
  };
  struct SortObjectByDR
  {
  public:
    SortObjectByDR(const FourMom &p_):p(p_) {}
    
    template<class T> bool operator()(const Ptr<T> &lhs, const Ptr<T> &rhs) 
    { 
      return lhs->GetFourMom().GetDR(p) < rhs->GetFourMom().GetDR(p);
    }

  private:

    SortObjectByDR();
    FourMom p;
  };

  //-----------------------------------------------------------------------------
  template<class T> std::vector<Ptr<T> > FindAndSort(const std::vector<Ptr<T> > &parts, CutFlow &cut, const EventInfo &info) {
    std::vector<Ptr<T> > results;

    for(unsigned i = 0; i < parts.size(); ++i) {
      const Ptr<T> &p = parts.at(i);
      
      if(cut.PassCut(p.ref(), info.GetMCChannel(), info.GetWeight())) {
	results.push_back(p);
      }
    }

    std::sort(results.begin(), results.end(), SortObjectByPt());

    return results;
  }

  //-----------------------------------------------------------------------------
  template<class T, class Y> Ptr<T> GetMinDRPtr(const Y &obj, const std::vector<Ptr<T> > &vec)
  {
    double min_dr = 0.0;
    Ptr<T> res;

    for(unsigned i = 0; i < vec.size(); ++i) {
      const Ptr<T> &p = vec.at(i);

      if(p->GetBarcode() == obj.GetBarcode()) {
	continue;
      }

      const double dr = obj.GetFourMom().GetDR(p->GetFourMom());
      
      if(res.valid()) {
	if(dr < min_dr) {
	  min_dr = dr;
	  res    = p;
	}
      }
      else {
	min_dr = dr;
	res    = p;
      }
    }

    return res;
  }

  //-----------------------------------------------------------------------------
  template<class T, class Y> Ptr<T> GetMinDRPtr(const Y &obj, const std::vector<Ptr<T> > &vec, const std::set<uint64_t> &vetoes)
  {
    double min_dr = 0.0;
    Ptr<T> res;

    for(unsigned i = 0; i < vec.size(); ++i) {
      const Ptr<T> &p = vec.at(i);

      if(p->GetBarcode() == obj.GetBarcode()) {
	continue;
      }
      if(vetoes.count(p->GetBarcode()) > 0) {
	continue;
      }

      const double dr = obj.GetFourMom().GetDR(p->GetFourMom());
      
      if(res.valid()) {
	if(dr < min_dr) {
	  min_dr = dr;
	  res    = p;
	}
      }
      else {
	min_dr = dr;
	res    = p;
      }
    }

    return res;
  }

  //-----------------------------------------------------------------------------
  template<class T, class Y> std::pair<bool, double> GetMinDR(const Ptr<Y> &ptr, 
							      const std::vector<Ptr<T> > &vec,
							      double default_dr=0.0) 
  {
    std::pair<bool, double> res(false, default_dr);

    for(unsigned i = 0; i < vec.size(); ++i) {
      const Ptr<T> &p = vec.at(i);

      if(p->GetBarcode() == ptr->GetBarcode()) {
	continue;
      }

      const double dr = ptr->GetFourMom().GetDR(p->GetFourMom());
      
      if(res.first) {
	res.second = std::min<double>(res.second, dr);
      }
      else {
	res.first  = true;
	res.second = dr;	
      }
    }

    return res;
  }

  //-----------------------------------------------------------------------------
  template<class T, class Y> std::pair<bool, double> GetMinVecDR(const std::vector<Ptr<T> > &vec1, 
								 const std::vector<Ptr<Y> > &vec2,
								 double default_dr) 
  {
    std::pair<bool, double> res(false, default_dr);

    for(unsigned i = 0; i < vec1.size(); ++i) {
      const Ptr<T> &p1 = vec1.at(i);

      for(unsigned j = 0; j < vec2.size(); ++j) {
	const Ptr<Y> &p2 = vec2.at(j);
	
	if(p1->GetBarcode() == p2->GetBarcode()) {
	  continue;
	}

	const double dr = p1->GetFourMom().GetDR(p2->GetFourMom());
      
	if(res.first) {
	  res.second = std::min<double>(res.second, dr);
	}
	else {
	  res.first  = true;
	  res.second = dr;	
	}
      }
    }

    return res;
  }

  //-----------------------------------------------------------------------------
  template<class T, class Y> std::pair<Ptr<T>, Ptr<Y> > GetMinVecDRPair(const std::vector<Ptr<T> > &vec1, 
									const std::vector<Ptr<Y> > &vec2)
  {
    std::pair<Ptr<T>, Ptr<Y> > match;

    match.first  = Ptr<T>();
    match.second = Ptr<Y>();

    double min_dr = 0.0;
    
    for(unsigned i = 0; i < vec1.size(); ++i) {
      const Ptr<T> &p1 = vec1.at(i);

      for(unsigned j = 0; j < vec2.size(); ++j) {
	const Ptr<Y> &p2 = vec2.at(j);
	
	if(p1->GetBarcode() == p2->GetBarcode()) {
	  continue;
	}

	const double dr = p1->GetFourMom().GetDR(p2->GetFourMom());
      
	if(match.first.valid() && match.second.valid()) {
	  if(dr < min_dr) {
	    min_dr       = dr;
	    match.first  = p1;
	    match.second = p2;
	  }
	}
	else {
	  min_dr       = dr;
	  match.first  = p1;
	  match.second = p2;
	}
      }
    }
    
    return match;
  }
  

  //-----------------------------------------------------------------------------
  template<class T, class Y> std::pair<bool, double> GetMinDPhi(const Ptr<Y> &ptr, 
								const std::vector<Ptr<T> > &vec,
								double default_dphi=0.0) 
  {
    std::pair<bool, double> res(false, default_dphi);

    for(unsigned i = 0; i < vec.size(); ++i) {
      const Ptr<T> &p = vec.at(i);

      if(p->GetBarcode() == ptr->GetBarcode()) {
	continue;
      }

      const double dphi = std::fabs(ptr->GetFourMom().GetDPhi(p->GetFourMom()));
      
      if(res.first) {
	res.second = std::min<double>(res.second, dphi);
      }
      else {
	res.first  = true;
	res.second = dphi;	
      }
    }

    return res;
  }

  //-----------------------------------------------------------------------------
  template<class T, class Y> std::pair<bool, double> GetMinVecDPhi(const std::vector<Ptr<T> > &vec1, 
								   const std::vector<Ptr<Y> > &vec2,
								   double default_dphi) 
  {
    std::pair<bool, double> res(false, default_dphi);

    for(unsigned i = 0; i < vec1.size(); ++i) {
      const Ptr<T> &p1 = vec1.at(i);

      for(unsigned j = 0; j < vec2.size(); ++j) {
	const Ptr<Y> &p2 = vec2.at(j);
	
	if(p1->GetBarcode() == p2->GetBarcode()) {
	  continue;
	}

	const double dphi = std::fabs(p1->GetFourMom().GetDPhi(p2->GetFourMom()));
      
	if(res.first) {
	  res.second = std::min<double>(res.second, dphi);
	}
	else {
	  res.first  = true;
	  res.second = dphi;	
	}
      }
    }

    return res;
  }

  //-----------------------------------------------------------------------------
  template<class T> bool IsOS(const T &l1, const T &l2) 
  {
    if((l1.GetCharge() > 0 && l2.GetCharge() < 0) || 
       (l1.GetCharge() < 0 && l2.GetCharge() > 0)) {
      return true;
    }
    
    return false;
  }

  //-----------------------------------------------------------------------------
  template<class T, class Y> std::pair<double, Ptr<Y> > FindMinDR(const T &obj, const std::vector<Ptr<Y> > &ptrs) 
  {
    std::pair<double, Ptr<Y> > match(0.0, Ptr<Y>());

    for(const Ptr<Y> &ptr: ptrs) {
      const double dr = ptr->GetFourMom().GetDR(obj.GetFourMom());
      
      if(match.second.valid()) {
	if(dr < match.first) {
	  match.first  = dr;
	  match.second = ptr;
	}
      }
      else {
	match.first  = dr;
	match.second = ptr;     
      }
    }
    
    return match;
  }

  //-----------------------------------------------------------------------------
  template<class T, class Y> std::pair<double, Ptr<Y> > FindAndEraseMinDR(const T &obj, std::vector<Ptr<Y> > &ptrs) 
  {
    double mindr = 0.0;
    typename std::vector<Ptr<Y> >::iterator fit = ptrs.end();
    
    for(typename std::vector<Ptr<Y> >::iterator pit = ptrs.begin(); pit != ptrs.end(); ++pit) {

      const double dr = (*pit)->GetFourMom().GetDR(obj.GetFourMom());
      
      if(fit != ptrs.end()) {
	if(dr < mindr) {
	  mindr = dr;
	  fit   = pit;
	}
      }
      else {
	mindr = dr;
	fit   = pit;
      }
    }

    if(fit != ptrs.end()) {
      std::pair<double, Ptr<Y> > match(mindr, *fit);
      ptrs.erase(fit);
      
      return match;
    }
    
    return std::pair<double, Ptr<Y> >(0.0, Ptr<Y>());
  }

  //-----------------------------------------------------------------------------
  template<class T> bool AddVar(AlgEvent &alg, T &obj, unsigned var, double val, bool debug=false) {
    //
    // Add var if it does not exist, otherwise check values for consitency
    //
    double cur = 0.0;
    
    if(obj.GetVar(var, cur)) {
      if(debug && (val < cur || val > cur)) {
	alg.log() << "AddVar - different value already exist for " << var << ": " 
		  << "cur=" << std::setprecision(10) << cur << " and new=" << std::setprecision(10) << val << std::endl;
	return false;
    }
      
      return true;
    }
    
    return obj.AddVar(var, val);
  }

  //-----------------------------------------------------------------------------
  template<class T> int GetInt(Cut::Var var, const T &tp, int defval=0) {
    
    double val = defval;
  
    if(!tp.GetVar(var, val)) {
      return defval;
    }
    
    return static_cast<int>(val);
  }

  //-----------------------------------------------------------------------------
  template<class T> int GetLepCharge(const T &tp) {
    
    double val = 0.0;
  
    if(!tp.GetVar(Cut::PDG, val)) {
      std::cout << "GetLepCharge - " << tp.GetObjectType() << " - missing PDG variable" << std::endl;
      return 0;
    }
    
    if      (val > 0.0) { return -1; }
    else if (val < 0.0) { return +1; }

    std::cout << "GetLepCharge - " << tp.GetObjectType() << " - invalid PDG variable: " << val << std::endl;
    return 0;
  }

  //-----------------------------------------------------------------------------
  template<class T> void SavePtrVectorAsTree(TDirectory *dir, const std::string &key, const std::vector<Ptr<T> > &ptrs) {

    if(!dir || ptrs.empty()) {
      return;
    }
  
    T *p = new T();
    
    TTree *t = new TTree(key.c_str(), key.c_str());
    t->SetDirectory(dir);
    t->Branch("obj", &p);
    
    for(unsigned i = 0; i < ptrs.size(); ++i) {
      *p = ptrs.at(i).ref();
      t->Fill();
    }

    delete p;
  }

  //-----------------------------------------------------------------------------
  template<class T> void SaveObjVectorAsTree(TDirectory *dir, const std::string &key, const std::vector<T> &objs) {

    if(!dir || objs.empty()) {
      return;
    }
  
    T *p = new T();
    
    TTree *t = new TTree(key.c_str(), key.c_str());
    t->SetDirectory(dir);
    t->Branch("obj", &p);
    
    for(const T &obj: objs) {
      *p = obj;
      t->Fill();
    }

    delete p;
  }

  //-----------------------------------------------------------------------------
  template<class T> void SaveObjectAsTree(TDirectory *dir, const std::string &key, const T &obj) 
  {
    if(!dir) {
      return;
    }
  
    T *p = new T(obj);
    
    TTree *t = new TTree(key.c_str(), key.c_str());
    t->SetDirectory(dir);
    t->Branch("obj", &p);    
    t->Fill();

    delete p;
  }

  //-----------------------------------------------------------------------------
  template<class T> void ReadPtrVectorFromTree(TDirectory *dir, const std::string &key, std::vector<Ptr<T> > &ptrs) {

    if(!dir) {
      return;
    }

    T *p = 0;

    TTree *t = dynamic_cast<TTree *>(dir->Get(key.c_str()));

    if(!t) {
      return;
    }

    t->SetBranchStatus ("obj", 1);
    t->SetBranchAddress("obj", &p);

    for(int i = 0; i < t->GetEntries(); ++i) {
      t->GetEntry(i);    
      ptrs.push_back(ObjectFactory<T>::Instance().CreateObject(*p));
    }

    std::cout << "   \"" << key << "\": read " << ptrs.size() << " object pointer(s)" << std::endl;
  }

  //-----------------------------------------------------------------------------
  template<class T> void ReadObjVectorFromTree(TDirectory *dir, const std::string &key, std::vector<T> &ptrs) {

    if(!dir) {
      return;
    }

    T *p = 0;

    TTree *t = dynamic_cast<TTree *>(dir->Get(key.c_str()));

    if(!t) {
      return;
    }

    t->SetBranchStatus ("obj", 1);
    t->SetBranchAddress("obj", &p);

    for(int i = 0; i < t->GetEntries(); ++i) {
      t->GetEntry(i);    
      ptrs.push_back(*p);
    }
  }

  //-----------------------------------------------------------------------------
  template<class T> bool ReadObjectFromFile(const std::string &path, const std::string &key, T &obj)
  {
    TFile *file = TFile::Open(path.c_str(), "READ");
    
    if(!file->IsOpen()) {
      std::cout << "ReadObjectFromFile - failed to open file: " << path << std::endl;
      return false;
    }

    TTree *t = dynamic_cast<TTree *>(file->Get(key.c_str()));

    if(!t) {
      std::cout << "ReadObjectFromFile - failed to get tree: " << path << ":" << key << std::endl;
      file->Close();
      return false;
    }

    if(t->GetEntries() != 1) {
      std::cout << "ReadObjectFromFile - wrong number of entries: : " << path << ":" << key << " = " << t->GetEntries() << std::endl;

      file->Close();
      return false;
    }

    T *p = 0;

    t->SetBranchStatus ("obj", 1);
    t->SetBranchAddress("obj", &p);

    t->GetEntry(0);
    obj = *p;

    file->Close();
    
    return true;
  }
}

#endif
