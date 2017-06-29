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

void plotDists_semilep_reco(){

  TFile *f = new TFile("Ztautau1M.root");
  TFile *f1 = new TFile("Wplusmunu1M.root");

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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/At_semilep_bkg");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/At_semilep_bkg");
    int rebinval = 5;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco a_{T} distributions");
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
    //    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    //    h->GetXaxis()->SetRangeUser(0,60e3);
    //h->GetYaxis()->SetRangeUser(0.,80e-6);

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
      c1->SaveAs("plots/20161025/w+jets/semilep/reco/At_semilep_Ztautau_vs_w+jets_reco.pdf");
    }    
  }

  if (at){
    TCanvas *c23 = new TCanvas("c23","c23",800,600);
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Arccos_Et_t_semilep");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/Arccos_Et_t_semilep");
    int rebinval = 5;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco missing #slash{E}_{T} distributions");
    h->GetXaxis()->SetTitle("#alpha(#vec{#slash{E}_{T}},#hat{t}) [rad]");
    h->GetYaxis()->SetTitle("Normalised events");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    //c23->SetLogy();
    c23->SetRightMargin(0.15);
    c23->SetLeftMargin(0.12);
    c23->SetBottomMargin(0.12);
    //    h->GetXaxis()->SetLimits(-4.,3.);
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    //    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    //    h->GetXaxis()->SetRangeUser(0,60e3);
    //h->GetYaxis()->SetRangeUser(0.,80e-6);

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
      c23->SaveAs("plots/20161025/w+jets/semilep/reco/Alpha_semilep_Ztautau_vs_w+jets_reco.pdf");
    }    
  }

  if (at){
    TCanvas *c18 = new TCanvas("c18","c18",800,600);
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Et_dot_t_abs_semilep");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/Et_dot_t_abs_semilep");
    int rebinval = 20;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco missing #slash{E}_{T} distributions");
    h->GetXaxis()->SetTitle("|#vec{#slash{E}_{T}}.#hat{t}| [MeV]");
    h->GetYaxis()->SetTitle("Normalised events");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    //c18->SetLogy();
    c18->SetRightMargin(0.15);
    c18->SetLeftMargin(0.12);
    c18->SetBottomMargin(0.12);
    //    h->GetXaxis()->SetLimits(-4.,3.);
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    //    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    //    h->GetXaxis()->SetRangeUser(0,60e3);
    //h->GetYaxis()->SetRangeUser(0.,80e-6);

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
      c18->SaveAs("plots/20161025/w+jets/semilep/reco/Et_dot_abs_semilep_Ztautau_vs_w+jets_reco.pdf");
    }    
  }

  if (at){
    TCanvas *c22 = new TCanvas("c22","c22",800,600);
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Et_dot_t_semilep");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/Et_dot_t_semilep");
    int rebinval = 20;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco missing #slash{E}_{T} distributions");
    h->GetXaxis()->SetTitle("#vec{#slash{E}_{T}}.#hat{t} [MeV^{2}]");
    h->GetYaxis()->SetTitle("Normalised events");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    //c22->SetLogy();
    c22->SetRightMargin(0.15);
    c22->SetLeftMargin(0.12);
    c22->SetBottomMargin(0.12);
    //    h->GetXaxis()->SetLimits(-4.,3.);
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    //    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    //    h->GetXaxis()->SetRangeUser(0,60e3);
    //h->GetYaxis()->SetRangeUser(0.,80e-6);

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
      c22->SaveAs("plots/20161025/w+jets/semilep/reco/Et_dot_semilep_Ztautau_vs_w+jets_reco.pdf");
    }    
  }

  if (at){
    TCanvas *c19 = new TCanvas("c19","c19",800,600);
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Et_cross_t_abs_semilep");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/Et_cross_t_abs_semilep");
    int rebinval = 20;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco #slash{E}_{T} distributions");
    h->GetXaxis()->SetTitle("|(#vec{#slash{E}_{T}}#times#hat{t})_{Z}| [MeV]");
    h->GetYaxis()->SetTitle("Normalised events");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    //c19->SetLogy();
    c19->SetRightMargin(0.15);
    c19->SetLeftMargin(0.12);
    c19->SetBottomMargin(0.12);
    //    h->GetXaxis()->SetLimits(-4.,3.);
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    //    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    //    h->GetXaxis()->SetRangeUser(0,60e3);
    //h->GetYaxis()->SetRangeUser(0.,80e-6);

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
      c19->SaveAs("plots/20161025/w+jets/semilep/reco/Et_cross_abs_semilep_Ztautau_vs_w+jets_reco.pdf");
    }    
  }
  if (at){
    TCanvas *c21 = new TCanvas("c21","c21",800,600);
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Et_cross_t_semilep");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/Et_cross_t_semilep");
    int rebinval = 20;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco #slash{E}_{T} distributions");
    h->GetXaxis()->SetTitle("(#vec{#slash{E}_{T}}#times#hat{t})_{Z} [MeV^{2}]");
    h->GetYaxis()->SetTitle("Normalised events");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    //c21->SetLogy();
    c21->SetRightMargin(0.15);
    c21->SetLeftMargin(0.12);
    c21->SetBottomMargin(0.12);
    //    h->GetXaxis()->SetLimits(-4.,3.);
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    //    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    //    h->GetXaxis()->SetRangeUser(0,60e3);
    //h->GetYaxis()->SetRangeUser(0.,80e-6);

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
      c21->SaveAs("plots/20161025/w+jets/semilep/reco/Et_cross_semilep_Ztautau_vs_w+jets_reco.pdf");
    }    
  }

  if (at){
    TCanvas *c20 = new TCanvas("c20","c20",800,600);
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Et_semilep");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/Et_semilep");
    int rebinval = 20;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco #slash{E}_{T} distributions");
    h->GetXaxis()->SetTitle("#slash{E}_{T} [MeV]");
    h->GetYaxis()->SetTitle("Normalised events");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    //c20->SetLogy();
    c20->SetRightMargin(0.15);
    c20->SetLeftMargin(0.12);
    c20->SetBottomMargin(0.12);
    //    h->GetXaxis()->SetLimits(-4.,3.);
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    //    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    //    h->GetXaxis()->SetRangeUser(0,60e3);
    //h->GetYaxis()->SetRangeUser(0.,80e-6);

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
      c20->SaveAs("plots/20161025/w+jets/semilep/reco/Et_semilep_Ztautau_vs_w+jets_reco.pdf");
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Pt_semilep_lep_bkg");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/Pt_semilep_lep_bkg");
    int rebinval = 1;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco p_{T} distributions");
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
    //    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    //    h->GetXaxis()->SetRangeUser(0,100e3);
    //h->GetYaxis()->SetRangeUser(0.,80e-6);

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
      c2->SaveAs("plots/20161025/w+jets/semilep/reco/Pt_semilep_l_Ztautau_vs_w+jets_reco.pdf");
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Pt_semilep_had_bkg");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/Pt_semilep_had_bkg");
    int rebinval = 1;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco p_{T} distributions");
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
    //    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    //    h->GetXaxis()->SetRangeUser(0,100e3);
    h->GetYaxis()->SetRangeUser(0.,100e-6);

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
      c7->SaveAs("plots/20161025/w+jets/semilep/reco/Pt_semilep_h_Ztautau_vs_w+jets_reco.pdf");
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Eta_semilep_lep_bkg");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/Eta_semilep_lep_bkg");
    int rebinval = 1;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco #eta distributions");
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
    //    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    h->GetXaxis()->SetRangeUser(-2.9,2.9);
    h->GetYaxis()->SetRangeUser(0.,.69);

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
      c3->SaveAs("plots/20161025/w+jets/semilep/reco/Eta_semilep_l_Ztautau_vs_w+jets_reco.pdf");
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Eta_semilep_had_bkg");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/Eta_semilep_had_bkg");
    int rebinval = 1;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco #eta distributions");
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
    //    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    h->GetXaxis()->SetRangeUser(-2.9,2.9);
    h->GetYaxis()->SetRangeUser(0.,.69);

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
      c8->SaveAs("plots/20161025/w+jets/semilep/reco/Eta_semilep_h_Ztautau_vs_w+jets_reco.pdf");
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Phi_acop_semilep_bkg");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/Phi_acop_semilep_bkg");
    int rebinval = 5;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco #phi_{acop} distributions");
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
    //    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    //    h->GetXaxis()->SetRangeUser(-4.,.55);
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
      c4->SaveAs("plots/20161025/w+jets/semilep/reco/PhiAcop_semilep_Ztautau_vs_w+jets_reco.pdf");
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Sin_theta_alt_semilep_bkg");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/Sin_theta_alt_semilep_bkg");
    int rebinval = 5;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco sin(#theta^{*}_{#eta}) distributions");
    h->GetXaxis()->SetTitle("log_{10}(1-sin(#theta^{*}_{#eta}))");
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
    //    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    h->GetXaxis()->SetRangeUser(-8,0.);
    h->GetYaxis()->SetRangeUser(0.,1.1);

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
      c5->SaveAs("plots/20161025/w+jets/semilep/reco/Sin_theta_alt_semilep_Ztautau_vs_w+jets_reco.pdf");
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/PhiStarEta_semilep_bkg");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/PhiStarEta_semilep_bkg");
    int rebinval = 1;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco #Phi^{*}_{#eta} distributions");
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
    //    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow lh"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    //    h->GetXaxis()->SetRangeUser(-4.,5.);
    //h->GetYaxis()->SetRangeUser(0.,1.1);

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
      c6->SaveAs("plots/20161025/w+jets/semilep/reco/PhiStar_semilep_Ztautau_vs_w+jets_reco.pdf");
    }    
  }
}
