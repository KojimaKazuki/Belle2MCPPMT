#define SAVE 1
#define DATABASE 0

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "AnaGainCh2.cpp"
#include "DrawSquareCanvas.cpp"


void macroBasicAnaGain181017(){

#define Nch 16
#define NHV_MAX 7

  // #Input File: PMT-ID, Date# ;
  char fileNameIn[128];

  while (true){
    std::cout << "Please tell me the name of input file." << std::endl;
    std::cin >> fileNameIn;
 
      if (std::cin.fail()){
        std::cout << "Input Error! Please check it!\n" << std::endl;
        std::cin.clear();
        std::cin.ignore(1024, '\n');
        continue;
      }
      break;
  }
  std::cout << fileNameIn << std::endl;


  //char fileName0[128] = "/home/bqt/kojima/MCP_PMT/Data/1809/";
  //char fileName0[128] = "/home/bqt/kojima/MCP_PMT/Data/1810/";
  char fileName0[128] = "/home/bqt/kojima/MCP_PMT/Data/1811/";
  //char fileName0[128] = "/chai/sgt1/bfactory/mcppmt_data/kihon/03/";
  char fileName[128];
  sprintf( fileName, "%s%s" ,fileName0 ,fileNameIn);
  std::cout << "Input File: " << fileName << std::endl;


  char title[128];
  sprintf(title,"%s",fileNameIn);
  strtok(title,"/");
  std::cout << "Input File Name: " << title << std::endl;


  char file_gain[128];
  sprintf(file_gain,"%s",title);
  char id[64];
  sprintf(id,"%s",title);
  strtok(id,"_");
  char date[64];
  sprintf(date,"%s",title);
  strtok(date,"_");
  sprintf(date,"%s",strtok(NULL,".root"));
  if (strlen(date)>8) sprintf(date,"%s",strtok(date,"_"));


  // #Amplifier Version# ;
  int amp_ver = 0;
  if (strncmp(date,"20140401",8)>=0) amp_ver = 1;

  // #Old Data Judgement# ;
  bool old_data = false;
  if (strncmp(date,"20120531",8)<=0) old_data = true;


  // #Output File Name -> PDF Format# ;
  string fileOutPDF(file_gain);
  fileOutPDF.replace(fileOutPDF.find(".root"),5,"Gain.pdf");

  // #Output File Name -> CSV Format# ;
  string fileOutCSV(file_gain);
  fileOutCSV.replace(fileOutCSV.find(".root"),5,"Gain.csv");

  std::cout << "PMT ID: " << id << std::endl;
  std::cout << "Date: " << date << std::endl;
  std::cout << "Output File Name (PDF): " << fileOutPDF << std::endl;
  std::cout << "Output File Name (CSV): " << fileOutCSV << std::endl;
  


  // #Output File Open# ;
  std::ofstream ofs(fileOutCSV.c_str());

  if(!ofs){
  std::cout << "It has failed to open the file:" << fileOutCSV << std::endl;
  std::cin.get();
  return;
  }  

   
  // #Get HV Value# #Get ADC->Gain Value# ;
  double hv[NHV_MAX], err_hv[2][NHV_MAX];
  double Gain[Nch][NHV_MAX], err_Gain[2][Nch][NHV_MAX];
  double val, err_L, err_H, rate0;
  int ch = 0, iHV, HV_norm = 0;
  char thumbnail[128];
  TList *hv_set;

  loop_ch:
    iHV = 0;

    // #ADC->Gain Value for Each Channel at a HV Value# ;
    loop_HV:
      // #Get ADC->Gain Value# ;
      cout << AnaGainCh2(fileName, ch, iHV, false, &val, &err_L, &err_H);//,&rate0);
      cout << " e6 gain" << endl;
      Gain[ch][iHV] = val;
      err_Gain[0][ch][iHV] = err_L;
      err_Gain[1][ch][iHV] = err_H;


      // #Go to HV Value Directory# ;
      hv_set = (TList*)gDirectory->Get("hv_set");

      if (!hv_set) {
        TClonesArray par_hv;
        par_hv.SetClass("TParameter<int>");
        par_hv.SetName("hv_set");

        mcp ->Draw("hv >> histHV(400,0,4000)","","goff");
        TH1D *histHV = (TH1D*)gROOT->FindObject("histHV");

        int n = 0;
        for (int i=histHV->GetNbinsX()-1; i >= 0; i--) {
          if (histHV->GetBinContent(i+1)) {
            par_hv[n] = new TParameter<int>("HV",histHV->GetBinLowEdge(i+1));
            n++;
          }
        }

        hv_set = (TList*)par_hv;
      }


      const int nHV = hv_set ->GetEntries();

      if (Gain[ch][iHV]==0) goto end_HV;

      // #Get HV Value# ;
      hv[iHV] = (double)((TParameter<int>*)hv_set ->At(iHV)) ->GetVal();
      err_hv[0][iHV] = 0;
      err_hv[1][iHV] = 0;

      // #Get Normal HV Value# ;
      if (!strcmp((char*)((TParameter<int>*)hv_set ->At(iHV)) ->GetName(),"Nominal HV +0 V"))
        HV_norm = hv[iHV];
      // A = B  ==>  strcmp(A,B) = 0: Comparison of character strings.;

      iHV++;
      if (iHV < nHV) goto loop_HV;
    end_HV:

    if (!HV_norm) HV_norm = hv[1];


#if SAVE
    sprintf(thumbnail,"Title:ADC Ch%d",ch);
 
    if (ch == 0) c1 ->Print((fileOutPDF+string("(")).c_str(),thumbnail);
    else         c1 ->Print(fileOutPDF.c_str(),thumbnail);
#endif

    std::cout << "Ch" << ch << " done" << std::endl;
    ch ++;
    if (ch < Nch) goto loop_ch;
  end_ch:




  // #Gain Analyses for Each Channel# ;
  TCanvas *c2 = new TCanvas("c2","c2");
  c2 ->Divide(2,2);//Use 1 canvas for 4 channel at the same column; 


  TGraphAsymmErrors *graph_Gain_HV[Nch];
  TF1 *func_Gain[Nch];
  TPaveStats *st_Gain[Nch];


  gStyle->SetOptFit(0011);


  // #Graph: Gain(Y) vs. HV(X)# ;
  // 1 Graph for 4 Channel: (1,5,9,13),(2,6,10,14),(3,7,11,15),(4,8,12,16);
  char name[32];
  double HV_max;

  for (ch = 0; ch < Nch; ch++) {
    c2->cd(ch%4+1);

    sprintf(name,"graph_ch%d",ch); //graph_ch0;
    sprintf(title,"%s ch%d",id,ch); //JT0100 ch0;

    graph_Gain_HV[ch] = new TGraphAsymmErrors( iHV , hv, Gain[ch], err_hv[0], err_hv[1], err_Gain[0][ch], err_Gain[1][ch]);
    graph_Gain_HV[ch] -> SetMarkerStyle(20);
    graph_Gain_HV[ch] -> SetMinimum(0.0);
    graph_Gain_HV[ch] -> SetMaximum(4.0);
    graph_Gain_HV[ch] -> Draw("AP");
    graph_Gain_HV[ch] -> SetTitle(title);
    graph_Gain_HV[ch] -> SetName(name);
    graph_Gain_HV[ch] -> GetXaxis()->SetTitle("HV (V)");
    graph_Gain_HV[ch] -> GetYaxis()->SetTitle("Gain (#times10^{6})");


    // #Difinition of HV_max#
    HV_max = 0;

    // For Old Data;
    if (old_data) HV_max = hv[2];

    // For New Data; Classificated by Amplifier Version;
    for (int i=0; i < iHV; i++) {
      if (amp_ver==0) {
        if (Gain[ch][i] < 3.0 && HV_max < hv[i]) HV_max = hv[i];
      } else {
        if (Gain[ch][i] < 4.0 && HV_max < hv[i]) HV_max = hv[i];
      }

      if (old_data) {
        if (Gain[ch][i] < 2.0 && HV_max < hv[i]) HV_max = hv[i];
      }
    }


    // #Fitting for Graph: Gain(Y) vs. HV(X)# exp(aX+b) ;
    graph_Gain_HV[ch] ->Fit("expo","","", hv[iHV-1] , HV_max); // HV_min = hv[iHV-1], HV_max = hv[0];
    sprintf(name,"func_ch%d",ch);

    func_Gain[ch] = new TF1(name,"exp([0]+[1]*x)",0,4000);
    func_Gain[ch] ->SetParameters((TF1*)(gROOT->FindObject("expo"))->GetParameter(0),(TF1*)(gROOT->FindObject("expo"))->GetParameter(1));
    func_Gain[ch] ->SetLineStyle(9);
    func_Gain[ch] ->Draw("same");
    c2->Update();


    // #Statistics Box# ;
    st_Gain[ch] = (TPaveStats*)graph_Gain_HV[ch]->FindObject("stats");
    st_Gain[ch] ->SetX1NDC(0.2);
    st_Gain[ch] ->SetX2NDC(0.5);
    st_Gain[ch] ->SetY1NDC(0.76);
    st_Gain[ch] ->SetY2NDC(0.88);
    st_Gain[ch] ->Draw();
    c2->Update();

#if SAVE
    // #Print Graphs to Output PDF File# Gain(Y) vs. HV(X) ;
    if (ch%4==3) {
      sprintf(thumbnail,"Title :Gain(HV) Ch%d-%d",ch-3,ch);
      c2->Print(fileOutPDF.c_str(),thumbnail);
    }
#endif
  }



  // #Gain Analyses: Average of Gain of All Channels# ;
  TCanvas *c3 = new TCanvas("c3","c3");
  c3->SetLogy(1);

  char buf[256] = "";
  for (int ch = 0; ch < Nch; ch++) {
    sprintf(name,"ch%d",ch);
    

    if (ch == 0) sprintf(buf,"func_%s",name);
    else         sprintf(buf,"%s+func_%s",buf,name);//func_ch0+func_ch1+...+func_ch(k);
  }
  sprintf(buf,"(%s)/%d",buf,Nch);//(func_ch0+func_ch1+...+func_ch15)/16;
  std::cout << buf << std::endl;


  // #Average of 16 function: func_Gain[ch]#;
  // ch(k): func_Gain[ch = k]
  TF1 *func_GainAve = new TF1("GainAve",buf,0,4000);

  //Set the number of points used to draw the function;
  //In order to Change the Function (func_GainAve) into 1000 Bins Histogram;
  func_GainAve ->SetNpx(1000);
  func_GainAve ->Draw();

  func_GainAve ->GetXaxis()->SetRangeUser(HV_norm-400,HV_norm+100);
  func_GainAve ->GetYaxis()->SetRangeUser(0.1,4);
  func_GainAve ->GetXaxis()->SetTitle("HV / V");
  func_GainAve ->GetYaxis()->SetTitle("Gain / #times10^{6}");


  gStyle ->SetOptStat(0);
  gStyle ->SetOptFit(0011);


  // Change the Function into 1000 Bins Histogram in order to Fit it.;
  TH1D *hist_GainAve = (TH1D*)func_GainAve ->GetHistogram()->Clone();
  hist_GainAve ->SetName("Average");
  hist_GainAve ->SetStats(1);


  // #Parameter of Expo Fitting for the Average Gain of All Channels# ;
  // Fitting to the Histogram changed from the Average function;
  hist_GainAve ->Fit("expo");
  double par_ExpoGainAve[2];
  for (int i=0; i<2; i++){
    par_ExpoGainAve[i] = (TF1*)(gROOT->FindObject("expo")) ->GetParameter(i); 
  }


  c3 ->Update();
  TPaveStats *st = (TPaveStats*)hist_GainAve ->FindObject("stats");
  st ->SetX1NDC(0.53);
  st ->SetX2NDC(0.83);
  st ->SetY1NDC(0.275);
  st ->SetY2NDC(0.415);
//  func_GainAve ->Draw();
  st ->Draw();
  TText *text_id = new TText(0.18,0.82,id);
  text_id->SetNDC(1);
  text_id->Draw();
#if SAVE
  c3 ->Print(fileOutPDF.c_str(),"Title:Gain(HV) Average LogAxis");
#endif

  gStyle ->SetOptStat(1);
  gStyle ->SetOptFit(1);
  c3 ->SetLogy(0);
  c3 ->Update();
#if SAVE
  c3 ->Print(fileOutPDF.c_str(),"Title:Gain(HV) Average NormalAxis");
#endif


  // #Parameter of Expo Fitting for the Gain of Each Channel# ;
  double channel[Nch], par_ExpoGain[Nch][2];
  for (int ch = 0; ch < Nch; ch++){
    channel[ch] = ch;
    par_ExpoGain[ch][0] = func_Gain[ch] ->GetParameter(0);//b:exp(aX+b);
    par_ExpoGain[ch][1] = func_Gain[ch] ->GetParameter(1);//a:exp(aX+b);
  }


  // #Output(CSV Format) the Fitting Parameters of the Average Gain#;
  ofs << "#MCP-PMT ID" << "," << "Ch No." << "," << "Date" << "," << "Parameter1(b)" << "," << "Parameter2(a)" << std::endl;
  ofs << id << "," << "0" << "," << date << "," << par_ExpoGainAve[0] << "," << par_ExpoGainAve[1] << std::endl; 
  for (int ch = 0; ch < Nch; ch++){
  ofs << id << "," << channel[ch] + 1 << "," << date << "," << par_ExpoGain[ch][0] << "," << par_ExpoGain[ch][1] << std::endl; 
  }


  // #Calculate the Gain for Each of 16 Channels# ;
  double Gain0[2][Nch];
  const double gainAve[2] = {0.5, 1.}; // (x10^6)

  for (int i=0; i < 2; i++) {
    // #Get the HV Value HV0 at a Certain Average Gain# ;
    int HV0 = (int)func_GainAve->GetX(gainAve[i]); //GetX(Y): Get X Value satisfying Y=f(X);

    // #Get the Gain Value for Each Channel at HV0# ;
    for (int ch = 0; ch < Nch; ch++) {
      Gain0[i][ch] = exp(par_ExpoGain[ch][0]+par_ExpoGain[ch][1]*HV0);
    }

    TGraph *graph_Gain0 = new TGraph( Nch, channel, Gain0[i]);// Channel No.(X) vs. Gain0(Y);
    graph_Gain0 ->SetMarkerStyle(20);
    graph_Gain0 ->SetMinimum(0);
    graph_Gain0 ->SetMaximum(gainAve[i]*3);
    graph_Gain0 ->Draw("AP");
    graph_Gain0 ->GetXaxis()->SetTitle("channel");
    graph_Gain0 ->GetYaxis()->SetTitle("Gain (#times10^{6})");
    sprintf(title,"%s, %.1f#times10^{6}, %d V",id,gainAve[i],HV0);
    graph_Gain0 ->SetTitle(title);


    /*
    // #Maximum & Minimum Gain in the All Channels# at HV0 (at a Certain Average Gain) ;
    // #Get the Gain Ratio (Maximum / Minimum)# ; 
    double Gain_max, Gain_min, Gain_ratio[2];
    Gain_max = 0;
    Gain_min = 1e3;

    for (int ch = 0; ch < Nch; ch++){
      if (Gain0[i][ch] > Gain_max) Gain_max = Gain0[i][ch];
      if (Gain0[i][ch] < Gain_min) Gain_min = Gain0[i][ch];
    }
    Gain_ratio[i] = Gain_max/Gain_min;
    */


#if SAVE
    sprintf(thumbnail,"Title:Gain at %.1fe6", gainAve[i]);
    c3 ->Print(fileOutPDF.c_str(),thumbnail);
#endif
  }


  // #Make 16 Channels 2D Gain Distribution Plots at 2 Average Gains# ;
  DrawSquareCanvas("c4");

  for (int i = 0; i < 2; i++) {

    // #Plot the 2D Histogram# Weight(# of Entry) = Gain;
    TH2D *hist_Gain = new TH2D("gain2d",fileName,4,0,4,4,0,4); //16 Channels (4 by 4);

    for (int ch = 0; ch < Nch; ch++) {
      hist_Gain ->SetBinContent( 4-ch%4, 4-int(ch/4), Gain0[i][ch]);//Set the Number of Entry (the Gain Value) in a Bin (x,y).;
    }


    for (int b = 0; b < (int)sqrt(Nch); b++) {
      sprintf(buf,"%d",(int)sqrt(Nch)-b-1);
      hist_Gain ->GetXaxis() ->SetBinLabel(b+1,buf);
      hist_Gain ->GetYaxis() ->SetBinLabel(b+1,buf);
    }

    hist_Gain ->GetXaxis()->SetLabelSize(0.09);
    hist_Gain ->GetYaxis()->SetLabelSize(0.09);
    hist_Gain ->SetMinimum(0);
    hist_Gain ->SetMaximum(gainAve[i]*2);
    hist_Gain ->SetContour(99);
    hist_Gain ->SetStats(0);
    int HV0 = (int)func_GainAve ->GetX(gainAve[i]);
    sprintf(title,"%s, %.1f#times10^{6}, %d V",id,gainAve[i],HV0);
    hist_Gain ->SetTitle(title);
    hist_Gain ->Draw("colz");


#if SAVE
    sprintf(thumbnail,"Title:Gain at %.1fe6 (2D)",gainAve[i]);
    if (i==1) c4->Print((fileOutPDF+string(")")).c_str(),thumbnail);
    else      c4->Print(fileOutPDF.c_str(),thumbnail);
#endif
  }

  string input;
  std::cout << "OK? (yes/no): " << std::flush;
  std::cin >> input;
  if (input!="yes") return;

  std::cout << "Input File: " << fileName << std::endl;
  std::cout << "PMT ID: " << id << std::endl;
  std::cout << "Date: " << date << std::endl;
  std::cout << "Output File Name (PDF): " << fileOutPDF << std::endl;
  std::cout << "Output File Name (CSV): " << fileOutCSV << std::endl;

  return;
}
