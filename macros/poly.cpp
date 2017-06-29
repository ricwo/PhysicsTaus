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
#include <TH2Poly.h>
#include <TMultiGraph.h>

using namespace std;

void poly(const string histname, int rebinx = 8, int rebiny = 8){
  TFile *Z0 = new TFile("sherpa/Z+jets.root");
  TFile *W0 = new TFile("sherpa/W+jets.root");
  TH2F *hsig		= (TH2F*)Z0->Get(histname.c_str());
  TH2F *hbkg		= (TH2F*)W0->Get(histname.c_str());
  double intsig = hsig->Integral();
  double intbkg = hbkg->Integral();
  cout << hsig->Integral() << endl << hbkg->Integral() << endl;
  hsig->Rebin2D(rebinx,rebiny);
  hbkg->Rebin2D(rebinx,rebiny);
  TH2F *div	= (TH2F*)hsig->Clone("div");
  int nx = div->GetXaxis()->GetNbins();
  int ny = div->GetYaxis()->GetNbins();
  double Nsig = hsig->Integral();
  double Nbkg = hbkg->Integral();
  cout << Nsig << endl;
  cout << Nbkg << endl;
  cout << nx << " " << ny << endl;
  for( int i = 1; i <= nx; ++i ){
    for( int j = 1; j <= ny; ++j ){
      div->SetBinContent(i,j,0,0);
      double c1 = hsig->GetBinContent(i,j);
      double eff_sig = c1 / Nsig;;
      if( c1 > 0 ){
	double c2 = hbkg->GetBinContent(i,j);
	double eff_bkg = c2 / Nbkg;
	if( eff_sig > 1e-6 && eff_bkg > 0 ){
	  div->SetBinContent(i,j,eff_sig/eff_bkg);
	}
      }
    }
  }

  TCanvas *c = new TCanvas("c","c",800,600);
  gROOT->SetStyle("Plain");
  gROOT->ForceStyle();
  gStyle->SetOptStat(0); 
  gStyle->SetTitleFontSize(0.06);
  gStyle->SetTitleX(0.6);
  gStyle->SetTitleAlign(23);
  leg = new TLegend(0.83,0.75,1,0.90);
  leg->SetFillColor(0);
  leg->SetLineColor(1);
  leg->SetShadowColor(0);
  leg->SetTextSize(0.04);
  leg->SetTextFont(62);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  c->SetRightMargin(0.18);
  c->SetLeftMargin(0.14);
  c->SetBottomMargin(0.14);
  c->SetLogz();
  div->GetXaxis()->SetTitleSize(0.06);
  div->GetXaxis()->SetLabelSize(0.05);
  div->GetXaxis()->SetTitleOffset(1);
  div->GetYaxis()->SetTitleSize(0.06);
  div->GetYaxis()->SetLabelSize(0.05);
  div->GetYaxis()->SetTitleOffset(.9);
  div->GetZaxis()->SetTitleSize(0.06);
  div->GetZaxis()->SetLabelSize(0.05);
  div->GetZaxis()->SetTitleOffset(0.9);
  div->GetXaxis()->CenterTitle(kFALSE);
  div->GetYaxis()->CenterTitle(kFALSE);
  div->SetZTitle("#epsilon_{sig}/#epsilon_{bkg}");

  if( histname == "MvisvsMstar2" ){
    div->GetYaxis()->SetTitle("m_{vis}(#mu,#tau_{had}) [GeV]");
    div->GetXaxis()->SetTitle("m^{*}(#mu,E_{T}^{miss}) [GeV]");
    div->GetXaxis()->SetRangeUser(0,200);
    div->GetYaxis()->SetRangeUser(0,200);
  }
  //  div->GetYaxis()->SetRangeUser(-1,1);
  div->GetZaxis()->SetRangeUser(.1,1e3);
  div->Draw("colz");

  TLatex Tl; Tl.SetTextAlign(12); Tl.SetTextSize(0.04);
  Tl.DrawLatexNDC(0.17,0.85,"Z#rightarrow#tau#tau (sig) vs W(#rightarrowl#nu)+jets (bkg), #mu#tau_{had} final state"); 
  string savename = "plots/20170510/2D_"+histname+".pdf";
  c->SaveAs(savename.c_str());
}
