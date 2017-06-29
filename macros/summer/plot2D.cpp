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

void plot2D(){

  TFile *f = new TFile("Wplusmunu1M.root");

  bool saveplots = true;
  bool Phi_M	 = true;

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

    TH2F *Phi_M		= (TH2F*)f->Get("361101/plotEventAlgs/taus/ATdotMET_PhiStar_r1");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("a_{T}#upoint#slash{E}_{T} vs #phi^{*}_{#eta}");
    Phi_M->GetYaxis()->SetTitle("a_{T}#upoint#slash{E}_{T} [GeV^{2}]");
    Phi_M->GetXaxis()->SetTitle("log_{10}#phi^{*}_{#eta}");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(1.);
    Phi_M->Rebin2D(1,20);
    Phi_M->GetXaxis()->SetRangeUser(-1.5,.1);
    //    Phi_M->GetYaxis()->SetRangeUser(-6.,2.);
    c0->SetLogz();
    c0->SetRightMargin(0.15);
    c0->SetLeftMargin(0.12);
    c0->SetBottomMargin(0.12);

    Phi_M->Draw("COLZ");

    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.15,0.85,"W+jets#rightarrow#mu#nu, lh final state");
    Tl.DrawLatexNDC(0.15,0.8,Form("Correlation Factor %f", Phi_M->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c0->SaveAs("plots/20161103/w+jets/semilep/reco/PhiStar_ATdotMET_w+jets.pdf");
    }
    
  }


}

