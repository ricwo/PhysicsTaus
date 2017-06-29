#ifndef ANP_CUTBASE_H
#define ANP_CUTBASE_H

/**********************************************************************************
 * @Package: PhysicsTaus
 * @Class  : CutBase
 * @Author : Rustem Ospanov, Ricardo Woelker
 *
 * @Brief  :
 * 
 *  CutBase is a base class for plotting algorithms
 * 
 **********************************************************************************/

// C/C++
#include <string>
#include <vector>

// Base
#include "PhysicsBase/Ptr.h"
#include "PhysicsBase/Registry.h"

// Local
#include "PhysicsTaus/CutFlow.h"
#include "PhysicsTaus/HistBase.h"

class TDirectory;

namespace Anp
{
  class CutBase: public virtual HistBase
  {
  public:

    CutBase() {}
    virtual ~CutBase() {}

    Ptr<CutFlow> BookCut(const std::string &key, const Registry &reg);

    void PrintCutConfig(std::ostream &os = std::cout) const;

    void PrintCutFlows(std::ostream &os = std::cout) const;

    void SaveCutFlows(TDirectory *dir);

    void UseRawCounts(bool flag);

    void SaveCutsHists(TDirectory *dir);

  private:

    struct CutData
    {
      std::string  key;
      Ptr<CutFlow> cut;
    };

    typedef std::vector<CutData> CutVec;

  private:

    CutVec  fCuts;
  };
}

#endif
