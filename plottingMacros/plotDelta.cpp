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

void plotDelta(int rebinval = 4)
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
  
  TH2F *h2D		= (TH2F*)Z0->Get("AtvsDelta"); 
  TH2F *h22D		= (TH2F*)W0->Get("AtvsDelta");

  TH1D *h	= h2D->ProjectionX("h");
  TH1D *h2	= h22D->ProjectionX("h2");

  // create vector of pointers to histograms
  for( int i = 1; i < 2; i+=1 ){
    TCanvas *canvas = new TCanvas("canvas","canvas",800,600);
    TH1D *sig = h2D->ProjectionY("sig",1,55);
    TH1D *bkg = h22D->ProjectionY("bkg",1,55);
    sig->GetYaxis()->SetTitle("Entries");
    //    if( sig->Integral()>0) sig->Scale(1 / sig->Integral());
    //if( bkg->Integral()>0) bkg->Scale(1 / bkg->Integral());
    //    sig->Scale(1 / 17550.4);
    //    bkg->Scale(1 / );

    sig->SetFillStyle(1001);
    sig->SetFillColor(38);
    sig->SetLineColor(4);
    
    bkg->SetFillStyle(3003);
    bkg->SetFillColor(46);
    bkg->SetLineColor(2);

    sig->Rebin(rebinval);
    bkg->Rebin(rebinval);

    sig->GetXaxis()->CenterTitle(kFALSE);

    int maxbinsig = sig->GetMaximumBin();
    double maxsig = sig->GetBinContent(maxbinsig);
    int maxbinbkg = bkg->GetMaximumBin();
    double maxbkg = bkg->GetBinContent(maxbinbkg);
    double maxrange = maxsig > maxbkg ? maxsig * 1.1 : maxbkg * 1.1;
    sig->GetYaxis()->SetRangeUser(0, maxrange);
    

    sig->Draw("hist");
    bkg->Draw("same hist");
    string s = to_string(i);
    if( i< 10 ) s = '0'+s;
    string savestring = "plots/20170510/Gamma_Mstar3_55.pdf";
    TLatex Tl1; Tl1.SetTextAlign(12); Tl1.SetTextSize(0.03);
    string savelatex = "55 < #Delta <= 77";//+s;
    cout << i << "\t sig mean: " <<sig->GetMean() << "\t bkg mean: " << bkg->GetMean() << "\t centre: " <<(sig->GetMean()+bkg->GetMean())/2 << endl;
    Tl1.DrawLatexNDC(0.15,0.85,savelatex.c_str());
    //canvas->SaveAs(savestring.c_str());
    delete canvas;
    delete sig;
    delete bkg;
  }
  
  TGaxis::SetMaxDigits(4); 
  h->GetYaxis()->SetTitle("Events / 2");
  //  h->Scale(1 / h->Integral("width"));
  //h2->Scale(1 / h2->Integral("width"));
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetLabelSize(0.04);
  h->GetXaxis()->SetLabelSize(0.04);
  h->GetXaxis()->SetTitleOffset(.9);
  h->GetYaxis()->SetTitleOffset(.83);
  h->GetXaxis()->CenterTitle(kFALSE);
  h->GetYaxis()->CenterTitle(kFALSE);
  h->Rebin(2);
  h2->Rebin(2);

  int maxbinsig = h->GetMaximumBin();
  double maxsig = h->GetBinContent(maxbinsig);
  int maxbinbkg = h2->GetMaximumBin();
  double maxbkg = h2->GetBinContent(maxbinbkg);
  double maxrange = maxsig > maxbkg ? maxsig * 1.2 : maxbkg * 1.2;
  h->GetYaxis()->SetRangeUser(0, maxrange);
  
  c1->SetBottomMargin(0.12);
  
  h->SetFillStyle(3005);
  h->SetFillColor(38);
  h->SetLineColor(4);
  
  h2->SetFillStyle(3004);
  h2->SetFillColor(46);
  h2->SetLineColor(2);
  
  hist_leg->AddEntry(h, Form("Z#rightarrow#tau#tau"),"F");
  hist_leg->AddEntry(h2, Form("W(#rightarrowl#nu)+jets"),"F");
  
  c1->Update();
  h->Draw("hist");
  h2->Draw("same hist");
  hist_leg->Draw("same");
  
  TLatex Tl; Tl.SetTextAlign(12); Tl.SetTextSize(0.04);
  Tl.DrawLatexNDC(0.15,0.85,"#mu#tau_{had} final state");
  //save plot
  const string savename = "plots/20170513/Delta_2D.pdf";
  c1->SaveAs(savename.c_str());
}
