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
#include <TAttFill.h>
#include <cstring>
#include <iomanip> // setprecision

using namespace std;

void printCutFlow(string hist)
{
  TFile *Z0 = new TFile("sherpa/Z+jets.root");
  TFile *W0 = new TFile("sherpa/W+jets.root");

  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  gROOT->SetStyle("Plain");
  gROOT->ForceStyle();
  gStyle->SetOptStat(0); 
  gStyle->SetTitleFontSize(0.04);
  gStyle->SetTitleX(0.5);
  gStyle->SetTitleAlign(23);
  hist_leg = new TLegend(0.6,0.75,0.8,0.82);
  hist_leg->SetFillColor(0);
  hist_leg->SetLineColor(1);
  hist_leg->SetShadowColor(0);
  hist_leg->SetTextSize(0.04);
  hist_leg->SetTextFont(62);
  hist_leg->SetBorderSize(0);
  
  TH1F *h		= (TH1F*)Z0->Get(hist.c_str()); 
  TH1F *h2		= (TH1F*)W0->Get(hist.c_str());
  for( int bin = 1; bin < h->GetXaxis()->GetNbins()+1; ++bin ){
    cout << bin << " " <<h->Integral(bin,bin) << " +/- "<< h->GetBinError(bin,bin) << "   " << h2->Integral(bin,bin) << " +/- " << h2->GetBinError(bin,bin) <<endl;
  }

  const char* title = "Events";
  //h2->Scale(1 / h2->Integral("width"));
  h->GetXaxis()->SetTitleSize(0.05);
  h->GetYaxis()->SetTitleSize(0.05);
  h->GetYaxis()->SetLabelSize(0.04);
  h->GetXaxis()->SetLabelSize(0.04);
  h->GetXaxis()->SetTitleOffset(1.);
  h->GetYaxis()->SetTitleOffset(0.95);
  h->GetXaxis()->CenterTitle(kFALSE);
  h->GetYaxis()->CenterTitle(kFALSE);
  
  
  c1->SetBottomMargin(0.12);

  h->SetFillStyle(3005);
  h->SetFillColor(38);
  h->SetLineColor(4);
  

  h2->SetFillStyle(3004);
  h2->SetFillColor(46);
  h2->SetLineColor(2);
  
  hist_leg->AddEntry(h, Form("Z#rightarrow#tau#tau"),"F");
  hist_leg->AddEntry(h2, Form("W(#rightarrowl#nu)+jets"),"F");
  

}
