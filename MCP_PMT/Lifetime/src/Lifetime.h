#ifndef LIFETIME_H_INCLUDED
#define LIFETIME_H_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <math.h>
#include <cstdlib>

#include "Camac.h"

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TH1.h"
#include "TROOT.h"
#include "TStyle.h"

#define CANVAS1 "c_adc1"
#define CANVAS2 "c_adc2"
#define CANVAS3 "c_tdc1"
#define CANVAS4 "c_tdc2"
//#define CANVAS5 "c_tdc_af"//for after-pulse monitor

using namespace std;

class Lifetime
{

 public:
  Lifetime(char *config,char *type);
  ~Lifetime();
  void InitCamac();
  void EndCamac();
  bool SetFileName(char *type,char *rootname);
  //add the new tree of main_af in SetBranch and MainLoop and DAQ
  void SetBranch(TTree *tree_main,TTree *tree_config,TTree *tree_af);//
  bool MainLoop(char *type,TTree *tree_main,TTree *tree_config,TTree *main_af);//
  bool DAQ(TTree *tree_main,TTree *tree_af,int event);
  bool DAQ_LED(TTree *tree_main,TTree *tree_af,int event);
  bool Scaler(int wait = 10);
  void GetTempHumid(char *ip);
  bool Monitor(char *type,int event);
  //void ReadCanvas(TCanvas *c1,TCanvas *c2,TCanvas *c3,TCanvas *c4,TCanvas *c5);//for after-pulse monitor
  void ReadCanvas(TCanvas *c1,TCanvas *c2,TCanvas *c3,TCanvas *c4);
  void CompleteFlag();
  void Calib(TTree  *tree_main,TTree *tree_af);

  char PMTname[8][64];
  int HV[8];
  int TDCslot[33];
  int TDCch[33];
  int ADCslot[33];
  int ADCch[33];
  int ADC2slot[33];
  int ADC2ch[33];
  //to take after pulse//
  int TDCafslot[6];
  int TDCafch[6];
  //                  //
  int SCALERslot;
  int DAQrate;
  int LEDrate;
  int rPMTrate;
  float AmpGain[32];
  int TDCregion_MCP[6];//
  int TDCregion_REF[2];
  int Nevent;
  int Nslot;
  vector<int> SlotList;
  Camac *cam;
  char DirName[16];
  int StartQE;
  float initQE[8];
  float initQE_err[8];
  double PreCharge[8];

 private:
  
  bool CheckRunNumber(char *type);
  void MakeSlotList();

  char RunType[16];
  char RootFileName[128];
  int PrevCount_ADC[33];
  int PrevCount_TDC[33];
  int PrevCount_TDC_af[6];
  int ScalerWait;
  int Nrun;
  int Nfile;
  bool flag_Canvas;
  bool flag_Complete;
  bool flag_CAMAC;
  TCanvas *c_adc1;
  TCanvas *c_adc2;
  TCanvas *c_tdc1;
  TCanvas *c_tdc2;
  //TCanvas *c_tdc_af;//for after-pulse monitor
  TH1I *h_adc[33];
  TH1I *h_tdc[33];
  TH1I *h_tdc_af[6];
  //define structure to insert TTree
  typedef struct{
    unsigned short int adc[32];
    unsigned short int adc2[32];//add by muroyama
    unsigned short int tdc[32];
    unsigned short int adc_ref;
    unsigned short int tdc_ref;
    unsigned short int tdc_af[6];
    unsigned short int tdc_main[6];
    unsigned short int adc_main[6];
    unsigned short int adc2_main[6];
    int time;
    int event;
  }data_t;

  typedef struct{
    char pmt1[32];
    char pmt2[32];
    char pmt3[32];
    char pmt4[32];
    char pmt5[32];
    char pmt6[32];
    char pmt7[32];
    char pmt8[32];
    int HV[8];
    float DAQrate;
    float LEDrate;
    float rPMTrate;
    float temp;
    float humid;
  }config_t;

  data_t TreeContent;
  config_t TreeConfig;

};

#endif //LIFETIME_H_INCLUDED
