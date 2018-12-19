#include "utility.h"
#include "LifetimeAna.h"

using namespace std;

int main(int argc,char *argv[]){

  if(argc != 2){
    cerr << "Usage: ./LEDSummary [run no]" << endl;
    return -1;
  }
  int i = 0;
  int Nrun = atoi(argv[1]);
  char fname[128];
    
  LifetimeAna *lifeana = new LifetimeAna("../config.txt");
  TApplication *app = new TApplication("app",&argc,argv);
  sprintf(fname,"/home/takt/lifetime/dataset/%s/data/laser/lifetime_laser%d.root",lifeana->DirName,Nrun);
  TFile *f_laser = new TFile(fname);
  TTree *tree_laser = (TTree*)f_laser->Get("mcp");
  lifeana->SetLaserAddress_MCP(tree_laser);
  lifeana->GetPedestal(tree_laser);
  f_laser->Close();

  lifeana->EraseTempdata();
  
  while(1){
    sprintf(fname,"/home/takt/lifetime/dataset/%s/data/led/lifetime_LED%d_file%d.root",lifeana->DirName,Nrun,i);
    TFile *f = new TFile(fname,"read");
    if(f->IsZombie())break;
    if(f->GetBytesRead() < 1000){
      cerr << "This file has only " << f->GetBytesRead() << " bytes." << endl;
      break;
    }
    TTree *tree1 = (TTree*)f->Get("mcp");
    TTree *tree2 = (TTree*)f->Get("config");
    lifeana->SetLaserAddress_MCP(tree1);
    lifeana->SetLaserAddress_CONF(tree2);
    lifeana->GetCharge(tree1,tree2,i);
    //cout << f->IsZombie() << endl;
    f->Close();
    //delete f;
    i++;
  }

  sprintf(fname,"/home/takt/lifetime/dataset/%s/dst/led/LED_run%d.root",lifeana->DirName,Nrun);
  TFile *f_led = new TFile(fname,"recreate");
  TTree *tree_led = new TTree("led","led");
  TTree *tree_charge = new TTree("charge","charge");
  lifeana->SetBranch_LEDSummary(tree_led,tree_charge);
  lifeana->ReadData(tree_led);
  lifeana->SaveLEDsumData(tree_charge);
  tree_led->Write();
  tree_charge->Write();
  f_led->Close();
  delete lifeana;
  return 0;
}
