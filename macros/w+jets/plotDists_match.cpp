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

void plotDists_match(){

  TFile *f = new TFile("Ztautau4M.root");

  bool saveplots = true;
  bool at	 = true;


  if (at){
    TCanvas *c1 = new TCanvas("c1","c1",800,600);
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.4,0.75,0.73,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/DPhi_match");
    TH1F *h2       	= (TH1F*)f->Get("361510/plotEventAlgs/taus/DPhi_no_match");
    int rebinval = 2;
    TGaxis::SetMaxDigits(4); 
    h->GetXaxis()->SetTitle("#Delta#phi(#slash{E}_{T},l) [rad] / #pi)");
    //h->GetXaxis()->SetTitle("#Delta#phi(#slash{E}_{T},l) [rad] / #pi)");
    //h->GetXaxis()->SetTitle("a_{T}#upoint#slash{E}_{T}*M_{T} [GeV]");
    h->GetYaxis()->SetTitle("Normalised events");
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);



    c1->SetRightMargin(0.15);
    c1->SetLeftMargin(0.12);
    c1->SetBottomMargin(0.12);

    h->Rebin(rebinval);
    h->SetFillStyle(1001);
    h->SetFillColor(38);
    h->SetLineColor(4);

    h2->Rebin(rebinval);
    h2->SetFillStyle(3001);
    h2->SetFillColor(46);
    h2->SetLineColor(49);

    hist_leg->AddEntry(h, Form("#tau^{reco} has #DeltaR match, 98.98%% of events"),"F");
    hist_leg->AddEntry(h2, Form("#tau^{reco} has no #DeltaR match, 1.02%% of events"),"F");

    //h->GetXaxis()->SetRangeUser(0,100);
    h->SetMinimum(0);
    h->SetMaximum(9);
    //h->GetXaxis()->SetRangeUser(-.1e5,.1e5);

    c1->Update();
    h->Draw("hist");
    h2->Draw("hist same");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"Z#rightarrow#tau_{h}#tau_{l}");

    //save plot
    if (saveplots){
      c1->SaveAs("plots/20161129/DPhi_match.pdf");
    }    
  }
}

