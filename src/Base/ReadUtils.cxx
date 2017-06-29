// C/C++
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>

// Boost
#include <boost/foreach.hpp>

// ROOT
#include "TLeaf.h"
#include "TTree.h"

// Local
#include "PhysicsTaus/ReadUtils.h"

using namespace std;

//-----------------------------------------------------------------------------
// VarData
//-----------------------------------------------------------------------------
Anp::VarData::VarData():
  var    (Cut::NONE),
  type   (kNone),
  valid  (false),
  val_dbl(0.0),
  val_flt(0.0),
  val_int(0),
  val_snt(0),
  val_lnt(0),
  val_unt(0),
  val_u64(0),
  val_uln(0),
  val_bln(false),
  val_chr(0)
{
}


//-----------------------------------------------------------------------------
void Anp::VarData::Clear()
{
  val_dbl = 0.0;
  val_flt = 0.0;
  val_int = 0;
  val_snt = 0;
  val_lnt = 0;
  val_unt = 0;
  val_u64 = 0;
  val_uln = 0;
  val_bln = false;
  val_chr = 0;
}

//-----------------------------------------------------------------------------
void Anp::VarData::Print() const
{
  cout << "   key=" << key << " var=" << Cut::Convert2Str(var) << endl;
}

//-----------------------------------------------------------------------------
double Anp::VarData::GetVal() const
{
  if     (type == kDouble ) return static_cast<double>(val_dbl);
  else if(type == kFloat  ) return static_cast<double>(val_flt);
  else if(type == kInt    ) return static_cast<double>(val_int);
  else if(type == kShort  ) return static_cast<double>(val_snt);
  else if(type == kLong   ) return static_cast<double>(val_lnt);
  else if(type == kUInt   ) return static_cast<double>(val_unt);
  else if(type == kULong64) return static_cast<double>(val_u64);
  else if(type == kULong  ) return static_cast<double>(val_uln);
  else if(type == kBool   ) return static_cast<double>(val_bln);
  else if(type == kChar   ) return static_cast<double>(val_chr);

  cout << "VarData::GetVal - using undefined type" << endl;
  return 0.0;
}

//-----------------------------------------------------------------------------
bool Anp::VarData::SetVarBranch(TTree *tree, bool debug)
{
  //
  // Init branch 
  //
  TBranch *b = tree->GetBranch(branch.c_str());
  if(!b) {
    if(debug) {
      cout << "VarData::SetVarBranch - missing branch: " << branch << endl;
    }

    return false;
  }

  tree->SetBranchStatus(branch.c_str(), 1);

  if(b->GetNleaves() != 1) {
    cout << "VarData::SetVarBranch - branch has more than 1 leaf: " << branch << endl;
    b->Print();
    return false;   
  }
  
  TLeaf *l = 0;

  if(b->GetListOfLeaves()) {
    l = dynamic_cast<TLeaf *>(b->GetListOfLeaves()->First());
  }

  if(!l) {
    cout << "VarData::SetVarBranch - missing leaf: " << branch << endl;
    b->Print();
    return false;
  }

  //
  // Determine type and set branch address
  //
  type = GetType(l->GetTypeName());

  if(debug) {
    cout << "VarData::SetVarBranch - " << setw(10) << std::left << l->GetTypeName() 
	 << "\"" << branch << "\""  << endl;
  }
  
  valid = true;

  if     (type == kDouble ) return SetBranch(tree, branch, val_dbl);
  else if(type == kFloat  ) return SetBranch(tree, branch, val_flt);
  else if(type == kInt    ) return SetBranch(tree, branch, val_int);
  else if(type == kShort  ) return SetBranch(tree, branch, val_snt);
  else if(type == kLong   ) return SetBranch(tree, branch, val_lnt);
  else if(type == kUInt   ) return SetBranch(tree, branch, val_unt);
  else if(type == kULong64) return SetBranch(tree, branch, val_u64);
  else if(type == kULong  ) return SetBranch(tree, branch, val_uln);
  else if(type == kBool   ) return SetBranch(tree, branch, val_bln);
  else if(type == kChar   ) return SetBranch(tree, branch, val_chr);

  valid = false;

  cout << "VarData::SetVarBranch - using undefined type: " << l->GetTypeName() << endl;
  return false;
}

//-----------------------------------------------------------------------------
Anp::VarData::Type Anp::VarData::GetType(const std::string &t) const
{
  //
  // Convert known type strings to enum
  //

  if     (t == "Double_t" ) return kDouble;
  else if(t == "Float_t"  ) return kFloat;
  else if(t == "Int_t"    ) return kInt;
  else if(t == "Short_t"  ) return kShort;
  else if(t == "Long_t"   ) return kLong;
  else if(t == "UInt_t"   ) return kUInt;
  else if(t == "ULong64_t") return kULong64;
  else if(t == "ULong_t"  ) return kULong;
  else if(t == "Bool_t"   ) return kBool;
  else if(t == "Char_t"   ) return kChar;

  cout << "VarData::GetType - unknown type: " << t << endl;
  return kNone;
}

//-----------------------------------------------------------------------------
std::string Anp::VarData::AsStr(VarData::Type t) const
{
  //
  // Convert known type strings to enum
  //
  switch(t) 
    {
    case kDouble : return "Double";
    case kFloat  : return "Float";
    case kInt    : return "Int";
    case kShort  : return "Short";
    case kLong   : return "Long";
    case kUInt   : return "UInt";
    case kULong64: return "ULong64";
    case kULong  : return "ULong";
    case kBool   : return "Bool";
    case kChar   : return "Char";
    case kNone   : return "None";
    default:      break;
    }
  
  cout << "VarData::GetType - unknown type: " << t << endl;
  return "None";
}

//-----------------------------------------------------------------------------
// VecData
//-----------------------------------------------------------------------------
Anp::VecData::VecData():
  var    (Cut::NONE),
  debug  (false),
  vec_dbl(0),
  vec_flt(0),
  vec_int(0),
  vec_snt(0),
  vec_lnt(0),
  vec_unt(0),
  vec_u64(0),
  vec_uln(0),
  vec_bln(0),
  vec_chr(0),
  vvv_int(0),
  vvv_flt(0)
{
}

//-----------------------------------------------------------------------------
void Anp::VecData::Clear()
{
  if(vec_dbl) vec_dbl->clear();
  if(vec_flt) vec_flt->clear();
  if(vec_int) vec_int->clear();
  if(vec_snt) vec_snt->clear();
  if(vec_lnt) vec_lnt->clear();
  if(vec_unt) vec_unt->clear();
  if(vec_u64) vec_u64->clear();
  if(vec_uln) vec_uln->clear();
  if(vec_bln) vec_bln->clear();
  if(vec_chr) vec_chr->clear();

  if(vvv_int) vvv_int->clear();
  if(vvv_flt) vvv_flt->clear();
}

//-----------------------------------------------------------------------------
void Anp::VecData::Delete()
{
  if(vec_dbl) delete vec_dbl;
  if(vec_flt) delete vec_flt;
  if(vec_int) delete vec_int;
  if(vec_snt) delete vec_snt;
  if(vec_lnt) delete vec_lnt;
  if(vec_unt) delete vec_unt;
  if(vec_u64) delete vec_u64;
  if(vec_uln) delete vec_uln;
  if(vec_bln) delete vec_bln;
  if(vec_chr) delete vec_chr;

  if(vvv_int) delete vvv_int;
  if(vvv_flt) delete vvv_flt;
}

//-----------------------------------------------------------------------------
unsigned Anp::VecData::GetSize() const
{
  if(vec_dbl) return vec_dbl->size();
  if(vec_flt) return vec_flt->size();
  if(vec_int) return vec_int->size();
  if(vec_snt) return vec_snt->size();
  if(vec_lnt) return vec_lnt->size();
  if(vec_unt) return vec_unt->size();
  if(vec_u64) return vec_u64->size();
  if(vec_uln) return vec_uln->size();
  if(vec_bln) return vec_bln->size();
  if(vec_chr) return vec_chr->size();

  if(vvv_int) return vvv_int->size();
  if(vvv_flt) return vvv_flt->size();

  return 0;
}

//-----------------------------------------------------------------------------
bool Anp::VecData::IsVector() const
{
  return bool(vvv_int) || bool(vvv_flt);
}

//-----------------------------------------------------------------------------
double Anp::VecData::GetVal(unsigned index) const
{
  if(vec_dbl && index < vec_dbl->size()) return static_cast<double>(vec_dbl->at(index));
  if(vec_flt && index < vec_flt->size()) return static_cast<double>(vec_flt->at(index));
  if(vec_int && index < vec_int->size()) return static_cast<double>(vec_int->at(index));
  if(vec_snt && index < vec_snt->size()) return static_cast<double>(vec_snt->at(index));
  if(vec_lnt && index < vec_lnt->size()) return static_cast<double>(vec_lnt->at(index));
  if(vec_unt && index < vec_unt->size()) return static_cast<double>(vec_unt->at(index));
  if(vec_u64 && index < vec_u64->size()) return static_cast<double>(vec_u64->at(index));
  if(vec_uln && index < vec_uln->size()) return static_cast<double>(vec_uln->at(index));
  if(vec_bln && index < vec_bln->size()) return static_cast<double>(vec_bln->at(index));
  if(vec_chr && index < vec_chr->size()) return static_cast<double>(vec_chr->at(index));

  cout << "VecData::GetVal - no valid vector" << branch << "/" << key << endl;
  return 0;
}

//-----------------------------------------------------------------------------
void Anp::VecData::Print() const
{
  cout << "   key=" << key << " var=" << Cut::Convert2Str(var) << endl;
}

//-----------------------------------------------------------------------------
bool Anp::VecData::SetVarBranch(TTree *tree)
{
  //
  // Init branch 
  //
  TBranch *b = tree->GetBranch(branch.c_str());
  if(!b) {
    return false;
  }
  if(b->GetNleaves() != 1) {
    cout << "VecData::SetVarBranch - branch has more than 1 leaf: " << branch << endl;
    b->Print();
    return false;   
  }
  
  TLeaf *l = 0;

  if(b->GetListOfLeaves()) {
    l = dynamic_cast<TLeaf *>(b->GetListOfLeaves()->First());
  }

  if(!l) {
    cout << "VecData::SetVarBranch - missing leaf: " << branch << endl;
    b->Print();
    return false;
  }

  //
  // Determine type and set branch address
  //
  const Type type = GetType(l->GetTypeName());

  if(false) {
    cout << "VecData::SetVarBranch - " << setw(10) << std::left << l->GetTypeName() 
	 << "\"" << branch << "\""  << endl;
  }

  if(type == kDouble) { 
    vec_dbl = new vector<Double_t>();
    return SetBranch(tree, branch, vec_dbl);
  }
  else if(type == kFloat) { 
    vec_flt = new vector<Float_t>();
    return SetBranch(tree, branch, vec_flt);
  }
  else if(type == kInt) {
    vec_int = new vector<Int_t>();
    return SetBranch(tree, branch, vec_int);
  }
  else if(type == kShort) {
    vec_snt = new vector<Short_t>();
    return SetBranch(tree, branch, vec_snt);
  }
  else if(type == kLong) {
    vec_lnt = new vector<Long_t>();
    return SetBranch(tree, branch, vec_lnt);
  }
  else if(type == kUInt) {
    vec_unt = new vector<UInt_t>();
    return SetBranch(tree, branch, vec_unt);
  }
  else if(type == kULong64) {
    vec_u64 = new vector<ULong64_t>();
    return SetBranch(tree, branch, vec_u64);
  }
  else if(type == kULong) {
    vec_uln = new vector<uint64_t>();
    return SetBranch(tree, branch, vec_uln);
  }
  else if(type == kBool) {
    vec_bln = new vector<Bool_t>();
    return SetBranch(tree, branch, vec_bln);
  }
  else if(type == kChar) {
    vec_chr = new vector<char>();
    return SetBranch(tree, branch, vec_chr);
  }
  else if(type == kVecInt) {
    vvv_int = new vector<vector<int> >();
    return SetBranch(tree, branch, vvv_int);
  }
  else if(type == kVecFloat) {
    vvv_flt = new vector<vector<float> >();
    return SetBranch(tree, branch, vvv_flt);
  }

  cout << "VecData::SetVarBranch - using undefined type" << endl;
  return false;
}

//-----------------------------------------------------------------------------
Anp::VecData::Type Anp::VecData::GetType(const std::string &t) const
{
  //
  // Convert known type strings to enum
  //
  if(t.find("vector") == string::npos) {
    cout << "VecData::GetType - non vector type: " << t << endl;
    return kNone;
  }

  if     (t == "vector<double>"        ) return kDouble;
  else if(t == "vector<float>"         ) return kFloat;
  else if(t == "vector<int>"           ) return kInt;
  else if(t == "vector<short>"         ) return kShort;
  else if(t == "vector<unsigned>"      ) return kUInt;
  else if(t == "vector<unsigned int>"  ) return kUInt;
  else if(t == "vector<ULong64_t>"     ) return kULong64;
  else if(t == "vector<ULong_t>"       ) return kULong;
  else if(t == "vector<bool>"          ) return kBool;
  else if(t == "vector<char>"          ) return kChar;
  else if(t == "vector<vector<int> >"  ) return kVecInt;
  else if(t == "vector<vector<float> >") return kVecFloat;

  cout << "VecData::GetType - unknown type: " << t << endl;
  return kNone;
}

//---------------------------------------------------------------------------------------
std::vector<Anp::VarNick> Anp::ReadNicks(const std::string &key, const Registry &reg)
{
  //
  // Read variable nicknames
  //
  vector<string> str_vars;
  reg.Get(key, str_vars);
  
  vector<VarNick> nicks;

  BOOST_FOREACH(const string &val, str_vars) {
    const size_t vpos = val.find(":");

    if(vpos == string::npos) {
      continue;
    }

    VarNick nick;
    nick.nick = val.substr(0, vpos);
    nick.var  = val.substr(vpos+1, val.size());
    nick.key  = Cut::Convert2Var(nick.var);
    
    if(nick.key != Cut::NONE) {
      nicks.push_back(nick);
    }
    else {
      cout << "ReadNicks - unknown value: " << val << endl;
    }
  }

  return nicks;
}
