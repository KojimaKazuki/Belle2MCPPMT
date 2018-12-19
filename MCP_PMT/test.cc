#include "utility.h"

#define CH_LED 0x3
#define CH_LASER 0x18

int main( int argc, char *argv[] ){
  
  /*if(argc != 3 && argc != 4){
    cerr << "Usage: ./test [slot] [channel] [LED clock (option)]" << endl;
    return -1;
    }*/
  if(argc != 2 && argc != 3){
    cerr << "Usage: ./test [Hit number] [LED clock (option)]" << endl;
    return -1;
  }

  //int TDCslot[6] = {2, 2, 4, 4, 5, 5};//HV1ch0,HV1ch1,HV1ch2,HV1ch3,HV2ch0,HV2ch1 
  //int TDCslot[6] = {2, 2, 4, 4, 5, 5};//HV1ch0,HV1ch1,HV1ch3 test
  int TDCslot[6] = {2, 2, 4, 5, 7, 7};//HV1ch0,HV1ch1,HV1ch3,HV2ch1,HV2ch2,HV2ch3 
  //int TDCslot[6] = {2, 2, 2, 2, 7, 7};//test PMT1 PMT2
  //int TDCslot[6] = {4, 4, 4, 4, 7, 7};//test PMT3 PMT4
  int TDCafslot[6] = {6, 6, 6, 6, 6, 6};

  //int TDCafslot[6] = {5, 5, 5, 5, 6, 6};
  int ADC1slot[6] = {19,15,15,11,11,11};//HV1ch0,HV1ch1,HV1ch3,HV2ch1,HV2ch2,HV2ch3 
  //int ADC1slot[6] = {19,15,15,19,19,19};//HV1ch0,HV1ch1,HV1ch3 test
  //int ADC2slot[6] = {16,16,16,16,16,15};//test PMT1 PMT2 ADC2
  //int ADC2slot[6] = {19,19,19,19,16,15};//test PMT3 PMT4 ADC2
  //int ADC1slot[6] = {19,15,15,15,16,15};//test PMT1 ADC1
  //int ADC1slot[6] = {15,15,15,15,15,15};//test PMT2 PMT3
  //int ADC1slot[6] = {15,15, 8,11,15,15};//test PMT4 
  //int ch[12]   = {1, 0, 2, 4, 3, 0, 4, 4,0, 3, 7, 11}; 
  //int TDCch[6]   = {0, 4, 0, 4, 0, 4}; //HV1ch0,HV1ch1,HV1ch2,HV1ch3,HV2ch0,HV2ch1 
  //int TDCch[6]   = {0, 4, 4, 0, 4, 4}; //HV1ch0,HV1ch1,HV1ch3 test
  int TDCch[6]   = {0, 4, 4, 4, 0, 6};//HV1ch0,HV1ch1,HV1ch3,HV2ch1,HV2ch2,HV2ch3 
  //int TDCch[6]   = {0, 1, 2, 3, 0, 4};//test PMT1 PMT3
  //int TDCch[6]   = {4, 5, 6, 7, 0, 4};//test PMT2 PMT4
  //int TDCafch[6]   = {0, 2, 3, 4, 5, 6};//ref PMT
  int TDCafch[6]   = {1, 2, 3, 4, 5, 6};
  //int TDCafch[6]   = {4, 5, 6, 7, 5, 6};
  int ADC1ch[6]   = {0, 6,14, 5, 9,15};//HV1ch0,HV1ch1,HV1ch3,HV2ch1,HV2ch2,HV2ch3  
  //int ADC1ch[6]   = {0, 6,14,11,12,13}; //HV1ch0,HV1ch1,HV1ch3 test 
  //int ADC2ch[6]   = {3, 5, 6, 7, 9, 7};// test PMT1 ADC2
  //int ADC1ch[6]   = {0, 3, 4, 5, 9, 7};// test PMT1 ADC1
  //int ADC2ch[6]   = {8, 9, 10, 11,10,11}; //test PMT2 ADC2
  //int ADC1ch[6]   = {6, 7, 8, 9,10,11}; //test PMT2 ADC1
  //int ADC2ch[6]   = {2,3,4,5,10,11}; //test PMT3 ADC2
  //int ADC1ch[6]   = {10,11,12,13,10,11}; //test PMT3 ADC1
  //int ADC2ch[6]   = {7,8,9,10,10,11}; //test PMT4 ADC2
  //int ADC1ch[6]   = {14,15, 0,0,10,11}; //test PMT4 ADC1
  //int ADCafslot[6] = {16,16,16,16,16,16};//measurement for af charge
  //int ADCafch[6]   = { 3, 5, 6, 7, 8, 9};//measurement for af charge

  /*int Test[16] = {};
  int testch[16] = {}; 
  for(int a=0;a<16;a++){
    Test[a] = 19;//# of ch
    testch[a] = a;
    }*/
  int N = atoi(argv[1]); //hit number
  int write = 0;

  //if( argc == 3 ) write = 1;

  Camac *camac = new Camac();
  camac->Init(23);
  camac->Clear(23);
  
  if     ( write == 0 ) camac->Write(23, CH_LASER);
  else if( write == 1 ) camac->Write(23, CH_LED  ); //swich to laser and LED run
  
  //int tdc_af[8], tdc[8];
  int tdc[6];
  int tdc_af[6];
  int adc1[6];
  //int adc2[6];
  //int test[16];
  TFile *f = new TFile("../data/test/test.root", "recreate");
  //TFile *f = new TFile("af.root", "recreate");
  TTree *t = new TTree("t", "t");
  //t->Branch("tdc_af", tdc_af, "tdc_af[8]/I");
  //t->Branch("test", test, "test[16]/I");
  t->Branch("tdc", tdc, "tdc[6]/I");
  t->Branch("tdc_af", tdc_af, "tdc_af[6]/I");
  t->Branch("adc1", adc1, "adc1[6]/I");
  //t->Branch("adc_af", adc_af, "adc_af[6]/I");
  //t->Branch("adc2", adc2, "adc2[6]/I");
  
  for( int j=0; j<N; j++){ 
    /*for(int b = 0;b<16;b++){
      camac->Init(Test[b]);
      camac->Clear(Test[b]);
      camac->LAM(Test[b]);
      test[b] = camac->Read(Test[b],testch[b]);
      }*/
    for (int k = 0;k<6;k++){
      camac->Init(TDCslot[k]);
      camac->Clear(TDCslot[k]);
      
      camac->Init(TDCafslot[k]);
      camac->Clear(TDCafslot[k]);
      
      camac->Init(ADC1slot[k]);
      camac->Clear(ADC1slot[k]);
     
      //camac->Init(ADCafslot[k]);
      //camac->Clear(ADCafslot[k]);
      //camac->Init(ADC2slot[k]);
      //camac->Clear(ADC2slot[k]);
      
  
    }
     for (int k = 0;k<6;k++){ 
       camac->LAM(TDCslot[k]);
       camac->LAM(TDCafslot[k]);
       camac->LAM(ADC1slot[k]);
       //camac->LAM(ADCafslot[k]);
       //camac->LAM(ADC2slot[k]);
  
       tdc[k] = camac->Read(TDCslot[k], TDCch[k]);
       tdc_af[k] = camac->Read(TDCafslot[k], TDCafch[k]);
       adc1[k] = camac->Read(ADC1slot[k], ADC1ch[k]);
       //adc_af[k] = camac->Read(ADCafslot[k], ADCafch[k]);
       //adc2[k] = camac->Read(ADC2slot[k], ADC2ch[k]);
  
     }
     //if (k == 0) for( int i=0; i<8; i++ ) tdc_af[i] = camac->Read(slot[k], i);
     //else if (k == 1) for( int i=0; i<8; i++ ) tdc[i] = camac->Read(slot[k], i);
     //for (int k = 8;k<12;k++){ camac->LAM(slot[k]);
     //	adc[k-8] = camac->Read(slot[k], ch[k]);
  
     t->Fill();  
  }
  t->Write();
  f->Close();
  delete camac;
  return 0;
}
