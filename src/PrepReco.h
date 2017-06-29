// Dear emacs, this is -*- c++ -*-
#ifndef ANP_PREPRECO_H
#define ANP_PREPRECO_H

/**********************************************************************************
 * @Package: PhysicsTaus
 * @Class  : PrepReco
 * @Author : Rustem Ospanov, Ricardo Woelker
 *
 * @Brief  :
 * 
 *  Prepare event candidate
 * 
 **********************************************************************************/

// C/C++
#include <iostream>
#include <set>
#include <string>

// Base
#include "PhysicsBase/AlgEvent.h"

// Common
#include "PhysicsData/RecoEvent.h"

// Local
#include "PhysicsTaus/CutBase.h"
#include "CopyHist.h"

namespace Anp
{
  class PrepReco: public virtual AlgEvent, public virtual CutBase
  {
  public:

    PrepReco();
    virtual ~PrepReco();    

    void Config(const Registry &reg);

    void Save(TDirectory *dir);
    
    bool Init();
    
    void Exec();
    
    void Done();

    void Signal(const Anp::Registry &reg, const std::string &key);

  private:

    struct RunData
    {
      RunData():run(0), weight(0.0) {}
 
      unsigned run;
      double   weight;
    };

    struct ChanData
    {
      ChanData(): eventN(0.0), eventW(0.0) {}

      double  eventN;
      double  eventW;
    };

    typedef std::vector<Cut::Var>         CutVec;
    typedef std::vector<RunData>          RunVec;

    typedef std::pair<unsigned, uint64_t> RunEvent;
    typedef std::set<RunEvent>            RunEventSet;
    typedef std::map<unsigned, ChanData>  ChanMap;
    
  private:
    
    void ProcessReco (RecoEvent &event);
    void ProcessTrue (RecoEvent &event);

    void ProcessCount(int mc_chan, double weight);

    bool ReadSumWeightHist(const std::string &path);

    template<class T> bool AddVar(T &obj, Cut::Var var, double val);

    template<class T> void Filter(std::vector<Ptr<T> > &objs, int pdg);

    template<class T> void AddLepMomVars(T &obj, double mass);

    template<class T> void SetCharge(std::vector<Ptr<T> > &objs);

  private:

    TDirectory                 *fDir;
    
    // Properties:
    bool                        fDebug;
    bool                        fDebugVars;
    bool                        fPrint;
    bool                        fNoWeight;   
    bool                        fDoCopy;
    bool                        fDoCount;
    bool                        fDoLepIdFilter;
    bool                        fDoTrue;
    bool                        fCheckDuplicate;
    bool                        fAddLeptonMass;
    bool                        fReadSumWeight;

    double                      fMassElec;
    double                      fMassMuon;
    double                      fMassTau;

    // Variables:
    Ptr<CopyHist>               fCopy;

    ChanMap                     fChanMap;

    RunVec                      fRuns;
    RunEventSet                 fCheck;

    CutVec                      fWeights;
    CutVec                      fWeightsInv;
    CutVec                      fWeightsDiv;

    RecoBranch                  fReco;
  };
}

#endif
