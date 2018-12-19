#include "utility.h"

using namespace std;

int main(int argc,char *argv[]){

  if(argc != 2){
    cerr << "Usage: ./lifetime [type]" << endl;
    cerr << "  [type] = led, laser, pedestal or test" << endl;
    return -1;
  }
  
  char type[16];
  char fname[128];
  sprintf(type,"%s",argv[1]);

  if(strcmp(type,"led") != 0 && strcmp(type,"laser") != 0 && strcmp(type,"pedestal") != 0 && strcmp(type,"test") != 0){
    cerr << "Error: [type] must be led, laser, pedestal or test." << endl;
    return -1;
  }

  TApplication *app = new TApplication("app",&argc,argv);
  TCanvas *c_adc1 = new TCanvas("c_adc1","c_adc1",0,0,600,400);
  TCanvas *c_adc2 = new TCanvas("c_adc2","c_adc2",600,0,600,400);
  TCanvas *c_tdc1 = new TCanvas("c_tdc1","c_tdc1",0,400,600,400);
  TCanvas *c_tdc2 = new TCanvas("c_tdc2","c_tdc2",600,400,600,400);
  //TCanvas *c_tdc_af = new TCanvas("c_tdc_af","c_tdc_af",600,400,600,400);//for after-pulse monitor

  Lifetime *life = new Lifetime("../config.txt",type);
  life->InitCamac();
  if(!life->SetFileName(type,fname)){
    delete life;
    return -1;
  }

  if( strcmp(type,"led") == 0 || strcmp(type,"test") == 0 ){
    life->TDCregion_MCP[0] = 1;
    life->TDCregion_MCP[1] = 4000;
    life->TDCregion_MCP[2] = 1;
    life->TDCregion_MCP[3] = 4000;
    life->TDCregion_MCP[4] = 1;
    life->TDCregion_MCP[5] = 4000;
  }
  
  TFile *f = new TFile(fname,"recreate");
  TTree *tree1 = new TTree("mcp","mcp");
  TTree *tree2 = new TTree("config","config");
  TTree *tree3 = new TTree("af","af");
  
  life->SetBranch(tree1,tree2,tree3);
  //life->ReadCanvas(c_adc1,c_adc2,c_tdc1,c_tdc2,c_tdc_af);//for after-pulse monitor
  life->ReadCanvas(c_adc1,c_adc2,c_tdc1,c_tdc2);
  life->GetTempHumid("192.168.6.90");
  if(!life->MainLoop(type,tree1,tree2,tree3)){
    cout << endl << endl << endl;
    cout << "   **************************" << endl;
    cout << "   * !!! DAQ was failed !!! *" << endl;
    cout << "   **************************" << endl;
    cout << endl << endl << endl;
    return -1;
  }
  
  
  tree1->Write();
  tree2->Write();
  tree3->Write();
  f->Close();

  life->CompleteFlag();
  life->EndCamac();

  char ssh[256];
  //cout << "commad: " << ssh << endl;
  //sprintf(ssh,"ssh bfactory@localhost 'rsync -avr --progress /home/takt/lifetime/dataset/%s /panfs/higgs/bfactory/mcppmt_data/life/dataset'",life->DirName);
  sprintf(ssh,"ssh bfactory@localhost 'rsync -avr --progress /home/takt/lifetime/dataset/%s /chai/sgt1/bfactory/mcppmt_data/life/dataset'",life->DirName);

  delete life;
  
  //system(ssh);

  return 0;

}
