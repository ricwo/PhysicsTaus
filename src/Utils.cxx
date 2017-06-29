
// C/C++
#include <cmath>
#include <chrono>
#include <thread>

// ROOT
#include "TVector2.h"
#include "TH1.h"
#include "TSystem.h"

// RooFit
#include "RooAbsReal.h"

// Base
#include "PhysicsBase/HistMan.h"
#include "PhysicsData/TruthVtx.h"

// Local
#include "PhysicsTaus/Utils.h"

using namespace std;

//======================================================================================================
TDirectory* Anp::GetRecursiveDir(TDirectory *dir, std::string path)
{
  return Anp::GetDir(dir, path);
}

//======================================================================================================
double Anp::GetDeltaPhi(double phi1, double phi2)
{
  //
  // Compute delta R
  //
  return TVector2::Phi_mpi_pi(phi1-phi2);
}

//======================================================================================================
double Anp::GetDeltaPhi(double x1, double y1, double x2, double y2)
{
  //
  // Compute delta R
  //
  TVector2 v1(x1, y1);
  TVector2 v2(x2, y2);

  return v1.DeltaPhi(v2);
}

//======================================================================================================
double Anp::GetDeltaR(double eta1, double phi1, double eta2, double phi2)
{
  //
  // Compute delta R
  //
  const double d1 = eta1-eta2;
  const double d2 = TVector2::Phi_mpi_pi(phi1-phi2);

  return std::sqrt(d1*d1 + d2*d2);
}

//======================================================================================================
TH1* Anp::GetTH1(const std::string &key, const std::string &dirbase, TDirectory * dir)
{
  //
  // Create TH1 histogram and set directory
  //
  TH1 *h = Anp::HistMan::Instance().CreateTH1(key, dirbase);
  if(!h) {
    std::cerr << "Anp::GetTH1 - missing TH1 for key=" << key << std::endl;
    return 0;
  }
  if(!dir) {
    std::cerr << "Anp::GetTH1 - input directory not valid " << std::endl;
    return 0;
  }

  h->Sumw2();
  Anp::SetDir(h, dir);

  
  return h;
}

//======================================================================================================
std::vector<Anp::Ptr<Anp::TruthPart> > Anp::FindMothers(const Ptr<TruthPart> &t, 
							const std::vector<Ptr<TruthPart> > &truths)
{
  //
  // Find all mothers of this TruthPart
  //
  vector<Ptr<TruthPart> > mothers;

  for(unsigned i = 0; i < truths.size(); ++i) {
    const Ptr<TruthPart> &m = truths.at(i);

    if(m->IsChild(t.ref())) {
      mothers.push_back(m);
    }
  }

  return mothers;
}

//======================================================================================================
void Anp::FindMothersRecursive(const Ptr<TruthPart> &t, 
			       const std::vector<Ptr<TruthPart> > &truths,
			       vector<Ptr<TruthPart> > &mothers)
{
  //
  // Find all mothers of this TruthPart
  //
  for(unsigned i = 0; i < truths.size(); ++i) {
    const Ptr<TruthPart> &m = truths.at(i);

    if(m->IsChild(t.ref())) {
      FindMothersRecursive(m,truths,mothers);
      mothers.push_back(m);
    }
  }
}

//======================================================================================================
std::set<int> Anp::ReadIntSet(const Registry &reg, const std::string key)
{
  vector<int> vals;
  reg.Get(key, vals);

  set<int> sval;
  sval.insert(vals.begin(), vals.end());

  return sval;
}

//======================================================================================================
void Anp::PrintRooAbsReal(const RooAbsReal &pdf, int contents)
{
  pdf.printMultiline(std::cout, contents);
}

//======================================================================================================
std::vector<Anp::Registry> Anp::GetRegistryVec(const std::string &key, const Registry &reg)
{
  vector<string> keys;
  vector<Registry> regs;
  
  if(!reg.Get(key, keys)) {
    cout << "GetRegistryVec - missing key=\"" << key << "\" in Registry:" << endl;
    reg.Print();
    return regs;
  }

  for(const string &k: keys) {
    regs.push_back(Registry());

    if(!reg.Get(k, regs.back())) {
      cout << "GetRegistryVec - missing Registry with key=\"" << k << "\" in Registry:" << endl;
      reg.Print();      
    }
  }

  return regs;
}

//======================================================================================================
vector<pair<double, double> > Anp::DivideByEqualBins(vector<double> &vars, int nbin, double vmin, double vmax)
{
  //
  // Compute nbin itervals that contain equal number of vars
  //
  vector<pair<double, double> > bins;

  if(nbin < 1 || vars.size() < 2) {
    if(vmin < vmax) {
      bins.push_back(pair<double, double>(vmin, vmax));
    }

    return bins;
  }

  std::sort(vars.begin(), vars.end());
  
  if(nbin == 1) {
    if(vmin < vmax) {
      bins.push_back(pair<double, double>(vmin, vmax));
    }
    else {
      bins.push_back(pair<double, double>(vars.front(), vars.back()+1.0e-5));
    }
    return bins;
  }

  int    cbin = 0;
  double cval = vars.front();

  for(unsigned i = 0; i < vars.size(); ++i) {
    int ibin = static_cast<int>(double(nbin*i)/double(vars.size()));

    if(ibin != cbin) {
      const double nval = vars.at(i);

      bins.push_back(pair<double, double>(cval, nval));

      cbin = ibin;
      cval = nval;
    }
    else if(i+1 == vars.size()) {
      ibin++;

      const double nval = vars.at(i);

      bins.push_back(pair<double, double>(cval, nval+1.0e-5));
    }
  }

  if(vmin < vmax && !bins.empty()) {
    bins.front().first  = vmin;
    bins.back ().second = vmax;
  }

  return bins;
}

//======================================================================================================
std::string Anp::PrintPart(const TruthPart &tp)
{
  stringstream str;
 
  str << "(PDG, barcode, status, type, pt [GeV]) = (";
  
  const int pdg = GetInt(Cut::PDG,         tp);
  const int bar = GetInt(Cut::TrueBarcode, tp);
  const int sts = GetInt(Cut::Status,      tp);
  const int ltp = GetInt(Cut::ListType,    tp, -1);
  
  if(pdg >= 0) {
    str << " " << setw(3) << pdg << ", ";
  }
  else {
    str << "-" << setw(3) << std::abs(pdg) << ", ";
  }

  const string spt = Anp::Round2Pair(0.001*tp.GetFourMom().GetPt(), 1.0, 5).first;

  str << setw(7) << bar << ", "
      << setw(2) << sts << ", ";

  if(ltp >= 0) {
    str << " " << setw(1) << ltp << ", ";
  }
  else {
    str << "-" << setw(1) << ltp << ", ";
  }

  str << spt << ")";

  return str.str();
}

//======================================================================================================
std::string Anp::PrintVars(const VarHolder &vh)
{
  stringstream str;
 
  const VarEntryVec &vars = vh.GetVars();

  str << "VarHolder - contains " << vars.size() << " variable(s)" << endl;
  
  for(unsigned i = 0; i < vars.size(); ++i) {
    const VarEntry &v = vars.at(i);
    const Cut::Var var = Cut::Convert2Var(v.key());

    str << "   " << setw(6) << v.key() << " " << setw(30) << Cut::AsStr(var) << " = " << v.data() << endl;
  }

  return str.str();
}

//======================================================================================================
double Anp::GetDPt(const FourMom &tp, const FourMom &lv)
{
  //
  // Compute delta pt between truth and reco
  //
  if(tp.GetPt() > 0.0) {
    return (lv.GetPt() - tp.GetPt())/tp.GetPt();
  }

  return 1.0;
}

//======================================================================================================
std::vector<Anp::Ptr<Anp::TruthPart> > Anp::FindRangeTrueBarcode(Cut::Var key, 
								 const TruthPart &tp, 
								 const std::vector<Ptr<TruthPart> > &tvec)
{
  std::vector<Ptr<TruthPart> > ovec;
  vector<int> ivec;
  
  if(!tp.GetVarVec(key, ivec)) {
    cout << "FindRangeTrueBarcode - missing index vector" << endl;
    return ovec;
  }
  
  for(const Ptr<TruthPart> &ptr: tvec) {
    double value = 0.0;
    int    index = 0;

    if(ptr->GetVar(Cut::TrueBarcode, value)) {
      index = static_cast<int>(value);    

      if(std::find(ivec.begin(), ivec.end(), index) != ivec.end()) {
	ovec.push_back(ptr);
      }
    }
    else {
      cout << "FindRangeTrueBarcode - number of index value(s) = " << ivec.size() << ": ";      
    }
  }

  return ovec;
}

//======================================================================================================
int Anp::FindRecursiveRangeTrueBarcode(Cut::Var key, 
				       std::vector<Anp::Ptr<Anp::TruthPart> > &result,
				       const TruthPart &tp, 
				       const std::vector<Ptr<TruthPart> > &tvec,
				       bool debug)
{
  if(debug) {
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << __LINE__ << " FindRecursiveRangeTrueBarcode - begin: " << PrintPart(tp) << endl;
  }

  vector<int> ivec;
  
  if(!tp.GetVarVec(key, ivec)) {
    cout << "FindRecursiveRangeTrueBarcode - missing index vector" << endl;
    return 0;
  }

  if(debug) {
    cout << __LINE__ << " FindRecursiveRangeTrueBarcode - ivec.size()=" << ivec.size() << endl;
  }

  int index = 0;
  
  if(!tp.GetVar(Cut::TrueBarcode, index)) {
    cout << "FindRecursiveRangeTrueBarcode - top TruthParticle missing TrueBarcode" << endl;
    return 0;
  }
  
  std::set<int> index_set;
  index_set.insert(index);

  for(const Ptr<TruthPart> &ptr: result) {
    if(ptr->GetVar(Cut::TrueBarcode, index)) {
      index_set.insert(index);
    }
  }
    
  std::vector<Ptr<TruthPart> > ovec;

  for(const Ptr<TruthPart> &ptr: tvec) {
    if(!ptr->GetVar(Cut::TrueBarcode, index)) {
      cout << "FindRecursiveRangeTrueBarcode - missing Cut::TrueBarcode variable" << endl;
      continue;
    }

    if(std::find(ivec.begin(), ivec.end(), index) == ivec.end()) {
      continue;
    }
      
    if(index_set.count(index) == 0) {
      ovec.push_back(ptr);
      index_set.insert(index);
	
      if(debug) {
	cout << __LINE__ << " FindRecursiveRangeTrueBarcode - selected: " << PrintPart(ptr.ref()) << endl;
      }
    }
    else {
      static int icount = 0;
      icount++;

      if(icount < 10) {
	cout << "FindRecursiveRangeTrueBarcode - logic error: current index=top index: " << index << endl
	     << "   input   TruthPart: " << PrintPart(tp)        << endl
	     << "   current TruthPart: " << PrintPart(ptr.ref()) << endl;
      
	if (icount == 9) {
	  cout << "   ...last message" << endl;
	}
      }
    }
  }

  //
  // Collect current results
  //
  result.insert(result.end(), ovec.begin(), ovec.end());

  //
  // Recursive call for parents
  //
  for(const Ptr<TruthPart> &optr: ovec) {
    FindRecursiveRangeTrueBarcode(key, result, optr.ref(), tvec, debug);
  }

  return 1;
}

//======================================================================================================
Anp::Ptr<Anp::TruthVtx> Anp::FindDecayVtx(const TruthPart &p, const std::vector<Ptr<TruthVtx> > &vtxs)
{
  const int vtx_barcode = GetInt(Cut::VtxEnd, p);

  for(const Ptr<TruthVtx> &v: vtxs) {
    const int barcode = GetInt(Cut::TrueBarcode, v.ref());
    
    if(vtx_barcode == barcode) {
      return v;
    }
  }
  
  return Ptr<TruthVtx>();
}

//======================================================================================================
Anp::Ptr<Anp::TruthVtx> Anp::FindProdVtx(const TruthPart &p, const std::vector<Ptr<TruthVtx> > &vtxs)
{
  const int vtx_barcode = GetInt(Cut::VtxProd, p);

  for(const Ptr<TruthVtx> &v: vtxs) {
    const int barcode = GetInt(Cut::TrueBarcode, v.ref());
    
    if(vtx_barcode == barcode) {
      return v;
    }
  }
  
  return Ptr<TruthVtx>();
}

//======================================================================================================
bool Anp::IsGenerator(const TruthPart &tp)
{
  const int status  =          GetInt(Cut::Status,      tp);
  const int barcode = std::abs(GetInt(Cut::TrueBarcode, tp));

  return barcode < 200000 && (status < 200 ||
			      status % 1000 == 1 || status % 1000 == 2 ||
			      status == 10902);
}

//======================================================================================================
bool Anp::IsSameChargeSign(const VarHolder &l, const VarHolder &r)
{
  int pdg0 = 0; 
  int pdg1 = 0; 

  if(!l.GetVar(Cut::PDG, pdg0) || pdg0 == 0) {
    cout << "IsSameChargeSign - missing pdg0" << endl;
    return false;
  }

  if(!r.GetVar(Cut::PDG, pdg1) || pdg1 == 0) {
    cout << "IsSameChargeSign - missing pdg1" << endl;
    return false;
  }
  
  if((pdg0 < 0 && pdg1 > 0) || (pdg0 > 0 && pdg1 < 0)) {
    return false;
  }
  
  return true;
}

//======================================================================================================
Anp::Ptr<Anp::TruthPart> Anp::FindTruthPart(Cut::Var var, 
					    const VarHolder &rec_obj, 
					    const std::vector<Ptr<TruthPart> > &truths)
{
  int truth_bc = 0;
  
  if(!rec_obj.GetVar(var, truth_bc)) {
    return Ptr<TruthPart>();
  }
  
  for(const Ptr<TruthPart> &p: truths) {
    if(truth_bc == GetInt(Cut::TrueBarcode, p.ref())) {
      return p;
    }
  }   
  
  return Ptr<TruthPart>();
}

namespace Anp
{
  bool button_called_state = true;

  TDialogCanvas *current_dialog = 0;

  std::map<std::string, TDialogCanvas *> button_dialogs;
}

//======================================================================================================
void ANP_button_called_function()
{
  Anp::button_called_state = true;
  
  cout << "ANP_button_called_function() - is called" << endl;

  if(Anp::current_dialog) {
    Anp::current_dialog->Paint();
  }
}

//======================================================================================================
int Anp::WaitForButton(const std::string &canvas)
{
  button_called_state = false;

  TDialogCanvas *dialog = 0;

  std::map<std::string, TDialogCanvas *>::iterator dit = button_dialogs.find(canvas);

  if(dit == button_dialogs.end()) {
    dialog = new TDialogCanvas(canvas.c_str(), "", 150, 50);
    dialog->cd();
    dialog->Draw();

    dit = button_dialogs.insert(std::map<std::string, TDialogCanvas *>::value_type(canvas, dialog)).first;
  }
  else {
    dialog = dit->second;
  }
  
  TButton *but1 = new TButton("Next", "ANP_button_called_function()", 0.05, 0.05, 0.95, 0.95);
  but1->Draw();

  dialog->Modified();
  dialog->Update();
  dialog->cd();

  while(true) {
    
    dialog->WaitPrimitive();

    if(!dialog->IsRetained()) {
      return -1;
    }
    
    if(button_called_state) {
      return 1;
    }
  }
  
  return 0;
}

//======================================================================================================
std::string Anp::RemoveSpaces(const std::string &input)
{
  std::string out;
  
  for(unsigned i = 0; i < input.size(); ++i) {
    if(input.at(i) != ' ') {
      out += input.at(i);
    }
  }

  return out;
}
