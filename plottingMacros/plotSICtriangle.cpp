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

void plotSICtriangle(){

  TFile *f = new TFile("sherpa/Z+jets.root");
  TFile *f1 = new TFile("sherpa/W+jets.root");

  bool saveplots = false;
  bool aT	 = true;
  bool atdivphi	 = true;
  bool eff	 = true;
  TGraph *dt	 = new TGraph();
  TGraph *dt2	 = new TGraph();
  TGraph *dt3	 = new TGraph();
  TGraph *dt4	 = new TGraph();
  
  //utility line
  TGraph *util	= new TGraph();
  for( int i = 1; i < 101; ++i ){
    util->SetPoint( i, (float)i / float(100), (float)i / float(100)/sqrt((float)i / (float)100) );
  }
  
  TH1F *allsig_h       	= (TH1F*)f->Get("Mvis");
  TH1F *allbkg_h       	= (TH1F*)f1->Get("Mvis");
  double allsig		= allsig_h->Integral();
  double allbkg		= allbkg_h->Integral();
  double allsigsuper = allsig;
  double allbkgsuper = allbkg;
  cout << allsig << " " << allbkg << " allsig allbkg" << endl;

  TH1F *atsig		= (TH1F*)f->Get("PassAtlasCuts");
  TH1F *atbkg       	= (TH1F*)f1->Get("PassAtlasCuts");

  TGraph *atlas = new TGraph();
  atlas->SetPoint(0, atsig->GetBinContent(2)/(atsig->GetBinContent(1)+atsig->GetBinContent(2)), atsig->GetBinContent(2)/(atsig->GetBinContent(1)+atsig->GetBinContent(2)) / sqrt(atbkg->GetBinContent(2)/(atbkg->GetBinContent(1)+atbkg->GetBinContent(2))) );
  cout << "ATLAS point: ";
  atlas->Print();

   TH1D *MVA_BDT_50trees_10percentnode_beta015_effBvsS92 = new TH1D("MVA_BDT_50trees_10percentnode_beta015_effBvsS92","MVA_BDT_50trees_10percentnode_beta015",100,0,1);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(1,0.0003518624);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(2,0.0003518624);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(3,0.0003838547);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(4,0.0005619893);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(5,0.000739343);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(6,0.0009166967);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(7,0.001136525);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(8,0.001366526);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(9,0.001631825);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(10,0.00195423);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(11,0.002186563);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(12,0.002362149);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(13,0.002669638);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(14,0.003043876);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(15,0.003335095);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(16,0.003688514);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(17,0.004075836);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(18,0.004363136);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(19,0.004656827);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(20,0.005047236);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(21,0.005439704);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(22,0.0057639);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(23,0.006244936);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(24,0.006669011);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(25,0.007137098);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(26,0.007583647);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(27,0.007994453);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(28,0.008492799);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(29,0.008993809);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(30,0.009436632);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(31,0.01017292);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(32,0.01090427);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(33,0.01153812);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(34,0.01214617);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(35,0.01273757);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(36,0.0132985);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(37,0.01384291);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(38,0.01452303);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(39,0.01506569);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(40,0.01578598);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(41,0.01653872);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(42,0.01739091);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(43,0.01828372);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(44,0.01934787);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(45,0.02018229);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(46,0.02116142);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(47,0.02230199);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(48,0.02325442);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(49,0.02412418);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(50,0.02549961);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(51,0.02670786);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(52,0.02796989);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(53,0.02930175);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(54,0.03089473);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(55,0.03255228);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(56,0.03382736);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(57,0.03514255);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(58,0.03699195);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(59,0.03851753);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(60,0.04031217);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(61,0.04198727);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(62,0.04409143);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(63,0.04576493);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(64,0.0477182);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(65,0.04954837);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(66,0.05175958);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(67,0.05376142);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(68,0.05642241);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(69,0.05915302);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(70,0.06138433);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(71,0.06441355);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(72,0.06685236);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(73,0.06932106);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(74,0.07262255);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(75,0.07599412);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(76,0.07977102);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(77,0.08377416);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(78,0.08738123);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(79,0.09153515);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(80,0.09574762);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(81,0.09992813);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(82,0.1063113);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(83,0.113509);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(84,0.1207553);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(85,0.12842);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(86,0.1371035);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(87,0.1455762);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(88,0.1549414);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(89,0.1667361);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(90,0.1797062);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(91,0.1965828);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(92,0.2138513);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(93,0.2356568);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(94,0.2592378);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(95,0.2917182);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(96,0.3392919);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(97,0.4079127);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(98,0.4606751);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(99,0.5864668);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(100,0.7754199);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetEntries(100);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetStats(0);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetLineColor(8);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetLineWidth(3);
   for( int i = 1; i < 101; ++ i){
     double y = MVA_BDT_50trees_10percentnode_beta015_effBvsS92->GetBinContent(i);
     if( y> 0 ) MVA_BDT_50trees_10percentnode_beta015_effBvsS92->SetBinContent(i, (double)i / 100 / (double)sqrt(y) );
   }

   TH1D *MVA_BDT_50trees_10percentnode_beta015_effBvsS9 = new TH1D("MVA_BDT_50trees_10percentnode_beta015_effBvsS9","MVA_BDT_50trees_10percentnode_beta015",100,0,1);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(1,0.001089269);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(2,0.001089269);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(3,0.001089269);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(4,0.001089269);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(5,0.001089269);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(6,0.001089269);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(7,0.001142476);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(8,0.001368719);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(9,0.001614604);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(10,0.001810707);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(11,0.001953773);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(12,0.002278931);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(13,0.002552637);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(14,0.002826343);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(15,0.00308262);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(16,0.003450092);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(17,0.003915044);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(18,0.004250623);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(19,0.004635084);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(20,0.005368128);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(21,0.00583522);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(22,0.006261281);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(23,0.006786088);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(24,0.007341809);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(25,0.007896464);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(26,0.008405327);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(27,0.008883963);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(28,0.009555404);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(29,0.01012528);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(30,0.01065464);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(31,0.01181267);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(32,0.01249527);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(33,0.01316899);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(34,0.0137162);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(35,0.01447867);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(36,0.01528066);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(37,0.01593324);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(38,0.01689403);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(39,0.0179022);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(40,0.01891805);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(41,0.01949368);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(42,0.02092646);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(43,0.02195328);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(44,0.02301801);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(45,0.02441295);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(46,0.02540017);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(47,0.02639016);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(48,0.02751216);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(49,0.02879941);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(50,0.03019494);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(51,0.03163182);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(52,0.03330114);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(53,0.03472319);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(54,0.03632463);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(55,0.03748718);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(56,0.03905184);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(57,0.04074618);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(58,0.04242411);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(59,0.04459232);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(60,0.04621696);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(61,0.04851588);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(62,0.0509497);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(63,0.05358396);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(64,0.05627673);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(65,0.05879797);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(66,0.06216061);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(67,0.06543933);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(68,0.06795847);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(69,0.07099201);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(70,0.0739214);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(71,0.07666528);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(72,0.08062765);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(73,0.08441207);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(74,0.08866267);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(75,0.09299937);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(76,0.09762775);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(77,0.103413);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(78,0.1096735);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(79,0.1139654);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(80,0.1219974);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(81,0.1270965);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(82,0.1338433);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(83,0.1420905);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(84,0.1479463);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(85,0.158505);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(86,0.1678078);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(87,0.1772981);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(88,0.1898618);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(89,0.2022433);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(90,0.2177741);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(91,0.2304614);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(92,0.250496);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(93,0.2742424);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(94,0.3046701);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(95,0.3376659);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(96,0.3946285);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(97,0.454304);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(98,0.556482);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(99,0.6468427);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(100,0.8613754);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetEntries(100);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetStats(0);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetLineColor(7);
   MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetLineWidth(3);
   for( int i = 1; i < 101; ++ i){
     double y = MVA_BDT_50trees_10percentnode_beta015_effBvsS9->GetBinContent(i);
     if( y> 0 ) MVA_BDT_50trees_10percentnode_beta015_effBvsS9->SetBinContent(i, (double)i / 100 / (double)sqrt(y) );
   }

   TH1D *MVA_BDT_50trees_8percentnode_beta02_effBvsS5 = new TH1D("MVA_BDT_50trees_8percentnode_beta02_effBvsS5","MVA_BDT_50trees_8percentnode_beta02",100,0,1);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(1,0.0009973332);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(2,0.0009973332);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(3,0.0009973332);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(4,0.00125118);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(5,0.00161282);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(6,0.00197446);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(7,0.002333737);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(8,0.002728415);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(9,0.003170382);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(10,0.003702956);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(11,0.004083997);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(12,0.004531896);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(13,0.00516451);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(14,0.005652135);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(15,0.006125911);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(16,0.006603643);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(17,0.00711685);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(18,0.007532722);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(19,0.007957537);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(20,0.008560717);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(21,0.009232461);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(22,0.009740924);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(23,0.01022994);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(24,0.01084634);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(25,0.01137162);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(26,0.01203929);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(27,0.01271976);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(28,0.0135334);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(29,0.01419508);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(30,0.01477019);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(31,0.01552688);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(32,0.01624745);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(33,0.01703104);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(34,0.0177343);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(35,0.01850132);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(36,0.01937099);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(37,0.02034735);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(38,0.02141086);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(39,0.02230192);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(40,0.02352535);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(41,0.02463194);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(42,0.02578932);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(43,0.02720685);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(44,0.0285719);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(45,0.03001516);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(46,0.03117513);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(47,0.03239696);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(48,0.03339515);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(49,0.0348168);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(50,0.03615713);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(51,0.03793665);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(52,0.03948232);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(53,0.04079426);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(54,0.04306379);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(55,0.04534831);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(56,0.04737121);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(57,0.04908751);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(58,0.05121867);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(59,0.0535375);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(60,0.05536283);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(61,0.05769885);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(62,0.0588463);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(63,0.06061933);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(64,0.06283659);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(65,0.06504162);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(66,0.0675673);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(67,0.07025114);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(68,0.07349463);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(69,0.07642591);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(70,0.08117726);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(71,0.08567967);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(72,0.08810783);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(73,0.09227191);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(74,0.09549522);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(75,0.09954673);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(76,0.1042123);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(77,0.1096898);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(78,0.1165988);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(79,0.1227647);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(80,0.1278168);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(81,0.1349481);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(82,0.1423969);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(83,0.1508447);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(84,0.1601403);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(85,0.1683585);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(86,0.1768017);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(87,0.1861944);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(88,0.1970736);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(89,0.2140274);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(90,0.2328061);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(91,0.246982);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(92,0.2672861);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(93,0.2901876);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(94,0.3133736);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(95,0.3483383);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(96,0.3959503);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(97,0.4564943);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(98,0.5391923);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(99,0.6626157);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(100,0.8399608);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetEntries(100);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetStats(0);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetLineColor(6);
   MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetLineWidth(3);
   for( int i = 1; i < 101; ++ i){
     double y = MVA_BDT_50trees_8percentnode_beta02_effBvsS5->GetBinContent(i);
     if( y> 0 ) MVA_BDT_50trees_8percentnode_beta02_effBvsS5->SetBinContent(i, (double)i / 100 / (double)sqrt(y) );
   }

 TH1D *MVA_BDT_100trees_10percentnode_bet02_effBvsS = new TH1D("MVA_BDT_100trees_10percentnode_bet02_effBvsS","MVA_BDT_100trees_10percentnode_bet02",100,0,1);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(1,0.0001604994);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(2,0.0002278027);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(3,0.000378842);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(4,0.0005298813);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(5,0.0007607442);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(6,0.0009906726);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(7,0.001409982);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(8,0.001677626);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(9,0.002014909);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(10,0.002241364);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(11,0.002669776);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(12,0.002899402);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(13,0.003357905);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(14,0.003743757);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(15,0.00424957);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(16,0.004800946);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(17,0.005165562);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(18,0.005612858);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(19,0.006164693);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(20,0.006582612);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(21,0.006980495);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(22,0.007397726);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(23,0.007927743);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(24,0.008583968);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(25,0.009232924);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(26,0.009810134);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(27,0.0105392);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(28,0.01127416);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(29,0.01173935);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(30,0.01242558);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(31,0.01314859);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(32,0.01395235);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(33,0.01463162);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(34,0.01533284);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(35,0.01609386);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(36,0.01672558);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(37,0.01756729);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(38,0.01831259);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(39,0.01913491);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(40,0.02007193);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(41,0.02116758);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(42,0.02198453);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(43,0.02283286);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(44,0.02391784);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(45,0.0250158);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(46,0.02608949);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(47,0.02698992);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(48,0.02797879);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(49,0.02893653);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(50,0.03008938);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(51,0.03147469);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(52,0.03274128);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(53,0.0338896);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(54,0.03559846);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(55,0.03712332);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(56,0.03862165);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(57,0.04031887);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(58,0.04224584);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(59,0.04428668);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(60,0.04600067);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(61,0.04776959);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(62,0.04958438);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(63,0.05157479);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(64,0.05376635);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(65,0.0563432);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(66,0.05864828);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(67,0.06126698);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(68,0.06316642);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(69,0.06591171);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(70,0.06892405);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(71,0.07149683);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(72,0.07433486);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(73,0.07735532);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(74,0.08043053);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(75,0.0839582);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(76,0.0876185);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(77,0.0918548);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(78,0.09585433);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(79,0.1007248);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(80,0.1058521);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(81,0.1115395);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(82,0.1184035);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(83,0.1247662);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(84,0.1316307);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(85,0.1401277);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(86,0.148799);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(87,0.1589145);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(88,0.168964);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(89,0.1817387);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(90,0.1974948);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(91,0.2118524);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(92,0.2296409);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(93,0.2538082);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(94,0.2817391);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(95,0.3120615);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(96,0.3536072);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(97,0.4283057);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(98,0.4958173);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(99,0.6038265);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(100,0.7673341);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetEntries(100);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetStats(0);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetLineColor(4);
   MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetLineWidth(3);
   for( int i = 1; i < 101; ++ i){
     double y = MVA_BDT_100trees_10percentnode_bet02_effBvsS->GetBinContent(i);
     if( y> 0 ) MVA_BDT_100trees_10percentnode_bet02_effBvsS->SetBinContent(i, (double)i / 100 / (double)sqrt(y) );
   }

   if( aT ){
     TH2F *h6             = (TH2F*)f->Get("AtvsGamma");
     TH2F *h7		  = (TH2F*)f1->Get("AtvsGamma");
     cout << h6->Integral() << endl;
     for( int i = 1; i<h6->GetNbinsX()+1; ++i){
       if( h7->Integral(1,i,-1,-1)/allbkgsuper> 0 ) dt2->SetPoint(i,h6->Integral(1,i,-1,-1)/allsigsuper,h6->Integral(1,i,-1,-1)/allsigsuper/sqrt(h7->Integral(1,i,-1,-1)/allbkgsuper));
     }
     delete h6; delete h7;
   }

   if( aT ){
     TH2F *h6             = (TH2F*)f->Get("AtvsDelta");
     TH2F *h7		  = (TH2F*)f1->Get("AtvsDelta");
     cout << h6->Integral() << endl;
     for( int i = 1; i<h6->GetNbinsX()+1; ++i){
       if( h7->Integral(1,i,-1,-1)/allbkgsuper> 0 ) dt->SetPoint(i,h6->Integral(1,i,-1,-1)/allsigsuper,h6->Integral(1,i,-1,-1)/allsigsuper/sqrt(h7->Integral(1,i,-1,-1)/allbkgsuper));
     }
     delete h6; delete h7;
   }

   if( aT ){
     TH2F *h6             = (TH2F*)f->Get("Mstar3DivPtvsGamma");
     TH2F *h7		  = (TH2F*)f1->Get("Mstar3DivPtvsGamma");
     cout << h6->Integral() << endl;
     for(double i = 1; i < 91; ++i){
       double c = 90 - i;
       double sig = 0;
       double bkg = 0;
       for( double x = 1; x <= i; ++x ){
	 double yup = -x + c;
	 sig += h6->Integral(x,x,1,yup);
	 sig += h7->Integral(x,x,1,yup);
       }
       dt4->SetPoint(i, sig / allsigsuper, bkg / allbkgsuper);
     }

     delete h6; delete h7;
   }



 
  if(  eff ){
    TCanvas *c20 = new TCanvas("c20","c20",800,600);
    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();
    gStyle->SetOptStat(0); 
    gStyle->SetTitleFontSize(0.03);
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);
    hist_leg1 = new TLegend(0.83,0.55,1,0.90);
    hist_leg1->SetFillColor(0);
    hist_leg1->SetLineColor(1);
    hist_leg1->SetShadowColor(0);
    hist_leg1->SetTextSize(0.04);
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
    dt->GetXaxis()->SetTitleOffset(0.9);
    dt->GetYaxis()->SetTitleOffset(0.9);
    //    dt->GetXaxis()->SetRangeUser(0.0,.49);
    dt->GetYaxis()->SetRangeUser(0.,3.9);
    dt->SetMarkerStyle(8);
    dt->SetMarkerSize(.3);
    dt->SetLineWidth(3);
    dt->SetLineColor(40);
    dt2->SetLineColor(46);
    dt2->SetLineWidth(3);
    dt2->SetLineStyle(1);
    dt4->SetLineColor(2);
    dt4->SetLineWidth(3);
    dt4->SetLineStyle(2);
    util->SetLineColor(1);
    util->SetLineStyle(2);
    util->SetLineWidth(1);
    atlas->SetMarkerStyle(3);
    atlas->SetMarkerColor(1);
    atlas->SetMarkerSize(1.5);

    hist_leg1->AddEntry(dt, Form("#Delta"), "l");
    hist_leg1->AddEntry(dt2, Form("#Gamma"),"l"); 
    hist_leg1->AddEntry(MVA_BDT_50trees_8percentnode_beta02_effBvsS5, Form("BDT2"),"l"); 
    hist_leg1->AddEntry(MVA_BDT_50trees_10percentnode_beta015_effBvsS9, Form("BDT3"),"l"); 
    hist_leg1->AddEntry(MVA_BDT_50trees_10percentnode_beta015_effBvsS92, Form("BDT4"),"l"); 
    hist_leg1->AddEntry(MVA_BDT_100trees_10percentnode_bet02_effBvsS, Form("BDT5"),"l"); 

    cout << dt->GetX()[TMath::LocMax(dt->GetN(),dt->GetY())] << " " << dt->GetY()[TMath::LocMax(dt->GetN(),dt->GetY())] << endl;
    cout << dt2->GetX()[TMath::LocMax(dt2->GetN(),dt2->GetY())] << " " << dt2->GetY()[TMath::LocMax(dt2->GetN(),dt2->GetY())] << endl;
    int maxbin = MVA_BDT_50trees_8percentnode_beta02_effBvsS5->GetMaximumBin();
    cout << MVA_BDT_50trees_8percentnode_beta02_effBvsS5->GetBinCenter(maxbin) << " " << MVA_BDT_50trees_8percentnode_beta02_effBvsS5->GetBinContent(maxbin) << endl;
    maxbin = MVA_BDT_50trees_10percentnode_beta015_effBvsS9->GetMaximumBin();
    cout << MVA_BDT_50trees_10percentnode_beta015_effBvsS9->GetBinCenter(maxbin) << " " << MVA_BDT_50trees_10percentnode_beta015_effBvsS9->GetBinContent(maxbin) << endl;
    maxbin = MVA_BDT_50trees_10percentnode_beta015_effBvsS92->GetMaximumBin();
    cout << MVA_BDT_50trees_10percentnode_beta015_effBvsS92->GetBinCenter(maxbin) << " " << MVA_BDT_50trees_10percentnode_beta015_effBvsS92->GetBinContent(maxbin) << endl;
    maxbin = MVA_BDT_100trees_10percentnode_bet02_effBvsS->GetMaximumBin();
    cout << MVA_BDT_100trees_10percentnode_bet02_effBvsS->GetBinCenter(maxbin) << " " << MVA_BDT_100trees_10percentnode_bet02_effBvsS->GetBinContent(maxbin) << endl;


    hist_leg1->AddEntry(atlas, Form("ATLAS"),"p");
    dt->Draw("A C");
    dt2->Draw("same C");
    //    dt4->Draw("same L");
    MVA_BDT_50trees_8percentnode_beta02_effBvsS5->Draw("same C");
    MVA_BDT_50trees_10percentnode_beta015_effBvsS9->Draw("same C");
    MVA_BDT_50trees_10percentnode_beta015_effBvsS92->Draw("same C");
    MVA_BDT_100trees_10percentnode_bet02_effBvsS->Draw("same C");
    util->Draw("same");
    atlas->Draw("same P");
    hist_leg1->Draw("same");

    TLatex Tl;
    Tl.SetTextAlign(12);
    Tl.SetTextSize(0.04);
    Tl.DrawLatexNDC(0.17,0.85,"Z#rightarrow#tau#tau (sig) vs W(#rightarrowl#nu)+jets (bkg), #mu#tau_{had} final state");

    //save plot
    if (1==1){
      c20->SaveAs("plots/20170510/SIC_triangle.pdf");
    }    

  }

}

