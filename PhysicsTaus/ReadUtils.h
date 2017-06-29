// This is -*- c++ -*-
#ifndef ANP_READUTILS_H
#define ANP_READUTILS_H

/**********************************************************************************
 * @Package: PhysicsTaus
 * @Class  : ReadUtils
 * @Author : Rustem Ospanov, Ricardo Woelker
 *
 * @Brief  :
 *  
 * Helper functions, classes and templates for reading flat trees
 *
 **********************************************************************************/

// C/C++
#include <map>
#include <set>
#include <vector>

// Base
#include "PhysicsBase/Ptr.h"
#include "PhysicsBase/Registry.h"

// Data
#include "PhysicsData/RecJet.h"
#include "PhysicsData/RecElec.h"
#include "PhysicsData/RecMuon.h"

// Local
#include "CutVars.h"

class TFile;
class TTree;

namespace Anp
{
  //-----------------------------------------------------------------------------------
  // Helper class to read float/double tree branch
  //
  struct VarData 
  {
    enum Type {
      kNone = 0,
      kFloat,
      kDouble,
      kShort,
      kLong,
      kInt,
      kUInt,
      kULong,
      kULong64,
      kBool,
      kChar
    };

    VarData();
    ~VarData() {}
    
    void Clear();

    void Print() const;
    
    double GetVal() const;
    
    template<typename T> T GetFullValue() const;

    template<class T> bool AddVar(T &obj) const
    { 
      if     (type == kDouble ) {                         return obj.AddVar(var, val_dbl); }
      else if(type == kFloat  ) {                         return obj.AddVar(var, val_flt); }
      else if(type == kInt    ) { CheckVal(val_int, obj); return obj.AddVar(var, val_int); }
      else if(type == kShort  ) {                         return obj.AddVar(var, val_snt); }
      else if(type == kLong   ) {                         return obj.AddVar(var, val_lnt); }
      else if(type == kBool   ) {                         return obj.AddVar(var, val_bln); }
      else if(type == kChar   ) {                         return obj.AddVar(var, val_chr); }
      else if(type == kUInt   ) { CheckVal(val_unt, obj); return obj.AddVar(var, val_unt); }
      else if(type == kULong64) { CheckVal(val_u64, obj); return obj.AddVar(var, val_u64); }
      
      std::cout << "VarData::AddVar<" << obj.GetObjectType() 
		<< "> - missing value for: " << Cut::AsStr(var)  
		<< " type=" << AsStr(type) << " valid=" << valid << std::endl;

      return false;
    }

    bool SetVarBranch(TTree *tree, bool debug=false);

    Type GetType(const std::string &t) const;

    std::string AsStr(Type t) const;

    template<typename T, class Y> void CheckVal(T val, const Y &obj) const
    {
      if(static_cast<T>(double(val)) != val) {
	static unsigned icount = 0;
	
	if(++icount < 1000) {
	  std::cout << "CheckVal - " << obj.GetObjectType() 
		    << " - loss of precision for var=\"" << Cut::AsStr(var) << "\""
		    << " type=" << AsStr(type) << " valid=" << valid 
		    << ": " << static_cast<T>(double(val)) << "!=" << val << std::endl;
	}
	else if(icount == 1000) {
	  std::cout << "   ...last warning" << std::endl;
	}
      }      
    }

    bool operator<(const VarData &rhs) const { return branch < rhs.branch; }
   
    std::string  key;
    std::string  branch;
    std::string  nick;

    Cut::Var     var;
    Type         type;
    bool         valid;

    Double_t              val_dbl;
    Float_t               val_flt;
    Int_t                 val_int;
    Short_t               val_snt;
    Long_t                val_lnt;
    UInt_t                val_unt;
    ULong64_t             val_u64;
    uint64_t              val_uln;
    Bool_t                val_bln;
    Char_t                val_chr;
  };

  //-----------------------------------------------------------------------------------
  // Helper class to read float/double tree branch
  //
  struct VecData 
  {
    enum Type {
      kNone,
      kFloat,
      kDouble,
      kInt,
      kShort,
      kLong,
      kUInt,
      kULong,
      kULong64,
      kBool,
      kChar,
      kVecInt,
      kVecFloat
    };

    VecData();
    ~VecData() {}
    
    void Clear();

    void Delete();

    void Print() const;
    
    bool SetVarBranch(TTree *tree);

    Type GetType(const std::string &t) const;

    unsigned GetSize() const;
    double   GetVal (unsigned index) const;
    
    bool IsVector() const;

    template<typename T> T       GetFullValue(unsigned index) const;
    template<typename T> Float_t GetFullValue(unsigned index, unsigned sysindex) const;

    template<class T> bool AddVar(T &obj, unsigned index) const
    {
      if(vvv_int && index < vvv_int->size()) {
	if(debug) {
	  std::cout << "VecData - " << branch << "/" << key << ": add vvv_int at " << index << std::endl;
	}

	return obj.AddVec(var, vvv_int->at(index));
      }

      if(vvv_flt && index < vvv_flt->size()) {
	if(debug) {
	  std::cout << "VecData - " << branch << "/" << key << ": add vvv_flt at " << index << std::endl;
	}

	return obj.AddVec(var, vvv_flt->at(index));
      }

      if(index < GetSize()) {
	if(debug) {
	  std::cout << "VecData - " << branch << "/" << key << ": add vector at " << index << std::endl;
	}

	return obj.AddVar(var, GetVal(index));
      }

      //      std::cout << "VecData - failed to add data for: " << obj.GetObjectType() << std::endl;
      return false;
    }

    bool operator<(const VarData &rhs) const { return key < rhs.key; }
    
    std::string  key;
    std::string  branch;
    Cut::Var     var;
    bool         debug;

    std::vector<Double_t>  *vec_dbl;
    std::vector<Float_t>   *vec_flt;
    std::vector<Int_t>     *vec_int;
    std::vector<Short_t>   *vec_snt;
    std::vector<Long_t>    *vec_lnt;
    std::vector<UInt_t>    *vec_unt;
    std::vector<ULong64_t> *vec_u64;
    std::vector<uint64_t>  *vec_uln;
    std::vector<Bool_t>    *vec_bln;
    std::vector<char>      *vec_chr;

    std::vector<std::vector<int> >   *vvv_int;
    std::vector<std::vector<float> > *vvv_flt;
  };

  struct VarNick
  {
    VarNick() :key(Cut::NONE) {}
    
    std::string nick;
    std::string var;      
    Cut::Var    key;
  };

  std::vector<VarNick> ReadNicks(const std::string &key, const Registry &reg);

  //-----------------------------------------------------------------------------
  // Split path using "/" separator
  //
  std::pair<std::string, std::string> inline SplitPath(const std::string &file, bool remove_trailing_slash = true) 
  {
    std::pair<std::string, std::string> res("", file);

    const size_t ipos = file.find_last_of('/');

    if(ipos != std::string::npos) {
      res.first  = file.substr(0, ipos+1);
      res.second = file.substr(ipos+1, std::string::npos);
      
      if(remove_trailing_slash && res.first.size() > 0 && res.first.at(res.first.size()-1) == '/') {
	res.first = res.first.substr(0, res.first.size()-1);	
      }
    }
    
    return res;
  }

  //-----------------------------------------------------------------------------
  // Helper functions for reading trees
  //
  template<class T> inline TBranch* SetBranch(TTree *tree, const std::string &branch, T &var)
  {
    if(!tree) {
      return 0;
    }
    
    TBranch *b = 0;

    if(tree->GetBranch(branch.c_str())) {
      tree->SetBranchStatus (branch.c_str(),    1);
      tree->SetBranchAddress(branch.c_str(), &var, &b); 
    }

    return b;
  }

  template<class T> inline TBranch* SetBranch(TTree *tree, const std::string &branch, T *&var)
  {
    if(!tree) {
      return 0;
    }

    TBranch *b = 0;
    
    if(tree->GetBranch(branch.c_str())) {
      tree->SetBranchStatus (branch.c_str(),    1);
      tree->SetBranchAddress(branch.c_str(), &var, &b); 
    }

    return b;
  }
}


//-----------------------------------------------------------------------------
namespace Anp
{
  template<> inline uint64_t VarData::GetFullValue<uint64_t>() const 
  { 
    if(type == kULong) return val_uln;
    std::cout << "VarData::GetFullValue<uint64_t> - missing value for: " << Cut::AsStr(var) << std::endl;
    return 0;
  }

  template<> inline Float_t VarData::GetFullValue<Float_t>() const 
  { 
    if(type == kFloat) return val_flt;
    std::cout << "VarData::GetFullValue<Float_t> - missing value for: " << Cut::AsStr(var) << std::endl;
    return 0;
  }
}


//-----------------------------------------------------------------------------
namespace Anp
{
  template<> inline uint64_t VecData::GetFullValue<uint64_t>(unsigned index) const 
  { 
    if(vec_uln && index < vec_uln->size()) return vec_uln->at(index);
    std::cout << "VecData::GetFullValue<uint64_t> - missing value for: " << Cut::AsStr(var) << std::endl;
    return 0;
  }
  template<> inline Int_t VecData::GetFullValue<Int_t>(unsigned index) const 
  { 
    if(vec_int && index < vec_int->size()) return vec_int->at(index);
    std::cout << "VecData::GetFullValue<Int_t> - missing value for: " << Cut::AsStr(var) << std::endl;
    return 0;
  }
  template<> inline UInt_t VecData::GetFullValue<UInt_t>(unsigned index) const 
  { 
    if(vec_unt && index < vec_unt->size()) return vec_unt->at(index);
    std::cout << "VecData::GetFullValue<UInt_t> - missing value for: " << Cut::AsStr(var) << std::endl;
    return 0;
  }
  template<> inline ULong64_t VecData::GetFullValue<ULong64_t>(unsigned index) const 
  { 
    if(vec_u64 && index < vec_u64->size()) return vec_u64->at(index);
    std::cout << "VecData::GetFullValue<ULong64_t> - missing value for: " << Cut::AsStr(var) << std::endl;
    return 0;
  }
  template<> inline Short_t VecData::GetFullValue<Short_t>(unsigned index) const 
  { 
    if(vec_snt && index < vec_snt->size()) return vec_snt->at(index);
    std::cout << "VecData::GetFullValue<Short_t> - missing value for: " << Cut::AsStr(var) << std::endl;
    return 0;
  }
  template<> inline Double_t VecData::GetFullValue<Double_t>(unsigned index) const 
  { 
    if(vec_dbl && index < vec_dbl->size()) return vec_dbl->at(index);
    std::cout << "VecData::GetFullValue<Double_t> - missing value for: " << Cut::AsStr(var) << std::endl;
    return 0;
  }
  template<> inline Float_t VecData::GetFullValue<Float_t>(unsigned index) const 
  { 
    if(vec_flt && index < vec_flt->size()) return vec_flt->at(index);
    std::cout << "VecData::GetFullValue<Float_t> - missing value for: " << Cut::AsStr(var) << std::endl;
    return 0;
  }
  template<> inline Bool_t VecData::GetFullValue<Bool_t>(unsigned index) const 
  { 
    if(vec_bln && index < vec_bln->size()) return vec_bln->at(index);
    std::cout << "VecData::GetFullValue<Bool_t> - missing value for: " << Cut::AsStr(var) << std::endl;
    return 0;
  }
  template<> inline Char_t VecData::GetFullValue<Char_t>(unsigned index) const 
  { 
    if(vec_chr && index < vec_chr->size()) return vec_chr->at(index);
    std::cout << "VecData::GetFullValue<Bool_t> - missing value for: " << Cut::AsStr(var) << std::endl;
    return 0;
  }
}

#endif
