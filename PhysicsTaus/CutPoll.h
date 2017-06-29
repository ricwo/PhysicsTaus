#ifndef ANP_CUTPOLL_H
#define ANP_CUTPOLL_H

/**********************************************************************************
 * @Package: PhysicsStudy
 * @Class  : CutPoll
 * @Author : Rustem Ospanov, Ricardo Woelker
 *
 * @Brief  :
 * 
 *  Count events with weight by sample for cut-flows
 * 
 **********************************************************************************/

// C/C++
#include <map>
#include <set>

// ROOT
#include "TH1.h"

// Base
#include "PhysicsBase/Ptr.h"

// Data
#include "PhysicsData/VarEntry.h"

namespace Anp
{
  typedef std::set<unsigned> SampleSet;

  class CutPoll
  {
  public:

    CutPoll();
    ~CutPoll() {}
    
    void CountEvent(unsigned sample, double weight);
    
    void Print(const std::string &key) const;
    
    std::string Convert2Str(double val, double err) const;

    bool GetCountError(                         double &val, double &err) const;
    bool GetCountError(unsigned         sample, double &val, double &err) const;
    bool GetCountError(const SampleSet &sample, double &val, double &err) const;

    std::pair<std::string, std::string> GetCountErrorAsPair()                        const;
    std::pair<std::string, std::string> GetCountErrorAsPair(unsigned         sample) const;
    std::pair<std::string, std::string> GetCountErrorAsPair(const SampleSet &sample) const;

    void SetName(const std::string &name) { fName   = name; }
    void UseRaw (bool               flag) { fUseRaw = flag; }

    const std::string& GetName() const { return fName; }
    TH1*               GetHist() const { return fHist; }

    void AddPlotVar(const Ptr<VarEntry> &ptr, const std::string &key, const std::string &hist);

  private:

    struct Count 
    {
      Count() :sumn(0), sumw(0.0), sumw2(0.0) {}
      
      unsigned sumn;
      double   sumw;
      double   sumw2;
    };

  private:

    typedef std::map<unsigned, Count> SampleMap;
    
  private:

    bool           fUseRaw;
    std::string    fName;
    std::string    fHistKey;

    SampleMap      fSamples;

    TH1           *fHist;
    Ptr<VarEntry>  fVarPtr;
  };

  //
  // Inlined functions
  //
  inline void Anp::CutPoll::CountEvent(unsigned sample, double weight)
  {
    Count &c = fSamples[sample];

    c.sumn  += 1;
    c.sumw  += weight;
    c.sumw2 += weight*weight;

    if(fHist && fVarPtr.valid()) {
      fHist->Fill(fVarPtr->GetData(), weight);
    }
  }
}

#endif
