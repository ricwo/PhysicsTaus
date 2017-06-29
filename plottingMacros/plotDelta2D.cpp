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

using namespace std;

void plotDelta2D()
{
  TFile *Z0 = new TFile("sherpa/Z.root");
  TFile *W0 = new TFile("sherpa/W.root");

  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  gROOT->SetStyle("Plain");
  gROOT->ForceStyle();
  gStyle->SetOptStat(0); 
  gStyle->SetTitleFontSize(0.02);
  gStyle->SetTitleX(0.5);
  gStyle->SetTitleAlign(23);
  hist_leg = new TLegend(0.6,0.75,0.8,0.82);
  hist_leg->SetFillColor(0);
  hist_leg->SetLineColor(1);
  hist_leg->SetShadowColor(0);
  hist_leg->SetTextSize(0.03);
  hist_leg->SetTextFont(62);
  hist_leg->SetBorderSize(0);
  
  TH2F *h		= (TH2F*)Z0->Get("RhovsDelta"); 
  TH2F *h2		= (TH2F*)W0->Get("RhovsDelta");

  int rebinval = 1;
  TGaxis::SetMaxDigits(4); 
  //  h->GetYaxis()->SetTitle("Entries");
  h->Scale(1 / h->Integral("width"));
  h2->Scale(1 / h2->Integral("width"));
  h->GetXaxis()->SetTitleSize(0.04);
  h->GetYaxis()->SetTitleSize(0.03);
  h->GetYaxis()->SetLabelSize(0.03);
  h->GetXaxis()->SetLabelSize(0.03);
  h->GetXaxis()->SetTitleOffset(1.15);
  h->GetYaxis()->SetTitleOffset(1.25);
  h->GetXaxis()->CenterTitle(kFALSE);
  h->GetYaxis()->CenterTitle(kFALSE);

  h2->GetXaxis()->SetTitleSize(0.04);
  h2->GetYaxis()->SetTitleSize(0.03);
  h2->GetYaxis()->SetLabelSize(0.03);
  h2->GetXaxis()->SetLabelSize(0.03);
  h2->GetXaxis()->SetTitleOffset(1.15);
  h2->GetYaxis()->SetTitleOffset(1.25);
  h2->GetXaxis()->CenterTitle(kFALSE);
  h2->GetYaxis()->CenterTitle(kFALSE);
  //  h->GetYaxis()->SetRangeUser(0,.16);
  h2->Rebin2D(1,10);
  c1->SetLogz();
  
  c1->SetBottomMargin(0.12);
  
  h->SetFillStyle(1001);
  h->SetFillColor(38);
  h->SetLineColor(4);
  
  h2->SetFillStyle(3001);
  h2->SetFillColor(46);
  h2->SetLineColor(2);
  
  //hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{#mu}#tau_{h}"),"l");
  hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"),"l");
  
  c1->Update();
  h2->Draw("colz");
  //h2->Draw("same hist");
  hist_leg->Draw("same");
  
  TLatex Tl; Tl.SetTextAlign(12); Tl.SetTextSize(0.03);
  Tl.DrawLatexNDC(0.15,0.85,"SHERPA, #muh final state, Triangular cuts");
  //save plot
  const string savename = "plots/20170330/Delta_2D_bkg.pdf";
  c1->SaveAs(savename.c_str());
}
