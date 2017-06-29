#include <TH1F.h>
#include <TH2F.h>
#include <THStack.h>
#include <TLegend.h>
#include <TFile.h>
#include <TMath.h>
#include <TLine.h>
#include <TCanvas.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include <set>
#include <TLatex.h>
#include <TPaveText.h>
#include <TPad.h>
#include <TEfficiency.h>
#include <TDirectory.h>

using namespace std;

void plotDR(){

  TFile *ftautau = new TFile("Ztautau10M.root");
  TFile *fee	 = new TFile("Zee10M.root");
  TFile *fmumu	 = new TFile("Zmumu10M.root");

  bool saveplots = true;
  bool DR	 = true;


  if (DR){
    TCanvas *c1 = new TCanvas("c1");
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.04);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.65,0.65,0.87,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(0);
    //hist_leg->SetLineWidth(0);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    

    //declare the histograms needed for plotting
    //fcombined->Cd("361510/plotEventAlgs");
    //fcombined->Pwd();
    TH1F *ee_DRcomb           	= (TH1F*)fee->Get("361500/plotEventAlgs/DRcomb");
    TH1F *mumu_DRcomb          	= (TH1F*)fmumu->Get("361505/plotEventAlgs/DRcomb");    
    TH1F *tautau_DRcomb       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/DRcomb");
    double rebinval		= 20;
    tautau_DRcomb->SetTitle("#DeltaR of reco-lepton and truth-#tau");
    tautau_DRcomb->GetXaxis()->SetTitle("#DeltaR_{l^{reco}#tau^{truth}}");
    tautau_DRcomb->GetYaxis()->SetTitle("Events / 0.02");
    tautau_DRcomb->GetXaxis()->SetTitleSize(0.04);
    ee_DRcomb->Rebin(rebinval);
    mumu_DRcomb->Rebin(rebinval);
    tautau_DRcomb->Rebin(rebinval);
    ee_DRcomb->GetYaxis()->SetTitleSize(0.06);
    tautau_DRcomb->GetXaxis()->SetTitleOffset(1.15);
    tautau_DRcomb->GetYaxis()->SetTitleOffset(0.95);
    //ee_DRcomb->SetTitle("#phi^{*}_#eta distribution");
    //==============================================================================//
    //USE SetLimits instead of SetRangeUser!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
    // works with log scale//
    //tautau_DRcomb->GetXaxis()->SetRangeUser(0,.4);
    //ee_DRcomb->GetYaxis()->SetLimits(1.,1000000.);
    //ee_DRcomb->GetYaxis()->SetRangeUser(1.,100000.);
    //tautau_DRcomb->GetYaxis()->SetRangeUser(1.,100000.); 

    ee_DRcomb->SetLineColor(2);
    ee_DRcomb->SetLineWidth(1);
    hist_leg->AddEntry(ee_DRcomb, "Z#rightarrow ee");

    mumu_DRcomb->SetLineColor(4);
    mumu_DRcomb->SetLineWidth(1);
    hist_leg->AddEntry(mumu_DRcomb, "Z#rightarrow #mu#mu");

    tautau_DRcomb->SetLineColor(1);
    tautau_DRcomb->SetLineWidth(1);
    hist_leg->AddEntry(tautau_DRcomb, "Z#rightarrow #tau#tau");


    c1->SetLogy();
    tautau_DRcomb->Draw("");
    //    ee_DRcomb->Draw("same");
    //mumu_DRcomb->Draw("same");
    //hist_leg->Draw("same");

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    //Tl.DrawLatexNDC(0.5,0.8,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau");
    //    Tl.DrawLatexNDC(0.5,0.75,Form("%i entries", NentriesPhiM));

    //save plot
    if (saveplots){
      c1->SaveAs("plots/DR/DRtautau.pdf");
    }

    
  }


}

