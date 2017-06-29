// This is -*- c++ -*-
#ifndef ANP_RUNNEXUS_H
#define ANP_RUNNEXUS_H

/**********************************************************************************
 * @Package: PhysicsTaus
 * @Class  : RunNexus
 * @Author : Rustem Ospanov, Ricardo Woelker
 *
 * @Brief  :
 * 
 * This class creates, configures and executes a list of AlgEvent algorithms.
 * 
 **********************************************************************************/

// C/C++
#include <vector>

// Base
#include "PhysicsBase/AlgEvent.h"
#include "PhysicsBase/NtupleSvc.h"

// Common
#include "PhysicsData/RecoEvent.h"

// Local
#include "PhysicsTaus/CutBase.h"

namespace Anp
{
  class RunNexus: public virtual AlgEvent, public virtual CutBase
  {
  public:
    
    RunNexus();
    virtual ~RunNexus();
    
    void Config(const Anp::Registry &reg);
    
    void Signal(const Anp::Registry &reg, const std::string &signal);

    void Save(TDirectory *);

    bool Init();

    void Exec();

    void Done();

  private:

    struct AlgData
    {
      AlgData(const std::string &k) :dir(0), key(k) {}

      Handle<AlgEvent> alg;
      Ptr<CutFlow>     cut;
      TDirectory      *dir;
      std::string      key;
    };

    struct AlgList
    {
      AlgList() :file(0) {}

      TFile                *file;
      std::vector<AlgData>  algs;      
    };

    struct RunData
    {
      explicit RunData(const std::string &k) :key(k) {}

      std::string      key;
      std::string      name;
      std::vector<int> runs; 

      bool MatchRun(int run) const { return std::find(runs.begin(), runs.end(), run) != runs.end(); }
    };

    typedef std::map<int, AlgList> AlgMap;
    typedef std::vector<RunData>   RunVec;

  private:

    void RunAlgs(int ival);

    void MakeAlg(const std::string &sval, const std::string &postfix, AlgData &data, TDirectory *dir);

    TFile* GetAlgFile(int ival);

  private:

    TDirectory                      *fDir;         // Output directory

    bool                             fDebug;
    bool                             fPrint;
    bool                             fDoFile;

    std::string                      fAlgName;     // Variable name    
    std::string                      fDirName;     // Output directory name
    std::string                      fVarName;     // Variable name    
    std::string                      fFileKey;     // File for unique alg

    Cut::Var                         fVar;

    RecoBranch                       fReco;        // RecoEvent branch (contains candidates)

    Registry                         fRegTop;
    Registry                         fRegAlg;

    AlgMap                           fAlgs;
    RunVec                           fRuns;
  }; 
}

#endif
