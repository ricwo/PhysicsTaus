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
  TGraph *dt14	 = new TGraph();
  TGraph *dt15	 = new TGraph();
  
  //utility line
  TGraph *util	= new TGraph();
  for( int i = 1; i < 101; ++i ){
    util->SetPoint( i, (float)i / float(100), (float)i / (float)100 / sqrt((float)i / (float)100) );
  }
  
  TH1F *allsig_h       	= (TH1F*)f->Get("Mvis");
  TH1F *allbkg_h       	= (TH1F*)f1->Get("Mvis");
  double allsig		= allsig_h->Integral();
  double allbkg		= allbkg_h->Integral();

  TH1F *atsig		= (TH1F*)f->Get("PassAtlasCuts");
  TH1F *atbkg       	= (TH1F*)f1->Get("PassAtlasCuts");

  TGraph *atlas = new TGraph();
  atlas->SetPoint(0, atsig->GetBinContent(2)/(atsig->GetBinContent(1)+atsig->GetBinContent(2)), atsig->GetBinContent(2)/(atsig->GetBinContent(1)+atsig->GetBinContent(2))/sqrt(atbkg->GetBinContent(2)/(atbkg->GetBinContent(1)+atbkg->GetBinContent(2))) );
  cout << "ATLAS point: ";
  atlas->Print();

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
      dt2->SetPoint( i, hc->GetBinContent(i) * scale_sig,  hc->GetBinContent(i) * scale_sig / sqrt(hc2->GetBinContent(i) * scale_bkg));
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
      if( hc2->GetBinContent(i) * scale_bkg > 0 ) dt7->SetPoint( i, hc->GetBinContent(i) * scale_sig, hc->GetBinContent(i) * scale_sig / sqrt(hc2->GetBinContent(i) * scale_bkg ));
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
      if(hc2->GetBinContent(i)>0) dt6->SetPoint( i, hc->GetBinContent(i) * scale_sig, hc->GetBinContent(i) * scale_sig / sqrt(hc2->GetBinContent(i) * scale_bkg));
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
      if( hc2->GetBinContent(i) * scale_bkg > 0 ) dt8->SetPoint( i, hc->GetBinContent(i) * scale_sig, hc->GetBinContent(i) * scale_sig / sqrt(hc2->GetBinContent(i) * scale_bkg));
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
      dt->SetPoint( i, h->Integral(70-i,70+i)/allsig, h->Integral(70-i,70+i)/allsig / sqrt(h2->Integral(70-i,70+i)/allbkg));
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
      dt4->SetPoint( i, 1 - hc->GetBinContent(i), (1 - hc->GetBinContent(i))/sqrt(1 - hc2->GetBinContent(i)));
    }
    delete h; delete h2; delete hc; delete hc2;
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
    delete h; delete h2; delete hc; delete hc2;
  }

  if( atdivphi ){
    TH2D *h		= (TH2D*)f->Get("PtAsymvsMstar2");
    TH2D *h2       	= (TH2D*)f1->Get("PtAsymvsMstar2");
    int nbinsy = h->GetYaxis()->GetNbins();
    double nsig = h->Integral();
    double nbkg = h2->Integral();
    for( int i = 1; i <nbinsy+1; ++i ){
      if(h2->Integral(-1,-1,nbinsy-i,nbinsy)/nbkg>0) dt15->SetPoint( i, h->Integral(-1,-1,nbinsy-i,nbinsy)/nsig, (h->Integral(-1,-1,nbinsy-i,nbinsy)/nsig)/sqrt(h2->Integral(-1,-1,nbinsy-i,nbinsy)/nbkg));
    }
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
    c20->SetLeftMargin(0.12);
    c20->SetTopMargin(0.05);
    c20->SetBottomMargin(0.11);
    //c20->SetLogy();
    //    c20->SetLogx();
    dt->GetXaxis()->SetTitle("#epsilon_{sig}");
    dt->GetYaxis()->SetTitle("#epsilon_{sig}/#sqrt{#epsilon_{bkg}}");
    dt->GetXaxis()->SetTitleSize(0.06);
    dt->GetYaxis()->SetTitleSize(0.06);
    dt->GetXaxis()->SetTitleOffset(.72);
    dt->GetYaxis()->SetTitleOffset(.75);
    //    dt->GetXaxis()->SetRangeUser(0.0,1);
    dt->GetYaxis()->SetRangeUser(0.,3.4);
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
    hist_leg1->AddEntry(dt, Form("m_{vis}(#mu,#tau_{had})"),"l");
    hist_leg1->AddEntry(dt11, Form("#phi_{acop}(#mu,#tau_{had})"),"l");
    hist_leg1->AddEntry(dt7, Form("m_{T}(#mu,E_{T}^{miss})"),"l");
    hist_leg1->AddEntry(dt6, Form("m^{*}(#mu,E_{T}^{miss})"),"l");
    hist_leg1->AddEntry(dt8, Form("m^{*}(#mu,#tau_{had},E_{T}^{miss})"),"l");
    hist_leg1->AddEntry(dt4, Form("SumCos"),"l");
    hist_leg1->AddEntry(dt15, Form("#rho"),"l");
    hist_leg1->AddEntry(atlas, Form("ATLAS"),"p");
    cout << dt->GetX()[TMath::LocMax(dt->GetN(),dt->GetY())]<< " " << dt->GetY()[TMath::LocMax(dt->GetN(),dt->GetY())] << endl;
    cout << dt11->GetX()[TMath::LocMax(dt11->GetN(),dt11->GetY())]<< " " << dt11->GetY()[TMath::LocMax(dt11->GetN(),dt11->GetY())] << endl;
    cout << dt7->GetX()[TMath::LocMax(dt7->GetN(),dt7->GetY())]<< " " << dt7->GetY()[TMath::LocMax(dt7->GetN(),dt7->GetY())] << endl;
    cout << dt6->GetX()[TMath::LocMax(dt6->GetN(),dt6->GetY())]<< " " << dt6->GetY()[TMath::LocMax(dt6->GetN(),dt6->GetY())] << endl;
    cout << dt8->GetX()[TMath::LocMax(dt8->GetN(),dt8->GetY())]<< " " << dt8->GetY()[TMath::LocMax(dt8->GetN(),dt8->GetY())] << endl;
    cout << dt4->GetX()[TMath::LocMax(dt4->GetN(),dt4->GetY())]<< " " << dt4->GetY()[TMath::LocMax(dt4->GetN(),dt4->GetY())] << endl;
    cout << dt15->GetX()[TMath::LocMax(dt15->GetN(),dt15->GetY())]<< " " << dt15->GetY()[TMath::LocMax(dt15->GetN(),dt15->GetY())] << endl;
    cout << atlas->GetX()[TMath::LocMax(atlas->GetN(),atlas->GetY())]<< " " << atlas->GetY()[TMath::LocMax(atlas->GetN(),atlas->GetY())] << endl;
    dt->Draw("AL");
    dt6->Draw("same L");
    dt8->Draw("same L");
    dt7->Draw("same L");
    dt4->Draw("same L");
    dt11->Draw("same L");
    dt15->Draw("same L");
    util->Draw("same");
    atlas->Draw("same P");
    hist_leg1->Draw("same");

    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.14,0.9,"Z#rightarrow#tau#tau (sig) vs W(#rightarrowl#nu)+jets (bkg), #mu#tau_{had} final state");

    //save plot
    if (1==1){
      c20->SaveAs("plots/20170508/SIC_primary.pdf");
    }    

  }

}

