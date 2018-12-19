#include <iostream>
#include <sstream>
#include <stdio.h>
#include "TROOT.h"
#include "TH1.h"

macroBasicTWC1(const char *filename){
    gStyle->SetOptFit();
    Int_t HV;
    
    TFile *file = new TFile(filename);
    TTree *tree = (TTree*)file->Get("t");
    Int_t chnum[6] = {0,1,2,3,4,5,6};

    Int_t n = 4
    
    tree->Project("hvhisto(2000,1600,3600)","hv");
    TH1F* hvh = (TH1F*)gROOT ->FindObject("hvhisto");
    Int_t l; 
    for(Int_t n = 0;  n < hvh->GetXaxis()->GetNbins()  ;  n++){  
        if(hvh->GetBinContent(n+1) > 0)  l = n+1;
    }
    HV = (Int_t)hvh->GetXaxis()->GetBinCenter(l);
    
    TCanvas *c1 = new TCanvas("c1","c1");    
    TCanvas *c2 = new TCanvas("c2","c2");
    TCanvas *c3 = new TCanvas("c3","c3");
    TCanvas *c4 = new TCanvas("c4","c4");
  
    char iposHV_cut[128];    
    char tdc_cut[128];
    char adc_cut[128];
    sprintf(tdc_cut,"1200<tdc[%d]&&tdc[%d]<1500",chnum[n],chnum[n]);
    sprintf(iposHV_cut,"hv==%d&&ipos==%d",HV,chnum[n]);
    sprintf(adc_cut,"100<adc[%d]&&adc[%d]<200",chnum[n],chnum[n]);

    //#TDC data cut#;
    std::ostringstream cut;
    cut << tdc_cut << "&&" << iposHV_cut /*<< "&&" << adc_cut*/;



    //#Pedestal data cut#;
    char tdc_cut_pedestal[128];
    sprintf(tdc_cut_pedestal,1500<tdc[%d]&&tdc[%d]<4100);//12bit_tdc
    std::ostringstream cutpedestal;
    cutpedestal << tdc_cut_pedestal << "&&" << iposHV_cut;//total_pedestal_data_cut



    //#TDC Histogram#;
    c1 ->cd();
 
    char tdc_ch[128];
    sprintf(tdc_ch,"tdc[%d]>>hist_tdc%d(100,1300,1500)",chnum[n],chnum[n]);
    tree->Draw(tdc_ch,cut.str().c_str());

    char tdc_num[128];
    sprintf(tdc_num,"hist_tdc%d",chnum[n]);
    TH1F* hist1tdc = (TH1F*)gROOT ->FindObject(tdc_num);
    hist1tdc -> SetXTitle("tdc[1/25ps]");
    hist1tdc -> SetYTitle("#tdc_input");



    //#ADC Histogram#;
    c2 ->cd();

    char adc_ch[128];
    sprintf(adc_ch,"adc[%d]>>hist_adc%d(100,100,200)",chnum[n],chnum[n]);
    tree->Draw(adc_ch, cut.str().c_str());

    char adc_num[128];
    sprintf(adc_num,"hist_adc%d",chnum[n]);
    TH1F* hist1adc = (TH1F*)gROOT ->FindObject(adc_num);
    hist1adc -> SetXTitle("adc[1/0.25pC]");
    hist1adc -> SetYTitle("#adc_input");



    //#ADC Pedestal Evaluation#;
    sprintf(adc_ch,"adc[%d]>>hist_pedestal%d(100,100,200)",chnum[n],chnum[n]);
    tree->Draw(adc_ch, cutpedestal.str().c_str());

    char pedestal_num[128];
    sprintf(pedestal_num,"hist_pedestal%d",chnum[n]);
    TH1F* hist1pedestal = (TH1F*)gROOT ->FindObject(pedestal_num);

    Double_t pedestal = hist1pedestal ->GetMean(); 



    //#TDC vs. ADC 2D-Histogram#
    c3 ->cd();

    char tdc_adc[128];
    sprintf(tdc_adc,"tdc[%d]:adc[%d]>>2Dhist_tdc_adc%d(120,80,200,200,1300,1500)",chnum[n],chnum[n],chnum[n]);
    tree->Draw(tdc_adc,cut.str().c_str(),"colz");

    char tdc_adcchnum[128];
    sprintf(tdc_adcchnum,"2Dhist_tdc_adc%d",chnum[n]);
    TH2F *hist1tdc_adc = (TH2F*)gROOT ->FindObject(tdc_adcnum);
    h1tdc_adc -> SetXTitle("adc[1/0.25pC]");
    h1tdc_adc-> SetYTitle("tdc[1/25ps]");
    h1tdc_adc-> SetTitle(tdc_adcnum);


    
    //#TDC timewalk Correction#;
    //#1#TDC Projection for each ADC value (zone)#;
    
    //#1-1#Makeing histogram for each ADC value (zone)#;
    const Int_t Nd = 20;//the number of division; interval = +/-2 & duplicated inteval 1;
    const Int_t delta_adc = 0;//not use;
    Double_t twc_end = pedestal + Nd;


    //#1-1#variables;
    Double_t adcstart[Nd];
    Double_t adcend[Nd];
    Double_t adccnt[Nd];//pedestal is double type;
    Double_t adcerr[Nd];//Quantization error;
    //#1-2#variables;
    Double_t gausmean = 0;   
    Double_t gaussigma = 0;
    Double_t t[Nd];
    Double_t t_err[Nd];

    c4 ->cd();
    

    for(Int_t i = 0; i < Nd; i++){
        adccnt[i] = pedestal + i; //+delta_adc;
        adcstart[i] = adccnt[i] - 0.5;
        adcend[i] = adccnt[i] +0.5;
        adcerr[i] = 1/sqrt(12);

        sprintf(adccut, "<adc[%d]&&adc[%d]<", chnum[n], chnum[n]);

        std::ostringstream adc_cut2;
        adc_cut2 << adcstart[i] << adccut << adcend[i];
        std::ostringstream tdc_cut2;
        cut2 << tdc_cut << "&&" << iposHV_cut << "&&" << adc_cut2.str().c_str();//total projection cut;

        tree -> Draw(tdc_ch, cut2.str().c_str());
    }

    //#1-2#TDC Gausian Fitting for each ADC value (zone)#
    TH1F* hist1tdc = (TH1F*)gROOT -> FindObject(tdc_num);
    Double_t mean = hist1tdc ->GetMean();
    Double_t rms = hist1tdc ->GetRMS();
    Double_t N = hist1tdc ->GetEntries();

    hist1tdc ->Fit("gaus","","",mean-rms, mean+0.5*rms);

        //should develop -> void?;
        if(N == 0 && i == 0){
            t[i] = 0;
            t_err[i] = 0;
        }
        elseif(N == 0 && i > 0){
            t[i] = t[i-1];
            t_err[i] = t_err[i-1];
        }
        else{
            t[i] = gausmean;
            t_err[i] = gaussigma/sqrt(N);
        }

    c4 ->Update();
    }


    //#2#Plot for Timewalk Correction;
    //#2-1#Fitting;
    c3 ->cd();
    
    TGraphErrors* projection = new TGraphErrors(Nd,adccnt,t,adcerr,t_err);//t:tdccnt mean;
    projection ->Draw("same");
    projection ->GetXaxis()->SetTitle("adc[/0.25pC]");
    projection ->GetYaxis()->SetTitle("tdc[1/25ps]");

    TF1* func1twc = new TF1("func1twc","([0]/sqrt(x-[1]) + [2])",pedestal,twc_end);
    func1twc ->SetParameters(1,120,1200);
    func1twc ->SetLineColor(kRed);
    func1twc ->SetLineWidth(2);
    projection ->Fit("func1twc","","same",pedestal,twc_end);

    Double_t p0 = func1twc ->GetParameter(0);
    Double_t p1 = func1twc ->GetParameter(1);
    Double_t p2 = func1twc ->GetParameter(2);

    printf("TDC = %lf/sqrt(ADC-%lf) + %lf",p0,p1,p2);
    

    //#2-2#TDC Timewalk Correction;
    char tdc_twc[512];
    sprintf(tdc_twc"tdc[%d] - %lf/sqrt(adc[%d]-%lf)",chnum[n],p0,chnum[n],p1);



    //TTS(transit Time Spread) & MCP-PMT's Temporal Resolution (=TTS standard deviation);
    std::ostringstream adc_twc;
    adc_twc << pedestal << adccut << twc_end;
    std::ostringstream twccut;
    twccut << cut.str().c_str() << "&&" << adc_twc.str().c_str();
    tree ->Draw(tdc_twc, twccut.str().c_str());

    char tdctwcnum[512];
    sprintf(tdctwcnum,"tdc_twc%d",chnum[n]);
    TH1F* hist2tdc = (TH1F*)gROOT ->FindObject(tdctwcnum);
    hist2tdc -> SetXTitle("tdc[1/25ps]");
    hist2tdc -> SetYTitle("#TDC input (after TWC)");

    TF1* func2TTS = new TF1("func2TTS","[0]*([1]TMath::Gaus(x,[2],[3],1)+(1-[1])*TMath::Gaus(x,[4],[5],1))",-50,50);
    func2TTS ->SetLineColor(kMagenta);
    func2TTS ->SetLineWidth(1);

    hist2tdc -> Fit("func2TTS");

    Double_t sigma0 = func2TTS ->GetParameter(3);
    printf("Temporal Resolution = %lf",sigma0);


}

