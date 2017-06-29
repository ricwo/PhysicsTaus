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

void plotDists_MET_semilep_reco(){

  TFile *f = new TFile("Ztautau1M.root");
  TFile *f1 = new TFile("Wplusmunu1M.root");

  bool saveplots = true;
  bool MET	 = true;


  if (MET){
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/CTdotMET");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/CTdotMET");
    int rebinval = 5;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco c_{T}#upoint#slash{E}_{T} distributions");
    h->GetXaxis()->SetTitle("c^{l}_{T}#upoint#slash{E}_{T} [GeV^{2}]");
    h->GetYaxis()->SetTitle("Normalised events / 5 GeV^{2}");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    c1->SetRightMargin(0.15);
    c1->SetLeftMargin(0.12);
    c1->SetBottomMargin(0.12);
    //    c1->SetLogy();
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets(#rightarrow #tau_{fake})"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));

    h->GetXaxis()->SetRangeUser(-200,200);


    h->Draw("");
    h2->Draw("SAME");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"all #phi^{*}_{#eta}");
    //    Tl.DrawLatexNDC(0.25,0.85,"#phi^{*}_{#eta}<0.35 - #epsilon_{sig}=0.98 - #epsilon_{bkg}=0.43");
    //save plot
    if (saveplots){
      c1->SaveAs("plots/20161101/w+jets/semilep/reco/CTdotMET_all_semilep_Ztautau_vs_w+jets_reco.pdf");
    }    
  }

  if (MET){
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/CTdotMET_r1");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/CTdotMET_r1");
    int rebinval = 5;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco c_{T}#upoint#slash{E}_{T} distributions");
    h->GetXaxis()->SetTitle("c^{l}_{T}#upoint#slash{E}_{T} [GeV^{2}]");
    h->GetYaxis()->SetTitle("Normalised events / 5 GeV^{2}");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    c2->SetRightMargin(0.15);
    c2->SetLeftMargin(0.12);
    c2->SetBottomMargin(0.12);
    //    c2->SetLogy();
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets(#rightarrow #tau_{fake})"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));

    h->GetXaxis()->SetRangeUser(-200,200);


    h->Draw("");
    h2->Draw("SAME");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    //    Tl.DrawLatexNDC(0.15,0.85,"all #phi^{*}_{#eta}");
    Tl.DrawLatexNDC(0.15,0.85,"0.1<#phi^{*}_{#eta}<0.35, #epsilon_{sig}=0.98, #epsilon_{bkg}=0.43");
    //save plot
    if (saveplots){
      c2->SaveAs("plots/20161101/w+jets/semilep/reco/CTdotMET_r1_semilep_Ztautau_vs_w+jets_reco.pdf");
    }    
  }
  if (MET){
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/CTdotMET_r2");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/CTdotMET_r2");
    int rebinval = 10;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco c_{T}#upoint#slash{E}_{T} distributions");
    h->GetXaxis()->SetTitle("c^{l}_{T}#upoint#slash{E}_{T} [GeV^{2}]");
    h->GetYaxis()->SetTitle("Normalised events / 10 GeV^{2}");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    c3->SetRightMargin(0.15);
    c3->SetLeftMargin(0.12);
    c3->SetBottomMargin(0.12);
    //    c3->SetLogy();
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets(#rightarrow #tau_{fake})"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));

    h->GetXaxis()->SetRangeUser(-300,300);


    h->Draw("");
    h2->Draw("SAME");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    //    Tl.DrawLatexNDC(0.15,0.85,"all #phi^{*}_{#eta}");
    Tl.DrawLatexNDC(0.15,0.85,"#phi^{*}_{#eta}>0.35, #epsilon_{sig}=0.02, #epsilon_{bkg}=0.57");
    //save plot
    if (saveplots){
      c3->SaveAs("plots/20161101/w+jets/semilep/reco/CTdotMET_r2_semilep_Ztautau_vs_w+jets_reco.pdf");
    }    
  }

  if (MET){
    TCanvas *c4 = new TCanvas("c4","c4",800,600);
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/CTdotMET_r3");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/CTdotMET_r3");
    int rebinval = 5;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco c_{T}#upoint#slash{E}_{T} distributions");
    h->GetXaxis()->SetTitle("c^{l}_{T}#upoint#slash{E}_{T} [GeV^{2}]");
    h->GetYaxis()->SetTitle("Normalised events / 5 GeV^{2}");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    c4->SetRightMargin(0.15);
    c4->SetLeftMargin(0.12);
    c4->SetBottomMargin(0.12);
    //    c4->SetLogy();
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets(#rightarrow #tau_{fake})"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));

    h->GetXaxis()->SetRangeUser(-200,200);


    h->Draw("");
    h2->Draw("SAME");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    //    Tl.DrawLatexNDC(0.15,0.85,"all #phi^{*}_{#eta}");
    Tl.DrawLatexNDC(0.15,0.85,"#phi^{*}_{#eta}<0.1, #epsilon_{sig}=0.65, #epsilon_{bkg}=0.14");
    //save plot
    if (saveplots){
      c4->SaveAs("plots/20161101/w+jets/semilep/reco/CTdotMET_r3_semilep_Ztautau_vs_w+jets_reco.pdf");
    }    
  }

  if (MET){
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/MT_MET_l");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/MT_MET_l");
    int rebinval = 20;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco M_{T}(#slash{E}_{T}l) distributions");
    h->GetXaxis()->SetTitle("M_{T}(#slash{E}_{T}l) [GeV]");
    h->GetYaxis()->SetTitle("Normalised events / 2 GeV");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    c5->SetRightMargin(0.15);
    c5->SetLeftMargin(0.12);
    c5->SetBottomMargin(0.12);
    //    c5->SetLogy();
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets(#rightarrow #tau_{fake})"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));

    //    h->GetXaxis()->SetRangeUser(-200,200);


    h->Draw("");
    h2->Draw("SAME");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"all #phi^{*}_{#eta}");
    //Tl.DrawLatexNDC(0.15,0.85,"#phi^{*}_{#eta}<0.1, #epsilon_{sig}=0.65, #epsilon_{bkg}=0.14");
    //save plot
    if (saveplots){
      c5->SaveAs("plots/20161101/w+jets/semilep/reco/MT_MET_l_all_semilep_Ztautau_vs_w+jets_reco.pdf");
    }    
  }
  if (MET){
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/MT_MET_l_r1");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/MT_MET_l_r1");
    int rebinval = 20;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco M_{T}(#slash{E}_{T}l) distributions");
    h->GetXaxis()->SetTitle("M_{T}(#slash{E}_{T}l) [GeV]");
    h->GetYaxis()->SetTitle("Normalised events / 2 GeV");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    c6->SetRightMargin(0.15);
    c6->SetLeftMargin(0.12);
    c6->SetBottomMargin(0.12);
    //    c6->SetLogy();
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets(#rightarrow #tau_{fake})"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));

    //    h->GetXaxis()->SetRangeUser(-200,200);


    h->Draw("");
    h2->Draw("SAME");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    //Tl.DrawLatexNDC(0.15,0.85,"all #phi^{*}_{#eta}");
    Tl.DrawLatexNDC(0.15,0.85,"0.1<#phi^{*}_{#eta}<0.35, #epsilon_{sig}=0.98, #epsilon_{bkg}=0.43");
    //save plot
    if (saveplots){
      c6->SaveAs("plots/20161101/w+jets/semilep/reco/MT_MET_l_r1_semilep_Ztautau_vs_w+jets_reco.pdf");
    }    
  }

  if (MET){
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/MT_MET_l_r2");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/MT_MET_l_r2");
    int rebinval = 50;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco M_{T}(#slash{E}_{T}l) distributions");
    h->GetXaxis()->SetTitle("M_{T}(#slash{E}_{T}l) [GeV]");
    h->GetYaxis()->SetTitle("Normalised events / 5 GeV");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    c7->SetRightMargin(0.15);
    c7->SetLeftMargin(0.12);
    c7->SetBottomMargin(0.12);
    //    c7->SetLogy();
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets(#rightarrow #tau_{fake})"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));

    h->GetYaxis()->SetRangeUser(0,40e-3);


    h->Draw("");
    h2->Draw("SAME");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;

    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"#phi^{*}_{#eta}>0.35, #epsilon_{sig}=0.02, #epsilon_{bkg}=0.57");
    //save plot
    if (saveplots){
      c7->SaveAs("plots/20161101/w+jets/semilep/reco/MT_MET_l_r2_semilep_Ztautau_vs_w+jets_reco.pdf");
    }    
  }
  if (MET){
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/MT_MET_l_r3");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/MT_MET_l_r3");
    int rebinval = 50;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco M_{T}(#slash{E}_{T}l) distributions");
    h->GetXaxis()->SetTitle("M_{T}(#slash{E}_{T}l) [GeV]");
    h->GetYaxis()->SetTitle("Normalised events / 5 GeV");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    c8->SetRightMargin(0.15);
    c8->SetLeftMargin(0.12);
    c8->SetBottomMargin(0.12);
    //    c8->SetLogy();
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets(#rightarrow #tau_{fake})"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));

    //    h->GetYaxis()->SetRangeUser(0,40e-3);


    h->Draw("");
    h2->Draw("SAME");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;

    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"#phi^{*}_{#eta}<0.1, #epsilon_{sig}=0.65, #epsilon_{bkg}=0.14");
    //save plot
    if (saveplots){
      c8->SaveAs("plots/20161101/w+jets/semilep/reco/MT_MET_l_r3_semilep_Ztautau_vs_w+jets_reco.pdf");
    }    
  }

  if (MET){
    TCanvas *c9 = new TCanvas("c9","c9",800,600);
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/CT");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/CT");
    int rebinval = 20;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco c_{T} distributions");
    h->GetXaxis()->SetTitle("c_{T} [GeV]");
    h->GetYaxis()->SetTitle("Normalised events / 5 GeV");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    c9->SetRightMargin(0.15);
    c9->SetLeftMargin(0.12);
    c9->SetBottomMargin(0.12);
    //    c9->SetLogy();
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets(#rightarrow #tau_{fake})"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));

    //    h->GetYaxis()->SetRangeUser(0,40e-3);


    h->Draw("");
    h2->Draw("SAME");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;

    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"all #phi^{*}_{#eta}");
    //save plot
    if (saveplots){
      c9->SaveAs("plots/20161101/w+jets/semilep/reco/cT_all_semilep_Ztautau_vs_w+jets_reco.pdf");
    }    
  }

  if (MET){
    TCanvas *c10 = new TCanvas("c10","c10",800,600);
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/CT_r1");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/CT_r1");
    int rebinval = 20;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco c_{T} distributions");
    h->GetXaxis()->SetTitle("c_{T} [GeV]");
    h->GetYaxis()->SetTitle("Normalised events / 5 GeV");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    c10->SetRightMargin(0.15);
    c10->SetLeftMargin(0.12);
    c10->SetBottomMargin(0.12);
    //    c10->SetLogy();
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets(#rightarrow #tau_{fake})"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));

    //    h->GetYaxis()->SetRangeUser(0,40e-3);


    h->Draw("");
    h2->Draw("SAME");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;

    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"0.1<#phi^{*}_{#eta}<0.35, #epsilon_{sig}=0.98, #epsilon_{bkg}=0.43");
    //save plot
    if (saveplots){
      c10->SaveAs("plots/20161101/w+jets/semilep/reco/cT_r1_semilep_Ztautau_vs_w+jets_reco.pdf");
    }    
  }

  if (MET){
    TCanvas *c11 = new TCanvas("c11","c11",800,600);
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/CT_r2");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/CT_r2");
    int rebinval = 20;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco c_{T} distributions");
    h->GetXaxis()->SetTitle("c_{T} [GeV]");
    h->GetYaxis()->SetTitle("Normalised events / 5 GeV");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    c11->SetRightMargin(0.15);
    c11->SetLeftMargin(0.12);
    c11->SetBottomMargin(0.12);
    //    c11->SetLogy();
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets(#rightarrow #tau_{fake})"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));

    //    h->GetYaxis()->SetRangeUser(0,40e-3);


    h->Draw("");
    h2->Draw("SAME");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;

    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"#phi^{*}_{#eta}>0.35, #epsilon_{sig}=0.02, #epsilon_{bkg}=0.57");
    //save plot
    if (saveplots){
      c11->SaveAs("plots/20161101/w+jets/semilep/reco/cT_r2_semilep_Ztautau_vs_w+jets_reco.pdf");
    }    
  }


  if (MET){
    TCanvas *c12 = new TCanvas("c12","c12",800,600);
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

    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/CT_r3");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/CT_r3");
    int rebinval = 20;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco c_{T} distributions");
    h->GetXaxis()->SetTitle("c_{T} [GeV]");
    h->GetYaxis()->SetTitle("Normalised events / 5 GeV");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    c12->SetRightMargin(0.15);
    c12->SetLeftMargin(0.12);
    c12->SetBottomMargin(0.12);
    //    c12->SetLogy();
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets(#rightarrow #tau_{fake})"));
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));

    //    h->GetYaxis()->SetRangeUser(0,40e-3);


    h->Draw("");
    h2->Draw("SAME");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;

    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"#phi^{*}_{#eta}<0.1, #epsilon_{sig}=0.65, #epsilon_{bkg}=0.14");
    //save plot
    if (saveplots){
      c12->SaveAs("plots/20161101/w+jets/semilep/reco/cT_r3_semilep_Ztautau_vs_w+jets_reco.pdf");
    }    
  }



}

