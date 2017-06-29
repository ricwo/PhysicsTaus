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

void plotAngularDistLepLep(){

  TFile *ftautau = new TFile("Ztautau5M.root");

  bool saveplots = true;
  bool angular   = false;
  bool starred	 = true;
  
  //  bool Phi_acop_tau_tau = true;
  //bool Phi_acop_had_had = true;

  if (angular){
    TCanvas *c0 = new TCanvas("c0");
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.64,0.62,0.80,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);

    TH1F *Phi_M		= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/DPhi_tau_lep");
    TH1F *Phi_M1       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_acop_lep_tau_tau");
    TH1F *Phi_M2       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_acop_lep_lep");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("Angular distributions of #tau#tau#rightarrowhh");
    Phi_M->GetXaxis()->SetTitle("log_{10}x");
    Phi_M->GetYaxis()->SetTitle("Events");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(1.);
    Phi_M->Rebin(8);
    Phi_M1->Rebin(8);
    Phi_M2->Rebin(8);
    c0->SetLogy();
    c0->SetRightMargin(0.15);
    c0->SetLeftMargin(0.12);
    c0->SetBottomMargin(0.12);
    Phi_M->GetXaxis()->SetRangeUser(-6.,2.);
    Phi_M->GetYaxis()->SetRangeUser(10,80000);
    Phi_M1->SetLineColor(2);
    Phi_M2->SetLineColor(4);
    Phi_M->SetLineWidth(2);
    Phi_M1->SetLineWidth(2);
    Phi_M2->SetLineWidth(2);

    hist_leg->AddEntry(Phi_M, Form("x = #Delta#phi_{#tau^{truth}l_{obs}}"));
    hist_leg->AddEntry(Phi_M1, Form("x = #phi_{acop}^{ll truth}"));
    hist_leg->AddEntry(Phi_M2, Form("x = #phi_{acop}^{ll obs.}"));
    
    //c0->SetLogx();
    Phi_M->Draw("");
    Phi_M1->Draw("same");
    Phi_M2->Draw("same");
    hist_leg->Draw("same");
   
    

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau, ll final state");
    //    Tl.DrawLatexNDC(0.15,0.8,Form("Correlation factor %f", Phi_M->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c0->SaveAs("plots/20160623/dileptonic/Angular_dists_lep_lep.pdf");
    }    
  }


  if (starred){
    TCanvas *c1 = new TCanvas("c1");
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.62,0.62,0.80,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);

    TH1F *Phi_M		= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/DPhi_sin_theta_truth_reco_lep_lep");
    TH1F *Phi_M1       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_truth_lep_lep");
    TH1F *Phi_M2       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_reco_lep_lep");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("Angular distributions of #tau#tau#rightarrowhh, starred variables");
    Phi_M->GetXaxis()->SetTitle("log_{10}x");
    Phi_M->GetYaxis()->SetTitle("Events");
    Phi_M->GetXaxis()->CenterTitle();
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(1.);
    Phi_M->Rebin(8);
    Phi_M1->Rebin(8);
    Phi_M2->Rebin(8);
    c1->SetLogy();
    c1->SetRightMargin(0.15);
    c1->SetLeftMargin(0.12);
    c1->SetBottomMargin(0.12);
    Phi_M->GetXaxis()->SetRangeUser(-6.,2.);
    Phi_M->GetYaxis()->SetRangeUser(10,80000);
    Phi_M1->SetLineColor(2);
    Phi_M2->SetLineColor(4);
    Phi_M->SetLineWidth(2);
    Phi_M1->SetLineWidth(2);
    Phi_M2->SetLineWidth(2);

    hist_leg->AddEntry(Phi_M, Form("x = #Delta#phi_{#tau^{truth}l}sin#theta^{*}_{#eta}"));
    hist_leg->AddEntry(Phi_M1, Form("x = #phi_{#eta}^{* ll truth}"));
    hist_leg->AddEntry(Phi_M2, Form("x = #phi_{#eta}^{* ll obs.}"));
    
    //c1->SetLogx();
    Phi_M->Draw("");
    Phi_M1->Draw("same");
    Phi_M2->Draw("same");
    hist_leg->Draw("same");
   
    

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau, ll final state");
    //    Tl.DrawLatexNDC(0.15,0.8,Form("Correlation factor %f", Phi_M->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c1->SaveAs("plots/20160623/dileptonic/Angular_dists_starred_lep_lep.pdf");
    }    
  }


}

