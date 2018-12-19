#include "utility.h"
#include "LifetimeAna.h"
#include "TLegend.h"

using namespace std;

float RefHitCorrection(float Nhit){
  char temp[256];
  int i = 0;
  int Ntotal = 0;
  ifstream fin("../config.txt");
  for(i = 0;i < 15;i++)fin.getline(temp,256);
  fin >> temp >> Ntotal;//basically N total = 5000000 nut sometimes it is changed to 500000. 
  float R = Nhit / Ntotal;
  if(R < 0.1){
    Ntotal = 500000;
    R = Nhit / Ntotal;
  }else if(R > 1){
    Ntotal = 5000000;
    R = Nhit / Ntotal;
  }
  float lambda = -log(1 - R);
  cout << "Nhit: " << R << "->" << lambda << endl;
  return lambda * Ntotal;
}

int main(int argc,char *argv[]){

  if(argc < 2 || argc > 3){
    cerr << "Usage: ./LaserSummary [run no]" << endl;
    return -1;
  }

  if(atoi(argv[1]) <= 0){
    cerr << "Error: [run no] must be larger than 0. (" << argv[1] << ")" << endl;
    return -1;
  }

  bool flag_end = false;

  if(argc == 3){
    if(atoi(argv[2]) == 1)flag_end = true;
  }

  int color[8] = {1,2,3,4,1,2,3,4};
  int style[8] = {1,1,1,1,2,2,2,2};

  int i = 0;
  int j = 0;
  int k = 0;
  int StartRun = 0;
  int Nrun = atoi(argv[1]) + 1;
  ifstream fin;
  char fname[128];
  char buf[64];
  char psname[128];
  float Nhit[32][1000];
  float Nhit_err[32][1000];
  float Nhit_ref = 0;
  float gain[32];
  float gain_err[32];
  float Nhit4[8][1000];
  float gain4[8][1000];
  float Nhit_ref_corr = 0;
  float rQE[8][1000];
  float rQEch[32][1000];
  float rgain[8][1000];
  float rQE_err[8][1000];
  float rQEch_err[32][1000];
  float rgain_err[8][1000];
  float gain4_err[8][1000];
  float GetCharge4[8];
  float GetCharge[32];
  float day[1000];
  float init_day = 0;
  float Nhitsum = 0;
  float Nhit4_err[8][1000];
  float rQE_temp = 0;
  float rQE_temp_err = 0;

  float rQE0[8];
  float rgain0[8];
  float charge[8][1000] = {0};
  float charge_rQE[8][1000] = {0};
  float max_charge = 0;
  float max_charge_rQE = 0;

  typedef struct{
    float rQE[32];
    float rQE4[8];
    float rQE_err[32];
    float rQE4_err[32];
    float gain[32];
    float gain_err[32];
    float gain4[8];
    float gain4_err[8];
    float rgain[32];
    float rgain_err[32];
    float rgain4[8];
    float rgain4_err[8];
    float charge[32];
    float charge4[32];
    float day;
    float Nhit_ref;
  }fill_t;
  fill_t summary;
  int Npoints[8] = {0};

  float prevQ[32] = {0};

  TApplication *app = new TApplication("app",&argc,argv);
  //TGraph *g[8];
  //for(i = 0;i < 8;i++)g[i] = new TGraph();
  LifetimeAna *lifeana = new LifetimeAna("../config.txt");

  //sprintf(lifeana->DirName,"life01");

  sprintf(fname,"/home/takt/lifetime/dataset/%s/dst/laser/laser_run%d.root",lifeana->DirName,Nrun-1);
  //TFile *f_new = new TFile(fname,"recreate");
  //TTree *tree_new = new TTree("laser","laser");
  //tree_new->Branch("rQE",summary.rQE,"rQE[32]/F");
  //tree_new->Branch("rQE4",summary.rQE4,"rQE4[8]/F");
  //tree_new->Branch("rQE_err",summary.rQE_err,"rQE_err[32]/F");
  //tree_new->Branch("rQE4_err",summary.rQE4_err,"rQE4_err[8]/F");
  //tree_new->Branch("gain",summary.gain,"gain[32]/F");
  //tree_new->Branch("gain4",summary.gain4,"gain4[8]/F");
  //tree_new->Branch("gain_err",summary.gain_err,"gain_err[32]/F");
  //tree_new->Branch("gain4_err",summary.gain4_err,"gain4_err[8]/F");
  //tree_new->Branch("rgain4",summary.rgain4,"rgain4[8]/F");
  //tree_new->Branch("rgain4_err",summary.rgain4_err,"rgain4_err[8]/F");
  //tree_new->Branch("charge",summary.charge,"charge[32]/F");
  //tree_new->Branch("charge4",summary.charge4,"charge4[32]/F");
  //tree_new->Branch("day",&summary.day,"day/F");
  //tree_new->Branch("Nhit_ref",&summary.Nhit_ref,"Nhit_ref/F");

  for(i = 0;i < Nrun;i++){
    sprintf(fname,"/home/takt/lifetime/dataset/%s/converted/ascii/lifetime%d.dat",lifeana->DirName,i);
    fin.open(fname);
    if(!fin){
      cerr << "Error: " << fname << " does not exist." << endl;
      return -1;
    }
    for(j = 0;j < 32;j++){
      fin >> Nhit[j][i] >> gain[j] >> gain_err[j];
      summary.gain[j] = gain[j];
      summary.gain_err[j] = gain_err[j];
      //cout << "Nhitch: " << j << " " << Nhit[j][i] << endl;
    }
    fin >> Nhit_ref >> buf >> buf;
    fin >> day[i];
    //if(i == 0)init_day = day[i];
    //day[i] = (day[i] - init_day) / 3600;
    //cout << "DAYS=" << day[i] << " " << init_day << endl;
    Nhit_ref = RefHitCorrection(Nhit_ref);
    summary.Nhit_ref = Nhit_ref;
    fin.close();

    for(j = 0;j < 8;j++){
      Nhitsum = Nhit[4 * j][i] + Nhit[4 * j + 1][i] + Nhit[4 * j + 3][i] + Nhit[4 * j + 3][i];
      Nhit4[j][i] = Nhitsum / Nhit_ref;
      Nhit4_err[j][i] = sqrt(pow(sqrt(Nhitsum) / Nhit_ref,2) + pow(Nhitsum / Nhit_ref / Nhit_ref * sqrt(Nhit_ref),2));

      for(k = 0;k < 4;k++){
	//cout << Nhit[4 * j + k][i] << "->";
	Nhit_err[4 * j + k][i] = sqrt(pow(sqrt(Nhit[4 * j + k][i]) / Nhit_ref,2) + pow(Nhit[4 * j + k][i] / Nhit_ref / Nhit_ref * sqrt(Nhit_ref),2));
	Nhit[4 * j + k][i] = Nhit[4 * j + k][i] / Nhit_ref;
	//cout << Nhit[4 * j + k][i] << endl;
      }

      //cout << Nhitsum << " " << Nhit_ref << " " << Nhit4[j][i] << " " << Nhit4_err[j][i] << endl;
      gain4[j][i] = gain[4 * j] + gain[4 * j + 1] + gain[4 * j + 3] + gain[4 * j + 3];
      gain4_err[j][i] = sqrt(pow(gain_err[4 * j],2) + pow(gain_err[4 * j + 1],2) + pow(gain_err[4 * j + 2],2) + pow(gain_err[4 * j + 3],2));
      //cout << "gain: " << gain4[j][i] << " " << gain4_err[j][i] << endl;
      summary.gain4[j] = gain4[j][i];
      summary.gain4_err[j] = gain4_err[j][i];
    
      if(i >= lifeana->StartQE){
	if(Nhit4[j][lifeana->StartQE] > 0.001){
	  rQE[j][ Npoints[j] ] = Nhit4[j][i] / Nhit4[j][lifeana->StartQE] * lifeana->initQE[j];
	  //cout << "NNN" << Npoints[j] << endl;
	  rQE_temp_err = sqrt(pow(Nhit4_err[j][i] / Nhit4[j][lifeana->StartQE],2) + pow(Nhit4[j][i] / Nhit4[j][lifeana->StartQE] / Nhit4[j][lifeana->StartQE] * Nhit4_err[j][lifeana->StartQE],2));
	  rQE_err[j][ Npoints[j] ] = sqrt(pow(rQE_temp_err * lifeana->initQE[j],2) + pow(rQE_temp * lifeana->initQE_err[j],2));

	  for(k = 0;k < 4;k++){
	    // cout << j << ", " << k << endl;
	    // cout << "AAA" << endl;
	    rQEch[4 * j + k][ Npoints[j] ] = Nhit[4 * j + k][i] / Nhit[4 * j + k][lifeana->StartQE] * lifeana->initQE[j];
	    //cout << "BBB" << " " << Nhit_err[4 * j + k][i] << " " << Nhit[4 * j + k][lifeana->StartQE] << " " << lifeana->StartQE << " " << Nhit4[j][i] << " " << Nhit4[j][lifeana->StartQE] << endl;
	    rQE_temp_err = sqrt(pow(Nhit_err[4 * j + k][i] / Nhit[4 * j + k][lifeana->StartQE],2) + pow(Nhit[4 * j + k][i] / Nhit[4 * j + k][lifeana->StartQE] / Nhit[4 * j + k][lifeana->StartQE] * Nhit_err[4 * j + k][lifeana->StartQE],2));
	    //cout << "CCC " << rQE_temp_err << endl;
	    
	    rQEch_err[4 * j + k][ Npoints[j] ] = sqrt(pow(rQE_temp_err * lifeana->initQE[j],2) + pow(rQE_temp * lifeana->initQE_err[j],2));
	    //cout << "DDD" << endl;
	    //cout << Npoints[j] << " " << rQEch[4 * j + k][ Npoints[j] ] << " " << rQEch_err[4 * j + k][ Npoints[j] ] << endl;
	  }
      


	}else{
	  rQE[j][ Npoints[j] ] = 0;
	  rQE_err[j][ Npoints[j] ] = 0;
	  
	  for(k = 0;k < 4;k++){
	    rQEch[4 * j + k][ Npoints[j] ] = 0;
	    rQEch_err[4 * j + k][ Npoints[j] ] = 0;
	  }
	  
	}

	//cout << "OK" << endl;
	if(gain4[j][lifeana->StartQE] != 0){
	  rgain[j][i] = gain4[j][i] / gain4[j][lifeana->StartQE];
	  rgain_err[j][i] = sqrt(pow(gain4_err[j][i] / gain4[j][lifeana->StartQE],2) + pow(gain4[j][i] / gain4[j][lifeana->StartQE] / gain4[j][lifeana->StartQE] * gain4_err[j][lifeana->StartQE],2));
	}
	else rgain[j][i] = 0;
      }else{
	rQE[j][ Npoints[j] ] = 1 * lifeana->initQE[j];
	rgain[j][i] = 1;
	rQE_err[j][ Npoints[j] ] = 0;
	rgain_err[j][i] = 0;
      
	for(k = 0;k < 4;k++){
	  rQEch[4 * j + k][ Npoints[j] ] = 1 * lifeana->initQE[j];
	  rQEch_err[4 * j + k][ Npoints[j] ] = 0;
	}
	
      }
      summary.rQE[j] = rQE[j][ Npoints[j] ];
      summary.rgain[j] = rgain[j][i];
      summary.rQE_err[j] = rQE_err[j][ Npoints[j] ];
      summary.rgain_err[j] = rgain_err[j][i];
    }

    if(i == 0){
      for(j = 0;j < 8;j++){
	charge[j][i] = 0;
	charge_rQE[j][i] = 0;
	summary.charge4[j] = 0;
      }
      for(j = 0;j < 32;j++){
	summary.charge[j] = 0;
      }
    }else if(i < Nrun && i > 0){
      sprintf(fname,"/home/takt/lifetime/dataset/%s/dst/led/LED_run%d.root",lifeana->DirName,i - 1);
      TFile *f = new TFile(fname);
      if(f->IsZombie())return -1;
      TTree *tree = (TTree*)f->Get("charge");
      tree->SetBranchAddress("total_charge4",GetCharge4);
      tree->SetBranchAddress("total_charge",GetCharge);
      tree->GetEntry(0);
      for(j = 0;j < 8;j++){
	charge[j][i] = GetCharge4[j] / 1000;
	summary.charge4[j] = GetCharge4[j] / 1000;
	if(i > 0){
	  charge[j][i] = GetCharge4[j] / 1000 + charge[j][i - 1];
	  charge_rQE[j][Npoints[j]] = charge[j][i] + lifeana->PreCharge[j];
	  summary.charge4[j] = GetCharge4[j] / 1000 + charge[j][i - 1];
	}
	cout << "Q=" << i << " " << charge[j][i] << endl;
	if(rQE[j][ Npoints[j] ] > 0)Npoints[j]++;
      }
      for(j = 0;j < 32;j++){
	summary.charge[j] = GetCharge[j] / 1000 + prevQ[j];
	prevQ[j] = summary.charge[j];
      }
    }
    //tree_new->Fill();
  }
  for(j = 0;j < 8;j++){
    if(max_charge < charge[j][i - 1]){
      max_charge = charge[j][i - 1];
    }
    if(max_charge_rQE < charge_rQE[j][Npoints[j] - 1]){
      max_charge_rQE = charge_rQE[j][Npoints[j] - 1];
    }
  }
  
  //tree_new->Write();
  //f_new->Close();

  for(j = 0;j < 8;j++){
    //rQE[j][0] = 1;
    //rQE_err[j][0] = 0;
  }

  cout << "Max charge=" << max_charge << endl;
  TGraphErrors *g_rQE[8];
  TGraphErrors *g_rQE2[8];
  TGraphErrors *g_rQEch[32];
  TGraphErrors *g_rgain[8];
  TGraphErrors *g_day[8];
  ofstream fout("../misc/log");
  for(j = 0;j < 8;j++){
    g_rQE[j] = new TGraphErrors(Npoints[j],charge_rQE[j],rQE[j],0,rQE_err[j]);
    g_rQE2[j] = new TGraphErrors();
    g_rgain[j] = new TGraphErrors(i,charge[j],rgain[j],0,rgain_err[j]);
    g_day[j] = new TGraphErrors(i,day,charge[j],0,0);
    fout << "PMT" << j + 1 << ": rQE=" << rQE[j][Npoints[j] - 1] << "+/-" << rQE_err[j][Npoints[j] - 1] << ", charge=" << charge_rQE[j][Npoints[j] - 1] << endl;
    for(int k=0;k < i - 1;k++){
      g_rQE2[j]->SetPoint(k,charge[j][k + 1],rQE[j][k]);
      g_rQE2[j]->SetPointError(k,0,rQE_err[j][k]);
    }
  }
  
  fout.close();

  TCanvas *c1 = new TCanvas("c1","c1");
  gPad->DrawFrame(0,0,max_charge_rQE,1.5,"Relative QE;Output charge (mC/cm^{2});Relative QE");
  TLegend *leg = new TLegend(0.9,0.7,1,0.9,"","NDC");
  for(j = 0;j < 8;j++){
    g_rQE[j]->SetLineColor(color[j]);
    g_rQE[j]->SetLineStyle(style[j]);
    g_rQE[j]->SetMarkerColor(color[j]);
    g_rQE[j]->SetLineWidth(2);
    g_rQE[j]->Draw("pl");
    leg->AddEntry(g_rQE[j],lifeana->PMTname[j],"pl");
  }
  leg->Draw();
  gPad->SetGridx();
  gPad->SetGridy();
  sprintf(psname,"/home/takt/lifetime/dataset/%s/summary/lifetime_run%d.ps(",lifeana->DirName,Nrun - 1);
  c1->Print(psname);
  
  TCanvas *c1_1 = new TCanvas("c1_1","c1_1");
  gPad->DrawFrame(0,0,max_charge,1.5,"Relative QE;Output charge (mC/cm^{2});Relative QE");
  for(j = 0;j < 8;j++){
    g_rQE2[j]->SetLineColor(color[j]);
    g_rQE2[j]->SetLineStyle(style[j]);
    g_rQE2[j]->SetMarkerColor(color[j]);
    g_rQE2[j]->SetLineWidth(2);
    g_rQE2[j]->Draw("pl");
  }
  leg->Draw();
  gPad->SetGridx();
  gPad->SetGridy();
  sprintf(psname,"/home/takt/lifetime/dataset/%s/summary/lifetime_run%d.ps",lifeana->DirName,Nrun - 1);
  c1_1->Print(psname);
  


  TCanvas *c2 = new TCanvas("c2","c2");
  gPad->DrawFrame(0,0,max_charge,5,"Relative Gain;Output charge (mC/cm^{2});Relative Gain");
  for(j = 0;j < 8;j++){
    g_rgain[j]->SetLineColor(color[j]);
    g_rgain[j]->SetMarkerColor(color[j]);
    g_rgain[j]->SetLineStyle(style[j]);
    g_rgain[j]->SetLineWidth(2);
    g_rgain[j]->Draw("pl");
  }
  leg->Draw();
  gPad->SetGridx();
  gPad->SetGridy();
  sprintf(psname,"/home/takt/lifetime/dataset/%s/summary/lifetime_run%d.ps",lifeana->DirName,Nrun - 1);
  c2->Print(psname);

  TCanvas *c3 = new TCanvas("c3","c3");
  TH1F *draw = c3->DrawFrame(day[0],0,day[i - 1],max_charge,"Output charge;;Output charge (mC/cm^{2})");
  gStyle->SetTimeOffset(0);
  draw->GetXaxis()->SetTimeDisplay(1);
  draw->GetXaxis()->SetTimeFormat("#splitline{20%y}{%m/%d}");
  gStyle->SetLabelOffset(0.1,"X");
  for(j = 0;j < 8;j++){
    g_day[j]->SetLineColor(color[j]);
    g_day[j]->SetMarkerColor(color[j]);
    g_day[j]->SetLineStyle(style[j]);
    g_day[j]->SetLineWidth(2);
    g_day[j]->Draw("pl");
  }

  leg->Draw();
  gPad->SetGridx();
  gPad->SetGridy();

  double gx = 0;
  double gy = 0;
  double er = 0;
  double gx1=0;
  double gy1=0;
  double er1=0;
  char outname[128];
  ofstream foutsum;
  //char PMTname[8][64] = {"none","none","none","none","KT0074","KT0139","JT0940","XM0376"};
  for(i = 0;i < 8;i++){
    sprintf(outname,"/home/takt/lifetime/dataset/%s/summary/%s.dat",lifeana->DirName,lifeana->PMTname[i]);
    foutsum.open(outname,ios::app);
    //foutsum.open(outname);
    for(j = 0;j < Npoints[i]+10;j++){
      g_rQE[i]->GetPoint(j,gx,gy);
      er = g_rQE[i]->GetErrorY(j);
      g_rgain[i]->GetPoint(j,gx1,gy1);
      er1=g_rgain[i]->GetErrorY(j);
      foutsum << gx << " " << gy << " 0 " << er << " " << gx1 << " " << gy1 << " 0 " << er1 << endl;
      //cout << i << " " << j << " " << gx << " " << gy << " " << er << endl;
    }
    foutsum.close();
  
    for(k = 0;k < 4;k++){
      sprintf(outname,"/home/takt/lifetime/dataset/%s/dst/laser/%s_ch%d.dat",lifeana->DirName,lifeana->PMTname[i],k);
      foutsum.open(outname);
      for(int l = 0;l < Npoints[i];l++){
	//cout << Npoints[i] << endl;
	foutsum << charge_rQE[i][l] << " " << rQEch[4 * i + k][l] << " 0 " << rQEch_err[4 * i + k][l] << endl;
	//cout << charge_rQE[i][l] << " " << rQEch[4 * i + k][l] << " 0 " << rQEch_err[4 * i + k][l] << endl;
      }
      foutsum.close();
    }
  }
  
  double x1, y1;
  double x2, y2;
  char temp_out[128];
  ofstream fout_corr;
  for(i = 0;i < 8;i++){
    sprintf(temp_out,"../misc/corr_PMT%d",i + 1);
    fout_corr.open(temp_out);
    for(j = 0;j < g_rQE2[i]->GetN() - 1;j++){
      g_rQE2[i]->GetPoint(j,x1,y1);
      g_rgain[i]->GetPoint(j + 1,x2,y2);
      fout_corr << y2 << " " << y1 << endl;
    }
    fout_corr.close();
  }

  sprintf(psname,"/home/takt/lifetime/dataset/%s/summary/lifetime_run%d.ps)",lifeana->DirName,Nrun - 1);
  c3->Print(psname);
  
  delete lifeana;

  cout << endl << endl;
  cout << "Click \"File\"->\"Quit ROOT\" in any canvas (do not use Ctrl+C)" << endl;

  if(!flag_end)app->Run();
  return 0;
}
