#!/bin/zsh


if [ "$1" = "b" ]; then
root -b -q -l 'plotDists.cpp ("AT",16,0,155," / 1.6 GeV")';
root -b -q -l 'plotDists.cpp ("Mvis",4,0,395," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("Mvis_l_MET",4,0,395," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("PhiStar",4,-4,2.3,"")';
#root -b -q -l 'plotDists.cpp ("Sin_theta_semilep_bkg",10,-1,0.1,"")';
root -b -q -l 'plotDists.cpp ("Mstar2",4,0,0," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("Mstar3",4,0,0," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("MT3",4,0,0," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("MT_MET_l",4,0,0," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("PtMu",2,0,0," / 2 GeV")';
root -b -q -l 'plotDists.cpp ("TauBDTScore",2,0,0," / 0.02")';
fi

if [ "$1" = "n" ]; then
root -b -q -l 'plotDists.cpp ("mMuNu",8,0,795)';
root -b -q -l 'plotDists.cpp ("mTauNu",8,0,795)';
root -b -q -l 'plotDists.cpp ("mMuTau",4,0,395)';
root -b -q -l 'plotDists.cpp ("m2Hat",8,0,795)';
root -b -q -l 'plotDists.cpp ("m3Hat",8,0,795)';
root -b -q -l 'plotDists.cpp ("mHat",8,0,795)';
root -b -q -l 'plotDists.cpp ("EtaNu",4,-7,7)';
root -b -q -l 'plotDists.cpp ("RhoNu",4,-1,1)';
fi

if [ "$1" = "G" ]; then
root -b -q -l 'plotDists.cpp ("PassGammaDeltaAt",4,0,155," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaMvis",4,0,300," / 8 GeV")';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaMt",4,0,0," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaMstar3",4,0,200," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaPhistar",1,-3.,1.8)';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaAtDotMet",32, -600, 600," / 32 GeV^{2}")';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaEt",1,0,200," / GeV")';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaMrem",4,0,200," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaPtMu",2,0,120," / 2 GeV")';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaMremstar",4,0,200," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaDPhiLepMet",2,0,0," / 0.02")';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaDPhiLepHad",2,0,0," / 0.02")';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaDPhiHadMet",2,0,0," / 0.02")';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaSumCos",8,-2,2," / 0.02")';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaTwistLepMet",1,0,0," / 0.005")';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaTwistLepHad",1,0,0," / 0.005")';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaMstar2",4,0,200," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaTwistMetHad",1,0,0," / 0.005")';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaPtRatio",1,0,3," / 0.1")';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaPtAsym",2,-1,1," / 0.02")';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaMstar2DivPt",1,0,3," / 0.05")';
root -b -q -l 'plotDists.cpp ("PassGammaDeltaMstar3DivPt",1,0,6," / 0.01")';
fi

if [ "$1" = "1" ]; then
root -b -q -l 'plotDists.cpp ("Roi1At",4,0,155," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("Roi1Mvis",4,0,300," / 8 GeV")';
root -b -q -l 'plotDists.cpp ("Roi1Mt",4,0,0," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("Roi1Mstar3",4,0,200," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("Roi1Phistar",1,-3.,1.8)';
root -b -q -l 'plotDists.cpp ("Roi1AtDotMet",32, -600, 600," / 32 GeV^{2}")';
root -b -q -l 'plotDists.cpp ("Roi1Et",1,0,200," / GeV")';
root -b -q -l 'plotDists.cpp ("Roi1Mrem",4,0,200," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("Roi1PtMu",2,0,120," / 2 GeV")';
root -b -q -l 'plotDists.cpp ("Roi1Mremstar",4,0,200," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("Roi1DPhiLepMet",2,0,0," / 0.02")';
root -b -q -l 'plotDists.cpp ("Roi1DPhiLepHad",2,0,0," / 0.02")';
root -b -q -l 'plotDists.cpp ("Roi1DPhiHadMet",2,0,0," / 0.02")';
root -b -q -l 'plotDists.cpp ("Roi1SumCos",8,-2,2," / 0.02")';
root -b -q -l 'plotDists.cpp ("Roi1TwistLepMet",1,0,0," / 0.005")';
root -b -q -l 'plotDists.cpp ("Roi1TwistLepHad",1,0,0," / 0.005")';
root -b -q -l 'plotDists.cpp ("Roi1Mstar2",4,0,200," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("Roi1TwistMetHad",1,0,0," / 0.005")';
root -b -q -l 'plotDists.cpp ("Roi1PtRatio",1,0,3," / 0.1")';
root -b -q -l 'plotDists.cpp ("Roi1PtAsym",2,-1,1," / 0.02")';
root -b -q -l 'plotDists.cpp ("Roi1Mstar2DivPt",1,0,3," / 0.05")';
root -b -q -l 'plotDists.cpp ("Roi1Mstar3DivPt",1,0,6," / 0.01")';
fi

if [ "$1" = "2" ]; then
root -b -q -l 'plotDists.cpp ("Roi2At",4,0,155," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("Roi2Mvis",4,0,300," / 8 GeV")';
root -b -q -l 'plotDists.cpp ("Roi2Mt",4,0,0," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("Roi2Mstar3",4,0,200," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("Roi2Phistar",1,-3.,1.8)';
root -b -q -l 'plotDists.cpp ("Roi2AtDotMet",32, -600, 600," / 32 GeV^{2}")';
root -b -q -l 'plotDists.cpp ("Roi2Et",1,0,200," / GeV")';
root -b -q -l 'plotDists.cpp ("Roi2Mrem",4,0,200," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("Roi2PtMu",2,0,120," / 2 GeV")';
root -b -q -l 'plotDists.cpp ("Roi2Mremstar",4,0,200," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("Roi2DPhiLepMet",2,0,0," / 0.02")';
root -b -q -l 'plotDists.cpp ("Roi2DPhiLepHad",2,0,0," / 0.02")';
root -b -q -l 'plotDists.cpp ("Roi2DPhiHadMet",2,0,0," / 0.02")';
root -b -q -l 'plotDists.cpp ("Roi2SumCos",8,-2,2," / 0.02")';
root -b -q -l 'plotDists.cpp ("Roi2TwistLepMet",1,0,0," / 0.005")';
root -b -q -l 'plotDists.cpp ("Roi2TwistLepHad",1,0,0," / 0.005")';
root -b -q -l 'plotDists.cpp ("Roi2Mstar2",4,0,200," / 4 GeV")';
root -b -q -l 'plotDists.cpp ("Roi2TwistMetHad",1,0,0," / 0.005")';
root -b -q -l 'plotDists.cpp ("Roi2PtRatio",1,0,3," / 0.1")';
root -b -q -l 'plotDists.cpp ("Roi2PtAsym",2,-1,1," / 0.02")';
root -b -q -l 'plotDists.cpp ("Roi2Mstar2DivPt",1,0,3," / 0.05")';
root -b -q -l 'plotDists.cpp ("Roi2Mstar3DivPt",1,0,6," / 0.01")';
fi

