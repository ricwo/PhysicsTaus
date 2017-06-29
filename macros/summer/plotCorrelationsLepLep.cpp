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

void plotCorrelationsLepLep(){

  TFile *ftautau = new TFile("Ztautau5M.root");

  bool saveplots = true;
  bool Phi_M	 = true;
  bool Phi_M_alt = false;
  bool DPhi_pt	 = true;
  bool DPhi_phi	 = false;
  bool Pt_frac	 = true;
  bool M_pt	 = true;
  bool Phi_Mt	 = false;
  bool Phi_pT	 = false;
  bool DPhi_M    = false;
  bool dphi_2D   = false;
  bool Mratio	 = false;


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

    TH2F *Phi_M		= (TH2F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_M_fraction_lep_lep");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("#phi^{*ll}_{#eta} vs. m^{obs.}_{#tau_{lep}#tau_{lep}} / m^{MC}_{#tau#tau}");
    Phi_M->GetXaxis()->SetTitle("m^{obs.}_{ll} / m^{MC}_{#tau#tau}");
    Phi_M->GetYaxis()->SetTitle("log_{10}#phi^{*ll}_{#eta}");
    Phi_M->GetXaxis()->SetTitleSize(0.07);
    Phi_M->GetYaxis()->SetTitleSize(0.07);
    Phi_M->GetXaxis()->SetTitleOffset(0.8);
    Phi_M->GetYaxis()->SetTitleOffset(0.8);
    //    Phi_M->Rebin2D(2,2);
    Phi_M->GetXaxis()->SetRangeUser(0.,1.2);
    Phi_M->GetYaxis()->SetRangeUser(-6.,2.0);

    // c0->SetLeftMargin(0.5);
    // c0->SetBottomMargin(0.5);

    //Phi_M->GetYaxis()->SetRangeUser(0.0008,10.);
    //c0->SetLogy();
    //    c0->SetLogz();
    c0->SetRightMargin(0.14);
    c0->SetLeftMargin(0.14);
    c0->SetBottomMargin(0.15);
    //c0->SetLogx();
    Phi_M->Draw("COLZ");
    //hist_leg->Draw("same");
   
    

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.05);
    Tl.DrawLatexNDC(0.15,0.85,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau, ll final state");
    Tl.DrawLatexNDC(0.15,0.8,Form("Correlation factor %f", Phi_M->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c0->SaveAs("plots/20160711/dileptonic/Z0/Phi_M_fraction_lep_lep.pdf");
    }
    
  }

  if (dphi_2D){
    TCanvas *c11 = new TCanvas("c11");
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

    Phi_M->SetTitle("#Delta#phi_{l^{-}_{obs}#tau^{-}_{truth}} vs. #Delta#phi_{l^{+}_{obs}#tau^{+}_{truth}}");
    Phi_M->GetXaxis()->SetTitle("log_{10}#Delta#phi_{l^{+}_{obs}#tau^{+}_{truth}}");
    Phi_M->GetYaxis()->SetTitle("log_{10}#Delta#phi_{l^{-}_{obs}#tau^{-}_{truth}}");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(1.);
    //    Phi_M->Rebin2D(2,2);
    Phi_M->GetXaxis()->SetRangeUser(-6.,0);
    Phi_M->GetYaxis()->SetRangeUser(-6.,0);

    //Phi_M->GetYaxis()->SetRangeUser(0.0008,10.);
    //c11->SetLogy();
    c11->SetLogz();
    c11->SetRightMargin(0.15);
    c11->SetLeftMargin(0.12);
    c11->SetBottomMargin(0.12);
    //c11->SetLogx();
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
      c11->SaveAs("plots/20160711/dileptonic/Z0/DPhi_2D_lep_lep.pdf");
    }
    
  }

  if (Phi_M_alt){
    TCanvas *c10 = new TCanvas("c10");
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

    TH2F *Phi_M		= (TH2F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_M_fraction_alt_lep_lep");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("#phi^{*#tau_{lep}#tau_{lep}}_{#eta} vs. m^{obs.}_{#tau_{lep}#tau_{lep}} / m^{MC}_{#tau#tau}, #phi^{*#tau_{lep}#tau_{lep}}_{#eta} < #Delta#phi_{#tau_{lep}^{obs.}#tau^{truth}}");
    Phi_M->GetXaxis()->SetTitle("m^{obs.}_{#tau_{lep}#tau_{lep}} / m^{MC}_{#tau#tau}");
    Phi_M->GetYaxis()->SetTitle("log_{10}#phi^{*#tau_{lep}#tau_{lep}}_{#eta}");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(1.);
    Phi_M->GetXaxis()->SetRangeUser(0.,1.3);
    Phi_M->GetYaxis()->SetRangeUser(-6.,1.8);

    //Phi_M->GetYaxis()->SetRangeUser(0.0008,10.);
    //c10->SetLogy();
    //    c10->SetLogz();
    c10->SetRightMargin(0.15);
    c10->SetLeftMargin(0.12);
    c10->SetBottomMargin(0.12);
    //c10->SetLogx();
    Phi_M->Draw("COLZ");
    //hist_leg->Draw("same");
   
    

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau, hh final state");
    Tl.DrawLatexNDC(0.15,0.8,Form("Correlation factor %f", Phi_M->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c10->SaveAs("plots/20160711/dileptonic/Z0/Phi_M_fraction_alt_lep_lep.pdf");
    }
    
  }


  if (Phi_Mt){
    TCanvas *c6 = new TCanvas("c6");
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

    TH2F *Phi_M		= (TH2F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_Mt_fraction_lep_lep");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("#phi^{*#tau_{lep}#tau_{lep}}_{#eta} vs. m^{obs.}_{T(#tau_{lep}#tau_{lep})} / m^{MC}_{T(#tau#tau)}");
    Phi_M->GetXaxis()->SetTitle("m^{obs.}_{T(#tau_{lep}#tau_{lep})} / m^{MC}_{T(#tau#tau)}");
    Phi_M->GetYaxis()->SetTitle("log_{10}#phi^{*#tau_{lep}#tau_{lep}}_{#eta}");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(1.);
    //Phi_M->Rebin2D(10,1);
    Phi_M->GetXaxis()->SetRangeUser(0.,1.2);
    Phi_M->GetYaxis()->SetRangeUser(-6.,3.);

    //Phi_M->GetYaxis()->SetRangeUser(0.0008,10.);
    //c6->SetLogy();
    c6->SetLogz();
    c6->SetRightMargin(0.15);
    c6->SetLeftMargin(0.12);
    c6->SetBottomMargin(0.12);
    //c6->SetLogx();
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
      c6->SaveAs("plots/20160621/Phi_Mt_fraction_lep_lep.pdf");
    }
    
  }


  if (Phi_pT){
    TCanvas *c7 = new TCanvas("c7");
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

    TH2F *Phi_M		= (TH2F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_star_pT_fraction_lep_lep");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("#phi^{*#tau_{lep}#tau_{lep}}_{#eta} vs. p_{T}^{#tau_{lep},#tau_{lep}^{obs.}} / p_{T}^{#tau^{truth}_{lep,had}}");
    Phi_M->GetXaxis()->SetTitle("p_{T}^{#tau_{lep},#tau_{lep}^{obs.}} / p_{T}^{#tau^{truth}_{lep,had}}");
    Phi_M->GetYaxis()->SetTitle("log_{10}#phi^{*#tau_{lep}#tau_{lep}}_{#eta}");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(0.8);
    Phi_M->GetYaxis()->SetTitleOffset(1.);
    //Phi_M->Rebin2D(10,1);
    Phi_M->GetXaxis()->SetRangeUser(0.,1.6);
    Phi_M->GetYaxis()->SetRangeUser(-6.,3.);

    //Phi_M->GetYaxis()->SetRangeUser(0.0008,10.);
    //c7->SetLogy();
    c7->SetLogz();
    c7->SetRightMargin(0.15);
    c7->SetLeftMargin(0.12);
    c7->SetBottomMargin(0.12);
    //c7->SetLogx();
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
      c7->SaveAs("plots/20160621/Phi_pT_fraction_lep_lep.pdf");
    }
  }


  if (DPhi_M){
    TCanvas *c8 = new TCanvas("c8");
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

    TH2F *Phi_M		= (TH2F*)ftautau->Get("361510/plotEventAlgs/taus/DPhi_M_fraction_lep_lep");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("#Delta#phi_{#tau_{had,lep}^{obs.}#tau^{truth}_{had,lep}} vs. m^{obs.}_{T(#tau_{lep}#tau_{lep})} / m^{MC}_{T(#tau#tau)}");
    Phi_M->GetXaxis()->SetTitle("m^{obs.}_{T(#tau_{lep}#tau_{lep})} / m^{MC}_{T(#tau#tau)}");
    Phi_M->GetYaxis()->SetTitle("log_{10}#Delta#phi_{#tau_{had,lep}^{obs.}#tau^{truth}_{had,lep}}");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(1.);
    //Phi_M->Rebin2D(10,1);
    Phi_M->GetXaxis()->SetRangeUser(0.,1.2);
    Phi_M->GetYaxis()->SetRangeUser(-6.,1.);

    //Phi_M->GetYaxis()->SetRangeUser(0.0008,10.);
    //c8->SetLogy();
    c8->SetLogz();
    c8->SetRightMargin(0.15);
    c8->SetLeftMargin(0.12);
    c8->SetBottomMargin(0.12);
    //c8->SetLogx();
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
      c8->SaveAs("plots/20160621/DPhi_M_fraction_lep_lep.pdf");
    }
    
  }




  if (DPhi_pt){
    TCanvas *c2 = new TCanvas("c2");
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

    TH2F *Phi_M		= (TH2F*)ftautau->Get("361510/plotEventAlgs/taus/DPhi_pT_fraction_lep_lep");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("#Delta#phi_{#tau_{lep}^{obs.}#tau^{truth}} vs. p_{T}^{#tau_{lep}^{obs.}} / p_{T}^{#tau_{truth}}");
    //Phi_M->GetZaxis()->SetTitle("Events / 0.00001");
    Phi_M->GetYaxis()->SetTitle("log_{10}#Delta#phi_{l^{obs}#tau^{truth}}");
    Phi_M->GetXaxis()->SetTitle("p_{T}^{l_{obs}} / p_{T}^{#tau_{truth}}");/*m^{obs.}_{ll} [GeV/c^{2}]*/
    //    Phi_M->GetZaxis()->SetTitle("Events / (0.0001*log_{10}(#phi^{*#tau_{lep}#tau_{lep}}_{#eta}))");
    Phi_M->GetXaxis()->SetTitleSize(0.07);
    Phi_M->GetYaxis()->SetTitleSize(0.07);
    Phi_M->GetXaxis()->SetTitleOffset(0.8);
    Phi_M->GetYaxis()->SetTitleOffset(0.8);
    Phi_M->Rebin2D(2,1);
    //    Phi_M->GetYaxis()->SetRangeUser(-6.,0.);
    Phi_M->GetYaxis()->SetRangeUser(-6.,0.8);
    Phi_M->GetXaxis()->SetRangeUser(0,1.2);

    //Phi_M->GetYaxis()->SetRangeUser(0.0008,10.);
    //c2->SetLogy();
    c2->SetLogz();
    c2->SetRightMargin(0.15);
    c2->SetLeftMargin(0.14);
    c2->SetBottomMargin(0.14);
    //c2->SetLogx();
    Phi_M->Draw("COLZ");
    //hist_leg->Draw("same");
   
    

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.05);
    Tl.DrawLatexNDC(0.15,0.85,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau, ll final state");
    Tl.DrawLatexNDC(0.15,0.8,Form("Correlation factor %f", Phi_M->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c2->SaveAs("plots/20160711/dileptonic/Z0/DPhi_pT_fraction_lep_lep.pdf");
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

    TH2F *Phi_M		= (TH2F*)ftautau->Get("361510/plotEventAlgs/taus/M_fraction_pT_fraction_lep_lep");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("p_{T}^{#tau_{lep}^{obs. (1,2)}} / p_{T}^{#tau^{truth (1,2)}} vs. m^{obs.}_{#tau_{lep}#tau_{lep}} / m^{MC}_{#tau#tau}");
    Phi_M->GetZaxis()->SetTitle("Events / 0.0004");
    Phi_M->GetXaxis()->SetTitle("m^{obs.}_{#tau_{lep}#tau_{lep}} / m^{MC}_{#tau#tau}");
    Phi_M->GetYaxis()->SetTitle("p_{T}^{#tau_{lep}}^{obs. (1,2)} / p_{T}^{#tau^{truth (1,2)}}");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(1.);
    Phi_M->Rebin2D(2,2);
    Phi_M->GetXaxis()->SetRangeUser(0.,1.2);
    Phi_M->GetYaxis()->SetRangeUser(0.,1.4);
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
    Tl.DrawLatexNDC(0.15,0.88,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau, ll final state");
    Tl.DrawLatexNDC(0.15,0.83,Form("Correlation factor %f", Phi_M->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c5->SaveAs("plots/20160711/dileptonic/Z0/M_fraction_pT_fraction_lep_lep.pdf");
    }
    
  }


  if (Pt_frac){
    TCanvas *c4 = new TCanvas("c4");
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

    TH2F *Phi_M		= (TH2F*)ftautau->Get("361510/plotEventAlgs/taus/Pt_fraction_lep_lep");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("p_{T}^{#tau_{-lep}^{obs.}} / p_{T}^{#tau^{truth}_{-lep}} vs. p_{T}^{#tau_{+lep}^{obs.}} / p_{T}^{#tau^{truth}_{+lep}}");
    Phi_M->GetZaxis()->SetTitle("Events / 0.0004");
    Phi_M->GetXaxis()->SetTitle("p_{T}^{+l^{obs}} / p_{T}^{#tau^{truth}_{+lep}}");
    Phi_M->GetYaxis()->SetTitle("p_{T}^{-l^{obs}} / p_{T}^{#tau^{truth}_{-lep}}");
    Phi_M->GetXaxis()->SetTitleSize(0.07);
    Phi_M->GetYaxis()->SetTitleSize(0.07);
    Phi_M->GetXaxis()->SetTitleOffset(0.8);
    Phi_M->GetYaxis()->SetTitleOffset(0.8);
    Phi_M->Rebin2D(2,2);
    Phi_M->GetXaxis()->SetRangeUser(0.,1.4);
    Phi_M->GetYaxis()->SetRangeUser(0.,1.4);
    //    c4->SetLogz();
    c4->SetRightMargin(0.15);
    c4->SetLeftMargin(0.14);
    c4->SetBottomMargin(0.16);
    //c4->SetLogx();
    Phi_M->Draw("COLZ");
    //hist_leg->Draw("same");
   
    

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.05);
    Tl.DrawLatexNDC(0.15,0.85,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau, ll final state");
    Tl.DrawLatexNDC(0.15,0.8,Form("Correlation factor %f", Phi_M->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c4->SaveAs("plots/20160711/dileptonic/Z0/Pt_fraction_lep_lep.pdf");
    }
    
  }


  if (DPhi_phi){
    TCanvas *c3 = new TCanvas("c3");
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

    TH2F *Phi_M		= (TH2F*)ftautau->Get("361510/plotEventAlgs/taus/DPhi_Phi_star_lep_lep");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("#phi^{*#tau_{lep}#tau_{lep}}_{#eta} vs. #Delta#phi_{#tau_{had,lep}^{obs.}#tau^{truth}_{had,lep}}");
    //Phi_M->GetZaxis()->SetTitle("Events / 0.00001");
    Phi_M->GetXaxis()->SetTitle("log_{10}#Delta#phi_{#tau_{had,lep}^{obs.}#tau^{truth}_{had,lep}}");
    Phi_M->GetYaxis()->SetTitle("log_{10}#phi^{*#tau_{lep}#tau_{lep}}_{#eta}");/*m^{obs.}_{ll} [GeV/c^{2}]*/
    //    Phi_M->GetZaxis()->SetTitle("Events / (0.0001*log_{10}(#phi^{*#tau_{lep}#tau_{lep}}_{#eta}))");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(1.);
    //Phi_M->Rebin2D(10,1);
    Phi_M->GetXaxis()->SetRangeUser(-7.,0.);
    //Phi_M->GetYaxis()->SetRangeUser(-6.,3.);

    //Phi_M->GetYaxis()->SetRangeUser(0.0008,10.);
    //c3->SetLogy();
    c3->SetLogz();
    c3->SetRightMargin(0.15);
    c3->SetLeftMargin(0.12);
    c3->SetBottomMargin(0.12);
    //c3->SetLogx();
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
      c3->SaveAs("plots/20160621/DPhi_phi_star_lep_lep.pdf");
    }
    
  }


  if (Mratio){
    TCanvas *c1 = new TCanvas("c1");
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.65,0.63,0.86,0.85);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.025);
    //    hist_leg->SetTextFont(62);

    bool gaus = true;

    TH2F *Phi_M		= (TH2F*)ftautau->Get("361510/plotEventAlgs/Phi_M");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    //Phi_M->SetTitle("#phi^{*}_{#eta} vs. m^{obs.}_{ll}/m^{MC}_{#tau#tau}, #DeltaR < .15 && p_{T} > 20 GeV");
    Phi_M->SetTitle("m^{obs.}_{ll}/m^{MC}_{#tau#tau} distribution for different #phi^{*}_{#eta}, #DeltaR < .15 && p_{T} > 10 GeV");
    Phi_M->GetZaxis()->SetTitle("Events / 0.02");
    Phi_M->GetXaxis()->SetTitle("m^{obs.}_{ll}/m^{MC}_{#tau#tau}");
    //    Phi_M->GetYaxis()->SetTitle("y");
    Phi_M->GetYaxis()->SetTitle("#phi^{*}_{#eta}");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(0.85);
    Phi_M->Rebin2D(10, 1);

    TH1D *px1 = Phi_M->ProjectionX("px1",1,475); //phi<.1
    px1->SetLineColor(1);
    hist_leg->AddEntry(px1, "log#phi^{*}_{#eta} < -2.25");
    //px1->GetXaxis()->SetRangeUser(0.,0.4);
    px1->GetXaxis()->SetLimits(0.49,0.52);
    px1->GetYaxis()->SetRangeUser(10,2000);
    px1->Rebin(2);
    if (gaus) px1->Fit("gaus");
    if (gaus) px1->GetFunction("gaus")->SetLineColor(1);
    if (gaus) px1->GetFunction("gaus")->SetLineWidth(1);
    TLine *px1line = new TLine(px1->GetMean(),10,px1->GetMean(),2000);
    px1line->SetLineColor(1);
    px1  ->GetYaxis()->SetTitle("Events / 0.02");

    TH1D *px2 = Phi_M->ProjectionX("px2",475,515); // .1<phi<1.0
    px2->SetLineColor(2);
    px2->Rebin(2);
    if (gaus) px2->Fit("gaus");
    if (gaus) px2->GetFunction("gaus")->SetLineColor(2);
    if (gaus) px2->GetFunction("gaus")->SetLineWidth(1);
    TLine *px2line = new TLine(px2->GetMean(),10,px2->GetMean(),2000);
    px2line->SetLineColor(2);
    hist_leg->AddEntry(px2, "-2.25 < log#phi^{*}_{#eta} < -1.85");

    TH1D *px3 = Phi_M->ProjectionX("px3",515,545); // 1.0<phi<10
    px3->SetLineColor(4);
    px3->Rebin(2);
    if (gaus) px3->Fit("gaus");
    if (gaus) px3->GetFunction("gaus")->SetLineColor(4);
    if (gaus) px3->GetFunction("gaus")->SetLineWidth(1);
    TLine *px3line = new TLine(px3->GetMean(),10,px3->GetMean(),2000);
    px3line->SetLineColor(4);
    hist_leg->AddEntry(px3, "-1.85 < log#phi^{*}_{#eta} < -1.55");

    TH1D *px4 = Phi_M->ProjectionX("px4",545,575); // 1.0<phi<10
    px4->SetLineColor(6);
    px4->Rebin(2);
    if (gaus) px4->Fit("gaus");
    if (gaus) px4->GetFunction("gaus")->SetLineColor(6);
    if (gaus) px4->GetFunction("gaus")->SetLineWidth(1);
    TLine *px4line = new TLine(px4->GetMean(),10,px4->GetMean(),2000);
    px4line->SetLineColor(6);
    hist_leg->AddEntry(px4, "-1.55 < log#phi^{*}_{#eta} < -1.25");

    TH1D *px5 = Phi_M->ProjectionX("px5",575,605); // 1.0<phi<10
    px5->SetLineColor(3);
    px5->Rebin(2);
    if (gaus) px5->Fit("gaus");
    if (gaus) px5->GetFunction("gaus")->SetLineColor(8);
    if (gaus) px5->GetFunction("gaus")->SetLineWidth(1);
    TLine *px5line = new TLine(px5->GetMean(),10,px5->GetMean(),2000);
    px5line->SetLineColor(3);
    hist_leg->AddEntry(px5, "-1.25 < log#phi^{*}_{#eta} < -0.95");

    TH1D *px6 = Phi_M->ProjectionX("px6",605,1000); // 1.0<phi<10
    px6->SetLineColor(7);
    px6->Rebin(2);
    if (gaus) px6->Fit("gaus");
    if (gaus) px6->GetFunction("gaus")->SetLineColor(9);
    if (gaus) px6->GetFunction("gaus")->SetLineWidth(1);
    TLine *px6line = new TLine(px6->GetMean(),10,px6->GetMean(),2000);
    px6line->SetLineColor(7);
    hist_leg->AddEntry(px6, "-0.95 < log#phi^{*}_{#eta}");


    c1->SetLogy();
    c1->SetRightMargin(0.15);
    px1->Draw("");
    px2->Draw("same");
    px3->Draw("same");
    px4->Draw("same");
    px5->Draw("same");
    px6->Draw("same");
    if (gaus) px1line->Draw("same");
    if (gaus) px2line->Draw("same");
    if (gaus) px3line->Draw("same");
    if (gaus) px4line->Draw("same");
    if (gaus) px5line->Draw("same");
    if (gaus) px6line->Draw("same");
    hist_leg->Draw("same");
   
    

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    //Tl.DrawLatexNDC(0.5,0.8,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau");
    //    Tl.DrawLatexNDC(0.5,0.75,Form("%i entries", NentriesPhiM));

    //save plot
    if (saveplots){
      c1->SaveAs("plots/20160609/Mratio_Phi_Slices_Gaussians_zoom.pdf");
    }
    
  }

}

