// This is -*- c++ -*-
#ifndef ANP_HISTKEY_H
#define ANP_HISTKEY_H

/**********************************************************************************
 * @Package: PhysicsTaus
 * @Class  : HistKey
 * @Author : Rustem Ospanov, Ricardo Woelker
 *
 * @Brief  :
 * 
 *  HistKey makes and fills histograms for var keys
 * 
 **********************************************************************************/

// C/C++
#include <map>
#include <stdint.h>

// ROOT
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

// Data
#include "PhysicsData/VarHolder.h"

class TDirectory;

namespace Anp
{
  class HistKey
  {
  public:

    HistKey();
    virtual ~HistKey();

    void SetHistKey   (const std::string &v) { fHistKey    = v; }
    void SetHistDir   (const std::string &v) { fHistDir    = v; }
    void SetHistPrefix(const std::string &v) { fHistPrefix = v; }

    void ConfigHist(const std::string &key, const Registry &reg);

    void SetDirHist(TDirectory *dir);

    TH2* BookHist2d(std::string hname, uint32_t keyX, uint32_t keyY);

    void FillHist(uint32_t key, double val, double weight, uint32_t offset, const std::string &prefix);
    void FillHist(uint32_t key, double val, double weight);

    void FillHists (const VarHolder   &vars,  double weight) { FillHists(vars.GetVars(), weight); }
    void FillHists (const VarEntryVec &vars,  double weight);
    void FillHists (const VarEntryVec &vars,  double weight, uint32_t offset, const std::string &prefix);
    void FillHist2d(const VarHolder   &vars,  double weight);
    
  private:

    struct Hist1d {
    Hist1d() :h(0), key(0) {}

      TH1 *h;
      int key;
    };

    struct Hist2d {
      Hist2d() :h(0), keyx(0), keyy(0) {}

      TH2 *h;
      int  keyx;
      int  keyy;
    };

    typedef std::vector<Hist1d>           Hist1dVec;
    typedef std::map<uint32_t, Hist1dVec> Hist1dMap;

    typedef std::map<uint32_t, TH1 * >    TH1Map;
    typedef std::map<std::string, Hist2d> TH2Map;

  private:

    void FillTH1 (TH1 *h, double val, double weight);
    TH1* FindHist(uint32_t key, uint32_t offset=0, const std::string &prefix = "");
    TH1* MakeHist(uint32_t key, uint32_t offset=0, const std::string &prefix = "");

    void Init1d();
    void Init2d(const VarHolder &vars);

  private:

    bool                         fDebug;
    bool                         fPrint;
    bool                         fSetSumw2;
    bool                         fAllVis;

    std::string                  fHistDir;
    std::string                  fHistKey;
    std::string                  fHistPrefix;

    std::vector<std::string>     fHistKeys1d;
    std::vector<std::string>     fHistKeys2d;

    bool                         fInit2d;

    Hist1dMap                    fHist1dMap;    // 1d histogram map
    
    TH1Map                       fTH1Map;       // 1d histogram map
    TH2Map                       fTH2Map;       // 2d histogram map
  };

  //-----------------------------------------------------------------------------
  //
  // Inlined functions
  //
  //-----------------------------------------------------------------------------
  inline TH1* HistKey::FindHist(uint32_t key, uint32_t offset, const std::string &prefix) {
    //
    // Find histogram by key, create new if the key does not exist
    //
    TH1Map::iterator hit = fTH1Map.find(key+offset);
    if(hit != fTH1Map.end()) {
      return hit->second;
    }

    if(fDebug) {
      std::cout << "HistKey::FindHist - make hist with key: " << key << std::endl;
    }

    return MakeHist(key, offset, prefix);
  }

  //-----------------------------------------------------------------------------
  inline void HistKey::FillTH1(TH1 *h, double val, double weight) {
    //
    // Read new event entry
    //
    if(!h) {
      return;
    }

    if(fDebug) {
      std::cout << "HistKey::FillTH1 - fill hist..." << std::endl;
    }

    if(fAllVis) {
      const double xmax = h->GetXaxis()->GetXmax();
      const double xmin = h->GetXaxis()->GetXmin();
      
      if(xmin < val && val < xmax) {
	h->Fill(val, weight);
      }
      else if (!(val < xmax)) {
	h->Fill(h->GetXaxis()->GetBinCenter(h->GetNbinsX()), weight);
      }
      else if (!(val > xmin)) {
	h->Fill(h->GetXaxis()->GetBinCenter(1), weight);
      }
      else {
	std::cout << "HistKey::FillHists - logic ERROR: " << fHistKey << std::endl;
      }
    }
    else {
      h->Fill(val, weight);
    }
  }

  //-----------------------------------------------------------------------------
  inline void HistKey::FillHist(uint32_t key, double val, double weight, uint32_t offset, const std::string &prefix) {
    //
    // Read new event entry
    //
    TH1 *h = FindHist(key, offset, prefix);
    if(h) {
      FillTH1(h, val, weight);
    }
  }

  //-----------------------------------------------------------------------------
  inline void HistKey::FillHist(uint32_t key, double val, double weight) {
    //
    // Read new event entry
    //
    TH1 *h = FindHist(key);
    if(h) {
      FillTH1(h, val, weight);
    }

    if(!fHist1dMap.empty()) {
      Hist1dMap::iterator hit = fHist1dMap.find(key);
      
      if(hit != fHist1dMap.end()) {
	for(Hist1d &h: hit->second) {
	  FillTH1(h.h, val, weight);
	}
      }
    }
  }

  //-----------------------------------------------------------------------------
  inline void HistKey::FillHists(const VarEntryVec &vars, double weight) {
    //
    // Read new event entry
    //
    for(const VarEntry &var: vars) {
      if(fDebug) {
	std::cout << "HistKey::FillHists - key: " << var.GetKey() << std::endl;
      }

      FillHist(var.GetKey(), var.GetData(), weight);
    }   
  }

  //-----------------------------------------------------------------------------
  inline void HistKey::FillHists(const VarEntryVec &vars, double weight, uint32_t offset, const std::string &prefix) {
    //
    // Read new event entry
    //
    for(const VarEntry &var: vars) {
      if(fDebug) {
	std::cout << "HistKey::FillHists - key: " << var.GetKey() << std::endl;
      }

      FillHist(var.GetKey(), var.GetData(), weight, offset, prefix);
    }
  }
}

#endif
