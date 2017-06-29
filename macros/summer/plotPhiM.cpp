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

void plotPhiM(){

  TFile *ftautau = new TFile("Ztautau10M.root");
  TFile *fee	 = new TFile("root/Zee10M.root");
  TFile *fmumu	 = new TFile("root/Zmumu10M.root");

  bool saveplots = true;
  bool Phi_M	 = true;
  bool Mratio	 = false;


  if (Phi_M){
    TCanvas *c0 = new TCanvas("c0");
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat("e"); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.75,0.65,0.97,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);

    TH2F *Phi_M		= (TH2F*)ftautau->Get("361510/plotEventAlgs/taus/Phi_M_fraction_mu_tau");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    Phi_M->SetTitle("#phi^{*#mu#tau_{had}}_{#eta} vs. m^{obs.}_{#mu#tau_{had}}");
    //Phi_M->GetZaxis()->SetTitle("Events / 0.00001");
    Phi_M->GetXaxis()->SetTitle("m^{obs.}_{#mu#tau_{had}} / m^{MC}_{#tau#tau}");
    Phi_M->GetYaxis()->SetTitle("log_{10}(#phi^{*#mu#tau_{had}}_{#eta})");/*m^{obs.}_{ll} [GeV/c^{2}]*/
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(1.);
    //Phi_M->Rebin2D(10,1);
    Phi_M->GetXaxis()->SetRangeUser(15.0e3,160.0e3);
    Phi_M->GetYaxis()->SetRangeUser(-6.,2.);

    //Phi_M->GetYaxis()->SetRangeUser(0.0008,10.);
    //c0->SetLogy();
    c0->SetLogz();
    c0->SetRightMargin(0.15);
    c0->SetLeftMargin(0.12);
    //c0->SetLogx();
    Phi_M->Draw("COLZ");
    //hist_leg->Draw("same");
   
    

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.15,0.85,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau, #muh final state");
    Tl.DrawLatexNDC(0.15,0.8,Form("Correlation Factor %f", Phi_M->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c0->SaveAs("plots/20160620/Phi_M_fraction_tau_mu.pdf");
    }
    
  }


  if (Mratio){
    TCanvas *c1 = new TCanvas("c1");
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.65,0.63,0.86,0.85);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.025);
    //    hist_leg->SetTextFont(62);

    bool gaus = true;

    TH2F *Phi_M		= (TH2F*)ftautau->Get("361510/plotEventAlgs/Phi_M");

    int   NentriesPhiM	= (Phi_M->GetEntries());

    //Phi_M->SetTitle("#phi^{*}_{#eta} vs. m^{obs.}_{ll}/m^{MC}_{#tau#tau}, #DeltaR < .15 && p_{T} > 20 GeV");
    Phi_M->SetTitle("m^{obs.}_{ll}/m^{MC}_{#tau#tau} distribution for different #phi^{*}_{#eta}, #DeltaR < .15 && p_{T} > 10 GeV");
    Phi_M->GetZaxis()->SetTitle("Events / 0.02");
    Phi_M->GetXaxis()->SetTitle("m^{obs.}_{ll}/m^{MC}_{#tau#tau}");
    //    Phi_M->GetYaxis()->SetTitle("y");
    Phi_M->GetYaxis()->SetTitle("#phi^{*}_{#eta}");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(0.85);
    Phi_M->Rebin2D(10, 1);

    TH1D *px1 = Phi_M->ProjectionX("px1",1,475); //phi<.1
    px1->SetLineColor(1);
    hist_leg->AddEntry(px1, "log#phi^{*}_{#eta} < -2.25");
    //px1->GetXaxis()->SetRangeUser(0.,0.4);
    px1->GetXaxis()->SetLimits(0.49,0.52);
    px1->GetYaxis()->SetRangeUser(10,2000);
    px1->Rebin(2);
    if (gaus) px1->Fit("gaus");
    if (gaus) px1->GetFunction("gaus")->SetLineColor(1);
    if (gaus) px1->GetFunction("gaus")->SetLineWidth(1);
    TLine *px1line = new TLine(px1->GetMean(),10,px1->GetMean(),2000);
    px1line->SetLineColor(1);
    px1  ->GetYaxis()->SetTitle("Events / 0.02");

    TH1D *px2 = Phi_M->ProjectionX("px2",475,515); // .1<phi<1.0
    px2->SetLineColor(2);
    px2->Rebin(2);
    if (gaus) px2->Fit("gaus");
    if (gaus) px2->GetFunction("gaus")->SetLineColor(2);
    if (gaus) px2->GetFunction("gaus")->SetLineWidth(1);
    TLine *px2line = new TLine(px2->GetMean(),10,px2->GetMean(),2000);
    px2line->SetLineColor(2);
    hist_leg->AddEntry(px2, "-2.25 < log#phi^{*}_{#eta} < -1.85");

    TH1D *px3 = Phi_M->ProjectionX("px3",515,545); // 1.0<phi<10
    px3->SetLineColor(4);
    px3->Rebin(2);
    if (gaus) px3->Fit("gaus");
    if (gaus) px3->GetFunction("gaus")->SetLineColor(4);
    if (gaus) px3->GetFunction("gaus")->SetLineWidth(1);
    TLine *px3line = new TLine(px3->GetMean(),10,px3->GetMean(),2000);
    px3line->SetLineColor(4);
    hist_leg->AddEntry(px3, "-1.85 < log#phi^{*}_{#eta} < -1.55");

    TH1D *px4 = Phi_M->ProjectionX("px4",545,575); // 1.0<phi<10
    px4->SetLineColor(6);
    px4->Rebin(2);
    if (gaus) px4->Fit("gaus");
    if (gaus) px4->GetFunction("gaus")->SetLineColor(6);
    if (gaus) px4->GetFunction("gaus")->SetLineWidth(1);
    TLine *px4line = new TLine(px4->GetMean(),10,px4->GetMean(),2000);
    px4line->SetLineColor(6);
    hist_leg->AddEntry(px4, "-1.55 < log#phi^{*}_{#eta} < -1.25");

    TH1D *px5 = Phi_M->ProjectionX("px5",575,605); // 1.0<phi<10
    px5->SetLineColor(3);
    px5->Rebin(2);
    if (gaus) px5->Fit("gaus");
    if (gaus) px5->GetFunction("gaus")->SetLineColor(8);
    if (gaus) px5->GetFunction("gaus")->SetLineWidth(1);
    TLine *px5line = new TLine(px5->GetMean(),10,px5->GetMean(),2000);
    px5line->SetLineColor(3);
    hist_leg->AddEntry(px5, "-1.25 < log#phi^{*}_{#eta} < -0.95");

    TH1D *px6 = Phi_M->ProjectionX("px6",605,1000); // 1.0<phi<10
    px6->SetLineColor(7);
    px6->Rebin(2);
    if (gaus) px6->Fit("gaus");
    if (gaus) px6->GetFunction("gaus")->SetLineColor(9);
    if (gaus) px6->GetFunction("gaus")->SetLineWidth(1);
    TLine *px6line = new TLine(px6->GetMean(),10,px6->GetMean(),2000);
    px6line->SetLineColor(7);
    hist_leg->AddEntry(px6, "-0.95 < log#phi^{*}_{#eta}");


    c1->SetLogy();
    c1->SetRightMargin(0.15);
    px1->Draw("");
    px2->Draw("same");
    px3->Draw("same");
    px4->Draw("same");
    px5->Draw("same");
    px6->Draw("same");
    if (gaus) px1line->Draw("same");
    if (gaus) px2line->Draw("same");
    if (gaus) px3line->Draw("same");
    if (gaus) px4line->Draw("same");
    if (gaus) px5line->Draw("same");
    if (gaus) px6line->Draw("same");
    hist_leg->Draw("same");
   
    

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    //Tl.DrawLatexNDC(0.5,0.8,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau");
    //    Tl.DrawLatexNDC(0.5,0.75,Form("%i entries", NentriesPhiM));

    //save plot
    if (saveplots){
      c1->SaveAs("plots/20160609/Mratio_Phi_Slices_Gaussians_zoom.pdf");
    }
    
  }

}

