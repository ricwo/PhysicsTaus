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
#include <TAttLine.h>

using namespace std;

void plotAngularDist(){

  TFile *ftautau = new TFile("Ztautau5M.root");

  bool saveplots = true;
  bool angular   = true;
  bool starred	 = false;
  bool eta	 = false;
  bool pt	 = false;
  
  //  bool Phi_acop_tau_tau = true;
  //bool Phi_acop_had_had = true;

  if (pt){
    TCanvas *c8 = new TCanvas("c8","c8",800,600);
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.72,0.62,0.80,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);

    TH1F *h		= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Pt_e_truth");
    TH1F *h1		= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Pt_e_reco");
    TH1F *h2       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Pt_e_match");
    TH1F *h3       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Pt_mu_truth");
    TH1F *h4       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Pt_mu_reco");
    TH1F *h5       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Pt_mu_match");

    int   NentriesPhiM	= (h->GetEntries());
    int rebinval = 4;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("p_{T} distributions, truth vs. reco");
    h->GetXaxis()->SetTitle("p_{T} [MeV]");
    h->GetYaxis()->SetTitle("Events / 2 GeV");
    h->GetXaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    c8->SetRightMargin(0.15);
    c8->SetLeftMargin(0.12);
    c8->SetBottomMargin(0.12);
    //    h->GetXaxis()->SetRangeUser(-6.,2.);
    h->GetYaxis()->SetRangeUser(0,18000);
    h->Rebin(rebinval);
    //    h->SetFillStyle(1001);
    //h->SetFillColor(2);
    h->SetLineWidth(3);

    h1->SetLineColor(1);
    h1->SetLineStyle(1);
    h1->Rebin(rebinval);
    h1->SetLineWidth(2);

    h2->SetLineColor(1);
    h2->SetLineWidth(1);
    h2->Rebin(rebinval);
    h2->SetLineStyle(2);

    h3->SetLineColor(2);
    h3->SetLineWidth(3);
    h3->Rebin(rebinval);
    h3->SetLineStyle(1);

    h4->SetLineColor(2);
    h4->SetLineWidth(2);
    h4->Rebin(rebinval);
    h4->SetLineStyle(1);

    h5->SetLineColor(2);
    h5->SetLineWidth(1);
    h5->Rebin(rebinval);
    h5->SetLineStyle(2);



    hist_leg->AddEntry(h, Form("e^{truth}"));
    hist_leg->AddEntry(h1, Form("e^{reco}"));
    hist_leg->AddEntry(h2, Form("e^{match}"));
    hist_leg->AddEntry(h3, Form("#mu^{truth}"));
    hist_leg->AddEntry(h4, Form("#tau^{reco}"));
    hist_leg->AddEntry(h5, Form("#tau^{match}"));



    h->GetXaxis()->SetRangeUser(0,55000);
    h->Draw("F");
    h1->Draw("same");
    h2->Draw("same");
    h3->Draw("same");
    h4->Draw("same");
    h5->Draw("same");

    hist_leg->Draw("same");
   
    

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau, lh final state");
    //    Tl.DrawLatexNDC(0.15,0.8,Form("Correlation factor %f", h->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c8->SaveAs("plots/20160706/general/pT_dist.pdf");
    }    
  }


  if (eta){
    TCanvas *c7 = new TCanvas("c7","c7",800,600);
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.72,0.62,0.80,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);

    TH1F *h		= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Eta_e_truth");
    TH1F *h1		= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Eta_e_reco");
    TH1F *h2		= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Eta_e_match");
    TH1F *h3       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Eta_mu_truth");
    TH1F *h4       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Eta_mu_reco");
    TH1F *h5       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Eta_mu_match");


    int   NentriesPhiM	= (h->GetEntries());
    int rebinval = 2;
    TGaxis::SetMaxDigits(2); 
    h->SetTitle("#eta distributions, truth vs. reco");
    h->GetXaxis()->SetTitle("#eta");
    h->GetYaxis()->SetTitle("Events / 0.2");
    h->GetXaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.);
    h->GetYaxis()->SetLabelSize(0.03);
    c7->SetLogy();
    c7->SetRightMargin(0.15);
    c7->SetLeftMargin(0.12);
    c7->SetBottomMargin(0.12);
    //    h->GetXaxis()->SetRangeUser(-6.,2.);
    h->GetYaxis()->SetRangeUser(200,22000);
    h->Rebin(rebinval);
    h->SetLineWidth(3);

    h1->SetLineColor(1);
    h1->SetLineStyle(1);
    h1->Rebin(rebinval);
    h1->SetLineWidth(2);

    h2->SetLineColor(1);
    h2->SetLineWidth(1);
    h2->Rebin(rebinval);
    h2->SetLineStyle(2);

    h3->SetLineColor(2);
    h3->SetLineWidth(3);
    h3->Rebin(rebinval);
    h3->SetLineStyle(1);

    h4->SetLineColor(2);
    h4->SetLineWidth(2);
    h4->Rebin(rebinval);
    h4->SetLineStyle(1);

    h5->SetLineColor(2);
    h5->SetLineWidth(1);
    h5->Rebin(rebinval);
    h5->SetLineStyle(2);

    hist_leg->AddEntry(h, Form("e^{truth}"));
    hist_leg->AddEntry(h1, Form("e^{reco}"));
    hist_leg->AddEntry(h2, Form("e^{match}"));
    hist_leg->AddEntry(h3, Form("#mu^{truth}"));
    hist_leg->AddEntry(h4, Form("#tau^{reco}"));
    hist_leg->AddEntry(h5, Form("#tau^{match}"));



    
    //c7->SetLogx();
    h->Draw("");
    h1->Draw("same");
    h2->Draw("same");
    h3->Draw("same");
    h4->Draw("same");
    h5->Draw("same");
    hist_leg->Draw("same");
   
    

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau, lh final state");
    //    Tl.DrawLatexNDC(0.15,0.8,Form("Correlation factor %f", h->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c7->SaveAs("plots/20160706/general/Eta_dist.pdf");
    }    
  }

  if (angular){
    TCanvas *c0 = new TCanvas("c0");
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.56,0.62,0.71,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);

    TH1F *h		= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/DPhi_tau_daughter_mu");
    TH1F *h1       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/DPhi_tau_daughter_elec");
    TH1F *h2       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/DPhi_tau_daughter_hadron_sum");
    int   NentriesPhiM	= (h->GetEntries());

    h->SetTitle("#Delta#phi between truth-level #tau and truth-level daughters");
    h->GetXaxis()->SetTitle("log_{10}#Delta#phi_{#tau, daughter}");
    h->GetYaxis()->SetTitle("Events");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.);
    h->Rebin(20);
    h1->Rebin(20);
    h2->Rebin(20);
    c0->SetLogy();
    c0->SetRightMargin(0.15);
    c0->SetLeftMargin(0.12);
    c0->SetBottomMargin(0.12);
    h->GetXaxis()->SetRangeUser(-5.,3.);
    h->GetYaxis()->SetRangeUser(1000,4000000);
    h->SetLineWidth(2);
    h1->SetLineColor(2);
    h1->SetLineWidth(2);
    h2->SetLineColor(4);
    h2->SetLineWidth(2);


    hist_leg->AddEntry(h, Form("#mu^{#pm}"));
    hist_leg->AddEntry(h1, Form("e^{#pm}")); 
    hist_leg->AddEntry(h2, Form("#sum daughters excl. #nu"));
    
    //c0->SetLogx();
    h->Draw("");
    h1->Draw("same");
    h2->Draw("same");
    hist_leg->Draw("same");
    cout<<h->Integral()<<endl<<h1->Integral()<<endl<<h2->Integral()<<endl;
    

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau, lh final state");
    //    Tl.DrawLatexNDC(0.15,0.8,Form("Correlation factor %f", h->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c0->SaveAs("plots/20160711/general/DPhi_tau_daughters.pdf");
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

    TH1F *Phi_M		= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/DPhi_sin_theta_truth_reco_had_had");
    TH1F *Phi_M1       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_truth_had_had");
    TH1F *Phi_M2       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_reco_had_had");

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
    Phi_M->GetXaxis()->SetRangeUser(-5.,2.);
    Phi_M->GetYaxis()->SetRangeUser(20,80000);
    Phi_M1->SetLineColor(2);
    Phi_M2->SetLineColor(4);
    Phi_M->SetLineWidth(2);
    Phi_M1->SetLineWidth(2);
    Phi_M2->SetLineWidth(2);

    hist_leg->AddEntry(Phi_M, Form("x = #Delta#phi_{#tau^{truth}#tau^{had. obs.}}sin#theta^{*}_{#eta}"));
    hist_leg->AddEntry(Phi_M1, Form("x = #phi_{#eta}^{* #tau#tau truth}"));
    hist_leg->AddEntry(Phi_M2, Form("x = #phi_{#eta}^{* #tau_{had}#tau_{had} obs.}"));
    
    //c1->SetLogx();
    Phi_M->Draw("");
    Phi_M1->Draw("same");
    Phi_M2->Draw("same");
    hist_leg->Draw("same");
   
    

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau, hh final state");
    //    Tl.DrawLatexNDC(0.15,0.8,Form("Correlation factor %f", Phi_M->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c1->SaveAs("plots/20160622/di-hadronic/Angular_dists_starred_had_had_coarse.pdf");
    }    
  }


}

