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

void plotEff_1D(){

  TFile *f = new TFile("Ztautau1M.root");
  TFile *f1 = new TFile("Wplusmunu1M.root");

  bool saveplots = true;
  bool aT	 = true;
  bool atdivphi	 = true;
  bool eff	 = true;
  //TH2F *e	 = new TH2F("e","e",60,0,1,60,0,1);
  TGraph *dt	 = new TGraph();
  TGraph *dt2	 = new TGraph();


  if( aT ){
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
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/AT");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/AT");
    int rebinval = 1;
    //    TGaxis::SetMaxDigits(3); 
    h->GetXaxis()->SetTitle("a_{T}");
    h->GetYaxis()->SetTitle("Normalised cumulative events");
    h->GetXaxis()->SetTitleSize(0.07);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(0.8);
    h->GetYaxis()->SetTitleOffset(0.8);
    //c102->SetLogy();
    //    h->GetXaxis()->SetRangeUser(0,70);
    h->Rebin(rebinval);
    h->SetLineWidth(1);
    h->SetLineStyle(1);

    h2->SetLineColor(2);
    h2->SetLineWidth(1);
    h2->Rebin(rebinval);
    h2->SetLineStyle(1);

    hist_leg->AddEntry(h, Form("Z#rightarrow#tau_{l}#tau_{h}"));
    hist_leg->AddEntry(h2, Form("W(#rightarrow#mu#nu)+jets"));
    //hist_leg->AddEntry(h2, Form("w+jets#rightarrow lh"));

    h->Scale(0.1 / h->Integral("width"));
    h2->Scale(0.1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kTRUE);
    TH1* hc2 = h2->GetCumulative(kTRUE);

    // Define the Canvas
    TCanvas *c102 = new TCanvas("c102", "c102", 800, 600);

    c102->SetRightMargin(0.15);
    c102->SetLeftMargin(0.14);
    c102->SetBottomMargin(0.16);
    
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
 
    //    hc->GetXaxis()->SetRangeUser(0.,70);
    hc->GetYaxis()->SetRangeUser(-.05,1.05);
    hc->Draw();               // Draw h1
    hc2->Draw("same");
    hist_leg->Draw("same");

    // now, fill the efficiency histogram
    int nbinsx = hc->GetXaxis()->GetNbins();
    double totalevssig = h->Integral("width");
    cout << totalevssig << endl;

    double totalevsbkg = h2->Integral("width");
    cout << totalevsbkg << endl;
    //r1: 30, r2: 30-60; r: 60+
    double r1sig = h->Integral(0, 32, "width");
    double r2sig = h->Integral(33,260, "width");
    double r3sig = h->Integral(261,2000, "width");

    double r1bkg = h2->Integral(0, 32, "width");
    double r2bkg = h2->Integral(33,260, "width");
    double r3bkg = h2->Integral(261,2000, "width");
    cout << nbinsx << endl;
    for( int i = 0; i < nbinsx+1; ++i ){
      if( hc2->GetBinContent(i) != 0 ){
	dt->SetPoint( i, hc->GetBinContent(i), hc->GetBinContent(i)/TMath::Sqrt(hc2->GetBinContent(i)));
	//cout << float(i)/2000 << " "<< hc->GetBinContent(i)/sqrt(hc2->GetBinContent(i)) << endl;
      }    
    }
    // Do not draw the Y axis label on the upper plot and redraw a small
    // axis instead, in order to avoid the first label (0) to be clipped.
    //h->GetYaxis()->SetLabelSize(0.);
    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    //axis->Draw();
    
    // lower plot will be in pad
    c102->cd();          // Go back to the main canvas before defining pad2
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

    h10->GetXaxis()->SetTitle("a_{T} [GeV]");
    h10->GetYaxis()->SetTitle("Ratio Z/w+jets");
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
    //    h10->GetYaxis()->SetRangeUser(.8,14.9);
    //    h10->GetXaxis()->SetRangeUser(0,100.0e3);
    h10->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h10->GetXaxis()->SetTitleSize(0.12);
    //h10->GetXaxis()->SetTitleFont(43);
    h10->GetXaxis()->SetTitleOffset(.9);
    h10->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetXaxis()->SetLabelSize(0.1);

    TLine *line1 = new TLine(0,1.,100.0e3,1.);
    line1->SetLineStyle(2);
    //    line1->Draw("same");

    pad1->cd();
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.05);
    Tl.DrawLatexNDC(0.15,0.85,"lh final state");

    //save plot
    if (saveplots){
      c102->SaveAs("plots/20161115/w+jets/semilep/reco/AT_Ztautau_vs_w+jets_reco.pdf");
    }    
  }

  if( atdivphi ){
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
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/MT_MET_add");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/MT_MET_add");
    int rebinval = 1;
    //    TGaxis::SetMaxDigits(3); 
    h->SetTitle("Reco a_{T}.#slash{E}_{T} distributions");
    h->GetXaxis()->SetTitle("log_{10}sin#theta^{*}_{#eta}");
    h->GetYaxis()->SetTitle("Normalised cumulative events");
    h->GetXaxis()->SetTitleSize(0.07);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetLabelSize(0.03);
    h->GetXaxis()->SetTitleOffset(0.8);
    h->GetYaxis()->SetTitleOffset(0.8);
    //c103->SetLogy();
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
    //hist_leg->AddEntry(h2, Form("w+jets#rightarrow lh"));

    h->Scale(.1 / h->Integral("width"));
    h2->Scale(.1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kTRUE);
    TH1* hc2 = h2->GetCumulative(kTRUE);

    // Define the Canvas
    TCanvas *c103 = new TCanvas("c103", "c103", 800, 600);

    c103->SetRightMargin(0.15);
    c103->SetLeftMargin(0.14);
    c103->SetBottomMargin(0.16);
    
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
 
    //hc->GetXaxis()->SetRangeUser(0.,100.0e3);
    hc->GetYaxis()->SetRangeUser(-.05,1.05);
    hc->Draw();               // Draw h1
    hc2->Draw("same");
    hist_leg->Draw("same");

    // now, fill the efficiency histogram
    int nbinsx = hc->GetXaxis()->GetNbins();
    cout << nbinsx << endl;
    // loop over bins 31 to 90
    //sig
    for( int i = 0; i < nbinsx+1; ++i ){
      //      dt2->SetPoint( i, 0.65 + .327* hc->GetBinContent(i), 0.136+.294*hc2->GetBinContent(i));      
      //      dt2->SetPoint( i, .503* hc->GetBinContent(i),  0.051*hc2->GetBinContent(i));
      if( hc2->GetBinContent(i) != 0 ){
	dt2->SetPoint( i, hc->GetBinContent(i), hc->GetBinContent(i)/sqrt(hc2->GetBinContent(i)));
	//cout << float(i)/2000 << " "<< hc->GetBinContent(i)/sqrt(hc2->GetBinContent(i)) << endl;
      }
    }

    // Do not draw the Y axis label on the upper plot and redraw a small
    // axis instead, in order to avoid the first label (0) to be clipped.
    //h->GetYaxis()->SetLabelSize(0.);
    TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    //axis->Draw();
    
    // lower plot will be in pad
    c103->cd();          // Go back to the main canvas before defining pad2
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

    h10->GetXaxis()->SetTitle("log#phi^{*}_{#eta}");
    h10->GetYaxis()->SetTitle("Ratio Z/w+jets");
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
    //    h10->GetYaxis()->SetRangeUser(.8,14.9);
    //    h10->GetXaxis()->SetRangeUser(0,100.0e3);
    h10->GetYaxis()->CenterTitle();
    
    // X axis ratio plot settings
    h10->GetXaxis()->SetTitleSize(0.12);
    //h10->GetXaxis()->SetTitleFont(43);
    h10->GetXaxis()->SetTitleOffset(.9);
    h10->GetXaxis()->SetLabelFont(62); // Absolute font size in pixel (precision 3)
    h10->GetXaxis()->SetLabelSize(0.1);

    TLine *line1 = new TLine(0,1.,100.0e3,1.);
    line1->SetLineStyle(2);
    //    line1->Draw("same");

    pad1->cd();
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.05);
    Tl.DrawLatexNDC(0.15,0.85,"lh final state");

    //save plot
    if (saveplots){
      c103->SaveAs("plots/20161115/w+jets/semilep/reco/MT_MET_add_Ztautau_vs_w+jets_reco.pdf");
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
    hist_leg1 = new TLegend(0.86,0.7,1,0.90);
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

    dt->SetTitle("Reco background vs signal efficiencies");
    dt->GetXaxis()->SetTitle("#epsilon_{sig}");
    dt->GetYaxis()->SetTitle("#epsilon_{sig}/#sqrt{#epsilon_{bkg}}");
    dt->GetXaxis()->SetTitleSize(0.06);
    dt->GetYaxis()->SetTitleSize(0.06);
    dt->GetXaxis()->SetTitleOffset(0.8);
    dt->GetYaxis()->SetTitleOffset(0.8);
    dt->GetXaxis()->SetRangeUser(0, 1.02);
    dt->GetYaxis()->SetRangeUser(0,3.4);
    dt->SetMarkerStyle(8);
    dt2->SetMarkerStyle(8);
    dt->SetMarkerSize(.6);
    dt2->SetMarkerSize(.6);
    dt->SetLineWidth(2);
    dt2->SetLineWidth(2);
    dt->SetLineColor(2);
    dt2->SetLineColor(4);
    dt->SetMarkerColor(2);
    dt2->SetMarkerColor(4);
    dt2->SetLineStyle(2);
    hist_leg1->AddEntry(dt, Form("a_{T}"), "l");
    hist_leg1->AddEntry(dt2, Form("+M_{T}(#slash{E}_{T},l)"),"l");
    //hist_leg1->AddEntry(dt3, Form("a_{T}, R2"),"p");
    dt->Draw("AL");
    dt2->Draw("same L");
    hist_leg1->Draw("same");

    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.2,0.82,"Z#rightarrow#tau_{l}#tau_{h} (sig) vs W(#rightarrow#mu#nu)+jets (bkg)");

    //save plot
    if (saveplots){
      c20->SaveAs("plots/20161115/w+jets/semilep/reco/SIC_fn_e_sig.pdf");
    }    

  }

}

