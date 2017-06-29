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

void plotDists(string hist, int rebin=1, double range_low=0, double range_high=0,const char* units = "",  double yrange_low = 0, double yrange_high = 0,  bool saveplots = true)
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
  cout << h->Integral() << " " << h2->Integral()<<endl;
  int rebinval = rebin;
  h->Rebin(rebinval);
  h2->Rebin(rebinval);
  const char* title = "Events";
  string total(string(title)+units);
  TGaxis::SetMaxDigits(4); 
  h->GetYaxis()->SetTitle(total.c_str());
  //  h->Scale(1 / h->Integral("width"));
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
  
  if( range_low != 0 || range_high != 0 ){
    h->GetXaxis()->SetRangeUser(range_low,range_high);
  }
  if( yrange_low != 0 || yrange_high != 0 ){
    h->GetYaxis()->SetRangeUser(yrange_low,yrange_high);
  }

  int maxbinsig = h->GetMaximumBin();
  double maxsig = h->GetBinContent(maxbinsig);
  int maxbinbkg = h2->GetMaximumBin();
  double maxbkg = h2->GetBinContent(maxbinbkg);
  double maxrange = maxsig > maxbkg ? maxsig * 1.2 : maxbkg * 1.2;
  h->GetYaxis()->SetRangeUser(0, maxrange);
    
  c1->Update();
  h->Draw("hist");
  h2->Draw("hist same");
  hist_leg->Draw("same");
  
  TLatex Tl; Tl.SetTextAlign(12); Tl.SetTextSize(0.04);
  if( strncmp(hist.c_str(), "Roi1", 4) == 0 ) Tl.DrawLatexNDC(0.15,0.85,"#mu#tau_{had} final state, #Gamma < 83");
  else if( strncmp(hist.c_str(), "Roi2", 4) == 0 ) Tl.DrawLatexNDC(0.15,0.85,"#mu#tau_{had} final state, #Gamma <= 90");
  else if( strncmp(hist.c_str(), "Roi3", 4) == 0 ) Tl.DrawLatexNDC(0.15,0.85,"#mu#tau_{had} final state, a_{T} < 10 GeV");
  else if( strncmp(hist.c_str(), "Roi4", 4) == 0 ) Tl.DrawLatexNDC(0.15,0.85,"#mu#tau_{had} final state, 10 < a_{T} < 30 GeV");
  else if( strncmp(hist.c_str(), "Roi5", 4) == 0 ) Tl.DrawLatexNDC(0.15,0.85,"#mu#tau_{had} final state, a_{T} > 30 GeV");
  else if( strncmp(hist.c_str(), "Pass", 4) == 0 ) Tl.DrawLatexNDC(0.15,0.85,"#mu#tau_{had} final state, #Gamma #leq 80");
  else Tl.DrawLatexNDC(0.15,0.85,"#mu#tau_{had} final state");
  
  //save plot
  if (saveplots){
    const string savename = "plots/20170624/"+hist+".pdf";
    c1->SaveAs(savename.c_str());
  }    
}
