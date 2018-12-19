#include "utility.h"
#include "LifetimeAna.h"

using namespace std;

int main(int argc,char *argv[]){
  
  if(argc < 2 || argc > 3){
    cerr << "Usage: ./MakePlot [Laser run #]" << endl;
    return -1;
  }
  
  char fname[128];
  int run = atoi(argv[1]);
  TH1I *h_adc;
  TH1F *h_cadc;
  TH1I *h_tdc;
  TH2I *h_2d;
  int flag = 0;
  if(argc == 3)flag = 1;

  TApplication *app = new TApplication("app",&argc,argv);
  TCanvas *c1 = new TCanvas("c1","c1");
  
  LifetimeAna *lifeana = new LifetimeAna("../config.txt");
  sprintf(fname,"/home/takt/lifetime/dataset/%s/data/laser/lifetime_laser%d.root",lifeana->DirName,run);
  cout << fname << endl;
  TFile *f = new TFile(fname);
  if(f->IsZombie()){
    cerr << fname << " does not exist." << endl;
    return -1;
  }
  TTree *tree = (TTree*)f->Get("mcp");
  if(!lifeana->SetLaserAddress_MCP(tree)) return -1;
  lifeana->GetCalibParameters(0);
  lifeana->MakeHist(tree);
  lifeana->DrawHist(c1,run,flag);
  lifeana->SaveLaserData(run);
  
  delete lifeana;
  //app->Run();
  return 0;
}
