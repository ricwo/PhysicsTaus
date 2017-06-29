// This is -*- c++ -*-
#ifndef ANP_CUTFLOW_H
#define ANP_CUTFLOW_H 

/**********************************************************************************
 * @Package: PhysicsTaus
 * @Class  : CutFlow
 * @Author : Rustem Ospanov, Ricardo Woelker
 *
 * @Brief  :
 * 
 *  Run analysis cut-flow
 * 
 **********************************************************************************/

// C/C++
#include <iostream>
#include <map>

// Base
#include "PhysicsBase/Ptr.h"
#include "PhysicsBase/Registry.h"

// Data
#include "PhysicsData/EventInfo.h"

// Local
#include "PhysicsTaus/CutItem.h"
#include "PhysicsTaus/CutPoll.h"

namespace Anp
{
  class CutFlow
  {
  public:

    CutFlow();
    virtual ~CutFlow();
    
    void ConfCut(const std::string &key, const Registry &reg);

    template<class T> Cut::State PassCut(const T &event);

    template<class T> Cut::State PassCut(const T &event, const EventInfo &info);

    template<class T> Cut::State PassCut(const T &event, unsigned id, double weight);

    double GetCutWeight() const { return fWeight; }

    void AddVarPtr(const Ptr<VarEntry> &ptr);

    void AddPlotVar(const Ptr<VarEntry> &ptr, const std::string &key, const std::string &hist);

    void PrintCut (std::ostream &os, const std::string &pad="   ") const { PrintCuts(os, pad); }
    void PrintCuts(std::ostream &os, const std::string &pad="   ") const;

    void PrintConf(std::ostream &os, const std::string &pad="") const;

    void SaveCutFlow(const std::string &key, TDirectory *dir) const;

    void UseRawCounts(bool flag);

    bool HasCuts() const { return fCuts.size(); }

  public:

    typedef std::vector<Ptr<CutFlow> > SubVec;

    struct CutPair
    {
      CutItem  icut;
      CutPoll  poll;
    };

    typedef std::vector<Ptr<CutPair> > CutVec;

  private:

    void AddCut(const std::string &key, const Registry &reg);

    void SaveSubFlow(const Ptr<CutPoll> &input, 
		     const std::string &key, TDirectory *dir, const CutVec &cuts) const;

    void SaveVarHist(const CutPoll &poll, const std::string &prefix, TDirectory *dir) const;

    std::ostream& log() const;

  private:

    bool                        fDebug;
    double                      fWeight;

    CutVec                      fCuts;
    SubVec                      fSubs;
    Ptr<CutPoll>                fInput;
  };

  //
  // Helper class to store all cut-flows into a single tex file
  //
  class CutFlowMan
  {
  public:
    
    static CutFlowMan& Instance();
    
    void AddCutFlow(const std::string &name, const std::string &cutflow) { fCutFlows[name] = cutflow; }
    void AddRawFlow(const std::string &name, const std::string &rawflow) { fRawFlows[name] = rawflow; }
    
    void WriteCutFlows(const std::string &path) const;
    void WriteRawFlows(const std::string &path) const;

  private:     
    
    CutFlowMan() {}
    ~CutFlowMan() {}
    
    CutFlowMan(const CutFlowMan &);
    const CutFlowMan& operator=(const CutFlowMan &);
    
    void WriteFlows(const std::string &path, const std::map<std::string, std::string> &flows) const;
    
    std::string ReplaceUnderscore(std::string str) const;

  private:
    
    std::map<std::string, std::string> fCutFlows;
    std::map<std::string, std::string> fRawFlows;
  };


  //-----------------------------------------------------------------------------
  // Template implementation
  //
  template<class T> inline Cut::State CutFlow::PassCut(const T &event) {
    return PassCut<T>(event, 0, 1.0);
  }

  template<class T> inline Cut::State CutFlow::PassCut(const T &event, const EventInfo &info) {
    return PassCut<T>(event, info.GetMCChannel(), info.GetWeight());
  }

  template<class T> inline Cut::State CutFlow::PassCut(const T &event, unsigned id, double weight) {
    //
    // Count all events
    //
    fWeight = weight;

    if(fInput.valid()) {
      fInput->CountEvent(id, fWeight);
    }

    if(fCuts.empty()) {
      return Cut::None;
    }

    //
    // Apply cuts
    //
    bool pass = true;
    
    for(unsigned i = 0; i < fCuts.size(); ++i) {
      CutPair &c = fCuts.at(i).ref();
      
      if(c.icut.PassCut(event) == Cut::Pass) {

	const std::vector<Anp::Cut::Var> &weights = c.icut.GetWeightVars();
	for(unsigned w = 0; w < weights.size(); ++w) {
	  double wval = 0.0;

	  if(event.GetVar(weights.at(w), wval)) {
	    fWeight *= wval;
	  }
	}

	c.poll.CountEvent(id, fWeight);	
      }
      else {
	pass = false;
	break;
      }
    }   

    if(pass) {
      for(unsigned j = 0; j < fSubs.size(); ++j) {
	fSubs.at(j)->PassCut(event, id, fWeight);
      }
      
      return Cut::Pass;
    }

    return Cut::Fail;
  }
}

#endif
