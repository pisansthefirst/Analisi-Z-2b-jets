#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <iomanip>
#include <string.h>

#include <algorithm> 

#include "TStyle.h"
#include "TClass.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "THStack.h"
#include "TLatex.h"
#include "TString.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TPad.h"
#include "TLine.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"

#include "TRandom.h"
#include "TRandom3.h"

namespace macro{
  void draw_simple_histo(TFile* file, TString name_histo, float norm_factor);
  void merge_hists(TFile* file, std::vector<TString> h_name_v, TString name_histo_merge);
}


TStyle* AtlasStyle()
{
  TStyle *atlasStyle = new TStyle("","Atlas style");

  // use plain black on white colors
  Int_t icol=0; // WHITE
  atlasStyle->SetFrameBorderMode(icol);
  atlasStyle->SetFrameFillColor(icol);
  atlasStyle->SetCanvasBorderMode(icol);
  atlasStyle->SetCanvasColor(icol);
  atlasStyle->SetPadBorderMode(icol);
  atlasStyle->SetPadColor(icol);
  atlasStyle->SetStatColor(icol);

  // set the paper & margin sizes
  atlasStyle->SetPaperSize(20,26);

  // set margin sizes
  atlasStyle->SetPadRightMargin(0.12);  
  atlasStyle->SetPadBottomMargin(0.12);
  atlasStyle->SetPadLeftMargin(0.16);

  // set title offsets (for axis label)
  atlasStyle->SetTitleXOffset(1.4);
  atlasStyle->SetTitleYOffset(1.4);

  // use large fonts
  Int_t font=42; // Helvetica
  Double_t tsize=0.035;
  atlasStyle->SetTextFont(font);

  atlasStyle->SetTextSize(tsize);
  atlasStyle->SetLabelFont(font,"x");
  atlasStyle->SetTitleFont(font,"x");
  atlasStyle->SetLabelFont(font,"y");
  atlasStyle->SetTitleFont(font,"y");
  atlasStyle->SetLabelFont(font,"z");
  atlasStyle->SetTitleFont(font,"z");

  atlasStyle->SetLabelSize(tsize,"x");
  atlasStyle->SetTitleSize(tsize,"x");
  atlasStyle->SetLabelSize(tsize,"y");
  atlasStyle->SetTitleSize(tsize,"y");
  atlasStyle->SetLabelSize(tsize,"z");
  atlasStyle->SetTitleSize(tsize,"z");

  // use bold lines and markers
  atlasStyle->SetMarkerStyle(20);
  atlasStyle->SetMarkerSize(1.2);
  atlasStyle->SetHistLineWidth(2.);
  atlasStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

  // do not display any of the standard histogram decorations
  atlasStyle->SetOptTitle(0);
  atlasStyle->SetOptStat(0);
  atlasStyle->SetOptFit(0);

  // put tick marks on top and RHS of plots
  atlasStyle->SetPadTickX(1);
  atlasStyle->SetPadTickY(1);

  return atlasStyle;

}

void SetAtlasStyle()
{
  std::cout << "\nApplying  style settings...\n" << std::endl ;
  TStyle* atlasStyle = AtlasStyle();
  gROOT->SetStyle("");
  gROOT->ForceStyle();
}

void setstat(){
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(000);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetStatColor(0);
  gStyle->SetOptStat(0);
  gStyle->SetTitleFillColor(0);
  gStyle->SetOptStat(0000);
  gStyle->SetOptTitle(0);
  gStyle->SetHistMinimumZero();
}

void macro::draw_simple_histo(TFile* file, TString name_histo, float norm_factor)
{
  TH1D* histo_alone;
  std::cout << "... --> Histo simple for: " << name_histo << std::endl;
  histo_alone = (TH1D*)file->Get(name_histo);
  std::cout << "... --> Got it ! : " << name_histo << std::endl;
  if (histo_alone){
    histo_alone->Sumw2();
    histo_alone->Scale(norm_factor);
 }

  else std::cout << "... --> NO HISTO ALONE (CHECK!!)--------"<<std::endl;

}

// ==================================================================================================================================
// ==================================================================================================================================

void draw_stuck_histo(TString item, TFile* file[3], int n_files, double lumi, TFile* OutPutFile, TString name_histo, int REBIN,int color, TString sum_name, float flag)
{
  gStyle->SetOptStat(0000);
  std::cout << "---> Histo stuck for: " << name_histo << " ---> Sum name: "<< sum_name <<std::endl;

  TCanvas* TC_histo = new TCanvas("TC_alone_"+sum_name,"TC_alone_"+sum_name,1000,800);
  TH1D* histo_alone;
  TH1D* sum;

  if (file[0]){
    std::cout << "... file ok" << std::endl;
    histo_alone = (TH1D*)file[0]->Get(name_histo);
    if (!histo_alone) std::cout << "... Problem to open histograms from root files. CHECK ITEM NAME: " << name_histo << std::endl;
  }
  else std::cout << "ERROR++++ in draw_stuck_histo. Problems to open file"<<std::endl;

  bool first=true;
  TH1D* TOT[13];
  TH1D* TEMP[13];

  for (int i=0; i<n_files; i++){
    if (file[i]){
      std::cout << "... file i: " << i <<std::endl;
      TOT[i]= (TH1D*) file[i]->Get("selectionCutFlow");
      if ( TOT[i]){
        double n_entries=1;
        n_entries=TOT[i]->GetBinContent(1);
        bool problem=true;
        
        if (n_entries == 0){
          problem=false;
          std::cout<< "PROBLEM!!!!!!!!!!!" <<std::endl;
        }
        
        if (n_entries>0){
        
          TH1D* temp = (TH1D*)file[i]->Get(name_histo);          
          if (!temp) continue;
          
          double s_weight=temp->Integral();
 
          std::cout << "... norm_factor = "<<flag<<std::endl;
          macro::draw_simple_histo(file[i], name_histo, flag);
          
          TEMP[i]=(TH1D*) file[i]->Get(name_histo);
          if (TEMP[i]) {
            if (first){
              histo_alone = (TH1D*)file[i]->Get(name_histo);
              sum = (TH1D*) histo_alone->Clone("h_sum");
              sum->SetName(sum_name);
              sum->SetTitle(sum_name);
              for(int i=0;i<sum->GetNbinsX()+1;i++){sum->SetBinContent(i,0);sum->SetBinError(i,0);}
              sum->Sumw2();
              first=false;
            }
            if (TEMP[i]->GetEntries()!=0) {   
              sum->Add(TEMP[i]);
              std::cout<<"... Integral: " << TEMP[i]->Integral() <<std::endl;
              std::cout<<"... " << std::endl;
            }
          }
        }
        else  flag == 0;
        } 
    }
    else
      std::cout<<"PROBLEMS TO OPEN FILE. file [i] where i = "<<i<<std::endl;
  }

  if (!first) {

	OutPutFile->cd();
        sum->SetName(sum_name);
        sum->SetTitle(sum_name);
		
	if (REBIN>1) sum->Rebin(REBIN);

	std::cout << "... Sum hist: " << sum->Integral() << std::endl;	
	sum->Write();
    std::cout<<"... Wrote histo sum with name "<<sum->GetName() <<std::endl;
    std::cout<< "=============================================== " <<std::endl;	
	   
    TC_histo->Close();
    delete sum;
    delete TC_histo;
  } else{
    OutPutFile->cd();
    TC_histo->Close();
    delete TC_histo;
    std::cout<<"... No histo sum " <<std::endl;
    std::cout<< "=============================================== " <<std::endl;	    
  }

  std::cout<< "" <<std::endl;        

}

// ==================================================================================================================================
// ==================================================================================================================================

void macro::merge_hists(TFile* file, std::vector<TString> h_name_v, TString name_histo_merge)
{
  file->cd();
    
  std::vector<TH1D*> histos;
  TH1D* histo_merge;

  std::cout << "... Merging hists" << std::endl;  
  for (int i=0;i<h_name_v.size();i++) {
	std::cout << "... Getting hist[" << i << "]: " << h_name_v[i] << std::endl;
    TH1D* histo_temp = (TH1D*) file->Get(h_name_v[i]);
    if (histo_temp) {
	   histos.push_back(histo_temp);
	   if (histos.size()==1) histo_merge = (TH1D*) histos[0]->Clone(name_histo_merge);
	   else histo_merge->Add(histo_temp);		
	}  
  }
  
  if (histos.size()>0) {  
	std::cout << "... ==> Merged hist: " << name_histo_merge <<  std::endl;
    histo_merge->SetName(name_histo_merge);
    histo_merge->SetTitle(name_histo_merge);
    histo_merge->Print();
    histo_merge->Write();
  } else {  
    std::cout << "... ==> No merged hist" <<  std::endl; 
  }
  std::cout << "..." <<  std::endl;
  
}

// ==================================================================================================================================
// ==================================================================================================================================

void draw_stuck_all_data(TString PATH, TString channel, TString region, TString item, TFile* OutputFile, std::vector<TString> h_sig_name, std::vector<TString>  h_sig_name_sh, std::vector<TString> h_bkg_name, std::vector<TString> h_data_name,
                    int REBIN, float x_min, float x_max, float y_min, float y_max, TString x_name, TString y_name, bool do_log) {

    gStyle->SetOptStat(0000);
    setstat();
    SetAtlasStyle();
    std::cout << "----------- ALLLLLL ============================ " << std::endl;
    OutputFile->cd();
    TString name_histo = channel + "_" + region + "_" + item;
    TCanvas* TC_histo = new TCanvas("TC_sum_"+name_histo,"TC_sum_"+name_histo,0,0,1200,1200);
    TC_histo->SetFillColor(0);
    TC_histo->SetBorderMode(0);
    TC_histo->SetBorderSize(2);
    TC_histo->SetTickx(1);
    TC_histo->SetTicky(1);
    TC_histo->SetLeftMargin(0.14);
    TC_histo->SetRightMargin(0.05);
    TC_histo->SetTopMargin(0.05);
    TC_histo->SetBottomMargin(0.13);
    TC_histo->SetFrameBorderMode(0);
    TC_histo->SetFrameBorderMode(0);

    if (do_log) { TC_histo->SetLogy(); }
    TC_histo->cd();
    THStack* sum = new THStack(name_histo, name_histo);

    std::vector<TH1D*> h_sig;
    std::vector<TH1D*> h_sig_ori;
    std::vector<TH1D*> h_sig_sh;
    std::vector<TH1D*> h_sig_ori_sh;
    std::vector<TH1D*> h_bkg;
    std::vector<TH1D*> h_bkg_ori;
    std::vector<TH1D*> h_data_1;
    std::vector<TH1D*> h_data_ori;

    TString sufFC[5]={"_NB","_1B","_1C","_NC","_L"};


     for (int i=0; i<h_sig_name.size();i++) {
     h_sig_ori.push_back( (TH1D*) OutputFile->Get(h_sig_name[i]));
     cout<<"punto 1"<<endl;
     if ( ! OutputFile->Get(h_sig_name[i])   )  { std::cout<<"  ERROR: Z+jets IS ZERO! "<<i<<std::endl; }
     TString histo_name_sig="h_Zjet"+sufFC[i]+"fxfx_new";
     h_sig.push_back( (TH1D*) h_sig_ori[i]->Clone(histo_name_sig));
     }

     for (int i=0; i<h_sig_name_sh.size();i++) {
     h_sig_ori_sh.push_back( (TH1D*) OutputFile->Get(h_sig_name_sh[i]));
     cout<<"punto 2"<<endl;
     if ( ! OutputFile->Get(h_sig_name_sh[i])   )  { std::cout<<"  ERROR: Z+jets IS ZERO! "<<i<<std::endl; }
     TString histo_name_sig_sh="h_Zjet"+sufFC[i]+"sh_new";
     h_sig_sh.push_back( (TH1D*) h_sig_ori_sh[i]->Clone(histo_name_sig_sh));
     }

    for (int i = 0; i < h_bkg_name.size(); i++) {
        h_bkg_ori.push_back((TH1D*)OutputFile->Get(h_bkg_name[i]));
        if (!OutputFile->Get(h_bkg_name[i])) { std::cout << "  ERROR: BKG IS ZERO! " << i << std::endl; }
        TString histo_name_bkg = h_bkg_name[i] + "_new";
        h_bkg.push_back((TH1D*)h_bkg_ori[i]->Clone(histo_name_bkg));
    }

for (int i = 0; i < h_data_name.size(); i++) {
    h_data_ori.push_back((TH1D*)OutputFile->Get(h_data_name[i]));
    if (!OutputFile->Get(h_data_name[i])) {
        std::cout << "  ERROR: Data histogram is zero! " << i << std::endl;
    }
    TString histo_name_data = "h_data_new";
    h_data_1.push_back((TH1D*)h_data_ori[i]->Clone(histo_name_data)); 
}

    TH1D* h_Zc = (TH1D*)h_sig[2]->Clone("h_Zc_new");
    h_Zc->Reset("ICESM");
    h_Zc->Add(h_sig[2]);
    h_Zc->Add(h_sig[3]);
    h_Zc->SetFillColor(TColor::GetColor("#D36BC6") );
    double dataIntegZc, integErrZc;
    dataIntegZc = h_Zc->IntegralAndError(0, h_Zc->GetNbinsX() + 1, integErrZc);

    std::vector<double> dataIntegSig, integErrSig;
    std::vector<double> dataIntegBkg, integErrBkg;

    TString hcol;
    for (int i = 0; i < h_sig_name.size(); i++) {
        if ((i != 2) && (i != 3)) {
            if (i == 0) hcol = "#ffffff";
            if (i == 1) hcol = "#930FA5";
            if (i == 4) hcol = "#D8A8D8";
            h_sig[i]->SetFillColor(TColor::GetColor(hcol));
        }
    
double integErrSigTemp;
double dataIntegSigTemp;
dataIntegSigTemp= h_sig[i]->IntegralAndError(0, h_sig[i]->GetNbinsX()+1, integErrSigTemp);
dataIntegSig.push_back(dataIntegSigTemp);
integErrSig.push_back(integErrSigTemp);

}

    for (int i = 0; i < h_bkg_name.size(); i++) {
      if (i==0) hcol="#7FD6DB";//stop
      if (i==1) hcol="#3D8E33";//dibos
      if (i==2) hcol="#AADD96";//ttbar
      if (i==3) hcol="#EA6BBF";//Ztautau #DB28A5
      if (i==4) hcol="#F9E27F"; //ZH
        h_bkg[i]->SetFillColor(TColor::GetColor(hcol));
        double integErrBkgTemp;
        double dataIntegBkgTemp;
        dataIntegBkgTemp = h_bkg[i]->IntegralAndError(0, h_bkg[i]->GetNbinsX() + 1, integErrBkgTemp);
        dataIntegBkg.push_back(dataIntegBkgTemp);
        integErrBkg.push_back(integErrBkgTemp);
    }
    
    sum ->Add(h_bkg[3]);//Ztau
    sum ->Add(h_bkg[1]);//diboson
    sum ->Add(h_bkg[0]);//stop
    sum ->Add(h_bkg[2]);//ttbar
    sum ->Add(h_bkg[4]);//ZH
    //segnale
    sum ->Add(h_sig[4]);//Z+l
    sum ->Add(h_Zc); //Z+nc
    sum ->Add(h_sig[1]);//Z+1b
    sum ->Add(h_sig[0]);//Z+2b

    if ((x_min==-999)&&(x_max=-999)&&(y_min=-999)&&(y_max=-999)) {
  
  y_min=0.002;
  if ( item=="diBJets_DPhi")
  { x_min=0;
  x_max=3.15;
  
  if(channel=="Mu")
  { y_max=42000;
  }
  if(channel=="El")
  { y_max=85000;
  }
  if(channel=="Lep")
  { y_max=70000;
  }
  
  }
  if( item=="diBJets_M")
  { x_min=0;
  x_max=1200;
  y_max=100000000;
  }
  
  if ( item=="diBJets_DR")
  { x_min=0.4;
  x_max=5;
  y_max=75000;
  }
  
  if ( item=="diBJets_DY")
  { x_min=0;
  x_max=4;
  y_max=100000;
  }
  
}

    TH2D* h_xy = new TH2D("h_xy", "h_xy", (x_max - x_min) / REBIN, x_min, x_max, 10, y_min, y_max);
    h_xy->GetXaxis()->SetTitle(x_name);
    h_xy->GetYaxis()->SetTitle(y_name);

    // DRAW PLOTS
    gStyle->SetOptStat(0000);
    setstat();
    SetAtlasStyle();

    h_xy->Draw();
    sum->Draw("HIST SAME");

    h_data_1[0]->SetMarkerStyle(20);
    h_data_1[0]->SetMarkerSize(1.5);
    h_data_1[0]->Draw("P SAME");

    h_xy->GetXaxis()->SetLabelSize(0.03);
    h_xy->GetXaxis()->SetTitleSize(0.04);
    h_xy->GetYaxis()->SetLabelSize(0.03);
    h_xy->GetYaxis()->SetTitleSize(0.04);
    h_xy->GetXaxis()->SetTitleOffset(1);
    h_xy->GetYaxis()->SetTitleOffset(1.6);

    h_xy->Draw("axis SAME");

    TLatex* tex0 = new TLatex();
    tex0->SetNDC();
    tex0->SetTextSize(0.03);
    tex0->DrawLatex(0.2,0.88,"#font[72]{ATLAS} Work In Progress" );

    TLatex* tex1 = new TLatex();
    tex1->SetNDC();
    tex1->DrawLatex(0.2,0.88-0.05,"#font[42]{13 TeV, 140 fb^{-1}}" );
    
    TLatex* tex2 = new TLatex();
    tex2->SetNDC();
    tex2->DrawLatex(0.2,0.88-0.09,"#font[42]{Z(#rightarrow ll)+#geq 2 b-jet}" );

    TLegend* leg1 = new TLegend(0.65, 0.62, 0.93, 0.9);
    leg1->AddEntry(h_sig[0], "Z(#rightarrow #mu#mu)+2b-jets ", "F");
    leg1->AddEntry(h_sig[1], "Z(#rightarrow #mu#mu)+1b-jet ", "F");
    leg1->AddEntry(h_Zc, "Z(#rightarrow #mu#mu)+c-jet ", "F");
    leg1->AddEntry(h_sig[4], "Z(#rightarrow #mu#mu)+light-jets ", "F");
    
    leg1->AddEntry(h_bkg[2], "ttbar ", "F");
    leg1->AddEntry(h_bkg[0], "single top ", "F");
    leg1->AddEntry(h_bkg[1], "Diboson ", "F");
    leg1->AddEntry(h_bkg[3], "Z(#rightarrow #tau #tau ) +jets ", "F");
    leg1->AddEntry(h_bkg[4], "ZH", "F");

    leg1->SetLineColor(0);
    leg1->SetLineStyle(1);
    leg1->SetLineWidth(1);
    leg1->SetFillColor(0);
    leg1->SetFillStyle(1001);
    leg1->SetTextColor(1);
    leg1->SetTextFont(42);
    leg1->SetFillColor(0);
    leg1->Draw("SAME");

    TC_histo->Print(PATH + channel + "_" + region + "_" + item + ".png");

    h_xy->Delete();
    delete TC_histo;
}

// ==================================================================================================================================
// ==================================================================================================================================

void draw_stuck_all_ratio(TString PATH, TString channel, TString region, TString item, TFile * OutputFile, std::vector<TString>  h_sig_name, std::vector<TString>  h_sig_name_sh, std::vector<TString>  h_bkg_name, std::vector<TString> h_data_name,
int REBIN, float x_min, float x_max,float y_min , float y_max, TString x_name, TString y_name, bool do_log)
{
  gStyle->SetOptStat(0000);
 setstat();
 SetAtlasStyle();
 std::cout << "----------- ALLLLLL ============================ "<< std::endl;

 OutputFile->cd();
 TString name_histo=channel+"_"+region+"_"+item;
 TCanvas * TC_histo = new TCanvas("TC_sum_"+name_histo,"TC_sum_"+name_histo,0,0,1200,1200);
 TC_histo->SetFillColor(0);
 TC_histo->SetBorderMode(0);
 TC_histo->SetBorderSize(2);
 TC_histo->SetTickx(1);
 TC_histo->SetTicky(1); 
 TC_histo->SetLeftMargin(0.14);
 TC_histo->SetRightMargin(0.05);
 TC_histo->SetTopMargin(0.05);
 TC_histo->SetBottomMargin(0.13);
 TC_histo->SetFrameBorderMode(0);
 TC_histo->SetFrameBorderMode(0);

 if (do_log) {TC_histo->SetLogy();}
 TC_histo->cd();
 
 THStack* sum = new THStack(name_histo,name_histo);

std::vector<TH1D*> h_sig;
std::vector<TH1D*> h_sig_ori;
std::vector<TH1D*> h_bkg;
std::vector<TH1D*> h_bkg_ori;
std::vector<TH1D*> h_sig_sh;
std::vector<TH1D*> h_sig_ori_sh;
std::vector<TH1D*> h_bkg_sh;
std::vector<TH1D*> h_bkg_ori_sh;
std::vector<TH1D*> h_data_1;
std::vector<TH1D*> h_data_ori;

TString sufFC[5]={"_NB","_1B","_1C","_NC","_L"};


 for (int i=0; i<h_sig_name_sh.size();i++) {
 h_sig_ori_sh.push_back( (TH1D*) OutputFile->Get(h_sig_name_sh[i]));
 if ( ! OutputFile->Get(h_sig_name_sh[i])   )  { std::cout<<"  ERROR: Z+jets IS ZERO! "<<i<<std::endl; }
 TString histo_name_sig_sh="h_Zjet"+sufFC[i]+"_sh_new";
 h_sig_sh.push_back( (TH1D*) h_sig_ori_sh[i]->Clone(histo_name_sig_sh));
 }
 
 for (int i=0; i<h_sig_name.size();i++) {
 h_sig_ori.push_back( (TH1D*) OutputFile->Get(h_sig_name[i]));
 if ( ! OutputFile->Get(h_sig_name[i])   )  { std::cout<<"  ERROR: Z+jets IS ZERO! "<<i<<std::endl; }
 TString histo_name_sig="h_Zjet"+sufFC[i]+"_fxfx_new";
 h_sig.push_back( (TH1D*) h_sig_ori[i]->Clone(histo_name_sig));
 }

 for (int i=0; i<h_bkg_name.size();i++) {
 h_bkg_ori.push_back((TH1D*) OutputFile->Get(h_bkg_name[i]));
 if ( ! OutputFile->Get(h_bkg_name[i] ))  { std::cout<<"  ERROR: BKG IS ZERO! "<<i<<std::endl; }
 TString histo_name_bkg=h_bkg_name[i]+"_new";
 h_bkg.push_back((TH1D*) h_bkg_ori[i]->Clone(histo_name_bkg));  
}         

for (int i = 0; i < h_data_name.size(); i++) {
    h_data_ori.push_back((TH1D*)OutputFile->Get(h_data_name[i]));
    if (!OutputFile->Get(h_data_name[i])) {
        std::cout << "  ERROR: Data histogram is zero! " << i << std::endl; }
    TString histo_name_data = "h_data_new";
    h_data_1.push_back((TH1D*) h_data_ori[i]->Clone(histo_name_data));
    std::cout << h_data_1[i] << " ";
}
//////////////////////////////////////////////////////////////////////////////////////////////SH
TString hcol;

TH1D * h_Zc_sh = (TH1D*) h_sig_sh[2]->Clone("h_Zjet_C_sh_new"); h_Zc_sh ->Reset("ICESM");
h_Zc_sh->Add(h_sig_sh[2]);
h_Zc_sh->Add(h_sig_sh[3]);

//////////////////////////////////////////////////////////////////////////////////////////FxFx
TH1D * h_Zc = (TH1D*) h_sig[2]->Clone("h_Zjet_C_fxfx_new"); h_Zc ->Reset("ICESM");
h_Zc->Add(h_sig[2]);
h_Zc->Add(h_sig[3]);
h_Zc->SetFillColor(TColor::GetColor("#D36BC6") );
double  dataIntegZc, integErrZc;
dataIntegZc = h_Zc->IntegralAndError(0, h_Zc->GetNbinsX()+1, integErrZc); 

std::vector<double> dataIntegSig, integErrSig;
std::vector<double> dataIntegBkg, integErrBkg;

for (int i=0; i<h_sig_name.size();i++) {
if ((i!=2)&&(i!=3)){
if (i == 0) hcol = "#ffffff";
if (i == 1) hcol = "#930FA5";
if (i == 4) hcol = "#D8A8D8";
h_sig[i]->SetFillColor(TColor::GetColor(hcol));
}

double integErrSigTemp;
double dataIntegSigTemp;
dataIntegSigTemp= h_sig[i]->IntegralAndError(0, h_sig[i]->GetNbinsX()+1, integErrSigTemp);
dataIntegSig.push_back(dataIntegSigTemp);
integErrSig.push_back(integErrSigTemp);
}

for (int i=0; i<h_bkg_name.size();i++) {
if (i==0) hcol="#7FD6DB";//stop
      if (i==1) hcol="#3D8E33";//dibos
      if (i==2) hcol="#AADD96";//ttbar
      if (i==3) hcol="#EA6BBF";//Ztautau #DB28A5
      if (i==4) hcol="#F9E27F"; //ZH

h_bkg[i]->SetFillColor(TColor::GetColor(hcol));

double integErrBkgTemp;
double dataIntegBkgTemp;
dataIntegBkgTemp= h_bkg[i]->IntegralAndError(0, h_bkg[i]->GetNbinsX()+1, integErrBkgTemp);
dataIntegBkg.push_back(dataIntegBkgTemp);
integErrBkg.push_back(integErrBkgTemp);
}

/////////////////////////////////////////////////////////////////////////////////FXFX
//fondi
sum ->Add(h_bkg[3]);//Ztau
sum ->Add(h_bkg[1]);//diboson
sum ->Add(h_bkg[0]);//stop
sum ->Add(h_bkg[2]);//ttbar
sum ->Add(h_bkg[4]);//ZH
//segnale
sum ->Add(h_sig[4]);//Z+l
sum ->Add(h_Zc); //Z+nc
sum ->Add(h_sig[1]);//Z+1b
sum ->Add(h_sig[0]);//Z+2b 

TH1D * h_sum_all= (TH1D*) h_bkg[0]->Clone("h_sum_all"); 
//fondi
h_sum_all->Add(h_bkg[3]);
h_sum_all->Add(h_bkg[1]);
h_sum_all->Add(h_bkg[2]);
h_sum_all->Add(h_bkg[4]); 
//segnale
h_sum_all->Add(h_sig[4]);
h_sum_all->Add(h_Zc);
h_sum_all->Add(h_sig[1]);
h_sum_all->Add(h_sig[0]);

TH1D* h_sum_bkg=(TH1D*) h_bkg[0]->Clone("h_sum_bkg"); 
h_sum_bkg->Add(h_bkg[3]);
h_sum_bkg->Add(h_bkg[1]);
h_sum_bkg->Add(h_bkg[2]);
h_sum_bkg->Add(h_bkg[4]); 
h_sum_bkg->Add(h_sig[1]);
h_sum_bkg->Add(h_Zc);
h_sum_bkg->Add(h_sig[4]); 
h_sum_bkg->Write();

TH1D * h_sig_reco= (TH1D*) h_sig[0]->Clone("h_sig_reco"); 
h_sig_reco->Write();

////////////////////////////////////////////////////////////////////////////////SHERPA
TH1D* h_sum_all_sh=(TH1D*) h_sig_sh[0]->Clone("h_sum_all_sh"); 
h_sum_all_sh->Add(h_bkg[3]);
h_sum_all_sh->Add(h_bkg[1]);
h_sum_all_sh->Add(h_bkg[0]);
h_sum_all_sh->Add(h_bkg[2]);
h_sum_all_sh->Add(h_bkg[4]); 
h_sum_all_sh->Add(h_sig_sh[1]);
h_sum_all_sh->Add(h_Zc_sh);
h_sum_all_sh->Add(h_sig_sh[4]); 
h_sum_all_sh->Write();

cout << "***************************************"<<endl;
cout<<" NUMBER OF EVENTS "<<endl;
cout << "***************************************"<<endl;
cout << std::fixed ;  
cout << std::setprecision(0);
cout<<" Z+ 2b-jets         "<<  dataIntegSig[0]<<" ± "<<   integErrSig[0]<<" Rel unc "<< (integErrSig[0]/dataIntegSig[0])*100.<<"\%"  <<endl;
cout<<" Z+ 1b-jet          "<<  dataIntegSig[1]<<" ± "<<   integErrSig[1]<<" Rel unc "<< (integErrSig[1]/ dataIntegSig[1])*100.<<"\%"   <<endl;
cout<<" Z+ c-jets          "<<  dataIntegZc<<" ± "<<   integErrZc<<" Rel unc "<< (integErrZc/ dataIntegZc)*100.<<"\%" <<endl;
cout<<" Z+ light-jets      "<<  dataIntegSig[4]<<" ± "<<   integErrSig[4]<<"   Rel unc "<<(integErrSig[4]/dataIntegSig[4])*100.<<"\%"  <<endl;
cout<<" single top         "<<  dataIntegBkg[0]<<" ± "<<   integErrBkg[0]<<" Rel unc "<<(integErrBkg[0]/ dataIntegBkg[0])*100.<<"\%" <<endl;
cout<<" Diboson         "<<   dataIntegBkg[1]<<" ± "<<   integErrBkg[1]<<" Rel unc "<<(integErrBkg[1]/ dataIntegBkg[1])*100.<<"\%" <<endl;
cout<<" ttbar              "<<  dataIntegBkg[2]<<" ± "<<   integErrBkg[2]<<" Rel unc "<<(integErrBkg[2]/ dataIntegBkg[2])*100.<<"\%" <<endl;
cout<<" Z->tautau                  "<<  dataIntegBkg[3]<<" ± "<<   integErrBkg[3]<<" Rel unc "<<(integErrBkg[3]/ dataIntegBkg[3])*100.<<"\%" <<endl;
cout<<"ZH        "<<  dataIntegBkg[4]<<" ± "<<   integErrBkg[4]<<" Rel unc "<<(integErrBkg[4]/ dataIntegBkg[4])*100.<<"\%" <<endl;
cout << "***************************************"<<endl;
cout << "***************************************"<<endl;

if ((x_min==-999)&&(x_max=-999)&&(y_min=-999)&&(y_max=-999)) {
  
  y_min=0.002;
  if ( item=="diBJets_DPhi")
  { x_min=0;
  x_max=3.15;
  
  if(channel=="Mu")
  { y_max=42000;
  }
  if(channel=="El")
  { y_max=85000;
  }
  if(channel=="Lep")
  { y_max=70000;
  }
  
  }
  if( item=="diBJets_M")
  { x_min=0;
  x_max=1200;
  y_max=100000000;
  }
  
  if ( item=="diBJets_DR")
  { x_min=0.4;
  x_max=5;
  y_max=75000;
  }
  if ( item=="diBJets_DY")
  { x_min=0;
  x_max=4;
  y_max=100000;
  }
}

 TH2D * h_xy= new TH2D("h_xy","h_xy",(x_max-x_min)/REBIN,x_min,x_max, 10,y_min, y_max );
 h_xy->GetXaxis()->SetTitle(x_name);
 h_xy->GetYaxis()->SetTitle(y_name);

 /// DRAW PLOTS //// 
  gStyle->SetOptStat(0000);
  setstat();
  SetAtlasStyle();

  h_xy->Draw();

  sum->Draw("HIST SAME");
  h_xy->GetXaxis()->SetLabelSize(0.03);
    h_xy->GetXaxis()->SetTitleSize(0.04);
    h_xy->GetYaxis()->SetLabelSize(0.03);
    h_xy->GetYaxis()->SetTitleSize(0.04);
    h_xy->GetXaxis()->SetTitleOffset(1);
    h_xy->GetYaxis()->SetTitleOffset(1.6);

 h_xy->Draw("axis SAME");
 TLatex *   tex0= new TLatex();
 tex0->SetNDC();
 tex0->SetTextSize(0.03);
 tex0->DrawLatex(0.2,0.88,"#font[72]{ATLAS} Work In Progress" );
 TLatex *   tex1 = new TLatex();
 tex1->SetNDC();
 tex1->DrawLatex(0.2,0.88-0.07,"#font[42]{13 TeV, 140 fb^{-1}}" );
 TLatex *   tex2 = new TLatex();
 tex2->SetNDC();
 tex2->DrawLatex(0.2,0.88-0.09,"#font[42]{Z(-> ll)+ >= 2Bjets}" );
 
  
 TLegend *leg1 = new TLegend(0.65, 0.62, 0.93, 0.9);
    leg1->AddEntry(h_sig[0], "Z(#rightarrow #mu#mu)+2b-jets ", "F");
    leg1->AddEntry(h_sig[1], "Z(#rightarrow #mu#mu)+1b-jet ", "F");
    leg1->AddEntry(h_Zc, "Z(#rightarrow #mu#mu)+c-jet ", "F");
    leg1->AddEntry(h_sig[4], "Z(#rightarrow #mu#mu)+light-jets ", "F");
    
    leg1->AddEntry(h_bkg[2], "ttbar ", "F");
    leg1->AddEntry(h_bkg[0], "single top ", "F");
    leg1->AddEntry(h_bkg[1], "Diboson ", "F");
    leg1->AddEntry(h_bkg[3], "Z(#rightarrow #tau #tau ) +jets ", "F");
    leg1->AddEntry(h_bkg[4], "ZH", "F");

      h_data_1[0]->SetMarkerStyle(20);
      h_data_1[0]->SetMarkerSize(1);
      h_data_1[0]->Draw("P SAME");
      TC_histo->Print(PATH+channel+"_"+region+"_"+item+".png");

  ///RATIO
  TCanvas *c1_test = new TCanvas("c1_test", "c1_test",0.,0.,800,1600.*6./9.);
  c1_test->cd();
  TPad *p1 = new TPad("pad1","top left pad",                    0.0, 1./3., 1.0, 1.0);
  TPad *p1_ratio = new TPad("pad1_ratio","bottom left pad"     , 0.0, 0.0, 1.0, 1./3.);
   p1->SetTopMargin(0.05);
   p1->SetBottomMargin(0.);
   p1->SetRightMargin(0.05);
   p1->SetLeftMargin(0.16);
   p1->SetFillStyle(4000);

   p1_ratio->SetTopMargin(0.);
   p1_ratio->SetBottomMargin(0.48);
   p1_ratio->SetBottomMargin(0.30);
   p1_ratio->SetRightMargin(0.05);
   p1_ratio->SetLeftMargin(0.16);
   p1_ratio->SetFillStyle(4000);

  p1->Draw();
  p1_ratio->Draw();

  p1->cd();
  if (do_log) {p1->SetLogy();}
  h_xy->GetXaxis()->SetTitle(x_name);
  h_xy->GetYaxis()->SetTitle(y_name);
  h_xy->GetXaxis()->SetLabelSize(0.04*3./2.);
  h_xy->GetXaxis()->SetLabelSize(0.04);
  h_xy->GetXaxis()->SetTitleSize(0.04);
  h_xy->GetYaxis()->SetLabelSize(0.03);
  h_xy->GetYaxis()->SetTitleSize(0.05);
  h_xy->GetXaxis()->SetTitleOffset(1);
  h_xy->GetYaxis()->SetTitleOffset(1.2);
  h_xy->GetXaxis()->SetLabelSize(0.);
  h_xy->DrawCopy();
  sum->Draw("HIST SAME");
  
  h_data_1[0]->Draw("P SAME");

  leg1->SetLineColor(0);
  leg1->SetLineStyle(1);
  leg1->SetLineWidth(1);
  leg1->SetFillColor(0);
  leg1->SetFillStyle(1001);
  leg1->SetTextColor(1);
  leg1->SetTextFont(42);
  leg1->SetFillColor(0);
  leg1->Draw("SAME");
  
  tex0->SetNDC();
  tex0->SetTextSize(0.03);
  tex0->DrawLatex(0.2,0.88,"#font[72]{ATLAS} Work In Progress" );
  tex1->DrawLatex(0.2,0.88-0.05,"#font[42]{13 TeV, 140 fb^{-1}}" );
  tex2->SetNDC();
  tex2->DrawLatex(0.2,0.88-0.09,"#font[42]{Z(#rightarrow ll)+#geq 2 b-jet}" );
  c1_test->cd();
  p1_ratio->cd();

  TH2D * h_rdt=new TH2D("h_rdt","h_rtd",(x_max-x_min)/REBIN,x_min,x_max, 50, 0.71, 1.29);

  h_rdt->GetYaxis()->SetTickLength(0.04);
  h_rdt->GetYaxis()->SetNdivisions(305);
  
  h_rdt->GetXaxis()->SetTitleSize(0.1);
  h_rdt->GetYaxis()->SetLabelSize(0.1);
  h_rdt->GetXaxis()->SetLabelSize(0.1);
  h_rdt->GetYaxis()->SetTitleSize(0.1);
  h_rdt->GetXaxis()->SetTitleOffset(1);
  h_rdt->GetYaxis()->SetTitleOffset(0.6);
  
  h_rdt->GetXaxis()->SetTitle(x_name);
  h_rdt->GetYaxis()->SetTitle("Montecarlo/Dati");

  c1_test->cd();
  p1_ratio->cd();
  h_rdt->Draw();
  
  //ratio MC/D
  TH1D* histo_ratio_data_fxfx= (TH1D*) h_sum_all->Clone("histo_ratio_data_fxfx");
  histo_ratio_data_fxfx->Divide(h_data_1[0]);
  histo_ratio_data_fxfx->GetXaxis()->SetTitle("");
  histo_ratio_data_fxfx->SetMarkerStyle(22);
  histo_ratio_data_fxfx->SetMarkerColor(kPink+10);
  histo_ratio_data_fxfx->SetLineColor(kWhite);
  histo_ratio_data_fxfx->SetMarkerSize(1.25);
  histo_ratio_data_fxfx->Draw("P SAME");
  
  TH1D* histo_ratio_data_sh=(TH1D*) h_sum_all_sh->Clone("histo_ratio_data_sh");
  histo_ratio_data_sh->Divide(h_data_1[0]);
  histo_ratio_data_sh->GetXaxis()->SetTitle("");
  histo_ratio_data_sh->SetMarkerStyle(21);
  histo_ratio_data_sh->SetMarkerColor(kViolet+1);
  histo_ratio_data_sh->SetLineColor(kWhite);
  histo_ratio_data_sh->SetMarkerSize(1.25);
  histo_ratio_data_sh->Draw("P SAME");
  histo_ratio_data_sh->Write();
  
  if ( item=="diBJets_DPhi")
    {  TLine* l=new TLine (0, 1, 3.15, 1);
  l->SetLineWidth(2);
  l->SetLineColor(kRed);
  l->Draw();
  
    }

    if( item=="diBJets_M")
    { TLine* l=new TLine (0, 1, 1200, 1);
  l->SetLineWidth(2);
  l->SetLineColor(kRed);
  l->Draw();
    }       
 
  if( item=="diBJets_DR")
    { TLine* l=new TLine (0.4, 1, 5, 1);
  l->SetLineWidth(2);
  l->SetLineColor(kRed);
  l->Draw();
    }
    if ( item=="diBJets_DY")
    {  TLine* l=new TLine (0, 1,4, 1);
  l->SetLineWidth(2);
  l->SetLineColor(kRed);
  l->Draw();
    }
  TLegend *lratio=new TLegend(0.7, 0.78, 0.90, 0.95);
  lratio->AddEntry(histo_ratio_data_fxfx, "MG FxFx", "P");
  lratio->AddEntry(histo_ratio_data_sh, "Sherpa", "P");
  lratio->SetLineColor(0);
  lratio->SetLineStyle(1);
  lratio->SetLineWidth(1);
  lratio->SetFillColor(0);
  lratio->SetFillStyle(1001);
  lratio->SetTextColor(1);
  lratio->SetTextFont(42);
  lratio->Draw("SAME");

  c1_test->Print(PATH+channel+"_"+region+"_"+item+"_ratio.png");

  //END RATIO

   sum->Write();
   h_xy->Delete();
   delete TC_histo; 
}

// ==================================================================================================================================
// ==================================================================================================================================

void draw_histo_bkg_sub(TFile* OutPutFile, TString item, TString channel, std::vector<TString> h_name_v, TString h_name_data_bs, int REBIN, float x_min, float x_max, float y_min, float y_max, TString x_name, TString y_name, bool do_log)
{
  std::cout << "... Draw histo for data and bkg sub: "<< h_name_data_bs << std::endl;
  OutPutFile->cd();

  TH1D *h_bkg = (TH1D*) OutPutFile->Get(h_name_v[1])->Clone("h_bkg");

  if (h_bkg) {
          h_bkg ->Sumw2(); //funzione che mi assicura il corretto calcolo dell'errore statistico anche per hist con vari scaling
      std::cout << "... Bkg[1] --> " << h_name_v[1] <<  std::endl;
      std::cout << "... Entries: " << h_bkg->Integral() <<  std::endl;
  }

  h_bkg->SetTitle("h_bkg_total");
  h_bkg->Write();

  TH1D *h_data_bs = (TH1D*)OutPutFile->Get(h_name_v[0])->Clone("h_data_bs");
  std::cout<< "... Sum hist data: " << h_data_bs->Integral() <<std::endl;
  std::cout<< "... Sum hist bkg: " << h_bkg->Integral() <<std::endl;

    if (h_data_bs) h_data_bs->Sumw2();
    h_data_bs->Add(h_bkg,-1.);
    h_data_bs->SetTitle(h_name_data_bs);
    h_data_bs->SetName(h_name_data_bs);
    std::cout<< "... Sum hist data - bkg: " << h_data_bs->Integral() <<std::endl;
    h_data_bs->Write();
    
    OutPutFile->cd();
   TString name_histo="D-BKG";
   TCanvas * TC_histo = new TCanvas(name_histo,name_histo,0,0,1200,1200);
   TC_histo->SetFillColor(0);
   TC_histo->SetBorderMode(0);
   TC_histo->SetBorderSize(2);
   TC_histo->SetTickx(1);
   TC_histo->SetTicky(1); 
   TC_histo->SetLeftMargin(0.14);
   TC_histo->SetRightMargin(0.05);
   TC_histo->SetTopMargin(0.05);
   TC_histo->SetBottomMargin(0.13);
   TC_histo->SetFrameBorderMode(0);
   TC_histo->SetFrameBorderMode(0);

 if (do_log) {TC_histo->SetLogy();}
 TC_histo->cd();
 
 if ((x_min==-999)&&(x_max=-999)&&(y_min=-999)&&(y_max=-999)) {
    
    y_min=0.1;
    if ( item=="diBJets_DPhi")
  { x_min=0;
  x_max=3.15;
  y_max=70000;
  }

  if( item=="diBJets_M")
  { x_min=0;
  x_max=1200;
  y_max=100000000;
  }      
  
  if ( item=="diBJets_DR")
  { x_min=0.4;
  x_max=5;
  y_max=75000;
  }
  if ( item=="diBJets_DY")
  { x_min=0;
  x_max=4;
  y_max=100000;
  }
    }       

   TH2D * h_xy= new TH2D("h_xy","h_xy",(x_max-x_min)/REBIN,x_min,x_max, 10, y_min, y_max );
   h_xy->GetXaxis()->SetTitle(x_name);
   h_xy->GetYaxis()->SetTitle(y_name);
   
  /// DRAW PLOTS //// 
  gStyle->SetOptStat(0000);
  setstat();
  SetAtlasStyle();

  h_xy->Draw();
  
    h_xy->GetXaxis()->SetLabelSize(0.03);
    h_xy->GetXaxis()->SetTitleSize(0.04);
    h_xy->GetYaxis()->SetLabelSize(0.03);
    h_xy->GetYaxis()->SetTitleSize(0.04);
    h_xy->GetXaxis()->SetTitleOffset(1);
    h_xy->GetYaxis()->SetTitleOffset(1.6);

  h_xy->Draw("axis SAME");
  h_data_bs->Draw("HISTO SAME");
  
  TLatex* tex0 = new TLatex();
    tex0->SetNDC();
    tex0->SetTextSize(0.03);
    tex0->DrawLatex(0.2,0.88,"#font[72]{ATLAS} Work In Progress" );

    TLatex* tex1 = new TLatex();
    tex1->SetNDC();
    tex1->DrawLatex(0.2,0.88-0.05,"#font[42]{13 TeV, 140 fb^{-1}}" );
    
    TLatex* tex2 = new TLatex();
    tex2->SetNDC();
    tex2->DrawLatex(0.2,0.88-0.09,"#font[42]{Z(#rightarrow ll)+#geq 2 b-jet}" );
  
  TLegend *leg1 = new TLegend(0.50,0.65,0.50+0.30,0.93);
  leg1->AddEntry(h_data_bs, "valori ", "F");

  TC_histo->Print("/home/pisansthefirst/Scrivania/istogrammi/mcAll_Lep/OUTPUT/"+channel+"/"+item+"/"+"D-BKG_"+channel+"_"+item+".png");
  
  h_bkg->Delete();
  h_data_bs->Delete();

  std::cout<< "=============================================== " <<std::endl;
  std::cout<< "" <<std::endl;

}

// ==================================================================================================================================
// ==================================================================================================================================

void draw_histo_div_1D_1D(TFile *OutPutFile, TString item, TString ch, TString unf_meth, TString SYS, TString name_histo_truth1, TString name_histo_truth2, TString name_unf_fact_plot, double WEIGHT1, double WEIGHT2 , int REBIN, float x_min, float x_max, float y_min, float y_max, TString x_name, TString y_name)
{
  TCanvas * TC_histo = new TCanvas("TC_histo","TC_histo", 1200, 1200);
  TC_histo->cd();
  TC_histo->SetFillColor(0);
    TC_histo->SetBorderMode(0);
    TC_histo->SetBorderSize(2);
    TC_histo->SetTickx(1);
    TC_histo->SetTicky(1);
    TC_histo->SetLeftMargin(0.14);
    TC_histo->SetRightMargin(0.05);
    TC_histo->SetTopMargin(0.05);
    TC_histo->SetBottomMargin(0.13);
    TC_histo->SetFrameBorderMode(0);
    TC_histo->SetFrameBorderMode(0);
    
  TH1D* histo_file1;
  TH1D* histo_file2;
  TH1D *histo_file1op2;
  setstat();
  TString name_histo_truth12=name_histo_truth2;
  
  OutPutFile->cd();
  histo_file1 = (TH1D*)OutPutFile->Get(name_histo_truth1)->Clone("histo_file1"); 
  histo_file2  = (TH1D*)OutPutFile->Get(name_histo_truth2)->Clone("histo_file2");

  if (!histo_file1 ) {std::cout<<" Histos not found, CHECK NAME "<<name_histo_truth1 <<std::endl;}
  if (!histo_file2 ) {std::cout<<" Histos not found, CHECK NAME "<<name_histo_truth2 <<std::endl;}
  
  setstat();
  histo_file1->Rebin(REBIN);
  histo_file1->SetStats(false);
  histo_file1->GetXaxis()->SetRangeUser(x_min,x_max);

  histo_file2->Rebin(REBIN);
  histo_file2->SetStats(false);
  histo_file2->GetXaxis()->SetRangeUser(x_min,x_max);

  histo_file1->Sumw2();
  histo_file2->Sumw2();

    histo_file1op2 = (TH1D*) histo_file1->Clone("histo_file1op2");
  for (int i=0;i<histo_file1op2->GetBinContent(i)+1;i++) 
  { histo_file1op2->SetBinError(i,0.);
  }

   histo_file1op2->Divide(histo_file2); 

  TString h_1op2_title = name_unf_fact_plot;
  histo_file1op2->SetTitle((const char *)h_1op2_title);
  histo_file1op2->SetName((const char *)h_1op2_title);

//PLOT
  gStyle->SetOptStat(0000);
  setstat();
  SetAtlasStyle();
  if ((x_min==-999)&&(x_max=-999)&&(y_min=-999)&&(y_max=-999)) {
    
    y_min=0.001;
    if ( item=="diBJets_DPhi")
    { x_min=0;
    x_max=3.15;
    y_max=8;
    }

    if( item=="diBJets_M")
    { x_min=0;
    x_max=1200;
    y_max=10;
    }       
    
    if ( item=="diBJets_DR")
  { x_min=0.4;
  x_max=5;
  y_max=10;
  }
  if ( item=="diBJets_DY")
  { x_min=0;
  x_max=4;
  y_max=10;
  }
  }   
  
   TH2D *h_xy=new TH2D("h_xy", "h_xy", 100, x_min, x_max, 100, y_min, y_max);
     h_xy->GetXaxis()->SetTitle(x_name);
     h_xy->GetYaxis()->SetTitle("U_{MC}");

     //bin_by_bin solo 
     if (unf_meth=="bin-by-bin"){
     if (ch=="Z") h_xy->GetYaxis()->SetTitle("Bin-by-bin correction (Z channel)");
     if (ch=="W") h_xy->GetYaxis()->SetTitle("Bin-by-bin correction (W channel)");
     }

  h_xy->Draw();
  histo_file1op2->GetXaxis()->SetRangeUser(x_min, x_max);
  histo_file1op2->SetLineColor(1);
  histo_file1op2->SetMarkerColor(1);
  histo_file1op2->SetMarkerStyle(21);
  histo_file1op2->SetMarkerSize(1.2);
  histo_file1op2->SetLineWidth(2);
  histo_file1op2->Draw("SAME");
  
   TLatex* tex0 = new TLatex();
    tex0->SetNDC();
    tex0->SetTextSize(0.03);
    tex0->DrawLatex(0.2,0.88,"#font[72]{ATLAS} Work In Progress" );

    TLatex* tex1 = new TLatex();
    tex1->SetNDC();
    tex1->DrawLatex(0.2,0.88-0.05,"#font[42]{13 TeV, 140 fb^{-1}}" );
    
    TLatex* tex2 = new TLatex();
    tex2->SetNDC();
    tex2->DrawLatex(0.2,0.88-0.09,"#font[42]{Z(#rightarrow ll)+#geq 2 b-jet}" );
  
  TC_histo->Print("/home/pisansthefirst/Scrivania/istogrammi/mcAll_Lep/OUTPUT/"+ch+"/"+item+"/"+item+"_"+unf_meth+"_"+ch+"_"+SYS+".png");
  OutPutFile->cd();
  histo_file1op2->Write();
  
  h_xy->Delete();
  delete TC_histo;
}

// ==================================================================================================================================
// ==================================================================================================================================

void draw_histo_fake_mult(TFile * OutPutFile, TString item, TString h_name, TString h_fake_corr_name, TString h_name_fcorr)
{ OutPutFile->cd();
  TH1D* histo = (TH1D*)OutPutFile->Get(h_name)->Clone("histo"); 
  TH1D *histo_fake_corr = (TH1D*)OutPutFile->Get(h_fake_corr_name)->Clone("histo_fake_corr");
  TH1D * histo_corr = (TH1D *)histo->Clone(h_name_fcorr);
  histo_corr->Sumw2();
  histo_corr->Multiply(histo_fake_corr);
  histo_corr->SetTitle(h_name_fcorr);
  histo_corr->SetName(h_name_fcorr);
  histo_corr->Write();

}

// ==================================================================================================================================
// ==================================================================================================================================

void fill_edges(Double_t edges[], std::vector<Double_t> vect)
{ 
for (int i=0; i<=vect.size(); i++)
  {edges[i]=vect[i];
  }
}

void convert_reco_xs(TFile * OutPutFile, double lumi, TString item, TString channel, TString h_reco_name, TString h_xs_name,  TString h_truthZ, std::vector<float> pub, std::vector<float> pub_err, TString h_sh, float x_min, float x_max, float y_min, float y_max, TString x_name, TString y_name, int REBIN, bool do_log)
{
   gStyle->SetOptStat(0000);
   setstat();
   SetAtlasStyle();
   OutPutFile->cd();
  
  TH1D* h_reco = (TH1D*)OutPutFile->Get(h_reco_name)->Clone("h_reco");
  TH1D* h_xs = (TH1D*)OutPutFile->Get(h_reco_name)->Clone("h_xs");
  
  TH1D* h_bin = (TH1D*)OutPutFile->Get(h_truthZ)->Clone("h_bin");
  TH1D* h_truth=(TH1D*)OutPutFile->Get(h_truthZ)->Clone("h_truth");
  
  TH1D* h_bin_sh = (TH1D*)OutPutFile->Get(h_sh)->Clone("h_bin_sh");
  TH1D* h_truth_sh=(TH1D*)OutPutFile->Get(h_sh)->Clone("h_truth_sh"); 
  
  for (int i=0;i<h_reco->GetNbinsX()+1;i++){
    float ent=h_reco->GetBinContent(i);
    float err=h_reco->GetBinError(i);
    float wid=h_reco->GetBinWidth(i)*lumi*2*1000; 
     h_xs->SetBinContent(i, ent/wid);
     h_xs->SetBinError(i, err/wid);
     }
  for (int i=0; i<h_bin->GetNbinsX()+1; i++){
    float ent_tr=h_bin->GetBinContent(i);
    float err_tr=h_bin->GetBinError(i);
    float wid_tr=h_bin->GetBinWidth(i)*lumi*2*1000;
    h_truth->SetBinContent(i, ent_tr/wid_tr);
    h_truth->SetBinError(i, err_tr/wid_tr);
    }
    for (int i=0; i<h_bin_sh->GetNbinsX()+1; i++){
    float ent_tr_sh=h_bin_sh->GetBinContent(i);
    float err_tr_sh=h_bin_sh->GetBinError(i);
    float wid_tr_sh=h_bin_sh->GetBinWidth(i)*lumi*2*1000; 
    h_truth_sh->SetBinContent(i, ent_tr_sh/wid_tr_sh);
    h_truth_sh->SetBinError(i, err_tr_sh/wid_tr_sh);
    }

  h_xs->SetName(h_xs_name);
  h_xs->SetTitle(h_xs_name);
  
  h_truth->SetName("cross_section_"+item+"_"+channel+"_truth");
  h_truth->SetTitle("cross_section_"+item+"_"+channel+"_truth");
  
  h_truth_sh->SetName("cross_section_Y_truth_sh");
  h_truth_sh->SetTitle("cross_section_Y_truth_sh"); 
  
  TCanvas * TC_histo = new TCanvas("TC_histo","TC_histo", 1200, 1200);
  TC_histo->cd();
    TC_histo->SetFillColor(0);
    TC_histo->SetBorderMode(0);
    TC_histo->SetBorderSize(2);
    TC_histo->SetTickx(1);
    TC_histo->SetTicky(1);
    TC_histo->SetLeftMargin(0.14);
    TC_histo->SetRightMargin(0.05);
    TC_histo->SetTopMargin(0.05);
    TC_histo->SetBottomMargin(0.13);
    TC_histo->SetFrameBorderMode(0);
    TC_histo->SetFrameBorderMode(0);
  if (do_log) {TC_histo->SetLogy();}
  
  Double_t edges[pub.size()];
  
  if ((x_min==-999)&&(x_max=-999)&&(y_min=-999)&&(y_max=-999)) {
    
    if( item=="diBJets_M")
    {x_min=0;
    y_min=0.0000002;
    x_max=1200;
    y_max=2;
    std::vector<Double_t> vec={0, 10, 30, 55, 80, 105, 130, 160, 190, 220, 260, 310, 400, 600, 900, 1200};
    fill_edges(edges, vec);
    } 
   if ( item=="diBJets_DPhi")
      {x_min=0;
      y_min=0.0001;
      x_max=3.15;
      y_max=1.2;
      std::vector<Double_t> vec={0, 0.2625, 0.525, 0.7875, 1.05, 1.3125, 1.575, 1.8375, 2.1, 2.3625, 2.625, 2.8875, 3.15};
      fill_edges(edges, vec);
      } 
      if ( item=="diBJets_DR")
      { x_min=0.4;
        y_min=0.0001;
        x_max=5;
        y_max=1.2;
        std::vector<Double_t> vec={0, 0.4, 0.9, 1.3, 1.8, 2.3, 2.7, 3.1, 3.5, 4, 5};
        fill_edges(edges, vec);
      } 
      if ( item=="diBJets_DY")
      { x_min=0;
        y_min=0.0001;
        x_max=4;
        y_max=1.2;
      } 
    
  }
    TH1D* h_pub=new TH1D("histo_pubbl", "histo_pubbl", pub.size(), edges);
       for(int i=0; i<=(pub.size()); i++)
      { 
       h_pub->SetBinContent(i+1, pub[i]);
       h_pub->SetBinError(i+1, pub_err[i]); 
      } 
    
   TH2D *h_xy=new TH2D("h_xy", "h_xy", 100, x_min, x_max, 100, y_min, y_max);
     h_xy->GetXaxis()->SetTitle(x_name);
     h_xy->GetYaxis()->SetTitle(y_name);
     h_xy->GetXaxis()->SetTitle(x_name);
     h_xy->GetYaxis()->SetTitle("Sezione d'urto differenziale [pb/GeV]");
     h_xy->GetXaxis()->SetLabelSize(0.04*3./2.);
     h_xy->GetXaxis()->SetLabelSize(0.04);
     h_xy->GetXaxis()->SetTitleSize(0.04);
     h_xy->GetYaxis()->SetLabelSize(0.03);
     h_xy->GetYaxis()->SetTitleSize(0.05);
     h_xy->GetXaxis()->SetTitleOffset(1);
     h_xy->GetYaxis()->SetTitleOffset(1.2);
     h_xy->Draw();
    
  h_truth->SetMarkerStyle(22);
  h_truth->SetMarkerColor(kPink+10);
  h_truth->SetLineColor(kPink+10);
  h_truth->SetMarkerSize(1.25);
  h_truth->Write();
  
  h_xs->SetMarkerStyle(20);
  h_xs->Write();
  
  h_truth_sh->SetMarkerStyle(21);
  h_truth_sh->SetMarkerColor(kViolet+1);
  h_truth_sh->SetLineColor(kViolet+1);
  h_truth_sh->SetMarkerSize(1.25);
  
  h_truth->Draw("SAME");
  h_truth_sh->Draw("SAME");
  h_xs->Draw("SAME");
  
  TLegend *l=new TLegend(0.70, 0.7, 0.90, 0.93);
  l->AddEntry(h_xs, "Dati ricostruiti", "PL");
  l->AddEntry(h_truth, "Truth FxFx", "PL");
  l->AddEntry(h_truth_sh, "Truth Sherpa", "PL");
  l->SetLineColor(0);
  l->SetLineStyle(1);
  l->SetLineWidth(1);
  l->SetFillColor(0);
  l->SetFillStyle(1001);
  l->SetTextColor(1);
  l->SetTextFont(42);
  l->Draw("SAME");
  
  TLatex* tex0 = new TLatex();
    tex0->SetNDC();
    tex0->SetTextSize(0.03);
    tex0->DrawLatex(0.2,0.88,"#font[72]{ATLAS} Work In Progress" );

    TLatex* tex1 = new TLatex();
    tex1->SetNDC();
    tex1->DrawLatex(0.2,0.88-0.05,"#font[42]{13 TeV, 140 fb^{-1}}" );
    
    TLatex* tex2 = new TLatex();
    tex2->SetNDC();
    tex2->DrawLatex(0.2,0.88-0.09,"#font[42]{Z(#rightarrow ll)+#geq 2 b-jet}" );

  TC_histo->Print("/home/pisansthefirst/Scrivania/istogrammi/mcAll_Lep/OUTPUT/"+channel+"/"+item+"/cross_section_"+ item +"_"+ channel +".png");
  
  ///RATIO
  TCanvas *c1_test = new TCanvas("c1_test", "c1_test",0.,0.,800,1600.*6./9.);
  c1_test->cd();
  TPad *p1 = new TPad("pad1","top left pad",                    0.0, 1./3., 1.0, 1.0);
  TPad *p1_ratio = new TPad("pad1_ratio","bottom left pad"     , 0.0, 0.0, 1.0, 1./3.);
   p1->SetTopMargin(0.05);
   p1->SetBottomMargin(0.);
   p1->SetRightMargin(0.05);
   p1->SetLeftMargin(0.16);
   p1->SetFillStyle(4000);
   p1->SetLogx();
   if(do_log){p1->SetLogy(); }

   p1_ratio->SetTopMargin(0.);
   p1_ratio->SetBottomMargin(0.48);
   p1_ratio->SetBottomMargin(0.30);
   p1_ratio->SetRightMargin(0.05);
   p1_ratio->SetLeftMargin(0.16);
   p1_ratio->SetFillStyle(4000);

  p1->Draw();
  p1_ratio->Draw();

  p1->cd();
  
    tex0->SetNDC();
    tex0->SetTextSize(0.03);
    tex0->DrawLatex(0.2,0.88,"#font[72]{ATLAS} Work In Progress" );
    tex1->SetNDC();
    tex1->DrawLatex(0.2,0.88-0.05,"#font[42]{13 TeV, 140 fb^{-1}}" );
    tex2->SetNDC();
    tex2->DrawLatex(0.2,0.88-0.09,"#font[42]{Z(#rightarrow ll)+#geq 2 b-jet}" );

  h_xy->GetXaxis()->SetTitle(x_name);
  h_xy->GetYaxis()->SetTitle("Sezione d'urto differenziale [pb/GeV]");
  h_xy->GetXaxis()->SetLabelSize(0.04*3./2.);
  h_xy->GetXaxis()->SetLabelSize(0.04);
  h_xy->GetXaxis()->SetTitleSize(0.04);
  h_xy->GetYaxis()->SetLabelSize(0.03);
  h_xy->GetYaxis()->SetTitleSize(0.05);
  h_xy->GetXaxis()->SetTitleOffset(1);
  h_xy->GetYaxis()->SetTitleOffset(1.2);
  h_xy->GetXaxis()->SetLabelSize(0.);
  
  h_xy->DrawCopy();
  
  h_truth->SetMarkerStyle(22);
  h_truth->SetMarkerColor(kPink+10);
  h_truth->SetLineColor(kPink+10);
  h_truth->SetMarkerSize(1.25);
  h_truth->Draw("SAME");
  
  h_xs->SetMarkerStyle(20);
  h_xs->Draw("SAME");
  
  h_truth_sh->Draw("SAME");
  h_truth_sh->SetMarkerStyle(21);
  h_truth_sh->SetMarkerColor(kViolet+1);
  h_truth_sh->SetLineColor(kViolet+1);
  h_truth_sh->SetMarkerSize(1.25);
  h_truth_sh->Draw("SAME");
  
  TLegend *l1=new TLegend(0.70, 0.7, 0.90, 0.93);
  l1->AddEntry(h_xs, "Dati ricostruiti", "PL");
  l1->AddEntry(h_truth, "Truth FxFx", "PL");
  l1->AddEntry(h_truth_sh, "Truth Sherpa", "PL");
  l1->SetLineColor(0);
  l1->SetLineStyle(1);
  l1->SetLineWidth(1);
  l1->SetFillColor(0);
  l1->SetFillStyle(1001);
  l1->SetTextColor(1);
  l1->SetTextFont(42);
  l1->Draw("SAME");

  TH2D * h_rdt=new TH2D("h_rdt","h_rtd",(x_max-x_min)/REBIN,12,x_max, 50, 0.25, 1.75);

  h_rdt->GetYaxis()->SetTickLength(0.04);
  h_rdt->GetYaxis()->SetNdivisions(305);
  h_rdt->GetXaxis()->SetTitleSize(0.1);
  h_rdt->GetYaxis()->SetLabelSize(0.1);
  h_rdt->GetXaxis()->SetLabelSize(0.1);
  h_rdt->GetYaxis()->SetTitleSize(0.1);
  h_rdt->GetXaxis()->SetTitleOffset(1);
  h_rdt->GetYaxis()->SetTitleOffset(0.6);
  h_rdt->GetXaxis()->SetTitle(x_name);
  h_rdt->GetYaxis()->SetTitle("Montecarlo/Dati");

    tex0->SetTextSize(0.03);
    tex0->DrawLatex(0.2,0.88,"#font[72]{ATLAS} Work In Progress" );
    tex1->SetNDC();
    tex1->DrawLatex(0.2,0.88-0.05,"#font[42]{13 TeV, 140 fb^{-1}}" );
    tex2->SetNDC();
    tex2->DrawLatex(0.2,0.88-0.09,"#font[42]{Z(#rightarrow ll)+#geq 2 b-jet}" );

  c1_test->cd();
  p1_ratio->cd();
  h_rdt->Draw();
  
  //ratio MC/D
  TH1D* histo_ratio_data= (TH1D*) h_truth->Clone("histo_ratio_data");
  histo_ratio_data->Divide(h_xs);
  histo_ratio_data->GetXaxis()->SetTitle("");
  histo_ratio_data->SetMarkerStyle(22);
  histo_ratio_data->SetMarkerSize(1.25);
  histo_ratio_data->SetMarkerColor(kPink+10);
  histo_ratio_data->SetLineColor(kWhite);
  histo_ratio_data->Draw("SAME");

  TH1D* histo_ratio_data_sh= (TH1D*) h_truth_sh->Clone("histo_ratio_data_sh");
  histo_ratio_data_sh->Divide(h_xs);
  histo_ratio_data_sh->GetXaxis()->SetTitle("");
  histo_ratio_data_sh->SetMarkerStyle(21);
  histo_ratio_data_sh->SetMarkerSize(1.25);
  histo_ratio_data_sh->SetMarkerColor(kViolet+1);
  histo_ratio_data_sh->SetLineColor(kWhite);
  histo_ratio_data_sh->Draw("SAME"); 

  TLegend *lratio=new TLegend(0.7, 0.78, 0.90, 0.95);
  lratio->AddEntry(histo_ratio_data, "MG FxFx", "P");
  lratio->AddEntry(histo_ratio_data_sh, "Sherpa", "P");
  lratio->SetLineColor(0);
  lratio->SetLineStyle(1);
  lratio->SetLineWidth(1);
  lratio->SetFillColor(0);
  lratio->SetFillStyle(1001);
  lratio->SetTextColor(1);
  lratio->SetTextFont(42);
  lratio->Draw("SAME");
  
  if ( item=="diBJets_DPhi")
    {  TLine* l=new TLine (0, 1, 3.15, 1);
  l->SetLineWidth(2);
  l->SetLineColor(kRed);
  l->Draw();
    }

    if( item=="diBJets_M")
    { TLine* l=new TLine (0, 1, 1200, 1);
  l->SetLineWidth(2);
  l->SetLineColor(kRed);
  l->Draw();
    }    
    
    if( item=="diBJets_DR")
    { TLine* l=new TLine (0.4, 1, 5, 1);
  l->SetLineWidth(2);
  l->SetLineColor(kRed);
  l->Draw();
    } 
    if ( item=="diBJets_DY")
    {  TLine* l=new TLine (0, 1, 4, 1);
  l->SetLineWidth(2);
  l->SetLineColor(kRed);
  l->Draw();
    }
 
  c1_test->Print("/home/pisansthefirst/Scrivania/istogrammi/mcAll_Lep/OUTPUT/"+channel+"/"+item+"/cross_section_"+ item +"_"+ channel +"_ratio.png");

  //END RATIO
  //DATI PUBBLICAZIONE
  if(channel=="Lep")
  {
  TCanvas *c2= new TCanvas("c2", "c2",0.,0.,800,1600.*6./9.);
  c2->cd();
  TPad *p = new TPad("pad1","top left pad",                    0.0, 1./3., 1.0, 1.0);
  TPad *p_ratio = new TPad("pad1_ratio","bottom left pad"     , 0.0, 0.0, 1.0, 1./3.);
   p->SetTopMargin(0.05);
   p->SetBottomMargin(0.);
   p->SetRightMargin(0.05);
   p->SetLeftMargin(0.16);
   p->SetFillStyle(4000);
   if(do_log){p->SetLogy(); }

   p_ratio->SetTopMargin(0.);
   p_ratio->SetBottomMargin(0.48);
   p_ratio->SetBottomMargin(0.30);
   p_ratio->SetRightMargin(0.05);
   p_ratio->SetLeftMargin(0.16);
   p_ratio->SetFillStyle(4000);

  p->Draw();
  p_ratio->Draw();

  p->cd();
  
    tex0->SetNDC();
    tex0->SetTextSize(0.03);
    tex0->DrawLatex(0.2,0.88,"#font[72]{ATLAS} Work In Progress" );
    tex1->SetNDC();
    tex1->DrawLatex(0.2,0.88-0.05,"#font[42]{13 TeV, 140 fb^{-1}}" );
    tex2->SetNDC();
    tex2->DrawLatex(0.2,0.88-0.09,"#font[42]{Z(#rightarrow ll)+#geq 2 b-jet}" );
    
     if ( item=="diBJets_DR")
      { x_min=0.4;
        y_min=0.0001;
        x_max=5;
        y_max=10;
      } 

  TH2D * h_p=new TH2D("h_p","h_p",100, x_min, x_max, 100, y_min, y_max);
  h_p->GetXaxis()->SetTitle(x_name);
  h_p->GetYaxis()->SetTitle("Sezione d'urto differenziale [pb/GeV]");
  h_p->GetXaxis()->SetLabelSize(0.04*3./2.);
  h_p->GetXaxis()->SetLabelSize(0.04);
  h_p->GetXaxis()->SetTitleSize(0.04);
  h_p->GetYaxis()->SetLabelSize(0.03);
  h_p->GetYaxis()->SetTitleSize(0.05);
  h_p->GetXaxis()->SetTitleOffset(1);
  h_p->GetYaxis()->SetTitleOffset(1.2);
  h_p->GetXaxis()->SetLabelSize(0.);
  h_p->DrawCopy();
  
  h_pub->SetMarkerStyle(20);
  h_pub->SetMarkerStyle(20);
  h_pub->SetMarkerSize(1.25);
  h_pub->SetMarkerColor(kGreen+1);
  h_pub->SetLineColor(kGreen+1);
  h_xs->Draw("SAME");
  h_pub->Draw("SAME");
  
  TLegend *l1=new TLegend(0.70, 0.7, 0.90, 0.93);
  l1->AddEntry(h_xs, "Dati ricostruiti", "PL");
  l1->AddEntry(h_pub, "Dati pubblicati", "PL");
  l1->SetLineColor(0);
  l1->SetLineStyle(1);
  l1->SetLineWidth(1);
  l1->SetFillColor(0);
  l1->SetFillStyle(1001);
  l1->SetTextColor(1);
  l1->SetTextFont(42);
  l1->Draw("SAME");
  
    tex0->SetTextSize(0.03);
    tex0->DrawLatex(0.2,0.88,"#font[72]{ATLAS} Work In Progress" );
    tex1->SetNDC();
    tex1->DrawLatex(0.2,0.88-0.05,"#font[42]{13 TeV, 140 fb^{-1}}" );
    tex2->SetNDC();
    tex2->DrawLatex(0.2,0.88-0.09,"#font[42]{Z(#rightarrow ll)+#geq 2 b-jet}" );
  
  p_ratio->cd();

  TH2D * h_rdt_p=new TH2D("h_rdt_p","h_rdt_p",(x_max-x_min)/REBIN,x_min,x_max, 50, 0.001, 1.99);

  h_rdt_p->GetYaxis()->SetTickLength(0.04);
  h_rdt_p->GetYaxis()->SetNdivisions(305);
  h_rdt_p->GetXaxis()->SetTitleSize(0.1);
  h_rdt_p->GetYaxis()->SetLabelSize(0.1);
  h_rdt_p->GetXaxis()->SetLabelSize(0.1);
  h_rdt_p->GetYaxis()->SetTitleSize(0.1);
  h_rdt_p->GetXaxis()->SetTitleOffset(1);
  h_rdt_p->GetYaxis()->SetTitleOffset(0.6);
  h_rdt_p->GetXaxis()->SetTitle(x_name);
  h_rdt_p->GetYaxis()->SetTitle("Dati ric./Dati pubbl.");
  
  h_rdt_p->Draw();

  TH1D* data_pub= (TH1D*) h_xs->Clone("data_pub");
  data_pub->Divide(h_pub);
  data_pub->GetXaxis()->SetTitle("");
  data_pub->SetMarkerStyle(20);
  data_pub->SetMarkerSize(1.25);
  data_pub->SetMarkerColor(kPink-7);
  data_pub->SetLineColor(kWhite);
  data_pub->Draw("SAME");
  
  if ( item=="diBJets_DPhi")
    {  TLine* l=new TLine (0, 1, 3.15, 1);
  l->SetLineWidth(2);
  l->SetLineColor(kRed);
  l->Draw();
    }

    if( item=="diBJets_M")
    { TLine* l=new TLine (0, 1, 1200, 1);
  l->SetLineWidth(2);
  l->SetLineColor(kRed);
  l->Draw();
    } 
    
     if( item=="diBJets_DR")
    { TLine* l=new TLine (0.4, 1, 5, 1);
  l->SetLineWidth(2);
  l->SetLineColor(kRed);
  l->Draw();
    } 
    if ( item=="diBJets_DY")
    {  TLine* l=new TLine (0, 1,4, 1);
  l->SetLineWidth(2);
  l->SetLineColor(kRed);
  l->Draw();
    }
  h_pub->Write();
  
  c2->Print("/home/pisansthefirst/Scrivania/istogrammi/mcAll_Lep/OUTPUT/"+channel+"/"+item+"/cross_section_publ_"+ item + "_"+ channel +".png");
  h_p->Delete();
}
  h_xy->Delete();
  delete TC_histo;
}
