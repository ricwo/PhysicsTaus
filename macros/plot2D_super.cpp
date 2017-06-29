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

void plot2D_super(){

  TFile *f = new TFile("Ztautau1M.root");
  TFile *f1 = new TFile("Wplusmunu1M.root");

  bool saveplots = true;
  bool hist	 = true;

  if (hist){
    TCanvas *c0 = new TCanvas("c0");
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.6,0.65,0.85,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetFillStyle(0);
    hist_leg->SetBorderSize(0);
    TGaxis::SetMaxDigits(3); 

    TH2F *h		= (TH2F*)f->Get("361510/plotEventAlgs/taus/MT_DPhi");
    TH2F *h2    	= (TH2F*)f1->Get("361101/plotEventAlgs/taus/MT_DPhi");

    int   NentriesPhiM	= (h->GetEntries());
    h->GetXaxis()->SetTitle("#Delta#phi(#slash{E}_{T},l) [rad] / #pi");
    h->GetYaxis()->SetTitle("M_{T}(#slash{E}_{T},l) [GeV]");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetXaxis()->SetTitleOffset(1.05);
    h->GetYaxis()->SetTitleOffset(.95);
    h->Rebin2D(4,4);
    h2->Rebin2D(4,4);
    h->Scale(1 / h->Integral("width") );
    h2->Scale(1 / h->Integral("width") );
    // h->GetXaxis()->SetRangeUser(-4e3,1e3);
    // h->GetYaxis()->SetRangeUser(0,150);
    // h2->GetXaxis()->SetRangeUser(-4e3,1e3);
    // h2->GetYaxis()->SetRangeUser(0,150);
    //    c0->SetLogz();
    // c0->SetRightMargin(0.1);
    // c0->SetLeftMargin(0.1);
    // c0->SetBottomMargin(0.1);

    hist_leg->AddEntry(h, "Signal", "F");
    hist_leg->AddEntry(h2, "Background", "F");


    h->SetFillColorAlpha(38,1);
    h->SetMarkerColorAlpha(38,1);
    h2->SetFillColorAlpha(46,.3);
    h2->SetMarkerColorAlpha(46,.65);
    h->Draw("");
    h2->Draw("same");
    hist_leg->Draw("same");

    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.15,0.85,"lh final state");

    //save plot
    if (saveplots){
      c0->SaveAs("plots/20161124/w+jets/semilep/reco/MT_vs_DPhi_super.pdf");
    }
    
  }


}

