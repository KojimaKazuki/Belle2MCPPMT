#include <iostream>
#include <sstream>
#include <stdio.h>
#include "TROOT.h"
#include "TH1.h"
#include "TMath.h"

//macroTWC1(const char *test*.root){
void macro_twc01(){
    gStyle ->SetOptFit();
    gStyle ->SetStatX(x = 0.88); //y at the top-right point of box;
    gStyle ->SetStatY(y = 0.88); //y at the top-right point of box;
    gStyle ->SetLabelSize(0.04,"x");
    gStyle ->SetLabelSize(0.04,"y");
    gStyle ->SetTitleXSize(0.04);
    gStyle ->SetTitleYSize(0.04);


    std::cout <<  "../data/test1807/test*.root" << std::endl;
    TChain *tree = new TChain("t","test");
    tree ->Add("../data/test1807/test*.root");
    //TFile *file = new TFile(test.root);
    //TTree *tree = (TTree*)file->Get("t");
    Int_t chnum[6] = {0,1,2,3,4,5};

    Int_t n = 4;


    TCanvas *c1 = new TCanvas("c1","c1");    
    c1 ->Divide(2,3);
    TCanvas *c2 = new TCanvas("c2","c2");
    TCanvas *c3 = new TCanvas("c3","c3");
    TCanvas *c4 = new TCanvas("c4","c4");
    TCanvas *c5 = new TCanvas("c5","c5");
    TCanvas *c6 = new TCanvas("c6","c6");
    TCanvas *c7 = new TCanvas("c7","c7");



    char tdc_cut[128];
    char adc_cut[128];
    sprintf(tdc_cut,"1200<tdc[%d]&&tdc[%d]<1500",chnum[n],chnum[n]);
    sprintf(adc_cut,"100<adc1[%d]&&adc1[%d]<200",chnum[n],chnum[n]);

    //#TDC data cut#;
    std::ostringstream cut;
    cut << tdc_cut /*<< "&&" << adc_cut*/;



    //#Pedestal data cut#;
    char tdc_cut_pedestal[128];
    sprintf(tdc_cut_pedestal,"4000<tdc[%d]",chnum[n]);//12bit_tdc
    std::ostringstream cutpedestal;
    cutpedestal << tdc_cut_pedestal;//total_pedestal_data_cut



    //#TDC Histogram#;
    c1 ->cd(1);

    char tdc_ch[128];
    sprintf(tdc_ch,"tdc[%d]>>Histogram_tdc%d(100,1200,1500)",chnum[n],chnum[n]);
    tree->Draw(tdc_ch,cut.str().c_str());

    char tdc_num[128];
    sprintf(tdc_num,"Histogram_tdc%d",chnum[n]);
    TH1F* hist1tdc = (TH1F*)gROOT ->FindObject(tdc_num);
    hist1tdc ->SetXTitle("tdc[1/25ps]");
    hist1tdc ->SetYTitle("#tdc_input");
    hist1tdc ->SetTitle("TDC Histogram");

    c2 ->cd();
    hist1tdc ->Draw();
    hist1tdc ->SetTitleOffset(1.2,"y");


    //#ADC Histogram#;
    c1 ->cd(2);

    char adc_ch[128];
    sprintf(adc_ch,"adc1[%d]>>Histogram_adc%d(100,100,200)",chnum[n],chnum[n]);
    tree->Draw(adc_ch, cut.str().c_str());

    char adc_num[128];
    sprintf(adc_num,"Histogram_adc%d",chnum[n]);
    TH1F* hist1adc = (TH1F*)gROOT ->FindObject(adc_num);
    hist1adc ->SetXTitle("adc[1/0.25pC]");
    hist1adc ->SetYTitle("#adc_input");
    hist1adc ->SetTitle("ADC Histogram");

    c3 ->cd();
    hist1adc ->Draw();
    hist1adc ->SetTitleOffset(1.2,"y");



    //#ADC Pedestal Evaluation#;
    c1 ->cd(3);
    sprintf(adc_ch,"adc1[%d]>>Histogram_pedestal%d(100,100,200)",chnum[n],chnum[n]);
    tree->Draw(adc_ch, cutpedestal.str().c_str());
 
    char pedestal_num[128];
    sprintf(pedestal_num,"Histogram_pedestal%d",chnum[n]);
    TH1F* hist1pedestal = (TH1F*)gROOT ->FindObject(pedestal_num);
    hist1pedestal ->SetXTitle("adc[1/0.25pC]");
    hist1pedestal ->SetYTitle("#adc_input");
    hist1pedestal ->SetTitle("ADC Pedestal Data Histogram");

    Double_t pedestal = hist1pedestal ->GetMean(); 

    c4 ->cd();
    hist1pedestal ->Draw();
    hist1pedestal ->SetTitleOffset(1.2,"y");



    //#TDC vs. ADC 2D-Histogram#
    c1 ->cd(4);

    char tdc_adc[128];
    sprintf(tdc_adc,"tdc[%d]:adc1[%d]>>2DHistogram_tdc_adc%d(100,100,200,100,1200,1500)",chnum[n],chnum[n],chnum[n]);
    tree->Draw(tdc_adc,cut.str().c_str(),"colz");

    char tdc_adcnum[128];
    sprintf(tdc_adcnum,"2DHistogram_tdc_adc%d",chnum[n]);
    TH2F *hist1tdc_adc = (TH2F*)gROOT ->FindObject(tdc_adcnum);
    hist1tdc_adc -> SetXTitle("adc[1/0.25pC]");
    hist1tdc_adc-> SetYTitle("tdc[1/25ps]");
    hist1tdc_adc-> SetTitle("2D Histogram (TDC vs. ADC)");



    //#TDC timewalk Correction#;
    //#1#TDC Projection for each ADC value (zone)#;

    const Int_t Nd = 20;//the number of division; interval = +/-1 & duplicated inteval 0(none);
    Double_t Nd2 = (double)Nd;//Double type Nd;
    const Int_t delta_adc = 0;//not use;
    Double_t twc_end = pedestal + Nd;
    

    //#1-1#variables;
    Double_t adcstart[Nd]={};
    Double_t adcend[Nd]={};
    Double_t adccnt[Nd]={};//pedestal is double type;
    Double_t adcerr[Nd]={};//Quantization error;
    //#1-2#variables;
    Double_t gausmean = 0;   
    Double_t gaussigma = 0;
    Double_t t[Nd]={};
    Double_t t_err[Nd]={};


    //#1-1#Making Histogram for Each ADC Value (Zone)#;  
    c7 ->cd();
    for(Int_t i = 0; i < Nd; i++){
        adccnt[i] = pedestal + i; //+delta_adc;
        adcstart[i] = adccnt[i] - 0.5;
        adcend[i] = adccnt[i] +0.5;
        adcerr[i] = 1/sqrt(12);


        //#Projection Cut #Data Cut for Each ADC Value (Zone) #TDC Cut & ADC Cut#;
        char adccut[128];
        sprintf(adccut, "<adc1[%d]&&adc1[%d]<", chnum[n], chnum[n]);
	
        std::ostringstream adc_cut2;
        adc_cut2 << adcstart[i] << adccut << adcend[i];
        std::ostringstream cut2;
        cut2 << tdc_cut << "&&" << adc_cut2.str().c_str();//total projection cut;

        

    //#1-2#TDC Gausian Fitting for each ADC value (zone)#
        char tdcprjct_ch[128];
        sprintf(tdcprjct_ch, "tdc[%d] >> Histogram_TDC_projection%d(100,1200,1500)",chnum[n],chnum[n]);
        tree -> Draw(tdcprjct_ch, cut2.str().c_str());


        char tdcprjct_num[128];
        sprintf(tdcprjct_num,"Histogram_TDC_projection%d",chnum[n]);

        TH1F* hist1prjct = (TH1F*)gROOT -> FindObject(tdcprjct_num);
        hist1prjct -> SetXTitle("tdc[1/25ps]");
        hist1prjct -> SetYTitle("#tdc_input");

        Double_t mean = hist1prjct ->GetMean();
        Double_t rms = hist1prjct ->GetRMS();
        Double_t N = hist1prjct ->GetEntries();

        hist1prjct ->Fit("gaus","","",mean-rms, mean+0.5*rms);
        gausmean = gaus ->GetParameter(1);
        gaussigma = gaus ->GetParameter(2);


    //should develop -> void?;
        if(N == 0 && i == 0){
            t[i] = 0;
            t_err[i] = 0;
        }
        else if(N == 0 && i > 0){
            t[i] = t[i-1];
            t_err[i] = t_err[i-1];
        }
        else{
            t[i] = gausmean;
            t_err[i] = gaussigma/sqrt(N);
        }

        c7 ->Update();
    }



    //#2#Plot for Timewalk Correction;
    //#2-1#Fitting;
    c5 ->cd();

    hist1tdc_adc -> Draw("colz");
    hist1tdc_adc ->SetTitleOffset(1.2,"y");


    TGraphErrors* projection = new TGraphErrors(Nd2,adccnt,t,adcerr,t_err);//t:tdccnt mean;
    projection ->Draw("same p");
    projection ->SetMaximum(1500);
    projection ->SetMinimum(1200);
    projection ->GetXaxis()->SetLimits(100,200);
    projection ->GetXaxis()->SetTitle("adc[/0.25pC]");
    projection ->GetYaxis()->SetTitle("tdc[1/25ps]");

    TF1* func1twc = new TF1("func1twc","([0]/sqrt(x-[1]) + [2])",pedestal,twc_end);
    func1twc ->SetParameters(1,pedestal,1200);
    func1twc ->SetLineColor(kRed);
    func1twc ->SetLineWidth(2);
    projection ->Fit("func1twc","","same",pedestal,twc_end);
    func1twc ->Draw("same");

    Double_t p0 = func1twc ->GetParameter(0);
    Double_t p1 = func1twc ->GetParameter(1);
    Double_t p2 = func1twc ->GetParameter(2);

    printf("TDC = %lf/sqrt(ADC-%lf) + %lf \n",p0,p1,p2);

  

    //#2-2#TDC Timewalk Correction;
    char tdc_twc[512];
    sprintf(tdc_twc,"tdc[%d] - %lf/sqrt(adc1[%d]-%lf) -%lf >> Histogram_tdc_twc%d",chnum[n],p0,chnum[n],p1,p2,chnum[n]);



    //#TTS(transit Time Spread) & MCP-PMT's Temporal Resolution (=TTS standard deviation)#;
    //#1#Making TTS Histogram#;
    c1 ->cd(5);

    std::ostringstream adc_twc;
    adc_twc << pedestal << adccut << twc_end;
    std::ostringstream twccut;
    twccut << cut.str().c_str() << "&&" << adc_twc.str().c_str();
    tree ->Draw(tdc_twc, twccut.str().c_str());

    char tdctwcnum[512];
    sprintf(tdctwcnum,"Histogram_tdc_twc%d",chnum[n]);
    TH1F* hist2tdc = (TH1F*)gROOT ->FindObject(tdctwcnum);
    hist2tdc ->SetXTitle("tdc[1/25ps]");
    hist2tdc ->SetYTitle("#TDC input (after TWC)");
    hist2tdc ->SetTitle("TTS");


    //#2#Double Gausian Fitting#;
    TF1 *func2TTS = new TF1("func2TTS","[0]*([1]*TMath::Gaus(x,[2],[3],1)+(1-[1])*TMath::Gaus(x,[4],[5],1))",-50,50);
    func2TTS ->SetLineColor(kRed);
    func2TTS ->SetLineWidth(2);

    func2TTS ->SetParameter(0,2000);
    func2TTS ->SetParameter(1,1);
    func2TTS ->SetParameter(2,0);
    func2TTS ->SetParameter(3,5);
    func2TTS ->SetParameter(4,10);
    func2TTS ->SetParameter(5,10);

    hist2tdc -> Fit("func2TTS");


    Double_t N_tot = func2TTS ->GetParameter(0);
    Double_t r0 = func2TTS ->GetParameter(1);
    Double_t t0mean = func2TTS ->GetParameter(2);
    Double_t sigma0 = func2TTS ->GetParameter(3);
    Double_t t1mean = func2TTS ->GetParameter(4);
    Double_t sigma1 = func2TTS ->GetParameter(5);


    TF1 *func3TTS0 = new TF1("func3TTS0","[0]*[1]*TMath::Gaus(x,[2],[3],1)",-30,50);
    TF1 *func3TTS1 = new TF1("func3TTS1","[0]*(1-[1])*TMath::Gaus(x,[2],[3],1)",-30,50);
    func3TTS0 ->SetLineColor(kOrange+1);
    func3TTS1 ->SetLineColor(kCyan);
    func3TTS0 ->SetLineWidth(1);
    func3TTS1 ->SetLineWidth(1);
    func3TTS0 ->SetLineStyle(2);
    func3TTS1 ->SetLineStyle(2);

    func3TTS0 ->SetParameter(0,N_tot);
    func3TTS0 ->SetParameter(1,r0);
    func3TTS0 ->SetParameter(2,t0mean);
    func3TTS0 ->SetParameter(3,sigma0);
    func3TTS1 ->SetParameter(0,N_tot);
    func3TTS1 ->SetParameter(1,r0);
    func3TTS1 ->SetParameter(2,t1mean);
    func3TTS1 ->SetParameter(3,sigma1);

    func3TTS0 ->Draw("same");
    func3TTS1 ->Draw("same");


    c6 ->cd();
    hist2tdc ->Draw();
    hist2tdc ->SetTitleOffset(1.2,"y");
    func3TTS0 ->Draw("same");
    func3TTS1 ->Draw("same");



    Double_t TR = sigma0*((double)25);
    printf("Temporal Resolution = %lf [ps] \n",TR);



}

