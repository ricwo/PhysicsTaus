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

using namespace std;

void plotNewVar_semilep_reco(string hist, int rebin=1, double range_low=0, double range_high=0, double yrange_low = 0, double yrange_high = 0, bool saveplots = true)
{
  TFile *f = new TFile("Ztautau1M.root");
  TFile *f1 = new TFile("Wmunu1M.root");
  TFile *f2 = new TFile("Wminusmunu2M.root");

  bool makeplot	 = true;

  const char * histname = hist.c_str();

  if (makeplot){
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

    const string inputname = "361510/plotEventAlgs/taus/"+hist;
    const string inputname2 = "361101/plotEventAlgs/taus/"+hist;
    const string inputname3 = "361104/plotEventAlgs/taus/"+hist;
    TH1F *h		= (TH1F*)f->Get(inputname.c_str());
    TH1F *h2      	= (TH1F*)f1->Get(inputname2.c_str());
    //    TH1F *h3       	= (TH1F*)f2->Get(inputname3.c_str());
    //h2->Add(h3);
    int rebinval = rebin;
    TGaxis::SetMaxDigits(4); 
    h->GetYaxis()->SetTitle("Entries");
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

    c1->SetBottomMargin(0.12);

    h->Rebin(rebinval);
    h->SetFillStyle(1001);
    h->SetFillColor(38);
    h->SetLineColor(4);

    h2->Rebin(rebinval);
    h2->SetFillStyle(3001);
    h2->SetFillColor(46);
    h2->SetLineColor(2);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"),"F");
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"),"F");

    h->SetMinimum(0.001);
    if( range_low != 0 || range_high != 0 ){
      h->GetXaxis()->SetRangeUser(range_low,range_high);
    }
    if( yrange_low != 0 || yrange_high != 0 ){
      h->GetYaxis()->SetRangeUser(yrange_low,yrange_high);
    }


    c1->Update();
    h->Draw("hist");
    h2->Draw("hist same");
    hist_leg->Draw("same");

    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"PYTHIA, #muh final state");//, M^{*} < 50 GeV");

    //save plot
    if (saveplots){
      const string savename = "plots/20170309/"+hist+"_old.pdf";
      c1->SaveAs(savename.c_str());
    }    
  }
}

