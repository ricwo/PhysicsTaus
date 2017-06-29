// Base
#include "PhysicsBase/Factory.h"

// Data
#include "PhysicsTaus/Utils.h"

// Local
#include "PlotEvent.h"
#include "TMath.h"
#include <cmath>

REGISTER_ANP_OBJECT(AlgEvent,PlotEvent)

using namespace std;

//======================================================================================================
Anp::PlotEvent::PlotEvent()
{
}

//======================================================================================================
void Anp::PlotEvent::PlotConf(const Registry &reg)
{
  //
  // Read configuration
  //
  reg.Get("PlotEvent::Print",      fPrint      = false);

  fCutEvent		= BookCut("CutEvent",		reg);
  fCutElec		= BookCut("CutElec",		reg);
  fCutMuon		= BookCut("CutMuon",		reg);
  fCutTau		= BookCut("CutTau",		reg);


  //
  // Configure object to histogram variables by keys
  // 
  fHistEvent = BookHist(reg);
  fHistElec  = BookHist(reg, "elec");
  fHistMuon  = BookHist(reg, "muon");
  fHistTau   = BookHist(reg, "tau");

}

//======================================================================================================
bool Anp::PlotEvent::PlotInit(){
  // 2017-06-21 new nu3D hists
  hmMuNu					= GetTH1("mMuNu");
  hmTauNu					= GetTH1("mTauNu");
  hmMuTau					= GetTH1("mMuTau");
  hm2Hat					= GetTH1("m2Hat");
  hmHat						= GetTH1("mHat");
  hm3Hat					= GetTH1("m3Hat");
  hEtaNu					= GetTH1("EtaNu");
  hmMuNuvsm2Hat					= GetTH2("mMuNuvsm2Hat");
  hmHatvsm3Hat					= GetTH2("mHatvsm3Hat");
  hSinThetaStarvsSinThetaHat			= GetTH2("SinThetaStarvsSinThetaHat");
  hEtvsNu					= GetTH2("EtvsNu");
  hkMuvskTau					= GetTH2("kMuvskTau");
  hPhiEtvsPhiNu					= GetTH2("PhiEtvsPhiNu");
  hmMuTauvsmMuNu				= GetTH2("mMuTauvsmMuNu");
  hRhoNu					= GetTH1("RhoNu");



  //2016-09-29
  //investigate cut on phi star
  
  // ### 2D histograms ## //

  // ====================================
  // Basic kinematic dists
  hEta_tau_reco					= GetTH1("Eta_tau_reco");
  hEta_mu_reco					= GetTH1("Eta_mu_reco");
  hPt_tau_reco					= GetTH1("Pt_tau_reco");
  hPt_mu_reco					= GetTH1("Pt_mu_reco");
  hGamma					= GetTH1("Gamma");
  hDelta					= GetTH1("Delta");
  hRho						= GetTH1("Rho");


  hPassGammaDelta      				= GetTH1("PassGammaDelta");



  hMremvsMstar3					= GetTH2("MremvsMstar3");
  hMremstarvsMstar3			       	= GetTH2("MremstarvsMstar3");
  hMremvsMstar2					= GetTH2("MremvsMstar2");
  hMremstarvsMstar2			       	= GetTH2("MremstarvsMstar2");
  hPtAsymvsMstar2      				= GetTH2("PtAsymvsMstar2");
  hAtvsMstar2      				= GetTH2("AtvsMstar2");
  hPhistarvsMstar2     				= GetTH2("PhistarvsMstar2");
  hMstar3vsMstar2     				= GetTH2("Mstar3vsMstar2");
  hMvisvsMstar2     				= GetTH2("MvisvsMstar2");
  hMvisvsMstar2HasDelta  	       		= GetTH2("MvisvsMstar2HasDelta");
  hMvisvsMstar2Sq				= GetTH2("MvisvsMstar2Sq");

  hRhovsDelta					= GetTH2("RhovsDelta");
  hAtvsDelta					= GetTH2("AtvsDelta");
  hSumcosvsDelta       				= GetTH2("SumcosvsDelta");
  hAtDotMETvsDelta				= GetTH2("AtDotMETvsDelta");
  hDPhilMETvsDelta				= GetTH2("DPhilMETvsDelta");
  hDPhilhvsDelta				= GetTH2("DPhilhvsDelta");
  hPhistarvsDelta				= GetTH2("PhistarvsDelta");
  hPtMuvsDelta					= GetTH2("PtMuvsDelta");

  hGammavsDelta					= GetTH2("GammavsDelta");
  hRhovsGamma					= GetTH2("RhovsGamma");
  hAtvsGamma					= GetTH2("AtvsGamma");
  hSumcosvsGamma       				= GetTH2("SumcosvsGamma");
  hAtDotMETvsGamma				= GetTH2("AtDotMETvsGamma");
  hDPhilMETvsGamma				= GetTH2("DPhilMETvsGamma");
  hDPhilhvsGamma				= GetTH2("DPhilhvsGamma");
  hPhistarvsGamma				= GetTH2("PhistarvsGamma");
  hPtMuvsGamma					= GetTH2("PtMuvsGamma");
  hMstar3vsGamma				= GetTH2("Mstar3vsGamma");
  hMstar3DivPtvsGamma				= GetTH2("Mstar3DivPtvsGamma");
  hRhovsDeltaHasGamma				= GetTH2("RhovsDeltaHasGamma");


  // ### Missing histos ## //
  hMstar2DivPt					= GetTH1("Mstar2DivPt");
  hMstar3DivPt					= GetTH1("Mstar3DivPt");

  hDoLeadTau					= GetTH1("DoLeadTau");
  hDoLeadMu					= GetTH1("DoLeadMu");
  hTauBDTScore					= GetTH1("TauBDTScore");

  // ### ROI PLOTS ### //
  hRoi1At					= GetTH1("Roi1At");
  hRoi1Mvis					= GetTH1("Roi1Mvis");
  hRoi1Mvis_l_MET			       	= GetTH1("Roi1Mvis_l_MET");
  hRoi1Mt					= GetTH1("Roi1Mt");
  hRoi1Mstar3					= GetTH1("Roi1Mstar3");
  hRoi1Mstar2					= GetTH1("Roi1Mstar2");
  hRoi1Phistar					= GetTH1("Roi1Phistar");
  hRoi1AtDotMet					= GetTH1("Roi1AtDotMet");
  hRoi1Et					= GetTH1("Roi1Et");
  hRoi1SumCos					= GetTH1("Roi1SumCos");
  hRoi1Mrem					= GetTH1("Roi1Mrem");
  hRoi1Mremstar					= GetTH1("Roi1Mremstar");
  hRoi1DPhiLepMet      				= GetTH1("Roi1DPhiLepMet");
  hRoi1DPhiLepHad      				= GetTH1("Roi1DPhiLepHad");
  hRoi1DPhiHadMet      				= GetTH1("Roi1DPhiHadMet");
  hRoi1Njets					= GetTH1("Roi1Njets");
  hRoi1Njets25					= GetTH1("Roi1Njets25");
  hRoi1TwistLepMet     				= GetTH1("Roi1TwistLepMet");
  hRoi1TwistLepHad     				= GetTH1("Roi1TwistLepHad");
  hRoi1TwistMetHad     				= GetTH1("Roi1TwistMetHad");
  hRoi1SingleJetPt     				= GetTH1("Roi1SingleJetPt");
  hRoi1PtRatio					= GetTH1("Roi1PtRatio");
  hRoi1PtAsym					= GetTH1("Roi1PtAsym");
  hRoi1PtMu					= GetTH1("Roi1PtMu");
  hRoi1Mstar2DivPt				= GetTH1("Roi1Mstar2DivPt");
  hRoi1Mstar3DivPt				= GetTH1("Roi1Mstar3DivPt");

  // ### ROI PLOTS ### //
  hRoi2At					= GetTH1("Roi2At");
  hRoi2Mvis					= GetTH1("Roi2Mvis");
  hRoi2Mvis_l_MET			       	= GetTH1("Roi2Mvis_l_MET");
  hRoi2Mt					= GetTH1("Roi2Mt");
  hRoi2Mstar3					= GetTH1("Roi2Mstar3");
  hRoi2Mstar2					= GetTH1("Roi2Mstar2");
  hRoi2Phistar					= GetTH1("Roi2Phistar");
  hRoi2AtDotMet					= GetTH1("Roi2AtDotMet");
  hRoi2Et					= GetTH1("Roi2Et");
  hRoi2SumCos					= GetTH1("Roi2SumCos");
  hRoi2Mrem					= GetTH1("Roi2Mrem");
  hRoi2Mremstar					= GetTH1("Roi2Mremstar");
  hRoi2DPhiLepMet      				= GetTH1("Roi2DPhiLepMet");
  hRoi2DPhiLepHad      				= GetTH1("Roi2DPhiLepHad");
  hRoi2DPhiHadMet      				= GetTH1("Roi2DPhiHadMet");
  hRoi2Njets					= GetTH1("Roi2Njets");
  hRoi2Njets25					= GetTH1("Roi2Njets25");
  hRoi2TwistLepMet     				= GetTH1("Roi2TwistLepMet");
  hRoi2TwistLepHad     				= GetTH1("Roi2TwistLepHad");
  hRoi2TwistMetHad     				= GetTH1("Roi2TwistMetHad");
  hRoi2SingleJetPt     				= GetTH1("Roi2SingleJetPt");
  hRoi2PtRatio					= GetTH1("Roi2PtRatio");
  hRoi2PtAsym					= GetTH1("Roi2PtAsym");
  hRoi2PtMu					= GetTH1("Roi2PtMu");
  hRoi2Mstar2DivPt				= GetTH1("Roi2Mstar2DivPt");
  hRoi2Mstar3DivPt				= GetTH1("Roi2Mstar3DivPt");



  hPassGammaDeltaRhovsDelta		       		= GetTH2("PassGammaDeltaRhovsDelta");
  hPassGammaDeltaAt					= GetTH1("PassGammaDeltaAt");
  hPassGammaDeltaMvis					= GetTH1("PassGammaDeltaMvis");
  hPassGammaDeltaMvis_l_MET			       	= GetTH1("PassGammaDeltaMvis_l_MET");
  hPassGammaDeltaMt					= GetTH1("PassGammaDeltaMt");
  hPassGammaDeltaMstar3					= GetTH1("PassGammaDeltaMstar3");
  hPassGammaDeltaMstar2					= GetTH1("PassGammaDeltaMstar2");
  hPassGammaDeltaPhistar					= GetTH1("PassGammaDeltaPhistar");
  hPassGammaDeltaAtDotMet					= GetTH1("PassGammaDeltaAtDotMet");
  hPassGammaDeltaEt					= GetTH1("PassGammaDeltaEt");
  hPassGammaDeltaSumCos					= GetTH1("PassGammaDeltaSumCos");
  hPassGammaDeltaMrem					= GetTH1("PassGammaDeltaMrem");
  hPassGammaDeltaMremstar					= GetTH1("PassGammaDeltaMremstar");
  hPassGammaDeltaDPhiLepMet      				= GetTH1("PassGammaDeltaDPhiLepMet");
  hPassGammaDeltaDPhiLepHad      				= GetTH1("PassGammaDeltaDPhiLepHad");
  hPassGammaDeltaDPhiHadMet      				= GetTH1("PassGammaDeltaDPhiHadMet");
  hPassGammaDeltaNjets					= GetTH1("PassGammaDeltaNjets");
  hPassGammaDeltaNjets25					= GetTH1("PassGammaDeltaNjets25");
  hPassGammaDeltaTwistLepMet     				= GetTH1("PassGammaDeltaTwistLepMet");
  hPassGammaDeltaTwistLepHad     				= GetTH1("PassGammaDeltaTwistLepHad");
  hPassGammaDeltaTwistMetHad     				= GetTH1("PassGammaDeltaTwistMetHad");
  hPassGammaDeltaSingleJetPt     				= GetTH1("PassGammaDeltaSingleJetPt");
  hPassGammaDeltaPtRatio					= GetTH1("PassGammaDeltaPtRatio");
  hPassGammaDeltaPtAsym					= GetTH1("PassGammaDeltaPtAsym");
  hPassGammaDeltaPtMu					= GetTH1("PassGammaDeltaPtMu");
  hPassGammaDeltaMstar2DivPt				= GetTH1("PassGammaDeltaMstar2DivPt");
  hPassGammaDeltaMstar3DivPt				= GetTH1("PassGammaDeltaMstar3DivPt");

  hNotPassGammaDeltaRhovsDelta		       		= GetTH2("NotPassGammaDeltaRhovsDelta");
  hNotPassGammaDeltaAt					= GetTH1("NotPassGammaDeltaAt");
  hNotPassGammaDeltaMvis					= GetTH1("NotPassGammaDeltaMvis");
  hNotPassGammaDeltaMvis_l_MET			       	= GetTH1("NotPassGammaDeltaMvis_l_MET");
  hNotPassGammaDeltaMt					= GetTH1("NotPassGammaDeltaMt");
  hNotPassGammaDeltaMstar3					= GetTH1("NotPassGammaDeltaMstar3");
  hNotPassGammaDeltaMstar2					= GetTH1("NotPassGammaDeltaMstar2");
  hNotPassGammaDeltaPhistar					= GetTH1("NotPassGammaDeltaPhistar");
  hNotPassGammaDeltaAtDotMet					= GetTH1("NotPassGammaDeltaAtDotMet");
  hNotPassGammaDeltaEt					= GetTH1("NotPassGammaDeltaEt");
  hNotPassGammaDeltaSumCos					= GetTH1("NotPassGammaDeltaSumCos");
  hNotPassGammaDeltaMrem					= GetTH1("NotPassGammaDeltaMrem");
  hNotPassGammaDeltaMremstar					= GetTH1("NotPassGammaDeltaMremstar");
  hNotPassGammaDeltaDPhiLepMet      				= GetTH1("NotPassGammaDeltaDPhiLepMet");
  hNotPassGammaDeltaDPhiLepHad      				= GetTH1("NotPassGammaDeltaDPhiLepHad");
  hNotPassGammaDeltaDPhiHadMet      				= GetTH1("NotPassGammaDeltaDPhiHadMet");
  hNotPassGammaDeltaNjets					= GetTH1("NotPassGammaDeltaNjets");
  hNotPassGammaDeltaNjets25					= GetTH1("NotPassGammaDeltaNjets25");
  hNotPassGammaDeltaTwistLepMet     				= GetTH1("NotPassGammaDeltaTwistLepMet");
  hNotPassGammaDeltaTwistLepHad     				= GetTH1("NotPassGammaDeltaTwistLepHad");
  hNotPassGammaDeltaTwistMetHad     				= GetTH1("NotPassGammaDeltaTwistMetHad");
  hNotPassGammaDeltaSingleJetPt     				= GetTH1("NotPassGammaDeltaSingleJetPt");
  hNotPassGammaDeltaPtRatio					= GetTH1("NotPassGammaDeltaPtRatio");
  hNotPassGammaDeltaPtAsym					= GetTH1("NotPassGammaDeltaPtAsym");
  hNotPassGammaDeltaPtMu					= GetTH1("NotPassGammaDeltaPtMu");
  hNotPassGammaDeltaMstar2DivPt				= GetTH1("NotPassGammaDeltaMstar2DivPt");
  hNotPassGammaDeltaMstar3DivPt				= GetTH1("NotPassGammaDeltaMstar3DivPt");



  //19/10/2016 testhists
  hSumCos					= GetTH1("SumCos");
  hPassAtlasCuts       				= GetTH1("PassAtlasCuts");
  hPassAtlasCuts3      				= GetTH1("PassAtlasCuts3");
  hMT2vsMvis					= GetTH2("MT2vsMvis");
  hEtaVsMT2					= GetTH2("EtaVsMT2");
  hQeta						= GetTH1("Qeta");
  hMvis						= GetTH1("Mvis");
  hPtMu						= GetTH1("PtMu");
  hMvis_l_MET				       	= GetTH1("Mvis_l_MET");
  hMvisMstar2cut	       	       		= GetTH1("MvisMstar2cut");
  hMstar3Mstar2cut           	      		= GetTH1("Mstar3Mstar2cut");
  hDPhiVsMstar2					= GetTH2("DPhiVsMstar2");
  hDPhiVsMstar3					= GetTH2("DPhiVsMstar3");
  hDPhiVsMT2					= GetTH2("DPhiVsMT2");
  hDPhiVsMT3					= GetTH2("DPhiVsMT3");
  hMstar3_mu_h_vsMtautau			= GetTH2("Mstar3_mu_h_vsMtautau");
  hMstar3_e_h_vsMtautau				= GetTH2("Mstar3_e_h_vsMtautau");
  hEventPassTrig       				= GetTH1("EventPassTrig");
  hDPhi_match					= GetTH1("DPhi_match");
  hMstar3_M_mumu	       		       	= GetTH2("Mstar3_M_mumu");
  hTwist_DPhi					= GetTH2("Twist_DPhi");
  hTwist_MET_l					= GetTH1("Twist_MET_l");
  hMT_DPhi					= GetTH2("MT_DPhi");
  hATdotMET_tim_MT				= GetTH1("ATdotMET_tim_MT");
  hATdotMET_div_MT				= GetTH1("ATdotMET_div_MT");
  hMT2_ATdotMET					= GetTH2("MT2_ATdotMET");
  hPhi_MET_l					= GetTH1("Phi_MET_l");
  hMstar3	    				= GetTH1("Mstar3");
  hMstar2	    				= GetTH1("Mstar2");
  hMstar_no_cut	    				= GetTH1("Mstar_no_cut");
  hMT3						= GetTH1("MT3");
  hMT3_2       					= GetTH1("MT3_2");
  hMT_1_2					= GetTH2("MT_1_2");
  hM_MT_div_SinTheta				= GetTH2("M_MT_div_SinTheta");
  hAThatdotMET					= GetTH1("AThatdotMET");
  hM_lh						= GetTH1("M_lh");
  hM_lh_no_cut				       	= GetTH1("M_lh_no_cut");
  hATdotMET_PhiStar				= GetTH2("ATdotMET_PhiStar");
  hPhiStar					= GetTH1("PhiStar");
  hATdotMET_MT					= GetTH2("ATdotMET_MT");
  hATdotMETdivPhiStar	      	       		= GetTH1("ATdotMETdivPhiStar");
  hATdotMETtimesPhiStar	      	       		= GetTH1("ATdotMETtimesPhiStar");
  hPhiStarDivATdotMET	      	       		= GetTH1("PhiStarDivATdotMET");
  hAT						= GetTH1("AT");
  hMT						= GetTH1("MT");
  hATdotMET					= GetTH1("ATdotMET");
  hMT_MET_l					= GetTH1("MT_MET_l");
  hEt_semilep					= GetTH1("Et_semilep");
  hEt_dilep					= GetTH1("Et_dilep");
  hEt_phi_semilep      				= GetTH1("Et_phi_semilep");
  hArccos_Et_t_semilep            		= GetTH1("Arccos_Et_t_semilep");
  hCos_Et_t_semilep            			= GetTH1("Cos_Et_t_semilep");
  hLogCos_Et_t_semilep            	       	= GetTH1("LogCos_Et_t_semilep");

  hPhi_acop_semilep_bkg			        = GetTH1("Phi_acop_semilep_bkg");

  return true;
}

//======================================================================================================
void Anp::PlotEvent::PlotDone()
{
  SaveCutsHists(GetDir());
}

//======================================================================================================
void Anp::PlotEvent::PlotExec()
{
  /*
    Process raw ntuple event
   */
  
  if(fReco.IsValidEvent()) {
    for(RecoEvent &reco: fReco.GetRef()) {
      Ptr<CandEvent> cand = PrepCandEvent(reco);
      if(cand.valid()) {
	//
	// Save selected candidate event
	//
	reco.GetVec<CandEvent>().push_back(cand);

	//
	// Process selected candidate events
	//
	FillCandEvent(cand.ref());
	PlotCandEvent(cand.ref());
      }
    }
  }
}

//======================================================================================================
Anp::Ptr<Anp::CandEvent> Anp::PlotEvent::PrepCandEvent(const RecoEvent &reco)
{
  /*
    Select reconstructed objects and make candidate event
   */

  Ptr<CandEvent> cand = ObjectFactory<CandEvent>::Instance().CreateObject(Anp::CandEvent(reco, AlgEvent::GetName()));
  cand->CopyVarsFrom(reco);
  cand->GetReco() = reco.MakeCopy();

  FillCandRecObjects(cand.ref());

  vector<Ptr<RecElec> >		&elecs		= cand->GetVec<RecElec>();
  vector<Ptr<RecMuon> >		&muons		= cand->GetVec<RecMuon>();
  vector<Ptr<RecTau > >		&taus		= cand->GetVec<RecTau >();
  vector<Ptr<Lepton> >		&leps		= cand->GetVec<Lepton >();


  SelectObjects(cand->GetReco().GetVec<RecMuon>(),muons,fCutMuon.ref());
  SelectObjects(cand->GetReco().GetVec<RecElec>(), elecs, fCutElec.ref());
  SelectObjects(cand->GetReco().GetVec<RecTau >(), taus,  fCutTau.ref());


  // find leading tau
  Ptr<RecTau> leadTau;
  
  if( taus.size() > 0 && muons.size() == 1){
    leadTau = taus.at(0);
    for( auto t: taus ){
      if( t->GetFourMom().GetPt() > leadTau->GetFourMom().GetPt() ){
	leadTau = t;
      }
    }
    int taucharge = -200;
    leadTau->GetVar(Cut::Charge, taucharge );
    cand->AddVar(Cut::OppositeCharge, taucharge * muons.at(0).ref().GetCharge());
  }
  else cand->AddVar(Cut::OppositeCharge, 0);
  
  int trigcounter = 1;
  if( trigcounter>0 ) FillTH1(hEventPassTrig, 1);
  else FillTH1(hEventPassTrig, 0);

  for(Ptr<RecElec> &elec: elecs) {
    if (fCutElec->PassCut(elec.ref()) ) leps.push_back(ObjectFactory<Lepton>::Instance().CreateObject(Anp::Lepton(elec)));
  }

  for(Ptr<RecMuon> &muon: muons) {
    if (fCutMuon->PassCut(muon.ref()) ) leps.push_back(ObjectFactory<Lepton>::Instance().CreateObject(Anp::Lepton(muon)));
  }

  return cand;
}


//====================================================================
bool Anp::PlotEvent::MuonTrig(RecMuon &muon, CandEvent &cand){

    int runNumber = cand.GetVar(Cut::Run);

    // data16 A
    if (runNumber >= 297730 && runNumber <= 299584){
        if (muon.GetVar(Cut::match_HLT_mu24_iloose) == 1 || muon.GetVar(Cut::match_HLT_mu24_ivarloose) == 1 || muon.GetVar(Cut::match_HLT_mu24_iloose_L1MU15) == 1 || muon.GetVar(Cut::match_HLT_mu24_ivarloose_L1MU15) == 1 || muon.GetVar(Cut::match_HLT_mu40) == 1 || muon.GetVar(Cut::match_HLT_mu50) == 1) return true;
    }

    // data16 B-D3
    else if (runNumber >= 300279 && runNumber <= 302872){
        if (muon.GetVar(Cut::match_HLT_mu24_imedium) == 1 || muon.GetVar(Cut::match_HLT_mu24_ivarmedium) == 1 || muon.GetVar(Cut::match_HLT_mu50) == 1) return true;
    }

    // data16 D4-E
    else if (runNumber >= 302919 && runNumber <= 303892){
        if (muon.GetVar(Cut::match_HLT_mu24_imedium) == 1 || muon.GetVar(Cut::match_HLT_mu24_ivarmedium) == 1 ||
                muon.GetVar(Cut::match_HLT_mu26_imedium) == 1 || muon.GetVar(Cut::match_HLT_mu26_ivarmedium) == 1 || muon.GetVar(Cut::match_HLT_mu50) == 1) return true;
    }

    // data16 F-G2
    else if (runNumber >= 303943 && runNumber <= 305293){
        if (muon.GetVar(Cut::match_HLT_mu26_imedium) == 1 || muon.GetVar(Cut::match_HLT_mu26_ivarmedium) == 1 || muon.GetVar(Cut::match_HLT_mu50) == 1) return true;
    }

    // data16 G3-rest
    else if (runNumber >= 305380){
        if (muon.GetVar(Cut::match_HLT_mu26_ivarmedium) == 1 || muon.GetVar(Cut::match_HLT_mu50) == 1) return true; 
    }

    // data15 trigger 
    else if (runNumber <= 284484){
        if (muon.GetVar(Cut::match_HLT_mu20_iloose_L1MU15) == 1 || muon.GetVar(Cut::match_HLT_mu24_iloose_L1MU15) == 1 || muon.GetVar(Cut::match_HLT_mu24_imedium) == 1 || muon.GetVar(Cut::match_HLT_mu26_imedium) == 1 || muon.GetVar(Cut::match_HLT_mu40) == 1 || muon.GetVar(Cut::match_HLT_mu50) == 1) return true;
    }

    return false;
}

//====================================================================
void Anp::PlotEvent::FillCandRecObjects(CandEvent &cand)
{

    vector<Ptr<RecElec>> &elecs = cand.GetReco().GetVec<RecElec>();
    vector<Ptr<RecMuon>> &muons = cand.GetReco().GetVec<RecMuon>();
    vector<Ptr<RecTau>>  &taus  = cand.GetReco().GetVec<RecTau>();

    // Et cone / pT variables
    // Pt cone/ pT variables
    // For electrons

    int muonTrigCounter{0};

    for ( Ptr<RecElec> &elec: elecs) {
        float Pt20 = 0.;
        float Pt30 = 0.;
        float Et20 = 0.;
        float Et30 = 0.;

        float Pt = elec->GetFourMom().GetPt();
        elec->GetVar(Cut::PtVarCone20, Pt20);
        elec->GetVar(Cut::PtVarCone30, Pt30);
        elec->GetVar(Cut::EtTopoCone20, Et20);
        elec->GetVar(Cut::EtTopoCone30, Et30);

        elec->AddVar(Cut::PtVarCone20OverPt, Pt20 / Pt);
        elec->AddVar(Cut::PtVarCone30OverPt, Pt30 / Pt);
        elec->AddVar(Cut::EtCone20OverPt, Et20 / Pt);

    }

    // For muons
    for ( Ptr<RecMuon> &muon: muons) {
        float Pt20 = 0.;
        float Pt30 = 0.;
        float Et20 = 0.;
        float Et30 = 0.;

        float Pt = muon->GetFourMom().GetPt();

        muon->GetVar(Cut::PtVarCone20, Pt20);
        muon->GetVar(Cut::PtVarCone30, Pt30);
        muon->GetVar(Cut::EtTopoCone20, Et20);
        muon->GetVar(Cut::EtTopoCone30, Et30);

        muon->AddVar(Cut::PtVarCone20OverPt, Pt20 / Pt);
        muon->AddVar(Cut::PtVarCone30OverPt, Pt30 / Pt);
        muon->AddVar(Cut::EtCone20OverPt, Et20 / Pt);
   
	if (MuonTrig(*muon, cand)) muonTrigCounter++;

    }

    cand.AddVar(Cut::NTrigMuon, muonTrigCounter);

    //cand.AddVar(Cut::NTrigMuon, muonTrigCounter);
    float drcut = 0.2;
    //overlap removal
    //hierarchy: muons, elecs, taus
    for( Ptr<RecTau> &t: taus){
      int hasMuonOL = 0;
      int hasElecOL = 0;
      for( Ptr<RecMuon> &m: muons) {
	float dr = t->GetFourMom().GetDR(m->GetFourMom());
	if( dr < drcut ) hasMuonOL = 1;
      }
      for( Ptr<RecElec> &e: elecs) {
	float dr = t->GetFourMom().GetDR(e->GetFourMom());
	if( dr < drcut ) hasElecOL = 1;
      }
      if( hasElecOL + hasMuonOL > 0 ) t->AddVar(Cut::TauOverlap, 0);
      else t->AddVar(Cut::TauOverlap, 1);
    }
    
    // Hierarchy muons > elecs > taus > jets
}

//==================
//==================


void Anp::PlotEvent::FillCandEvent(CandEvent &cand)
{
  /*
    Compute variables for event and selected objects
   */

  vector<Ptr<Lepton> >		&leps        = cand.GetVec<Lepton    >();
  vector<Ptr<RecElec> >		&elecs       = cand.GetVec<RecElec   >();
  vector<Ptr<RecMuon> >		&muons       = cand.GetVec<RecMuon   >();
  vector<Ptr<RecJet > >		&jets        = cand.GetVec<RecJet    >();
  vector<Ptr<RecTau > >		&taus        = cand.GetVec<RecTau    >();
  vector<Ptr<TruthPart > >      &truthparts  = cand.GetVec<TruthPart >();

  cand.AddVar(Cut::NLepton,  leps .size());
  cand.AddVar(Cut::NElec,    elecs.size());
  cand.AddVar(Cut::NMuon,    muons.size());
  cand.AddVar(Cut::NTau,     taus .size());
  cand.AddVar(Cut::NJet,     jets .size());
  cand.AddVar(Cut::NTruth,   truthparts.size());
}

//======================================================================================================
template <class T, class T2>
float Anp::PlotEvent::PhiStarEta(T &p0, T2 &p1)
{
  const TLorentzVector v0	= p0.GetFourMom().GetLV();
  const TLorentzVector v1      	= p1.GetFourMom().GetLV();
  float etaplus       		= v0.Eta();
  float etaminus      		= v1.Eta();
  float dphi			= TMath::Abs(v0.DeltaPhi(v1));
  float phiacop		= TMath::Pi() - dphi;
  float thetastareta		= TMath::ACos(TMath::TanH((etaminus - etaplus)/2));
  return TMath::Tan(phiacop/2) * TMath::Sin(thetastareta);
}
//======================================================================================================
template <class T, class T2>
TVector3 Anp::PlotEvent::GetEventAxis(T &p0, T2 &p1)
{
  const TLorentzVector v0	= p0.GetFourMom().GetLV();
  const TLorentzVector v1      	= p1.GetFourMom().GetLV();
  TVector3 vec0			= v0.Vect();
  TVector3 vec1			= v1.Vect();
  vec0.SetZ(0);
  vec1.SetZ(0);
  return (vec0 - vec1).Unit();
}
//======================================================================================================
template <class T, class T2>
float Anp::PlotEvent::aT(T &p0, T2 &p1)
{
  const TLorentzVector v0	= p0.GetFourMom().GetLV();
  const TLorentzVector v1      	= p1.GetFourMom().GetLV();
  TVector3 vec0	= v0.Vect();
  TVector3 vec1	= v1.Vect();
  vec0.SetZ(0);
  vec1.SetZ(0);
  float dphi = abs(v0.DeltaPhi(v1));
  if( dphi > TMath::Pi() / 2. ){ 
    return ((vec0+vec1).Cross((vec0-vec1).Unit())).Mag();
  } else return (vec0+vec1).Mag();
}
//======================================================================================================
template <class T, class T2>
TVector3 Anp::PlotEvent::aTvec(T &p0, T2 &p1)
{
  const TLorentzVector v0	= p0.GetFourMom().GetLV();
  const TLorentzVector v1      	= p1.GetFourMom().GetLV();
  TVector3 vec0	= v0.Vect();
  TVector3 vec1	= v1.Vect();
  vec0.SetZ(0);
  vec1.SetZ(0);
  TVector3 Q	= vec0 + vec1;
  TVector3 that = (vec0 - vec1).Unit();
  float dphi = abs(v0.DeltaPhi(v1));
  if( dphi > TMath::Pi() / 2. ){ 
    return Q - Q.Dot(that) * that;
  } else return Q;
  
}

//======================================================================================================
float Anp::PlotEvent::GetMT(float ptl, float dphi, float met){
  return TMath::Sqrt( 2*ptl*met*(1-cos(abs(dphi))));
}
//======================================================================================================
bool Anp::PlotEvent::IsHadronicBool (const TruthPart &particle, 
					const vector<Ptr<TruthPart> > &AllTruths) const
{
  std::vector<Anp::Ptr<Anp::TruthPart> > children = Anp::FindRangeTrueBarcode(Cut::IndexRangeChild, 
									      particle, 
									      AllTruths);   
  const int pdgidinput	= abs(Anp::GetInt(Cut::PDG, particle, 0));
  if (pdgidinput!=15) {
    cout << "Input particle is not tau" << endl;
    return false;
  }
  bool isHadronicTau = false;
  for(Ptr<TruthPart> &child: children) {
    if (IsHadron(child.ref())) isHadronicTau = true;
    
  }
  if (children.size()==1) {
    const int pdgidchild	= abs(Anp::GetInt(Cut::PDG, children.at(0).ref(), 0));
    if (pdgidchild==15)	{
      return IsHadronicBool(children.at(0).ref(), AllTruths);
    }
  }
  return isHadronicTau;
}
//======================================================================================================
bool Anp::PlotEvent::IsFromTauDecay (const TruthPart &particle, 
					const vector<Ptr<TruthPart> > &AllTruths) const
{
  std::vector<Anp::Ptr<Anp::TruthPart> > parents = Anp::FindRangeTrueBarcode(Cut::IndexRangeParent, 
									      particle, 
									      AllTruths);   
  const int pdgidinput	= abs(Anp::GetInt(Cut::PDG, particle, 0));
  if (pdgidinput!=11&&pdgidinput!=13) {
    cout << "Input particle is not light lepton" << endl;
    return false;
  }
  bool isFromTauDecay = false;
  for(Ptr<TruthPart> &parent: parents) {
    int pdgid	= abs(Anp::GetInt(Cut::PDG, parent.ref(), 0));
    if (pdgid==15) isFromTauDecay = true;
  }
  return isFromTauDecay;
}
//======================================================================================================
bool Anp::PlotEvent::IsTauPionNuDecay (const TruthPart	&particle, 
					  const vector<Ptr<TruthPart> > &AllTruths) const
{
  std::vector<Anp::Ptr<Anp::TruthPart> > children = Anp::FindRangeTrueBarcode(Cut::IndexRangeChild, 
									      particle, 
									      AllTruths);   
  const int pdgidinput	= abs(Anp::GetInt(Cut::PDG, particle, 0));
  if (pdgidinput!=15) {
    cout << "Input particle is not tau" << endl;
    return false;
  }
  bool isTauPionNuDecay = false;
  bool hasPionChild	= false;
  bool hasTauNuChild	= false;
  for(Ptr<TruthPart> &child: children) {
    int pdgid	= abs(Anp::GetInt(Cut::PDG, child.ref(), 0));
    if (pdgid==211) hasPionChild = true;
    else if (pdgid==16) hasTauNuChild = true;
  }
  if (children.size()==1) {
    const int pdgidchild	= abs(Anp::GetInt(Cut::PDG, children.at(0).ref(), 0));
    if (pdgidchild==15)	{
      return IsTauPionNuDecay(children.at(0).ref(), AllTruths);
    }
  }
  if (children.size()==2&&hasPionChild&&hasTauNuChild) isTauPionNuDecay = true;
  return isTauPionNuDecay;
}
//======================================================================================================
bool Anp::PlotEvent::IsHadron (const TruthPart &p)  const
{
  switch(abs(p.GetPdgId())) {
  case 11: return false;
  case 12: return false;
  case 13: return false;
  case 14: return false;
  case 16: return false;
  case 22: return false;
  case 23: return false;
  case 24: return false;
  default: return true;
  }
}
//======================================================================================================
bool Anp::PlotEvent::IsLeptonicBool (const TruthPart	&particle, 
					const vector<Ptr<TruthPart> > &AllTruths) const
{
  std::vector<Anp::Ptr<Anp::TruthPart> > children = Anp::FindRangeTrueBarcode(Cut::IndexRangeChild, 
									      particle, 
									      AllTruths);   
  const int pdgidinput	= abs(Anp::GetInt(Cut::PDG, particle, 0));
  if (pdgidinput!=15) {
    cout << "Input particle is not tau" << endl;
    return false;
  }
  bool isLeptonicTau = false;

  if (children.size()==1) {
    const int pdgidchild	= abs(Anp::GetInt(Cut::PDG, children.at(0).ref(), 0));
    if (pdgidchild==15)	{
      return IsLeptonicBool(children.at(0).ref(), AllTruths);
    }
  }
  return isLeptonicTau;
}
//======================================================================================================
bool Anp::PlotEvent::DecaysToPdg (const TruthPart	&particle, 
				       const vector<Ptr<TruthPart> > &AllTruths,
				       const int pdg) const
{
  std::vector<Anp::Ptr<Anp::TruthPart> > children = Anp::FindRangeTrueBarcode(Cut::IndexRangeChild, 
									      particle, 
									      AllTruths);   
  const int pdgidinput	= abs(Anp::GetInt(Cut::PDG, particle, 0));
  if (pdgidinput!=15) {
    cout << "Input particle is not tau" << endl;
    return false;
  }
  bool decaystopdg = false;
  bool hasTauNuChild = true;
  bool hasLepNuChild = true;
  if (children.size()>1){
    for(Ptr<TruthPart> &child: children) {
      int pdgid	= abs(Anp::GetInt(Cut::PDG, child.ref(), 0));
      if (pdgid == pdg) decaystopdg = true;
      else if (pdgid == 16) hasTauNuChild = true;
      else if (pdgid == (pdg+1) ) hasLepNuChild = true;
    }
  }
  if (children.size()==1) {
    const int pdgidchild	= abs(Anp::GetInt(Cut::PDG, children.at(0).ref(), 0));
    if (pdgidchild==15)	{
      return DecaysToPdg(children.at(0).ref(), AllTruths, pdg);
    }
  }
  if ((decaystopdg&&hasTauNuChild)&&hasLepNuChild) return true;
  else return false;
}
//======================================================================================================
bool Anp::PlotEvent::GammaDeltaPass(TLorentzVector &vl, TLorentzVector &vh, TVector3 &METvec) const {
  float ptl = vl.Pt();
  float pth = vh.Pt();
  float rho = (pth - ptl)/(pth + ptl);
  float etal = vl.Eta();
  float etah = vh.Eta();
  float sintheta   = sin(acos(tanh((etah-etal)/2)));
  TVector3 pT_l = vl.Vect();
  pT_l.SetZ(0);
  float mT_MET_l = sqrt( pow(vl.Et() + METvec.Mag(),2) - (pT_l + METvec).Mag2() );
  float mstar = mT_MET_l / 1.0e3 / sintheta;
  float mvis = (vl+vh).M();
  mvis /= 1.0e3;
  float x = mstar;
  for( float apex = .1; apex < 91; apex += .1 ){
    float delta = apex / 5;
    float ylo =  7*x/9 - apex * 7/9 + 70;
    float yup = -7*x/9 + apex * 7/9 + 70;
    if( x < apex ){ 
      if( mvis < yup && mvis > ylo ){
	float c = -.424;
	float ylo_rhodelta = .05*delta + c;
	if( rho > ylo_rhodelta && rho < .8 ){
	  return true;
	}
      }
    }
  }
  return false;
}
//======================================================================================================
float Anp::PlotEvent::GetGamma(TLorentzVector &vl, TLorentzVector &vh, TVector3 &METvec) const {
  float ptl = vl.Pt();
  float pth = vh.Pt();
  float rho = (pth - ptl)/(pth + ptl);
  float delta = GetDelta(vl,vh,METvec);
  if( delta <= 0 ) return -2.e4;
  float yloprev = 1.0e3;
  for( float apex = .1; apex <= 90; apex += .1 ){
    float c = 0.8 - 1.8*apex/90;
    float slope = 0.01 * apex / 90 + 0.01;
    float ylo = slope*delta + c;
    if( delta < 45 && delta < apex && rho < yloprev && rho > ylo && rho < 0.8 ) {
      return apex;
    }
    else if( delta >= 45 && delta < apex && rho < yloprev && rho > ylo && rho < 0.8 && rho > 0.5 ) {
      return apex;
    }
    yloprev = ylo;
  }
  return -2.e4; // else return -20k
}
//======================================================================================================
float Anp::PlotEvent::GetDelta(TLorentzVector &vl, TLorentzVector &vh, TVector3 &METvec) const {
  float etal = vl.Eta();
  float etah = vh.Eta();
  float sintheta   = sin(acos(tanh((etah-etal)/2)));
  TVector3 pT_l = vl.Vect();
  pT_l.SetZ(0);
  float mT_MET_l = sqrt( pow(vl.Et() + METvec.Mag(),2) - (pT_l + METvec).Mag2() );
  float mstar = mT_MET_l / 1.0e3 / sintheta;
  float mvis = (vl+vh).M();
  mvis /= 1.0e3;
  float x = mstar;
  float yloprev = 1.0e3;
  float yupprev = 0;
  for( float apex = .1; apex < 91; apex += .1 ){
    float delta = apex;
    float ylo =  7*x/9 - apex * 7/9 + 70;
    float yup = -7*x/9 + apex * 7/9 + 70;
    if( x < apex ){
      if(((mvis < yup && mvis > yupprev) ||  (mvis > ylo && mvis < yloprev))){
	return delta;
      }
    }
    yloprev = ylo;
    yupprev = yup;
  }
  return -2.e4; // else return -20k
}
//======================================================================================================
void Anp::PlotEvent::PlotCandEvent(CandEvent &cand)
{
  /*
    Plot event variables and object variables
   */

  if(!fCutEvent->PassCut(cand)) {
    return;
  }

  //
  // Plot selected objects
  //
  vector<Ptr<RecTau>  >	    &taus	= cand.GetVec<RecTau>();
  vector<Ptr<Lepton>  >	    &leps	= cand.GetVec<Lepton>();
  vector<Ptr<TruthPart>  >	    &truth	= cand.GetVec<TruthPart>();
  float Et	= cand.GetVar(Cut::MET_RefFinal_et);
  float Et_phi	= cand.GetVar(Cut::MET_RefFinal_phi);
  // from it, construct missing MET 3-vector
  const TVector3 Etvec( Et * cos(Et_phi), Et * sin(Et_phi), 0 );
  // FIND THE Neutrino Vector

  //=======================================================================================================
  //=======================================================================================================
  //H->tautau->lh

  // find leading lepton
  Ptr<Lepton> leadLep;
  Ptr<Lepton> secLep;
  if( leps.size() > 0 ){
    leadLep = leps.at(0);
    for( auto l: leps ){
      if( l->GetFourMom().GetPt() > leadLep->GetFourMom().GetPt() ){
	leadLep = l;
      }
    }
    if( leps.size() == 1 && leps.at(0)->IsMuon() ) FillTH1(hDoLeadMu, 0.);
    else if( leps.at(0)->IsMuon() ) FillTH1(hDoLeadMu, 1.);
    if( leps.size() > 1){
      secLep = leps.at(1);
      for( auto l: leps ){
	if( (l->GetFourMom().GetPt() < leadLep->GetFourMom().GetPt()) &&
	    (l->GetFourMom().GetPt() > secLep->GetFourMom().GetPt()) ){
	  secLep = l;
	}
      }
    }
  }
  
  // find leading tau
  Ptr<RecTau> leadTau;
  Ptr<RecTau> secTau;
  
  if( taus.size() > 0 ){
    leadTau = taus.at(0);
    for( auto t: taus ){
      if( t->GetFourMom().GetPt() > leadTau->GetFourMom().GetPt() ){
	leadTau = t;
      }
    }
    if( taus.size() == 1 ) FillTH1(hDoLeadTau, 0.);
    else FillTH1(hDoLeadTau, 1.);
    if( taus.size() > 1){
      secTau = taus.at(1);
      for( auto t: taus ){
	if( (t->GetFourMom().GetPt() < leadTau->GetFourMom().GetPt()) &&
	    (t->GetFourMom().GetPt() > secTau->GetFourMom().GetPt()) ){
	  secTau = t;
	}
      }
    }
  }

  
  bool bkg = true;

  if( bkg ){
    if( taus.size() > 0 && leps.size() > 0 ){
      taus.at(0) = leadTau;
      leps.at(0) = leadLep;
      float lepptvarcone30 = 0;
      leadLep->GetVar(Cut::PtVarCone30OverPt, lepptvarcone30);
      float lepetcone20 = 0;
      leadLep->GetVar(Cut::EtCone20OverPt, lepetcone20);
      float taubdt = 0;
      leadTau->GetVar(Cut::BDTJetScore, taubdt);
      // check opposite charge
      int taucharge = 0;
      taus.at(0)->GetVar(Cut::Charge, taucharge );
      int isTightTau = 0;
      taus.at(0)->GetVar(Cut::JetBDTSigTight, isTightTau);
      FillTH1(hTauBDTScore, taubdt);
      float mlh	= (taus.at(0).ref().GetFourMom().GetLV() + 
			   leps.at(0).ref().GetFourMom().GetLV() ).M();
      float etah	= taus.at(0).ref().GetFourMom().GetLV().Eta();
      float etal	= leps.at(0).ref().GetFourMom().GetLV().Eta();
      float sintheta	= sin(acos(tanh((etah-etal)/2)));
      TVector3 pT_l	= leps.at(0).ref().GetFourMom().GetLV().Vect();
      pT_l.SetZ(0);
      TVector3 pT_h	= taus.at(0).ref().GetFourMom().GetLV().Vect();
      pT_h.SetZ(0);      
      float mT3	= sqrt( pow(taus.at(0).ref().GetFourMom().GetLV().Et() + 
				    leps.at(0).ref().GetFourMom().GetLV().Et() +
				    Et,2) - 
				(pT_l + pT_h + Etvec).Mag2()  );      
      FillTH1(hM_lh_no_cut, mlh / 1e3);
      FillTH1(hMstar_no_cut, mT3 / sintheta / 1e3 );
    }
    if( taus.size() > 0 && leps.size() > 0 /*&& passesMinvcut*/){
      float phistar	= PhiStarEta(taus.at(0).ref(), leps.at(0).ref());
      float phiacop	= TMath::Pi() - abs(taus.at(0).ref().GetFourMom().GetDPhi(leps.at(0).ref().GetFourMom()));
      float dphi_lh    = abs(taus.at(0).ref().GetFourMom().GetLV().DeltaPhi(leps.at(0).ref().GetFourMom().GetLV()));
      float pth	= taus.at(0).ref().GetFourMom().GetLV().Pt();
      float ptl	= leps.at(0).ref().GetFourMom().GetLV().Pt();
      float rho	= (pth-ptl)/(pth+ptl);
      float etah	= taus.at(0).ref().GetFourMom().GetLV().Eta();
      float etal	= leps.at(0).ref().GetFourMom().GetLV().Eta();
      float sintheta	= sin(acos(tanh((etah-etal)/2)));
      float a_T	= aT(taus.at(0).ref(),leps.at(0).ref());
      TVector3 tHat	= GetEventAxis(leps.at(0).ref(), taus.at(0).ref());
      TVector3 EtCrossT	= Etvec.Cross(tHat);
      float dphi_l_MET = abs(Etvec.DeltaPhi(leps.at(0).ref().GetFourMom().GetLV().Vect()));
      float dphi_h_MET = abs(Etvec.DeltaPhi(taus.at(0).ref().GetFourMom().GetLV().Vect()));
      TVector3 pT_l	= leps.at(0).ref().GetFourMom().GetLV().Vect();
      pT_l.SetZ(0);
      TVector3 pT_h	= taus.at(0).ref().GetFourMom().GetLV().Vect();
      pT_h.SetZ(0);
      TVector3 muonVec  = leps.at(0).ref().GetFourMom().GetLV().Vect();
      float muonX	= muonVec.X(); // this is v1_x
      float muonY	= muonVec.Y(); // this is v1_y
      TVector3 tauVec   = taus.at(0).ref().GetFourMom().GetLV().Vect();
      float tauX	= tauVec.X(); // this is v2_x
      float tauY	= tauVec.Y(); // this is v2_y
      float Ex		= Etvec.X();
      float Ey		= Etvec.Y();
      float det		= muonX*tauY - tauX*muonY;
      float a		= (tauY*Ex - tauX*Ey)/det;
      float b		= (muonX*Ey - muonY*Ex)/det;
      TLorentzVector nu1(a*muonVec,(a*muonVec).Mag()); 
      TLorentzVector nu2(b*tauVec,(b*tauVec).Mag());
      TLorentzVector nuSum = nu1+nu2;
      TLorentzVector muonLV	= leps.at(0).ref().GetFourMom().GetLV();
      TLorentzVector tauLV	= taus.at(0).ref().GetFourMom().GetLV();
      float mTauNu		= (nuSum + tauLV).M() / 1.e3;
      float mMuTau		= (tauLV + muonLV).M() / 1.e3;
      float mMuNu		= (muonLV + nuSum).M() / 1.e3;
      float etaNu	       = nuSum.Eta();
      float mT2dagger = sqrt( pow(leps.at(0).ref().GetFourMom().GetLV().Et() + nuSum.Et() ,2) - (pT_l + Etvec).Mag2() );
      float sinthetahat		= sin(acos(tanh((etaNu-etal)/2)));
      float m2Hat		= mT2dagger / sinthetahat / 1.e3;
      float mHat		= (nuSum + leps.at(0).ref().GetFourMom().GetLV()+taus.at(0).ref().GetFourMom().GetLV()).M() / 1.e3;
      float m3Hat		= sqrt(mTauNu*mTauNu + mMuTau*mMuTau + mMuNu*mMuNu);
      
      // let's plot these new variables alright?????
      FillTH1(hmMuNu, mMuNu);
      FillTH1(hmTauNu, mTauNu);
      FillTH1(hmMuTau, mMuTau);
      FillTH1(hm2Hat, m2Hat);
      FillTH1(hmHat, mHat);
      FillTH1(hm3Hat, m3Hat);
      FillTH1(hEtaNu, nuSum.Eta());
      FillTH1(hRhoNu, (nu2.P() - nu1.P()) / nuSum.P());
      FillTH2(hmMuNuvsm2Hat, m2Hat, mMuNu);
      FillTH2(hmMuTauvsmMuNu, mMuNu, mMuTau );
      FillTH2(hmHatvsm3Hat, m3Hat, mHat);
      FillTH2(hSinThetaStarvsSinThetaHat, sinthetahat, sintheta );
      FillTH2(hEtvsNu, nuSum.P() / 1.e3, Et / 1.e3 );
      FillTH2(hPhiEtvsPhiNu,  nuSum.Phi()/TMath::Pi(), Etvec.Phi()/TMath::Pi() );
      FillTH2(hkMuvskTau, b, a);

      float mT_MET_l  = sqrt( pow(leps.at(0).ref().GetFourMom().GetLV().Et() + Et,2) -
			       (pT_l + Etvec).Mag2() );
      mT_MET_l	       = mT_MET_l / 1e3;
      TVector3 a_Tvec	= aTvec(taus.at(0).ref(),leps.at(0).ref());
      TVector3 AThat	= a_Tvec.Unit();
      float AThatdotMET= AThat.Dot(Etvec); 
      pT_l.SetZ(0);
      float betabar			= abs(Etvec.DeltaPhi(tHat));
      if( betabar > TMath::Pi() / 2) betabar = TMath::Pi() - betabar;
      float alphabar		= abs(leps.at(0).ref().GetFourMom().GetLV().Vect().DeltaPhi(tHat));
      if( alphabar > TMath::Pi() / 2) alphabar = TMath::Pi() - alphabar;
      TVector3 cT	= pT_l - pT_l.Dot(tHat) * tHat;
      float aTdotMET   = a_Tvec.Dot(Etvec) / 1e6;
      float sumcos	= TMath::Cos(abs(leps.at(0).ref().GetFourMom().GetLV().Vect().DeltaPhi(Etvec))) +
	TMath::Cos(taus.at(0).ref().GetFourMom().GetLV().Vect().DeltaPhi(Etvec));
      float mvis_MET_l	= sqrt(pow((leps.at(0).ref().GetFourMom().GetLV().E()+Et),2)
			       -(Etvec + pT_l).Mag2());
      mvis_MET_l       	/= 1e3;
      float mvis	= (leadTau->GetFourMom().GetLV() + leadLep->GetFourMom().GetLV()).M();
      mvis		/= 1e3;
      float mTlh      	= sqrt( pow(taus.at(0).ref().GetFourMom().GetLV().Et() + 
				    leps.at(0).ref().GetFourMom().GetLV().Et(), 2) -
				(pT_l + pT_h).Mag2() );
      mTlh		= mTlh / 1e3;
      float mlh	= (taus.at(0).ref().GetFourMom().GetLV() +
			   leps.at(0).ref().GetFourMom().GetLV() ).M();
      mlh		= mlh / 1e3;

      float mT3_2	= sqrt( pow(sqrt(pth*ptl + mTlh*mTlh*1e6)+ Et, 2) - 
				(pT_l + pT_h + Etvec ).Mag2() );
      mT3_2	        = mT3_2 / 1e3;
      float mT3	= sqrt( pow(taus.at(0).ref().GetFourMom().GetLV().Et() + 
				    leps.at(0).ref().GetFourMom().GetLV().Et() +
				    Et,2) - 
				(pT_l + pT_h + Etvec).Mag2()  );
      mT3		= mT3 / 1e3;
      float twist_MET_l= abs(atan( dphi_l_MET / (abs(etal))));
      float twist_MET_h= abs(atan( dphi_h_MET / (abs(etah))));
      float twist_lh	= abs(atan( dphi_lh / (abs(etah-etal))));
      int tauOL = 0;
      taus.at(0).ref().GetVar(Cut::TauOverlap,tauOL);
      FillTH1(hRho, rho);
      FillTH1(hPt_tau_reco, pth/1e3);
      FillTH1(hPt_mu_reco, ptl/1e3);
      FillTH1(hEta_tau_reco, etah);
      FillTH1(hEta_mu_reco, etal);
      FillTH2(hM_MT_div_SinTheta, mTlh / sintheta, mlh );
      FillTH2(hDPhiVsMstar2, mT_MET_l / sintheta, abs(dphi_l_MET) / TMath::Pi() );
      FillTH2(hDPhiVsMstar3, mT3 / sintheta , abs(dphi_l_MET) / TMath::Pi());
      FillTH2(hDPhiVsMT2, mT_MET_l , abs(dphi_l_MET) / TMath::Pi());
      FillTH2(hDPhiVsMT3, mT3 , abs(dphi_l_MET) / TMath::Pi());
      FillTH1(hM_lh, mlh );
      FillTH1(hTwist_MET_l, twist_MET_l / TMath::Pi() );
      FillTH2(hTwist_DPhi, abs(dphi_l_MET) / TMath::Pi(), twist_MET_l / TMath::Pi() );
      FillTH1(hMT3, mT3);
      FillTH1(hMT3_2, mT3_2);
      FillTH2(hMT_1_2, mT3_2, mT3);
      FillTH1(hMstar3, mT3 / sintheta );
      FillTH1(hMstar3DivPt, mT3 / sintheta / (pth / 1e3) );
      FillTH1(hMstar2, mT_MET_l / sintheta );
      FillTH1(hMstar2DivPt, mT_MET_l / sintheta / (pth / 1e3) );
      FillTH1(hPhi_MET_l, abs(dphi_l_MET) / TMath::Pi() );
      FillTH2(hMT2_ATdotMET, a_Tvec.Dot(Etvec) / 1e6, mT_MET_l);
      FillTH1(hATdotMET_div_MT, a_Tvec.Dot(Etvec) / 1e6 / mT_MET_l );
      FillTH1(hATdotMET_tim_MT, a_Tvec.Dot(Etvec) / 1e6 * mT_MET_l );
      FillTH2(hMT_DPhi, abs(dphi_l_MET) / TMath::Pi(), mT_MET_l);
      int lepcharge = leps.at(0).ref().GetCharge();
      float lep_eta   = abs( leps.at(0).ref().GetFourMom().GetLV().Eta() );
      float mt = sqrt( pow(leps.at(0).ref().GetFourMom().GetLV().Et() + Et,2) -
			(pT_l + Etvec).Mag2() );
      float mrem	= sqrt(2*Et*ptl*(2-sumcos));
      float mremstar	= mrem / sintheta;
      mrem		/= 1e3;
      mremstar		/= 1e3;

      //####### ATLAS CUTS #######//
      if( mt < 50.0e3 && 
	  sumcos > -0.15 && 
	  mvis > 45.0 && mvis < 85.0) 
	FillTH1(hPassAtlasCuts3, 1);
      else FillTH1(hPassAtlasCuts3, 0);
      if( mt < 50.0e3 && 
	  sumcos > -0.15 && 
	  mvis > 45.0 && mvis < 85.0 &&
	  ptl < 40e3 &&
	  dphi_lh > 2.4) 
	FillTH1(hPassAtlasCuts, 1);
      else FillTH1(hPassAtlasCuts, 0);
      //####### ATLAS CUTS #######//

      //####### 2D correlations #######//
      FillTH2(hMstar3vsMstar2, mT_MET_l/sintheta, mT3/sintheta);
      FillTH2(hMvisvsMstar2Sq, pow(mT_MET_l/sintheta,2), pow(mvis,2));
      FillTH2(hPtAsymvsMstar2, mT_MET_l/sintheta, (pth-ptl)/(pth+ptl));
      FillTH2(hMremvsMstar2, mT_MET_l/sintheta, mrem);
      FillTH2(hMremstarvsMstar2, mT_MET_l/sintheta, mremstar);
      FillTH2(hMremvsMstar3, mT3/sintheta, mrem);
      FillTH2(hMremstarvsMstar3,  mT3/sintheta, mremstar);
      FillTH2(hAtvsMstar2, mT_MET_l/sintheta, a_T/1e3);
      FillTH2(hMvisvsMstar2, mT_MET_l/sintheta, mvis);
      FillTH2(hPhistarvsMstar2, mT_MET_l/sintheta, log10(phistar));
      //####### 2D correlations #######//

      FillTH1(hDPhi_match, dphi_l_MET / TMath::Pi() );
      FillTH1(hQeta, lepcharge * lep_eta);
      FillTH2(hEtaVsMT2, mT_MET_l, lep_eta );
      FillTH2(hMT2vsMvis, mvis, mT_MET_l );
      FillTH1(hSumCos, sumcos);
      if( mT_MET_l / sintheta < 30 ){
	FillTH1(hMvisMstar2cut, mvis);
	FillTH1(hMstar3Mstar2cut, mT3 / sintheta);
      }
      FillTH1(hMvis, mvis);
      FillTH1(hPtMu, ptl / 1.0e3 );
      FillTH1(hMvis_l_MET, mvis_MET_l);
      FillTH1(hPhiStarDivATdotMET, log10(abs(phistar / aTdotMET)));
      FillTH1(hATdotMETdivPhiStar, aTdotMET / phistar);
      FillTH1(hATdotMETtimesPhiStar, aTdotMET * phistar);
      FillTH1(hAT, a_T / 1000);
      FillTH1(hMT, mT3);
      FillTH1(hATdotMET, aTdotMET);
      FillTH1(hMT_MET_l, mT_MET_l );
      FillTH2(hATdotMET_MT, mT_MET_l, aTdotMET);
      FillTH2(hATdotMET_PhiStar, log10(phistar), aTdotMET);
      FillTH1(hPhiStar, log10(phistar));
      FillTH1(hAThatdotMET, AThatdotMET );
      FillTH1(hEt_semilep, Etvec.Mag());
      FillTH1(hEt_phi_semilep, Et_phi);
      FillTH1(hPhi_acop_semilep_bkg, log10(phiacop));

      
      TLorentzVector vl = leps.at(0)->GetFourMom().GetLV();
      TLorentzVector vh = taus.at(0)->GetFourMom().GetLV();

      TVector3 metvec = Etvec;

      float d = GetDelta(vl,vh,metvec);

      if( d>0 ) {
	FillTH2(hMvisvsMstar2HasDelta, mT_MET_l/sintheta, mvis);
	FillTH1(hDelta, d);
      }
      


      if( d> 0){
	FillTH2(hRhovsDelta, d, rho );
	FillTH2(hAtvsDelta, d, a_T / 1e3 );
	FillTH2(hSumcosvsDelta, d, sumcos );
	FillTH2(hAtDotMETvsDelta, d, aTdotMET );
	FillTH2(hDPhilMETvsDelta, d, dphi_l_MET / TMath::Pi() );
	FillTH2(hDPhilhvsDelta, d, dphi_lh / TMath::Pi() );
	FillTH2(hPhistarvsDelta, d, log10(phistar) );
	FillTH2(hPtMuvsDelta, d, ptl / 1.0e3 );
      }

      float gamma = GetGamma(vl, vh, metvec);
      //ROI1:
      //0.0350066 yip 0.535142 83 13
      if( gamma > 0 && gamma <=83 ){
	FillTH1(hRoi1At, a_T / 1e3);
	FillTH1(hRoi1Mvis, mvis);
	FillTH1(hRoi1Mvis_l_MET, mvis_MET_l);
	FillTH1(hRoi1Mt, mT_MET_l);
	FillTH1(hRoi1Mstar3, mT3 / sintheta);
	FillTH1(hRoi1Mstar2, mT_MET_l / sintheta);
	FillTH1(hRoi1Phistar, log10(phistar));
	FillTH1(hRoi1AtDotMet, aTdotMET);
	FillTH1(hRoi1Et, Et/1e3);
	FillTH1(hRoi1Mrem, mrem);
	FillTH1(hRoi1Mremstar, mremstar);
	FillTH1(hRoi1DPhiLepMet, dphi_l_MET / TMath::Pi());
	FillTH1(hRoi1DPhiLepHad, dphi_lh / TMath::Pi());
	FillTH1(hRoi1DPhiHadMet, dphi_h_MET / TMath::Pi());
	FillTH1(hRoi1TwistLepMet, twist_MET_l / TMath::Pi());
	FillTH1(hRoi1TwistLepHad, twist_lh / TMath::Pi());
	FillTH1(hRoi1TwistMetHad, twist_MET_h / TMath::Pi());
	FillTH1(hRoi1SumCos, sumcos);
	FillTH1(hRoi1PtRatio, pth / ptl );
	FillTH1(hRoi1PtAsym, (pth-ptl)/(pth+ptl) );
	FillTH1(hRoi1PtMu, ptl / 1.0e3 );
	FillTH1(hRoi1Mstar3DivPt, mT3 / sintheta / (pth / 1e3) );
	FillTH1(hRoi1Mstar2DivPt, mT_MET_l / sintheta / (pth / 1e3) );
      }

      //ROI2: 0.0500554  0.616288 90 0
      if( gamma > 0 && gamma <=90 ){
	FillTH1(hRoi2At, a_T / 1e3);
	FillTH1(hRoi2Mvis, mvis);
	FillTH1(hRoi2Mvis_l_MET, mvis_MET_l);
	FillTH1(hRoi2Mt, mT_MET_l);
	FillTH1(hRoi2Mstar3, mT3 / sintheta);
	FillTH1(hRoi2Mstar2, mT_MET_l / sintheta);
	FillTH1(hRoi2Phistar, log10(phistar));
	FillTH1(hRoi2AtDotMet, aTdotMET);
	FillTH1(hRoi2Et, Et/1e3);
	FillTH1(hRoi2Mrem, mrem);
	FillTH1(hRoi2Mremstar, mremstar);
	FillTH1(hRoi2DPhiLepMet, dphi_l_MET / TMath::Pi());
	FillTH1(hRoi2DPhiLepHad, dphi_lh / TMath::Pi());
	FillTH1(hRoi2DPhiHadMet, dphi_h_MET / TMath::Pi());
	FillTH1(hRoi2TwistLepMet, twist_MET_l / TMath::Pi());
	FillTH1(hRoi2TwistLepHad, twist_lh / TMath::Pi());
	FillTH1(hRoi2TwistMetHad, twist_MET_h / TMath::Pi());
	FillTH1(hRoi2SumCos, sumcos);
	FillTH1(hRoi2PtRatio, pth / ptl );
	FillTH1(hRoi2PtAsym, (pth-ptl)/(pth+ptl) );
	FillTH1(hRoi2PtMu, ptl / 1.0e3 );
	FillTH1(hRoi2Mstar3DivPt, mT3 / sintheta / (pth / 1e3) );
	FillTH1(hRoi2Mstar2DivPt, mT_MET_l / sintheta / (pth / 1e3) );
      }
      

      if( gamma> 0){
	if(d>0) FillTH2(hGammavsDelta, d, gamma );
	if(d>0) FillTH2(hRhovsDeltaHasGamma, d, rho);
	FillTH1(hGamma, gamma);
	FillTH2(hRhovsGamma, gamma, rho );
	FillTH2(hAtvsGamma, gamma, a_T / 1e3 );
	FillTH2(hSumcosvsGamma, gamma, sumcos );
	FillTH2(hAtDotMETvsGamma, gamma, aTdotMET );
	FillTH2(hDPhilMETvsGamma, gamma, dphi_l_MET / TMath::Pi() );
	FillTH2(hDPhilhvsGamma, gamma, dphi_lh / TMath::Pi() );
	FillTH2(hPhistarvsGamma, gamma, log10(phistar) );
	FillTH2(hMstar3vsGamma, gamma, mT3 / sintheta );
	FillTH2(hMstar3DivPtvsGamma, gamma, mT3 / sintheta / (pth / 1e3) );
	FillTH2(hPtMuvsGamma, gamma, ptl / 1.0e3 );
      }

      if( gamma > 0 && gamma<=80 ){
	if(d>0) FillTH1(hPassGammaDeltaRhovsDelta, d, rho);
	FillTH1(hPassGammaDelta,1);
	FillTH1(hPassGammaDeltaAt, a_T / 1e3);
	FillTH1(hPassGammaDeltaMvis, mvis);
	FillTH1(hPassGammaDeltaMvis_l_MET, mvis_MET_l);
	FillTH1(hPassGammaDeltaMt, mT_MET_l);
	FillTH1(hPassGammaDeltaMstar3, mT3 / sintheta);
	FillTH1(hPassGammaDeltaMstar2, mT_MET_l / sintheta);
	FillTH1(hPassGammaDeltaPhistar, log10(phistar));
	FillTH1(hPassGammaDeltaAtDotMet, aTdotMET);
	FillTH1(hPassGammaDeltaEt, Et/1e3);
	FillTH1(hPassGammaDeltaMrem, mrem);
	FillTH1(hPassGammaDeltaMremstar, mremstar);
	FillTH1(hPassGammaDeltaDPhiLepMet, dphi_l_MET / TMath::Pi());
	FillTH1(hPassGammaDeltaDPhiLepHad, dphi_lh / TMath::Pi());
	FillTH1(hPassGammaDeltaDPhiHadMet, dphi_h_MET / TMath::Pi());
	FillTH1(hPassGammaDeltaTwistLepMet, twist_MET_l / TMath::Pi());
	FillTH1(hPassGammaDeltaTwistLepHad, twist_lh / TMath::Pi());
	FillTH1(hPassGammaDeltaTwistMetHad, twist_MET_h / TMath::Pi());
	FillTH1(hPassGammaDeltaSumCos, sumcos);
	FillTH1(hPassGammaDeltaPtRatio, pth / ptl );
	FillTH1(hPassGammaDeltaPtAsym, (pth-ptl)/(pth+ptl) );
	FillTH1(hPassGammaDeltaPtMu, ptl / 1.0e3 );
	FillTH1(hPassGammaDeltaMstar3DivPt, mT3 / sintheta / (pth / 1e3) );
	FillTH1(hPassGammaDeltaMstar2DivPt, mT_MET_l / sintheta / (pth / 1e3) );
      }
      else {
	if(d>0) FillTH1(hNotPassGammaDeltaRhovsDelta, d, rho);
	FillTH1(hNotPassGammaDeltaAt, a_T / 1e3);
	FillTH1(hNotPassGammaDeltaMvis, mvis);
	FillTH1(hNotPassGammaDeltaMvis_l_MET, mvis_MET_l);
	FillTH1(hNotPassGammaDeltaMt, mT_MET_l);
	FillTH1(hNotPassGammaDeltaMstar3, mT3 / sintheta);
	FillTH1(hNotPassGammaDeltaMstar2, mT_MET_l / sintheta);
	FillTH1(hNotPassGammaDeltaPhistar, log10(phistar));
	FillTH1(hNotPassGammaDeltaAtDotMet, aTdotMET);
	FillTH1(hNotPassGammaDeltaEt, Et/1e3);
	FillTH1(hNotPassGammaDeltaMrem, mrem);
	FillTH1(hNotPassGammaDeltaMremstar, mremstar);
	FillTH1(hNotPassGammaDeltaDPhiLepMet, dphi_l_MET / TMath::Pi());
	FillTH1(hNotPassGammaDeltaDPhiLepHad, dphi_lh / TMath::Pi());
	FillTH1(hNotPassGammaDeltaDPhiHadMet, dphi_h_MET / TMath::Pi());
	FillTH1(hNotPassGammaDeltaTwistLepMet, twist_MET_l / TMath::Pi());
	FillTH1(hNotPassGammaDeltaTwistLepHad, twist_lh / TMath::Pi());
	FillTH1(hNotPassGammaDeltaTwistMetHad, twist_MET_h / TMath::Pi());
	FillTH1(hNotPassGammaDeltaSumCos, sumcos);
	FillTH1(hNotPassGammaDeltaPtRatio, pth / ptl );
	FillTH1(hNotPassGammaDeltaPtAsym, (pth-ptl)/(pth+ptl) );
	FillTH1(hNotPassGammaDeltaPtMu, ptl / 1.0e3 );
	FillTH1(hNotPassGammaDeltaMstar3DivPt, mT3 / sintheta / (pth / 1e3) );
	FillTH1(hNotPassGammaDeltaMstar2DivPt, mT_MET_l / sintheta / (pth / 1e3) );
	FillTH1(hPassGammaDelta,0);
      }

      // ### ROIs ### //
    }
  }
}



