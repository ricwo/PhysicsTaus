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
#include <TGraph2D.h>
#include <TMultiGraph.h>

using namespace std;

void plotEff_2D_sherpa(const string histname){

  TFile *Z0 = new TFile("sherpa/Z+jets.root");
  TFile *W0 = new TFile("sherpa/W+jets.root");

  bool saveplots = false;
  bool aT	 = true;
  bool atdivphi	 = true;
  bool eff	 = true;
  TGraph *dt	 = new TGraph();

  TMultiGraph *mg = new TMultiGraph();
  
  //utility line
  TGraph *util	= new TGraph();
  for( int i = 1; i < 101; ++i ) util->SetPoint( i, (float)i / float(100), (float)i / (float)100 );
  TGraph *atlas = new TGraph();
  atlas->SetPoint(0,0.377524417496,0.019659 );
  TGraph *roi = new TGraph();
  string title;
  if( 1==1 ){
    TH2F *hsig		= (TH2F*)Z0->Get(histname.c_str());
    TH2F *hbkg		= (TH2F*)W0->Get(histname.c_str());
    const int max = hsig->GetNbinsY();
    Int_t MyPalette[max];
    Double_t Red[]    = {0., 0.0, 1.0, 1.0, 1.0};
    Double_t Green[]  = {0., 0.0, 0.0, 1.0, 1.0};
    Double_t Blue[]   = {0., 1.0, 0.0, 0.0, 1.0};
    Double_t Length[] = {0., .25, .50, .75, 1.0};
    Int_t FI = TColor::CreateGradientColorTable(5, Length, Red, Green, Blue, max);
    for (int i=0;i<max;i++) MyPalette[i] = FI+i;
    TH1D* hsig_all = hsig->ProjectionX("hsig_all",0,max);
    TH1D* hbkg_all = hbkg->ProjectionX("hbkg_all",0,max);
    double allsig		= hsig_all->Integral();
    double allbkg		= hbkg_all->Integral();
    cout << max << endl;
    cout << allsig << endl;
    cout << allbkg << endl;
    cout << hsig_all->GetBinWidth(1) << endl;
    for( int i = 1; i < max; ++i ){
      TH1D* hsig_sub = hsig->ProjectionX("hsig_sub",0,i);
      TH1D* hbkg_sub = hbkg->ProjectionX("hbkg_sub",0,i);
      double scale_sig	= hsig_sub->Integral() / allsig;
      double scale_bkg  = hbkg_sub->Integral() / allbkg;
      hsig_sub->Scale(1 / hsig_sub->Integral());
      hbkg_sub->Scale(1 / hbkg_sub->Integral());
      TH1* hsig_cum = hsig_sub->GetCumulative(kTRUE);
      TH1* hbkg_cum = hbkg_sub->GetCumulative(kTRUE);
      TGraph* tmp = new TGraph();
      for( int c = 0; c < hsig_cum->GetNbinsX() ; ++c ){
	if( hsig_cum->GetBinContent(c) * scale_sig > 0 ){
	  tmp->SetPoint( c, hsig_cum->GetBinContent(c) * scale_sig, hbkg_cum->GetBinContent(c) * scale_bkg);
	}
      }
      if( tmp->GetN() > 0 ){
	tmp->SetLineColor(MyPalette[i]);
	tmp->SetLineWidth(0.7);
	mg->Add(tmp);
      }

    }
  }

  TGraph *dt2	 = new TGraph();
  TH1F* hsig = (TH1F*)Z0->Get("Mstar2");
  TH1F* hbkg = (TH1F*)W0->Get("Mstar2");
  for( int i = 1; i<hsig->GetXaxis()->GetNbins()+1; ++i){
    dt2->SetPoint(i, hsig->Integral(1,i)/hsig->Integral(), hbkg->Integral(1,i)/hbkg->Integral());
  }

  TGraph *dt3	 = new TGraph();
  TH1F* hsig1 = (TH1F*)Z0->Get("Delta");
  TH1F* hbkg1 = (TH1F*)W0->Get("Delta");
  for( int i = 1; i<hsig1->GetXaxis()->GetNbins()+1; ++i){
    dt3->SetPoint(i, hsig1->Integral(1,i)/hsig->Integral(), hbkg1->Integral(1,i)/hbkg->Integral());
  }

  TGraph *dt4	 = new TGraph();
  TH1F* hsig2 = (TH1F*)Z0->Get("Gamma");
  TH1F* hbkg2 = (TH1F*)W0->Get("Gamma");
  for( int i = 1; i<hsig2->GetXaxis()->GetNbins()+1; ++i){
    dt4->SetPoint(i, hsig2->Integral(1,i)/hsig->Integral(), hbkg2->Integral(1,i)/hbkg->Integral());
  }


  if(  eff ){
    TCanvas *c20 = new TCanvas("c20","c20",800,600);
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat("e"); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg1 = new TLegend(0.815,0.50,1,0.9);
    hist_leg1->SetFillColor(0);
    hist_leg1->SetLineColor(1);
    hist_leg1->SetShadowColor(0);
    hist_leg1->SetTextSize(0.038);
    hist_leg1->SetTextFont(62);
    hist_leg1->SetFillStyle(0);
    hist_leg1->SetBorderSize(0);
    c20->SetRightMargin(0.18);
    c20->SetLeftMargin(0.14);
    c20->SetBottomMargin(0.14);
    c20->SetTopMargin(0.1);
    atlas->GetXaxis()->SetTitleOffset(0.8);
    atlas->GetYaxis()->SetTitleOffset(0.95);
    atlas->SetMarkerColor(1);
    atlas->SetMarkerStyle(3);
    atlas->SetMarkerSize(1.5);
    util->SetLineColor(1);
    util->SetLineStyle(2);
    util->SetLineWidth(1);
    dt->SetLineWidth(2);
    dt->SetLineColor(4);
    dt2->SetLineWidth(4);
    dt3->SetLineWidth(4);
    dt4->SetLineWidth(4);
    dt2->SetLineColor(8);
    dt3->SetLineColor(7);
    dt4->SetLineColor(28);
    hist_leg1->AddEntry(dt, Form("#splitline{m_{vis}(#mu,#tau_{had})}{+m^{*}(#mu,E_{T}^{miss})}"), "l");
    hist_leg1->AddEntry(dt2, Form("m^{*}(#mu,E_{T}^{miss})"), "l");
    hist_leg1->AddEntry(dt3, Form("#Delta"), "l");
    hist_leg1->AddEntry(dt4, Form("#Gamma"), "l");
    hist_leg1->AddEntry(atlas, Form("ATLAS"), "p" );
    //hist_leg1->AddEntry(roi, Form("ROI"), "p");
    mg->Draw("A L");
    dt2->Draw("same L");
    dt3->Draw("same L");
    dt4->Draw("same L");
    atlas->Draw("same P");
    util->Draw("same");
    hist_leg1->Draw("same");
    mg->GetXaxis()->SetTitle("#epsilon_{sig}");
    mg->GetYaxis()->SetTitle("#epsilon_{bkg}");
    mg->GetXaxis()->SetTitleSize(0.06);
    mg->GetYaxis()->SetTitleSize(0.06);
    mg->GetXaxis()->SetRangeUser(0,0.49);
    mg->GetYaxis()->SetRangeUser(0,0.03);
    TLatex Tl; Tl.SetTextAlign(12); Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.17,0.93,"Z#rightarrow#tau#tau (sig) vs W(#rightarrowl#nu)+jets (bkg), #mu#tau_{had} final state"); 
    string savename = "plots/20170512/Eff_2D_"+histname+"+mstar+delta.pdf";
    c20->SaveAs(savename.c_str());
  }
}
