#include <iostream>
#include <fstream>
#include "TH2.h"
#include "Lifetime.h"
#include "TGraph.h"
#include "TF1.h"
#include "TLatex.h"

#define N_CALIB 0

class LifetimeAna : public Lifetime
{
 public:
  LifetimeAna(char *config);
  ~LifetimeAna();
  void SetBranch_LEDSummary(TTree *tree_mcp,TTree *tree_charge);
  bool SetLaserAddress_MCP(TTree *tree);
  bool SetLaserAddress_CONF(TTree *tree);
  void MakeHist(TTree *tree);
  void GetPedestal(TTree *tree);
  void DrawHist(TCanvas *c,int Nrun,int flag);
  bool SelectTrueHit(float adc_psbst[4],int ch); 
  void MakeCalibrationConstant(TTree *tree,int Ncalib,int PMT);
  void GetCalibrationFunction(TCanvas *c,int Ncalib); 
  bool GetCalibParameters(int Ncalib); 
  float ADCCalibration(int adc,int ch);
  void SaveLaserData(int Nrun);
  void EraseTempdata();
  void GetCharge(TTree *tree_mcp,TTree *tree_config,int Nfile);
  void SaveLEDsumData(TTree *tree_charge);
  void ReadData(TTree *tree_mcp);
  float Pedestal[33];
  float Pedestal2[32];//add by muroyama for ADC2

 private:
  TH1F *h_adc[33];
  TH1I *h_padc[33];
  TH1I *h_padc2[32];//add by muroyama for ADC2
  TH1I *h_tdc[33];
  TH1F *h_cadc[33];
  TH2F *h_2d[33];
  float att[13];
  float Q[4];
  int StartTime;
  int PrevTime;
  int LaserTime;
  typedef struct{
    unsigned short int adc[32];
    unsigned short int adc2[32];//add by muroyama since 2017 10/17
    unsigned short int tdc[32];
    unsigned short int adc_ref;
    unsigned short int tdc_ref;
    int time;
    int event;
  }laserMCP_t;
  typedef struct{
    float DAQrate;
    float LEDrate;
    float rPMTrate;
    float temp;
    float humid;
  }laserCONF_t;
  typedef struct{
    float TotalCharge[32];
    float TotalCharge4[8];
    float Charge[32];
    float Charge4[8];
    float Amp[32];
    float adc[32];
    float adc2[32];//add by muroyama since 2017 10/17
    float temp;
    float humid;
    int time;
    int file;
    float LEDrate;
  }LEDSummary_t;

  laserMCP_t GetLaser;
  laserCONF_t GetConfig;
  LEDSummary_t LEDsum;
  float CalibPar[32][4];
  float CalibPar2[16][8];//add by muroyama for 16ch amp calibration of adc
  int NoiseCount[33];
  float NoiseCorr[33];
  int ped_old[16];//add by muroyama
  ofstream fout;

};
