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

void plotSICprimary(){

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
  double atlasx = atsig->GetBinContent(2)/(atsig->GetBinContent(1)+atsig->GetBinContent(2));
  double atlasy = atbkg->GetBinContent(2)/(atbkg->GetBinContent(1)+atbkg->GetBinContent(2));
  atlas->SetPoint(0, atlasx, atlasx / sqrt(atlasy));
 
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
      if( hc2->GetBinContent(i) > 0 ) dt->SetPoint( i, hc->GetBinContent(i), hc->GetBinContent(i)/sqrt(hc2->GetBinContent(i)));
    }
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
      if( hc2->GetBinContent(i) > 0 ) dt2->SetPoint( i, hc->GetBinContent(i) * scale_sig,  hc->GetBinContent(i) * scale_sig/sqrt(hc2->GetBinContent(i) * scale_bkg));
    }
  }
  
  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("M_lh");//M_lh
    TH1F *h2       	= (TH1F*)f1->Get("M_lh");
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
      if( hc2->GetBinContent(i) > 0 ) dt3->SetPoint( i, hc->GetBinContent(i),  hc->GetBinContent(i)/sqrt(hc2->GetBinContent(i)));
    }

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
      if( hc2->GetBinContent(i) > 0 ) dt7->SetPoint( i, hc->GetBinContent(i) * scale_sig, hc->GetBinContent(i) * scale_sig / sqrt(hc2->GetBinContent(i) * scale_bkg) );
    }
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
      if( hc2->GetBinContent(i) > 0 ) dt6->SetPoint( i, hc->GetBinContent(i) * scale_sig, hc->GetBinContent(i) * scale_sig / sqrt(hc2->GetBinContent(i) * scale_bkg));
      if(  hc->GetBinContent(i) > .6 && flag1 ){
	cout << i << endl; flag1 = false;
	cout << hc->GetBinContent(i) * scale_sig << endl;
	cout << hc2->GetBinContent(i) * scale_bkg << endl;
      }
    }
  }

  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("Mstar3");
    TH1F *h2       	= (TH1F*)f1->Get("Mstar3");
    double scale_sig	= h->Integral() / allsig;
    double scale_bkg	= h2->Integral() / allbkg;
    int rebinval = 1;
    h->Rebin(rebinval);
    h->Scale(.5 / h->Integral("width"));
    h2->Scale(.5 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kTRUE);
    TH1* hc2 = h2->GetCumulative(kTRUE);
    int nbinsx = hc->GetXaxis()->GetNbins();
    for( int i = 0; i < nbinsx + 1; ++i ){
      if( hc2->GetBinContent(i) > 0 ) dt8->SetPoint( i, hc->GetBinContent(i) * scale_sig, hc->GetBinContent(i) * scale_sig / sqrt(hc2->GetBinContent(i) * scale_bkg));
    }
  }

  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("Mvis");
    TH1F *h2       	= (TH1F*)f1->Get("Mvis");
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
      if( hc2->GetBinContent(i) > 0 ) dt9->SetPoint( i, hc->GetBinContent(i) * scale_sig, hc->GetBinContent(i) * scale_sig/sqrt(hc2->GetBinContent(i) * scale_bkg));
    }
  }

  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("SumCos");
    TH1F *h2       	= (TH1F*)f1->Get("SumCos");
    int rebinval = 1;
    h->Rebin(rebinval);
    h->Scale(.01 / h->Integral("width"));
    h2->Scale(.01 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kFALSE);
    TH1* hc2 = h2->GetCumulative(kFALSE);
    int nbinsx = hc->GetXaxis()->GetNbins();
    for( int i = nbinsx; i > 0; --i ){
      if( hc2->GetBinContent(i) > 0 ) dt4->SetPoint( nbinsx-i, hc->GetBinContent(i), (hc->GetBinContent(i))/sqrt(hc2->GetBinContent(i)));
    }
  }


  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("PhiStar");
    TH1F *h2       	= (TH1F*)f1->Get("PhiStar");
    int rebinval = 1;
    h->Rebin(rebinval);
    h->Scale(.01 / h->Integral("width"));
    h2->Scale(.01 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kTRUE);
    TH1* hc2 = h2->GetCumulative(kTRUE);
    int nbinsx = hc->GetXaxis()->GetNbins();
    for( int i = 0; i < nbinsx+1; ++i ){
      if( hc2->GetBinContent(i) > 0 ) dt10->SetPoint( i, hc->GetBinContent(i), hc->GetBinContent(i)/sqrt(hc2->GetBinContent(i)));
    }
  }


  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("Phi_acop_semilep_bkg");
    TH1F *h2       	= (TH1F*)f1->Get("Phi_acop_semilep_bkg");
    int rebinval = 1;
    h->Rebin(rebinval);
    h->Scale(.01 / h->Integral("width"));
    h2->Scale(.01 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kTRUE);
    TH1* hc2 = h2->GetCumulative(kTRUE);
    int nbinsx = hc->GetXaxis()->GetNbins();
    for( int i = 0; i < nbinsx+1; ++i ){
      if( hc2->GetBinContent(i) > 0 ) dt11->SetPoint( i, hc->GetBinContent(i), hc->GetBinContent(i)/sqrt(hc2->GetBinContent(i)));
    }
  }

  if( atdivphi ){
    TH1F *h		= (TH1F*)f->Get("Mstar2DivPt");
    TH1F *h2       	= (TH1F*)f1->Get("Mstar2DivPt");
    int rebinval = 1;
    h->Rebin(rebinval);
    h->Scale(.1 / h->Integral("width"));
    h2->Scale(.1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kTRUE);
    TH1* hc2 = h2->GetCumulative(kTRUE);
    int nbinsx = hc->GetXaxis()->GetNbins();
    for( int i = 0; i < nbinsx+1; ++i ){
      if( hc2->GetBinContent(i) > 0 ) dt12->SetPoint( i, hc->GetBinContent(i), hc->GetBinContent(i)/sqrt(hc2->GetBinContent(i)));
    }
  }

  if( atdivphi ){
    TH2D *hsig		= (TH2D*)f->Get("MremstarvsMstar2");
    TH2D *hbkg       	= (TH2D*)f1->Get("MremstarvsMstar2");
    TH1D* h = hsig->ProjectionY("h");
    TH1D* h2 = hbkg->ProjectionY("h2");
    h->Scale(1 / h->Integral("width"));
    h2->Scale(1 / h2->Integral("width"));
    TH1* hc = h->GetCumulative(kTRUE);
    TH1* hc2 = h2->GetCumulative(kTRUE);
    int nbinsx = hc->GetXaxis()->GetNbins();
    for( int i = 0; i < nbinsx+1; ++i ){
      if( hc2->GetBinContent(i) > 0 ) dt13->SetPoint( i, hc->GetBinContent(i), hc->GetBinContent(i)/sqrt(hc2->GetBinContent(i)));
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
    hist_leg1 = new TLegend(0.83,0.15,1,0.90);
    hist_leg1->SetFillColor(0);
    hist_leg1->SetLineColor(1);
    hist_leg1->SetShadowColor(0);
    hist_leg1->SetTextSize(0.03);
    hist_leg1->SetTextFont(62);
    hist_leg1->SetFillStyle(0);
    hist_leg1->SetBorderSize(0);
    c20->SetRightMargin(0.18);
    c20->SetLeftMargin(0.14);
    c20->SetBottomMargin(0.14);
    //c20->SetLogy();
    //    c20->SetLogx();
    dt->GetXaxis()->SetTitle("#epsilon_{sig}");
    dt->GetYaxis()->SetTitle("#epsilon_{sig}/#sqrt{#epsilon_{bkg}}");
    dt->GetXaxis()->SetTitleSize(0.06);
    dt->GetYaxis()->SetTitleSize(0.06);
    dt->GetXaxis()->SetTitleOffset(0.8);
    dt->GetYaxis()->SetTitleOffset(0.8);
    //    dt->GetXaxis()->SetRangeUser(0.0,.49);
    dt->GetYaxis()->SetRangeUser(0.,3.4);
    dt->SetMarkerStyle(8);
    dt2->SetMarkerStyle(8);
    dt3->SetMarkerStyle(8);
    dt4->SetMarkerStyle(8);
    dt5->SetMarkerStyle(8);
    dt->SetMarkerSize(.3);
    dt2->SetMarkerSize(.3);
    dt3->SetMarkerSize(.3);
    dt->SetLineWidth(2);
    dt2->SetLineWidth(2);
    dt3->SetLineWidth(2);
    dt4->SetLineWidth(2);
    dt5->SetLineWidth(2);
    dt6->SetLineWidth(2);
    dt7->SetLineWidth(2);
    dt8->SetLineWidth(2);
    dt9->SetLineWidth(2);
    dt10->SetLineWidth(2);
    dt11->SetLineWidth(2);
    dt12->SetLineWidth(2);
    dt13->SetLineWidth(2);
    dt->SetLineColor(40);
    dt2->SetLineColor(46);
    dt3->SetLineColor(6);
    dt4->SetLineColor(2);
    dt5->SetLineColor(41);
    dt6->SetLineColor(7);
    dt7->SetLineColor(8);
    dt8->SetLineColor(4);
    dt9->SetLineColor(6);
    dt10->SetLineColor(1);
    dt11->SetLineColor(9);
    dt12->SetLineColor(42);
    dt13->SetLineColor(38);
    dt4->SetLineStyle(1);
    dt5->SetLineStyle(1);
    dt6->SetLineStyle(1);
    dt2->SetMarkerColor(4);
    util->SetLineColor(1);
    util->SetLineStyle(2);
    util->SetLineWidth(2);
    atlas->SetMarkerStyle(22);
    atlas->SetMarkerColor(2);
    atlas->SetMarkerSize(1.2);
    hist_leg1->AddEntry(dt, Form("a_{T}"), "l");
    hist_leg1->AddEntry(dt4, Form("SumCos"),"l");
    hist_leg1->AddEntry(dt2, Form("#Delta#phi(#slash{E}_{T},l)"),"l");
    hist_leg1->AddEntry(dt6, Form("M^{*}_{l#slash{E}_{T}}"),"l");
    hist_leg1->AddEntry(dt8, Form("M^{*}_{l#tau_{h}#slash{E}_{T}}"),"l");
    hist_leg1->AddEntry(dt7, Form("M_{T}^{l#slash{E}_{T}}"),"l");
    hist_leg1->AddEntry(dt9, Form("M_{vis}^{l#slash{E}_{T}}"),"l");
    hist_leg1->AddEntry(dt10, Form("#phi^{*}_{#eta}"),"l");
    hist_leg1->AddEntry(dt11, Form("#phi_{acop}(#tau,l)"),"l");
    hist_leg1->AddEntry(dt12, Form("M^{*}_{l#slash{E}_{T}}/p_{T}^{#tau}"),"l");
    hist_leg1->AddEntry(dt13, Form("M^{*}_{rem}"),"l");
    hist_leg1->AddEntry(atlas, Form("ATLAS cuts"),"p");
    dt->Draw("AL");
    dt2->Draw("same L");
    dt6->Draw("same L");
    dt8->Draw("same L");
    dt7->Draw("same L");
    dt9->Draw("same L");
    dt4->Draw("same C");
    dt10->Draw("same L");
    dt11->Draw("same L");
    dt12->Draw("same L");    
    dt13->Draw("same L");
    atlas->Draw("same P");
    hist_leg1->Draw("same");

    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.03);
    Tl.DrawLatexNDC(0.17,0.85,"SHERPA Z#rightarrow#tau#tau (sig) vs W(#rightarrow#mu#nu)+jets (bkg), #muh final state");

    //save plot
    if (1==1){
      c20->SaveAs("plots/20170508/SIC_primary.pdf");
    }    

  }

}

