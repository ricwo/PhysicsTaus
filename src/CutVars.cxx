// C/C++
#include <map>

// Base
#include "PhysicsBase/Registry.h"

// Data
#include "PhysicsData/VarHolder.h"

// Local
#include "PhysicsTaus/CutVars.h"

#define REGISTER_TAU_VAR(VAR) RegisterPhysicsTausVar( VAR, #VAR)

using namespace std;

//======================================================================================================
namespace Anp
{
  namespace Cut
  {
    typedef std::map<Cut::Var, std::string> VarMap;
    
    static VarMap gPhysicsTausVars;

    static bool PhysicsTausVarFlag = RegisterAllPhysicsTausVars();

    static bool gDebugVars = false;
  }
}

//======================================================================================================
bool Anp::Cut::RegisterPhysicsTausVar(Var var, const std::string &name)
{
  for(const VarMap::value_type &v: gPhysicsTausVars) {
    if(name == v.second) {
      cout << "Cut::RegisterPhysicsTausVar - WARNING - variable with this name already exists: var=" << var << ", name=" << name << endl;
      return false;      
    }
  }

  pair<VarMap::iterator, bool> vit = gPhysicsTausVars.insert(VarMap::value_type(var, name));

  if(!vit.second) {
    cout << "Cut::RegisterPhysicsTausVar - WARNING - variable with this key already exists: var=" << var << ", name=" << name << endl;
    return false;
  }

  if(gDebugVars) {
    cout << "Cut::RegisterPhysicsTausVar - DEBUG - add variable: " << var << ", name=" << name << endl;
  }

  return Anp::RegisterVar(var, name);  
}

//======================================================================================================
const std::vector<std::string>& Anp::Cut::GetAllVarNames()
{
  static vector<string> names;

  if(names.empty()) {
    for(const VarMap::value_type &v: gPhysicsTausVars) {
      names.push_back(v.second);
    }
  }

  return names;
}

//======================================================================================================
const std::vector<Anp::Cut::Var>& Anp::Cut::GetAllVarEnums()
{
  static vector<Anp::Cut::Var> enums;

  if(enums.empty()) {
    for(const VarMap::value_type &v: gPhysicsTausVars) {
      enums.push_back(v.first);
    }
  }

  return enums;
}

//======================================================================================================
const std::string& Anp::Cut::Convert2Str(Var var)
{
  const VarMap::const_iterator vit = gPhysicsTausVars.find(var);

  if(vit == gPhysicsTausVars.end()) {
    cout << "Cut::Convert2Str - WARNING - unknown variable: " << var << endl;

    static string novar = "UNKNOWN";
    return novar;
  }
  
  return vit->second;
}

//======================================================================================================
Anp::Cut::Var Anp::Cut::Convert2Var(const std::string &var)
{
  for(const VarMap::value_type &v: gPhysicsTausVars) {
    if(var == v.second) {
      return v.first;
    }
  }

  if(gDebugVars) {
    cout << "Cut::Convert2Var - WARNING - unknown variable: " << var << endl;
  }

  return NONE;
}
//======================================================================================================
Anp::Cut::Var Anp::Cut::Convert2Var(uint32_t key)
{
  const vector<Var> &vars = GetAllVarEnums();
  
  //
  // Find matching enum by value
  //
  for(Var var: vars) {
    if(static_cast<uint32_t>(var) == key) {
      return var;
    }
  }

  cout << "Cut::Convert2Var - WARNING - unknown key: " << key << endl;

  return NONE;
}

//======================================================================================================
Anp::Cut::Var Anp::Cut::ReadVar(const Registry &reg, 
				const std::string &key, 
				const std::string &caller)
{
  //
  // Read vector of variable names and convert to Var enums
  //
  const vector<Var> vars = ReadVars(reg, key, caller);

  if(vars.size() == 1) {
    return vars.front();
  }

  return NONE;
}

//======================================================================================================
std::vector<Anp::Cut::Var> Anp::Cut::ReadVars(const Registry &reg, 
					      const std::string &key, 
					      const std::string &caller)
{
  //
  // Read vector of variable names and convert to Var enums
  //
  vector<string> keys;
  reg.Get(key, keys);

  vector<Var> vars;  

  for(unsigned i = 0; i < keys.size(); ++i) {
    const Var var = Cut::Convert2Var(keys.at(i));
    if(var != NONE) {
      vars.push_back(var);
    }
    else {
      cout << caller << " - unknown variable name: " << keys.at(i) << endl;
    }
  }

  return vars;
}

//======================================================================================================
std::set<int> Anp::Cut::ReadVarsAsSet(const Registry &reg, 
				      const std::string &key, 
				      const std::string &caller)
{
  //
  // Read vector of variable names and convert to Var enums
  //
  vector<string> keys;
  reg.Get(key, keys);

  set<int> vars;  
  
  for(unsigned i = 0; i < keys.size(); ++i) {
    const Var var = Cut::Convert2Var(keys.at(i));
    if(var != NONE) {
      vars.insert(static_cast<unsigned>(var));
    }
    else {
      cout << caller << " - unknown variable name: " << keys.at(i) << endl;
    }
  }

  return vars;
}

//======================================================================================================
std::vector<Anp::Cut::Var> Anp::Cut::ReadVars(const std::string &config, 
					      const std::string &caller)
{
  //
  // Read vector of variable names and convert to Var enums
  //
  vector<string> keys;
  Anp::StringTok(keys, config, ", ");

  vector<Var> vars;  

  for(unsigned i = 0; i < keys.size(); ++i) {
    const Var var = Cut::Convert2Var(keys.at(i));
    if(var != NONE) {
      vars.push_back(var);
    }
    else {
      cout << caller << " - unknown variable name: " << keys.at(i) << endl;
    }
  }

  return vars;
}

//======================================================================================================
void Anp::Cut::PrintCutNames()
{
  const vector<Var> vars = Cut::GetAllVarEnums();
  
  cout << "PrintCutNames - print " << vars.size() << " enum(s)" << endl;
  
  for(unsigned i = 0; i < vars.size(); ++i) {
    cout << "   " << setw(10) << vars.at(i) << ": " << Cut::AsStr(vars.at(i)) << endl;
  }
}

//======================================================================================================
const std::string& Anp::Cut::AsStr(Var var)
{
  return Convert2Str(var);
}

//======================================================================================================
std::string Anp::Cut::AsStr(uint32_t key, double val)
{
  stringstream s;

  const Var var = Convert2Var(key);
  
  if(var != NONE) {
    s << AsStr(var) << ": " << val;
  }
  else {
    s <<       var  << ": " << val;
  }

  return s.str();
}

//======================================================================================================
bool Anp::Cut::RegisterAllPhysicsTausVars()
{
  // 1/6/16 define new variables for phistar eta etc
  REGISTER_TAU_VAR( PhiStarEta );
  REGISTER_TAU_VAR( PhiAcop );
  REGISTER_TAU_VAR( ThetaStarEta );
  REGISTER_TAU_VAR( Mratio );
  REGISTER_TAU_VAR( DPhi );
  REGISTER_TAU_VAR( Ptl0 );
  REGISTER_TAU_VAR( Ptl1 );
  REGISTER_TAU_VAR( Ptcomb );
  //REGISTER_TAU_VAR( Mratio_PhiStarEta );


  REGISTER_TAU_VAR( NONE );
  REGISTER_TAU_VAR( Charge );
  REGISTER_TAU_VAR( Et );
  REGISTER_TAU_VAR( Eta );
  REGISTER_TAU_VAR( ClPhi );
  REGISTER_TAU_VAR( ClEta );
  REGISTER_TAU_VAR( ClEt );
  REGISTER_TAU_VAR( EtaBE2 );
  REGISTER_TAU_VAR( EtaAbs );
  REGISTER_TAU_VAR( EtaID );
  REGISTER_TAU_VAR( EtaMS );
  REGISTER_TAU_VAR( Energy );
  REGISTER_TAU_VAR( Mass );
  REGISTER_TAU_VAR( Phi );
  REGISTER_TAU_VAR( Theta );
  REGISTER_TAU_VAR( Pt );
  REGISTER_TAU_VAR( Px );
  REGISTER_TAU_VAR( Py );
  REGISTER_TAU_VAR( Pz );
  REGISTER_TAU_VAR( ClusterPt );
  REGISTER_TAU_VAR( ClusterEt );
  REGISTER_TAU_VAR( ClusterEta );
  REGISTER_TAU_VAR( ClusterPhi );
  REGISTER_TAU_VAR( PtID );
  REGISTER_TAU_VAR( PtMS );
  REGISTER_TAU_VAR( PtOrg );
  REGISTER_TAU_VAR( SumPt );
  REGISTER_TAU_VAR( QoverP );
  //  REGISTER_TAU_VAR( MET );

  //test some new ones
  REGISTER_TAU_VAR( MatchTruthLepton );
  REGISTER_TAU_VAR( NTruth );
  REGISTER_TAU_VAR( M_truth_tau_tau );
  REGISTER_TAU_VAR( M_obs_tau_tau );
  REGISTER_TAU_VAR( IsTauPionNuDecay );
  REGISTER_TAU_VAR( NMuonsInDilep );
  REGISTER_TAU_VAR( DeltaRTruthDaughter );
  REGISTER_TAU_VAR( NElecsInDilep );
  REGISTER_TAU_VAR( M_obs_had_had );
  REGISTER_TAU_VAR( HasRecoLinkHadTau );
  REGISTER_TAU_VAR( HasRecoLinkLepTau );
  REGISTER_TAU_VAR( HasRecoLinkHadHad );
  REGISTER_TAU_VAR( NDileps );
  REGISTER_TAU_VAR( NNuElec );
  REGISTER_TAU_VAR( NNuMu );
  REGISTER_TAU_VAR( NDihad );
  REGISTER_TAU_VAR( NDilep );
  REGISTER_TAU_VAR( NSemilep );
  REGISTER_TAU_VAR( NSemilepEl );
  REGISTER_TAU_VAR( NSemilepMu );
  REGISTER_TAU_VAR( NJustMu );
  REGISTER_TAU_VAR( NJustEl );
  REGISTER_TAU_VAR( NMixed );

  RegisterPhysicsTausVar( PreSamplerB,    "PreSamplerB");
  RegisterPhysicsTausVar( EMB1,           "EMB1");
  RegisterPhysicsTausVar( EMB2,           "EMB2");
  RegisterPhysicsTausVar( EMB3,           "EMB3");
  RegisterPhysicsTausVar( TileBar0,       "TileBar0");
  RegisterPhysicsTausVar( TileBar1,       "TileBar1");
  RegisterPhysicsTausVar( TileBar2,       "TileBar2");

  RegisterPhysicsTausVar( RecoBarcode,            "RecoBarcode");
  RegisterPhysicsTausVar( TrueBarcode,            "TrueBarcode");
  RegisterPhysicsTausVar( TrueBarcodeGeantElec,   "TrueBarcodeGeantElec");
  RegisterPhysicsTausVar( TrueBarcodeGeantPhoton, "TrueBarcodeGeantPhoton");
  RegisterPhysicsTausVar( TrackBarcode,           "TrackBarcode");

  RegisterPhysicsTausVar( Author,         "Author");
  RegisterPhysicsTausVar( Authors,        "Authors");
  RegisterPhysicsTausVar( AuthorMuonBoy,  "AuthorMuonBoy");
  RegisterPhysicsTausVar( AuthorStaco,    "AuthorStaco");
  RegisterPhysicsTausVar( AuthorMuTag,    "AuthorMuTag");
  RegisterPhysicsTausVar( AuthorMuGirl,   "AuthorMuGirl");
  RegisterPhysicsTausVar( BitCombined,    "BitCombined");
  RegisterPhysicsTausVar( BitStandAlone,  "BitStandAlone");
  RegisterPhysicsTausVar( BitTight,       "BitTight");
  RegisterPhysicsTausVar( BitTightPP,     "BitTightPP");
  RegisterPhysicsTausVar( BitMedium,      "BitMedium");
  RegisterPhysicsTausVar( BitMediumPP,    "BitMediumPP");
  RegisterPhysicsTausVar( BitLoose,       "BitLoose");
  RegisterPhysicsTausVar( BitLoosePP,     "BitLoosePP");
  RegisterPhysicsTausVar( BitFEB,         "BitFEB");
  RegisterPhysicsTausVar( BitOQ,          "BitOQ");
  RegisterPhysicsTausVar( BitRefFinal,    "BitRefFinal");
  RegisterPhysicsTausVar( BitLocHadTopo,  "BitLocHadTopo");

  RegisterPhysicsTausVar( IsoGradient,        "IsoGradient");
  RegisterPhysicsTausVar( IsoGradientLoose,   "IsoGradientLoose");
  RegisterPhysicsTausVar( IsoLoose,           "IsoLoose");
  RegisterPhysicsTausVar( IsoLooseTrackOnly,  "IsoLooseTrackOnly");
  RegisterPhysicsTausVar( IsoTight,           "IsoTight");

  RegisterPhysicsTausVar( ElecConv,       "ElecConv");
  RegisterPhysicsTausVar( isLooseBLPixLH, "isLooseBLPixLH");
  RegisterPhysicsTausVar( isLooseLH,      "isLooseLH");
  RegisterPhysicsTausVar( isLoosePP,      "isLoosePP");
  RegisterPhysicsTausVar( isMediumLH,     "isMediumLH");
  RegisterPhysicsTausVar( isMediumPP,     "isMediumPP");
  RegisterPhysicsTausVar( isTightLH,      "isTightLH");
  RegisterPhysicsTausVar( NTrigMuon,      "NTrigMuon");
  RegisterPhysicsTausVar( isTightPP,      "isTightPP");
  RegisterPhysicsTausVar( isVeryLooseLH,  "isVeryLooseLH");
  RegisterPhysicsTausVar( isVeryTightLH,  "isVeryTightLH");

  RegisterPhysicsTausVar( Match_EF_e12Tvh_loose1,  "Match_EF_e12Tvh_loose1");
  RegisterPhysicsTausVar( Match_EF_e22vh_medium1,  "Match_EF_e22vh_medium1");
  RegisterPhysicsTausVar( Match_EF_e24vhi_medium1, "Match_EF_e24vhi_medium1");
  RegisterPhysicsTausVar( Match_EF_e45_medium1,    "Match_EF_e45_medium1");
  RegisterPhysicsTausVar( Match_EF_e60_medium1,    "Match_EF_e60_medium1");
    
    
  RegisterPhysicsTausVar( EF_e24vhi_medium1,      "EF_e24vhi_medium1");
  RegisterPhysicsTausVar( EF_e60_medium1,         "EF_e60_medium1");
  RegisterPhysicsTausVar( EF_mu24i_tight,         "EF_mu24i_tight");
  RegisterPhysicsTausVar( EF_mu36_tight,          "EF_mu36_tight");

  RegisterPhysicsTausVar( HLT_mu4,                "HLT_mu4");
  RegisterPhysicsTausVar( HLT_mu4_msonly,         "HLT_mu4_msonly");
  RegisterPhysicsTausVar( HLT_mu6,                "HLT_mu6");
  RegisterPhysicsTausVar( HLT_mu6_msonly,         "HLT_mu6_msonly");
  RegisterPhysicsTausVar( HLT_mu10,               "HLT_mu10");
  RegisterPhysicsTausVar( HLT_mu10_msonly,        "HLT_mu10_msonly");
  RegisterPhysicsTausVar( HLT_mu14,               "HLT_mu14");
  RegisterPhysicsTausVar( HLT_mu18,               "HLT_mu18");
  RegisterPhysicsTausVar( HLT_mu20,               "HLT_mu20");
  RegisterPhysicsTausVar( HLT_mu20_iloose_L1MU15, "HLT_mu20_iloose_L1MU15");
  RegisterPhysicsTausVar( HLT_mu20_L1MU15,        "HLT_mu20_L1MU15");
  RegisterPhysicsTausVar( HLT_mu20_msonly,        "HLT_mu20_msonly");
  RegisterPhysicsTausVar( HLT_mu22,               "HLT_mu22");
  RegisterPhysicsTausVar( HLT_mu24,               "HLT_mu24");
  RegisterPhysicsTausVar( HLT_mu24_iloose_L1MU15, "HLT_mu24_iloose_L1MU15");
  RegisterPhysicsTausVar( HLT_mu24_imedium,       "HLT_mu24_imedium");
  RegisterPhysicsTausVar( HLT_mu24_L1MU15,        "HLT_mu24_L1MU15");
  RegisterPhysicsTausVar( HLT_mu26,               "HLT_mu26");
  RegisterPhysicsTausVar( HLT_mu26_imedium,       "HLT_mu26_imedium");
  RegisterPhysicsTausVar( HLT_mu50,               "HLT_mu50");

  RegisterPhysicsTausVar( HLT_noalg_L1MU4,        "HLT_noalg_L1MU4");
  RegisterPhysicsTausVar( HLT_noalg_L1MU6,        "HLT_noalg_L1MU6");
  RegisterPhysicsTausVar( HLT_noalg_L1MU10,       "HLT_noalg_L1MU10");
  RegisterPhysicsTausVar( HLT_noalg_L1MU15,       "HLT_noalg_L1MU15");
  RegisterPhysicsTausVar( HLT_noalg_L1MU20,       "HLT_noalg_L1MU20");

  RegisterPhysicsTausVar( HLT_e24_lhmedium_L1EM18VH,         "HLT_e24_lhmedium_L1EM18VH");
  RegisterPhysicsTausVar( HLT_e24_lhmedium_L1EM20VH,         "HLT_e24_lhmedium_L1EM20VH");
  RegisterPhysicsTausVar( HLT_e24_lhmedium_iloose_L1EM18VH,  "HLT_e24_lhmedium_iloose_L1EM18VH");
  RegisterPhysicsTausVar( HLT_e24_tight_iloose,	      "HLT_e24_tight_iloose");
  RegisterPhysicsTausVar( HLT_e24_lhtight_iloose, 	      "HLT_e24_lhtight_iloose");
  RegisterPhysicsTausVar( HLT_e24_tight_iloose_L1EM20VH,     "HLT_e24_tight_iloose_L1EM20VH");
  RegisterPhysicsTausVar( HLT_e24_lhtight_iloose_L1EM20VH,   "HLT_e24_lhtight_iloose_L1EM20VH");
  RegisterPhysicsTausVar( HLT_e26_tight_iloose,	      "HLT_e26_tight_iloose");
  RegisterPhysicsTausVar( HLT_e26_lhtight_iloose,	      "HLT_e26_lhtight_iloose");
  RegisterPhysicsTausVar( HLT_e60_medium,		      "HLT_e60_medium");
  RegisterPhysicsTausVar( HLT_e60_lhmedium, 		      "HLT_e60_lhmedium");
  RegisterPhysicsTausVar( HLT_e120_lhloose,		      "HLT_e120_lhloose");
  RegisterPhysicsTausVar( HLT_e140_loose,		      "HLT_e140_loose");
  RegisterPhysicsTausVar( HLT_e140_lhloose,                  "HLT_e140_lhloose");
  RegisterPhysicsTausVar( dsid,				     "dsid");
      
  RegisterPhysicsTausVar( L1_MU4,                            "L1_MU4");
  RegisterPhysicsTausVar( L1_MU6,                            "L1_MU6");
  RegisterPhysicsTausVar( L1_MU10,                           "L1_MU10");
  RegisterPhysicsTausVar( L1_MU15,                           "L1_MU15");
  RegisterPhysicsTausVar( L1_MU20,                           "L1_MU20");

  RegisterPhysicsTausVar( match_HLT_e24_lhmedium_L1EM18VH,               "match_HLT_e24_lhmedium_L1EM18VH");           
  RegisterPhysicsTausVar( match_HLT_e24_lhmedium_L1EM20VH,		  "match_HLT_e24_lhmedium_L1EM20VH");		   
  RegisterPhysicsTausVar( match_HLT_e24_lhmedium_iloose_L1EM18VH,	  "match_HLT_e24_lhmedium_iloose_L1EM18VH");
  RegisterPhysicsTausVar( match_HLT_e24_lhmedium_nod0_L1EM20VH,		  "match_HLT_e24_lhmedium_nod0_L1EM20VH");
  RegisterPhysicsTausVar( match_HLT_e24_tight_iloose,		          "match_HLT_e24_tight_iloose");		   
  RegisterPhysicsTausVar( match_HLT_e24_lhtight_iloose, 		  "match_HLT_e24_lhtight_iloose");		   
  RegisterPhysicsTausVar( match_HLT_e24_lhtight_nod0_ivarloose,	          "match_HLT_e24_lhtight_nod0_ivarloose");	   
  RegisterPhysicsTausVar( match_HLT_e24_tight_iloose_L1EM20VH,	          "match_HLT_e24_tight_iloose_L1EM20VH");	   
  RegisterPhysicsTausVar( match_HLT_e24_lhtight_iloose_L1EM20VH, 	  "match_HLT_e24_lhtight_iloose_L1EM20VH");	   
  RegisterPhysicsTausVar( match_HLT_e26_tight_iloose,		          "match_HLT_e26_tight_iloose");		   
  RegisterPhysicsTausVar( match_HLT_e26_lhtight_iloose,		  "match_HLT_e26_lhtight_iloose");		   
  RegisterPhysicsTausVar( match_HLT_e26_lhtight_nod0_ivarloose,		  "match_HLT_e26_lhtight_nod0_ivarloose");		   
  RegisterPhysicsTausVar( match_HLT_e60_medium,			  "match_HLT_e60_medium");			   
  RegisterPhysicsTausVar( match_HLT_e60_lhmedium, 			  "match_HLT_e60_lhmedium");			   
  RegisterPhysicsTausVar( match_HLT_e60_lhmedium_nod0, 			  "match_HLT_e60_lhmedium_nod0");
  RegisterPhysicsTausVar( match_HLT_e120_lhloose,			  "match_HLT_e120_lhloose");			   
  RegisterPhysicsTausVar( match_HLT_e140_loose,			  "match_HLT_e140_loose");			   
  RegisterPhysicsTausVar( match_HLT_e140_lhloose, 			  "match_HLT_e140_lhloose");			   
  RegisterPhysicsTausVar( match_HLT_e140_lhloose_nod0, 			  "match_HLT_e140_lhloose_nod0");			   
  RegisterPhysicsTausVar( match_HLT_mu20_iloose_L1MU15,		  "match_HLT_mu20_iloose_L1MU15");		   
  RegisterPhysicsTausVar( match_HLT_mu24_imedium,			  "match_HLT_mu24_imedium");			   
  RegisterPhysicsTausVar( match_HLT_mu24_ivarmedium,			  "match_HLT_mu24_ivarmedium");			   
  RegisterPhysicsTausVar( match_HLT_mu24_iloose,			  "match_HLT_mu24_iloose");
  RegisterPhysicsTausVar( match_HLT_mu24_iloose_L1MU15,			  "match_HLT_mu24_iloose_L1MU15");
  RegisterPhysicsTausVar( match_HLT_mu24_ivarloose, 			  "match_HLT_mu24_ivarloose");			   			   
  RegisterPhysicsTausVar( match_HLT_mu24_ivarloose_L1MU15, 			  "match_HLT_mu24_ivarloose_L1MU15");			   			   
  RegisterPhysicsTausVar( match_HLT_mu26_imedium, 			  "match_HLT_mu26_imedium");			   
  RegisterPhysicsTausVar( match_HLT_mu26_ivarmedium, 			  "match_HLT_mu26_ivarmedium");			   
  RegisterPhysicsTausVar( match_HLT_mu26_ivarloose, 			  "match_HLT_mu26_ivarloose");			   
  RegisterPhysicsTausVar( match_HLT_mu40,				  "match_HLT_mu40");				   
  RegisterPhysicsTausVar( match_HLT_mu50,				  "match_HLT_mu50");
  RegisterPhysicsTausVar( match_HLT_mu60_0eta105_msonly,		  "match_HLT_mu60_0eta105_msonly");
  RegisterPhysicsTausVar( match_HLT_2e12_loose_L12EM10VH,		  "match_HLT_2e12_loose_L12EM10VH");		   
  RegisterPhysicsTausVar( match_HLT_2e12_lhloose_L12EM10VH,	          "match_HLT_2e12_lhloose_L12EM10VH");	   
  RegisterPhysicsTausVar( match_HLT_e17_lhloose_2e9_lhloose,	          "match_HLT_e17_lhloose_2e9_lhloose");	   
  RegisterPhysicsTausVar( match_HLT_2mu14,				  "match_HLT_2mu14");				   
  RegisterPhysicsTausVar( match_HLT_2mu10,				  "match_HLT_2mu10");				   
  RegisterPhysicsTausVar( match_HLT_mu18_mu8noL1,			  "match_HLT_mu18_mu8noL1");			   
  RegisterPhysicsTausVar( match_HLT_e17_loose_mu14,		          "match_HLT_e17_loose_mu14");		   
  RegisterPhysicsTausVar( match_HLT_e17_lhloose_mu14, 		          "match_HLT_e17_lhloose_mu14");		   
  RegisterPhysicsTausVar( match_HLT_e7_medium_mu24,		          "match_HLT_e7_medium_mu24");		   
  RegisterPhysicsTausVar( match_HLT_e7_lhmedium_mu24,		          "match_HLT_e7_lhmedium_mu24");		   
  RegisterPhysicsTausVar( match_HLT_mu10,				  "match_HLT_mu10");				   
  RegisterPhysicsTausVar( match_HLT_mu14,				  "match_HLT_mu14");				   
  RegisterPhysicsTausVar( match_HLT_mu18,				  "match_HLT_mu18");				   
  RegisterPhysicsTausVar( match_HLT_mu24,				  "match_HLT_mu24");				   
  RegisterPhysicsTausVar( match_HLT_e17_loose,			          "match_HLT_e17_loose");			   
  RegisterPhysicsTausVar( match_HLT_e17_lhloose,			  "match_HLT_e17_lhloose");			   
  RegisterPhysicsTausVar( match_HLT_e12_loose,			          "match_HLT_e12_loose");			   
  RegisterPhysicsTausVar( match_HLT_e12_lhloose,			  "match_HLT_e12_lhloose");			   
  RegisterPhysicsTausVar( match_HLT_e5_loose,			          "match_HLT_e5_loose");			   
  RegisterPhysicsTausVar( match_HLT_e5_lhloose,                          "match_HLT_e5_lhloose");      
  RegisterPhysicsTausVar( match_HLT_e300_etcut,				 "match_HLT_e300_etcut");
  
  RegisterPhysicsTausVar( isCombined,                        "isCombined");
  RegisterPhysicsTausVar( isLoose,                           "isLoose");
  RegisterPhysicsTausVar( isMedium,                          "isMedium");
  RegisterPhysicsTausVar( isTight,                           "isTight");
  RegisterPhysicsTausVar( momBalSignif,                      "momBalSignif");
  RegisterPhysicsTausVar( numPrecLayers,                     "numPrecLayers");
  RegisterPhysicsTausVar( scatCurvSignif,                    "scatCurvSignif");
  RegisterPhysicsTausVar( scatNeighSignif,                   "scatNeighSignif");

  RegisterPhysicsTausVar( Match_EF_mu13,                     "Match_EF_mu13");
  RegisterPhysicsTausVar( Match_EF_mu18_tight,               "Match_EF_mu18_tight");
  RegisterPhysicsTausVar( Match_EF_mu24_tight,               "Match_EF_mu24_tight");
  RegisterPhysicsTausVar( Match_EF_mu24i_tight,              "Match_EF_mu24i_tight");
  RegisterPhysicsTausVar( Match_EF_mu36_tight,               "Match_EF_mu36_tight");
  RegisterPhysicsTausVar( Match_EF_mu40_MSonly_barrel_tight, "Match_EF_mu40_MSonly_barrel_tight");
  RegisterPhysicsTausVar( Match_EF_mu8_EFFS,                 "Match_EF_mu8_EFFS");      

  RegisterPhysicsTausVar( JVT,                  "JVT");
  RegisterPhysicsTausVar( JVTXF,                "JVTXF");
  RegisterPhysicsTausVar( JVTWeight,            "JVTWeight");
  RegisterPhysicsTausVar( BTagMV2c00,           "BTagMV2c00");
  RegisterPhysicsTausVar( BTagMV2c10,           "BTagMV2c10");
  RegisterPhysicsTausVar( BTagMV2c20,           "BTagMV2c20");
  RegisterPhysicsTausVar( BTagEventWeight,      "BTagEventWeight");
  RegisterPhysicsTausVar( jetFitterComb,        "jetFitterComb");
  RegisterPhysicsTausVar( JetFlavorTruth,       "JetFlavorTruth");
  RegisterPhysicsTausVar( JetFlavorTruthGhost,  "JetFlavorTruthGhost");
  RegisterPhysicsTausVar( isLooseBad,           "isLooseBad");
  RegisterPhysicsTausVar( EtaEM,                "EtaEM");
  RegisterPhysicsTausVar( EMFrac,               "EMFrac");

  RegisterPhysicsTausVar( BDTJetScore,     "BDTJetScore");
  RegisterPhysicsTausVar( BDTEleScore,     "BDTEleScore");
  RegisterPhysicsTausVar( EleBDTLoose,     "EleBDTLoose");
  RegisterPhysicsTausVar( EleBDTMedium,    "EleBDTMedium");
  RegisterPhysicsTausVar( EleBDTTight,     "EleBDTTight"); 
  RegisterPhysicsTausVar( JetBDTSigLoose,  "JetBDTSigLoose");
  RegisterPhysicsTausVar( JetBDTSigMedium, "JetBDTSigMedium");
  RegisterPhysicsTausVar( JetBDTSigTight,  "JetBDTSigTight");
  RegisterPhysicsTausVar( MuonVeto,        "MuonVeto");

  RegisterPhysicsTausVar( IdSF,            "IdSF");
  RegisterPhysicsTausVar( IdSFStat,        "IdSFStat");
  RegisterPhysicsTausVar( IdSFSys,         "IdSFSys");
  RegisterPhysicsTausVar( eVetoSF,         "eVetoSF");
  RegisterPhysicsTausVar( eVetoSFUp,       "eVetoSFUp");
  RegisterPhysicsTausVar( eVetoSFDown,     "eVetoSFDown");
  RegisterPhysicsTausVar( NWideTrack,      "NWideTrack");
  RegisterPhysicsTausVar( passEleOLR,      "passEleOLR");
  RegisterPhysicsTausVar( ChargeIDBDTTight,      "ChargeIDBDTTight");
  RegisterPhysicsTausVar( isTruthMatched,  "isTruthMatched");
  RegisterPhysicsTausVar( isHadronicTau,   "isHadronicTau");

  RegisterPhysicsTausVar( D0,             "D0");
  RegisterPhysicsTausVar( D02D,           "D02D");
  RegisterPhysicsTausVar( D0Sig,          "D0Sig");
  RegisterPhysicsTausVar( Z0,             "Z0");
  RegisterPhysicsTausVar( Z0Sig,          "Z0Sig");
  RegisterPhysicsTausVar( Z0Sin,          "Z0Sin");

  RegisterPhysicsTausVar( ExpectBL,       "ExpectBL");
  RegisterPhysicsTausVar( ExpectIBL,      "ExpectIBL");
  RegisterPhysicsTausVar( BLHit,          "BLHit");
  RegisterPhysicsTausVar( NBL,            "NBL");
  RegisterPhysicsTausVar( NIBL,           "NIBL");
  RegisterPhysicsTausVar( NPIX,           "NPIX");
  RegisterPhysicsTausVar( NPIXHole,       "NPIXHole");
  RegisterPhysicsTausVar( NPIXPlusNDead,  "NPIXPlusNDead");
  RegisterPhysicsTausVar( NSCT,           "NSCT");
  RegisterPhysicsTausVar( NSCTHole,       "NSCTHole");
  RegisterPhysicsTausVar( NSCTPlusNDead,  "NSCTPlusNDead");
  RegisterPhysicsTausVar( NSIL,           "NSIL");
  RegisterPhysicsTausVar( NSILHole,       "NSILHole");
  RegisterPhysicsTausVar( NSILPlusNDead,  "NSILPlusNDead");

  RegisterPhysicsTausVar( NTRT,           "NTRT");
  RegisterPhysicsTausVar( NTRTHL,         "NTRTHL");
  RegisterPhysicsTausVar( NTRTPass,       "NTRTPass");

  RegisterPhysicsTausVar( NTruePromptElec,        "NTruePromptElec");
  RegisterPhysicsTausVar( NTruePromptMuon,        "NTruePromptMuon");
  RegisterPhysicsTausVar( NTruePromptTau,         "NTruePromptTau");
  RegisterPhysicsTausVar( NTruePromptLepton,      "NTruePromptLepton");

  RegisterPhysicsTausVar( NTrueMatchElec,         "NTrueMatchElec");
  RegisterPhysicsTausVar( NTrueMatchMuon,         "NTrueMatchMuon");
  RegisterPhysicsTausVar( NTrueMatchTau,          "NTrueMatchTau");
  RegisterPhysicsTausVar( NTrueMatchLepton,       "NTrueMatchLepton");

  RegisterPhysicsTausVar( NTrueElecWrongSign,     "NTrueElecWrongSign");
  RegisterPhysicsTausVar( NTrueElecRightSign,     "NTrueElecRightSign");
      
  RegisterPhysicsTausVar( MatchTauVisDR,             "MatchTauVisDR");
  RegisterPhysicsTausVar( MatchTauVisDPt,            "MatchTauVisDPt");
  RegisterPhysicsTausVar( MatchTauVisPt,             "MatchTauVisPt");
  RegisterPhysicsTausVar( TruthHadTauMatchIndexPDG,  "TruthHadTauMatchIndexPDG");
  RegisterPhysicsTausVar( TruthHadTauMatchMotherPDG, "TruthHadTauMatchMotherPDG");
  RegisterPhysicsTausVar( TruthHadTauIsHiggs,        "TruthHadTauIsHiggs");
  RegisterPhysicsTausVar( TruthHadTauIsHiggsTauTau,  "TruthHadTauIsHiggsTauTau");
      
  RegisterPhysicsTausVar( MatchRecoLep,   "MatchRecoLep");
  RegisterPhysicsTausVar( MatchRecoPV,    "MatchRecoPV");
  RegisterPhysicsTausVar( MatchRecoPU,    "MatchRecoPU");

  RegisterPhysicsTausVar( HasPromptParent,     "HasPromptParent");
  RegisterPhysicsTausVar( HasBQuarkParent,     "HasBQuarkParent");
  RegisterPhysicsTausVar( HasCQuarkParent,     "HasCQuarkParent");

  
  RegisterPhysicsTausVar( MatchTrigDR,         "MatchTrigDR");
  RegisterPhysicsTausVar( MatchTrigTag,        "MatchTrigTag");
  RegisterPhysicsTausVar( MatchTrigProbe,      "MatchTrigProbe");
  RegisterPhysicsTausVar( MatchTrigBit,        "MatchTrigBit");
  RegisterPhysicsTausVar( MatchTrigNLep,       "MatchTrigNLep");
  RegisterPhysicsTausVar( MatchTrigNLepTight,  "MatchTrigNLepTight");
  RegisterPhysicsTausVar( MatchTrigNElec,      "MatchTrigNElec");
  RegisterPhysicsTausVar( MatchTrigNElecTight, "MatchTrigNElecTight");
  RegisterPhysicsTausVar( MatchTrigNMuon,      "MatchTrigNMuon");
  RegisterPhysicsTausVar( MatchTrigNMuonTight, "MatchTrigNMuonTight");

  
  RegisterPhysicsTausVar( EtCone10,       "EtCone10");
  RegisterPhysicsTausVar( EtCone20,       "EtCone20");
  RegisterPhysicsTausVar( EtCone20Rel,    "EtCone20Rel");
  RegisterPhysicsTausVar( EtCone30,       "EtCone30");
  RegisterPhysicsTausVar( EtCone30Rel,    "EtCone30Rel");
  RegisterPhysicsTausVar( EtCone40,       "EtCone40");
  RegisterPhysicsTausVar( EtCone40Rel,    "EtCone40Rel");

  RegisterPhysicsTausVar( EtCone20OverPt, "EtCone20OverPt");
  RegisterPhysicsTausVar( PtVarCone20OverPt, "PtVarCone20OverPt");
  RegisterPhysicsTausVar( PtVarCone30OverPt, "PtVarCone30OverPt");

  RegisterPhysicsTausVar( OneMuonEvt, "OneMuonEvt");
  RegisterPhysicsTausVar( OneTau, "OneTau");
  RegisterPhysicsTausVar( OppositeCharge, "OppositeCharge");
  RegisterPhysicsTausVar( TauOverlap, "TauOverlap");

  RegisterPhysicsTausVar( PtCone10,       "PtCone10");
  RegisterPhysicsTausVar( PtCone20,       "PtCone20");
  RegisterPhysicsTausVar( PtCone20Rel,    "PtCone20Rel");
  RegisterPhysicsTausVar( PtCone30,       "PtCone30");
  RegisterPhysicsTausVar( PtCone30Rel,    "PtCone30Rel");
  RegisterPhysicsTausVar( PtCone40,       "PtCone40");
  RegisterPhysicsTausVar( PtCone40Rel,    "PtCone40Rel");
  RegisterPhysicsTausVar( PtCone50,       "PtCone50");
  RegisterPhysicsTausVar( PtCone50Rel,    "PtCone50Rel");
  RegisterPhysicsTausVar( PtCone60,       "PtCone60");
  RegisterPhysicsTausVar( PtCone60Rel,    "PtCone60Rel");
  RegisterPhysicsTausVar( PtCone70,       "PtCone70");
  RegisterPhysicsTausVar( PtCone70Rel,    "PtCone70Rel");
  RegisterPhysicsTausVar( PtCone80,       "PtCone80");
  RegisterPhysicsTausVar( PtCone80Rel,    "PtCone80Rel");

  RegisterPhysicsTausVar( EtTopoCone20,     "EtTopoCone20");
  RegisterPhysicsTausVar( EtTopoCone20Rel,  "EtTopoCone20Rel");
  RegisterPhysicsTausVar( EtTopoCone30,     "EtTopoCone30");
  RegisterPhysicsTausVar( EtTopoCone30Rel,  "EtTopoCone30Rel");
  RegisterPhysicsTausVar( EtTopoCone40,     "EtTopoCone40");
  RegisterPhysicsTausVar( EtTopoCone40Rel,  "EtTopoCone40Rel");

  RegisterPhysicsTausVar( PtVarCone20,      "PtVarCone20");
  RegisterPhysicsTausVar( PtVarCone20Rel,   "PtVarCone20Rel");
  RegisterPhysicsTausVar( PtVarCone30,      "PtVarCone30");
  RegisterPhysicsTausVar( PtVarCone30Rel,   "PtVarCone30Rel");
  RegisterPhysicsTausVar( PtVarCone40,      "PtVarCone40");
  RegisterPhysicsTausVar( PtVarCone40Rel,   "PtVarCone40Rel");

  RegisterPhysicsTausVar( isolationLoose,                   "isolationLoose");
  RegisterPhysicsTausVar( isolationLooseTrackOnly,          "isolationLooseTrackOnly");
  RegisterPhysicsTausVar( isolationGradient,                "isolationGradient");
  RegisterPhysicsTausVar( isolationGradientLoose,           "isolationGradientLoose");
  RegisterPhysicsTausVar( isolationFixedCutTight,           "isolationFixedCutTight");
  RegisterPhysicsTausVar( isolationFixedCutTightTrackOnly,  "isolationFixedCutTightTrackOnly");
  RegisterPhysicsTausVar( isolationFixedCutLoose,           "isolationFixedCutLoose");
      
  RegisterPhysicsTausVar( GroupSize,         "GroupSize");
  RegisterPhysicsTausVar( Layer,             "Layer");
  RegisterPhysicsTausVar( TimeOverT,         "TimeOverT");

  RegisterPhysicsTausVar( HasCharge,         "HasCharge");
  RegisterPhysicsTausVar( IsStable,          "IsStable");
  RegisterPhysicsTausVar( IsLepton,          "IsLepton");
  RegisterPhysicsTausVar( IsHadron,          "IsHadron");
  RegisterPhysicsTausVar( IsObject,          "IsObject");
  RegisterPhysicsTausVar( IsChargeFlip,      "IsChargeFlip");
  RegisterPhysicsTausVar( IsRealLepton,      "IsRealLepton");
  RegisterPhysicsTausVar( ParentPDG,         "ParentPDG");
  RegisterPhysicsTausVar( MatchTruthParticlePDG,         "MatchTruthParticlePDG");
  RegisterPhysicsTausVar( MatchTruthParticleDR,         "MatchTruthParticleDR");
  RegisterPhysicsTausVar( MatchTruthParticleMuonPDG,         "MatchTruthParticleMuonPDG");
  RegisterPhysicsTausVar( MatchTruthParticleMuonDR,         "MatchTruthParticleMuonDR");
  RegisterPhysicsTausVar( MatchTruthHadronicTau,         "MatchTruthHadronicTau");
  RegisterPhysicsTausVar( MatchTruthLeptonicTau,         "MatchTruthLeptonicTau");
  RegisterPhysicsTausVar( MatchTruthLeptonicTauMuon,         "MatchTruthLeptonicTauMuon");
  RegisterPhysicsTausVar( M_tau_tau,	     "M_tau_tau");
  RegisterPhysicsTausVar( M_mu_tau,	     "M_mu_tau");
  RegisterPhysicsTausVar( IsHadronicTau,     "IsHadronicTau");
  RegisterPhysicsTausVar( PDG,               "PDG");
  RegisterPhysicsTausVar( BIT,               "BIT");
  RegisterPhysicsTausVar( Status,            "Status");
  RegisterPhysicsTausVar( IndexRangeParent,  "IndexRangeParent");
  RegisterPhysicsTausVar( IndexRangeChild,   "IndexRangeChild");
  RegisterPhysicsTausVar( Index,             "Index");
  RegisterPhysicsTausVar( NChild,            "NChild");
  RegisterPhysicsTausVar( NParent,           "NParent");
  RegisterPhysicsTausVar( ListType,          "ListType");
  RegisterPhysicsTausVar( TruthType,         "TruthType");
  RegisterPhysicsTausVar( TruthOrigin,       "TruthOrigin");

  RegisterPhysicsTausVar( Run,                 "Run");
  RegisterPhysicsTausVar( Event,               "Event");
  RegisterPhysicsTausVar( LumiBlock,           "LumiBlock");
  RegisterPhysicsTausVar( bcid,                "bcid");
  RegisterPhysicsTausVar( backgroundFlags,     "backgroundFlags");
  RegisterPhysicsTausVar( NMuon,               "NMuon");
  RegisterPhysicsTausVar( NElec,               "NElec");
  RegisterPhysicsTausVar( NElecChargeMisid,    "NElecChargeMisid");
  RegisterPhysicsTausVar( NElecCentral,        "NElecCentral");
  RegisterPhysicsTausVar( NElecForward,        "NElecForward");
  RegisterPhysicsTausVar( NObj,                "NObj");
  RegisterPhysicsTausVar( NObjElec,            "NObjElec"); 
  RegisterPhysicsTausVar( NObjMuon,            "NObjMuon");
  RegisterPhysicsTausVar( NLepton,             "NLepton");
  RegisterPhysicsTausVar( NLeptonTight,        "NLeptonTight");
  RegisterPhysicsTausVar( NLeptonPair,         "NLeptonPair");
  RegisterPhysicsTausVar( NLeptonPairSFOS,     "NLeptonPairSFOS");
  RegisterPhysicsTausVar( NLeptonPairZCand,    "NLeptonPairZCand");
  RegisterPhysicsTausVar( NZCand,              "NZCand"); 
  RegisterPhysicsTausVar( NCluster,            "NCluster");
  RegisterPhysicsTausVar( NJet,                "NJet");
  RegisterPhysicsTausVar( NJetT,               "NJetT");
  RegisterPhysicsTausVar( MinJetDR,            "MinJetDR");
  RegisterPhysicsTausVar( NJetBTag,            "NJetBTag");

  RegisterPhysicsTausVar( NJetBTagTrueB,       "NJetBTagTrueB");
  RegisterPhysicsTausVar( NJetCombined,        "NJetCombined");
  RegisterPhysicsTausVar( NJetCombinedT,       "NJetCombinedT");
  RegisterPhysicsTausVar( NTau,                "NTau");
  RegisterPhysicsTausVar( NAntiTau,            "NAntiTau");
  RegisterPhysicsTausVar( NTrack,              "NTrack");
  RegisterPhysicsTausVar( NPriVtx,             "NPriVtx");
  RegisterPhysicsTausVar( NRecVtx,             "NRecVtx");
  RegisterPhysicsTausVar( NRecoPileUp,         "NRecoPileUp");
  RegisterPhysicsTausVar( NTrueB,              "NTrueB");
  RegisterPhysicsTausVar( NTrueLep,            "NTrueLep");
  RegisterPhysicsTausVar( NTrueNu,             "NTrueNu");
  RegisterPhysicsTausVar( NTrueTop,            "NTrueTop");
  RegisterPhysicsTausVar( NTrueW,              "NTrueW");
  RegisterPhysicsTausVar( NTrueZ,              "NTrueZ");
  RegisterPhysicsTausVar( NParticle,           "NParticle");
  RegisterPhysicsTausVar( NPair,               "NPair");

  RegisterPhysicsTausVar( EventMu,             "EventMu");
  RegisterPhysicsTausVar( higgsDecayMode,      "higgsDecayMode");
  RegisterPhysicsTausVar( ttbarDecayMode,      "ttbarDecayMode");
  RegisterPhysicsTausVar( ttbarXPt,            "ttbarXPt");

  RegisterPhysicsTausVar( VtxNTrack,           "VtxNTrack");
  RegisterPhysicsTausVar( VtxSumPt,            "VtxSumPt");
  RegisterPhysicsTausVar( VtxSumPt2,           "VtxSumPt2");
  RegisterPhysicsTausVar( VtxIndex,            "VtxIndex");
  RegisterPhysicsTausVar( PosX,                "PosX");
  RegisterPhysicsTausVar( PosY,                "PosY");
  RegisterPhysicsTausVar( PosZ,                "PosZ");
  RegisterPhysicsTausVar( AbsZ,                "AbsZ");
  RegisterPhysicsTausVar( PosR,                "PosR");
  RegisterPhysicsTausVar( VtxX,                "VtxX");
  RegisterPhysicsTausVar( VtxY,                "VtxY");
  RegisterPhysicsTausVar( VtxZ,                "VtxZ");
  RegisterPhysicsTausVar( VtxProd,             "VtxProd");
  RegisterPhysicsTausVar( VtxEnd,              "VtxEnd");
  RegisterPhysicsTausVar( VtxType,             "VtxType");

  RegisterPhysicsTausVar( Lep0Pt,              "Lep0Pt");
  RegisterPhysicsTausVar( Lep1Pt,              "Lep1Pt");
  RegisterPhysicsTausVar( Lep2Pt,              "Lep2Pt");
  RegisterPhysicsTausVar( Lep3Pt,              "Lep3Pt");

  RegisterPhysicsTausVar( Lep0Energy,          "Lep0Energy");
  RegisterPhysicsTausVar( Lep1Energy,          "Lep1Energy");
  RegisterPhysicsTausVar( Lep2Energy,          "Lep2Energy");
  RegisterPhysicsTausVar( Lep3Energy,          "Lep3Energy");
 
  RegisterPhysicsTausVar( Lep0Eta,             "Lep0Eta");
  RegisterPhysicsTausVar( Lep1Eta,             "Lep1Eta");
  RegisterPhysicsTausVar( Lep2Eta,             "Lep2Eta");
  RegisterPhysicsTausVar( Lep3Eta,             "Lep3Eta");
 
  RegisterPhysicsTausVar( Lep0Phi,             "Lep0Phi");
  RegisterPhysicsTausVar( Lep1Phi,             "Lep1Phi");
  RegisterPhysicsTausVar( Lep2Phi,             "Lep2Phi");
  RegisterPhysicsTausVar( Lep3Phi,             "Lep3Phi");

  RegisterPhysicsTausVar( Lep0D0,              "Lep0D0");
  RegisterPhysicsTausVar( Lep1D0,              "Lep1D0");
  RegisterPhysicsTausVar( Lep0Z0,              "Lep0Z0");
  RegisterPhysicsTausVar( Lep1Z0,              "Lep1Z0");

  RegisterPhysicsTausVar( Lep0EtaAbs,          "Lep0EtaAbs");
  RegisterPhysicsTausVar( Lep1EtaAbs,          "Lep1EtaAbs");
  RegisterPhysicsTausVar( Lep2EtaAbs,          "Lep2EtaAbs");
  RegisterPhysicsTausVar( Lep3EtaAbs,          "Lep3EtaAbs");

  RegisterPhysicsTausVar( Lep0PDG,             "Lep0PDG");
  RegisterPhysicsTausVar( Lep1PDG,             "Lep1PDG");
  RegisterPhysicsTausVar( Lep2PDG,             "Lep2PDG");
  RegisterPhysicsTausVar( Lep3PDG,             "Lep3PDG");

  RegisterPhysicsTausVar( Lep0Var,             "Lep0Var");
  RegisterPhysicsTausVar( Lep1Var,             "Lep1Var");
  RegisterPhysicsTausVar( Lep2Var,             "Lep2Var");
  RegisterPhysicsTausVar( Lep3Var,             "Lep3Var");
   
  RegisterPhysicsTausVar( Lep0MatchTruePDG,    "Lep0MatchTruePDG");
  RegisterPhysicsTausVar( Lep1MatchTruePDG,    "Lep1MatchTruePDG");
  RegisterPhysicsTausVar( Lep2MatchTruePDG,    "Lep2MatchTruePDG");
  RegisterPhysicsTausVar( Lep3MatchTruePDG,    "Lep3MatchTruePDG");
   
  RegisterPhysicsTausVar( Lep0MatchTrig,       "Lep0MatchTrig");
  RegisterPhysicsTausVar( Lep1MatchTrig,       "Lep1MatchTrig");
  RegisterPhysicsTausVar( Lep2MatchTrig,       "Lep2MatchTrig");
  RegisterPhysicsTausVar( Lep3MatchTrig,       "Lep3MatchTrig");
      
  RegisterPhysicsTausVar( Lep0EtCone20Rel,     "Lep0EtCone20Rel");
  RegisterPhysicsTausVar( Lep1EtCone20Rel,     "Lep1EtCone20Rel");
  RegisterPhysicsTausVar( Lep2EtCone20Rel,     "Lep2EtCone20Rel");
  RegisterPhysicsTausVar( Lep3EtCone20Rel,     "Lep3EtCone20Rel");

  RegisterPhysicsTausVar( Lep0PtCone20Rel,     "Lep0PtCone20Rel");
  RegisterPhysicsTausVar( Lep1PtCone20Rel,     "Lep1PtCone20Rel");
  RegisterPhysicsTausVar( Lep2PtCone20Rel,     "Lep2PtCone20Rel");
  RegisterPhysicsTausVar( Lep3PtCone20Rel,     "Lep3PtCone20Rel");

  RegisterPhysicsTausVar( Lep0PtCone40Rel,     "Lep0PtCone40Rel");
  RegisterPhysicsTausVar( Lep1PtCone40Rel,     "Lep1PtCone40Rel");
  RegisterPhysicsTausVar( Lep2PtCone40Rel,     "Lep2PtCone40Rel");
  RegisterPhysicsTausVar( Lep3PtCone40Rel,     "Lep3PtCone40Rel");

  RegisterPhysicsTausVar( Lep0D0Sig,           "Lep0D0Sig");
  RegisterPhysicsTausVar( Lep1D0Sig,           "Lep1D0Sig");
  RegisterPhysicsTausVar( Lep2D0Sig,           "Lep2D0Sig");
  RegisterPhysicsTausVar( Lep3D0Sig,           "Lep3D0Sig");

  RegisterPhysicsTausVar( Lep0Z0Sin,           "Lep0Z0Sin");
  RegisterPhysicsTausVar( Lep1Z0Sin,           "Lep1Z0Sin");
  RegisterPhysicsTausVar( Lep2Z0Sin,           "Lep2Z0Sin");
  RegisterPhysicsTausVar( Lep3Z0Sin,           "Lep3Z0Sin");

  RegisterPhysicsTausVar( Mlll,                "Mlll");
  RegisterPhysicsTausVar( Mllll,               "Mllll");
  RegisterPhysicsTausVar( MllEMu,              "MllEMu");
  RegisterPhysicsTausVar( MllZ1,               "MllZ1"); 
  RegisterPhysicsTausVar( MllZ2,               "MllZ2"); 
  RegisterPhysicsTausVar( PtllZ1,              "PtllZ1"); 
  RegisterPhysicsTausVar( PtllZ2,              "PtllZ2"); 
  RegisterPhysicsTausVar( Mllsub,              "Mllsub"); 
  RegisterPhysicsTausVar( Ptllsub,             "Ptllsub"); 
  RegisterPhysicsTausVar( Ptlll,               "Ptlll"); 
  RegisterPhysicsTausVar( Ptll01,              "Ptll01"); 
  RegisterPhysicsTausVar( Ptll02,              "Ptll02"); 
  RegisterPhysicsTausVar( Ptll12,              "Ptll12"); 

  RegisterPhysicsTausVar( HasSFOSPair,         "HasSFOSPair");

  RegisterPhysicsTausVar( ZBestCandM,          "ZBestCandM");
  RegisterPhysicsTausVar( ZBestCandPt,         "ZBestCandPt");
  RegisterPhysicsTausVar( ZBestCandDPhill,     "ZBestCandDPhill");
  RegisterPhysicsTausVar( ZBestCandPDG,        "ZBestCandPDG");

  RegisterPhysicsTausVar( Jet0Pt,              "Jet0Pt");
  RegisterPhysicsTausVar( Jet1Pt,              "Jet1Pt");
  RegisterPhysicsTausVar( Jet2Pt,              "Jet2Pt");
  RegisterPhysicsTausVar( Jet3Pt,              "Jet3Pt");

  RegisterPhysicsTausVar( Jet0Eta,             "Jet0Eta");
  RegisterPhysicsTausVar( Jet1Eta,             "Jet1Eta");
  RegisterPhysicsTausVar( Jet2Eta,             "Jet2Eta");
  RegisterPhysicsTausVar( Jet3Eta,             "Jet3Eta");
    
  RegisterPhysicsTausVar( Tau0Pt,              "Tau0Pt");
  RegisterPhysicsTausVar( Tau1Pt,              "Tau1Pt");
  RegisterPhysicsTausVar( Tau2Pt,              "Tau2Pt");
  RegisterPhysicsTausVar( Tau0Eta,             "Tau0Eta");
  RegisterPhysicsTausVar( Tau1Eta,             "Tau1Eta");
  RegisterPhysicsTausVar( Tau2Eta,             "Tau2Eta");

  RegisterPhysicsTausVar( DEtall,              "DEtall");
  RegisterPhysicsTausVar( DEtajj,              "DEtajj");
  RegisterPhysicsTausVar( DPhill,              "DPhill");
  RegisterPhysicsTausVar( DPhilljj,            "DPhilljj");
  RegisterPhysicsTausVar( DPhillLeadJet,       "DPhillLeadJet");
  RegisterPhysicsTausVar( DPhijj,              "DPhijj");
  RegisterPhysicsTausVar( DRbb,                "DRbb");
  RegisterPhysicsTausVar( DRjb,                "DRjb");
  RegisterPhysicsTausVar( DRjj,                "DRjj");
  RegisterPhysicsTausVar( DRll,                "DRll");
  RegisterPhysicsTausVar( DRlj,                "DRlj");
  RegisterPhysicsTausVar( DRlb,                "DRlb");
  RegisterPhysicsTausVar( DRle,                "DRle");
  RegisterPhysicsTausVar( DRlm,                "DRlm");
  RegisterPhysicsTausVar( DRee,                "DRee");
  RegisterPhysicsTausVar( DRem,                "DRem");
  RegisterPhysicsTausVar( DReb,                "DReb");
  RegisterPhysicsTausVar( DRej,                "DRej");
  RegisterPhysicsTausVar( DRme,                "DRme");
  RegisterPhysicsTausVar( DRmm,                "DRmm");
  RegisterPhysicsTausVar( DRmb,                "DRmb");
  RegisterPhysicsTausVar( DRmj,                "DRmj");
    
  RegisterPhysicsTausVar( MET,                 "MET");
  RegisterPhysicsTausVar( MET_x,               "MET_x");
  RegisterPhysicsTausVar( MET_y,               "MET_y");
  RegisterPhysicsTausVar( MET_phi,             "MET_phi");
  RegisterPhysicsTausVar( MET_sum,             "MET_sum");
  RegisterPhysicsTausVar( MET_Truth_sumet,     "MET_Truth_sumet");
  RegisterPhysicsTausVar( MET_RefFinal_et,     "MET_RefFinal_et");
  RegisterPhysicsTausVar( MET_RefFinal_sumet,     "MET_RefFinal_sumet");
  RegisterPhysicsTausVar( MET_RefFinal_softTrk_et,     "MET_RefFinal_softTrk_et");
  RegisterPhysicsTausVar( MET_RefFinal_phi,    "MET_RefFinal_phi");
  RegisterPhysicsTausVar( MET_Objects_pt,      "MET_Objects_pt");
  RegisterPhysicsTausVar( MET_Objects_phi,     "MET_Objects_phi");
  RegisterPhysicsTausVar( METRel,              "METRel");
  RegisterPhysicsTausVar( MET_RefFinal_softClus_phi,              "MET_RefFinal_softClus_phi");
  RegisterPhysicsTausVar( MET_RefFinal_softClus_et,              "MET_RefFinal_softClus_et");
  

  RegisterPhysicsTausVar( MT,                  "MT");
  RegisterPhysicsTausVar( Mjj,                 "Mjj");
  RegisterPhysicsTausVar( Mll,                 "Mll");
  RegisterPhysicsTausVar( Mll01,               "Mll01");
  RegisterPhysicsTausVar( Mll02,               "Mll02");
  RegisterPhysicsTausVar( Mll03,               "Mll03");
  RegisterPhysicsTausVar( Mll12,               "Mll12");
  RegisterPhysicsTausVar( Mll13,               "Mll13");
  RegisterPhysicsTausVar( Mll23,               "Mll23");
  RegisterPhysicsTausVar( OSLepPair,           "OSLepPair");

  RegisterPhysicsTausVar( Mtt,                 "Mtt");
  RegisterPhysicsTausVar( Ptll,                "Ptll");
  RegisterPhysicsTausVar( Ptth,                "Ptth");
  RegisterPhysicsTausVar( Phill,               "Phill");
  RegisterPhysicsTausVar( Etall,               "Etall");
  RegisterPhysicsTausVar( Pttot,               "Pttot");
  RegisterPhysicsTausVar( CosThetaStar,        "CosThetaStar");

  RegisterPhysicsTausVar( HT,                  "HT");
  RegisterPhysicsTausVar( SumPtLep,            "SumPtLep");
  RegisterPhysicsTausVar( SumPtJet,            "SumPtJet");
  RegisterPhysicsTausVar( SumPtBJet,           "SumPtBJet");
  RegisterPhysicsTausVar( SumJetMV1,           "SumJetMV1");

  RegisterPhysicsTausVar( WCandJet,            "WCandJet");
  RegisterPhysicsTausVar( WCandMjj,            "WCandMjj");
  RegisterPhysicsTausVar( WCandPt,             "WCandPt");
  RegisterPhysicsTausVar( WCandDR,             "WCandDR");

  RegisterPhysicsTausVar( HFOR,                "HFOR");
  RegisterPhysicsTausVar( EventWeight,         "EventWeight");
  RegisterPhysicsTausVar( TruthEventIndex,     "TruthEventIndex");
  RegisterPhysicsTausVar( PUWeight,            "PUWeight");

  RegisterPhysicsTausVar( MCWeight,            "MCWeight");
  RegisterPhysicsTausVar( MCWeightOrg,         "MCWeightOrg");
  RegisterPhysicsTausVar( IsBlinded,           "IsBlinded");
  RegisterPhysicsTausVar( IsDuplicate,         "IsDuplicate");
  RegisterPhysicsTausVar( TopPtReweighting,    "TopPtReweighting");
  RegisterPhysicsTausVar( PassEventCleaning,   "PassEventCleaning");
  RegisterPhysicsTausVar( PassGRL,             "PassGRL");
  RegisterPhysicsTausVar( PassTriggerMatch,    "PassTriggerMatch");
  RegisterPhysicsTausVar( PassOverlap,         "PassOverlap");

  RegisterPhysicsTausVar( XSWeight,             "XSWeight");
  RegisterPhysicsTausVar( PDFWeight,            "PDFWeight");
  RegisterPhysicsTausVar( SysWeight,            "SysWeight");
  RegisterPhysicsTausVar( MCChannel,            "MCChannel");
  RegisterPhysicsTausVar( LepSF0Weight,         "LepSF0Weight");
  RegisterPhysicsTausVar( LepSF1Weight,         "LepSF1Weight");
  RegisterPhysicsTausVar( TrigSFWeight,         "TrigSFWeight");

  RegisterPhysicsTausVar( pileupEventWeight_075,            "pileupEventWeight_075");
  RegisterPhysicsTausVar( pileupEventWeight_080,            "pileupEventWeight_080");
  RegisterPhysicsTausVar( pileupEventWeight_088,            "pileupEventWeight_088");
  RegisterPhysicsTausVar( pileupEventWeight_090,            "pileupEventWeight_090");
  RegisterPhysicsTausVar( pileupEventWeight_094,            "pileupEventWeight_094");
  RegisterPhysicsTausVar( pileupEventWeight_103,            "pileupEventWeight_103");    

  RegisterPhysicsTausVar( pdf_id1,                          "pdf_id1");
  RegisterPhysicsTausVar( pdf_id2,                          "pdf_id2");
  RegisterPhysicsTausVar( pdf_x1,                           "pdf_x1");
  RegisterPhysicsTausVar( pdf_x2,                           "pdf_x2");
  RegisterPhysicsTausVar( pdf_scale,                        "pdf_scale");

  return true;
}

