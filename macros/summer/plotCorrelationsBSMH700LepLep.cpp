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

void plotCorrelationsBSMH700LepLep(){

  TFile *ftautau = new TFile("H700_tautau_hh.root");

  bool saveplots = true;
  bool Phi_M	 = true;
  bool DPhi_pt	 = true;
  bool Pt_frac_vs= true;
  bool M_pt	 = true;


  if (Phi_M){
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

    TH2F *Phi_M		= (TH2F*)ftautau->Get("342320/plotEventAlgs/taus/Phi_M_fraction_had_had");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("#phi^{*hh}_{#eta} vs. m^{obs.}_{hh} / m^{MC}_{#tau#tau}");
    Phi_M->GetXaxis()->SetTitle("m^{obs.}_{hh} / m^{MC}_{#tau#tau}");
    Phi_M->GetYaxis()->SetTitle("log_{10}#phi^{*hh}_{#eta}");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(1.);
    //    Phi_M->Rebin2D(2,2);
    Phi_M->GetXaxis()->SetRangeUser(0.,1.6);
    Phi_M->GetYaxis()->SetRangeUser(-6.,2.0);

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
    Tl.DrawLatexNDC(0.50,0.85,"Powheg+Pythia8+EvtGen H(700)#rightarrow#tau#tau#rightarrowhh");
    Tl.DrawLatexNDC(0.50,0.8,Form("Correlation factor %f", Phi_M->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c0->SaveAs("plots/20160711/di-hadronic/H700/Phi_M_fraction_had_had.pdf");
    }
    
  }



  if (DPhi_pt){
    TCanvas *c20 = new TCanvas("c20");
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

    TH2F *Phi_M		= (TH2F*)ftautau->Get("342320/plotEventAlgs/taus/DPhi_pT_fraction_had_had");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("#Delta#phi_{h^{obs}#tau^{truth}} vs. p_{T}^{h^{obs}} / p_{T}^{#tau_{truth}}");
    //Phi_M->GetZaxis()->SetTitle("Events / 0.00001");
    Phi_M->GetYaxis()->SetTitle("log_{10}#Delta#phi_{h^{obs}#tau^{truth}}");
    Phi_M->GetXaxis()->SetTitle("p_{T}^{h^{obs}} / p_{T}^{#tau_{truth}}");/*m^{obs.}_{ll} [GeV/c^{2}]*/
    //    Phi_M->GetZaxis()->SetTitle("Events / (0.0001*log_{10}(#phi^{*#tau_{lep}#tau_{lep}}_{#eta}))");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(1.);
    Phi_M->Rebin2D(2,1);
    //    Phi_M->GetYaxis()->SetRangeUser(-6.,0.);
    Phi_M->GetYaxis()->SetRangeUser(-7.,0.8);
    Phi_M->GetXaxis()->SetRangeUser(0,1.6);

    //Phi_M->GetYaxis()->SetRangeUser(0.0008,10.);
    //c20->SetLogy();
    c20->SetLogz();
    c20->SetRightMargin(0.15);
    c20->SetLeftMargin(0.12);
    c20->SetBottomMargin(0.12);
    //c20->SetLogx();
    Phi_M->Draw("COLZ");
    //hist_leg->Draw("same");
   
    

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"Powheg+Pythia8+EvtGen H(700)#rightarrow#tau#tau#rightarrowhh");
    Tl.DrawLatexNDC(0.15,0.8,Form("Correlation factor %f", Phi_M->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c20->SaveAs("plots/20160711/di-hadronic/H700/DPhi_pT_fraction_had_had.pdf");
    }
  }


  if (M_pt){
    TCanvas *c5 = new TCanvas("c5");
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

    TH2F *Phi_M		= (TH2F*)ftautau->Get("342320/plotEventAlgs/taus/M_fraction_pT_fraction_had_had");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("p_{T}^{h^{obs}} / p_{T}^{#tau^{truth}} vs. m^{obs.}_{hh} / m^{MC}_{#tau#tau}");
    Phi_M->GetZaxis()->SetTitle("Events / 0.0004");
    Phi_M->GetXaxis()->SetTitle("m^{obsh}_{hh} / m^{MC}_{#tau#tau}");
    Phi_M->GetYaxis()->SetTitle("p_{T}^{h^{obs}} / p_{T}^{#tau^{truth}}");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(1.);
    Phi_M->Rebin2D(2,2);
    Phi_M->GetXaxis()->SetRangeUser(0.,1.6);
    Phi_M->GetYaxis()->SetRangeUser(0.,2.0);
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
    Tl.DrawLatexNDC(0.15,0.88,"Powheg+Pythia8+EvtGen H(700)#rightarrow#tau#tau#rightarrowhh");
    Tl.DrawLatexNDC(0.15,0.83,Form("Correlation factor %f", Phi_M->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c5->SaveAs("plots/20160711/di-hadronic/H700/M_fraction_pT_fraction_had_had.pdf");
    }
    
  }

  if (Pt_frac_vs){
    TCanvas *c9 = new TCanvas("c9");
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

    TH2F *Phi_M		= (TH2F*)ftautau->Get("342320/plotEventAlgs/taus/Pt_fraction_had_had");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("p_{T}^{#tau_{+/-had}^{obs.}} / p_{T}^{#tau^{truth}_{+/-had}} vs. p_{T}^{#tau_{+/-had}^{obs.}} / p_{T}^{#tau^{truth}_{+/-had}}");
    Phi_M->GetZaxis()->SetTitle("Events / 0.0004");
    Phi_M->GetXaxis()->SetTitle("p_{T}^{#tau_{+/-h}^{obs}} / p_{T}^{#tau^{truth}_{+/-had}}");
    Phi_M->GetYaxis()->SetTitle("p_{T}^{#tau_{+/-h}^{obs}} / p_{T}^{#tau^{truth}_{+/-had}}");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(1.);
    Phi_M->Rebin2D(2,2);
    Phi_M->GetXaxis()->SetRangeUser(0.,1.3);
    Phi_M->GetYaxis()->SetRangeUser(0.,1.8);
    //    c9->SetLogz();
    c9->SetRightMargin(0.15);
    c9->SetLeftMargin(0.12);
    c9->SetBottomMargin(0.12);
    //c9->SetLogx();
    Phi_M->Draw("COLZ");
    //hist_leg->Draw("same");
   
    

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.45,0.85,"Powheg+Pythia8+EvtGen H(700)#rightarrow#tau#tau#rightarrowhh");
    Tl.DrawLatexNDC(0.45,0.8,Form("Correlation factor %f", Phi_M->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c9->SaveAs("plots/20160711/di-hadronic/H700/Pt_fraction_had_had.pdf");
    }
    
  }


}

