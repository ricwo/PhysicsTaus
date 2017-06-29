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

void plotAngularDistRatio(){

  TFile *ftautau = new TFile("Ztautau10M.root");

  bool saveplots = true;
  bool angular   = false;
  bool starred	 = false;
  bool angular_fraction	 = true;
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

    TPad *pad1		= new TPad("pad1","pad1",0,0.3,1,1,0);
    TPad *pad2		= new TPad("pad2","pad2",0,0.05,1,0.3);
    pad1->cd();

    TH1F *Phi_M		= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/DPhi_tau_had");
    TH1F *Phi_M1       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_acop_tau_tau");
    TH1F *Phi_M2       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_acop_had_had");

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
    pad1->SetLogy();
    pad1->SetRightMargin(0.15);
    pad1->SetLeftMargin(0.12);
    pad1->SetBottomMargin(0.12);
    Phi_M->GetXaxis()->SetRangeUser(-5.,2.);
    Phi_M->GetYaxis()->SetRangeUser(20,80000);
    Phi_M1->SetLineColor(2);
    Phi_M2->SetLineColor(4);
    Phi_M->SetLineWidth(2);
    Phi_M1->SetLineWidth(2);
    Phi_M2->SetLineWidth(2);

    hist_leg->AddEntry(Phi_M, Form("x = #Delta#phi_{#tau^{truth}#tau^{had. obs.}}"));
    hist_leg->AddEntry(Phi_M1, Form("x = #phi_{acop}^{#tau#tau truth}"));
    hist_leg->AddEntry(Phi_M2, Form("x = #phi_{acop}^{#tau_{had}#tau_{had} obs.}"));
    
    //c0->SetLogx();
    pad1->cd();
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
      c0->SaveAs("plots/20160623/di-hadronic/Angular_dists_had_had_coarse.pdf");
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
      c1->SaveAs("plots/20160623/di-hadronic/Angular_dists_starred_had_had_coarse.pdf");
    }    
  }

  if (angular_fraction) {
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.69,0.62,0.80,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);
    TH1F *h0		= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/DPhi_tau_had");
    TH1F *h1       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_acop_tau_tau");
    TH1F *h2       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_acop_had_had");
    h0->Rebin(20);
    h1->Rebin(20);
    h2->Rebin(20);

    
    // Define the Canvas
    TCanvas *c = new TCanvas("c", "canvas", 800, 600);
    
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->SetLogy();
    pad1->cd();               // pad1 becomes the current pad
    h0->SetStats(0);          // No statistics on upper plot
    h0->GetYaxis()->SetTitle("Events");
 


    h0->SetLineWidth(2);
    h0->SetLineColor(1);
    h1->SetLineWidth(2);
    h1->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->SetLineColor(4);

    h0->GetYaxis()->SetRangeUser(20,200000);
    h0->GetXaxis()->SetRangeUser(-6.,2.);
    h0->SetTitle("Angular distributions #tau#tau#rightarrowhh");
    h0->SetTitleSize(0.1);

    hist_leg->AddEntry(h0, Form("x_{1} = #Delta#phi_{#tau^{truth}#tau^{had. obs.}}"));
    hist_leg->AddEntry(h1, Form("x_{2} = #phi_{acop}^{#tau#tau truth}"));
    hist_leg->AddEntry(h2, Form("x_{3} = #phi_{acop}^{#tau_{had}#tau_{had} obs.}"));

    h0->Draw();               // Draw h1
    h1->Draw("same");
    h2->Draw("same");
    hist_leg->Draw("same");

    
    // Do not draw the Y axis label on the upper plot and redraw a small
    // axis instead, in order to avoid the first label (0) to be clipped.
    h1->GetYaxis()->SetLabelSize(0.);
    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    //axis->Draw();
    
    // lower plot will be in pad
    c->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.2);
    pad2->SetGridx(); // vertical grid
    pad2->Draw();
    pad2->cd();       // pad2 becomes the current pad
    
    // Define the ratio plot
    TH1F *h3 = (TH1F*)h1->Clone("h3");
    h3->SetLineColor(kBlack);
    h3->Sumw2();
    h3->SetStats(0);      // No statistics on lower plot
    h3->Divide(h2);
    h3->SetMarkerStyle(21);
    h3->GetXaxis()->SetTitle("log_{10}x_{i=1,2,3}");
    h3->GetXaxis()->SetTitleOffset(.85);
    h3->GetYaxis()->SetTitleOffset(0.35);
    TLine *line = new TLine(-6.,1.,2.,1);
    h3->Draw("ep");       // Draw the ratio plot
    line->SetLineStyle(2);
    line->Draw("same");
    
    
    // Ratio plot (h3) settings
    h3->SetTitle(""); // Remove the ratio title
    
    // Y axis ratio plot settings
    h3->GetYaxis()->SetTitle("x_{2} / x_{3}");
    h3->GetYaxis()->SetNdivisions(505);
    h3->GetYaxis()->SetTitleSize(0.1);
    // h3->GetYaxis()->SetTitleFont(43);
    // h3->GetYaxis()->SetTitleOffset(1.55);
    h3->GetYaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h3->GetYaxis()->SetLabelSize(0.1);
    h3->GetYaxis()->SetRangeUser(0.5,1.5);
    h3->GetXaxis()->SetRangeUser(-6.,2.);
    h3->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h3->GetXaxis()->SetTitleSize(0.1);
    // h3->GetXaxis()->SetTitleFont(43);
    // h3->GetXaxis()->SetTitleOffset(4.);
    h3->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h3->GetXaxis()->SetLabelSize(0.1);

    pad1->cd();
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.15,0.85,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau, hh final state");

    //save plot
    if (saveplots){
      c->SaveAs("plots/20160623/di-hadronic/Angular_dists_had_had_fraction.pdf");
    }    
  }
  

  if (angular_fraction) {
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.69,0.62,0.80,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);
    TH1F *h0		= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/DPhi_sin_theta_truth_reco_had_had");
    TH1F *h1       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_truth_had_had");
    TH1F *h2       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_reco_had_had");

    h0->Rebin(20);
    h1->Rebin(20);
    h2->Rebin(20);

    
    // Define the Canvas
    TCanvas *c3 = new TCanvas("c3", "canvas3", 800, 600);
    
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->SetLogy();
    pad1->cd();               // pad1 becomes the current pad
    h0->SetStats(0);          // No statistics on upper plot
    h0->GetYaxis()->SetTitle("Events");
 


    h0->SetLineWidth(2);
    h0->SetLineColor(1);
    h1->SetLineWidth(2);
    h1->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->SetLineColor(4);

    h0->GetYaxis()->SetRangeUser(20,200000);
    h0->GetXaxis()->SetRangeUser(-6.,2.);
    h0->SetTitle("Angular distributions #tau#tau#rightarrowhh, starred variables");
    h0->SetTitleSize(0.1);

    hist_leg->AddEntry(h0, Form("x_{1} = #Delta#phi_{#tau^{truth}#tau^{had. obs.}}sin#theta^{*}_{#eta}"));
    hist_leg->AddEntry(h1, Form("x_{2} = #phi_{#eta}^{* #tau#tau truth}"));
    hist_leg->AddEntry(h2, Form("x_{3} = #phi_{#eta}^{* #tau_{had}#tau_{had} obs.}"));
    

    h0->Draw();               // Draw h1
    h1->Draw("same");
    h2->Draw("same");
    hist_leg->Draw("same");

    
    // Do not draw the Y axis label on the upper plot and redraw a small
    // axis instead, in order to avoid the first label (0) to be clipped.
    h1->GetYaxis()->SetLabelSize(0.);
    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    //axis->Draw();
    
    // lower plot will be in pad
    c3->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.2);
    pad2->SetGridx(); // vertical grid
    pad2->Draw();
    pad2->cd();       // pad2 becomes the current pad
    
    // Define the ratio plot
    TH1F *h3 = (TH1F*)h1->Clone("h3");
    h3->SetLineColor(kBlack);
    h3->Sumw2();
    h3->SetStats(0);      // No statistics on lower plot
    h3->Divide(h2);
    h3->SetMarkerStyle(21);
    h3->GetXaxis()->SetTitle("log_{10}x_{i=1,2,3}");
    h3->GetXaxis()->SetTitleOffset(.85);
    h3->GetYaxis()->SetTitleOffset(0.35);
    TLine *line = new TLine(-6.,1.,2.,1);
    h3->Draw("ep");       // Draw the ratio plot
    line->SetLineStyle(2);
    line->Draw("same");
    
    
    // Ratio plot (h3) settings
    h3->SetTitle(""); // Remove the ratio title
    
    // Y axis ratio plot settings
    h3->GetYaxis()->SetTitle("x_{2} / x_{3}");
    h3->GetYaxis()->SetNdivisions(505);
    h3->GetYaxis()->SetTitleSize(0.1);
    // h3->GetYaxis()->SetTitleFont(43);
    // h3->GetYaxis()->SetTitleOffset(1.55);
    h3->GetYaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h3->GetYaxis()->SetLabelSize(0.1);
    h3->GetYaxis()->SetRangeUser(0.5,1.5);
    h3->GetXaxis()->SetRangeUser(-6.,2.);
    h3->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h3->GetXaxis()->SetTitleSize(0.1);
    // h3->GetXaxis()->SetTitleFont(43);
    // h3->GetXaxis()->SetTitleOffset(4.);
    h3->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h3->GetXaxis()->SetLabelSize(0.1);

    pad1->cd();
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.15,0.85,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau, hh final state");

    //save plot
    if (saveplots){
      c3->SaveAs("plots/20160623/di-hadronic/Angular_dists_starred_had_had_fraction.pdf");
    }    
  }

  
}

