#include <TH1F.h>
#include <TH2F.h>
#include <TH1.h>
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

void plotAt(){

  TFile *f = new TFile("Ztautau1M.root");
  TFile *f1 = new TFile("Wplusmunu1M.root");

  bool saveplots = true;
  bool at	 = true;


  if (at){
    TCanvas *c9 = new TCanvas("c9","c9",800,600);
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.55,0.62,0.65,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/At_dilep_bkg_truth");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/At_dilep_bkg_truth");
    int rebinval = 10;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("True a_{T} distributions");
    h->GetXaxis()->SetTitle("a_{T} [MeV]");
    h->GetYaxis()->SetTitle("Normalised events / 10 GeV");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    //c9->SetLogy();
    c9->SetRightMargin(0.15);
    c9->SetLeftMargin(0.12);
    c9->SetBottomMargin(0.12);
    //    h->GetXaxis()->SetLimits(-4.,3.);
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{l}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));

    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    h->GetXaxis()->SetRangeUser(0,150e3);
    h->GetYaxis()->SetRangeUser(0.,0.07e-3);
    TH1* hc = h->GetCumulative();
    TH1* hc2 = h2->GetCumulative();

    h->Draw("C");
    h2->Draw("C SAME");
    //    hc->Draw("");
    //    hc2->Draw("same");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"ll final state");
    //save plot
    if (saveplots){
      c9->SaveAs("plots/20161016/w+jets/dilep/At_Ztautau_vs_wplusmunu.pdf");
    }    
  }



}

