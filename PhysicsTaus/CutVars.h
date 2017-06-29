// Dear emacs, this is -*- c++ -*-
#ifndef ANP_NEWVARS_H
#define ANP_NEWVARS_H

/**********************************************************************************
 * @Package: PhysicsTaus
 * @Author : Rustem Ospanov, Ricardo Woelker
 *
 * @Brief  :
 * 
 *  List of event variables for cuts
 * 
 **********************************************************************************/

// C/C++
#include <set>
#include <stdint.h>
#include <string>
#include <vector>

// ROOT
#include "Rtypes.h"

namespace Anp
{
  class Registry;

  namespace Cut
  {
    enum Var 
    {
      //
      // Object kinematic variables
      //
      NONE = 10000, 
      Charge,
      Et,      
      Eta,      
      EtaAbs,
      EtaID,
      EtaMS,
      Energy,
      Mass,
      Phi,
      Theta,
      Pt,
      Px,
      Py,
      Pz,
      ClusterEt,
      ClusterPt,
      ClusterEta,
      ClusterPhi,
      PtID,
      PtMS,
      PtOrg,
      SumPt,
      QoverP,

      PreSamplerB,
      EMB1,
      EMB2,
      EMB3,
      TileBar0,
      TileBar1,
      TileBar2,

      RecoBarcode,
      TrueBarcode,
      TrueBarcodeGeantElec,
      TrueBarcodeGeantPhoton,
      TrackBarcode,

      //
      // Object id variables
      //
      Author,
      Authors,
      AuthorMuonBoy,
      AuthorStaco,
      AuthorMuTag,
      AuthorMuGirl,
      BitCombined,
      BitStandAlone,
      BitTight,
      BitTightPP,
      BitMedium,
      BitMediumPP,
      BitLoose,
      BitLoosePP,
      BitFEB,
      BitOQ,
      BitRefFinal,
      BitLocHadTopo,

      IsoGradient,
      IsoGradientLoose,
      IsoLoose,
      IsoLooseTrackOnly,
      IsoTight,

      //
      // Electrons
      //
      ClPhi,
      ClEta,
      ClEt,
      EtaBE2,
      ElecConv,
      isLooseBLPixLH,
      isLooseLH,
      isLoosePP,
      isMediumLH,
      isMediumPP,
      isTightLH,
      isTightPP,
      isVeryLooseLH,
      isVeryTightLH,      

      Match_EF_e12Tvh_loose1,
      Match_EF_e22vh_medium1,
      Match_EF_e24vhi_medium1,
      Match_EF_e45_medium1,
      Match_EF_e60_medium1,

      //
      // Muons
      //
      isCombined,
      isLoose,
      isMedium,
      isTight,
      momBalSignif,
      numPrecLayers,
      scatCurvSignif,
      scatNeighSignif,
      Match_EF_mu13,
      Match_EF_mu18_tight,
      Match_EF_mu24_tight,
      Match_EF_mu24i_tight,
      Match_EF_mu36_tight,
      Match_EF_mu40_MSonly_barrel_tight,
      Match_EF_mu8_EFFS,     

      //
      // Jets
      //
      JVT,
      JVTXF,
      JVTWeight,
      BTagMV2c00,
      BTagMV2c10,
      BTagMV2c20,
      BTagEventWeight,
      jetFitterComb,
      JetFlavorTruth,
      JetFlavorTruthGhost,
      isLooseBad,
      EtaEM,
      EMFrac,  
      
      //
      // Taus
      //
      BDTEleScore,
      BDTJetScore,
      EleBDTLoose,
      EleBDTMedium,
      EleBDTTight,
      JetBDTSigLoose,
      JetBDTSigMedium,
      JetBDTSigTight,
      MuonVeto,

      IdSF,
      IdSFSys,
      IdSFStat,
      eVetoSF,
      eVetoSFUp,
      eVetoSFDown,
      NWideTrack,
      passEleOLR,
      ChargeIDBDTTight,
      isTruthMatched,
      isHadronicTau,

      
      //
      // ID track variables
      //
      D0,
      D02D,
      D0Sig,
      Z0,
      Z0Sig,
      Z0Sin,

      ExpectBL,
      ExpectIBL,
      BLHit,
      NBL,
      NIBL,
      NPIX,
      NPIXHole,
      NPIXPlusNDead,
      NSCT,
      NSCTHole,
      NSCTPlusNDead,
      NSIL,
      NSILHole,
      NSILPlusNDead,

      NTRT,
      NTRTHL,
      NTRTPass,

      MatchNTrueElecPrompt,
      MatchNTrueElecBdecay,
      MatchNTrueMuonPrompt,
      MatchNTrueMuonBdecay,

      NTrueMatchElec,
      NTrueMatchMuon,
      NTrueMatchTau,
      NTrueMatchLepton,

      NTruePromptElec,
      NTruePromptMuon,
      NTruePromptTau,
      NTruePromptLepton,

      NTrueElecWrongSign,
      NTrueElecRightSign,
      
      MatchTauVisDR,
      MatchTauVisDPt,
      MatchTauVisPt,
      TruthHadTauMatchIndexPDG,
      TruthHadTauMatchMotherPDG,
      TruthHadTauIsHiggs,
      TruthHadTauIsHiggsTauTau,

      MatchRecoLep,
      MatchRecoPV,
      MatchRecoPU,

      HasPromptParent,
      HasBQuarkParent,
      HasCQuarkParent,

      MatchTrigDR,
      MatchTrigTag,
      MatchTrigProbe,
      MatchTrigBit,
      MatchTrigNLep,
      MatchTrigNLepTight,
      MatchTrigNMuon,
      MatchTrigNMuonTight,
      MatchTrigNElec,
      MatchTrigNElecTight,     

      //
      // Trigger 
      //
      EF_e24vhi_medium1,
      EF_e60_medium1,
      EF_mu24i_tight,
      EF_mu36_tight,

      HLT_mu4,
      HLT_mu4_msonly,
      HLT_mu6,
      HLT_mu6_msonly,
      HLT_mu10,
      HLT_mu10_msonly,
      HLT_mu14,
      HLT_mu18,
      HLT_mu20,
      HLT_mu20_iloose_L1MU15,
      HLT_mu20_L1MU15,
      HLT_mu20_msonly,
      HLT_mu22,
      HLT_mu24,
      HLT_mu24_iloose_L1MU15,
      HLT_mu24_ivarloose_L1MU15,
      HLT_mu24_imedium,
      HLT_mu24_L1MU15,
      HLT_mu26,
      HLT_mu26_imedium,
      HLT_mu50,

      HLT_noalg_L1MU4,
      HLT_noalg_L1MU6,
      HLT_noalg_L1MU10,
      HLT_noalg_L1MU15,
      HLT_noalg_L1MU20,

      HLT_e24_lhmedium_L1EM18VH,
      HLT_e24_lhmedium_L1EM20VH,
      HLT_e24_lhmedium_iloose_L1EM18VH,
      HLT_e24_tight_iloose,
      HLT_e24_lhtight_iloose, 
      HLT_e24_tight_iloose_L1EM20VH,
      HLT_e24_lhtight_iloose_L1EM20VH, 
      HLT_e26_tight_iloose,
      HLT_e26_lhtight_iloose,
      HLT_e60_medium,
      HLT_e60_lhmedium, 
      HLT_e120_lhloose,
      HLT_e140_loose,
      HLT_e140_lhloose,
      
      L1_MU4,
      L1_MU6,
      L1_MU10,
      L1_MU15,
      L1_MU20,

      //
      // trigger match
      //
      //new trigger vars
      match_HLT_e24_lhmedium_nod0_L1EM20VH,
      match_HLT_e24_lhtight_nod0_ivarloose,
      match_HLT_e24_lhmedium_L1EM18VH,
      match_HLT_e24_lhmedium_L1EM20VH,
      match_HLT_e24_lhmedium_iloose_L1EM18VH,
      match_HLT_e24_tight_iloose,
      match_HLT_e24_lhtight_iloose, 
      match_HLT_e24_tight_iloose_L1EM20VH,
      match_HLT_e24_lhtight_iloose_L1EM20VH, 
      match_HLT_e26_tight_iloose,
      match_HLT_e26_lhtight_iloose,
      match_HLT_e26_lhtight_nod0_ivarloose,
      match_HLT_e60_medium,
      match_HLT_e60_lhmedium, 
      match_HLT_e60_lhmedium_nod0, 
      match_HLT_e120_lhloose,
      match_HLT_e140_loose,
      match_HLT_e140_lhloose, 
      match_HLT_e140_lhloose_nod0, 
      match_HLT_e300_etcut,
      match_HLT_mu20_iloose_L1MU15,
      match_HLT_mu24_imedium,
      match_HLT_mu24_ivarmedium,
      match_HLT_mu24_iloose,
      match_HLT_mu24_iloose_L1MU15,
      match_HLT_mu24_ivarloose,
      match_HLT_mu24_ivarloose_L1MU15,
      match_HLT_mu26_imedium, 
      match_HLT_mu26_ivarmedium, 
      match_HLT_mu26_ivarloose, 
      match_HLT_mu40,
      match_HLT_mu50,
      match_HLT_mu60_0eta105_msonly,
      match_HLT_2e12_loose_L12EM10VH,
      match_HLT_2e12_lhloose_L12EM10VH,
      match_HLT_e17_lhloose_2e9_lhloose,
      match_HLT_2mu14,
      match_HLT_2mu10,
      match_HLT_mu18_mu8noL1,
      match_HLT_e17_loose_mu14,
      match_HLT_e17_lhloose_mu14, 
      match_HLT_e7_medium_mu24,
      match_HLT_e7_lhmedium_mu24,
      dsid,

      // Added low pt threshold trigger for MM back up
      match_HLT_mu10,
      match_HLT_mu14,
      match_HLT_mu18,
      match_HLT_mu24,
      match_HLT_e17_loose,
      match_HLT_e17_lhloose,
      match_HLT_e12_loose,
      match_HLT_e12_lhloose,
      match_HLT_e5_loose,
      match_HLT_e5_lhloose,
      
      //
      // Isolation variables
      //
      EtCone10,
      EtCone20,
      EtCone20Rel,
      EtCone30,
      EtCone30Rel,
      EtCone40,
      EtCone40Rel,

      EtCone20OverPt,
      PtVarCone20OverPt,
      PtVarCone30OverPt,
      OneMuonEvt,
      OneTau,
      OppositeCharge,
      TauOverlap,
      NTrigMuon,

      PtCone10,
      PtCone20,
      PtCone20Rel,
      PtCone30,
      PtCone30Rel,
      PtCone40,
      PtCone40Rel,
      PtCone50,
      PtCone50Rel,
      PtCone60,
      PtCone60Rel,
      PtCone70,
      PtCone70Rel,
      PtCone80,
      PtCone80Rel,

      EtTopoCone20,
      EtTopoCone20Rel,
      EtTopoCone30,
      EtTopoCone30Rel,
      EtTopoCone40,
      EtTopoCone40Rel,

      PtVarCone20,
      PtVarCone20Rel,
      PtVarCone30,
      PtVarCone30Rel,
      PtVarCone40,
      PtVarCone40Rel,

      isolationLoose,
      isolationLooseTrackOnly,
      isolationGradient,
      isolationGradientLoose,
      isolationFixedCutTight,
      isolationFixedCutTightTrackOnly,
      isolationFixedCutLoose,

      // 
      // Pixel hit and cluster variables
      //
      GroupSize,
      Layer,
      TimeOverT,

      // 
      // Truth variables      
      //
      HasCharge,
      IsStable,
      IsLepton,
      IsHadron,
      IsObject,
      IsChargeFlip,
      IsRealLepton,
      ParentPDG,
      MatchTruthParticlePDG,
      MatchTruthParticleDR,
      MatchTruthHadronicTau,
      MatchTruthLeptonicTau,
      MatchTruthParticleMuonPDG,
      MatchTruthParticleMuonDR,
      MatchTruthLeptonicTauMuon,
      MatchTruthLepton,
      NMuonsInDilep,
      NElecsInDilep,
      NDileps,
      NDihad,
      NDilep,
      NSemilep,
      NSemilepEl,
      NSemilepMu,
      NMixed,
      NJustMu,
      NJustEl,
      NNuMu,
      NNuElec,
      M_truth_tau_tau,
      M_obs_tau_tau,
      DeltaRTruthDaughter,
      HasRecoLinkHadTau,
      HasRecoLinkLepTau,
      M_tau_tau,
      M_mu_tau,
      IsHadronicTau,
      IsTauPionNuDecay,
      BIT,
      PDG,
      Status,
      IndexRangeParent,
      IndexRangeChild,
      Index,
      NChild,
      NParent,
      ListType,
      TruthType,
      TruthOrigin,

      //
      // Event variables
      //
      // 1/6/16 test variables
      PhiStarEta,
      Mratio,
      PhiAcop,
      ThetaStarEta,
      DPhi,
      Ptl0,
      Ptl1,
      Ptcomb,
      M_obs_had_had,
      HasRecoLinkHadHad,
      Run,
      Event,
      LumiBlock,
      bcid,
      backgroundFlags,
      NMuon,
      NElec,
      NElecChargeMisid,
      NElecCentral,
      NElecForward,
      NObj,
      NObjElec,
      NObjMuon,
      NCluster,
      NLepton,
      NLeptonTight,
      NLeptonPair,
      NLeptonPairSFOS, 
      NLeptonPairZCand,
      NZCand, 
      NJet,
      NTruth,
      NJetT,
      MinJetDR,
      NJetBTag,
      NJetBTagTrueB,
      NJetCombined,
      NJetCombinedT,
      NTau,
      NAntiTau,
      NTrack,     
      NPriVtx,
      NRecVtx,
      NRecoPileUp,
      NTrueB,
      NTrueLep,
      NTrueNu,
      NTrueTop,
      NTrueW,
      NTrueZ,
      NParticle,
      NPair,

      EventMu,
      higgsDecayMode,
      ttbarDecayMode,
      ttbarXPt,

      Lep0Pt,
      Lep1Pt,
      Lep2Pt,
      Lep3Pt,

      Lep0Energy,
      Lep1Energy,
      Lep2Energy,
      Lep3Energy,

      Lep0Eta,
      Lep1Eta,
      Lep2Eta,
      Lep3Eta,

      Lep0Phi,
      Lep1Phi,
      Lep2Phi,
      Lep3Phi,

      Lep0EtaAbs,
      Lep1EtaAbs,
      Lep2EtaAbs,
      Lep3EtaAbs,

      Lep0PDG,
      Lep1PDG,
      Lep2PDG,
      Lep3PDG,

      Lep0Var,
      Lep1Var,
      Lep2Var,
      Lep3Var,

      Lep0D0,
      Lep1D0,
      Lep0Z0,
      Lep1Z0,

      Lep0MatchTruePDG,
      Lep1MatchTruePDG,
      Lep2MatchTruePDG,
      Lep3MatchTruePDG,

      Lep0MatchTrig,
      Lep1MatchTrig,
      Lep2MatchTrig,
      Lep3MatchTrig,
      
      Lep0EtCone20Rel,
      Lep1EtCone20Rel,
      Lep2EtCone20Rel,
      Lep3EtCone20Rel,

      Lep0PtCone20Rel,
      Lep1PtCone20Rel,
      Lep2PtCone20Rel,
      Lep3PtCone20Rel,

      Lep0PtCone40Rel,
      Lep1PtCone40Rel,
      Lep2PtCone40Rel,
      Lep3PtCone40Rel,
     
      Lep0D0Sig,
      Lep1D0Sig,
      Lep2D0Sig,
      Lep3D0Sig,

      Lep0Z0Sin,
      Lep1Z0Sin,
      Lep2Z0Sin,
      Lep3Z0Sin,

      Mlll,
      Mllll,
      MllEMu,
      MllZ1, 
      MllZ2, 
      PtllZ1, 
      PtllZ2, 
      HasSFOSPair, 
      Mllsub, 
      Ptllsub, 

      ZBestCandM,
      ZBestCandPt,
      ZBestCandDPhill,
      ZBestCandPDG,

      Jet0Pt,
      Jet1Pt,
      Jet2Pt,
      Jet3Pt,

      Jet0Eta,
      Jet1Eta,
      Jet2Eta,
      Jet3Eta,

      Tau0Pt,
      Tau1Pt,
      Tau2Pt,
      Tau0Eta,      
      Tau1Eta,
      Tau2Eta,
       
      DEtajj,
      DEtall,
      DPhijj,
      DPhill,
      DPhilljj,
      DPhillLeadJet,
      DRbb,
      DRjb,
      DRjj,
      DRll,
      DRlj,
      DRlb,
      DRle,
      DRlm,
      DRee,
      DRem,
      DReb,
      DRej,
      DRme,
      DRmm,
      DRmb,
      DRmj,

      MET, 
      MET_x,
      MET_y,
      MET_phi,
      MET_sum,
      MET_Truth_sumet,
      MET_RefFinal_et, 
      MET_RefFinal_sumet,
      MET_RefFinal_softTrk_et,  
      MET_RefFinal_phi,
      MET_RefFinal_softClus_et, 
      MET_RefFinal_softClus_phi, 
      MET_Objects_pt, 
      MET_Objects_phi, 
      METRel,

      MT,
      Mjj,
      Mll,
      Mll01,
      Mll02,
      Mll03,
      Mll12,
      Mll13,
      Mll23,
      OSLepPair, 

      Mtt,
      Ptll,
      Ptlll,
      Ptll01,
      Ptll02,
      Ptll12,
      Ptth,
      Etall,
      Phill,
      Pttot,
      CosThetaStar,

      HT,
      SumPtLep,
      SumPtJet,
      SumPtBJet,
      SumJetMV1,
      
      WCandJet,
      WCandMjj,
      WCandPt,
      WCandDR,      

      HFOR,
      EventWeight,
      MCWeight,
      MCWeightOrg,
      PUWeight,      
      XSWeight,
      PDFWeight,
      SysWeight,
      MCChannel,
      LepSF0Weight,
      LepSF1Weight,
      TrigSFWeight,

      TruthEventIndex,
      IsBlinded,
      IsDuplicate,
      TopPtReweighting,
      PassEventCleaning,
      PassGRL,
      PassTriggerMatch,
      PassOverlap,

      VtxNTrack,
      VtxSumPt,
      VtxSumPt2,
      VtxIndex,
      PosX,
      PosY,
      PosZ,
      AbsZ,
      PosR,
      VtxX,
      VtxY,
      VtxZ,
      VtxProd,
      VtxEnd,
      VtxType,

      pileupEventWeight_075,
      pileupEventWeight_080,
      pileupEventWeight_088,
      pileupEventWeight_090,
      pileupEventWeight_094,
      pileupEventWeight_103,
      pdf_id1, 
      pdf_id2,  
      pdf_x1, 
      pdf_x2,  
      pdf_scale,
    };

    bool RegisterAllPhysicsTausVars();
    
    bool RegisterPhysicsTausVar(Var var, const std::string &name);

    const std::vector<std::string>  & GetAllVarNames();
    const std::vector<Anp::Cut::Var>& GetAllVarEnums();

    std::string        AsStr      (uint32_t key, double val);
    const std::string& AsStr      (Var var);
    const std::string& Convert2Str(Var var);

    Var Convert2Var(const std::string &var);   
    Var Convert2Var(uint32_t           key);   

    Cut::Var FindVar(uint32_t key);
    
    Anp::Cut::Var ReadVar(const Anp::Registry &reg, 
			  const std::string &key, 
			  const std::string &caller = "ReadVar");
    
    std::vector<Anp::Cut::Var> ReadVars(const Anp::Registry &reg, 
					const std::string &key, 
					const std::string &caller = "ReadVars");

    std::vector<Anp::Cut::Var> ReadVars(const std::string &config,
					const std::string &caller = "ReadVars");

    std::set<int> ReadVarsAsSet(const Anp::Registry &reg, 
				const std::string &key, 
				const std::string &caller = "ReadVars");

    void PrintCutNames();

    template<class T> inline bool PassCut(const T &obj, Var var) 
      {
	float val = 0.0;
	if(obj.GetVar(var, val) && val > 0.0) {
	  return true;
	}
	
	return false;
      }
  }
}

#endif
