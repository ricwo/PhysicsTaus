#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
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

void plotROC(){

  TFile *f = new TFile("sherpa/Z+jets.root");
  TFile *f1 = new TFile("sherpa/W+jets.root");

  bool saveplots = false;
  bool aT	 = true;
  bool atdivphi	 = true;
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
  TGraph *dt13	 = new TGraph();
  TGraph *dt14	 = new TGraph();
  TGraph *dt15	 = new TGraph();
  
  //utility line
  TGraph *util	= new TGraph();
  for( int i = 1; i < 101; ++i ){
    util->SetPoint( i, (float)i / float(100), (float)i / (float)100 );
  }
  
  TH1F *allsig_h       	= (TH1F*)f->Get("Mvis");
  TH1F *allbkg_h       	= (TH1F*)f1->Get("Mvis");
  double allsig		= allsig_h->Integral();
  double allbkg		= allbkg_h->Integral();

  TH1F *atsig		= (TH1F*)f->Get("PassAtlasCuts");
  TH1F *atbkg       	= (TH1F*)f1->Get("PassAtlasCuts");

  TGraph *atlas = new TGraph();
  atlas->SetPoint(0, atsig->GetBinContent(2)/(atsig->GetBinContent(1)+atsig->GetBinContent(2)), atbkg->GetBinContent(2)/(atbkg->GetBinContent(1)+atbkg->GetBinContent(2)) );
  cout << "ATLAS point: ";
  atlas->Print();

TH1D *MVA_BDT_effBvsS = new TH1D("MVA_BDT_effBvsS","MVA_BDT",100,0,1);
   MVA_BDT_effBvsS->SetBinContent(1,5.148993e-06);
   MVA_BDT_effBvsS->SetBinContent(2,0.0001191319);
   MVA_BDT_effBvsS->SetBinContent(3,0.0003646999);
   MVA_BDT_effBvsS->SetBinContent(4,0.0003968731);
   MVA_BDT_effBvsS->SetBinContent(5,0.0004499842);
   MVA_BDT_effBvsS->SetBinContent(6,0.0006312247);
   MVA_BDT_effBvsS->SetBinContent(7,0.001053774);
   MVA_BDT_effBvsS->SetBinContent(8,0.001212393);
   MVA_BDT_effBvsS->SetBinContent(9,0.001357217);
   MVA_BDT_effBvsS->SetBinContent(10,0.001612154);
   MVA_BDT_effBvsS->SetBinContent(11,0.001965667);
   MVA_BDT_effBvsS->SetBinContent(12,0.002167311);
   MVA_BDT_effBvsS->SetBinContent(13,0.002651908);
   MVA_BDT_effBvsS->SetBinContent(14,0.002853393);
   MVA_BDT_effBvsS->SetBinContent(15,0.003071964);
   MVA_BDT_effBvsS->SetBinContent(16,0.003428454);
   MVA_BDT_effBvsS->SetBinContent(17,0.003524064);
   MVA_BDT_effBvsS->SetBinContent(18,0.003769131);
   MVA_BDT_effBvsS->SetBinContent(19,0.0041715);
   MVA_BDT_effBvsS->SetBinContent(20,0.004640292);
   MVA_BDT_effBvsS->SetBinContent(21,0.005006208);
   MVA_BDT_effBvsS->SetBinContent(22,0.00544224);
   MVA_BDT_effBvsS->SetBinContent(23,0.005830808);
   MVA_BDT_effBvsS->SetBinContent(24,0.006249569);
   MVA_BDT_effBvsS->SetBinContent(25,0.00658633);
   MVA_BDT_effBvsS->SetBinContent(26,0.006877057);
   MVA_BDT_effBvsS->SetBinContent(27,0.007351571);
   MVA_BDT_effBvsS->SetBinContent(28,0.007899889);
   MVA_BDT_effBvsS->SetBinContent(29,0.008344388);
   MVA_BDT_effBvsS->SetBinContent(30,0.009004913);
   MVA_BDT_effBvsS->SetBinContent(31,0.009408764);
   MVA_BDT_effBvsS->SetBinContent(32,0.009902366);
   MVA_BDT_effBvsS->SetBinContent(33,0.01032152);
   MVA_BDT_effBvsS->SetBinContent(34,0.01104943);
   MVA_BDT_effBvsS->SetBinContent(35,0.0116408);
   MVA_BDT_effBvsS->SetBinContent(36,0.01219806);
   MVA_BDT_effBvsS->SetBinContent(37,0.01289157);
   MVA_BDT_effBvsS->SetBinContent(38,0.01362596);
   MVA_BDT_effBvsS->SetBinContent(39,0.01424263);
   MVA_BDT_effBvsS->SetBinContent(40,0.01480101);
   MVA_BDT_effBvsS->SetBinContent(41,0.01557512);
   MVA_BDT_effBvsS->SetBinContent(42,0.01640648);
   MVA_BDT_effBvsS->SetBinContent(43,0.0171356);
   MVA_BDT_effBvsS->SetBinContent(44,0.01795731);
   MVA_BDT_effBvsS->SetBinContent(45,0.01905272);
   MVA_BDT_effBvsS->SetBinContent(46,0.01975506);
   MVA_BDT_effBvsS->SetBinContent(47,0.02053762);
   MVA_BDT_effBvsS->SetBinContent(48,0.0212325);
   MVA_BDT_effBvsS->SetBinContent(49,0.02209851);
   MVA_BDT_effBvsS->SetBinContent(50,0.02304704);
   MVA_BDT_effBvsS->SetBinContent(51,0.024176);
   MVA_BDT_effBvsS->SetBinContent(52,0.02503035);
   MVA_BDT_effBvsS->SetBinContent(53,0.02578809);
   MVA_BDT_effBvsS->SetBinContent(54,0.02686979);
   MVA_BDT_effBvsS->SetBinContent(55,0.02774339);
   MVA_BDT_effBvsS->SetBinContent(56,0.02915926);
   MVA_BDT_effBvsS->SetBinContent(57,0.03020152);
   MVA_BDT_effBvsS->SetBinContent(58,0.03107403);
   MVA_BDT_effBvsS->SetBinContent(59,0.03224592);
   MVA_BDT_effBvsS->SetBinContent(60,0.0335172);
   MVA_BDT_effBvsS->SetBinContent(61,0.03533867);
   MVA_BDT_effBvsS->SetBinContent(62,0.03715581);
   MVA_BDT_effBvsS->SetBinContent(63,0.03891174);
   MVA_BDT_effBvsS->SetBinContent(64,0.04077703);
   MVA_BDT_effBvsS->SetBinContent(65,0.04254619);
   MVA_BDT_effBvsS->SetBinContent(66,0.04442897);
   MVA_BDT_effBvsS->SetBinContent(67,0.04668682);
   MVA_BDT_effBvsS->SetBinContent(68,0.0487202);
   MVA_BDT_effBvsS->SetBinContent(69,0.0508586);
   MVA_BDT_effBvsS->SetBinContent(70,0.05305218);
   MVA_BDT_effBvsS->SetBinContent(71,0.05527572);
   MVA_BDT_effBvsS->SetBinContent(72,0.05754246);
   MVA_BDT_effBvsS->SetBinContent(73,0.0599047);
   MVA_BDT_effBvsS->SetBinContent(74,0.06251156);
   MVA_BDT_effBvsS->SetBinContent(75,0.06561637);
   MVA_BDT_effBvsS->SetBinContent(76,0.06937421);
   MVA_BDT_effBvsS->SetBinContent(77,0.07339774);
   MVA_BDT_effBvsS->SetBinContent(78,0.07750908);
   MVA_BDT_effBvsS->SetBinContent(79,0.08135196);
   MVA_BDT_effBvsS->SetBinContent(80,0.08604115);
   MVA_BDT_effBvsS->SetBinContent(81,0.09135769);
   MVA_BDT_effBvsS->SetBinContent(82,0.09715779);
   MVA_BDT_effBvsS->SetBinContent(83,0.1030014);
   MVA_BDT_effBvsS->SetBinContent(84,0.1093158);
   MVA_BDT_effBvsS->SetBinContent(85,0.1163746);
   MVA_BDT_effBvsS->SetBinContent(86,0.122545);
   MVA_BDT_effBvsS->SetBinContent(87,0.1315795);
   MVA_BDT_effBvsS->SetBinContent(88,0.1410529);
   MVA_BDT_effBvsS->SetBinContent(89,0.1531915);
   MVA_BDT_effBvsS->SetBinContent(90,0.1655334);
   MVA_BDT_effBvsS->SetBinContent(91,0.1794013);
   MVA_BDT_effBvsS->SetBinContent(92,0.2004183);
   MVA_BDT_effBvsS->SetBinContent(93,0.2190105);
   MVA_BDT_effBvsS->SetBinContent(94,0.2438273);
   MVA_BDT_effBvsS->SetBinContent(95,0.2738242);
   MVA_BDT_effBvsS->SetBinContent(96,0.3136991);
   MVA_BDT_effBvsS->SetBinContent(97,0.364692);
   MVA_BDT_effBvsS->SetBinContent(98,0.4393408);
   MVA_BDT_effBvsS->SetBinContent(99,0.5503016);
   MVA_BDT_effBvsS->SetBinContent(100,0.7546117);
   MVA_BDT_effBvsS->SetMinimum(-0.07114573);
   MVA_BDT_effBvsS->SetMaximum(0.06084605);
   MVA_BDT_effBvsS->SetEntries(100);
   MVA_BDT_effBvsS->SetStats(0);
   MVA_BDT_effBvsS->SetLineWidth(2);
   MVA_BDT_effBvsS->SetLineColor(1);
   MVA_BDT_effBvsS->SetLineStyle(2);
 
  if( aT ){
    TH1F *h		= (TH1F*)f->Get("AT");
    TH1F *h2       	= (TH1F*)f1->Get("AT");
    int rebinval = 1;
    h->Rebin(rebinval);
    h->Scale(.1 / h->Integral("width"));
    h2->Scale(.1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kTRUE);
    TH1* hc2 = h2->GetCumulative(kTRUE);
    int nbinsx = hc->GetXaxis()->GetNbins();
    for( int i = 0; i < nbinsx+1; ++i ){
      //dt->SetPoint( i, hc->GetBinContent(i), hc2->GetBinContent(i));
    }
    delete h; delete h2; delete hc; delete hc2;
  }

  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("DPhi_match");
    TH1F *h2       	= (TH1F*)f1->Get("DPhi_match");
    double scale_sig	= h->Integral() / allsig;
    double scale_bkg	= h2->Integral() / allbkg;
    int rebinval = 1;

    h->Rebin(rebinval);

    h->Scale(.001 / h->Integral("width"));
    h2->Scale(.001 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kTRUE);
    TH1* hc2 = h2->GetCumulative(kTRUE);
    int nbinsx = hc->GetXaxis()->GetNbins();
    for( int i = 0; i < nbinsx+1; ++i ){
      dt2->SetPoint( i, hc->GetBinContent(i) * scale_sig,  hc2->GetBinContent(i) * scale_bkg);
    }
    delete h; delete h2; delete hc; delete hc2;
  }

  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("MT_MET_l");
    TH1F *h2       	= (TH1F*)f1->Get("MT_MET_l");
    double scale_sig	= h->Integral() / allsig;
    double scale_bkg	= h2->Integral() / allbkg;
    int rebinval = 1;
    h->Rebin(rebinval);
    h->Scale(.1 / h->Integral("width"));
    h2->Scale(.1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kTRUE);
    TH1* hc2 = h2->GetCumulative(kTRUE);
    int nbinsx = hc->GetXaxis()->GetNbins();
    for( int i = 0; i < nbinsx + 1; ++i ){
      dt7->SetPoint( i, hc->GetBinContent(i) * scale_sig, hc2->GetBinContent(i) * scale_bkg );
    }
    delete h; delete h2; delete hc; delete hc2;
  }

  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("Mstar2");
    TH1F *h2       	= (TH1F*)f1->Get("Mstar2");
    double scale_sig	= h->Integral() / allsig;
    double scale_bkg	= h2->Integral() / allbkg;
    int rebinval = 1;
    h->Rebin(rebinval);
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kTRUE);
    TH1* hc2 = h2->GetCumulative(kTRUE);
    int nbinsx = hc->GetXaxis()->GetNbins();
    bool flag1 = true; bool flag2 = true;
    for( int i = 0; i < nbinsx + 1; ++i ){
      dt6->SetPoint( i, hc->GetBinContent(i) * scale_sig, hc2->GetBinContent(i) * scale_bkg);
      if(  hc->GetBinContent(i) > .6 && flag1 ){
      }
    }
    delete h; delete h2; delete hc; delete hc2;
  }

  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("Mstar3");
    TH1F *h2       	= (TH1F*)f1->Get("Mstar3");
    double scale_sig	= h->Integral() / allsig;
    double scale_bkg	= h2->Integral() / allbkg;
    int rebinval = 1;
    h->Rebin(rebinval);
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kTRUE);
    TH1* hc2 = h2->GetCumulative(kTRUE);
    int nbinsx = hc->GetXaxis()->GetNbins();
    for( int i = 0; i < nbinsx + 1; ++i ){
      dt8->SetPoint( i, hc->GetBinContent(i) * scale_sig, hc2->GetBinContent(i) * scale_bkg);
    }
    delete h; delete h2; delete hc; delete hc2;
  }

  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("Mvis");
    TH1F *h2       	= (TH1F*)f1->Get("Mvis");
    double scale_sig	= h->Integral() / allsig;
    double scale_bkg	= h2->Integral() / allbkg;
    TH1* hc = h->GetCumulative(kTRUE);
    TH1* hc2 = h2->GetCumulative(kTRUE);
    int nbinsx = hc->GetXaxis()->GetNbins();
    for( int i = 1; i < 200; ++i ){
      dt->SetPoint( i, h->Integral(70-i,70+i)/allsig, h2->Integral(70-i,70+i)/allbkg);
    }
    delete h; delete h2; delete hc; delete hc2;
  }

  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("SumCos");
    TH1F *h2       	= (TH1F*)f1->Get("SumCos");
    int rebinval = 1;
    h->Rebin(rebinval);
    h->Scale(.01 / h->Integral("width"));
    h2->Scale(.01 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kTRUE);
    TH1* hc2 = h2->GetCumulative(kTRUE);
    int nbinsx = hc->GetXaxis()->GetNbins();
    for( int i = 0; i < nbinsx+1; ++i ){
      dt4->SetPoint( i, 1 - hc->GetBinContent(i), 1 - hc2->GetBinContent(i));
    }
    delete h; delete h2; delete hc; delete hc2;
  }

  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("m2Hat");
    TH1F *h2       	= (TH1F*)f1->Get("m2Hat");
    int rebinval = 1;
    h->Rebin(rebinval);
    h->Scale(1. / h->Integral("width"));
    h2->Scale(1. / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kTRUE);
    TH1* hc2 = h2->GetCumulative(kTRUE);
    int nbinsx = hc->GetXaxis()->GetNbins();
    for( int i = 0; i < nbinsx+1; ++i ){
      dt11->SetPoint( i, hc->GetBinContent(i), hc2->GetBinContent(i));
    }
    delete h; delete h2; delete hc; delete hc2;
  }

  if( atdivphi ){
    TH2D *h		= (TH2D*)f->Get("PtAsymvsMstar2");
    TH2D *h2       	= (TH2D*)f1->Get("PtAsymvsMstar2");
    int nbinsy = h->GetYaxis()->GetNbins();
    double nsig = h->Integral();
    double nbkg = h2->Integral();
    for( int i = 1; i <nbinsy+1; ++i ){
      dt15->SetPoint( i, h->Integral(-1,-1,nbinsy-i,nbinsy)/nsig, h2->Integral(-1,-1,nbinsy-i,nbinsy)/nbkg);
    }
    //    dt14->Print();
    delete h; delete h2;
  }

  if(  eff ){
    TCanvas *c20 = new TCanvas("c20","c20",800,600);
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat("e"); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg1 = new TLegend(0.81,0.4,.95,0.95);
    hist_leg1->SetFillColor(0);
    hist_leg1->SetLineColor(1);
    hist_leg1->SetShadowColor(0);
    hist_leg1->SetTextSize(0.032);
    hist_leg1->SetTextFont(62);
    hist_leg1->SetFillStyle(0);
    hist_leg1->SetBorderSize(0);
    c20->SetRightMargin(0.2);
    c20->SetLeftMargin(0.1);
    c20->SetTopMargin(0.05);
    c20->SetBottomMargin(0.1);
    //c20->SetLogy();
    //    c20->SetLogx();
    dt->GetXaxis()->SetTitle("#epsilon_{sig}");
    dt->GetYaxis()->SetTitle("#epsilon_{bkg}");
    dt->GetXaxis()->SetTitleSize(0.06);
    dt->GetYaxis()->SetTitleSize(0.06);
    dt->GetXaxis()->SetTitleOffset(.72);
    dt->GetYaxis()->SetTitleOffset(.75);
    dt->GetXaxis()->SetRangeUser(0.0,.49);
    dt->GetYaxis()->SetRangeUser(0.,0.03);
    dt->SetMarkerStyle(8);
    dt2->SetMarkerStyle(8);
    dt3->SetMarkerStyle(8);
    dt4->SetMarkerStyle(8);
    dt5->SetMarkerStyle(8);
    dt->SetMarkerSize(.3);
    dt2->SetMarkerSize(.3);
    dt3->SetMarkerSize(.3);
    dt->SetLineWidth(3);
    dt2->SetLineWidth(3);
    dt3->SetLineWidth(3);
    dt4->SetLineWidth(3);
    dt5->SetLineWidth(3);
    dt6->SetLineWidth(3);
    dt7->SetLineWidth(3);
    dt8->SetLineWidth(3);
    dt9->SetLineWidth(3);
    dt10->SetLineWidth(3);
    dt11->SetLineWidth(3);
    dt12->SetLineWidth(3);
    dt13->SetLineWidth(3);
    dt14->SetLineWidth(3);
    dt15->SetLineWidth(3);
    dt->SetLineColor(40);
    dt2->SetLineColor(46);
    dt3->SetLineColor(6);
    dt4->SetLineColor(2);
    dt5->SetLineColor(41);
    dt6->SetLineColor(6);
    dt7->SetLineColor(8);
    dt8->SetLineColor(4);
    dt9->SetLineColor(6);
    dt10->SetLineColor(1);
    dt11->SetLineColor(7);
    dt12->SetLineColor(42);
    dt13->SetLineColor(38);
    dt14->SetLineColor(28);
    dt15->SetLineColor(28);
    dt4->SetLineStyle(1);
    dt5->SetLineStyle(1);
    dt6->SetLineStyle(1);
    dt15->SetLineStyle(1);
    dt14->SetLineStyle(2);
    dt2->SetMarkerColor(4);
    util->SetLineColor(1);
    util->SetLineStyle(2);
    util->SetLineWidth(1);
    atlas->SetMarkerStyle(3);
    atlas->SetMarkerColor(1);
    atlas->SetMarkerSize(1.5);
    //    hist_leg1->AddEntry(dt, Form("a_{T}"), "l");
    hist_leg1->AddEntry(dt, Form("m_{vis}(#mu,#tau_{had})"),"l");
    hist_leg1->AddEntry(dt11, Form("test"),"l");
    hist_leg1->AddEntry(dt7, Form("m_{T}(#mu,E_{T}^{miss})"),"l");

    //hist_leg1->AddEntry(dt2, Form("#Delta#phi(E_{T}^{miss},#mu)"),"l");
    hist_leg1->AddEntry(dt6, Form("m^{*}(#mu,E_{T}^{miss})"),"l");
    hist_leg1->AddEntry(dt8, Form("m^{*}(#mu,#tau_{had},E_{T}^{miss})"),"l");
    hist_leg1->AddEntry(dt4, Form("SumCos"),"l");
    //hist_leg1->AddEntry(dt10, Form("#phi^{*}_{#eta}"),"l");

    //hist_leg1->AddEntry(dt12, Form("m^{*}(#mu,E_{T}^{miss})/p_{T}^{#tau}"),"l");
    //hist_leg1->AddEntry(dt13, Form("m^{*}_{rem}"),"l");
    //hist_leg1->AddEntry(dt14, Form("p_{T}^{#mu}"),"l");
    hist_leg1->AddEntry(dt15, Form("#rho"),"l");
    //hist_leg1->AddEntry(MVA_BDT_effBvsS, Form("BDT"),"l");
    hist_leg1->AddEntry(atlas, Form("ATLAS"),"p");
    dt->Draw("AL");
    //dt2->Draw("same L");
    dt6->Draw("same L");
    dt8->Draw("same L");
    dt7->Draw("same L");
    //dt9->Draw("same L");
    dt4->Draw("same L");
    //dt10->Draw("same L");
    dt11->Draw("same L");
    //dt12->Draw("same L");    
    //dt13->Draw("same L");
    //dt14->Draw("same L");
    dt15->Draw("same L");
    //MVA_BDT_effBvsS->Draw("same L");
    util->Draw("same");
    atlas->Draw("same P");
    hist_leg1->Draw("same");

    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.14,0.9,"Z#rightarrow#tau#tau (sig) vs W(#rightarrowl#nu)+jets (bkg), #mu#tau_{had} final state");

    //save plot
    if (1==1){
      c20->SaveAs("plots/20170624/ROC.pdf");
    }    

  }

}

