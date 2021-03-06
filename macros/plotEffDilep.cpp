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

using namespace std;

void plotEffDilep(){

  TFile *f = new TFile("Ztautau1M.root");
  TFile *f1 = new TFile("ttbar1M.root");

  bool saveplots = true;
  bool phistar   = true;
  bool phiacop   = true;
  bool eff	 = true;
  bool pt	 = true;
  bool eta	 = true;
  bool sintheta  = true;

  //TH2F *e	 = new TH2F("e","e",60,0,1,60,0,1);
  TGraph *dt	 = new TGraph();
  TGraph *dt2	 = new TGraph();
  TGraph *dt3    = new TGraph();
  TGraph *dt4    = new TGraph();
  TGraph *dt5    = new TGraph();
  dt->SetName("dt");
  dt2->SetName("dt2");
  dt3->SetName("dt3");
  dt4->SetName("dt4");
  dt5->SetName("dt5");

  if( sintheta ){
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.7,0.47,0.8,0.77);
    hist_leg->SetFillColor(0);
    hist_leg->SetFillStyle(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.04);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Sin_theta_dilep");
    TH1F *h2       	= (TH1F*)f1->Get("410000/plotEventAlgs/taus/Sin_theta_dilep");
    int rebinval = 10;
    //    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reconstructed sin(#theta^{*}_{#eta}) distributions");
    h->GetXaxis()->SetTitle("log_{10}sin#theta^{*}_{#eta}");
    h->GetYaxis()->SetTitle("Normalised cumulative events");
    h->GetXaxis()->SetTitleSize(0.07);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(0.8);
    h->GetYaxis()->SetTitleOffset(0.8);
    //c101->SetLogy();
    //    h->GetXaxis()->SetRangeUser(-4.1,8);
    h->Rebin(rebinval);
    h->SetLineWidth(1);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(1);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{l}"));
    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow#tau_{l}#tau_{l}"));

    h->Scale(.01 / h->Integral("width"));
    h2->Scale(.01 / h2->Integral("width"));
    TH1* hc = h->GetCumulative();
    TH1* hc2 = h2->GetCumulative();

    // Define the Canvas
    TCanvas *c101 = new TCanvas("c101", "c101", 800, 600);

    c101->SetRightMargin(0.15);
    c101->SetLeftMargin(0.14);
    c101->SetBottomMargin(0.16);
    
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    //    pad1->SetLogy();
    pad1->cd();               // pad1 becomes the current pad
    hc->SetStats(0);          // No statistics on upper plot
    hc->GetYaxis()->SetTitle("Normalised cumulative events");
    hc->GetYaxis()->SetTitleSize(0.04);
 
    hc->GetXaxis()->SetRangeUser(-.6,0.);
    hc->GetYaxis()->SetRangeUser(-.05,1.05);
    hc->Draw();               // Draw h1
    hc2->Draw("same");
    hist_leg->Draw("same");

    // now, fill the efficiency histogram
    int nbinsx = hc->GetXaxis()->GetNbins();
    cout << nbinsx << endl;
    // loop over bins 31 to 90
    for( int i = 640; i < 701; ++i ){
      dt5->SetPoint( i, hc->GetBinContent(i), hc2->GetBinContent(i));
    }

    // Do not draw the Y axis label on the upper plot and redraw a small
    // axis instead, in order to avoid the first label (0) to be clipped.
    //h->GetYaxis()->SetLabelSize(0.);
    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    //axis->Draw();
    
    // lower plot will be in pad
    c101->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.3);
    pad2->SetGridx(); // vertical grid
    pad2->Draw();
    pad2->cd();       // pad2 becomes the current pad
    
    // Define the ratio plot
    TH1F *h10 = (TH1F*)hc->Clone("h10");
    h10->SetLineColor(kBlack);
    h10->SetStats(0);      // No statistics on lower plot
    h10->Divide(hc2);
    h10->SetLineStyle(1);
    h10->SetLineWidth(1);
    //    h10->SetMarkerStyle(21);

    h10->GetXaxis()->SetTitle("log_{10}sin(#theta^{*}_{#eta})");
    h10->GetYaxis()->SetTitle("Ratio Z/t#bar{t}");
    h10->GetXaxis()->SetTitleSize(0.07);
    h10->GetYaxis()->SetTitleSize(0.07);
    h10->GetXaxis()->SetTitleOffset(.2);
    h10->GetYaxis()->SetTitleOffset(0.6);
    TLine *line = new TLine(-6.,1.,2.,1);
    h10->Draw("");
    line->SetLineStyle(2);
    //    line->Draw("same");
    
    
    // Ratio plot (h10) settings
    h10->SetTitle(""); // Remove the ratio title
    
    // Y axis ratio plot settings
    //    h10->GetYaxis()->SetTitle("logx_{2} / logx_{3}");
    h10->GetYaxis()->SetNdivisions(505);
    h10->GetYaxis()->SetTitleSize(0.08);
    // h10->GetYaxis()->SetTitleFont(43);
    h10->GetYaxis()->SetTitleOffset(0.45);
    h10->GetYaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetYaxis()->SetLabelSize(0.1);
    h10->GetYaxis()->SetRangeUser(-.1,1.2);
    h10->GetXaxis()->SetRangeUser(-.6,0.);
    h10->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h10->GetXaxis()->SetTitleSize(0.12);
    //h10->GetXaxis()->SetTitleFont(43);
    h10->GetXaxis()->SetTitleOffset(.9);
    h10->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetXaxis()->SetLabelSize(0.1);

    TLine *line1 = new TLine(-.6,1.,0.,1.);
    line1->SetLineStyle(2);
    line1->Draw("same");

    pad1->cd();
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.05);
    Tl.DrawLatexNDC(0.15,0.85,"ll final state");

    //save plot
    if (saveplots){
      c101->SaveAs("plots/20161011/dilep/Sin_theta_ll_cumulative_Ztautau_vs_ttbar.pdf");
    }    
  }


  if( eta ){
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.7,0.47,0.8,0.77);
    hist_leg->SetFillColor(0);
    hist_leg->SetFillStyle(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.04);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Eta_dilep");
    TH1F *h2       	= (TH1F*)f1->Get("410000/plotEventAlgs/taus/Eta_dilep");
    int rebinval = 1;
    //    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reconstructed #eta distributions");
    h->GetXaxis()->SetTitle("log_{10}#phi^{*}_{#eta}");
    h->GetYaxis()->SetTitle("Normalised cumulative events");
    h->GetXaxis()->SetTitleSize(0.07);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(0.8);
    h->GetYaxis()->SetTitleOffset(0.8);
    //c99->SetLogy();
    //    h->GetXaxis()->SetRangeUser(-4.1,8);
    h->Rebin(rebinval);
    h->SetLineWidth(1);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(1);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{l}"));
    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow#tau_{l}#tau_{l}"));

    h->Scale(.1 / h->Integral("width"));
    h2->Scale(.1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative();
    TH1* hc2 = h2->GetCumulative();

    // Define the Canvas
    TCanvas *c99 = new TCanvas("c99", "c99", 800, 600);

    c99->SetRightMargin(0.15);
    c99->SetLeftMargin(0.14);
    c99->SetBottomMargin(0.16);
    
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    //    pad1->SetLogy();
    pad1->cd();               // pad1 becomes the current pad
    hc->SetStats(0);          // No statistics on upper plot
    hc->GetYaxis()->SetTitle("Normalised cumulative events");
    hc->GetYaxis()->SetTitleSize(0.04);
 
    hc->GetXaxis()->SetRangeUser(-5.,5.);
    hc->GetYaxis()->SetRangeUser(-.05,1.05);
    hc->Draw();               // Draw h1
    hc2->Draw("same");
    hist_leg->Draw("same");

    // now, fill the efficiency histogram
    int nbinsx = hc->GetXaxis()->GetNbins();
    cout << nbinsx << endl;
    // loop over bins 31 to 90
    for( int i = 0; i < 101; ++i ){
      dt4->SetPoint( i, hc->GetBinContent(i), hc2->GetBinContent(i));
    }

    // Do not draw the Y axis label on the upper plot and redraw a small
    // axis instead, in order to avoid the first label (0) to be clipped.
    //h->GetYaxis()->SetLabelSize(0.);
    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    //axis->Draw();
    
    // lower plot will be in pad
    c99->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.3);
    pad2->SetGridx(); // vertical grid
    pad2->Draw();
    pad2->cd();       // pad2 becomes the current pad
    
    // Define the ratio plot
    TH1F *h10 = (TH1F*)hc->Clone("h10");
    h10->SetLineColor(kBlack);
    h10->SetStats(0);      // No statistics on lower plot
    h10->Divide(hc2);
    h10->SetLineStyle(1);
    h10->SetLineWidth(1);
    //    h10->SetMarkerStyle(21);

    h10->GetXaxis()->SetTitle("#eta");
    h10->GetYaxis()->SetTitle("Ratio Z/t#bar{t}");
    h10->GetXaxis()->SetTitleSize(0.07);
    h10->GetYaxis()->SetTitleSize(0.07);
    h10->GetXaxis()->SetTitleOffset(.2);
    h10->GetYaxis()->SetTitleOffset(0.6);
    TLine *line = new TLine(-6.,1.,2.,1);
    h10->Draw("");
    line->SetLineStyle(2);
    //    line->Draw("same");
    
    
    // Ratio plot (h10) settings
    h10->SetTitle(""); // Remove the ratio title
    
    // Y axis ratio plot settings
    //    h10->GetYaxis()->SetTitle("logx_{2} / logx_{3}");
    h10->GetYaxis()->SetNdivisions(505);
    h10->GetYaxis()->SetTitleSize(0.08);
    // h10->GetYaxis()->SetTitleFont(43);
    h10->GetYaxis()->SetTitleOffset(0.45);
    h10->GetYaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetYaxis()->SetLabelSize(0.1);
    h10->GetYaxis()->SetRangeUser(0.5,2.8);
    h10->GetXaxis()->SetRangeUser(-5.,5.);
    h10->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h10->GetXaxis()->SetTitleSize(0.12);
    //h10->GetXaxis()->SetTitleFont(43);
    h10->GetXaxis()->SetTitleOffset(.9);
    h10->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetXaxis()->SetLabelSize(0.1);

    TLine *line1 = new TLine(-5.,1.,5.,1.);
    line1->SetLineStyle(2);
    line1->Draw("same");

    pad1->cd();
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.05);
    Tl.DrawLatexNDC(0.15,0.85,"ll final state");

    //save plot
    if (saveplots){
      c99->SaveAs("plots/20161011/dilep/Eta_ll_cumulative_Ztautau_vs_ttbar.pdf");
    }    
  }

  if( pt ){
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.7,0.47,0.8,0.77);
    hist_leg->SetFillColor(0);
    hist_leg->SetFillStyle(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.04);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Pt_dilep");
    TH1F *h2       	= (TH1F*)f1->Get("410000/plotEventAlgs/taus/Pt_dilep");
    int rebinval = 1;
    //    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reconstructed p_{T} distributions");
    h->GetXaxis()->SetTitle("log_{10}#phi^{*}_{#eta}");
    h->GetYaxis()->SetTitle("Normalised cumulative events");
    h->GetXaxis()->SetTitleSize(0.07);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(0.8);
    h->GetYaxis()->SetTitleOffset(0.8);
    //c98->SetLogy();
    //    h->GetXaxis()->SetRangeUser(-4.1,8);
    h->Rebin(rebinval);
    h->SetLineWidth(1);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(1);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{l}"));
    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow#tau_{l}#tau_{l}"));

    h->Scale(1000. / h->Integral("width"));
    h2->Scale(1000. / h2->Integral("width"));
    TH1* hc = h->GetCumulative();
    TH1* hc2 = h2->GetCumulative();

    // Define the Canvas
    TCanvas *c98 = new TCanvas("c98", "c98", 800, 600);

    c98->SetRightMargin(0.15);
    c98->SetLeftMargin(0.14);
    c98->SetBottomMargin(0.16);
    
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    //    pad1->SetLogy();
    pad1->cd();               // pad1 becomes the current pad
    hc->SetStats(0);          // No statistics on upper plot
    hc->GetYaxis()->SetTitle("Normalised cumulative events");
    hc->GetYaxis()->SetTitleSize(0.04);
 
    hc->GetXaxis()->SetRangeUser(0,150e3);
    hc->GetYaxis()->SetRangeUser(-.05,1.05);
    hc->Draw();               // Draw h1
    hc2->Draw("same");
    hist_leg->Draw("same");

    // now, fill the efficiency histogram
    int nbinsx = hc->GetXaxis()->GetNbins();
    cout << nbinsx << endl;
    // loop over bins 31 to 90
    for( int i = 21; i < 201; ++i ){
      dt3->SetPoint( i, hc->GetBinContent(i), hc2->GetBinContent(i));
    }

    // Do not draw the Y axis label on the upper plot and redraw a small
    // axis instead, in order to avoid the first label (0) to be clipped.
    //h->GetYaxis()->SetLabelSize(0.);
    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    //axis->Draw();
    
    // lower plot will be in pad
    c98->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.3);
    pad2->SetGridx(); // vertical grid
    pad2->Draw();
    pad2->cd();       // pad2 becomes the current pad
    
    // Define the ratio plot
    TH1F *h10 = (TH1F*)hc->Clone("h10");
    h10->SetLineColor(kBlack);
    h10->SetStats(0);      // No statistics on lower plot
    h10->Divide(hc2);
    h10->SetLineStyle(1);
    h10->SetLineWidth(1);
    //    h10->SetMarkerStyle(21);

    h10->GetXaxis()->SetTitle("p_{T}");
    h10->GetYaxis()->SetTitle("Ratio Z/t#bar{t}");
    h10->GetXaxis()->SetTitleSize(0.07);
    h10->GetYaxis()->SetTitleSize(0.07);
    h10->GetXaxis()->SetTitleOffset(.2);
    h10->GetYaxis()->SetTitleOffset(0.6);
    TLine *line = new TLine(-6.,1.,2.,1);
    h10->Draw("");
    line->SetLineStyle(2);
    //    line->Draw("same");
    
    
    // Ratio plot (h10) settings
    h10->SetTitle(""); // Remove the ratio title
    
    // Y axis ratio plot settings
    //    h10->GetYaxis()->SetTitle("logx_{2} / logx_{3}");
    h10->GetYaxis()->SetNdivisions(505);
    h10->GetYaxis()->SetTitleSize(0.08);
    // h10->GetYaxis()->SetTitleFont(43);
    h10->GetYaxis()->SetTitleOffset(0.45);
    h10->GetYaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetYaxis()->SetLabelSize(0.1);
    h10->GetYaxis()->SetRangeUser(0.5,1.9);
    h10->GetXaxis()->SetRangeUser(0.,150e3);
    h10->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h10->GetXaxis()->SetTitleSize(0.12);
    //h10->GetXaxis()->SetTitleFont(43);
    h10->GetXaxis()->SetTitleOffset(.9);
    h10->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetXaxis()->SetLabelSize(0.1);

    TLine *line1 = new TLine(0,1,150e3,1);
    line1->SetLineStyle(2);
    line1->Draw("same");

    pad1->cd();
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.05);
    Tl.DrawLatexNDC(0.15,0.85,"ll final state");

    //save plot
    if (saveplots){
      c98->SaveAs("plots/20161011/dilep/Pt_ll_cumulative_Ztautau_vs_ttbar_h.pdf");
    }    
  }

  if( phistar ){
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.7,0.47,0.8,0.77);
    hist_leg->SetFillColor(0);
    hist_leg->SetFillStyle(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.04);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/PhiStarEta_dilep");
    TH1F *h2       	= (TH1F*)f1->Get("410000/plotEventAlgs/taus/PhiStarEta_dilep");
    int rebinval = 1;
    //    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reconstructed #Phi^{*}_{#eta} distributions");
    h->GetXaxis()->SetTitle("log_{10}#phi^{*}_{#eta}");
    h->GetYaxis()->SetTitle("Normalised cumulative events");
    h->GetXaxis()->SetTitleSize(0.07);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(0.8);
    h->GetYaxis()->SetTitleOffset(0.8);
    //c96->SetLogy();
    //    h->GetXaxis()->SetRangeUser(-4.1,8);
    h->Rebin(rebinval);
    h->SetLineWidth(1);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(1);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{l}"));
    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow#tau_{l}#tau_{l}"));

    h->Scale(.1 / h->Integral("width"));
    h2->Scale(.1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative();
    TH1* hc2 = h2->GetCumulative();

    // Define the Canvas
    TCanvas *c96 = new TCanvas("c96", "c96", 800, 600);

    c96->SetRightMargin(0.15);
    c96->SetLeftMargin(0.14);
    c96->SetBottomMargin(0.16);
    
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    //    pad1->SetLogy();
    pad1->cd();               // pad1 becomes the current pad
    hc->SetStats(0);          // No statistics on upper plot
    hc->GetYaxis()->SetTitle("Normalised cumulative events");
    hc->GetYaxis()->SetTitleSize(0.04);
 
    hc->GetXaxis()->SetRangeUser(-4.,2.);
    hc->GetYaxis()->SetRangeUser(-.05,1.05);
    hc->Draw();               // Draw h1
    hc2->Draw("same");
    hist_leg->Draw("same");

    // now, fill the efficiency histogram
    int nbinsx = hc->GetXaxis()->GetNbins();
    cout << nbinsx << endl;
    // loop over bins 31 to 90
    for( int i = 31; i < 91; ++i ){
      //e->Fill(hc->GetBinContent(i), hc2->GetBinContent(i));
      dt->SetPoint( i, hc->GetBinContent(i), hc2->GetBinContent(i));
    }

    // Do not draw the Y axis label on the upper plot and redraw a small
    // axis instead, in order to avoid the first label (0) to be clipped.
    //h->GetYaxis()->SetLabelSize(0.);
    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    //axis->Draw();
    
    // lower plot will be in pad
    c96->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.3);
    pad2->SetGridx(); // vertical grid
    pad2->Draw();
    pad2->cd();       // pad2 becomes the current pad
    
    // Define the ratio plot
    TH1F *h10 = (TH1F*)hc->Clone("h10");
    h10->SetLineColor(kBlack);
    h10->SetStats(0);      // No statistics on lower plot
    h10->Divide(hc2);
    h10->SetLineStyle(1);
    h10->SetLineWidth(1);
    //    h10->SetMarkerStyle(21);

    h10->GetXaxis()->SetTitle("log_{10}#Phi^{*}_{#eta}");
    h10->GetYaxis()->SetTitle("Ratio Z/t#bar{t}");
    h10->GetXaxis()->SetTitleSize(0.07);
    h10->GetYaxis()->SetTitleSize(0.07);
    h10->GetXaxis()->SetTitleOffset(.2);
    h10->GetYaxis()->SetTitleOffset(0.6);
    TLine *line = new TLine(-6.,1.,2.,1);
    h10->Draw("");
    line->SetLineStyle(2);
    //    line->Draw("same");
    
    
    // Ratio plot (h10) settings
    h10->SetTitle(""); // Remove the ratio title
    
    // Y axis ratio plot settings
    //    h10->GetYaxis()->SetTitle("logx_{2} / logx_{3}");
    h10->GetYaxis()->SetNdivisions(505);
    h10->GetYaxis()->SetTitleSize(0.08);
    // h10->GetYaxis()->SetTitleFont(43);
    h10->GetYaxis()->SetTitleOffset(0.45);
    h10->GetYaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetYaxis()->SetLabelSize(0.1);
    h10->GetYaxis()->SetRangeUser(0,10.5);
    h10->GetXaxis()->SetRangeUser(-4.,2.);
    h10->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h10->GetXaxis()->SetTitleSize(0.12);
    //h10->GetXaxis()->SetTitleFont(43);
    h10->GetXaxis()->SetTitleOffset(.9);
    h10->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetXaxis()->SetLabelSize(0.1);

    pad1->cd();
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.05);
    Tl.DrawLatexNDC(0.15,0.85,"ll final state");

    //save plot
    if (saveplots){
      c96->SaveAs("plots/20161011/dilep/PhiStar_ll_cumulative_Ztautau_vs_ttbar.pdf");
    }    
  }

  if( phiacop ){
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.5,0.47,0.6,0.77);
    hist_leg->SetFillColor(0);
    hist_leg->SetFillStyle(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.04);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Phi_acop_dilep");
    TH1F *h2       	= (TH1F*)f1->Get("410000/plotEventAlgs/taus/Phi_acop_dilep");
    int rebinval = 10;
    //    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reconstructed #Phi_{acop} distributions");
    h->GetXaxis()->SetTitle("log_{10}#phi^_{acop}");
    h->GetYaxis()->SetTitle("Normalised cumulative events");
    h->GetXaxis()->SetTitleSize(0.07);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(0.8);
    h->GetYaxis()->SetTitleOffset(0.8);
    //c97->SetLogy();
    //    h->GetXaxis()->SetRangeUser(-4.1,8);
    h->Rebin(rebinval);
    h->SetLineWidth(1);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(1);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{l}"));
    hist_leg->AddEntry(h2, Form("t#bar{t}#rightarrow#tau_{l}#tau_{l}"));

    h->Scale(.1 / h->Integral("width"));
    h2->Scale(.1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative();
    TH1* hc2 = h2->GetCumulative();

    // Define the Canvas
    TCanvas *c97 = new TCanvas("c97", "c97", 800, 600);

    c97->SetRightMargin(0.15);
    c97->SetLeftMargin(0.14);
    c97->SetBottomMargin(0.16);
    
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    //    pad1->SetLogy();
    pad1->cd();               // pad1 becomes the current pad
    hc->SetStats(0);          // No statistics on upper plot
    hc->GetYaxis()->SetTitle("Normalised cumulative events");
    hc->GetYaxis()->SetTitleSize(0.04);
 
    hc->GetXaxis()->SetRangeUser(-4.,.55);
    hc->GetYaxis()->SetRangeUser(-.05,1.05);
    hc->Draw();               // Draw h1
    hc2->Draw("same");
    hist_leg->Draw("same");

    // now, fill the efficiency histogram
    int nbinsx = hc->GetXaxis()->GetNbins();
    cout << nbinsx << endl;
    // loop over bins 31 to 90
    for( int i = 31; i < 76; ++i ){
      //e->Fill(hc->GetBinContent(i), hc2->GetBinContent(i));
      dt2->SetPoint( i, hc->GetBinContent(i), hc2->GetBinContent(i));
    }

    // Do not draw the Y axis label on the upper plot and redraw a small
    // axis instead, in order to avoid the first label (0) to be clipped.
    //h->GetYaxis()->SetLabelSize(0.);
    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    //axis->Draw();
    
    // lower plot will be in pad
    c97->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.3);
    pad2->SetGridx(); // vertical grid
    pad2->Draw();
    pad2->cd();       // pad2 becomes the current pad
    
    // Define the ratio plot
    TH1F *h10 = (TH1F*)hc->Clone("h10");
    h10->SetLineColor(kBlack);
    h10->SetStats(0);      // No statistics on lower plot
    h10->Divide(hc2);
    h10->SetLineStyle(1);
    h10->SetLineWidth(1);
    //    h10->SetMarkerStyle(21);

    h10->GetXaxis()->SetTitle("log_{10}#Phi_{acop}");
    h10->GetYaxis()->SetTitle("Ratio Z/t#bar{t}");
    h10->GetXaxis()->SetTitleSize(0.07);
    h10->GetYaxis()->SetTitleSize(0.07);
    h10->GetXaxis()->SetTitleOffset(.2);
    h10->GetYaxis()->SetTitleOffset(0.6);
    TLine *line = new TLine(-6.,1.,2.,1);
    h10->Draw("");
    line->SetLineStyle(2);
    //    line->Draw("same");
    
    
    // Ratio plot (h10) settings
    h10->SetTitle(""); // Remove the ratio title
    
    // Y axis ratio plot settings
    //    h10->GetYaxis()->SetTitle("logx_{2} / logx_{3}");
    h10->GetYaxis()->SetNdivisions(505);
    h10->GetYaxis()->SetTitleSize(0.08);
    // h10->GetYaxis()->SetTitleFont(43);
    h10->GetYaxis()->SetTitleOffset(0.45);
    h10->GetYaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetYaxis()->SetLabelSize(0.1);
    h10->GetYaxis()->SetRangeUser(0,10.5);
    h10->GetXaxis()->SetRangeUser(-4.,.55);
    h10->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h10->GetXaxis()->SetTitleSize(0.12);
    //h10->GetXaxis()->SetTitleFont(43);
    h10->GetXaxis()->SetTitleOffset(.9);
    h10->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetXaxis()->SetLabelSize(0.1);

    pad1->cd();
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.05);
    Tl.DrawLatexNDC(0.15,0.85,"ll final state");

    //save plot
    if (saveplots){
      c97->SaveAs("plots/20161011/dilep/PhiAcop_ll_cumulative_Ztautau_vs_ttbar.pdf");
    }    
  }

  if(  eff ){
    TCanvas *c20 = new TCanvas("c20","c20",800,600);
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat("e"); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg1 = new TLegend(0.86,0.68,1,0.9);
    hist_leg1->SetFillColor(0);
    hist_leg1->SetLineColor(1);
    hist_leg1->SetShadowColor(0);
    hist_leg1->SetTextSize(0.03);
    hist_leg1->SetTextFont(62);
    hist_leg1->SetFillStyle(0);
    hist_leg1->SetBorderSize(0);
    c20->SetRightMargin(0.15);
    c20->SetLeftMargin(0.14);
    c20->SetBottomMargin(0.14);

    dt->SetTitle("Background vs signal efficiency");
    dt->GetXaxis()->SetTitle("#epsilon_{sig}");
    dt->GetYaxis()->SetTitle("#epsilon_{bkg}");
    dt->GetXaxis()->SetTitleSize(0.06);
    dt->GetYaxis()->SetTitleSize(0.06);
    dt->GetXaxis()->SetTitleOffset(0.8);
    dt->GetYaxis()->SetTitleOffset(0.8);
    dt->GetXaxis()->SetRangeUser(0,1.05);
    dt->GetYaxis()->SetRangeUser(0,1.05);
    dt->SetLineWidth(2);
    dt2->SetLineWidth(2);
    dt3->SetLineWidth(2);
    dt4->SetLineWidth(2);
    dt5->SetLineWidth(2);
    dt->SetLineColor(1);
    dt2->SetLineColor(2);
    dt3->SetLineColor(6);
    dt4->SetLineColor(7);
    dt5->SetLineColor(8);
    hist_leg1->AddEntry(dt, Form("#phi^{*}_{#eta}"), "l");
    hist_leg1->AddEntry(dt2, Form("#phi_{acop}"),"l");
    hist_leg1->AddEntry(dt3, Form("p^{l}_{T}"),"l");
    hist_leg1->AddEntry(dt4, Form("#eta^{l}"),"l");
    hist_leg1->AddEntry(dt5, Form("sin#theta^{*}_{#eta}"),"l");
    dt->Draw();
    dt2->Draw("same");
    dt3->Draw("same");
    dt4->Draw("same");
    dt5->Draw("same");
    hist_leg1->Draw("same");

    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.2,0.82,"Z#rightarrow#tau_{l}#tau_{l} (sig) vs t#bar{t}#rightarrow#tau_{l}#tau_{l} (bkg)");

    //save plot
    if (saveplots){
      c20->SaveAs("plots/20161011/dilep/Eff_ll_Ztautau_vs_ttbar.pdf");
    }    

  }

}

