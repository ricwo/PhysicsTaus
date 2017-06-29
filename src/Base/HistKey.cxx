
// ROOT
#include "TDirectory.h"

// Base
#include "PhysicsBase/HistMan.h"
#include "PhysicsBase/Registry.h"

// Local
#include "PhysicsTaus/CutVars.h"
#include "PhysicsTaus/HistKey.h"
#include "PhysicsTaus/Utils.h"

using namespace std;

//==============================================================================
Anp::HistKey::HistKey():
  fInit2d(false)
{
}

//==============================================================================
Anp::HistKey::~HistKey()
{
}

//==============================================================================
void Anp::HistKey::ConfigHist(const std::string &key, const Registry &reg)
{
  //
  // Read configuration
  //
  reg.Get(key, "DebugHist",   fDebug    = false);
  reg.Get(key, "PrintHist",   fPrint    = false);
  reg.Get(key, "SetSumw2",    fSetSumw2 = false);
  reg.Get(key, "AllVis",      fAllVis   = true);
  reg.Get(key, "HistKey",     fHistKey);
  reg.Get(key, "KeyHist",     fHistKey);
  reg.Get(key, "HistPrefix",  fHistPrefix);

  reg.GetVec(key, "Hist1dVars", fHistKeys1d);
  reg.GetVec(key, "Hist2dVars", fHistKeys2d);

  if(fHistKey.empty()) {
    cout << "HistKey::ConfigHist - missing HistKey with key=" << key << endl;
    reg.Print();
  }

  if(fPrint) {
    cout << "HistKey::ConfigHist - HistKey: " << fHistKey << endl
	 << "   HistPrefix:      " << fHistPrefix        << endl
	 << "   AllVis:          " << fAllVis            << endl
	 << "   Hist1dVars size: " << fHistKeys1d.size() << endl
	 << "   Hist2dVars size: " << fHistKeys2d.size() << endl;
  }

  Init1d();
}

//==============================================================================
void Anp::HistKey::SetDirHist(TDirectory *dir)
{
  for(TH1Map::value_type &h: fTH1Map) {
    if(h.second) {
      h.second->SetDirectory(Anp::GetDir(fHistDir, dir));
    }
  }

  for(TH2Map::value_type &h: fTH2Map) {
    if(h.second.h) {
      h.second.h->SetDirectory(Anp::GetDir(fHistDir, dir));
    }
  }

  for(Hist1dMap::value_type &hv: fHist1dMap) {
    for(Hist1d &h: hv.second) {
      if(h.h) {
	h.h->SetDirectory(Anp::GetDir(fHistDir, dir));
      }
    }
  }
}

//==============================================================================
TH1* Anp::HistKey::MakeHist(uint32_t key, uint32_t offset, const std::string &prefix)
{
  //
  // Find var name which matches this key
  //
  const Cut::Var var = Cut::Convert2Var(key);
  
  if(var == Cut::NONE) {
    //
    // Unknown key - do nothing, use null pointer
    //
    return fTH1Map.insert(TH1Map::value_type(key+offset, 0)).first->second;
  }

  //
  // Create histogram
  //  
  const string vname = Cut::AsStr(var);
  const string hname = fHistPrefix + prefix +vname;

  TH1 *h = Anp::HistMan::Instance().CreateTH1(hname, fHistKey);  
  if(!h) {
    h = Anp::HistMan::Instance().CreateTH1(vname, fHistKey);
  }

  if(h) {
    h->SetDirectory(0);
    h->SetName(hname.c_str());

    if(fSetSumw2) {
      h->Sumw2();
    }
  }
  
  return fTH1Map.insert(TH1Map::value_type(key+offset, h)).first->second;
}

//==============================================================================
TH2* Anp::HistKey::BookHist2d(std::string hname, uint32_t keyX, uint32_t keyY)
{    
  //
  // Book new 2d histogram
  //
  const Cut::Var varX = Cut::Convert2Var(keyX);
  const Cut::Var varY = Cut::Convert2Var(keyY);
  
  if(varX == Cut::NONE || varY == Cut::NONE) {
    //
    // Unknown vars - nothing to do
    //
    cout << "HistKey::BookHist2d - unknown keys" << endl;
    return 0;
  }

  //
  // Create histogram
  //  
  if(hname.empty()) {
    hname = Cut::AsStr(varY)+"_vs_"+Cut::AsStr(varX);
  }

  //
  // Book 2d histogram, first find matching outer map
  //

  TH2Map::iterator hit = fTH2Map.find(hname);
  if(hit != fTH2Map.end()) {
    //
    // This 2d histogram is already booked - nothing to do
    //
    return hit->second.h;
  }

  TH2 *h = Anp::HistMan::Instance().CreateTH2(hname, fHistKey);
  
  if(h) {
    h->SetDirectory(0);
    h->SetName((fHistPrefix + hname).c_str());

    if(fSetSumw2) {
      h->Sumw2();
    }

    Hist2d hist;
    hist.h    = h;
    hist.keyx = keyX;
    hist.keyy = keyY;

    fTH2Map[hname] = hist;
  }
  else {
    if(fDebug) {
      cout << "HistKey::BookHist2d - unknown histogram: " << hname << endl;
    }
  }

  return h;
}

//==============================================================================
void Anp::HistKey::FillHist2d(const VarHolder &vars, double weight)
{
  //
  // Init on 1st call
  //
  if(!fInit2d) {
    fInit2d = true;
    Init2d(vars);
  }

  //
  // Fill 2d histograms
  //
  if(fTH2Map.empty()) {
    if(fDebug) {
      cout << "HistKey::FillHist2d - no 2d histograms booked" << endl;
    }

    return;
  }

  for(const TH2Map::value_type &h: fTH2Map) {
    double var1 = 0.0, var2 = 0.0;
    
    if(vars.GetVar(h.second.keyx, var1) && vars.GetVar(h.second.keyy, var2)) {
      if(!h.second.h) {
	continue;
      }

      TH2 *h2 = h.second.h;

      double xval = var1;
      double yval = var2;

      if(fAllVis) {
	const double xmax = h2->GetXaxis()->GetXmax();
	const double xmin = h2->GetXaxis()->GetXmin();

	const double ymax = h2->GetYaxis()->GetXmax();
	const double ymin = h2->GetYaxis()->GetXmin();       

	bool change = false; 

	const int nbinx = h2->GetXaxis()->GetNbins();
	const int nbiny = h2->GetYaxis()->GetNbins();

	if      (!(var1 > xmin)) { xval = h2->GetXaxis()->GetBinCenter(1);     change = true; }
	else if (!(var1 < xmax)) { xval = h2->GetXaxis()->GetBinCenter(nbinx); change = true; }

	if      (!(var2 > ymin)) { yval = h2->GetYaxis()->GetBinCenter(1);     change = true; }
	else if (!(var2 < ymax)) { yval = h2->GetYaxis()->GetBinCenter(nbiny); change = true; }
	
	if(fDebug && change) {
	  cout << "HistKey::FillHist2d - force point to within visible histogram range: " << endl
	       << "   var1=" << var1 << ",  xval=" << xval << endl
	       << "   var2=" << var2 << ",  yval=" << yval << endl;
	}
      }
      
      h2->Fill(xval, yval, weight);
    }
    else {
      if(fDebug) {
	cout << "HistKey::FillHist2d - " <<  h.first << ": missing variables: " << h.second.keyx << ", " << h.second.keyy << endl;
      }
    }
  }
}

//==============================================================================
void Anp::HistKey::Init1d()
{
  for(const string &key: fHistKeys1d) {
    
    vector<string> vars1d;
    Anp::StringTok(vars1d, key, ":");

    if(vars1d.size() != 2) {
      cout << "HistKey:Init1d: - invalid key=\"" << key << "\"" << endl;	  
      continue;
    }

    const string &hname = vars1d.at(0);
    const Cut::Var var = Cut::Convert2Var(vars1d.at(1));   

    if(var == Cut::NONE) {
      cout << "HistKey:Init1d: - unknown var key=\"" << key << "\"" << endl;	  
      continue;
    }

    //
    // Create histogram
    //  
    TH1 *h = Anp::HistMan::Instance().CreateTH1(hname, fHistKey);  
    if(!h) {
      cout << "HistKey:Init1d: - failed to create TH1 for key=\"" << key << "\"" << endl;
      continue;
    }

    h->SetDirectory(0);
    h->SetName(hname.c_str());
    
    if(fSetSumw2) {
      h->Sumw2();
    }

    Hist1d hist;
    hist.key = var;
    hist.h   = h;

    fHist1dMap[var].push_back(hist);

    if(fPrint) {
      cout << "HistKey::Init1d - book TH1 for hist=" << hname << " var: " << Cut::AsStr(var) << endl;	  	
    }
  }
}

//==============================================================================
void Anp::HistKey::Init2d(const VarHolder &vars)
{
  for(const string &key: fHistKeys2d) {
    
    vector<string> vars2d;
    Anp::StringTok(vars2d, key, ":");

    Cut::Var var0 = Cut::NONE;
    Cut::Var var1 = Cut::NONE;
    string hname;

    if(vars2d.size() == 2) {
      var0 = Cut::Convert2Var(vars2d.at(0));
      var1 = Cut::Convert2Var(vars2d.at(1));   
    }
    else if(vars2d.size() == 3) {
      hname = vars2d.at(0);
      var0  = Cut::Convert2Var(vars2d.at(1));
      var1  = Cut::Convert2Var(vars2d.at(2));    
    }
    else {
      cout << "HistKey::Init2d - failed to parse key=\"" << key << "\"" << endl;
      continue;
    }

    if(var0 != Cut::NONE && var1 != Cut::NONE) {
      
      if(vars.HasKey(var0) && vars.HasKey(var1)) {
	BookHist2d(hname, var0, var1);

	if(fPrint) {
	  cout << "HistKey::Init2d - book TH2 for vars: " << Cut::AsStr(var0) << ", " << Cut::AsStr(var1) << endl;	  	
	}
      }
      else {
	if(fPrint) {
	  cout << "HistKey::Init2d - ignore TH2 for non-existing vars: " << Cut::AsStr(var0) << ", " << Cut::AsStr(var1) << endl;	  		
	}
      }
    }
    else {
      cout << "HistKey:Init2d: - invalid key=\"" << key << "\"" << endl;	  
    }
  }
}
