// This is -*- c++ -*-
#ifndef ANP_PLOTEVENT_H
#define ANP_PLOTEVENT_H

/**********************************************************************************
 * @Package: PhysicsTaus
 * @Class  : PlotEvent
 * @Author : Rustem Ospanov, Ricardo Woelker
 *
 * @Brief  :
 * 
 *  Prepare and plot event
 * 
 **********************************************************************************/

// Common
#include "PhysicsTaus/PlotBase.h"
#include "PhysicsData/RecoEvent.h"
//#include "PhysicsData/TrueEvent.h"

// Local
#include "PhysicsTaus/HistKey.h"
#include "PhysicsTaus/CutBase.h"

namespace Anp
{
  class PlotEvent: public virtual PlotBase, public virtual CutBase
  {
  public:

    PlotEvent();
    virtual ~PlotEvent() {}
    
    void PlotConf(const Registry &reg);
    
    bool PlotInit();

    void PlotExec();

    void PlotDone();

  private:

    template<class T> void SelectObjects(const std::vector<Ptr<T> > &ptrs, std::vector<Ptr<T> > &outs, CutFlow &cut);
    
    Ptr<CandEvent> PrepCandEvent(const RecoEvent &reco);

    void FillCandEvent(CandEvent &cand);

    void FillCandRecObjects(CandEvent &cand);

    bool IsHadronicBool (const TruthPart &particle, const std::vector<Ptr<TruthPart> > &AllTruths) const;

    bool IsLeptonicBool (const TruthPart &particle, const std::vector<Ptr<TruthPart> > &AllTruths) const;

    bool DecaysToPdg (const TruthPart &particle, const std::vector<Ptr<TruthPart> > &AllTruths, const int pdg) const;

    bool IsTauPionNuDecay (const TruthPart &particle, const std::vector<Ptr<TruthPart> > &AllTruths) const;

    bool IsFromTauDecay (const TruthPart &particle, const std::vector<Ptr<TruthPart> > &AllTruths) const;

    bool IsHadron (const TruthPart &p) const;

    bool MuonTrig(RecMuon &muon, CandEvent &cand) ;

    bool GammaDeltaPass(TLorentzVector& vl, TLorentzVector& vh, TVector3& METvec) const;

    float GetDelta(TLorentzVector &vl, TLorentzVector &vh, TVector3 &METvec) const;

    float GetGamma(TLorentzVector &vl, TLorentzVector &vh, TVector3 &METvec) const;
    
    float GetMT(float ptl, float dphi, float met);

    template <class T, class T2> float PhiStarEta(T &p0, T2 &p1);

    template <class T, class T2> float aT(T &p0, T2 &p1);

    template <class T, class T2> TVector3 GetEventAxis(T &p0, T2 &p1);

    template <class T, class T2> TVector3 aTvec(T &p0, T2 &p1);

    void PlotCandEvent(CandEvent &cand);

  private:

    bool                        fComputeMll;

    Ptr<CutFlow>                fCutEvent;
    Ptr<CutFlow>                fCutJet;
    Ptr<CutFlow>                fCutElec;
    Ptr<CutFlow>                fCutMuon;
    Ptr<CutFlow>                fCutTau;
    Ptr<CutFlow>		fCutTrigElec;
    Ptr<CutFlow>		fCutTrigMuon;
    Ptr<CutFlow>                fCutTruth;
    Ptr<CutFlow>                fCutMuonPre;
    Ptr<CutFlow>                fCutElecPre;
    Ptr<CutFlow>                fCutTruthTau;
    Ptr<CutFlow>		fCutTruthElec;
    Ptr<CutFlow>		fCutTruthMuon;

    Ptr<HistKey>                fHistEvent;
    Ptr<HistKey>                fHistMuon;
    Ptr<HistKey>                fHistElec;
    Ptr<HistKey>                fHistTau;    
    Ptr<HistKey>                fHistTruth;    
    Ptr<HistKey>                fHistDihad;    
    Ptr<HistKey>                fHistDilep;    
    Ptr<HistKey>                fHistSemilep;    
    Ptr<HistKey>		fHistltau;
    
    // 2017-04-27 Test some TH3s

    TH3			       *hThreeD_RhoMvisMstar;

    // 2017-06-21 new 3D MET plots
    TH1			       *hmMuNu;
    TH1			       *hmTauNu;
    TH1			       *hmMuTau;
    TH1			       *hm2Hat;
    TH1			       *hmHat;
    TH1			       *hm3Hat;
    TH1			       *hEtaNu;
    TH2			       *hmMuNuvsm2Hat;
    TH2			       *hmHatvsm3Hat;
    TH2			       *hSinThetaStarvsSinThetaHat;
    TH2			       *hEtvsNu;
    TH2			       *hPhiEtvsPhiNu;
    TH2			       *hmMuTauvsmMuNu;
    TH2			       *hkMuvskTau;
    TH1			       *hRhoNu;
    

    // 09/03/17 new hists
    TH1			       *hEt_softTrk;
    TH1			       *hEt_softClus;
    TH1			       *hEt_sumet;
    TH1			       *hEt;
    TH2			       *hPhistarAt2D;
    TH2			       *hMtPhistar2D;
    TH2			       *hEtAt2D;

    TH1			       *hGamma;
    TH1			       *hDelta;
    TH1			       *hRho;

    TH2			       *hRhoBinary;
    TH2			       *hRhovsDelta;
    TH2			       *hAtvsDelta;
    TH2			       *hAtDotMETvsDelta;
    TH2			       *hSumcosvsDelta;
    TH2			       *hDPhilMETvsDelta;
    TH2			       *hDPhilhvsDelta;
    TH2			       *hPhistarvsDelta;
    TH2			       *hPtMuvsDelta;

    TH2			       *hGammavsDelta;
    TH2			       *hRhovsGamma;
    TH2			       *hMstar3vsGamma;
    TH2			       *hMstar3DivPtvsGamma;
    TH2			       *hAtvsGamma;
    TH2			       *hAtDotMETvsGamma;
    TH2			       *hSumcosvsGamma;
    TH2			       *hDPhilMETvsGamma;
    TH2			       *hDPhilhvsGamma;
    TH2			       *hPhistarvsGamma;
    TH2			       *hPtMuvsGamma;
    TH2			       *hRhovsDeltaHasGamma;


    TH1			       *hPassGammaDelta;

    TH1			       *hDoLeadTau;
    TH1			       *hDoLeadMu;
    TH1			       *hTauBDTScore;

    TH1			       *hPtRatio;
    TH1			       *hPtAsym;
    
    TH2			       *hMstar3vsMstar2;
    TH2			       *hMvisvsMstar2Sq;
    TH2			       *hPtAsymvsMstar2;
    TH2			       *hMremvsMstar2;
    TH2			       *hMremvsMstar3;
    TH2			       *hMremstarvsMstar3;
    TH2			       *hMremstarvsMstar2;
    TH2			       *hAtvsMstar2;
    TH2			       *hPhistarvsMstar2;
    TH2			       *hMvisvsMstar2;
    TH2			       *hMvisvsMstar2HasDelta;

    TH1			       *hMstar3DivPt;
    TH1			       *hMstar2DivPt;


    // 25/10/16 Et histograms
    TH1			       *hSingleJetPt25;

    TH1			       *hRoi1Mstar2;
    TH1			       *hRoi1Mstar3DivPt;
    TH1			       *hRoi1Mstar2DivPt;
    TH1			       *hRoi1AtDotMetDivMet;
    TH1			       *hRoi1SingleJetPt25;
    TH1			       *hRoi1Mvis;
    TH1			       *hRoi1Mvis_l_MET;
    TH1			       *hRoi1At;
    TH1			       *hRoi1PtRatio;
    TH1			       *hRoi1PtAsym;
    TH1			       *hRoi1PtMu;
    TH1			       *hRoi1DPhiLepMet;
    TH1			       *hRoi1Mt;
    TH1			       *hRoi1SumCos;
    TH1			       *hRoi1Njets;
    TH1			       *hRoi1Njets25;
    TH1			       *hRoi1NjetsMinusNtaus;
    TH1			       *hRoi1DPhiLepHad;
    TH1			       *hRoi1DPhiHadMet;
    TH1			       *hRoi1Et;
    TH1			       *hRoi1TwistLepMet;
    TH1			       *hRoi1TwistLepHad;
    TH1			       *hRoi1TwistMetHad;
    TH1			       *hRoi1Mstar3;
    TH1			       *hRoi1Phistar;
    TH1			       *hRoi1AtDotMet;
    TH1			       *hRoi1SingleJetPt;
    TH1			       *hRoi1Mrem;
    TH1			       *hRoi1Mremstar;

    TH1			       *hRoi2Mstar2;
    TH1			       *hRoi2Mstar3DivPt;
    TH1			       *hRoi2Mstar2DivPt;
    TH1			       *hRoi2AtDotMetDivMet;
    TH1			       *hRoi2SingleJetPt25;
    TH1			       *hRoi2Mvis;
    TH1			       *hRoi2Mvis_l_MET;
    TH1			       *hRoi2PtRatio;
    TH1			       *hRoi2PtAsym;
    TH1			       *hRoi2PtMu;
    TH1			       *hRoi2DPhiLepMet;
    TH1			       *hRoi2Mt;
    TH1			       *hRoi2At;
    TH1			       *hRoi2SumCos;
    TH1			       *hRoi2Njets;
    TH1			       *hRoi2Njets25;
    TH1			       *hRoi2NjetsMinusNtaus;
    TH1			       *hRoi2DPhiLepHad;
    TH1			       *hRoi2DPhiHadMet;
    TH1			       *hRoi2Et;
    TH1			       *hRoi2TwistLepMet;
    TH1			       *hRoi2TwistLepHad;
    TH1			       *hRoi2TwistMetHad;
    TH1			       *hRoi2Mstar3;
    TH1			       *hRoi2Phistar;
    TH1			       *hRoi2AtDotMet;
    TH1			       *hRoi2SingleJetPt;
    TH1			       *hRoi2Mrem;
    TH1			       *hRoi2Mremstar;

    TH2			       *hNotPassGammaDeltaRhovsDelta;
    TH1			       *hNotPassGammaDeltaMstar2;
    TH1			       *hNotPassGammaDeltaMstar3DivPt;
    TH1			       *hNotPassGammaDeltaMstar2DivPt;
    TH1			       *hNotPassGammaDeltaAtDotMetDivMet;
    TH1			       *hNotPassGammaDeltaSingleJetPt25;
    TH1			       *hNotPassGammaDeltaMvis;
    TH1			       *hNotPassGammaDeltaMvis_l_MET;
    TH1			       *hNotPassGammaDeltaPtRatio;
    TH1			       *hNotPassGammaDeltaPtAsym;
    TH1			       *hNotPassGammaDeltaPtMu;
    TH1			       *hNotPassGammaDeltaDPhiLepMet;
    TH1			       *hNotPassGammaDeltaMt;
    TH1			       *hNotPassGammaDeltaAt;
    TH1			       *hNotPassGammaDeltaSumCos;
    TH1			       *hNotPassGammaDeltaNjets;
    TH1			       *hNotPassGammaDeltaNjets25;
    TH1			       *hNotPassGammaDeltaNjetsMinusNtaus;
    TH1			       *hNotPassGammaDeltaDPhiLepHad;
    TH1			       *hNotPassGammaDeltaDPhiHadMet;
    TH1			       *hNotPassGammaDeltaEt;
    TH1			       *hNotPassGammaDeltaTwistLepMet;
    TH1			       *hNotPassGammaDeltaTwistLepHad;
    TH1			       *hNotPassGammaDeltaTwistMetHad;
    TH1			       *hNotPassGammaDeltaMstar3;
    TH1			       *hNotPassGammaDeltaPhistar;
    TH1			       *hNotPassGammaDeltaAtDotMet;
    TH1			       *hNotPassGammaDeltaSingleJetPt;
    TH1			       *hNotPassGammaDeltaMrem;
    TH1			       *hNotPassGammaDeltaMremstar;

    TH2			       *hPassGammaDeltaRhovsDelta;
    TH1			       *hPassGammaDeltaMstar2;
    TH1			       *hPassGammaDeltaMstar3DivPt;
    TH1			       *hPassGammaDeltaMstar2DivPt;
    TH1			       *hPassGammaDeltaAtDotMetDivMet;
    TH1			       *hPassGammaDeltaSingleJetPt25;
    TH1			       *hPassGammaDeltaMvis;
    TH1			       *hPassGammaDeltaMvis_l_MET;
    TH1			       *hPassGammaDeltaPtRatio;
    TH1			       *hPassGammaDeltaPtAsym;
    TH1			       *hPassGammaDeltaPtMu;
    TH1			       *hPassGammaDeltaDPhiLepMet;
    TH1			       *hPassGammaDeltaMt;
    TH1			       *hPassGammaDeltaAt;
    TH1			       *hPassGammaDeltaSumCos;
    TH1			       *hPassGammaDeltaNjets;
    TH1			       *hPassGammaDeltaNjets25;
    TH1			       *hPassGammaDeltaNjetsMinusNtaus;
    TH1			       *hPassGammaDeltaDPhiLepHad;
    TH1			       *hPassGammaDeltaDPhiHadMet;
    TH1			       *hPassGammaDeltaEt;
    TH1			       *hPassGammaDeltaTwistLepMet;
    TH1			       *hPassGammaDeltaTwistLepHad;
    TH1			       *hPassGammaDeltaTwistMetHad;
    TH1			       *hPassGammaDeltaMstar3;
    TH1			       *hPassGammaDeltaPhistar;
    TH1			       *hPassGammaDeltaAtDotMet;
    TH1			       *hPassGammaDeltaSingleJetPt;
    TH1			       *hPassGammaDeltaMrem;
    TH1			       *hPassGammaDeltaMremstar;


    TH1			       *hPt_mu;
    TH1			       *hPt_e;
    TH1			       *hPt_tau;
    TH1			       *hSingleJetPt;
    TH2			       *hMstarAt2D;

    TH1			       *hSumCos;
    TH1			       *hSumCos_r1;
    TH1			       *hSumCos_r2;
    TH1			       *hSumCos_r3;
    TH2			       *hMT2vsMvis;
    TH2			       *hEtaVsMT2;
    TH1			       *hQeta;
    TH1			       *hQeta_r1;
    TH1			       *hQeta_r2;
    TH1			       *hQeta_r3;
    TH1			       *hMvisMstar2cut;
    TH1			       *hMstar3Mstar2cut;
    TH1			       *hMvis;
    TH1			       *hPtMu;
    TH1			       *hMvis_l_MET;
    TH1			       *hMvis_r1;
    TH1			       *hMvis_r2;
    TH1			       *hMvis_r3;
    TH2			       *hDPhiVsMstar2;
    TH2			       *hDPhiVsMstar3;
    TH2			       *hDPhiVsMT2;
    TH2			       *hDPhiVsMT3;
    TH2			       *hMT3_mu_h_vsMtautau;
    TH2			       *hMT3_e_h_vsMtautau;
    TH2			       *hMstar3_mu_h_vsMtautau;
    TH2			       *hMstar3_e_h_vsMtautau;
    TH2			       *hMT2_mu_h_vsMtautau;
    TH2			       *hMT2_e_h_vsMtautau;
    TH2			       *hMstar2_mu_h_vsMtautau;
    TH2			       *hMstar2_e_h_vsMtautau;
    TH2			       *hNusumPtVsMET;
    TH1			       *hEventPassTrig;
    TH1			       *hPassAtlasCuts;
    TH1			       *hPassAtlasCuts3;
    TH1			       *hN_elec;
    TH1			       *hmu_e_OLR;
    TH1			       *hmu_tau_OLR;
    TH1			       *he_tau_OLR;
    TH1			       *hM_mu_mu;
    TH1			       *hM_mu_e;
    TH1			       *hM_e_e;
    TH1			       *hTauTruthMatch;
    TH1			       *hAT_match;
    TH1			       *hAT_no_match;
    TH1			       *hAT_match_r1;
    TH1			       *hAT_no_match_r1;
    TH1			       *hAT_match_r2;
    TH1			       *hAT_no_match_r2;
    TH1			       *hAT_match_r3;
    TH1			       *hAT_no_match_r3;
    TH1			       *hDPhi_match;
    TH1			       *hDPhi_no_match;
    TH1			       *hDPhi_match_r1;
    TH1			       *hDPhi_no_match_r1;
    TH1			       *hDPhi_match_r2;
    TH1			       *hDPhi_no_match_r2;
    TH1			       *hDPhi_match_r3;
    TH1			       *hDPhi_no_match_r3;
    TH2			       *hMstar_M_mumu;
    TH2			       *hMTcomp;
    TH2			       *hMT2vsMT3;
    TH2			       *hMT2vsMT3man;
    TH2			       *hMstar3_M_mumu;
    TH2			       *hMstar3man_M_mumu;
    TH2			       *hMT3_mumu_vs_M_mumu;
    TH2			       *hMT_mumu_vs_M_mumu;
    TH1			       *hAT_add_Mvis;
    TH1			       *hAT_add_DPhi;
    TH1			       *hAT_add_MT2;
    TH1			       *hAT_add_Mstar2;
    TH1			       *hAT_add_Mstar3;
    TH1			       *hAT_add_SumCos;
    TH1			       *hAT_add70sig_Mvis;
    TH1			       *hAT_add70sig_DPhi;
    TH1			       *hAT_add70sig_MT2;
    TH1			       *hAT_add70sig_Mstar2;
    TH1			       *hAT_add80sig_Mstar2;
    TH1			       *hAT_add80sig_Mstar2_70sig_DPhi;
    TH1			       *hAT_add90sig_Mstar2;
    TH1			       *hAT_add70sig_Mstar3;
    TH1			       *hAT_add70sig_SumCos;
    TH1			       *hAT_add5bkg_Mvis;
    TH1			       *hAT_add5bkg_DPhi;
    TH1			       *hAT_add5bkg_MT2;
    TH1			       *hAT_add5bkg_Mstar2;
    TH1			       *hAT_add5bkg_Mstar3;
    TH1			       *hAT_add5bkg_SumCos;

    TH1			       *hAT_add_MT_DPhi;
    TH1			       *hPhi_MET_l_add;
    TH1			       *hTwist_MET_l;
    TH1			       *hTwist_MET_l_r1;
    TH1			       *hTwist_MET_l_r2;
    TH1			       *hTwist_MET_l_r3;
    TH2			       *hMT_DPhi;
    TH2			       *hTwist_DPhi;
    TH1			       *hATdotMET_tim_MT_r1;
    TH1			       *hATdotMET_tim_MT_r2;
    TH1			       *hATdotMET_tim_MT_r3;
    TH1			       *hATdotMET_tim_MT;
    TH1			       *hATdotMET_div_MT_r1;
    TH1			       *hATdotMET_div_MT_r2;
    TH1			       *hATdotMET_div_MT_r3;
    TH1			       *hATdotMET_div_MT;
    TH2			       *hMT2_ATdotMET;
    TH1			       *hPhi_MET_l;
    TH1			       *hPhi_MET_l_r1;
    TH1			       *hPhi_MET_l_r2;
    TH1			       *hPhi_MET_l_r3;
    TH1			       *hMstar_no_cut;
    TH1			       *hMstar_no_cut_r1;
    TH1			       *hMstar_no_cut_r2;
    TH1			       *hMstar_no_cut_r3;
    TH1			       *hMstar3;
    TH1			       *hMstar3_r1;
    TH1			       *hMstar3_r2;
    TH1			       *hMstar3_r3;
    TH1			       *hMstar2;
    TH1			       *hMstar2_r1;
    TH1			       *hMstar2_r2;
    TH1			       *hMstar2_r3;
    TH1			       *hMT3;
    TH1			       *hMT3_2;
    TH2			       *hMT_1_2;
    TH2			       *hM_MT_div_SinTheta;
    TH2			       *hSinThetaRecoVsTruth;
    TH1			       *hM_lh;
    TH1			       *hM_lh_r1;
    TH1			       *hM_lh_r2;
    TH1			       *hM_lh_r3;
    TH1			       *hM_lh_no_cut;
    TH1			       *hM_lh_no_cut_r1;
    TH1			       *hM_lh_no_cut_r2;
    TH1			       *hM_lh_no_cut_r3;
    TH1			       *hAThatdotMET_add;
    TH1			       *hAThatdotMET;
    TH1			       *hAThatdotMET_r1;
    TH1			       *hAThatdotMET_r2;
    TH1			       *hAThatdotMET_r3;
    TH1			       *hMT_MET_add;
    TH1			       *hMT_MET_add_r1;
    TH1			       *hMT_MET_add_r2;
    TH1			       *hMT_MET_add_r3;
    TH1			       *hMjet;
    TH1			       *hMjetl;
    TH1			       *hMjet_r1;
    TH1			       *hMjetl_r1;
    TH1			       *hMjet_r2;
    TH1			       *hMjetl_r2;
    TH1			       *hMjet_r3;
    TH1			       *hMjetl_r3;
    TH1			       *hPhiStar;
    TH1			       *hPhiStar_r1;
    TH1			       *hPhiStar_r2;
    TH1			       *hPhiStar_r3;
    TH1			       *hMljet;
    TH1			       *hMljet_r1;
    TH1			       *hMtljet;
    TH1			       *hMtljet_r1;
    TH2			       *hATdotMET_PhiStar;
    TH2			       *hATdotMET_PhiStar_r1;
    TH2			       *hATdotMET_MT;
    TH2			       *hATdotMET_MT_r1;
    TH2			       *hATdotMET_MT_r2;
    TH2			       *hATdotMET_MT_r3;
    TH1			       *hPhiStarDivATdotMET;
    TH1			       *hPhiStarDivATdotMET_r1;
    TH1			       *hATdotMETdivPhiStar;
    TH1			       *hATdotMETtimesPhiStar;
    TH1			       *hATdotMETdivPhiStar_r1;
    TH1			       *hATdotMETtimesPhiStar_r1;
    TH1			       *hAlpha;
    TH1			       *hBeta;
    TH1			       *hAT;
    TH1			       *hMT;
    TH1			       *hAT_r1;
    TH1			       *hMT_r1;
    TH1			       *hAT_r2;
    TH1			       *hMT_r2;
    TH1			       *hAT_r3;
    TH1			       *hMT_r3;
    TH2			       *hAlphaPt;
    TH2			       *hBetaMET;
    TH2			       *hMET_mT;
    TH1			       *hMT_MET_l;
    TH1			       *hATdotMET;
    TH1			       *hMT_MET_l_r1;
    TH1			       *hATdotMET_r1;
    TH1			       *hMT_MET_l_r2;
    TH1			       *hATdotMET_r2;
    TH1			       *hMT_MET_l_r3;
    TH1			       *hATdotMET_r3;
    TH1			       *hCos_Et_t_semilep;
    TH1			       *hCos_Et_t_dilep;
    TH1			       *hArccos_Et_t_semilep;
    TH1			       *hArccos_Et_t_dilep;
    TH1			       *hLogCos_Et_t_semilep;
    TH1			       *hLogCos_Et_t_dilep;
    TH1			       *hEt_phi_semilep;
    TH1			       *hEt_phi_dilep;
    TH1			       *hEt_semilep;
    TH1			       *hEt_dilep;
    TH1			       *hEt_cross_t;
    TH1			       *hEt_cross_t_semilep;
    TH1			       *hEt_cross_t_dilep;
    TH1			       *hEt_cross_t_abs_semilep;
    TH1			       *hEt_cross_t_abs_dilep;

    TH1			       *hEt_cross_t_dihad;
    TH1			       *hEt_dot_t_semilep;
    TH1			       *hEt_dot_t_dilep;

    TH1			       *hEt_dot_t_abs_semilep;
    TH1			       *hEt_dot_t_abs_dilep;
    //phistar cut histograms
    TH1			       *hPassPhiCutTau;
    TH1			       *hRecTausTruthMatched;
    //19/10/16 testhists
    TH1			       *hDeltaR_elec_mu;
    TH1			       *hTestHist0;
    TH1			       *hTestHist1;
    TH1			       *hTestHist2;
    TH1			       *hTestHist3;
    TH1			       *hTestHist4;
    TH1			       *hTestHist5;
    TH1			       *hTestHist6;
    //4/10/16 new histos
    TH1			       *hAt_semilep;
    TH1			       *hAt_dilep;
    TH1			       *hAt_dihad;
    TH1			       *hAt_semilep_bkg;
    TH1			       *hAt_dilep_bkg;
    TH1			       *hAt_dihad_bkg;
    TH1			       *hAt_semilep_bkg_truth;
    TH1			       *hAt_dilep_bkg_truth;
    TH1			       *hAt_dilep_prompt_bkg_truth;
    TH1			       *hAt_dihad_bkg_truth;

    TH1			       *hPt_semilep_had;
    TH1			       *hPt_semilep_lep;
    TH1			       *hPt_dihad;
    TH1			       *hPt_dilep;
    TH1			       *hEta_semilep_had;
    TH1			       *hEta_semilep_lep;
    TH1			       *hEta_dihad;
    TH1			       *hEta_dilep;
    TH1			       *hEta_abs_semilep_had;
    TH1			       *hEta_abs_semilep_lep;
    TH1			       *hEta_abs_dihad;
    TH1			       *hEta_abs_dilep;
    TH1			       *hPhiStarEta_semilep;
    TH1			       *hPhiStarEta_dihad;
    TH1			       *hPhiStarEta_dilep;
    TH1			       *hPhi_acop_semilep;
    TH1			       *hPhi_acop_dihad;
    TH1			       *hPhi_acop_dilep;
    TH1			       *hSin_theta_semilep;
    TH1			       *hSin_theta_dihad;
    TH1			       *hSin_theta_dilep;
    //background hists
    TH1			       *hPt_semilep_had_bkg;
    TH1			       *hPt_semilep_lep_bkg;
    TH1			       *hPt_dihad_bkg;
    TH1			       *hPt_dilep_bkg;
    TH1			       *hEta_semilep_had_bkg;
    TH1			       *hEta_semilep_lep_bkg;
    TH1			       *hEta_dihad_bkg;
    TH1			       *hEta_dilep_bkg;
    TH1			       *hEta_abs_semilep_had_bkg;
    TH1			       *hEta_abs_semilep_lep_bkg;
    TH1			       *hEta_abs_dihad_bkg;
    TH1			       *hEta_abs_dilep_bkg;
    TH1			       *hPhiStarEta_semilep_bkg;
    TH1			       *hPhiStarEta_dihad_bkg;
    TH1			       *hPhiStarEta_dilep_bkg;
    TH1			       *hPhi_acop_semilep_bkg;
    TH1			       *hPhi_acop_dihad_bkg;
    TH1			       *hPhi_acop_dilep_bkg;
    TH1			       *hSin_theta_semilep_bkg;
    TH1			       *hSin_theta_alt_semilep_bkg;
    TH1			       *hSin_theta_dihad_bkg;
    TH1			       *hSin_theta_dilep_bkg;
    TH1			       *hSin_theta_alt_dilep_bkg;
    TH1			       *hPt_semilep_had_bkg_truth;
    TH1			       *hPt_semilep_lep_bkg_truth;
    TH1			       *hPt_dihad_bkg_truth;
    TH1			       *hPt_dilep_bkg_truth;
    TH1			       *hEta_semilep_had_bkg_truth;
    TH1			       *hEta_semilep_lep_bkg_truth;
    TH1			       *hEta_dihad_bkg_truth;
    TH1			       *hEta_dilep_bkg_truth;
    TH1			       *hEta_abs_semilep_had_bkg_truth;
    TH1			       *hEta_abs_semilep_lep_bkg_truth;
    TH1			       *hEta_abs_dihad_bkg_truth;
    TH1			       *hEta_abs_dilep_bkg_truth;
    TH1			       *hPhiStarEta_semilep_bkg_truth;
    TH1			       *hPhiStarEta_dihad_bkg_truth;
    TH1			       *hPhiStarEta_dilep_bkg_truth;
    TH1			       *hPhi_acop_semilep_bkg_truth;
    TH1			       *hPhi_acop_dihad_bkg_truth;
    TH1			       *hPhi_acop_dilep_bkg_truth;
    TH1			       *hSin_theta_semilep_bkg_truth;
    TH1			       *hSin_theta_alt_semilep_bkg_truth;
    TH1			       *hSin_theta_dihad_bkg_truth;
    TH1			       *hSin_theta_dilep_bkg_truth;
    TH1			       *hSin_theta_alt_dilep_bkg_truth;


    //general tau hists
    TH1			       *hDPhi_tau_daughter_mu;
    TH1			       *hDPhi_tau_daughter_elec;
    TH1			       *hDPhi_tau_daughter_pion;
    TH1			       *hDPhi_tau_daughter_pion_3prong;
    TH1			       *hDPhi_tau_daughter_pi0;
    TH1			       *hDPhi_tau_daughter_kaon;
    TH1			       *hDPhi_tau_daughter_k0;
    TH1			       *hDPhi_tau_daughter_hadron;
    TH1			       *hDPhi_tau_daughter_hadron_sum;
    TH1			       *hPt_e_reco;
    TH1			       *hPt_mu_reco;
    TH1			       *hPt_tau_reco;
    TH1			       *hEta_e_reco;
    TH1			       *hEta_mu_reco;
    TH1			       *hEta_tau_reco;
    TH1			       *hPt_e_truth;
    TH1			       *hPt_mu_truth;
    TH1			       *hPt_tau_truth;
    TH1			       *hPt_had_truth;
    TH1			       *hEta_e_truth;
    TH1			       *hEta_mu_truth;
    TH1			       *hEta_tau_truth;
    TH1			       *hEta_had_truth;
    TH1			       *hPt_e_match;
    TH1			       *hPt_mu_match;
    TH1			       *hEta_e_match;
    TH1			       *hEta_mu_match;



    //now for the dihadronic decay
    //general hists
    TH2			       *hPhi_M_fraction_had_had;
    TH2			       *hPhi_M_fraction_sub_had_had;
    TH2			       *hPhi_M_fraction_alt_had_had;
    TH2			       *hPhi_acop_difference_M_fraction_had_had;
    TH2			       *hDPhi_pT_fraction_had_had;
    TH2			       *hPt_fraction_had_had;
    TH2			       *hM_fraction_pT_fraction_had_had;
    //angular dists
    TH1			       *hDPhi_tau_had;
    TH1			       *hDPhi_truth_reco_had_had;
    TH1			       *hDPhi_truth_reco_pi_nu_had_had;
    TH2			       *hDPhi_2D_had_had;
    TH1			       *hPhi_acop_tau_tau;
    TH1			       *hPhi_acop_had_had;
    TH1			       *hPhi_truth_had_had;
    TH1			       *hPhi_reco_had_had;
    TH1			       *hDPhi_sin_theta_truth_reco_had_had;
    TH2			       *hPhi_acop_truth_obs_had_had;
    TH2			       *hDPhi_phi_acop_truth_had_had;
    TH2			       *hDPhi_phi_acop_truth_lep_had;
    TH2			       *hDPhi_phi_acop_truth_lep_lep;

    //for the dileptonic decay
    TH2			       *hPhi_M_fraction_lep_lep;
    TH2			       *hPhi_M_fraction_sub_lep_lep;
    TH2			       *hPhi_acop_difference_M_fraction_lep_lep;
    TH2			       *hPhi_M_fraction_alt_lep_lep;
    TH2			       *hDPhi_pT_fraction_lep_lep;
    TH2			       *hPt_fraction_lep_lep;
    TH2			       *hM_fraction_pT_fraction_lep_lep;
    //angular dists
    TH1			       *hDPhi_tau_lep;
    TH1			       *hDPhi_truth_reco_mu_lep_lep;
    TH2			       *hDPhi_2D_mu_lep_lep;
    TH2			       *hDPhi_2D_elec_lep_lep;
    TH1			       *hDPhi_truth_reco_elec_lep_lep;
    TH1			       *hPhi_acop_lep_tau_tau;
    TH1			       *hPhi_acop_lep_lep;
    TH1			       *hPhi_truth_lep_lep;
    TH1			       *hPhi_reco_lep_lep;
    TH1			       *hDPhi_sin_theta_truth_reco_lep_lep;
    TH2			       *hPhi_acop_truth_obs_lep_lep;


    //for the semi decay
    TH2			       *hPhi_M_fraction_lep_had;
    TH2			       *hPhi_M_fraction_sub_lep_had;
    TH2			       *hPhi_M_fraction_lep_lep_had;
    TH2			       *hPhi_M_fraction_had_lep_had;
    TH2			       *hPhi_acop_difference_M_fraction_lep_had;
    TH2			       *hPhi_M_fraction_alt_lep_had;
    TH2			       *hDPhi_pT_fraction_lep_had;
    TH2			       *hDPhi_pT_fraction_lep_lep_had;
    TH2			       *hDPhi_pT_fraction_had_lep_had;
    TH2			       *hPt_fraction_lep_had;
    TH2			       *hPt_fraction_lep_lep_had;
    TH2			       *hPt_fraction_had_lep_had;
    TH2			       *hPt_fraction_lep_vs_had;
    TH2			       *hM_fraction_pT_fraction_lep_had;
    TH2			       *hM_fraction_pT_fraction_lep_lep_had;
    TH2			       *hM_fraction_pT_fraction_had_lep_had;
    //angular dists
    TH1			       *hDPhi_tau_lep_had;
    TH2			       *hDPhi_2D_lep_had;
    TH1			       *hDPhi_truth_reco_lep_lep_had;
    TH1			       *hDPhi_truth_reco_had_lep_had;
    TH1			       *hPhi_acop_lep_had_tau_tau;
    TH1			       *hPhi_acop_lep_had;
    TH1			       *hPhi_truth_lep_had;
    TH1			       *hPhi_reco_lep_had;
    TH1			       *hDPhi_sin_theta_truth_reco_lep_had;
    TH2			       *hPhi_acop_truth_obs_lep_had;

    //for l+tau plots
    TH2			       *hPt_fraction_lep_vs_had_l_tau;
    TH1			       *hPhiStarEta_l_tau;
    TH1			       *hMratio_l_tau;
    TH2			       *hDPhi_pT_fraction_lep_l_tau;
    TH2			       *hDPhi_pT_fraction_had_l_tau;
    TH2			       *hPhi_M_fraction_l_tau;
    TH2			       *hM_fraction_pT_fraction_had_l_tau;
    TH2			       *hM_fraction_pT_fraction_lep_l_tau;
    TH1			       *hPhiStarEta_truth_l_tau;
    TH1			       *hPhiStarEta_truth_l_tau_no_sin;
    TH1			       *hPhiStarEta_truth;
    TH1			       *hPhiStarEta_truth_no_sin;


  };

  //==============================================================================
  template<class T> inline void PlotEvent::SelectObjects(const std::vector<Ptr<T> > &ptrs, 
							 std::vector<Ptr<T> > &outs, 
							 CutFlow &cut)
  {
    for(const Ptr<T> &ptr: ptrs) {
      
      if(cut.PassCut(ptr.ref())) {
	outs.push_back(ptr);
      }
    }
  }
}

#endif
