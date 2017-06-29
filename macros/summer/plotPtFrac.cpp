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

void plotPtFrac(){

  TFile *ftautau	= new TFile("Ztautau10M2.root");

  bool saveplots	= true;
  bool PtFraction	= true;
  bool DPhi		= false;


  if (PtFraction){
    TCanvas *c0 = new TCanvas("c0");
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat("e"); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.75,0.65,0.97,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);

    TH2F *Phi_M		= (TH2F*)ftautau->Get("361510/plotEventAlgs/taus/PtFraction_t1_t2");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("p_{T}^{l^{+}}/p_{T}^{#tau^{+}} vs. p_{T}^{l^{-}}/p_{T}^{#tau^{-}}");
    //Phi_M->GetZaxis()->SetTitle("Events / 0.00001");
    Phi_M->GetXaxis()->SetTitle("p_{T}^{l^{+}}/p_{T}^{#tau^{+}}");
    Phi_M->GetYaxis()->SetTitle("p_{T}^{l^{-}}/p_{T}^{#tau^{-}}");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(1.);
    //    Phi_M->Rebin2D(2,10);
    Phi_M->GetXaxis()->SetRangeUser(0.,1.2);
    Phi_M->GetYaxis()->SetRangeUser(0.,1.2);
    const double corr = Phi_M->GetCorrelationFactor();
    cout<<corr<<endl;

    //Phi_M->GetYaxis()->SetRangeUser(0.0008,10.);
    //c0->SetLogy();
    c0->SetLogz();
    c0->SetRightMargin(0.15);
    c0->SetLeftMargin(0.12);
    //c0->SetLogx();
    Phi_M->Draw("COLZ");
    //hist_leg->Draw("same");
   
    

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.25,0.85,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau, ll final state");
    Tl.DrawLatexNDC(0.25,0.8,Form("Correlation Factor: %f", corr));

    //save plot
    if (saveplots){
      c0->SaveAs("plots/20160616/pT_fraction_ll_corr_fac.pdf");
    }
    
  }

  if (DPhi){
    TCanvas *c1 = new TCanvas("c1");
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat("e"); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.75,0.65,0.97,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);

    TH2F *Phi_M		= (TH2F*)ftautau->Get("361510/plotEventAlgs/DPhi_t1_t2");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("#Delta#phi_{l#tau}^{+} vs. #Delta#phi_{l#tau}^{-}");
    //Phi_M->GetZaxis()->SetTitle("Events / 0.00001");
    Phi_M->GetXaxis()->SetTitle("log(#Delta#phi_{l#tau}^{+})");
    Phi_M->GetYaxis()->SetTitle("log(#Delta#phi_{l#tau}^{-})");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(1.);
    Phi_M->Rebin2D(2,2);
    Phi_M->GetXaxis()->SetRangeUser(-4.5,-1.);
    Phi_M->GetYaxis()->SetRangeUser(-4.5,-1.);


    //Phi_M->GetYaxis()->SetRangeUser(0.0008,10.);
    //c1->SetLogy();
    c1->SetLogz();
    c1->SetRightMargin(0.15);
    c1->SetLeftMargin(0.12);
    //c1->SetLogx();
    Phi_M->Draw("COLZ");
    //hist_leg->Draw("same");
   
    

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.3,0.8,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau");
    //    Tl.DrawLatexNDC(0.5,0.75,Form("%i entries", NentriesPhiM));

    //save plot
    if (saveplots){
      c1->SaveAs("plots/20160616/DPhi.pdf");
    }
    
  }



}

