#for cut in "nocut" "tauID08" "tauPt25" "muonMedium" "muonTight" "muonPt20" "elecMedium" "elecTight" "elecPt20"
for cut in "tauPt25"

do 


  python macros/plotRecoTau.py hist_410000_$cut.root --rkey=.*1lep_1tau_2bjet -o plots_410000_$cut -s;
  python macros/plotJetFlavour.py hist_410000_$cut.root --rkey=.*1lep_1tau_2bjet -o plots_410000_$cut -s;
  python macros/plotHists.py hist_410000_$cut.root --rkey=.*1lep_1tau_2bjet -o plots_410000_$cut -s;
  
  #python macros/plotTruthEMu.py hist_410000_$cut.root --rkey=.*1lep_1tau_2bjet -o plots_410000_$cut -s --logy;
  
  cp plots_410000_$cut/plot_prepCand_1lep_1tau_2bjet_plotTruthMatch_TruthMatch_* ~/Manchester/MPhys/Week11/Fri/$cut/;
  
  cp plots_410000_$cut/*FakeTauFlavour* ~/Manchester/MPhys/Week11/Fri/$cut/;

  cp plots_410000_$cut/*NTightBJets* plots_410000_$cut/*NTightTaus* plots_410000_$cut/*NElec* plots_410000_$cut/*NMuon* ~/Manchester/MPhys/Week11/Fri/$cut/;


  echo $cut " done"

done

echo "All done"
