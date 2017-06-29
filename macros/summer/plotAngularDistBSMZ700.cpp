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

void plotAngularDistBSMZ700(){

  TFile *ftautau = new TFile("Zprime700mutau.root");
  TFile *ftautau1 = new TFile("Zprime700etau.root");
  TFile *ftautau2 = new TFile("H700_tautau_lh.root");
  TFile *ftautau3 = new TFile("ZprimeSSM3000.root");

  bool saveplots = true;
  bool phi	 = false;
  bool mfraction = false;
  bool phitruth	 = true;

  //  bool Phi_acop_tau_tau = true;
  //bool Phi_acop_had_had = true;

  if (mfraction){
    TCanvas *c9 = new TCanvas("c9","c9",800,600);
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.62,0.62,0.80,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);

    TH1F *h		= (TH1F*)ftautau->Get("302006/plotEventAlgs/taus/Mratio_l_tau");
    TH1F *h1		= (TH1F*)ftautau1->Get("301981/plotEventAlgs/taus/Mratio_l_tau");
    h->Add(h,h1);
    TH1F *h2       	= (TH1F*)ftautau2->Get("342320/plotEventAlgs/taus/semilep/Mratio");
    TH1F *h3       	= (TH1F*)ftautau3->Get("301229/plotEventAlgs/taus/semilep/Mratio");


    int rebinval = 4;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("m^{obs.}_{l#tau_{had}} / m^{MC}_{#tau#tau} distributions");
    h->GetXaxis()->SetTitle("m^{obs.}_{l#tau_{had}} / m^{MC}_{#tau#tau}");
    h->GetYaxis()->SetTitle("Events / 0.04");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    c9->SetLogy();
    c9->SetRightMargin(0.15);
    c9->SetLeftMargin(0.12);
    c9->SetBottomMargin(0.12);
    h->GetYaxis()->SetRangeUser(10,4000);
    h->GetXaxis()->SetLimits(0.,1.2);
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    h3->SetLineColor(4);
    h3->SetLineWidth(2);
    h3->Rebin(rebinval);
    h3->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z'(700)#rightarrowl#tau"));
    hist_leg->AddEntry(h2, Form("H(700)#rightarrow#tau#tau"));
    hist_leg->AddEntry(h3, Form("Z'(3000)#rightarrow#tau#tau"));


    h->Draw("");
    h2->Draw("same");
    h3->Draw("same");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"BSM samples, lh final state");
    //    Tl.DrawLatexNDC(0.15,0.8,Form("Correlation factor %f", h->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c9->SaveAs("plots/20160711/general/M_fraction_dist.pdf");
    }    
  }


  if (phi){
    TCanvas *c8 = new TCanvas("c8","c8",800,600);
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.62,0.62,0.80,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);

    TH1F *h		= (TH1F*)ftautau->Get("302006/plotEventAlgs/taus/PhiStarEta_l_tau");
    TH1F *h1		= (TH1F*)ftautau1->Get("301981/plotEventAlgs/taus/PhiStarEta_l_tau");
    h->Add(h,h1);
    TH1F *h2       	= (TH1F*)ftautau2->Get("342320/plotEventAlgs/taus/semilep/PhiStarEta");
    TH1F *h3       	= (TH1F*)ftautau3->Get("301229/plotEventAlgs/taus/semilep/PhiStarEta");


    int rebinval = 1;
    TGaxis::SetMaxDigits(3); 
    h->SetTitle("#phi^{*}_{#eta} distributions");
    h->GetXaxis()->SetTitle("log_{10}#phi^{*l#tau_{had}}_{#eta}");
    h->GetYaxis()->SetTitle("Events");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    c8->SetLogy();
    c8->SetRightMargin(0.15);
    c8->SetLeftMargin(0.12);
    c8->SetBottomMargin(0.12);
    h->GetYaxis()->SetRangeUser(40,2000);
    h->GetXaxis()->SetLimits(-4.,3.);
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    h3->SetLineColor(4);
    h3->SetLineWidth(2);
    h3->Rebin(rebinval);
    h3->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z'(700)#rightarrowl#tau"));
    hist_leg->AddEntry(h2, Form("H(700)#rightarrow#tau#tau"));
    hist_leg->AddEntry(h3, Form("Z'(3000)#rightarrow#tau#tau"));


    h->Draw("");
    h2->Draw("same");
    h3->Draw("same");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"BSM samples, lh final state");
    //    Tl.DrawLatexNDC(0.15,0.8,Form("Correlation factor %f", h->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c8->SaveAs("plots/20160711/general/Phi_star_dist.pdf");
    }    
  }
  if (phitruth&&1==2){
    TCanvas *c9 = new TCanvas("c9","c9",800,600);
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.62,0.62,0.80,0.82);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);

    TH1F *h		= (TH1F*)ftautau->Get("302006/plotEventAlgs/taus/PhiStarEta_truth_l_tau_no_sin");
    TH1F *h1		= (TH1F*)ftautau1->Get("301981/plotEventAlgs/taus/PhiStarEta_truth_l_tau_no_sin");
    h->Sumw2();
    h1->Sumw2();
    h->Add(h,h1);
    TH1F *h2       	= (TH1F*)ftautau3->Get("301229/plotEventAlgs/taus/PhiStarEta_truth_no_sin");
    h2->Sumw2();

    int rebinval = 1;
    //    TGaxis::SetMaxDigits(3); 
    h->SetTitle("True #phi^{*}_{#eta} distributions");
    h->GetXaxis()->SetTitle("log_{10}#phi^{*truth}_{#eta}");
    h->GetYaxis()->SetTitle("Events");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    //c9->SetLogy();
    c9->SetRightMargin(0.15);
    c9->SetLeftMargin(0.12);
    c9->SetBottomMargin(0.12);
    //    h->GetXaxis()->SetLimits(-4.,3.);
    h->Rebin(rebinval);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(2);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z'(700)#rightarrowl#tau"));
    hist_leg->AddEntry(h2, Form("Z'(3000)#rightarrow#tau#tau"));

    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    h->Fit("gaus");
    h2->Fit("gaus");
    h->GetFunction("gaus")->SetLineWidth(1);
    h2->GetFunction("gaus")->SetLineColor(2);
    h2->GetFunction("gaus")->SetLineWidth(1);
    h->GetXaxis()->SetRangeUser(-5.,1.9);
    //    h->GetYaxis()->SetRangeUser(0.001,1.1);
    h->Sumw2();
    h2->Sumw2();
    TH1* hc = h->GetCumulative();
    TH1* hc2 = h2->GetCumulative();

    //    h->Draw();
    //    h2->Draw("same");
    hc->Draw("");
    hc2->Draw("same");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"BSM samples, lh final state");
    //    Tl.DrawLatexNDC(0.15,0.8,Form("Correlation factor %f", h->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c9->SaveAs("plots/20160711/general/Phi_star_truth_dist_no_sin_sum.pdf");
    }    
  }


  if (phitruth&&1==2){
    TCanvas *c7 = new TCanvas("c7","c7",800,600);
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.02);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.6,0.56,0.80,0.78);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.03);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);

    TH1F *h		= (TH1F*)ftautau->Get("302006/plotEventAlgs/taus/PhiStarEta_truth_l_tau");
    TH1F *h1		= (TH1F*)ftautau1->Get("301981/plotEventAlgs/taus/PhiStarEta_truth_l_tau");
    h->Sumw2();
    h1->Sumw2();
    h->Add(h,h1);
    TH1F *h2       	= (TH1F*)ftautau3->Get("301229/plotEventAlgs/taus/PhiStarEta_truth");
    h2->Sumw2();


    TH1F *h5		= (TH1F*)ftautau->Get("302006/plotEventAlgs/taus/PhiStarEta_truth_l_tau_no_sin");
    TH1F *h4		= (TH1F*)ftautau1->Get("301981/plotEventAlgs/taus/PhiStarEta_truth_l_tau_no_sin");
    h5->Sumw2();
    h4->Sumw2();
    h5->Add(h5,h4);
    TH1F *h6       	= (TH1F*)ftautau3->Get("301229/plotEventAlgs/taus/PhiStarEta_truth_no_sin");
    h6->Sumw2();

    int rebinval = 1;
    //    TGaxis::SetMaxDigits(3); 
    h->SetTitle("True #Phi^{*}_{#eta} and #Phi_{acop} distributions");
    h->GetXaxis()->SetTitle("log_{10}(#phi^{*truth}_{#eta}/tan(#Phi_{acop}/2)))");
    h->GetYaxis()->SetTitle("Events");
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
    h->SetLineWidth(1);
    h->SetLineStyle(1);

    h5->SetLineColor(1);
    h5->SetLineWidth(1);
    h5->Rebin(rebinval);
    h5->SetLineStyle(2);

    h2->SetLineColor(2);
    h2->SetLineWidth(1);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    h6->SetLineColor(2);
    h6->SetLineWidth(1);
    h6->Rebin(rebinval);
    h6->SetLineStyle(2);

    hist_leg->AddEntry(h, Form("Z'(700) #Phi^{*}_{#eta}"));
    hist_leg->AddEntry(h5, Form("Z'(700) tan(#Phi_{acop}/2)"));
    hist_leg->AddEntry(h2, Form("Z'(3000) #Phi^{*}_{#eta}"));
    hist_leg->AddEntry(h6, Form("Z'(3000) tan(#Phi_{acop}/2)"));

    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    h5->Scale(1 / h5->Integral("width"));
    h6->Scale(1 / h6->Integral("width"));
    h->Fit("gaus");
    h2->Fit("gaus");
    h->GetFunction("gaus")->SetLineWidth(1);
    h2->GetFunction("gaus")->SetLineColor(2);
    h2->GetFunction("gaus")->SetLineWidth(1);
    h->GetXaxis()->SetRangeUser(-4.,1.8);
    //    h->GetYaxis()->SetRangeUser(0.001,1.1);
    h->Sumw2();
    h2->Sumw2();
    TH1* hc = h->GetCumulative();
    TH1* hc2 = h2->GetCumulative();
    TH1* hc3 = h5->GetCumulative();
    TH1* hc4 = h6->GetCumulative();

    //    h->Draw();
    //    h2->Draw("same");
    hc->Draw("");
    hc2->Draw("same");
    hc3->Draw("same");
    hc4->Draw("same");

    hist_leg->Draw("same");
    
    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.15,0.85,"BSM samples, lh final state");
    //    Tl.DrawLatexNDC(0.15,0.8,Form("Correlation factor %f", h->GetCorrelationFactor()));

    //save plot
    if (saveplots){
      c7->SaveAs("plots/20160711/general/Phi_star_truth_dist_sum.pdf");
    }    
  }
  if (phitruth) {
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.69,0.47,0.80,0.77);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.04);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);
    TH1F *h		= (TH1F*)ftautau->Get("302006/plotEventAlgs/taus/PhiStarEta_truth_l_tau");
    TH1F *h1		= (TH1F*)ftautau1->Get("301981/plotEventAlgs/taus/PhiStarEta_truth_l_tau");
    h->Sumw2();
    h1->Sumw2();
    h->Add(h,h1);
    TH1F *h2       	= (TH1F*)ftautau2->Get("342320/plotEventAlgs/taus/PhiStarEta_truth");
    h2->Sumw2();


    TH1F *h5		= (TH1F*)ftautau->Get("302006/plotEventAlgs/taus/PhiStarEta_truth_l_tau_no_sin");
    TH1F *h4		= (TH1F*)ftautau1->Get("301981/plotEventAlgs/taus/PhiStarEta_truth_l_tau_no_sin");
    h5->Sumw2();
    h4->Sumw2();
    h5->Add(h5,h4);
    TH1F *h6       	= (TH1F*)ftautau2->Get("342320/plotEventAlgs/taus/PhiStarEta_truth_no_sin");
    h6->Sumw2();

    int rebinval = 1;
    //    TGaxis::SetMaxDigits(3); 
    h->SetTitle("True #Phi^{*}_{#eta} and #Phi_{acop} distributions");
    h->GetXaxis()->SetTitle("log_{10}(#phi^{*truth}_{#eta}/tan(#Phi_{acop}/2)))");
    h->GetYaxis()->SetTitle("Events");
    h->GetXaxis()->SetTitleSize(0.07);
    h->GetYaxis()->SetTitleSize(0.07);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(0.8);
    h->GetYaxis()->SetTitleOffset(0.8);
    //c96->SetLogy();
    //    h->GetXaxis()->SetRangeUser(-4.1,8);
    h->Rebin(rebinval);
    h->SetLineWidth(1);
    h->SetLineStyle(1);

    h5->SetLineColor(1);
    h5->SetLineWidth(1);
    h5->Rebin(rebinval);
    h5->SetLineStyle(2);

    h2->SetLineColor(2);
    h2->SetLineWidth(1);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    h6->SetLineColor(2);
    h6->SetLineWidth(1);
    h6->Rebin(rebinval);
    h6->SetLineStyle(2);

    hist_leg->AddEntry(h, Form("Z'(700) #Phi^{*}_{#eta}"));
    hist_leg->AddEntry(h5, Form("Z'(700) tan(#Phi_{acop}/2)"));
    hist_leg->AddEntry(h2, Form("H(700) #Phi^{*}_{#eta}"));
    hist_leg->AddEntry(h6, Form("H(700) tan(#Phi_{acop}/2)"));

    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    h5->Scale(1 / h5->Integral("width"));
    h6->Scale(1 / h6->Integral("width"));
    TH1* hc = h->GetCumulative();
    TH1* hc2 = h2->GetCumulative();
    TH1* hc3 = h5->GetCumulative();
    TH1* hc4 = h6->GetCumulative();

    
    // Define the Canvas
    TCanvas *c96 = new TCanvas("c96", "c96", 800, 600);

    c96->SetRightMargin(0.15);
    c96->SetLeftMargin(0.14);
    c96->SetBottomMargin(0.14);
    
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    //    pad1->SetLogy();
    pad1->cd();               // pad1 becomes the current pad
    hc->SetStats(0);          // No statistics on upper plot
    hc->GetYaxis()->SetTitle("Events");
 
    hc->GetXaxis()->SetRangeUser(-4.,0.5);
    hc->GetYaxis()->SetRangeUser(-.3,10.5);
    hc->Draw();               // Draw h1
    hc2->Draw("same");
    hc3->Draw("same");
    hc4->Draw("same");
    hist_leg->Draw("same");

    
    // Do not draw the Y axis label on the upper plot and redraw a small
    // axis instead, in order to avoid the first label (0) to be clipped.
    h1->GetYaxis()->SetLabelSize(0.);
    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    //axis->Draw();
    
    // lower plot will be in pad
    c96->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.2);
    pad2->SetGridx(); // vertical grid
    pad2->Draw();
    pad2->cd();       // pad2 becomes the current pad
    
    // Define the ratio plot
    TH1F *h10 = (TH1F*)hc->Clone("h10");
    h10->SetLineColor(kBlack);
    h10->Sumw2();
    h10->SetStats(0);      // No statistics on lower plot
    h10->Divide(hc2);
    h10->SetLineStyle(1);
    h10->SetLineWidth(2);
    //    h10->SetMarkerStyle(21);

    TH1F *h11 = (TH1F*)hc3->Clone("h11");
    h11->SetLineColor(kBlack);
    h11->SetLineStyle(2);
    h11->SetLineWidth(2);
    h11->Sumw2();
    h11->Divide(hc4);
    
    //    h11->SetMarkerStyle(21);

    h10->GetXaxis()->SetTitle("log_{10}x");
    h10->GetYaxis()->SetTitle("Ratio Z'(700)/H(700)");
    h10->GetXaxis()->SetTitleSize(0.07);
    h10->GetYaxis()->SetTitleSize(0.07);
    h10->GetXaxis()->SetTitleOffset(0.8);
    h10->GetYaxis()->SetTitleOffset(0.8);
    TLine *line = new TLine(-6.,1.,2.,1);
    h10->Draw("");
    h11->Draw("same");// Draw the ratio plot
    line->SetLineStyle(2);
    //    line->Draw("same");
    
    
    // Ratio plot (h10) settings
    h10->SetTitle(""); // Remove the ratio title
    
    // Y axis ratio plot settings
    //    h10->GetYaxis()->SetTitle("logx_{2} / logx_{3}");
    h10->GetYaxis()->SetNdivisions(505);
    h10->GetYaxis()->SetTitleSize(0.08);
    // h10->GetYaxis()->SetTitleFont(43);
    // h10->GetYaxis()->SetTitleOffset(1.55);
    h10->GetYaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetYaxis()->SetLabelSize(0.1);
    h10->GetYaxis()->SetRangeUser(0.9,3.9);
    h10->GetXaxis()->SetRangeUser(-4.,0.5);
    h10->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h10->GetXaxis()->SetTitleSize(0.12);
    //h10->GetXaxis()->SetTitleFont(43);
    h10->GetXaxis()->SetTitleOffset(.7);
    h10->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetXaxis()->SetLabelSize(0.1);

    pad1->cd();
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.05);
    Tl.DrawLatexNDC(0.15,0.85,"BSM samples, lh final state");

    //save plot
    if (saveplots){
      c96->SaveAs("plots/20160711/general/Phi_star_truth_dist_sum_ratio_ZH.pdf");
    }    
  }



  if (phitruth) {
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.69,0.47,0.80,0.77);
    hist_leg->SetFillColor(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.04);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);
    TH1F *h		= (TH1F*)ftautau->Get("302006/plotEventAlgs/taus/PhiStarEta_truth_l_tau");
    TH1F *h1		= (TH1F*)ftautau1->Get("301981/plotEventAlgs/taus/PhiStarEta_truth_l_tau");
    h->Sumw2();
    h1->Sumw2();
    h->Add(h,h1);
    TH1F *h2       	= (TH1F*)ftautau3->Get("301229/plotEventAlgs/taus/PhiStarEta_truth");
    h2->Sumw2();


    TH1F *h5		= (TH1F*)ftautau->Get("302006/plotEventAlgs/taus/PhiStarEta_truth_l_tau_no_sin");
    TH1F *h4		= (TH1F*)ftautau1->Get("301981/plotEventAlgs/taus/PhiStarEta_truth_l_tau_no_sin");
    h5->Sumw2();
    h4->Sumw2();
    h5->Add(h5,h4);
    TH1F *h6       	= (TH1F*)ftautau3->Get("301229/plotEventAlgs/taus/PhiStarEta_truth_no_sin");
    h6->Sumw2();

    int rebinval = 1;
    //    TGaxis::SetMaxDigits(3); 
    h->SetTitle("True #Phi^{*}_{#eta} and #Phi_{acop} distributions");
    h->GetXaxis()->SetTitle("log_{10}(#phi^{*truth}_{#eta}/tan(#Phi_{acop}/2)))");
    h->GetYaxis()->SetTitle("Events");
    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.03);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(1.15);
    h->GetYaxis()->SetTitleOffset(1.25);
    //c95->SetLogy();
    // c95->SetRightMargin(0.15);
    // c95->SetLeftMargin(0.12);
    // c95->SetBottomMargin(0.12);
    //    h->GetXaxis()->SetRangeUser(-4.1,8);
    h->Rebin(rebinval);
    h->SetLineWidth(1);
    h->SetLineStyle(1);

    h5->SetLineColor(1);
    h5->SetLineWidth(1);
    h5->Rebin(rebinval);
    h5->SetLineStyle(2);

    h2->SetLineColor(2);
    h2->SetLineWidth(1);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    h6->SetLineColor(2);
    h6->SetLineWidth(1);
    h6->Rebin(rebinval);
    h6->SetLineStyle(2);

    hist_leg->AddEntry(h, Form("Z'(700) #Phi^{*}_{#eta}"));
    hist_leg->AddEntry(h5, Form("Z'(700) tan(#Phi_{acop}/2)"));
    hist_leg->AddEntry(h2, Form("Z'(3000) #Phi^{*}_{#eta}"));
    hist_leg->AddEntry(h6, Form("Z'(3000) tan(#Phi_{acop}/2)"));

    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    h5->Scale(1 / h5->Integral("width"));
    h6->Scale(1 / h6->Integral("width"));
    TH1* hc = h->GetCumulative();
    TH1* hc2 = h2->GetCumulative();
    TH1* hc3 = h5->GetCumulative();
    TH1* hc4 = h6->GetCumulative();

    
    // Define the Canvas
    TCanvas *c95 = new TCanvas("c95", "c95", 800, 600);

    c95->SetRightMargin(0.15);
    c95->SetLeftMargin(0.14);
    c95->SetBottomMargin(0.14);
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    //    pad1->SetLogy();
    pad1->cd();               // pad1 becomes the current pad
    hc->SetStats(0);          // No statistics on upper plot
    hc->GetYaxis()->SetTitle("Events");
 
    hc->GetXaxis()->SetRangeUser(-4.,0.5);
    hc->GetYaxis()->SetRangeUser(-.3,10.5);
    hc->Draw();               // Draw h1
    hc2->Draw("same");
    hc3->Draw("same");
    hc4->Draw("same");
    hist_leg->Draw("same");

    
    // Do not draw the Y axis label on the upper plot and redraw a small
    // axis instead, in order to avoid the first label (0) to be clipped.
    h1->GetYaxis()->SetLabelSize(0.);
    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    //axis->Draw();
    
    // lower plot will be in pad
    c95->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.2);
    pad2->SetGridx(); // vertical grid
    pad2->Draw();
    pad2->cd();       // pad2 becomes the current pad
    
    // Define the ratio plot
    TH1F *h10 = (TH1F*)hc2->Clone("h10");
    h10->SetLineColor(kBlack);
    h10->Sumw2();
    h10->SetStats(0);      // No statistics on lower plot
    h10->Divide(hc);
    h10->SetLineStyle(1);
    h10->SetLineWidth(2);
    //    h10->SetMarkerStyle(21);

    TH1F *h11 = (TH1F*)hc4->Clone("h11");
    h11->SetLineColor(kBlack);
    h11->SetLineStyle(2);
    h11->SetLineWidth(2);
    h11->Sumw2();
    h11->Divide(hc3);
    
    //    h11->SetMarkerStyle(21);

    h10->GetXaxis()->SetTitle("log_{10}x");
    h10->GetYaxis()->SetTitle("Ratio Z'(3000)/Z'(700)");
    h10->GetXaxis()->SetTitleOffset(0.8);
    h10->GetYaxis()->SetTitleOffset(0.5);
    h10->GetXaxis()->SetTitleSize(0.2);
    h10->GetYaxis()->SetTitleSize(0.2);
    TLine *line = new TLine(-6.,1.,2.,1);
    h10->Draw("");
    h11->Draw("same");// Draw the ratio plot
    line->SetLineStyle(2);
    //    line->Draw("same");
    
    
    // Ratio plot (h10) settings
    h10->SetTitle(""); // Remove the ratio title
    
    // Y axis ratio plot settings
    //    h10->GetYaxis()->SetTitle("logx_{2} / logx_{3}");
    h10->GetYaxis()->SetNdivisions(505);
    h10->GetYaxis()->SetTitleSize(0.08);
    // h10->GetYaxis()->SetTitleFont(43);
    // h10->GetYaxis()->SetTitleOffset(1.55);
    h10->GetYaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetYaxis()->SetLabelSize(0.1);
    //    h10->GetYaxis()->SetRangeUser(0.5,1.5);
    h10->GetXaxis()->SetRangeUser(-4.,0.5);
    h10->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h10->GetXaxis()->SetTitleSize(0.12);
    //h10->GetXaxis()->SetTitleFont(43);
    h10->GetXaxis()->SetTitleOffset(.7);
    h10->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetXaxis()->SetLabelSize(0.1);

    pad1->cd();
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.15,0.85,"BSM samples, lh final state");

    //save plot
    if (saveplots){
      c95->SaveAs("plots/20160711/general/Phi_star_truth_dist_sum_ratio_Z.pdf");
    }    
  }


}

