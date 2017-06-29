// This is -*- c++ -*-
#ifndef ANP_PLOTBASE_H
#define ANP_PLOTBASE_H

/**********************************************************************************
 * @Package: PhysicsCommon
 * @Class  : PlotBase
 * @Author : Rustem Ospanov, Ricardo Woelker
 *
 * @Brief  :
 * 
 *  PlotBase is a base class for plotting algorithms
 * 
 **********************************************************************************/

// C/C++
#include <string>
#include <vector>

// ROOT
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

// Base
#include "PhysicsBase/AlgEvent.h"

// Data
#include "PhysicsData/RecoEvent.h"

// Local
#include "HistBase.h"

class TDirectory;
class TProfile;

namespace Anp
{
  class PlotBase: public virtual AlgEvent, public virtual HistBase
  {
  public:

    PlotBase();
    virtual ~PlotBase();

    void Config(const Registry &reg);

    void Save(TDirectory *dir);
    
    bool Init();
    
    void Exec();
    
    void Done();

    virtual void PlotConf(const Registry &) = 0;

    virtual bool PlotInit() = 0;

    virtual void PlotExec() = 0;

    virtual void PlotDone() = 0;

  protected:
    
    TDirectory* GetDir(const std::string &name = "") { return Anp::GetDir(fDir, name); }
    TDirectory* TopDir()                             { return fDirTop;                 }

    const std::string &GetDirName() const { return fDirName; }
    const std::string &GetKeyHist() const { return fKeyHist; }

  protected:

    RecoBranch                   fReco;         // RecoEvent branch

    bool                         fDebug;
    bool                         fPrint;

  private:

    TDirectory                  *fDir;          // Output ROOT directory
    TDirectory                  *fDirTop;       // Top    ROOT directory

    TH1                         *hEventWeight;  // Event weight sum

    std::string                  fDirName;      // TDirectory name
  };

  //-----------------------------------------------------------------------------
  // Inlined functions
  //
  inline void PlotBase::Exec() {
    //
    // Read new event entry
    //
    if(fDebug) {
      log() << "Exec() is called" << std::endl;
    }
  
    if(fReco.IsValidEvent()) {
      //
      // Execute virtual function for valid reco events
      //
      PlotExec();
      
      if(hEventWeight && fReco->size() == 1) {
	hEventWeight->Fill(0.5, fReco->at(0).GetEventInfo()->GetWeight());
      }
    }
  }
}

#endif
