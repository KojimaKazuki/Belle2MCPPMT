#include <iostream>
#include <sstream>
#include <stdio.h>
#include "TROOT.h"
#include "TH1.h"

macroBasicDAQ_original(const char *fname){
  gStyle->SetOptFit();
  Double_t mcpnum = 642;
  Double_t day = 20161128;
  Int_t HV;
  
  //char qedata[128];
  //sprintf(qedata, "JT0%d_%d.root",mcpnum,day);
  //sprintf(qedata, "KT0%d_%d.root",mcpnum,day);
  //char dataname[128];
  //sprintf(dataname,"JT0%d_%dkihon.ps",mcpnum,day);
  TFile *f = new TFile(fname);
  TTree *tree = (TTree*)f->Get("mcp");
  Int_t chnum[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  tree->Project("hvhisto(2000,1600,3600)","hv");
  TH1F* hvh = (TH1F*)gROOT ->FindObject("hvhisto");
  Int_t l; 
 s for(Int_t n = 0; n < hvh->GetXaxis()->GetNbins();n++){  
    if(hvh->GetBinContent(n+1) > 0) l = n+1;
  }
  HV  =(Int_t)hvh->GetXaxis()->GetBinCenter(l);
  
  TCanvas *c1 = new TCanvas("c1","c1");    
  c1->Divide(4,4);  
  TCanvas *c2 = new TCanvas("c2","c2");
  c2->Divide(4,4);
  TCanvas *c3 = new TCanvas("c3","c3");
  c3->Divide(4,4);
  for(Int_t n = 0; n<16;n++){
    //char HV_cut[128];
   //sprintf(HV_cut,"hv==%d",HV);
    //char ipos_cut[128];
    //sprintf(ipos_cut,"ipos==%d",chnum[n]);
    char iposHV_cut[128];
    //sprintf(iposHV_cut,"hv==%d&&ipos==15",HV);
    sprintf(iposHV_cut,"hv==%d&&ipos==%d",HV,chnum[n]);
    char tdc_cut[128];
    sprintf(tdc_cut,"1300<tdc[%d]&&tdc[%d]<1500",chnum[n],chnum[n]);
    char tdc_cut2[128];
    //sprintf(tdc_cut2,"1800<tdc[%d]&&tdc[%d]<1950",chnum[n],chnum[n]);
    sprintf(tdc_cut2,"1700<tdc[%d]&&tdc[%d]<1900",chnum[n],chnum[n]);
    char adc_cut[128];
    sprintf(adc_cut,"175<adc[%d]&&adc[%d]<200",chnum[n],chnum[n]);
    std::ostringstream cut;
    cut << tdc_cut << "&&" << iposHV_cut /*<< "&&" << adc_cut*/;
    std::ostringstream cut2;
    cut2 << tdc_cut2 << "&&" << iposHV_cut/* << "&&" << adc_cut*/;
    c1->cd(n+1);
    if( n<8 ){
      char tdc_ch[128];
      sprintf(tdc_ch,"tdc[%d]>>tdc%d(200,1300,1500)",chnum[n],chnum[n]);
      //tree->Draw(tdc_ch,iposHV_cut);
      tree->Draw(tdc_ch,cut.str().c_str());
    }
    else{
      char tdc_ch[128];
      sprintf(tdc_ch,"tdc[%d]>>tdc%d(200,1700,1900)",chnum[n],chnum[n]);
      //tree->Draw(tdc_ch,iposHV_cut);
      tree->Draw(tdc_ch,cut2.str().c_str());
    }
    char tdcnum[128];
    sprintf(tdcnum,"tdc%d",chnum[n]);
    TH1F* h1tdc = (TH1F*)gROOT ->FindObject(tdcnum);
    h1tdc -> SetXTitle("tdc[1/25ps]");
   
    
      c2->cd(n+1);
    //anvas *c2 = new TCanvas("c2","c2");
    if( n<8 ){
      char adc_ch[128];
      sprintf(adc_ch,"adc[%d]>>adc%d(200,50,250)",chnum[n],chnum[n]);
      tree->Draw(adc_ch, cut.str().c_str());
      //tree->Draw(adc_ch,iposHV_cut);
    }
    else{
      char adc_ch[128];
      sprintf(adc_ch,"adc[%d]>>adc%d(200,50,250)",chnum[n],chnum[n]);
      tree->Draw(adc_ch,cut2.str().c_str());
    }
    char adcnum[128];
    sprintf(adcnum,"adc%d",chnum[n]);
    TH1F* h1adc = (TH1F*)gROOT ->FindObject(adcnum );
    h1adc -> SetXTitle("adc[1/0.25pC]"); 
s  
    
      c3->cd(n+1);
    //anvas *c3 = new TCanvas("c3","c3");
    if(n<8){
	char tdc_adc[128];
	sprintf(tdc_adc,"tdc[%d]:adc[%d]>>tdc_adc%d(120,80,200,200,1300,1500)",chnum[n],chnum[n],chnum[n]);
	tree->Draw(tdc_adc,cut.str().c_str(),"colz");
    }
    else{
      char tdc_adc[128];
      sprintf(tdc_adc,"tdc[%d]:adc[%d]>>tdc_adc%d(120,80,200,200,1700,1900)",chnum[n],chnum[n],chnum[n]);
      tree->Draw(tdc_adc,cut2.str().c_str(),"colz");
      }
    char tdc_adcnum[128];
    sprintf(tdc_adcnum,"tdc_adc%d",chnum[n]);
    TH2F *h1tdc_adc = (TH2F*)gROOT ->FindObject(tdc_adcnum);
    h1tdc_adc -> SetXTitle("adc[1/0.25pC]");
    h1tdc_adc-> SetYTitle("tdc[1/25ps]");
    h1tdc_adc-> SetTitle(tdc_adcnum);
  }
  /*c1.Print("kihon.ps(");//attention print name
  c2.Print("kihon.ps");
  c3.Print("kihon.ps)");*/
}
