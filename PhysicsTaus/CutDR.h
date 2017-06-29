#ifndef ANP_CUTDR_H
#define ANP_CUTDR_H

/**********************************************************************************
 * @Package: PhysicsTaus
 * @Class  : CutDR
 * @Author : Rustem Ospanov, Ricardo Woelker
 *
 * @Brief  :
 * 
 *  CutDR is a helper class that implements DR cut with main goal
 *  to optimize performance of overlap removal cuts in PrepCand.
 * 
 **********************************************************************************/

// C/C++
#include <string>

// ROOT
#include "TH1.h"

namespace Anp
{
  class CutDR
  {
  public:

    CutDR();
    CutDR(const std::string &name, double mindr, bool debug);
    ~CutDR() {}

    void SetHistDR(TH1 *hall, TH1 *hpass);

    bool IsValid() const { return fValid; }

    bool PassCutDR(bool good, double dr);

  private:
    
    std::string                  fName;
    bool                         fDebug;
    bool                         fValid;

    double                       fMinDR;

    TH1                         *hDRAll;
    TH1                         *hDRPass;
  };

  inline CutDR::CutDR():
    fDebug (false), 
    fValid (false), 
    fMinDR (-1.0), 
    hDRAll (0),
    hDRPass(0)
    {
    }

  inline CutDR::CutDR(const std::string &name, double mindr, bool debug):
    fName  (name),
    fDebug (debug), 
    fValid (mindr > 0.0), 
    fMinDR (mindr), 
    hDRAll (0),
    hDRPass(0)
    {
    }

  inline void CutDR::SetHistDR(TH1 *hall, TH1 *hpass) {
    hDRAll  = hall;
    hDRPass = hpass;
  }

  inline bool CutDR::PassCutDR(bool good, double dr) {
    if(!good) {
      std::cout << "CutDR::PassCutDR - logic error: not good" << std::endl;
    }

    if(!fValid) {
      std::cout << "CutDR::PassCutDR - logic error: not valid" << std::endl;
    }

    const bool pass = dr > fMinDR;

    if(hDRAll) {
      hDRAll->Fill(dr);
    }
    if(hDRPass && pass) {
      hDRPass->Fill(dr);
    }

    if(fDebug) {
      std::cout << fName << "::PassCutDR - pass=" << pass << " mindr=" << fMinDR << " dr=" << dr << std::endl;
    }
    
    return pass;
  }
}

#endif
