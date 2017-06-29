// This is -*- c++ -*-
#ifndef ANP_READNTUPLE_H
#define ANP_READNTUPLE_H

/**********************************************************************************
 * @Package: PhysicsTaus
 * @Class  : ReadNtuple
 * @Author : Rustem Ospanov, Ricardo Woelker
 *
 * @Brief  :
 *  5
 *  Top level event loop manager.
 * 
 *  Module for reads flat ntuple files and creates AlgEvent instances.
 *  Created algorithms go through these steps:
 *
 *  - Config() - create and configure C++ algorithms
 *  - Init()   - initialize algorithms and read configuration (Registry from python or XML)
 *  - Exec()   - event loop
 *  - Done()   - finalize algorithms
 *
 *  This function configures and executes job using input XML file:
 *
 *  - Execute() - configure input files execute above functions 
 *                using registry read from input path to XML file
 *
 **********************************************************************************/

// C/C++
#include <map>
#include <set>
#include <vector>

// ROOT
#include "TStopwatch.h"

// Base
#include "PhysicsBase/AlgEvent.h"
#include "PhysicsBase/Registry.h"

// Common
#include "PhysicsData/RecoEvent.h"

// Local
#include "PhysicsTaus/CutVars.h"
#include "PhysicsTaus/ReadUtils.h"

class TFile;
class TTree;

namespace Anp
{
  class ReadNtuple: public AlgEvent
  {
  public:
    
    ReadNtuple();
    virtual ~ReadNtuple();
    
    void Execute(const std::string &config_path);

    void ExecuteRegistry(const Registry &reg);

  private:

    void Config(const Anp::Registry &reg);
    
    bool Init();

    void Exec();

    void Done();

  private:

    void ReadFile(const std::string &fpath, long &icount);
    
    void InitTree(TTree *tree);

    long CountNEvent(const std::vector<std::string> &fpaths);

    unsigned FillReco(RecoEvent &event);

    bool StopNow(long count) const { return fNEvent > 0 && count+1 > fNEvent; }

    bool SkipEvent(long count) const;

    template<class T> bool AddVar(T &obj, unsigned index) const;

  public:

    enum KeyBranch 
    {
      kNone,
      kClust,
      kElec,
      kMuon,
      kTauR,
      kTauV,
      kJet4,
      kJet6,
      kTrackJet,
      kRVtx,
      kGen,
      kTVtx,
      kTrack,
      kHit,
      kTruth,
      kTruthVtx
    };

  private:

    typedef std::vector<VarData>  VarVec;
    typedef std::vector<VecData>  VecVec;
    typedef std::vector<Cut::Var> CutVec;

    struct List
    {      
      unsigned GetVarSize() const;

      bool MatchKey(KeyBranch key) const { return bkeys.count(key) > 0; }

      std::string         prefix;
      VecVec              vars;
      std::set<KeyBranch> bkeys;

      void Print() const;
    };

    typedef std::vector<List>        ListVec;
    typedef std::vector<VarNick>     NickVec;
    typedef std::vector<std::string> StrVec;
    typedef std::set<int>            IntSet;

  private:

    template<class T> void FillMom(const VecVec &vecs, unsigned index, T &obj);

    template<class T> unsigned FillMomVec(const List &l, std::vector<Ptr<T> > &vec);

    template<class T> unsigned FillObjVec(const List &l, std::vector<Ptr<T> > &vec);

    unsigned FillGenEvent(const List   &l,    std::vector<Ptr<GenEvent > > &vec);
    unsigned FillRecoShw (const List   &l,    std::vector<Ptr<RecShower> > &vec);
    unsigned FillRecoHit (const List   &l,    std::vector<Ptr<RecHit   > > &vec);
    unsigned FillTruePar (const List   &l,    std::vector<Ptr<TruthPart> > &vec);

    void ReadNick(const Registry &reg);

    Cut::Var FindVar(const std::string &val) const;

    std::set<KeyBranch> GetKeyBranchSet(const std::string &prefix) const;

  private:    

    TFile                     *fFile;           // Output ROOT file pointer
    TStopwatch                 fTimerEvent;     // Event processing timer
    TStopwatch                 fTimerTotal;     // Total processing timer
    Registry                   fReg;            // Global Registry configuration

    StrVec                     fTreeNames;      // Names of input ROOT tree(s)

    std::string                fBranchJets;

    std::string                fJetVecPrefix;      // Prefix for vector of jet   variables
    std::string                fTrackJetVecPrefix; // Prefix for vector of track jet variables
    std::string                fElecVecPrefix;     // Prefix for vector of elec  variables
    std::string                fMuonVecPrefix;     // Prefix for vector of muon  variables
    std::string                fTauRVecPrefix;     // Prefix for vector of reco tau variables
    std::string                fTauVVecPrefix;     // Prefix for vector of true tau variables
    std::string                fTrackVecPrefix;    // Prefix for vector of track variables
    std::string                fHitVecPrefix;      // Prefix for vector of hit   variables
    std::string                fTruthVecPrefix;    // Prefix for vector of mc    variables
    std::string                fRecoVtxVecPrefix;  // Prefix for vector of reco vtx variables
    std::string                fTruthVtxVecPrefix; // Prefix for vector of mc   vtx variables

    RecoBranch                 fReco;              // Output RecoEvent
    JetBranch                  fJets;              // Output non-default jets
 
    Handle<AlgEvent>           fAlg;               // Algorithm to be executed
    
    CutVec                     fVetoVars;
    CutVec                     fVetoVecs;

    VarVec                     fVars;
    NickVec                    fNicks;

    ListVec                    fLists;
    ListVec                    fVVecs;

    // Properties:
    bool                       fDebug;          // Print debug info for algorithms
    bool                       fDebugVars;      // Print debug info for variables
    bool                       fCloseFile;
    bool                       fPrintFiles;     // Print names of input root files
    bool                       fFillTrueParts;

    long                       fNEvent;         // Maximum number of events to read
    long                       fNEventPerFile;  // Maximum number of events to read per file (for tests)
    long                       fNPrint;         // Number of events to print    
    unsigned                   fCompression;    // TFile compression factor
    double                     fEventFracMin;
    double                     fEventFracMax;

    // Variables:
    std::vector<std::string>   fInputFiles;     // Input files    
    long                       fICount;         // Number of events to read
  };

  //------------------------------------------------------------------------------------
  // Inlined functions
  //
  inline bool ReadNtuple::SkipEvent(long count) const 
  {
    if(!(fEventFracMin < fEventFracMax)) {
      return false;
    }
    
    if(fNEvent < 1) {
      return false;
    }
    
    const double ifrac = count/double(fNEvent);
    
    return !(fEventFracMin <= ifrac && ifrac < fEventFracMax);
  }
}

#endif
