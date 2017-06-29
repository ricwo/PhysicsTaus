// This is -*- c++ -*-
#ifndef ANP_RUNCHAIN_H
#define ANP_RUNCHAIN_H

/**********************************************************************************
 * @Package: PhysicsTaus
 * @Class  : RunChain
 * @Author : Rustem Ospanov, Ricardo Woelker
 *
 * @Brief  :
 * 
 * This class creates, configures and executes a list of AlgEvent algorithms.
 * 
 **********************************************************************************/

// C/C++
#include <vector>

// ROOT
#include "TStopwatch.h"

// Base
#include "PhysicsBase/RunAlgs.h"
#include "PhysicsBase/NtupleSvc.h"

// Common
#include "PhysicsData/RecoEvent.h"

// Local
#include "PhysicsTaus/CutBase.h"

namespace Anp
{
  class RunChain: public virtual AlgEvent, public virtual CutBase
  {
  public:
    
    RunChain();
    virtual ~RunChain();
    
    void Config(const Anp::Registry &reg);
    
    void Signal(const Anp::Registry &reg, const std::string &signal);

    void Save(TDirectory *);

    bool Init();

    void Exec();

    void Done();

  private:

    typedef std::vector<Ptr<CandEvent> > CandEventVec;

  private:

    void Process(RecoEvent &reco);

    double GetEventWeight(const CandEvent &event, const std::vector<Cut::Var> &weights) const;

  private:

    TDirectory                      *fDir;         // Output directory

    bool                             fDebug;
    bool                             fDebugVars;
    bool                             fPrint;
    bool                             fDoTimer;
    bool                             fNoWeight;

    bool                             fPrintConfig;
    bool                             fPrintCounts;
    bool                             fFirstEventInit;

    std::string                      fDirName;     // Output directory name
    std::string                      fCandName;    // Select candidate name
    uint32_t                         fCandId;

    bool                             fDoScaleWeight;
    double                           fScaleWeight;

    RecoBranch                       fReco;        // RecoEvent branch (contains candidates)
    Ptr<CutFlow>                     fCutCand;     // Candidate cut-flow

    Registry                         fReg;
    Handle<RunAlgs>                  fAlg;
    
    CandEventVec                     fInputCands;
    CandEventVec                     fEventCands;
    std::map<uint64_t, double>       fWeights;

    TStopwatch                       fTimerPrep;
    TStopwatch                       fTimerExec;

    std::vector<Cut::Var>            fCandWeightsData;
    std::vector<Cut::Var>            fCandWeightsSiml;

    double                           fRecoN;
    double                           fRecoW;
    double                           fCandN;
    double                           fCandW;
  }; 
}

#endif
