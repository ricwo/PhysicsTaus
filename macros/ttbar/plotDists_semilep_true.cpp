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

using namespace std;

void plotDists_semilep_true(){

  TFile *f = new TFile("Ztautau1M.root");
  TFile *f1 = new TFile("ttbar1M.root");

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
    hist_leg = new TLegend(0.55,0.62,0.65,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/At_semilep_bkg_truth");
    TH1F *h2       	= (TH1F*)f1->Get("410000/plotEventAlgs/taus/At_semilep_bkg_truth");
    int rebinval = 5;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("True a_{T} distributions");
    h->GetXaxis()->SetTitle("a_{T} [MeV]");
    h->GetYaxis()->SetTitle("Normalised events / 5 GeV");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    //c1->SetLogy();
    c1->SetRightMargin(0.15);
    c1->SetLeftMargin(0.12);
    c1->SetBottomMargin(0.12);
    //    h->GetXaxis()->SetLimits(-4.,3.);
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));

    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    h->GetXaxis()->SetRangeUser(0,60e3);
    h->GetYaxis()->SetRangeUser(0.,80e-6);

    h->Draw("C");
    h2->Draw("C SAME");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.25,0.85,"lh final state");
    //save plot
    if (saveplots){
      c1->SaveAs("plots/20161016/ttbar/semilep/true/At_semilep_Ztautau_vs_ttbar_true.pdf");
    }    
  }

  if (at){
    TCanvas *c2 = new TCanvas("c2","c2",800,600);
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.55,0.62,0.65,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Pt_semilep_lep_bkg_truth");
    TH1F *h2       	= (TH1F*)f1->Get("410000/plotEventAlgs/taus/Pt_semilep_lep_bkg_truth");
    int rebinval = 1;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("True p_{T} distributions");
    h->GetXaxis()->SetTitle("p^{l}_{T} [MeV]");
    h->GetYaxis()->SetTitle("Normalised events / 5 GeV");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    //c2->SetLogy();
    c2->SetRightMargin(0.15);
    c2->SetLeftMargin(0.12);
    c2->SetBottomMargin(0.12);
    //    h->GetXaxis()->SetLimits(-4.,3.);
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));

    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    h->GetXaxis()->SetRangeUser(0,100e3);
    h->GetYaxis()->SetRangeUser(0.,80e-6);

    h->Draw("C");
    h2->Draw("C SAME");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.25,0.85,"lh final state");
    //save plot
    if (saveplots){
      c2->SaveAs("plots/20161016/ttbar/semilep/true/Pt_semilep_l_Ztautau_vs_ttbar_true.pdf");
    }    
  }

  if (at){
    TCanvas *c7 = new TCanvas("c7","c7",800,600);
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.55,0.62,0.65,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Pt_semilep_had_bkg_truth");
    TH1F *h2       	= (TH1F*)f1->Get("410000/plotEventAlgs/taus/Pt_semilep_had_bkg_truth");
    int rebinval = 1;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("True p_{T} distributions");
    h->GetXaxis()->SetTitle("p^{h}_{T} [MeV]");
    h->GetYaxis()->SetTitle("Normalised events / 5 GeV");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    //c7->SetLogy();
    c7->SetRightMargin(0.15);
    c7->SetLeftMargin(0.12);
    c7->SetBottomMargin(0.12);
    //    h->GetXaxis()->SetLimits(-4.,3.);
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));

    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    h->GetXaxis()->SetRangeUser(0,100e3);
    h->GetYaxis()->SetRangeUser(0.,80e-6);

    h->Draw("C");
    h2->Draw("C SAME");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.25,0.85,"lh final state");
    //save plot
    if (saveplots){
      c7->SaveAs("plots/20161016/ttbar/semilep/true/Pt_semilep_h_Ztautau_vs_ttbar_true.pdf");
    }    
  }


  if (at){
    TCanvas *c3 = new TCanvas("c3","c3",800,600);
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.55,0.62,0.65,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Eta_semilep_lep_bkg_truth");
    TH1F *h2       	= (TH1F*)f1->Get("410000/plotEventAlgs/taus/Eta_semilep_lep_bkg_truth");
    int rebinval = 1;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("True #eta distributions");
    h->GetXaxis()->SetTitle("#eta^{l}");
    h->GetYaxis()->SetTitle("Normalised events");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    //c3->SetLogy();
    c3->SetRightMargin(0.15);
    c3->SetLeftMargin(0.12);
    c3->SetBottomMargin(0.12);
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));

    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    h->GetXaxis()->SetRangeUser(-4.9,4.9);
    h->GetYaxis()->SetRangeUser(0.,.49);

    h->Draw("C");
    h2->Draw("C SAME");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.25,0.85,"lh final state");
    //save plot
    if (saveplots){
      c3->SaveAs("plots/20161016/ttbar/semilep/true/Eta_semilep_l_Ztautau_vs_ttbar_true.pdf");
    }    
  }
  if (at){
    TCanvas *c8 = new TCanvas("c8","c8",800,600);
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.55,0.62,0.65,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Eta_semilep_had_bkg_truth");
    TH1F *h2       	= (TH1F*)f1->Get("410000/plotEventAlgs/taus/Eta_semilep_had_bkg_truth");
    int rebinval = 1;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("True #eta distributions");
    h->GetXaxis()->SetTitle("#eta^{h}");
    h->GetYaxis()->SetTitle("Normalised events");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    //c8->SetLogy();
    c8->SetRightMargin(0.15);
    c8->SetLeftMargin(0.12);
    c8->SetBottomMargin(0.12);
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));

    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    h->GetXaxis()->SetRangeUser(-4.9,4.9);
    h->GetYaxis()->SetRangeUser(0.,.49);

    h->Draw("C");
    h2->Draw("C SAME");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.25,0.85,"lh final state");
    //save plot
    if (saveplots){
      c8->SaveAs("plots/20161016/ttbar/semilep/true/Eta_semilep_h_Ztautau_vs_ttbar_true.pdf");
    }    
  }


  if (at){
    TCanvas *c4 = new TCanvas("c4","c4",800,600);
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.45,0.62,0.55,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Phi_acop_semilep_bkg_truth");
    TH1F *h2       	= (TH1F*)f1->Get("410000/plotEventAlgs/taus/Phi_acop_semilep_bkg_truth");
    int rebinval = 5;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("True #phi_{acop} distributions");
    h->GetXaxis()->SetTitle("log_{10}#phi_{acop}");
    h->GetYaxis()->SetTitle("Normalised events");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    //c4->SetLogy();
    c4->SetRightMargin(0.15);
    c4->SetLeftMargin(0.12);
    c4->SetBottomMargin(0.12);
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));

    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    h->GetXaxis()->SetRangeUser(-4.,.55);
    h->GetYaxis()->SetRangeUser(0.,1.4);

    h->Draw("");
    h2->Draw("SAME");
    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.25,0.85,"lh final state");
    //save plot
    if (saveplots){
      c4->SaveAs("plots/20161016/ttbar/semilep/true/PhiAcop_semilep_Ztautau_vs_ttbar_true.pdf");
    }    
  }

  if (at){
    TCanvas *c5 = new TCanvas("c5","c5",800,600);
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.55,0.62,0.65,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Sin_theta_semilep_bkg_truth");
    TH1F *h2       	= (TH1F*)f1->Get("410000/plotEventAlgs/taus/Sin_theta_semilep_bkg_truth");
    int rebinval = 5;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("True sin(#theta^{*}_{#eta}) distributions");
    h->GetXaxis()->SetTitle("log_{10}sin(#theta^{*}_{#eta})");
    h->GetYaxis()->SetTitle("Normalised events");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    //c5->SetLogy();
    c5->SetRightMargin(0.15);
    c5->SetLeftMargin(0.12);
    c5->SetBottomMargin(0.12);
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);
    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));

    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    h->GetXaxis()->SetRangeUser(-.6,0.);
    //    h->GetYaxis()->SetRangeUser(0.,.34);

    h->Draw("");
    h2->Draw("SAME");
    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.25,0.85,"lh final state");
    //save plot
    if (saveplots){
      c5->SaveAs("plots/20161016/ttbar/semilep/true/Sin_theta_semilep_Ztautau_vs_ttbar_true.pdf");
    }    
  }

  if (at){
    TCanvas *c6 = new TCanvas("c6","c6",800,600);
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.55,0.62,0.65,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/PhiStarEta_semilep_bkg_truth");
    TH1F *h2       	= (TH1F*)f1->Get("410000/plotEventAlgs/taus/PhiStarEta_semilep_bkg_truth");
    int rebinval = 1;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("True #Phi^{*}_{#eta} distributions");
    h->GetXaxis()->SetTitle("log_{10}#phi^{*}_{#eta}");
    h->GetYaxis()->SetTitle("Normalised events");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    //c6->SetLogy();
    c6->SetRightMargin(0.15);
    c6->SetLeftMargin(0.12);
    c6->SetBottomMargin(0.12);
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);
    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));

    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    h->GetXaxis()->SetRangeUser(-4.,5.);
    h->GetYaxis()->SetRangeUser(0.,1.1);

    h->Draw("");
    h2->Draw("SAME");
    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"lh final state");
    //save plot
    if (saveplots){
      c6->SaveAs("plots/20161016/ttbar/semilep/true/PhiStar_semilep_Ztautau_vs_ttbar_true.pdf");
    }    
  }
}

