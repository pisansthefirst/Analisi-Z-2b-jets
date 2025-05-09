#include "/home/pisansthefirst/Scrivania/istogrammi/mcAll_Lep/macro/StdArg.hpp"
#include "/home/pisansthefirst/Scrivania/istogrammi/mcAll_Lep/macro/comp_files_merge_functions_up_noAP.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <vector>

int main(int argc, char *argv[] )
{
	TString item, channel, region;
      
	StdArg arg(argc,argv);
  	arg.keys << "-item";
  	arg.keys << "-channel";
  	arg.keys << "-region";
  	
  	try {  // parse command line
    	arg.Process();
    	if ( arg.Key("-item")       ) item        = arg.Get<string>("-item");
    	if ( arg.Key("-channel") ) channel = arg.Get<string>("-channel");
    	if ( arg.Key("-region") ) region = arg.Get<string>("-region");
	}
 	catch (StdArg::BadInput) {
    	if (argc > 1) cout<< "Input error" <<endl;
    	return 1;
  	}
  	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  	
  	//setup grafici
  	double lumi = 140.0;
	int rebin=1;
	float flag = 1;
	float flagb, flagb_sh;
	float flagc, flagc_sh;
	float x_min=-999;
	float x_max=-999;
	float y_min=-999;
	float y_max=-999;
	TString x_name;
	TString y_name;
	bool do_log;
	
	//ROOT setup
	gROOT->SetBatch(); //disattiva la visualizzazione delle finestre grafiche
	gROOT->Reset(); //resetta root chiudendo Tfile aperti, svuotando la memoria e può resettare le macro caricate
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TString PATH;
	
	std::vector<TString> file_dibos;
	std::vector<TString> file_stops;
	std::vector<TString> file_stopt;
	std::vector<TString> file_stopWT;
	std::vector<TString> file_ttbar;
	std::vector<TString> file_ZH;
	std::vector<TString> file_Ztau;
	std::vector<TString> file_Zjet;
	std::vector<TString> file_Zjet_sh;
	std::vector<TString> file_data;
	
	if(channel=="El" || channel=="Mu")
	{ PATH="/home/pisansthefirst/Scrivania/istogrammi/mcAll_Lep/INPUTS/file_Giusy/";
	TString ch_label[]={"Zee", "Zmumu"};
        float flags_b[]={0.87, 0.81};
        float flags_c[]={1.06, 1.11};
        
        int i;
        if(channel=="El")
        {  i=0; }
        else if(channel=="Mu")
        { i=1; }
        file_Zjet.push_back("hist-"+ch_label[i]+"jet_FxFx_Giusy.root"); 
	file_Zjet_sh.push_back("hist-"+ch_label[i]+"_sh2211_Yi.root"); }
	
        else 
	{ flagb= 1.13;
	  flagc= 1.12;
	  flagb_sh= 0.84;
	  flagc_sh= 1.09;
	  file_Zjet_sh.push_back("hist-Zlljet_sh2211.root");
	  
	if ( item=="diBJets_DY"){
	PATH="/home/pisansthefirst/Scrivania/istogrammi/mcAll_Lep/INPUTS/file_DY/";
        x_name="#Delta Y_{bb}";
        y_name="Numero di eventi";
        do_log=false; 
        file_Zjet.push_back("hist-Zlljet_FxFx_Giusy.root");
        }
        else 
        { PATH="/home/pisansthefirst/Scrivania/istogrammi/mcAll_Lep/INPUTS/"; 

        if ( item=="diBJets_DR"){
        x_name="#Delta R_{bb}";
        y_name="Numero di eventi";
        do_log=false;
        file_Zjet.push_back("hist-Zlljet_FxFx_Semen.root"); }
        else {file_Zjet.push_back("hist-Zlljet_FxFx_Giusy.root");}
        
        if ( item=="diBJets_DPhi"){
        x_name="#Delta #Phi_{bb}";
        y_name="Numero di eventi";
        do_log=false; }
        
        if ( item=="diBJets_M"){
        x_name="m_{bb} [GeV]";
        y_name="Numero di eventi";
        do_log=true; }
        }
        
        }
        
	file_dibos.push_back("hist-Diboson.root"); 
	file_stops.push_back("hist-stop_s.root"); 
	file_stopt.push_back("hist-stop_t.root"); 
	file_stopWT.push_back("hist-stop_Wt.root"); 
	file_ttbar.push_back("hist-ttbar.root"); 
	file_ZH.push_back("hist-ZH.root"); 
	file_Ztau.push_back("hist-Ztautaujet_FxFx.root");
        file_data.push_back("hist-data.root");
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TFile* f_dibos[1];
	TString file_name_dibos=PATH+file_dibos.at(0); 
	f_dibos[0]=TFile::Open(file_name_dibos, "READ");

	TFile* f_stop[3];
	TString file_name_stops=PATH+file_stops.at(0);
	f_stop[0]= TFile::Open(file_name_stops,"READ");
	TString file_name_stopt=PATH+file_stopt.at(0);
	f_stop[1]= TFile::Open(file_name_stopt,"READ");
	TString file_name_stopWt=PATH+file_stopWT.at(0);
	f_stop[2]= TFile::Open(file_name_stopWt,"READ");

	TFile* f_ttbar[1];
	TString file_name_ttbar=PATH+file_ttbar.at(0);
	f_ttbar[0]= TFile::Open(file_name_ttbar,"READ");

	TFile* f_ZH[1];
	TString file_name_ZH=PATH+file_ZH.at(0); 
	f_ZH[0]=TFile::Open(file_name_ZH, "READ"); 
		
	TFile* f_Zjet[1];
	TString file_name_Zjet=PATH+file_Zjet.at(0); 
	f_Zjet[0]=TFile::Open(file_name_Zjet, "READ");
	
	TFile* f_Zjet_sh[1];
	TString file_name_Zjet_sh=PATH+file_Zjet_sh.at(0); 
	f_Zjet_sh[0]=TFile::Open(file_name_Zjet_sh, "READ"); 
		
	TFile* f_Ztau[1];
	TString file_name_Ztau=PATH+file_Ztau.at(0); 
	f_Ztau[0]=TFile::Open(file_name_Ztau, "READ");	
	
	TFile* f_data[1];
        TString file_name_data=PATH+file_data.at(0);
        f_data[0]= TFile::Open(file_name_data,"READ");
        
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
        TString histo_name_ori_dibos_vect[3]; //dibos
        TString histo_name_ori_stop_vect[3]; //stop
        for (int i=0; i<3; i++){   
          TString dibos_name[]={"WW_", "ZZ_", "WZ_"};
	  histo_name_ori_dibos_vect[i]=dibos_name[i]+channel+"_"+region+"_"+item;
	  
	  TString stop_name[]={"stops_", "stopt_", "stopWt_"};
          histo_name_ori_stop_vect[i]=stop_name[i]+channel+"_"+region+"_"+item;
        }
        
	TString histo_name_ori_ttbar ="ttbar_"+channel+"_"+region+"_"+item; //ttbar
	
	TString histo_name_ori_ZH="ggZllH125_"+channel+"_"+region+"_"+item; //ZH
	
	TString histo_name_ori_Ztau="ZFxFx_"+channel+"_"+region+"_"+item;; //Ztau
	
	/// STRING CONTAINING THE NAMES OF THE HISTOGRAMS///////////////////
	TString sufFC[5]={"_NB","_1B","_1C","_NC","_L"};
	TString histo_name_ori_Zjet_vect[5]; //Zjets MC
	TString histo_name_ori_Zjet_vect_sh[5];
	for (int i=0; i<5; i++){
	  histo_name_ori_Zjet_vect[i] ="ZFxFx"+sufFC[i]+"_"+channel+"_"+region+"_"+item;
	  histo_name_ori_Zjet_vect_sh[i]= "Z"+sufFC[i]+"_"+channel+"_"+region+"_"+item;
	}
	
	TString histo_name_ori_data ="data_"+channel+"_"+region+"_"+item; //dati reali
	//aggiunta del truth
	TString histo_name_ori_truth="ZFxFx_dressed"+channel+"_fiducial2B_truth_"+item;
	TString histo_name_ori_truth_sh="Z_dressed"+channel+"_fiducial2B_truth_"+item;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TString PATH_RESULTS="/home/pisansthefirst/Scrivania/istogrammi/mcAll_Lep/OUTPUT/"+channel+"/"+item+"/";
	
	TString output_name = PATH_RESULTS+"DetLevPlots_"+channel+"_"+region+"_"+item;
	output_name = output_name.Append(".root");
	
	TFile *OutPutFile = new TFile(output_name,"RECREATE","",0); 
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//vettore con i fondi
	std::vector<TString> h_bkg_vect;
	
	std::vector<TString> h_out_stop_vect;
	std::vector<TString> h_out_dibos_vect;
        for(int i=0; i<3; i++){
	  TString stop_name_vect[]={ "h_stops", "h_stopt", "h_stopWt"};
	  h_out_stop_vect.push_back(stop_name_vect[i]);
	  draw_stuck_histo(item, &f_stop[i], 1, lumi, OutPutFile, histo_name_ori_stop_vect[i], rebin, 0, h_out_stop_vect[i], flag);
	  
	  TString dibos_name_vect[]={"h_WW", "h_ZZ", "h_WZ"};
	  h_out_dibos_vect.push_back(dibos_name_vect[i]+"_"+channel);
	  draw_stuck_histo(item, f_dibos, 1, lumi, OutPutFile, histo_name_ori_dibos_vect[i], rebin, 0, h_out_dibos_vect[i], flag);
	}
	
	h_bkg_vect.push_back("h_stop");

	macro::merge_hists(OutPutFile, h_out_stop_vect,h_bkg_vect[0]);
	
	h_bkg_vect.push_back("h_dibos");
	macro::merge_hists(OutPutFile, h_out_dibos_vect,h_bkg_vect[1]);
	
	h_bkg_vect.push_back("h_ttbar");
	draw_stuck_histo(item,f_ttbar, 1, lumi, OutPutFile, histo_name_ori_ttbar, rebin, 0,h_bkg_vect[2], flag);
      
	h_bkg_vect.push_back("h_Ztau");
        draw_stuck_histo(item, f_Ztau, 1, lumi, OutPutFile, histo_name_ori_Ztau, rebin, 0,h_bkg_vect[3], flag);
    
        h_bkg_vect.push_back("h_ZH");
        draw_stuck_histo(item, f_ZH, 1, lumi, OutPutFile, histo_name_ori_ZH, rebin, 0,h_bkg_vect[4], flag);
        
        //vettore per Zjet con MC
	std::vector<TString> h_Zjet_vect;
	std::vector<TString> h_Zjet_vect_sh;
	//TString sufFC[5]={"_NB","_1B","_1C","_NC","_L"};
	for (int i=0; i<5; i++){
		h_Zjet_vect.push_back("h_Zjet"+sufFC[i]); //sufFC array dei titoli
		h_Zjet_vect_sh.push_back("h_sh"+sufFC[i]);
	
	  if (i==1)
	  { draw_stuck_histo(item, f_Zjet, 1, lumi, OutPutFile, histo_name_ori_Zjet_vect[i], rebin, 0, h_Zjet_vect[i], flagb);
	    draw_stuck_histo(item, f_Zjet_sh, 1, lumi, OutPutFile, histo_name_ori_Zjet_vect_sh[i], rebin, 0, h_Zjet_vect_sh[i], flagb_sh);
}
	  else if(i==2 || i==3 || i==4)
	   { draw_stuck_histo(item, f_Zjet, 1, lumi, OutPutFile, histo_name_ori_Zjet_vect[i], rebin, 0, h_Zjet_vect[i], flagc);
	    draw_stuck_histo(item, f_Zjet_sh, 1, lumi, OutPutFile, histo_name_ori_Zjet_vect_sh[i], rebin, 0, h_Zjet_vect_sh[i], flagc_sh); 
}
	   else
	   {  draw_stuck_histo(item, f_Zjet, 1, lumi, OutPutFile, histo_name_ori_Zjet_vect[i], rebin, 0, h_Zjet_vect[i], flag); 
	      draw_stuck_histo(item, f_Zjet_sh, 1, lumi, OutPutFile, histo_name_ori_Zjet_vect_sh[i], rebin, 0, h_Zjet_vect_sh[i], flag); 
}
	} 

	std::vector<TString> h_data;
        h_data.push_back("h_data");
        draw_stuck_histo(item, f_data,  1, lumi,OutPutFile, histo_name_ori_data, rebin, 0, h_data[0], flag);

        std::vector<TString> h_truth={"h_truth", "h_truth_sh"};
        draw_stuck_histo(item, f_Zjet,  1, lumi, OutPutFile, histo_name_ori_truth, rebin, 0, h_truth[0], flag);
        draw_stuck_histo(item, f_Zjet_sh,  1, lumi, OutPutFile, histo_name_ori_truth_sh, rebin, 0, h_truth[1], flag);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    draw_stuck_all_ratio(PATH_RESULTS,channel,region, item, OutPutFile, h_Zjet_vect, h_Zjet_vect_sh, h_bkg_vect,h_data,rebin,x_min,x_max,y_min,y_max,x_name,y_name,do_log);
    
   draw_stuck_all_data(PATH_RESULTS,channel,region, item,OutPutFile,h_Zjet_vect,h_Zjet_vect_sh, h_bkg_vect,h_data,rebin,x_min,x_max,y_min,y_max,x_name,y_name,do_log);
   
   std::vector<TString> h_name_v={"h_data", "h_sum_bkg"};
   TString h_data_bsZ_name= "h_data_bs";
   draw_histo_bkg_sub(OutPutFile, item, channel, h_name_v, h_data_bsZ_name, rebin, x_min, x_max, y_min, y_max, x_name, y_name, do_log);
   
   TString unf_meth="bin_by_bin";
   TString h_unf_bin_by_bin_name="h_unf_bin_by_bin";
   
   TString h_sigZ_name="h_sig_reco";
   TString h_sig_truthZ= h_truth[0];
   draw_histo_div_1D_1D(OutPutFile, item, channel, unf_meth, "nominal", h_sig_truthZ, h_sigZ_name, h_unf_bin_by_bin_name, 1, 1, 1, x_min, x_max, y_min, y_max, x_name, "U_MC");
   
   TString h_recoZ_name="histo_mult_UMC";
   draw_histo_fake_mult(OutPutFile, item, h_data_bsZ_name, h_unf_bin_by_bin_name, h_recoZ_name);
   
   TString h_xsZ_name="sez_urto";
   
	if ( item=="diBJets_DPhi"){        
        std::vector<float> publ1={0.322205, 0.392592, 0.425563, 0.347971, 0.354535, 0.362277, 0.388852, 0.423839, 0.483598, 0.520567, 0.616039, 0.670647};
        
        std::vector<float> publ_err1={0.0060927, 0.00625339, 0.00695696, 0.00617388, 0.00611004, 0.00671428, 0.00679904, 0.00716367, 0.00750524, 0.00785887, 0.00872058, 0.00891548};
        
        convert_reco_xs(OutPutFile, lumi, item, channel, h_recoZ_name, h_xsZ_name, h_truth[0], publ1, publ_err1, h_truth[1], x_min, x_max, y_min, y_max, x_name, "Sezione d'urto differenziale [pb/GeV]", 1, do_log);
        }

        if ( item=="diBJets_M"){
        
        std::vector<float> publ2={0.00000000000001, 0.00315913, 0.00870826, 0.00972312, 0.0085278, 0.00633112, 0.00456476, 0.00318979, 0.00227742, 0.00155733, 0.000935421, 0.000493091, 0.000154974, 3.18859e-05, 4.12769e-06};
        
        std::vector<float> publ_err2={0.000000000000000001, 7.507e-05, 0.00010261, 0.000101687, 8.71532e-05, 6.33807e-05, 4.74062e-05, 3.65751e-05, 2.81292e-05, 2.23439e-05, 1.6285e-05, 1.00551e-05, 3.76397e-06, 1.36591e-06, 5.59203e-07};

        convert_reco_xs(OutPutFile, lumi, item, channel, h_recoZ_name, h_xsZ_name, h_truth[0], publ2, publ_err2, h_truth[1], x_min, x_max, y_min, y_max, x_name, "Sezione d'urto differenziale [pb/GeV]", 1, do_log);
        }
        
        if ( item=="diBJets_DR"){
        
        std::vector<float> publ3={0.107155, 0.350312, 0.648751, 1.08776, 1.91567, 3.76678, 8.09884, 5.80535, 1.91521, 0.494374};
        
        std::vector<float> publ_err3={0.00331118, 0.00536549, 0.00826925, 0.00901723, 0.0115755, 0.0171385, 0.0244246, 0.0227189, 0.0110797, 0.00420144};
        
        convert_reco_xs(OutPutFile, lumi, item, channel, h_recoZ_name, h_xsZ_name, h_truth[0], publ3, publ_err3, h_truth[1], x_min, x_max, y_min, y_max, x_name, "Sezione d'urto differenziale [pb/GeV]", 1, do_log);
        }
        
        if ( item=="diBJets_DY"){
        
        std::vector<float> publ4={0.107155, 0.350312, 0.648751, 1.08776, 1.91567, 3.76678, 8.09884, 5.80535, 1.91521, 0.494374};
        
        std::vector<float> publ_err4={0.00331118, 0.00536549, 0.00826925, 0.00901723, 0.0115755, 0.0171385, 0.0244246, 0.0227189, 0.0110797, 0.00420144};
        
        convert_reco_xs(OutPutFile, lumi, item, channel, h_recoZ_name, h_xsZ_name, h_truth[0], publ4, publ_err4, h_truth[1], x_min, x_max, y_min, y_max, x_name, "Sezione d'urto differenziale [pb/GeV]", 1, do_log);
        }
   
    OutPutFile->Close();
}   	
	
	

