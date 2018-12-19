#include "utility.h"

#define CH_LED 0x3
#define CH_LASER 0x18

int main( int argc, char *argv[] ){
  
  /*if(argc != 3 && argc != 4){
    cerr << "Usage: ./test2 [slot] [channel] [LED clock (option)]" << endl;
    return -1;
    }*/
  if(argc != 2 && argc != 3){
    cerr << "Usage: ./test2 [Hit number] [LED clock (option)]" << endl;
    return -1;
  }
  int slot[8] = {17,17,17,17,11,11,11,15};
  int ch[8]   = {8,9,10,11,13,14,15,0}; 
  int N = atoi(argv[1]); //hit number
  int write = 0;

  //if( argc == 3 ) write = 1;

  Camac *camac = new Camac();
  camac->Init(23);
  camac->Clear(23);
  
  if     ( write == 0 ) camac->Write(23, CH_LASER);
  else if( write == 1 ) camac->Write(23, CH_LED  ); //swich to laser and LED run
  
  //int tdc_af[8], tdc[8];
  int adc1[4];
  int adc2[4];
  TFile *f = new TFile("ADCcalib_HV2ch3.root", "recreate");
  TTree *t = new TTree("t", "t");
  t->Branch("adc1", adc1, "adc1[4]/I");
  t->Branch("adc2", adc2, "adc2[4]/I");
  for( int j=0; j<N; j++){ 
    for (int k = 0;k<8;k++){
      camac->Init(slot[k]);
      camac->Clear(slot[k]);
    }
    for (int k = 0;k<4;k++){
      camac->LAM(slot[k]);
      //if (k == 0) for( int i=0; i<8; i++ ) tdc_af[i] = camac->Read(slot[k], i);
      adc1[k] = camac->Read(slot[k], ch[k]);
      //else if (k == 1) for( int i=0; i<8; i++ ) tdc[i] = camac->Read(slot[k], i);
    }
    for (int k = 4;k<8;k++){
      camac->LAM(slot[k]);
      adc2[k-4] = camac->Read(slot[k], ch[k]);
    }
      t->Fill();  
  }
  t->Write();
  f->Close();
  delete camac;
  return 0;
}
