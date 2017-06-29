// This is -*- c++ -*-
#ifndef ANP_PRINTEVENT_H
#define ANP_PRINTEVENT_H

/**********************************************************************************
 * @Package: PhysicsTaus
 * @Class  : PrintEvent
 * @Author : Rustem Ospanov, Ricardo Woelker
 *
 * @Brief  :
 * 
 *  Print event content
 * 
 **********************************************************************************/

// Common
#include "PhysicsTaus/PlotBase.h"
#include "PhysicsData/RecoEvent.h"

// Local
#include "PhysicsTaus/HistKey.h"
#include "PhysicsTaus/CutBase.h"

namespace Anp
{
  class PrintEvent: public virtual PlotBase, public virtual CutBase
  {
  public:

    PrintEvent();
    virtual ~PrintEvent() {}
    
    void PlotConf(const Registry &reg);
    
    bool PlotInit();

    void PlotExec();

    void PlotDone();

  private:

    template<class T> std::string PrintObjects(const std::vector<Ptr<T> > &ptrs, 
					       const std::string &key,
					       const std::string &pad);
    
    std::string PrintTruths(const std::vector<Ptr<TruthPart> > &ptrs, 
			    const std::string &key, 
			    const std::string &pad);
      
    void PrintRecoEvent(const RecoEvent &reco);

    void PrintCandEvent(const CandEvent &cand);

  private:

    bool                        fPrintCandEvent;
    bool                        fPrintRecoEvent;

    Ptr<CutFlow>                fCutEvent;
    Ptr<CutFlow>                fCutElec;
    Ptr<CutFlow>                fCutJet;
    Ptr<CutFlow>                fCutMuon;
    Ptr<CutFlow>                fCutTau;

    Ptr<HistKey>                fHistEvent;
    Ptr<HistKey>                fHistMuon;
    Ptr<HistKey>                fHistElec;

    TH1                        *hMll;
  };
}

#endif
