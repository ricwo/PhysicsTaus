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

void plot(){

  TFile *ftautau = new TFile("Ztautau1M.root");
  TFile *fee	 = new TFile("Zee1M.root");
  TFile *fmumu	 = new TFile("Zmumu1M.root");

  bool saveplots = true ;
  bool massdist  = false;
  bool Phi_M	 = true;
  bool comp_phi	 = false;
  bool comp_pt   = false;
  bool comp_ptlplus = false;
  bool comp_dphi = false;
  bool comp_eta  = false;

  if (Phi_M){
    TCanvas *c0 = new TCanvas("c0");
    //c0->SetTitle("#phi^{*}_{#eta} vs. m^{obs.}_{ll}/m^{MC}_{#tau#tau}");
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    //gStyle->SetTitleX(0.5);
    //gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.65,0.65,0.87,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    

    //declare the histograms needed for plotting
    //fcombined->Cd("361510/plotEventAlgs");
    //fcombined->Pwd();
    TH1F *Mtt		= (TH1F*)ftautau->Get("361510/plotEventAlgs/Mtt");
    TH1F *Mratio	= (TH1F*)ftautau->Get("361510/plotEventAlgs/Mratio");
    TH1F *PhiStarEta	= (TH1F*)ftautau->Get("361510/plotEventAlgs/PhiStarEta");
    TH2F *Phi_M		= (TH2F*)ftautau->Get("361510/plotEventAlgs/Phi_M");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    //Phi_M->SetTitle("#phi^{*}_{#eta} vs. m^{obs.}_{ll}/m^{MC}_{#tau#tau}");
    Phi_M->GetZaxis()->SetTitle("Events / 0.0001");
    Phi_M->GetXaxis()->SetTitle("m^{obs.}_{ll}/m^{MC}_{#tau#tau}");
    Phi_M->GetYaxis()->SetTitle("#phi^{*}_{#eta}");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(0.85);

    //==============================================================================//
    //USE SetLimits instead of SetRangeUser!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
    // works with log scale//
    Phi_M->GetYaxis()->SetLimits(0.008,20.);
 
    c0->SetLogy();
   c0->SetLogz();
    c0->SetRightMargin(0.15);
    //c0->SetLogx();
    Phi_M->Draw("COLZ");
    //    Phi_M->Print("all");

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.5,0.8,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau");
    //    Tl.DrawLatexNDC(0.5,0.75,Form("%i entries", NentriesPhiM));

    //save plot
    if (saveplots){
      c0->SaveAs("plots/Ztautau/Phi_Mll.pdf");
    }

    
  }



  if (comp_phi){
    TCanvas *c1 = new TCanvas("c1");
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    //gStyle->SetTitleX(0.5);
    //gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.65,0.65,0.87,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    

    //declare the histograms needed for plotting
    //fcombined->Cd("361510/plotEventAlgs");
    //fcombined->Pwd();
    TH1F *ee_PhiStarEta   	= (TH1F*)fee->Get("361500/plotEventAlgs/PhiStarEta");
    TH1F *mumu_PhiStarEta   	= (TH1F*)fmumu->Get("361505/plotEventAlgs/PhiStarEta");    
    TH1F *tautau_PhiStarEta   	= (TH1F*)ftautau->Get("361510/plotEventAlgs/PhiStarEta");

    ee_PhiStarEta->GetXaxis()->SetTitle("#phi^{*}_{#eta}");
    ee_PhiStarEta->GetYaxis()->SetTitle("Events / 0.1");
    ee_PhiStarEta->GetXaxis()->SetTitleSize(0.04);
    ee_PhiStarEta->GetYaxis()->SetTitleSize(0.06);
    ee_PhiStarEta->GetXaxis()->SetTitleOffset(1.15);
    ee_PhiStarEta->GetYaxis()->SetTitleOffset(0.85);
    ee_PhiStarEta->GetXaxis()->SetRangeUser(0,20);
    //ee_PhiStarEta->SetTitle("#phi^{*}_#eta distribution");

    ee_PhiStarEta->SetLineStyle(1);
    ee_PhiStarEta->SetLineWidth(1);
    hist_leg->AddEntry(ee_PhiStarEta, "Z#rightarrow ee");

    mumu_PhiStarEta->SetLineColor(2);
    mumu_PhiStarEta->SetLineWidth(1);
    hist_leg->AddEntry(mumu_PhiStarEta, "Z#rightarrow #mu#mu");

    tautau_PhiStarEta->SetLineColor(4);
    tautau_PhiStarEta->SetLineWidth(1);
    hist_leg->AddEntry(tautau_PhiStarEta, "Z#rightarrow #tau#tau");


    c1->SetLogy();
    c1->SetLogx();

    ee_PhiStarEta->Draw("");
    mumu_PhiStarEta->Draw("same");
    tautau_PhiStarEta->Draw("same");
    hist_leg->Draw("same");

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    //Tl.DrawLatexNDC(0.5,0.8,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau");
    //    Tl.DrawLatexNDC(0.5,0.75,Form("%i entries", NentriesPhiM));

    //save plot
    if (saveplots){
      c1->SaveAs("plots/compplots/comp_phi.pdf");
    }

    
  }


  if (comp_pt){
    TCanvas *c2 = new TCanvas("c2");
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    //gStyle->SetTitleX(0.5);
    //gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.65,0.65,0.87,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    

    //declare the histograms needed for plotting
    //fcombined->Cd("361510/plotEventAlgs");
    //fcombined->Pwd();
    TH1F *ee_Ptcomb   	= (TH1F*)fee->Get("361500/plotEventAlgs/Ptcomb");
    TH1F *mumu_Ptcomb   	= (TH1F*)fmumu->Get("361505/plotEventAlgs/Ptcomb");    
    TH1F *tautau_Ptcomb   	= (TH1F*)ftautau->Get("361510/plotEventAlgs/Ptcomb");

    ee_Ptcomb->GetXaxis()->SetTitle("p^{ll}_{T} [MeV]");
    ee_Ptcomb->GetYaxis()->SetTitle("Events / 0.1 MeV");
    ee_Ptcomb->GetXaxis()->SetTitleSize(0.04);
    ee_Ptcomb->GetYaxis()->SetTitleSize(0.06);
    ee_Ptcomb->GetXaxis()->SetTitleOffset(1.15);
    ee_Ptcomb->GetYaxis()->SetTitleOffset(0.85);
    ee_Ptcomb->GetXaxis()->SetRangeUser(0,120.0e3);
    //ee_Ptcomb->SetTitle("#phi^{*}_#eta distribution");

    ee_Ptcomb->SetLineStyle(1);
    ee_Ptcomb->SetLineWidth(1);
    hist_leg->AddEntry(ee_Ptcomb, "Z#rightarrow ee");

    mumu_Ptcomb->SetLineColor(2);
    mumu_Ptcomb->SetLineWidth(1);
    hist_leg->AddEntry(mumu_Ptcomb, "Z#rightarrow #mu#mu");

    tautau_Ptcomb->SetLineColor(4);
    tautau_Ptcomb->SetLineWidth(1);
    hist_leg->AddEntry(tautau_Ptcomb, "Z#rightarrow #tau#tau");


    c2->SetLogy();
    //c2->SetLogx();

    ee_Ptcomb->Draw("");
    mumu_Ptcomb->Draw("same");
    tautau_Ptcomb->Draw("same");
    hist_leg->Draw("same");

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    //Tl.DrawLatexNDC(0.5,0.8,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau");
    //    Tl.DrawLatexNDC(0.5,0.75,Form("%i entries", NentriesPhiM));

    //save plot
    if (saveplots){
      c2->SaveAs("plots/compplots/comp_pt.pdf");
    }

    
  }

  if (comp_ptlplus){
    TCanvas *c3 = new TCanvas("c3");
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    //gStyle->SetTitleX(0.5);
    //gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.65,0.65,0.87,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    

    //declare the histograms needed for plotting
    //fcombined->Cd("361510/plotEventAlgs");
    //fcombined->Pwd();
    TH1F *ee_Ptlplus   	= (TH1F*)fee->Get("361500/plotEventAlgs/Ptlplus");
    TH1F *mumu_Ptlplus   	= (TH1F*)fmumu->Get("361505/plotEventAlgs/Ptlplus");    
    TH1F *tautau_Ptlplus   	= (TH1F*)ftautau->Get("361510/plotEventAlgs/Ptlplus");

    ee_Ptlplus->GetXaxis()->SetTitle("p^{l^{+}}_{T} [MeV]");
    ee_Ptlplus->GetYaxis()->SetTitle("Events / 0.1 MeV");
    ee_Ptlplus->GetXaxis()->SetTitleSize(0.04);
    ee_Ptlplus->GetYaxis()->SetTitleSize(0.06);
    ee_Ptlplus->GetXaxis()->SetTitleOffset(1.15);
    ee_Ptlplus->GetYaxis()->SetTitleOffset(0.85);
    ee_Ptlplus->GetXaxis()->SetRangeUser(0,120.0e3);
    //ee_Ptlplus->SetTitle("#phi^{*}_#eta distribution");

    ee_Ptlplus->SetLineStyle(1);
    ee_Ptlplus->SetLineWidth(1);
    hist_leg->AddEntry(ee_Ptlplus, "Z#rightarrow ee");

    mumu_Ptlplus->SetLineColor(2);
    mumu_Ptlplus->SetLineWidth(1);
    hist_leg->AddEntry(mumu_Ptlplus, "Z#rightarrow #mu#mu");

    tautau_Ptlplus->SetLineColor(4);
    tautau_Ptlplus->SetLineWidth(1);
    hist_leg->AddEntry(tautau_Ptlplus, "Z#rightarrow #tau#tau");


    c3->SetLogy();
    //c2->SetLogx();

    ee_Ptlplus->Draw("");
    mumu_Ptlplus->Draw("same");
    tautau_Ptlplus->Draw("same");
    hist_leg->Draw("same");

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    //Tl.DrawLatexNDC(0.5,0.8,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau");
    //    Tl.DrawLatexNDC(0.5,0.75,Form("%i entries", NentriesPhiM));

    //save plot
    if (saveplots){
      c3->SaveAs("plots/compplots/ptlplus.pdf");
    }

    
  }



  if (comp_eta){
    TCanvas *c5 = new TCanvas("c5");
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    //gStyle->SetTitleX(0.5);
    //gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.80,0.65,0.97,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    

    //declare the histograms needed for plotting
    //fcombined->Cd("361510/plotEventAlgs");
    //fcombined->Pwd();
    TH1F *ee_EtaPlus		= (TH1F*)fee->Get("361500/plotEventAlgs/EtaPlus");
    TH1F *mumu_EtaPlus   	= (TH1F*)fmumu->Get("361505/plotEventAlgs/EtaPlus");    
    TH1F *tautau_EtaPlus   	= (TH1F*)ftautau->Get("361510/plotEventAlgs/EtaPlus");

    ee_EtaPlus->GetXaxis()->SetTitle("Single-lepton #eta_{l^{+}}");
    ee_EtaPlus->GetYaxis()->SetTitle("Events / 0.1");
    ee_EtaPlus->GetXaxis()->SetTitleSize(0.04);
    ee_EtaPlus->GetYaxis()->SetTitleSize(0.06);
    ee_EtaPlus->GetXaxis()->SetTitleOffset(1.15);
    ee_EtaPlus->GetYaxis()->SetTitleOffset(0.85);
    ee_EtaPlus->GetXaxis()->SetRangeUser(-4.,4.);
    //ee_EtaPlus->SetTitle("#phi^{*}_#eta distribution");

    ee_EtaPlus->SetLineStyle(1);
    ee_EtaPlus->SetLineWidth(1);
    hist_leg->AddEntry(ee_EtaPlus, "Z#rightarrow ee");

    mumu_EtaPlus->SetLineColor(2);
    mumu_EtaPlus->SetLineWidth(1);
    hist_leg->AddEntry(mumu_EtaPlus, "Z#rightarrow #mu#mu");

    tautau_EtaPlus->SetLineColor(4);
    tautau_EtaPlus->SetLineWidth(1);
    hist_leg->AddEntry(tautau_EtaPlus, "Z#rightarrow #tau#tau");


    c5->SetLogy();
    //c2->SetLogx();

    ee_EtaPlus->Draw("");
    mumu_EtaPlus->Draw("same");
    tautau_EtaPlus->Draw("same");
    hist_leg->Draw("same");

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    //Tl.DrawLatexNDC(0.5,0.8,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau");
    //    Tl.DrawLatexNDC(0.5,0.75,Form("%i entries", NentriesPhiM));

    //save plot
    if (saveplots){
      c5->SaveAs("plots/compplots/comp_eta.pdf");
    }

    
  }


  if (comp_dphi){
    TCanvas *c4 = new TCanvas("c4");
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    //gStyle->SetTitleX(0.5);
    //gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.55,0.65,0.72,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    

    //declare the histograms needed for plotting
    //fcombined->Cd("361510/plotEventAlgs");
    //fcombined->Pwd();
    TH1F *ee_DPhi   	= (TH1F*)fee->Get("361500/plotEventAlgs/DPhi");
    TH1F *mumu_DPhi   	= (TH1F*)fmumu->Get("361505/plotEventAlgs/DPhi");    
    TH1F *tautau_DPhi   	= (TH1F*)ftautau->Get("361510/plotEventAlgs/DPhi");

    ee_DPhi->GetXaxis()->SetTitle("Dilepton #Delta#phi [rad]");
    ee_DPhi->GetYaxis()->SetTitle("Events / 0.1");
    ee_DPhi->GetXaxis()->SetTitleSize(0.04);
    ee_DPhi->GetYaxis()->SetTitleSize(0.06);
    ee_DPhi->GetXaxis()->SetTitleOffset(1.15);
    ee_DPhi->GetYaxis()->SetTitleOffset(0.85);
    ee_DPhi->GetXaxis()->SetRangeUser(0,TMath::Pi());
    //ee_DPhi->SetTitle("#phi^{*}_#eta distribution");

    ee_DPhi->SetLineStyle(1);
    ee_DPhi->SetLineWidth(1);
    hist_leg->AddEntry(ee_DPhi, "Z#rightarrow ee");

    mumu_DPhi->SetLineColor(2);
    mumu_DPhi->SetLineWidth(1);
    hist_leg->AddEntry(mumu_DPhi, "Z#rightarrow #mu#mu");

    tautau_DPhi->SetLineColor(4);
    tautau_DPhi->SetLineWidth(1);
    hist_leg->AddEntry(tautau_DPhi, "Z#rightarrow #tau#tau");


    c4->SetLogy();
    //c2->SetLogx();

    ee_DPhi->Draw("");
    mumu_DPhi->Draw("same");
    tautau_DPhi->Draw("same");
    hist_leg->Draw("same");

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    //Tl.DrawLatexNDC(0.5,0.8,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau");
    //    Tl.DrawLatexNDC(0.5,0.75,Form("%i entries", NentriesPhiM));

    //save plot
    if (saveplots){
      c4->SaveAs("plots/compplots/comp_dphi.pdf");
    }

    
  }


}

