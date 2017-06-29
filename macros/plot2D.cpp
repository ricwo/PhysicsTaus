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

  TFile *f = new TFile("sherpa/Z+jets.root");
  TFile *f1 = new TFile("sherpa/W+jets.root");

  bool saveplots = true;
  bool hist	 = true;

  TGraph *line = new TGraph();
  for( int i =1; i < 401; ++i ) line->SetPoint(i, i, i);

  if (hist){
    TCanvas *c0 = new TCanvas("c0");
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.75,0.65,0.97,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.04);
    hist_leg->SetTextFont(62);
    TGaxis::SetMaxDigits(3); 

    TH2F *h    	= (TH2F*)f1->Get("RhovsDelta");

    h->GetXaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetXaxis()->SetTitleOffset(1.);
    h->GetYaxis()->SetTitleOffset(.9);
    h->GetXaxis()->CenterTitle(kFALSE);
    h->GetYaxis()->CenterTitle(kFALSE);
    h->Rebin2D(2,8);
    //    h->GetXaxis()->SetRangeUser(0,200);
    //h->GetYaxis()->SetRangeUser(0,200);
    c0->SetLogz();
    c0->SetRightMargin(0.15);
    c0->SetLeftMargin(0.12);
    c0->SetBottomMargin(0.14);

    h->Draw("COLZ");
    line->SetLineStyle(2);

    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    //Tl.DrawLatexNDC(0.15,0.93,"Z#rightarrow#tau#tau (sig), #mu#tau_{had} final state");
    Tl.DrawLatexNDC(0.15,0.93,"W(#rightarrowl#nu)+jets (bkg), #mu#tau_{had} final state");
    //    Tl.DrawLatexNDC(0.15,0.92,Form("Correlation Factor %f", h->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c0->SaveAs("plots/20170512/2D_RhovsDelta_bkg.pdf");
    }
    
  }


}

