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

void plotEff_newVars_reco(){

  TFile *f = new TFile("Ztautau2M.root");
  TFile *f1 = new TFile("Wplusmunu2M.root");

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
  
  //utility line
  TGraph *util	= new TGraph();
  for( int i = 1; i < 101; ++i ){
    util->SetPoint( i, (float)i / float(100), (float)i / (float)100 );
  }
  
  TH1F *allsig_h       	= (TH1F*)f->Get("361510/plotEventAlgs/taus/Mvis");
  TH1F *allbkg_h       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/Mvis");
  double allsig		= allsig_h->Integral();
  double allbkg		= allbkg_h->Integral();

  if( aT ){
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/AT");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/AT");
    int rebinval = 1;
    h->Rebin(rebinval);
    h->Scale(0.1 / h->Integral("width"));
    h2->Scale(0.1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kTRUE);
    TH1* hc2 = h2->GetCumulative(kTRUE);

    // now, fill the efficiency histogram
    int nbinsx = hc->GetXaxis()->GetNbins();
    for( int i = 0; i < nbinsx+1; ++i ){
      dt->SetPoint( i, hc->GetBinContent(i), hc2->GetBinContent(i));
    }
  }

  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/AT_add_DPhi");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/AT_add_DPhi");
    double scale_sig	= h->Integral() / allsig;
    double scale_bkg	= h2->Integral() / allbkg;
    int rebinval = 1;

    h->Rebin(rebinval);

    h->Scale(.1 / h->Integral("width"));
    h2->Scale(.1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kTRUE);
    TH1* hc2 = h2->GetCumulative(kTRUE);
    int nbinsx = hc->GetXaxis()->GetNbins();
    for( int i = 0; i < nbinsx+1; ++i ){
      dt2->SetPoint( i, hc->GetBinContent(i) * scale_sig,  hc2->GetBinContent(i) * scale_bkg);
    }

  }



  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/M_lh");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/M_lh");
    double scale_sig	= h->Integral() / allsig;
    double scale_bkg	= h2->Integral() / allbkg;
    int rebinval = 1;
    h->Rebin(rebinval);

    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kTRUE);
    TH1* hc2 = h2->GetCumulative(kTRUE);


    int nbinsx = hc->GetXaxis()->GetNbins();

    for( int i = 0; i < nbinsx+1; ++i ){
      dt3->SetPoint( i, hc->GetBinContent(i),  hc2->GetBinContent(i));
    }

  }

  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/Mstar3");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/Mstar3");
    int rebinval = 1;
    h->Rebin(rebinval);
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kTRUE);
    TH1* hc2 = h2->GetCumulative(kTRUE);
    int nbinsx = hc->GetXaxis()->GetNbins();
    for( int i = 0; i < nbinsx+1; ++i ){
      //dt4->SetPoint( i, hc->GetBinContent(i),  hc2->GetBinContent(i));
    }
  }


  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/MT");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/MT");
    int rebinval = 1;
    h->Rebin(rebinval);
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kTRUE);
    TH1* hc2 = h2->GetCumulative(kTRUE);
    int nbinsx = hc->GetXaxis()->GetNbins();
    for( int i = 0; i < nbinsx + 1; ++i ){
      dt5->SetPoint( i, hc->GetBinContent(i), hc2->GetBinContent(i));
    }
  }
  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/AT_add_MT2");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/AT_add_MT2");
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
      dt7->SetPoint( i, hc->GetBinContent(i) * scale_sig, hc2->GetBinContent(i) * scale_bkg);
    }
  }

  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/AT_add_Mstar2");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/AT_add_Mstar2");
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
      dt6->SetPoint( i, hc->GetBinContent(i) * scale_sig, hc2->GetBinContent(i) * scale_bkg);
    }
  }

  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/AT_add_Mstar3");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/AT_add_Mstar3");
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
      dt8->SetPoint( i, hc->GetBinContent(i) * scale_sig, hc2->GetBinContent(i) * scale_bkg);
    }
  }

  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/AT_add_Mvis");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/AT_add_Mvis");
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
      dt9->SetPoint( i, hc->GetBinContent(i) * scale_sig, hc2->GetBinContent(i) * scale_bkg);
    }
  }

  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("361510/plotEventAlgs/taus/SumCos");
    TH1F *h2       	= (TH1F*)f1->Get("361101/plotEventAlgs/taus/SumCos");
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
  }


  if(  eff ){
    TCanvas *c20 = new TCanvas("c20","c20",800,600);
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat("e"); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg1 = new TLegend(0.86,0.5,1,0.90);
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
    c20->SetLogy();
    //    c20->SetLogx();
    dt->GetXaxis()->SetTitle("#epsilon_{sig}");
    dt->GetYaxis()->SetTitle("#epsilon_{bkg}");
    dt->GetXaxis()->SetTitleSize(0.06);
    dt->GetYaxis()->SetTitleSize(0.06);
    dt->GetXaxis()->SetTitleOffset(0.8);
    dt->GetYaxis()->SetTitleOffset(0.8);
    dt->GetXaxis()->SetRangeUser(0,1.05);
    dt->GetYaxis()->SetRangeUser(0.008,1.05);
    dt->SetMarkerStyle(8);
    dt2->SetMarkerStyle(8);
    dt3->SetMarkerStyle(8);
    dt4->SetMarkerStyle(8);
    dt5->SetMarkerStyle(8);
    dt->SetMarkerSize(.3);
    dt2->SetMarkerSize(.3);
    dt3->SetMarkerSize(.3);
    dt->SetLineWidth(3);
    dt2->SetLineWidth(2);
    dt3->SetLineWidth(2);
    dt4->SetLineWidth(2);
    dt5->SetLineWidth(2);
    dt6->SetLineWidth(2);
    dt7->SetLineWidth(2);
    dt8->SetLineWidth(2);
    dt9->SetLineWidth(2);
    dt->SetLineColor(40);
    dt2->SetLineColor(46);
    dt3->SetLineColor(6);
    dt4->SetLineColor(2);
    dt5->SetLineColor(8);
    dt6->SetLineColor(7);
    dt7->SetLineColor(8);
    dt8->SetLineColor(4);
    dt9->SetLineColor(6);
    dt4->SetLineStyle(1);
    dt5->SetLineStyle(2);
    dt6->SetLineStyle(1);
    dt2->SetMarkerColor(4);
    util->SetLineColor(1);
    util->SetLineStyle(2);
    util->SetLineWidth(2);
    hist_leg1->AddEntry(dt, Form("a_{T}"), "l");
    hist_leg1->AddEntry(dt2, Form("a_{T}+#Delta#phi_{#slash{E}_{T},l}"),"l");
    hist_leg1->AddEntry(dt6, Form("a_{T}+M^{*}_{l#slash{E}_{T}}"),"l");
    hist_leg1->AddEntry(dt8, Form("a_{T}+M^{*}_{l#tau_{h}#slash{E}_{T}}"),"l");
    hist_leg1->AddEntry(dt7, Form("a_{T}+M_{T}^{l#slash{E}_{T}}"),"l");
    hist_leg1->AddEntry(dt9, Form("a_{T}+M_{vis}^{l#slash{E}_{T}}"),"l");
    hist_leg1->AddEntry(dt4, Form("#sumcos#Delta#phi"),"l");
    //hist_leg1->AddEntry(dt5, Form("M_{T}^{l#tau_{h}#slash{E}_{T}}"),"l");
    dt->Draw("AL");
    //    dt2->Draw("same L");
    dt2->Draw("same L");
    //dt5->Draw("same L");
    dt6->Draw("same L");
    dt8->Draw("same L");
    dt7->Draw("same L");
    dt9->Draw("same L");
    dt4->Draw("same L");
    util->Draw("same");
    hist_leg1->Draw("same");

    TLine line1(0.493,0,0.493,0.051);
    TLine line2(0,0.051,0.493,0.051);
    TLine line3(0.993,0,0.993,0.449);
    TLine line4(0,0.449,0.993,0.449);
    line1.Draw("same");
    line2.Draw("same");
    line3.Draw("same");
    line4.Draw("same");
    line1.SetLineStyle(4);
    line2.SetLineStyle(4);
    line3.SetLineStyle(4);
    line4.SetLineStyle(4);
    line1.SetLineColor(13);
    line2.SetLineColor(13);
    line3.SetLineColor(13);
    line4.SetLineColor(13);


    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.17,0.85,"Z#rightarrow#tau_{l}#tau_{h} (sig) vs W(#rightarrow#mu#nu)+jets (bkg)");
    Tl.DrawLatexNDC(0.2,0.33,"R_{1}");
    Tl.DrawLatexNDC(0.5,0.7,"R_{2}");
    Tl.DrawLatexNDC(0.7,0.8,"R_{3}");

    //save plot
    if (1==1){
      c20->SaveAs("plots/20161229/Eff_add_logy.pdf");
    }    

  }

}

