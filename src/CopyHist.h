#ifndef ANP_COPYHIST_H
#define ANP_COPYHIST_H

/**********************************************************************************
 * @Package: PhysicsTaus
 * @Class  : CopyHist
 * @Author : Rustem Ospanov, Ricardo Woelker
 *
 * @Brief  :
 * 
 *  Copy histograms from input file to output file
 * 
 **********************************************************************************/

// C/C++
#include <iostream>
#include <map>
#include <string>
#include <vector>

// Base
#include "PhysicsBase/Registry.h"
#include "PhysicsBase/AlgEvent.h"

// Local
#include "PhysicsTaus/CutVars.h"

class TH1;
class TDirectory;

namespace Anp
{
  class CopyHist: public virtual Callback
  {
  public:

    CopyHist();
    virtual ~CopyHist();    

    void Config(const Registry &reg, const std::string &key);

    void Signal(const Registry &reg, const std::string &key);

    void CopyObjects(int mc_chan, double weight);

    void SaveObjects(TDirectory *topdir);

    const std::string& GetName() const { return fName; }
    const std::string& GetType() const { return fType; }

  private:

    typedef std::map<std::string, TH1 *> HistMap;

    struct CopyData
    {
      CopyData() {}

      HistMap hists;
    };

    typedef std::vector<Cut::Var>        CutVec;
    typedef std::map<unsigned, CopyData> CopyMap;
    
  private:

   std::ostream& log() const;

  private:
    
    // Properties:
    bool                        fDebug;
    bool                        fPrint;
    std::string                 fName;
    std::string                 fType;

    std::vector<std::string>    fCopyNames;

    // Variables:
    bool                        fResetFile;
    int                         fCurrMCChan;

    std::string                 fInputFilePath;

    CopyMap                     fCopyMap;
  };
}

#endif
