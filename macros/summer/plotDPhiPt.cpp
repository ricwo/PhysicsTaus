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

void plotDPhiPt(){

  TFile *ftautau = new TFile("Ztautau10M.root");

  bool saveplots	= true;
  bool DPhi_Pt		= true;
  bool DPhi_Pt_slices	= false;


  if (DPhi_Pt){
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

    TH2F *Phi_M		= (TH2F*)ftautau->Get("361510/plotEventAlgs/DPhi_Pt_mu");

    int   NentriesPhiM	= (Phi_M->GetEntries());
    Phi_M->SetTitle("#Delta#phi_{#mu#tau} vs. p_{T}^{#mu}/p_{T}^{#tau}");
    //Phi_M->GetZaxis()->SetTitle("Events / 0.00001");
    Phi_M->GetXaxis()->SetTitle("log_{10}(#Delta#phi_{#mu#tau})");
    Phi_M->GetYaxis()->SetTitle("p_{T}^{#mu}/p_{T}^{#tau}");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    Phi_M->GetYaxis()->SetTitleOffset(1.);
    Phi_M->Rebin2D(2,1);
    //Phi_M->GetYaxis()->SetRangeUser(.1,1);

    Phi_M->GetXaxis()->SetRangeUser(-6.,0.);
    //    Phi_M->GetXaxis()->SetBinLabel(749, "#pi");

    Phi_M->GetYaxis()->SetRangeUser(0,1.2);
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
    Tl.DrawLatexNDC(0.2,0.85,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau, #mu final state");
    //    Tl.DrawLatexNDC(0.5,0.75,Form("%i entries", NentriesPhiM));

    //save plot
    if (saveplots){
      c0->SaveAs("plots/20160615/DPhi_pT_fraction_muons.pdf");
    }
    
  }


  if (DPhi_Pt_slices){
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

    bool gaus = false;

    TH2F *Phi_M		= (TH2F*)ftautau->Get("361510/plotEventAlgs/DPhi_Pt_mu2");

    int   NentriesPhiM	= (Phi_M->GetEntries());
    int rebinval	= 2;

    Phi_M->SetTitle("#Delta#phi_{#mu#tau} vs. p_{T}^{#mu}/p_{T}^{#tau}");
    Phi_M->GetXaxis()->SetTitleSize(0.04);
    Phi_M->GetYaxis()->SetTitleSize(0.06);
    Phi_M->GetXaxis()->SetTitleOffset(1.15);
    //    Phi_M->GetYaxis()->SetTitleOffset(0.85);
    //    Phi_M->GetXaxis()->SetRangeUser(-3.5,-.5);
    //    Phi_M->GetYaxis()->SetRangeUser(100,5000);
    //    Phi_M->Rebin2D(2, 10);

    TH1D *px1 = Phi_M->ProjectionX("px1",1,29,"o"); //phi<.1
    //px1->GetXaxis()->SetLimits(-3.5,-.5);
    px1->SetLineColor(1);
    px1->Rebin(rebinval);
    px1->GetXaxis()->SetTitle("-log_{10}(#Delta#phi_{#mu#tau})");
    hist_leg->AddEntry(px1, "p_{T}^{#mu}/p_{T}^{#tau} < 0.30");
    px1->GetXaxis()->SetRangeUser(0.5,3.5);
    //px1->GetYaxis()->SetRangeUser(100,5000);

    TH1D *px2 = Phi_M->ProjectionX("px2",30,36,"o"); // .1<phi<1.0
    px2->SetLineColor(2);
    px2->Rebin(rebinval);
    //px2->Scale(.5);
    hist_leg->AddEntry(px2, "0.30 < p_{T}^{#mu}/p_{T}^{#tau} < 0.37");

    TH1D *px3 = Phi_M->ProjectionX("px3",37,44); // 1.0<phi<10
    px3->SetLineColor(4);
    px3->Rebin(rebinval);
    //px3->Scale(.5);
    hist_leg->AddEntry(px3, "0.37 < p_{T}^{#mu}/p_{T}^{#tau} < 0.45");

    TH1D *px4 = Phi_M->ProjectionX("px4",45,54); // 1.0<phi<10
    px4->SetLineColor(6);
    px4->Rebin(rebinval);
    //px4->Scale(.5);
    hist_leg->AddEntry(px4, "0.45 < p_{T}^{#mu}/p_{T}^{#tau} < 0.55");

    TH1D *px5 = Phi_M->ProjectionX("px5",55,67); // 1.0<phi<10
    px5->SetLineColor(8);
    px5->Rebin(rebinval);
    //px5->Scale(.5);
    hist_leg->AddEntry(px5, "0.55 < p_{T}^{#mu}/p_{T}^{#tau} < 0.68");

    TH1D *px6 = Phi_M->ProjectionX("px6",68,120); // 1.0<phi<10
    px6->SetLineColor(kGray);
    px6->Rebin(rebinval);
    //px6->Scale(.5);
    hist_leg->AddEntry(px6, "0.68 < p_{T}^{#mu}/p_{T}^{#tau}");


    c1->SetLogy();
    c1->SetRightMargin(0.15);
    // px2->Draw("same");
    // px3->Draw("same");
    // px4->Draw("same");
    // px5->Draw("same");
    // px6->Draw("same");
    // hist_leg->Draw("same");

    // px1->GetXaxis()->SetRangeUser(1,3.5);
    // px2->GetXaxis()->SetRangeUser(1,3.5);
    // px3->GetXaxis()->SetRangeUser(1,3.5);
    // px4->GetXaxis()->SetRangeUser(1,3.5);
    // px5->GetXaxis()->SetRangeUser(1,3.5);
    // px6->GetXaxis()->SetRangeUser(1,3.5);
    //    px1->GetXaxis()->SetLimits(0,5);
    //    TF1 * f1 = new TF1("f1","[0]*ROOT::Math::lognormal_pdf(x,[0],[1] ) " ); 
    //    f1->SetParameters(
    TF1 * f1 = new TF1("f1","[0]*ROOT::Math::lognormal_pdf(x,[1],[2] ) ",1.2,2.0 ); 
   // set initial parameters 
    double p[3]; 
    p[0] = px1->GetEntries()*px1->GetXaxis()->GetBinWidth(1);   // area of the histogram 
    // find median of histogram 
    double prob[] = {0.5}; 
    double q[1]; 
    px1->GetQuantiles(1,q,prob);
    double median = q[0];
    // find mode of histogram 
    double  mode = px1->GetBinCenter( px1->GetMaximumBin());
    std::cout << "histogram mode is " << mode  << " median is " << median << std::endl;
    if (median < 0) { 
      Error("lognormal","not valid histogram median");
      return;
    }
    // m is log(median)
    p[1] = std::log(median); 
    // s2 is  log(median) - log(mode) 
    p[2] = std::sqrt( std::log(median/mode) ); 
    f1->SetParameters(p); 
    f1->SetParName(0,"A");
    f1->SetParName(1,"m");
    f1->SetParName(2,"s");
    f1->Print();
    f1->SetLineWidth(1);
    std::cout << f1->GetNpar() << std::endl;
    px1->Draw("");
    px1->GetYaxis()->SetRangeUser(200,1500);
    px1->GetXaxis()->SetRangeUser(1.3,2.);
    px1->Fit("f1","R");
    cout<<f1->GetParameter(1)<<" 123hallo"<<endl;
    TLine *px1line = new TLine(f1->GetMaximumX(),200,f1->GetMaximumX(),f1->GetMaximum());
    px1line->SetLineColor(1);
    px1line->SetLineWidth(2);
    px1line->SetLineStyle(2);
    cout<<f1->GetMaximumX()<<"max x"<<f1->GetMaximum()<<"max y"<<endl;
    px1line->Draw("same");
    //    cout<<f1->GetMaximumBin()<<"bin"<<endl;


    TF1 * f2 = new TF1("f2","[0]*ROOT::Math::lognormal_pdf(x,[1],[2] ) ",1.2,2.0 ); 
   // set initial parameters 
    double p2[3]; 
    p2[0] = px2->GetEntries()*px2->GetXaxis()->GetBinWidth(1);   // area of the histogram 
    // find median of histogram 
    double prob2[] = {0.5}; 
    double q2[1]; 
    px2->GetQuantiles(1,q2,prob2);
    double median2 = q2[0];
    // find mode of histogram 
    double  mode2 = px2->GetBinCenter( px2->GetMaximumBin());
    std::cout << "histogram mode is " << mode2  << " median is " << median2 << std::endl;
    if (median2 < 0) { 
      Error("lognormal","not valid histogram median");
      return;
    }
    // m is log(median)
    p2[1] = std::log(median2); 
    // s2 is  log(median) - log(mode) 
    p2[2] = std::sqrt( std::log(median2/mode2) ); 
    f2->SetParameters(p2); 
    f2->SetParName(0,"A2");
    f2->SetParName(1,"m2");
    f2->SetParName(2,"s2");
    f2->Print();
    std::cout << f2->GetNpar() << std::endl;
    px2->Draw("same");
    px2->Fit("f2","R");
    px2->GetFunction("f2")->SetLineColor(2);
    px2->GetFunction("f2")->SetLineWidth(1);
    TLine *px2line = new TLine(f2->GetMaximumX(),200,f2->GetMaximumX(),f2->GetMaximum());
    px2line->SetLineColor(2);
    px2line->SetLineStyle(2);
    px2line->SetLineWidth(2);
    px2line->Draw("same");

    TF1 * f3 = new TF1("f3","[0]*ROOT::Math::lognormal_pdf(x,[1],[2] ) ",1.2,2.0 ); 
   // set initial parameters 
    double p3[3]; 
    p3[0] = px3->GetEntries()*px3->GetXaxis()->GetBinWidth(1);   // area of the histogram 
    // find median of histogram 
    double prob3[] = {0.5}; 
    double q3[1]; 
    px3->GetQuantiles(1,q3,prob3);
    double median3 = q3[0];
    // find mode of histogram 
    double  mode3 = px3->GetBinCenter( px3->GetMaximumBin());
    std::cout << "histogram mode is " << mode3  << " median is " << median3 << std::endl;
    if (median3 < 0) { 
      Error("lognormal","not valid histogram median");
      return;
    }
    // m is log(median)
    p3[1] = std::log(median3); 
    // s2 is  log(median) - log(mode) 
    p3[2] = std::sqrt( std::log(median3/mode3) ); 
    f3->SetParameters(p3); 
    f3->SetParName(0,"A3");
    f3->SetParName(1,"m3");
    f3->SetParName(2,"s3");
    f3->SetLineColor(4);
    f3->SetLineWidth(1);
    f3->Print();
    std::cout << f3->GetNpar() << std::endl;
    px3->Draw("same");
    px3->Fit("f3","R");
    TLine *px3line = new TLine(f3->GetMaximumX(),200,f3->GetMaximumX(),f3->GetMaximum());
    px3line->SetLineColor(4);
    px3line->SetLineStyle(2);
    px3line->SetLineWidth(2);
    px3line->Draw("same");


    TF1 * f4 = new TF1("f4","[0]*ROOT::Math::lognormal_pdf(x,[1],[2] ) ",1.2,2.0 ); 
   // set initial parameters 
    double p4[3]; 
    p4[0] = px4->GetEntries()*px4->GetXaxis()->GetBinWidth(1);   // area of the histogram 
    // find median of histogram 
    double prob4[] = {0.5}; 
    double q4[1]; 
    px4->GetQuantiles(1,q4,prob4);
    double median4 = q4[0];
    // find mode of histogram 
    double  mode4 = px4->GetBinCenter( px4->GetMaximumBin());
    std::cout << "histogram mode is " << mode4  << " median is " << median4 << std::endl;
    if (median3 < 0) { 
      Error("lognormal","not valid histogram median");
      return;
    }
    // m is log(median)
    p4[1] = std::log(median4); 
    // s2 is  log(median) - log(mode) 
    p4[2] = std::sqrt( std::log(median4/mode4) ); 
    f4->SetParameters(p4); 
    f4->SetParName(0,"A4");
    f4->SetParName(1,"m4");
    f4->SetParName(2,"s4");
    f4->SetLineColor(6);
    f4->SetLineWidth(1);
    f4->Print();
    std::cout << f4->GetNpar() << std::endl;
    px4->Draw("same");
    px4->Fit("f4","R");
    TLine *px4line = new TLine(f4->GetMaximumX(),200,f4->GetMaximumX(),f4->GetMaximum());
    px4line->SetLineColor(6);
    px4line->SetLineStyle(2);
    px4line->SetLineWidth(2);
    px4line->Draw("same");
 

    TF1 * f5 = new TF1("f5","[0]*ROOT::Math::lognormal_pdf(x,[1],[2] ) ",1.2,2.0 ); 
   // set initial parameters 
    double p5[3]; 
    p5[0] = px5->GetEntries()*px5->GetXaxis()->GetBinWidth(1);   // area of the histogram 
    // find median of histogram 
    double prob5[] = {0.5}; 
    double q5[1]; 
    px5->GetQuantiles(1,q5,prob5);
    double median5 = q5[0];
    // find mode of histogram 
    double  mode5 = px5->GetBinCenter( px5->GetMaximumBin());
    std::cout << "histogram mode is " << mode5  << " median is " << median5 << std::endl;
    if (median5 < 0) { 
      Error("lognormal","not valid histogram median");
      return;
    }
    // m is log(median)
    p5[1] = std::log(median5); 
    // s2 is  log(median) - log(mode) 
    p5[2] = std::sqrt( std::log(median5/mode5) ); 
    f5->SetParameters(p5); 
    f5->SetParName(0,"A5");
    f5->SetParName(1,"m5");
    f5->SetParName(2,"s5");
    f5->SetLineColor(8);
    f5->SetLineWidth(1);
    f5->Print();
    std::cout << f5->GetNpar() << std::endl;
    px5->Draw("same");
    px5->Fit("f5","R");
    TLine *px5line = new TLine(f5->GetMaximumX(),200,f5->GetMaximumX(),f5->GetMaximum());
    px5line->SetLineColor(8);
    px5line->SetLineStyle(2);
    px5line->SetLineWidth(2);
    px5line->Draw("same");


    TF1 * f6 = new TF1("f6","[0]*ROOT::Math::lognormal_pdf(x,[1],[2] ) ",1.4,2.1 ); 
   // set initial parameters 
    double p6[3]; 
    p6[0] = px6->GetEntries()*px6->GetXaxis()->GetBinWidth(1);   // area of the histogram 
    // find median of histogram 
    double prob6[] = {0.5}; 
    double q6[1]; 
    px6->GetQuantiles(1,q6,prob6);
    double median6 = q6[0];
    // find mode of histogram 
    double  mode6 = px6->GetBinCenter( px6->GetMaximumBin());
    std::cout << "histogram mode is " << mode6  << " median is " << median6 << std::endl;
    if (median6 < 0) { 
      Error("lognormal","not valid histogram median");
      return;
    }
    // m is log(median)
    p6[1] = std::log(median6); 
    // s2 is  log(median) - log(mode) 
    p6[2] = std::sqrt( std::log(median6/mode6) ); 
    f6->SetParameters(p6); 
    f6->SetParName(0,"A6");
    f6->SetParName(1,"m6");
    f6->SetParName(2,"s6");
    f6->SetLineColor(kGray);
    f6->SetLineWidth(1);
    f6->Print();
    std::cout << f6->GetNpar() << std::endl;
    px6->Draw("same");
    px6->Fit("f6","R");
    hist_leg->Draw("same");
    TLine *px6line = new TLine(f6->GetMaximumX(),200,f6->GetMaximumX(),f6->GetMaximum());
    px6line->SetLineColor(kGray);
    px6line->SetLineStyle(2);
    px6line->SetLineWidth(2);
    px6line->Draw("same");



    // if (gaus){
    //   px1->Fit("f1","V");
    //   px1->GetFunction("f1")->SetLineColor(1);
    //   px1->GetFunction("f1")->SetLineWidth(1);
    //   // TLine *px1line = new TLine(px1->GetFunction("gaus")->GetParameter(1),100,px1->GetFunction("gaus")->GetParameter(1),px1->GetFunction("gaus")->GetMaximum());
    //   // px1line->SetLineColor(1);
    //   // px1line->SetLineStyle(2);

    //   px2->Fit("gaus");
    //   px2->GetFunction("gaus")->SetLineColor(2);
    //   px2->GetFunction("gaus")->SetLineWidth(1);
    //   TLine *px2line = new TLine(px2->GetFunction("gaus")->GetParameter(1),100,px2->GetFunction("gaus")->GetParameter(1),px2->GetFunction("gaus")->GetMaximum());
    //   px2line->SetLineColor(2);
    //   px2line->SetLineStyle(2);

    //   px3->Fit("gaus");
    //   px3->GetFunction("gaus")->SetLineColor(4);
    //   px3->GetFunction("gaus")->SetLineWidth(1);
    //   TLine *px3line = new TLine(px3->GetFunction("gaus")->GetParameter(1),100,px3->GetFunction("gaus")->GetParameter(1),px3->GetFunction("gaus")->GetMaximum());
    //   px3line->SetLineColor(4);
    //   px3line->SetLineStyle(2);

    //   px4->Fit("gaus");
    //   px4->GetFunction("gaus")->SetLineColor(6);
    //   px4->GetFunction("gaus")->SetLineWidth(1);
    //   TLine *px4line = new TLine(px4->GetFunction("gaus")->GetParameter(1),100,px4->GetFunction("gaus")->GetParameter(1),px4->GetFunction("gaus")->GetMaximum());
    //   px4line->SetLineColor(6);
    //   px4line->SetLineStyle(2);

    //   px5->Fit("gaus");
    //   px5->GetFunction("gaus")->SetLineColor(8);
    //   px5->GetFunction("gaus")->SetLineWidth(1);
    //   TLine *px5line = new TLine(px5->GetFunction("gaus")->GetParameter(1),100,px5->GetFunction("gaus")->GetParameter(1),px5->GetFunction("gaus")->GetMaximum());
    //   px5line->SetLineColor(8);
    //   px5line->SetLineStyle(2);

    //   px6->Fit("gaus");
    //   px6->GetFunction("gaus")->SetLineColor(kGray);
    //   px6->GetFunction("gaus")->SetLineWidth(1);
    //   TLine *px6line = new TLine(px6->GetFunction("gaus")->GetParameter(1),100,px6->GetFunction("gaus")->GetParameter(1),px6->GetFunction("gaus")->GetMaximum());
    //   px6line->SetLineColor(kGray);
    //   px6line->SetLineStyle(2);

    //   //px1line->Draw("same");
    //   px2line->Draw("same");
    //   px3line->Draw("same");
    //   px4line->Draw("same");
    //   px5line->Draw("same");
    //   px6line->Draw("same");
    // }

    //px1->GetYaxis()->SetRangeUser(100,5000);


   
    

    //Add some text
    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    //Tl.DrawLatexNDC(0.5,0.8,"Madgraph+Pythia8 Z/#gamma^{*}#rightarrow#tau#tau");
    //    Tl.DrawLatexNDC(0.5,0.75,Form("%i entries", NentriesPhiM));

    //save plot
    if (saveplots){
      c1->SaveAs("plots/20160613/DPhi_pT_fraction_slices_lognormal_fits_vert_zoom.pdf");
    }
    
  }

}

