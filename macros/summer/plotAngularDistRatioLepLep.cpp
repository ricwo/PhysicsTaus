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

void plotAngularDistRatioLepLep(){

  TFile *ftautau = new TFile("Ztautau5M.root");

  bool saveplots = true;
  bool angular_fraction	 = false;
  bool angular_fraction2 = true;
  bool twod	= false;
  bool dphi_2d  = false;


  if (angular_fraction) {
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.69,0.56,0.80,0.86);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);
    TH1F *h0		= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/DPhi_tau_lep");
    TH1F *h1       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_acop_lep_tau_tau");
    TH1F *h2       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_acop_lep_lep");
    TH1F *h4       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/DPhi_truth_reco_mu_lep_lep");
    TH1F *h5       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/DPhi_truth_reco_elec_lep_lep");
    h0->Rebin(20);
    h1->Rebin(20);
    h2->Rebin(20);
    h4->Rebin(20);
    h5->Rebin(20);
    

    
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
    h4->SetLineWidth(2);
    h4->SetLineColor(7);
    h5->SetLineWidth(2);
    h5->SetLineColor(8);
    

    h0->GetYaxis()->SetRangeUser(20,200000);
    h0->GetXaxis()->SetRangeUser(-6.,2.);
    h0->SetTitle("Angular distributions #tau#tau#rightarrowhh");
    h0->SetTitleSize(0.1);

    h4->GetXaxis()->SetRangeUser(-6.,2.);
    h5->GetXaxis()->SetRangeUser(-6.,2.);

    hist_leg->AddEntry(h0, Form("x_{1} = #Delta#phi_{#tau^{truth}l_{obs}}"));
    hist_leg->AddEntry(h1, Form("x_{2} = #phi_{acop}^{ll truth}"));
    hist_leg->AddEntry(h2, Form("x_{3} = #phi_{acop}^{ll obs.}"));
    hist_leg->AddEntry(h4, Form("x_{4} = #Delta#phi_{#mu obs}^{#mu truth}"));
    hist_leg->AddEntry(h5, Form("x_{5} = #Delta#phi_{e obs}^{e truth}"));

    h0->Draw();               // Draw h1
    h1->Draw("same");
    h2->Draw("same");
    h4->Draw("same");
    h5->Draw("same");
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
    h3->GetXaxis()->SetTitle("log_{10}x_{n=1,2,3,4,5}");
    h3->GetXaxis()->SetTitleOffset(.85);
    h3->GetYaxis()->SetTitleOffset(0.35);
    TLine *line = new TLine(-6.,1.,2.,1);
    h3->Draw("ep");       // Draw the ratio plot
    line->SetLineStyle(2);
    line->Draw("same");
    
    
    // Ratio plot (h3) settings
    h3->SetTitle(""); // Remove the ratio title
    
    // Y axis ratio plot settings
    h3->GetYaxis()->SetTitle("logx_{2} / logx_{3}");
    h3->GetYaxis()->SetNdivisions(505);
    h3->GetYaxis()->SetTitleSize(0.12);
    // h3->GetYaxis()->SetTitleFont(43);
    // h3->GetYaxis()->SetTitleOffset(1.55);
    h3->GetYaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h3->GetYaxis()->SetLabelSize(0.1);
    h3->GetYaxis()->SetRangeUser(0.5,1.5);
    h3->GetXaxis()->SetRangeUser(-6.,2.);
    h3->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h3->GetXaxis()->SetTitleSize(0.12);
    // h3->GetXaxis()->SetTitleFont(43);
    h3->GetXaxis()->SetTitleOffset(0.7);
    h3->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h3->GetXaxis()->SetLabelSize(0.1);

    pad1->cd();
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.15,0.85,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau, ll final state");

    //save plot
    if (saveplots){
      c->SaveAs("plots/20160629/dileptonic/Angular_dists_lep_lep_fraction.pdf");
    }    
  }
  

  if (angular_fraction2) {
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.69,0.56,0.80,0.86);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.04);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);
    TH1F *h0		= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/DPhi_sin_theta_truth_reco_lep_lep");
    TH1F *h1       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_truth_lep_lep");
    TH1F *h2       	= (TH1F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_reco_lep_lep");

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


    hist_leg->AddEntry(h0, Form("x_{1} = #Delta#phi_{#tau^{truth}l}sin#theta^{*}_{#eta}"));
    hist_leg->AddEntry(h1, Form("x_{2} = #phi_{#eta}^{* ll truth}"));
    hist_leg->AddEntry(h2, Form("x_{3} = #phi_{#eta}^{* ll obs.}"));

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
    h3->GetYaxis()->SetTitle("logx_{2} / logx_{3}");
    h3->GetYaxis()->SetNdivisions(505);
    h3->GetYaxis()->SetTitleSize(0.12);
    // h3->GetYaxis()->SetTitleFont(43);
    // h3->GetYaxis()->SetTitleOffset(1.55);
    h3->GetYaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h3->GetYaxis()->SetLabelSize(0.1);
    h3->GetYaxis()->SetRangeUser(0.5,1.5);
    h3->GetXaxis()->SetRangeUser(-6.,2.);
    h3->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h3->GetXaxis()->SetTitleSize(0.12);
    //h3->GetXaxis()->SetTitleFont(43);
    h3->GetXaxis()->SetTitleOffset(.7);
    h3->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h3->GetXaxis()->SetLabelSize(0.1);

    pad1->cd();
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.15,0.85,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau, ll final state");

    //save plot
    if (saveplots){
      c3->SaveAs("plots/20160629/dileptonic/Angular_dists_starred_lep_lep_fraction.pdf");
    }    
  }


  if (dphi_2d){
    TCanvas *c5 = new TCanvas("c5", "c5", 800, 600);
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

    TH2F *Phi_M		= (TH2F*)ftautau->Get("361510/plotEventAlgs/taus/DPhi_2D_mu_lep_lep");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("#Delta#phi_{#tau^{- truth}l^{- obs}} vs. #Delta#phi_{#tau^{+ truth}l^{+ obs}}");
    Phi_M->GetXaxis()->SetTitle("log_{10}#Delta#phi_{#tau^{+ truth}l^{+ obs}}");
    Phi_M->GetYaxis()->SetTitle("log_{10}#Delta#phi_{#tau^{- truth}l^{- obs}}");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(1.);
    //Phi_M->Rebin2D(10,1);
    Phi_M->GetXaxis()->SetRangeUser(-6.,1.5);
    Phi_M->GetYaxis()->SetRangeUser(-6.,1.5);

    //Phi_M->GetYaxis()->SetRangeUser(0.0008,10.);
    //c5->SetLogy();
    c5->SetLogz();
    c5->SetRightMargin(0.15);
    c5->SetLeftMargin(0.12);
    c5->SetBottomMargin(0.12);
    //c5->SetLogx();
    Phi_M->Draw("COLZ");
    //hist_leg->Draw("same");
   
    

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau, ll final state");
    Tl.DrawLatexNDC(0.15,0.8,Form("Correlation factor %f", Phi_M->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c5->SaveAs("plots/20160629/dileptonic/DPhi_2D_lep_lep.pdf");
    }
    
  }


  if (twod){
    TCanvas *c0 = new TCanvas("c0", "c0", 800, 600);
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

    TH2F *Phi_M		= (TH2F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_acop_truth_obs_lep_lep");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("#phi_{acop}^{#tau_{lep}#tau_{lep} obs.} vs. #phi_{acop}^{#tau#tau truth}");
    Phi_M->GetXaxis()->SetTitle("log_{10}#phi_{acop}^{#tau#tau truth}");
    Phi_M->GetYaxis()->SetTitle("log_{10}#phi_{acop}^{#tau_{lep}#tau_{lep} obs.}");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(1.);
    //Phi_M->Rebin2D(10,1);
    Phi_M->GetXaxis()->SetRangeUser(-6.,1.5);
    Phi_M->GetYaxis()->SetRangeUser(-6.,1.5);

    //Phi_M->GetYaxis()->SetRangeUser(0.0008,10.);
    //c0->SetLogy();
    c0->SetLogz();
    c0->SetRightMargin(0.15);
    c0->SetLeftMargin(0.12);
    c0->SetBottomMargin(0.12);
    //c0->SetLogx();
    Phi_M->Draw("COLZ");
    //hist_leg->Draw("same");
   
    

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau, ll final state");
    Tl.DrawLatexNDC(0.15,0.8,Form("Correlation factor %f", Phi_M->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c0->SaveAs("plots/20160629/dileptonic/Phi_acop_truth_vs_obs_lep_lep.pdf");
    }
    
  }

  
}

