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

void plotEff_MET_semilep_reco(){

  TFile *f = new TFile("Ztautau1M.root");
  TFile *f1 = new TFile("Wplusmunu1M.root");

  bool saveplots = true;
  bool MET       = true;
  bool eff	 = true;

  //TH2F *e	 = new TH2F("e","e",60,0,1,60,0,1);
  TGraph *dt	 = new TGraph();
  TGraph *dt2	 = new TGraph();
  TGraph *dt3	 = new TGraph();
  TGraph *dt4	 = new TGraph();
  TGraph *dt5	 = new TGraph();
  TGraph *dt6	 = new TGraph();
  TGraph *dt7	 = new TGraph();
  TGraph *dt8	 = new TGraph();
  TGraph *dt9	 = new TGraph();
  TGraph *dt10	 = new TGraph();
  TGraph *dt11	 = new TGraph();
  TGraph *dt12	 = new TGraph();
  dt->SetName("dt");
  dt2->SetName("dt2");
  dt3->SetName("dt3");
  dt4->SetName("dt4");
  dt5->SetName("dt5");
  dt6->SetName("dt6");
  dt7->SetName("dt7");
  dt8->SetName("dt8");
  dt9->SetName("dt9");
  dt10->SetName("dt10");
  dt11->SetName("dt11");
  dt12->SetName("dt12");

  //utility line
  TGraph *util	= new TGraph();
  util->SetName("util");
  for( int i = 1; i < 101; ++i ){
    util->SetPoint( i, i/100, i/100);
  }
  

  if( MET ){
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.65,0.37,0.75,0.67);
    hist_leg->SetFillColor(0);
    hist_leg->SetFillStyle(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.04);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/CTdotMET");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/CTdotMET");
    int rebinval = 1;
    //    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco c^{l}_{T}#upoint#slash{E}_{T} distributions");
    h->GetXaxis()->SetTitle("c^{l}_{T}#upoint#slash{E}_{T} [GeV^{2}]");
    h->GetYaxis()->SetTitle("Normalised cumulative events");
    h->GetXaxis()->SetTitleSize(0.07);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(0.8);
    h->GetYaxis()->SetTitleOffset(0.8);
    //    h->GetXaxis()->SetRangeUser(-4.1,8);
    h->Rebin(rebinval);
    h->SetLineWidth(1);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(1);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));

    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kFALSE);
    TH1* hc2 = h2->GetCumulative(kFALSE);

    // Define the Canvas
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);

    c1->SetRightMargin(0.15);
    c1->SetLeftMargin(0.14);
    c1->SetBottomMargin(0.16);
    
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    hc->SetStats(0);          // No statistics on upper plot
    hc->GetYaxis()->SetTitle("Normalised cumulative events");
    hc->GetYaxis()->SetTitleSize(0.04);
 
    hc->GetXaxis()->SetRangeUser(-200,200);
    //    hc->GetYaxis()->SetRangeUser(-.05,1.05);
    hc->Draw();               // Draw h1
    hc2->Draw("same");
    hist_leg->Draw("same");

    // now, fill the efficiency histogram
    int nbinsx = hc->GetXaxis()->GetNbins();
    cout << nbinsx << endl;
    // loop over bins 31 to 90
    for( int i = 0; i < nbinsx+1; ++i ){
      dt->SetPoint( i, hc->GetBinContent(i), hc2->GetBinContent(i));
    }

    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    c1->cd();          // Go back to the main canvas before defining pad2
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

    h10->GetXaxis()->SetTitle("c^{l}_{T}#upoint#slash{E}_{T} [GeV^{2}]");
    h10->GetYaxis()->SetTitle("Ratio Z/w+jets");
    h10->GetXaxis()->SetTitleSize(0.07);
    h10->GetYaxis()->SetTitleSize(0.07);
    h10->GetXaxis()->SetTitleOffset(.2);
    h10->GetYaxis()->SetTitleOffset(0.6);
    TLine *line = new TLine(-6.,1.,2.,1);
    h10->Draw("");
    line->SetLineStyle(2);
    //    line->Draw("same");
    
    

    h10->SetTitle(""); // Remove the ratio title
    h10->GetYaxis()->SetNdivisions(505);
    h10->GetYaxis()->SetTitleSize(0.08);
    // h10->GetYaxis()->SetTitleFont(43);
    h10->GetYaxis()->SetTitleOffset(0.45);
    h10->GetYaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetYaxis()->SetLabelSize(0.1);
    //    h10->GetYaxis()->SetRangeUser(.8,14.9);
    h10->GetXaxis()->SetRangeUser(-200,200);
    h10->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h10->GetXaxis()->SetTitleSize(0.12);
    //h10->GetXaxis()->SetTitleFont(43);
    h10->GetXaxis()->SetTitleOffset(.9);
    h10->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetXaxis()->SetLabelSize(0.1);

    TLine *line1 = new TLine(-200,1.,200,1.);
    line1->SetLineStyle(2);
    line1->Draw("same");

    pad1->cd();
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.05);
    Tl.DrawLatexNDC(0.15,0.85,"all #phi^{*}_{#eta}");

    //save plot
    if (saveplots){
      c1->SaveAs("plots/20161101/w+jets/semilep/reco/CTdotMET_all_cumulative_Ztautau_vs_w+jets_reco.pdf");
    }    
  }
  if( MET ){
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.65,0.37,0.75,0.67);
    hist_leg->SetFillColor(0);
    hist_leg->SetFillStyle(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.04);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/MT_MET_l");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/MT_MET_l");
    int rebinval = 1;
    //    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco M_{T}(#slash{E}_{T}l) distributions");
    h->GetXaxis()->SetTitle("M_{T}(#slash{E}_{T}l) [GeV]");
    h->GetYaxis()->SetTitle("Normalised cumulative events");
    h->GetXaxis()->SetTitleSize(0.07);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(0.8);
    h->GetYaxis()->SetTitleOffset(0.8);
    //    h->GetXaxis()->SetRangeUser(-4.1,8);
    h->Rebin(rebinval);
    h->SetLineWidth(1);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(1);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));

    h->Scale(.1 / h->Integral("width"));
    h2->Scale(.1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative();
    TH1* hc2 = h2->GetCumulative();

    // Define the Canvas
    TCanvas *c2 = new TCanvas("c2", "c2", 800, 600);

    c2->SetRightMargin(0.15);
    c2->SetLeftMargin(0.14);
    c2->SetBottomMargin(0.16);
    
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    hc->SetStats(0);          // No statistics on upper plot
    hc->GetYaxis()->SetTitle("Normalised cumulative events");
    hc->GetYaxis()->SetTitleSize(0.04);
 
    //hc->GetXaxis()->SetRangeUser(0.,100.0e3);
    //hc->GetYaxis()->SetRangeUser(-.05,1.05);
    hc->Draw();               // Draw h1
    hc2->Draw("same");
    hist_leg->Draw("same");

    // now, fill the efficiency histogram
    int nbinsx = hc->GetXaxis()->GetNbins();
    cout << nbinsx << endl;
    // loop over bins 31 to 90
    for( int i = 0; i < nbinsx+1; ++i ){
      dt2->SetPoint( i, hc->GetBinContent(i), hc2->GetBinContent(i));
    }

    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    c2->cd();          // Go back to the main canvas before defining pad2
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

    h10->GetXaxis()->SetTitle("M_{T}(#slash{E}_{T}l) [GeV]");
    h10->GetYaxis()->SetTitle("Ratio Z/w+jets");
    h10->GetXaxis()->SetTitleSize(0.07);
    h10->GetYaxis()->SetTitleSize(0.07);
    h10->GetXaxis()->SetTitleOffset(.2);
    h10->GetYaxis()->SetTitleOffset(0.6);
    TLine *line = new TLine(-6.,1.,2.,1);
    h10->Draw("");
    line->SetLineStyle(2);
    //    line->Draw("same");
    
    

    h10->SetTitle(""); // Remove the ratio title
    h10->GetYaxis()->SetNdivisions(505);
    h10->GetYaxis()->SetTitleSize(0.08);
    // h10->GetYaxis()->SetTitleFont(43);
    h10->GetYaxis()->SetTitleOffset(0.45);
    h10->GetYaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetYaxis()->SetLabelSize(0.1);
    //h10->GetYaxis()->SetRangeUser(.8,14.9);
    //h10->GetXaxis()->SetRangeUser(0,100.0e3);
    h10->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h10->GetXaxis()->SetTitleSize(0.12);
    //h10->GetXaxis()->SetTitleFont(43);
    h10->GetXaxis()->SetTitleOffset(.9);
    h10->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetXaxis()->SetLabelSize(0.1);

    TLine *line1 = new TLine(0,1.,200,1.);
    line1->SetLineStyle(2);
    line1->Draw("same");

    pad1->cd();
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.05);
    Tl.DrawLatexNDC(0.15,0.85,"all #phi^{*}_{#eta}");

    //save plot
    if (saveplots){
      c2->SaveAs("plots/20161101/w+jets/semilep/reco/MT_MET_l_all_cumulative_Ztautau_vs_w+jets_reco.pdf");
    }    
  }

  if( MET ){
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.65,0.37,0.75,0.67);
    hist_leg->SetFillColor(0);
    hist_leg->SetFillStyle(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.04);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/CTdotMET_r1");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/CTdotMET_r1");
    int rebinval = 1;
    //    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco c^{l}_{T}#upoint#slash{E}_{T} distributions");
    h->GetXaxis()->SetTitle("c^{l}_{T}#upoint#slash{E}_{T} [GeV^{2}]");
    h->GetYaxis()->SetTitle("Normalised cumulative events");
    h->GetXaxis()->SetTitleSize(0.07);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(0.8);
    h->GetYaxis()->SetTitleOffset(0.8);
    //    h->GetXaxis()->SetRangeUser(-4.1,8);
    h->Rebin(rebinval);
    h->SetLineWidth(1);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(1);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));

    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kFALSE);
    TH1* hc2 = h2->GetCumulative(kFALSE);

    // Define the Canvas
    TCanvas *c3 = new TCanvas("c3", "c3", 800, 600);

    c3->SetRightMargin(0.15);
    c3->SetLeftMargin(0.14);
    c3->SetBottomMargin(0.16);
    
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    hc->SetStats(0);          // No statistics on upper plot
    hc->GetYaxis()->SetTitle("Normalised cumulative events");
    hc->GetYaxis()->SetTitleSize(0.04);
 
    hc->GetXaxis()->SetRangeUser(-200,200);
    //    hc->GetYaxis()->SetRangeUser(-.05,1.05);
    hc->Draw();               // Draw h1
    hc2->Draw("same");
    hist_leg->Draw("same");

    // now, fill the efficiency histogram
    int nbinsx = hc->GetXaxis()->GetNbins();
    cout << nbinsx << endl;
    // loop over bins 31 to 90
    for( int i = 0; i < nbinsx+1; ++i ){
      dt3->SetPoint( i, hc->GetBinContent(i), hc2->GetBinContent(i));
    }

    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    c3->cd();          // Go back to the main canvas before defining pad2
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

    h10->GetXaxis()->SetTitle("c^{l}_{T}#upoint#slash{E}_{T} [GeV^{2}]");
    h10->GetYaxis()->SetTitle("Ratio Z/w+jets");
    h10->GetXaxis()->SetTitleSize(0.07);
    h10->GetYaxis()->SetTitleSize(0.07);
    h10->GetXaxis()->SetTitleOffset(.2);
    h10->GetYaxis()->SetTitleOffset(0.6);
    TLine *line = new TLine(-6.,1.,2.,1);
    h10->Draw("");
    line->SetLineStyle(2);
    //    line->Draw("same");
    
    

    h10->SetTitle(""); // Remove the ratio title
    h10->GetYaxis()->SetNdivisions(505);
    h10->GetYaxis()->SetTitleSize(0.08);
    // h10->GetYaxis()->SetTitleFont(43);
    h10->GetYaxis()->SetTitleOffset(0.45);
    h10->GetYaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetYaxis()->SetLabelSize(0.1);
    //    h10->GetYaxis()->SetRangeUser(.8,14.9);
    h10->GetXaxis()->SetRangeUser(-200,200);
    h10->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h10->GetXaxis()->SetTitleSize(0.12);
    //h10->GetXaxis()->SetTitleFont(43);
    h10->GetXaxis()->SetTitleOffset(.9);
    h10->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetXaxis()->SetLabelSize(0.1);

    TLine *line1 = new TLine(-200,1.,200,1.);
    line1->SetLineStyle(2);
    line1->Draw("same");

    pad1->cd();
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.15,0.85,"#phi^{*}_{#eta}<0.35, #epsilon_{sig}=0.98, #epsilon_{bkg}=0.43");

    //save plot
    if (saveplots){
      c3->SaveAs("plots/20161101/w+jets/semilep/reco/CTdotMET_r1_cumulative_Ztautau_vs_w+jets_reco.pdf");
    }    
  }


  if( MET ){
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.65,0.37,0.75,0.67);
    hist_leg->SetFillColor(0);
    hist_leg->SetFillStyle(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.04);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/MT_MET_l_r1");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/MT_MET_l_r1");
    int rebinval = 1;
    //    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco M_{T}(#slash{E}_{T}l) distributions");
    h->GetXaxis()->SetTitle("M_{T}(#slash{E}_{T}l) [GeV]");
    h->GetYaxis()->SetTitle("Normalised cumulative events");
    h->GetXaxis()->SetTitleSize(0.07);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(0.8);
    h->GetYaxis()->SetTitleOffset(0.8);
    //    h->GetXaxis()->SetRangeUser(-4.1,8);
    h->Rebin(rebinval);
    h->SetLineWidth(1);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(1);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));

    h->Scale(.1 / h->Integral("width"));
    h2->Scale(.1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative();
    TH1* hc2 = h2->GetCumulative();

    // Define the Canvas
    TCanvas *c4 = new TCanvas("c4", "c4", 800, 600);

    c4->SetRightMargin(0.15);
    c4->SetLeftMargin(0.14);
    c4->SetBottomMargin(0.16);
    
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    hc->SetStats(0);          // No statistics on upper plot
    hc->GetYaxis()->SetTitle("Normalised cumulative events");
    hc->GetYaxis()->SetTitleSize(0.04);
 
    //hc->GetXaxis()->SetRangeUser(0.,100.0e3);
    //hc->GetYaxis()->SetRangeUser(-.05,1.05);
    hc->Draw();               // Draw h1
    hc2->Draw("same");
    hist_leg->Draw("same");

    // now, fill the efficiency histogram
    int nbinsx = hc->GetXaxis()->GetNbins();
    cout << nbinsx << endl;
    // loop over bins 31 to 90
    for( int i = 0; i < nbinsx+1; ++i ){
      dt4->SetPoint( i, hc->GetBinContent(i), hc2->GetBinContent(i));
    }

    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    c4->cd();          // Go back to the main canvas before defining pad2
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

    h10->GetXaxis()->SetTitle("M_{T}(#slash{E}_{T}l) [GeV]");
    h10->GetYaxis()->SetTitle("Ratio Z/w+jets");
    h10->GetXaxis()->SetTitleSize(0.07);
    h10->GetYaxis()->SetTitleSize(0.07);
    h10->GetXaxis()->SetTitleOffset(.2);
    h10->GetYaxis()->SetTitleOffset(0.6);
    TLine *line = new TLine(-6.,1.,2.,1);
    h10->Draw("");
    line->SetLineStyle(2);
    //    line->Draw("same");
    
    

    h10->SetTitle(""); // Remove the ratio title
    h10->GetYaxis()->SetNdivisions(505);
    h10->GetYaxis()->SetTitleSize(0.08);
    // h10->GetYaxis()->SetTitleFont(43);
    h10->GetYaxis()->SetTitleOffset(0.45);
    h10->GetYaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetYaxis()->SetLabelSize(0.1);
    //h10->GetYaxis()->SetRangeUser(.8,14.9);
    //h10->GetXaxis()->SetRangeUser(0,100.0e3);
    h10->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h10->GetXaxis()->SetTitleSize(0.12);
    //h10->GetXaxis()->SetTitleFont(43);
    h10->GetXaxis()->SetTitleOffset(.9);
    h10->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetXaxis()->SetLabelSize(0.1);

    TLine *line1 = new TLine(0,1.,200,1.);
    line1->SetLineStyle(2);
    line1->Draw("same");

    pad1->cd();
    TLatex Tl;

    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.15,0.85,"#phi^{*}_{#eta}<0.35, #epsilon_{sig}=0.98, #epsilon_{bkg}=0.43");

    //save plot
    if (saveplots){
      c4->SaveAs("plots/20161101/w+jets/semilep/reco/MT_MET_l_r1_cumulative_Ztautau_vs_w+jets_reco.pdf");
    }    
  }

  if( MET ){
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.65,0.37,0.75,0.67);
    hist_leg->SetFillColor(0);
    hist_leg->SetFillStyle(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.04);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/CTdotMET_r2");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/CTdotMET_r2");
    int rebinval = 1;
    //    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco c^{l}_{T}#upoint#slash{E}_{T} distributions");
    h->GetXaxis()->SetTitle("c^{l}_{T}#upoint#slash{E}_{T} [GeV^{2}]");
    h->GetYaxis()->SetTitle("Normalised cumulative events");
    h->GetXaxis()->SetTitleSize(0.07);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(0.8);
    h->GetYaxis()->SetTitleOffset(0.8);
    //    h->GetXaxis()->SetRangeUser(-4.1,8);
    h->Rebin(rebinval);
    h->SetLineWidth(1);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(1);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));

    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kFALSE);
    TH1* hc2 = h2->GetCumulative(kFALSE);

    // Define the Canvas
    TCanvas *c5 = new TCanvas("c5", "c5", 800, 600);

    c5->SetRightMargin(0.15);
    c5->SetLeftMargin(0.14);
    c5->SetBottomMargin(0.16);
    
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    hc->SetStats(0);          // No statistics on upper plot
    hc->GetYaxis()->SetTitle("Normalised cumulative events");
    hc->GetYaxis()->SetTitleSize(0.04);
 
    hc->GetXaxis()->SetRangeUser(-200,200);
    //    hc->GetYaxis()->SetRangeUser(-.05,1.05);
    hc->Draw();               // Draw h1
    hc2->Draw("same");
    hist_leg->Draw("same");

    // now, fill the efficiency histogram
    int nbinsx = hc->GetXaxis()->GetNbins();
    cout << nbinsx << endl;
    // loop over bins 31 to 90
    for( int i = 0; i < nbinsx+1; ++i ){
      dt5->SetPoint( i, hc->GetBinContent(i), hc2->GetBinContent(i));
    }

    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    c5->cd();          // Go back to the main canvas before defining pad2
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

    h10->GetXaxis()->SetTitle("c^{l}_{T}#upoint#slash{E}_{T} [GeV^{2}]");
    h10->GetYaxis()->SetTitle("Ratio Z/w+jets");
    h10->GetXaxis()->SetTitleSize(0.07);
    h10->GetYaxis()->SetTitleSize(0.07);
    h10->GetXaxis()->SetTitleOffset(.2);
    h10->GetYaxis()->SetTitleOffset(0.6);
    TLine *line = new TLine(-6.,1.,2.,1);
    h10->Draw("");
    line->SetLineStyle(2);
    //    line->Draw("same");
    
    

    h10->SetTitle(""); // Remove the ratio title
    h10->GetYaxis()->SetNdivisions(505);
    h10->GetYaxis()->SetTitleSize(0.08);
    // h10->GetYaxis()->SetTitleFont(43);
    h10->GetYaxis()->SetTitleOffset(0.45);
    h10->GetYaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetYaxis()->SetLabelSize(0.1);
    //    h10->GetYaxis()->SetRangeUser(.8,14.9);
    h10->GetXaxis()->SetRangeUser(-200,200);
    h10->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h10->GetXaxis()->SetTitleSize(0.12);
    //h10->GetXaxis()->SetTitleFont(43);
    h10->GetXaxis()->SetTitleOffset(.9);
    h10->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetXaxis()->SetLabelSize(0.1);

    TLine *line1 = new TLine(-200,1.,200,1.);
    line1->SetLineStyle(2);
    line1->Draw("same");

    pad1->cd();
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.15,0.85,"#phi^{*}_{#eta}>0.35, #epsilon_{sig}=0.02, #epsilon_{bkg}=0.57");

    //save plot
    if (saveplots){
      c5->SaveAs("plots/20161101/w+jets/semilep/reco/CTdotMET_r2_cumulative_Ztautau_vs_w+jets_reco.pdf");
    }    
  }


  if( MET ){
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.65,0.37,0.75,0.67);
    hist_leg->SetFillColor(0);
    hist_leg->SetFillStyle(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.04);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/MT_MET_l_r2");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/MT_MET_l_r2");
    int rebinval = 1;
    //    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco M_{T}(#slash{E}_{T}l) distributions");
    h->GetXaxis()->SetTitle("M_{T}(#slash{E}_{T}l) [GeV]");
    h->GetYaxis()->SetTitle("Normalised cumulative events");
    h->GetXaxis()->SetTitleSize(0.07);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(0.8);
    h->GetYaxis()->SetTitleOffset(0.8);
    //    h->GetXaxis()->SetRangeUser(-4.1,8);
    h->Rebin(rebinval);
    h->SetLineWidth(1);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(1);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));

    h->Scale(.1 / h->Integral("width"));
    h2->Scale(.1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative();
    TH1* hc2 = h2->GetCumulative();

    // Define the Canvas
    TCanvas *c6 = new TCanvas("c6", "c6", 800, 600);

    c6->SetRightMargin(0.15);
    c6->SetLeftMargin(0.14);
    c6->SetBottomMargin(0.16);
    
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    hc->SetStats(0);          // No statistics on upper plot
    hc->GetYaxis()->SetTitle("Normalised cumulative events");
    hc->GetYaxis()->SetTitleSize(0.04);
 
    //hc->GetXaxis()->SetRangeUser(0.,100.0e3);
    //hc->GetYaxis()->SetRangeUser(-.05,1.05);
    hc->Draw();               // Draw h1
    hc2->Draw("same");
    hist_leg->Draw("same");

    // now, fill the efficiency histogram
    int nbinsx = hc->GetXaxis()->GetNbins();
    cout << nbinsx << endl;
    // loop over bins 31 to 90
    for( int i = 0; i < nbinsx+1; ++i ){
      dt6->SetPoint( i, hc->GetBinContent(i), hc2->GetBinContent(i));
    }

    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    c6->cd();          // Go back to the main canvas before defining pad2
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

    h10->GetXaxis()->SetTitle("M_{T}(#slash{E}_{T}l) [GeV]");
    h10->GetYaxis()->SetTitle("Ratio Z/w+jets");
    h10->GetXaxis()->SetTitleSize(0.07);
    h10->GetYaxis()->SetTitleSize(0.07);
    h10->GetXaxis()->SetTitleOffset(.2);
    h10->GetYaxis()->SetTitleOffset(0.6);
    TLine *line = new TLine(-6.,1.,2.,1);
    h10->Draw("");
    line->SetLineStyle(2);
    //    line->Draw("same");
    
    

    h10->SetTitle(""); // Remove the ratio title
    h10->GetYaxis()->SetNdivisions(505);
    h10->GetYaxis()->SetTitleSize(0.08);
    // h10->GetYaxis()->SetTitleFont(43);
    h10->GetYaxis()->SetTitleOffset(0.45);
    h10->GetYaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetYaxis()->SetLabelSize(0.1);
    //h10->GetYaxis()->SetRangeUser(.8,14.9);
    //h10->GetXaxis()->SetRangeUser(0,100.0e3);
    h10->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h10->GetXaxis()->SetTitleSize(0.12);
    //h10->GetXaxis()->SetTitleFont(43);
    h10->GetXaxis()->SetTitleOffset(.9);
    h10->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetXaxis()->SetLabelSize(0.1);

    TLine *line1 = new TLine(0,1.,200,1.);
    line1->SetLineStyle(2);
    line1->Draw("same");

    pad1->cd();
    TLatex Tl;

    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.15,0.85,"#phi^{*}_{#eta}>0.35, #epsilon_{sig}=0.02, #epsilon_{bkg}=0.57");

    //save plot
    if (saveplots){
      c6->SaveAs("plots/20161101/w+jets/semilep/reco/MT_MET_l_r2_cumulative_Ztautau_vs_w+jets_reco.pdf");
    }    
  }

  if( MET ){
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.65,0.37,0.75,0.67);
    hist_leg->SetFillColor(0);
    hist_leg->SetFillStyle(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.04);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/CTdotMET_r3");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/CTdotMET_r3");
    int rebinval = 1;
    //    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco c^{l}_{T}#upoint#slash{E}_{T} distributions");
    h->GetXaxis()->SetTitle("c^{l}_{T}#upoint#slash{E}_{T} [GeV^{2}]");
    h->GetYaxis()->SetTitle("Normalised cumulative events");
    h->GetXaxis()->SetTitleSize(0.07);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(0.8);
    h->GetYaxis()->SetTitleOffset(0.8);
    //    h->GetXaxis()->SetRangeUser(-4.1,8);
    h->Rebin(rebinval);
    h->SetLineWidth(1);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(1);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));

    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kFALSE);
    TH1* hc2 = h2->GetCumulative(kFALSE);

    // Define the Canvas
    TCanvas *c7 = new TCanvas("c7", "c7", 800, 600);

    c7->SetRightMargin(0.15);
    c7->SetLeftMargin(0.14);
    c7->SetBottomMargin(0.16);
    
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    hc->SetStats(0);          // No statistics on upper plot
    hc->GetYaxis()->SetTitle("Normalised cumulative events");
    hc->GetYaxis()->SetTitleSize(0.04);
 
    hc->GetXaxis()->SetRangeUser(-200,200);
    //    hc->GetYaxis()->SetRangeUser(-.05,1.05);
    hc->Draw();               // Draw h1
    hc2->Draw("same");
    hist_leg->Draw("same");

    // now, fill the efficiency histogram
    int nbinsx = hc->GetXaxis()->GetNbins();
    cout << nbinsx << endl;
    // loop over bins 31 to 90
    for( int i = 0; i < nbinsx+1; ++i ){
      dt7->SetPoint( i, hc->GetBinContent(i), hc2->GetBinContent(i));
    }

    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    c7->cd();          // Go back to the main canvas before defining pad2
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

    h10->GetXaxis()->SetTitle("c^{l}_{T}#upoint#slash{E}_{T} [GeV^{2}]");
    h10->GetYaxis()->SetTitle("Ratio Z/w+jets");
    h10->GetXaxis()->SetTitleSize(0.07);
    h10->GetYaxis()->SetTitleSize(0.07);
    h10->GetXaxis()->SetTitleOffset(.2);
    h10->GetYaxis()->SetTitleOffset(0.6);
    TLine *line = new TLine(-6.,1.,2.,1);
    h10->Draw("");
    line->SetLineStyle(2);
    //    line->Draw("same");
    
    

    h10->SetTitle(""); // Remove the ratio title
    h10->GetYaxis()->SetNdivisions(505);
    h10->GetYaxis()->SetTitleSize(0.08);
    // h10->GetYaxis()->SetTitleFont(43);
    h10->GetYaxis()->SetTitleOffset(0.45);
    h10->GetYaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetYaxis()->SetLabelSize(0.1);
    //    h10->GetYaxis()->SetRangeUser(.8,14.9);
    h10->GetXaxis()->SetRangeUser(-200,200);
    h10->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h10->GetXaxis()->SetTitleSize(0.12);
    //h10->GetXaxis()->SetTitleFont(43);
    h10->GetXaxis()->SetTitleOffset(.9);
    h10->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetXaxis()->SetLabelSize(0.1);

    TLine *line1 = new TLine(-200,1.,200,1.);
    line1->SetLineStyle(2);
    line1->Draw("same");

    pad1->cd();
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.15,0.85,"#phi^{*}_{#eta}<0.1, #epsilon_{sig}=0.65, #epsilon_{bkg}=0.14");
    //save plot
    if (saveplots){
      c7->SaveAs("plots/20161101/w+jets/semilep/reco/CTdotMET_r3_cumulative_Ztautau_vs_w+jets_reco.pdf");
    }    
  }


  if( MET ){
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.65,0.37,0.75,0.67);
    hist_leg->SetFillColor(0);
    hist_leg->SetFillStyle(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.04);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/MT_MET_l_r3");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/MT_MET_l_r3");
    int rebinval = 1;
    //    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco M_{T}(#slash{E}_{T}l) distributions");
    h->GetXaxis()->SetTitle("M_{T}(#slash{E}_{T}l) [GeV]");
    h->GetYaxis()->SetTitle("Normalised cumulative events");
    h->GetXaxis()->SetTitleSize(0.07);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(0.8);
    h->GetYaxis()->SetTitleOffset(0.8);
    //    h->GetXaxis()->SetRangeUser(-4.1,8);
    h->Rebin(rebinval);
    h->SetLineWidth(1);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(1);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));

    h->Scale(.1 / h->Integral("width"));
    h2->Scale(.1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative();
    TH1* hc2 = h2->GetCumulative();

    // Define the Canvas
    TCanvas *c8 = new TCanvas("c8", "c8", 800, 600);

    c8->SetRightMargin(0.15);
    c8->SetLeftMargin(0.14);
    c8->SetBottomMargin(0.16);
    
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    hc->SetStats(0);          // No statistics on upper plot
    hc->GetYaxis()->SetTitle("Normalised cumulative events");
    hc->GetYaxis()->SetTitleSize(0.04);
 
    //hc->GetXaxis()->SetRangeUser(0.,100.0e3);
    //hc->GetYaxis()->SetRangeUser(-.05,1.05);
    hc->Draw();               // Draw h1
    hc2->Draw("same");
    hist_leg->Draw("same");

    // now, fill the efficiency histogram
    int nbinsx = hc->GetXaxis()->GetNbins();
    cout << nbinsx << endl;
    // loop over bins 31 to 90
    for( int i = 0; i < nbinsx+1; ++i ){
      dt8->SetPoint( i, hc->GetBinContent(i), hc2->GetBinContent(i));
    }

    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    c8->cd();          // Go back to the main canvas before defining pad2
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

    h10->GetXaxis()->SetTitle("M_{T}(#slash{E}_{T}l) [GeV]");
    h10->GetYaxis()->SetTitle("Ratio Z/w+jets");
    h10->GetXaxis()->SetTitleSize(0.07);
    h10->GetYaxis()->SetTitleSize(0.07);
    h10->GetXaxis()->SetTitleOffset(.2);
    h10->GetYaxis()->SetTitleOffset(0.6);
    TLine *line = new TLine(-6.,1.,2.,1);
    h10->Draw("");
    line->SetLineStyle(2);
    //    line->Draw("same");
    
    

    h10->SetTitle(""); // Remove the ratio title
    h10->GetYaxis()->SetNdivisions(505);
    h10->GetYaxis()->SetTitleSize(0.08);
    // h10->GetYaxis()->SetTitleFont(43);
    h10->GetYaxis()->SetTitleOffset(0.45);
    h10->GetYaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetYaxis()->SetLabelSize(0.1);
    //h10->GetYaxis()->SetRangeUser(.8,14.9);
    //h10->GetXaxis()->SetRangeUser(0,100.0e3);
    h10->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h10->GetXaxis()->SetTitleSize(0.12);
    //h10->GetXaxis()->SetTitleFont(43);
    h10->GetXaxis()->SetTitleOffset(.9);
    h10->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetXaxis()->SetLabelSize(0.1);

    TLine *line1 = new TLine(0,1.,200,1.);
    line1->SetLineStyle(2);
    line1->Draw("same");

    pad1->cd();
    TLatex Tl;

    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.15,0.85,"#phi^{*}_{#eta}<0.1, #epsilon_{sig}=0.65, #epsilon_{bkg}=0.14");

    //save plot
    if (saveplots){
      c8->SaveAs("plots/20161101/w+jets/semilep/reco/MT_MET_l_r3_cumulative_Ztautau_vs_w+jets_reco.pdf");
    }    
  }

  if( MET ){
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg = new TLegend(0.65,0.37,0.75,0.67);
    hist_leg->SetFillColor(0);
    hist_leg->SetFillStyle(0);
    hist_leg->SetLineColor(1);
    hist_leg->SetShadowColor(0);
    hist_leg->SetTextSize(0.04);
    hist_leg->SetTextFont(62);
    hist_leg->SetBorderSize(0);
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/CT_r1");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/CT_r1");
    int rebinval = 1;
    //    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco c_{T} distributions");
    h->GetXaxis()->SetTitle("c_{T} [GeV]");
    h->GetYaxis()->SetTitle("Normalised cumulative events");
    h->GetXaxis()->SetTitleSize(0.07);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(0.8);
    h->GetYaxis()->SetTitleOffset(0.8);
    //    h->GetXaxis()->SetRangeUser(-4.1,8);
    h->Rebin(rebinval);
    h->SetLineWidth(1);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(1);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));

    h->Scale(.025 / h->Integral("width"));
    h2->Scale(.025 / h2->Integral("width"));
    TH1* hc = h->GetCumulative();
    TH1* hc2 = h2->GetCumulative();

    // Define the Canvas
    TCanvas *c9 = new TCanvas("c9", "c9", 800, 600);

    c9->SetRightMargin(0.15);
    c9->SetLeftMargin(0.14);
    c9->SetBottomMargin(0.16);
    
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    hc->SetStats(0);          // No statistics on upper plot
    hc->GetYaxis()->SetTitle("Normalised cumulative events");
    hc->GetYaxis()->SetTitleSize(0.04);
 
    //hc->GetXaxis()->SetRangeUser(0.,100.0e3);
    //hc->GetYaxis()->SetRangeUser(-.05,1.05);
    hc->Draw();               // Draw h1
    hc2->Draw("same");
    hist_leg->Draw("same");

    // now, fill the efficiency histogram
    int nbinsx = hc->GetXaxis()->GetNbins();
    cout << nbinsx << endl;
    // loop over bins 31 to 90
    for( int i = 0; i < nbinsx+1; ++i ){
      dt9->SetPoint( i, hc->GetBinContent(i), hc2->GetBinContent(i));
    }

    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    c9->cd();          // Go back to the main canvas before defining pad2
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

    h10->GetXaxis()->SetTitle("M_{T}(#slash{E}_{T}l) [GeV]");
    h10->GetYaxis()->SetTitle("Ratio Z/w+jets");
    h10->GetXaxis()->SetTitleSize(0.07);
    h10->GetYaxis()->SetTitleSize(0.07);
    h10->GetXaxis()->SetTitleOffset(.2);
    h10->GetYaxis()->SetTitleOffset(0.6);
    TLine *line = new TLine(-6.,1.,2.,1);
    h10->Draw("");
    line->SetLineStyle(2);
    //    line->Draw("same");
    
    

    h10->SetTitle(""); // Remove the ratio title
    h10->GetYaxis()->SetNdivisions(505);
    h10->GetYaxis()->SetTitleSize(0.08);
    // h10->GetYaxis()->SetTitleFont(43);
    h10->GetYaxis()->SetTitleOffset(0.45);
    h10->GetYaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetYaxis()->SetLabelSize(0.1);
    //h10->GetYaxis()->SetRangeUser(.8,14.9);
    //h10->GetXaxis()->SetRangeUser(0,100.0e3);
    h10->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h10->GetXaxis()->SetTitleSize(0.12);
    //h10->GetXaxis()->SetTitleFont(43);
    h10->GetXaxis()->SetTitleOffset(.9);
    h10->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetXaxis()->SetLabelSize(0.1);

    TLine *line1 = new TLine(0,1.,200,1.);
    line1->SetLineStyle(2);
    line1->Draw("same");

    pad1->cd();
    TLatex Tl;

    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.15,0.85,"0.1<#phi^{*}_{#eta}<0.35, #epsilon_{sig}=0.98, #epsilon_{bkg}=0.43");

    //save plot
    if (saveplots){
      c9->SaveAs("plots/20161101/w+jets/semilep/reco/cT_r1_cumulative_Ztautau_vs_w+jets_reco.pdf");
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
    hist_leg1 = new TLegend(0.81,0.45,1,0.90);
    hist_leg1->SetFillColor(0);
    hist_leg1->SetLineColor(1);
    hist_leg1->SetShadowColor(0);
    hist_leg1->SetTextSize(0.025);
    hist_leg1->SetTextFont(62);
    hist_leg1->SetFillStyle(0);
    hist_leg1->SetBorderSize(0);
    c20->SetRightMargin(0.20);
    c20->SetLeftMargin(0.14);
    c20->SetBottomMargin(0.14);

    dt->SetTitle("Reco background vs signal efficiencies");
    dt->GetXaxis()->SetTitle("#epsilon_{sig}");
    dt->GetYaxis()->SetTitle("#epsilon_{bkg}");
    dt->GetXaxis()->SetTitleSize(0.06);
    dt->GetYaxis()->SetTitleSize(0.06);
    dt->GetXaxis()->SetTitleOffset(0.8);
    dt->GetYaxis()->SetTitleOffset(0.8);
    dt->GetXaxis()->SetRangeUser(0,1.05);
    dt->GetYaxis()->SetRangeUser(0,1.05);
    dt->SetMarkerStyle(8);
    dt2->SetMarkerStyle(8);
    dt->SetMarkerSize(.8);
    dt2->SetMarkerSize(.8);
//    dt->SetMarkerColorAlpha(1,.7);
//    dt2->SetMarkerColorAlpha(2,.7);
    dt->SetLineWidth(2);
    dt2->SetLineWidth(2);
    dt3->SetLineWidth(2);
    dt4->SetLineWidth(2);
    dt5->SetLineWidth(2);
    dt6->SetLineWidth(2);
    dt7->SetLineWidth(2);    
    dt8->SetLineWidth(2);
    dt9->SetLineWidth(2);
    dt->SetLineColor(2);
    dt2->SetLineColor(46);
    dt3->SetLineColor(4);
    dt4->SetLineColor(9);
    dt5->SetLineColor(7);
    dt6->SetLineColor(7);
    dt7->SetLineColor(8);
    dt8->SetLineColor(8);
    dt9->SetLineColor(12);
    dt2->SetLineStyle(2);
    dt4->SetLineStyle(2);
    dt6->SetLineStyle(2);
    dt8->SetLineStyle(2);
    util->SetLineColor(1);
    util->SetLineStyle(1);
    util->SetLineWidth(3);
    hist_leg1->AddEntry(dt, Form("c^{l}_{T}#upoint#slash{E}_{T}, all #phi^{*}_{#eta}"), "l");
    hist_leg1->AddEntry(dt2, Form("M_{T}(#slash{E}_{T}l), all #phi^{*}_{#eta}"),"l");
    hist_leg1->AddEntry(dt3, Form("c^{l}_{T}#upoint#slash{E}_{T}, #phi^{*}_{#eta}<0.35"),"l");
    hist_leg1->AddEntry(dt4, Form("M_{T}(#slash{E}_{T}l), #phi^{*}_{#eta}<0.35"),"l");
    hist_leg1->AddEntry(dt5, Form("c^{l}_{T}#upoint#slash{E}_{T}, #phi^{*}_{#eta}>0.35"),"l");
    hist_leg1->AddEntry(dt6, Form("M_{T}(#slash{E}_{T}l), #phi^{*}_{#eta}>0.35"),"l");
    hist_leg1->AddEntry(dt7, Form("c^{l}_{T}#upoint#slash{E}_{T}, #phi^{*}_{#eta}<0.1"),"l");
    hist_leg1->AddEntry(dt8, Form("M_{T}(#slash{E}_{T}l), #phi^{*}_{#eta}<0.1"),"l");
    hist_leg1->AddEntry(dt9, Form("c_{T}, 0.1<#phi^{*}_{#eta}<0.35"),"l");
    dt->Draw("AL");
    dt2->Draw("same");
    dt3->Draw("same");
    dt4->Draw("same");
    dt5->Draw("same");
    dt6->Draw("same");
    dt7->Draw("same");
    dt8->Draw("same");
    dt9->Draw("same");
    util->Draw("same");
    hist_leg1->Draw("same");

    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.2,0.82,"Z#rightarrow#tau_{l}#tau_{h} (sig) vs W(#rightarrow#mu#nu)+jets (bkg)");
    //Tl.DrawLatexNDC(0.2,0.82,"Z#rightarrow#tau_{l}#tau_{h} (sig) vs w+jets#rightarrow lh (bkg)");

    //save plot
    if (saveplots){
      c20->SaveAs("plots/20161101/w+jets/semilep/reco/Eff_MET_Ztautau_vs_w+jets_reco.pdf");
    }    

  }

}

