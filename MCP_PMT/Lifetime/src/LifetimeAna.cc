#include "LifetimeAna.h"

using namespace std;

#define DEBUG 0


LifetimeAna::LifetimeAna(char *config) : Lifetime(config,"laser"){
  char hname[32];
  int i = 0;

  PrevTime = 0;

  for(i = 0;i < 32;i++){
    CalibPar[i][0] = 0;
    CalibPar[i][1] = 0;
    CalibPar[i][2] = 1;
    CalibPar[i][3] = 0;
    NoiseCount[i] = 0;
    LEDsum.TotalCharge[i] = 0;
    if(i < 4)LEDsum.TotalCharge4[i] = 0;
    /* calibration paraneter of 16ch amp (add by muroyama)*/
    if(i<16){
      CalibPar2[i][0] = 0;
      CalibPar2[i][1] = 0;
      CalibPar2[i][2] = 0;
      CalibPar2[i][3] = 0;
      CalibPar2[i][4] = 0;
      CalibPar2[i][5] = 0;
      CalibPar2[i][6] = 0;
      CalibPar2[i][7] = 0;
    }
  }
  NoiseCount[32] = 0;

  StartTime = -1;

  Q[0] = 4.34; //[pC]
  Q[1] = 4.31;
  Q[2] = 4.35;
  Q[3] = 4.38;
  
  att[0] = 0;
  att[1] = 0.69;
  att[2] = 1.65;
  att[3] = 2.55;
  att[4] = 3.90;
  att[5] = 4.82;
  att[6] = 5.72;
  att[7] = 6.76;
  att[8] = 8.17;
  att[9] = 9.22;
  att[10] = 9.87;
  att[11] = 10.75;
  att[12] = 80;

  ped_old[0] = 147;
  ped_old[1] = 86;
  ped_old[2] = 80;
  ped_old[3] = 82;
  ped_old[4] = 72;
  ped_old[5] = 77;
  ped_old[6] = 74;
  ped_old[7] = 71;
  ped_old[8] = 90;
  ped_old[9] = 91;
  ped_old[10] = 90;
  ped_old[11] = 90;
  ped_old[12] = 79;
  ped_old[13] = 122;
  ped_old[14] = 57;
  ped_old[15] = 199;//correction for ORTEC amp cADC
  
  /*
    att[0] = 0;
    att[1] = 1;
    att[2] = 2;
    att[3] = 3;
    att[4] = 4;
    att[5] = 5;
    att[6] = 6;
    att[7] = 7;
    att[8] = 8;
    att[9] = 9;
    att[10] = 10;
    att[11] = 11;
    att[12] = 999;
  */
  
  for(i = 0;i < 32;i++){
    sprintf(hname,"h_adc%d",i + 1);
    h_adc[i] = new TH1F(hname,hname,700,0,700);
    sprintf(hname,"h_tdc%d",i + 1);
    /*if(i < 24){
      h_tdc[i] = new TH1I(hname,hname,300,TDCregion_MCP[0],TDCregion_MCP[1]);
    }else{
      h_tdc[i] = new TH1I(hname,hname,300,TDCregion_MCP[0] - 100,TDCregion_MCP[1] - 100);
      }*/

    /* changed TDC window by muroyama since 2017 (life16) */
    if(i < 16){
      if(i == 0 || i == 4 || i == 10 || i == 12)h_tdc[i] = new TH1I(hname,hname,300,TDCregion_MCP[2],TDCregion_MCP[3]);
      else h_tdc[i] = new TH1I(hname,hname,300,TDCregion_MCP[0],TDCregion_MCP[1]);
    }else if(i<24){
      h_tdc[i] = new TH1I(hname,hname,300,TDCregion_MCP[4],TDCregion_MCP[5]);
    }else{
      h_tdc[i] = new TH1I(hname,hname,300,TDCregion_MCP[4] - 100,TDCregion_MCP[5] - 100);
    }
    sprintf(hname,"h_padc%d",i + 1);
    h_padc[i] = new TH1I(hname,hname,300,0,300);
    sprintf(hname,"h_padc_2%d",i + 1);
    h_padc2[i] = new TH1I(hname,hname,300,0,300);
    sprintf(hname,"h_cdc%d",i + 1);
    h_cadc[i] = new TH1F(hname,hname,100,-1,3);
    sprintf(hname,"h_2d%d",i + 1);
    /*if(i < 24){
      h_2d[i] = new TH2F(hname,hname,300,0,300,300,TDCregion_MCP[0],TDCregion_MCP[1]);
    }else{
      h_2d[i] = new TH2F(hname,hname,300,0,300,300,TDCregion_MCP[0] - 100,TDCregion_MCP[1] - 100);
      }*/
    /* changed TDC window by muroyama since 2017 (life16) */
    if(i < 16){
      if(i == 0 || i == 4 || i == 10 || i == 12)h_2d[i] = new TH2F(hname,hname,700,0,700,300,TDCregion_MCP[2],TDCregion_MCP[3]);
      else h_2d[i] = new TH2F(hname,hname,700,0,700,300,TDCregion_MCP[0],TDCregion_MCP[1]);
    }else if(i<24){
      h_2d[i] = new TH2F(hname,hname,300,0,300,300,TDCregion_MCP[4],TDCregion_MCP[5]);
    }else{
      h_2d[i] = new TH2F(hname,hname,300,0,300,300,TDCregion_MCP[4] - 100,TDCregion_MCP[5] - 100);
    }
  }
  h_adc[32] = new TH1F("h_adc_ref","h_adc_ref",500,0,500);
  h_tdc[32] = new TH1I("h_tdc_ref","h_tdc_ref",500,TDCregion_REF[0],TDCregion_REF[1]);
  h_padc[32] = new TH1I("h_padc_ref","h_padc_ref",500,0,500);
  h_cadc[32] = new TH1F("h_cadc_ref","h_cadc_ref",500,0,500);
  h_2d[32] = new TH2F("h_2d_ref","h_2d_ref",500,0,500,500,TDCregion_REF[0],TDCregion_REF[1]);
}

LifetimeAna::~LifetimeAna(){
  cout << "delete lifetime ana OK" << endl;
}

void LifetimeAna::SetBranch_LEDSummary(TTree *tree_mcp,TTree *tree_charge){
  tree_mcp->Branch("adc",LEDsum.adc,"adc[32]/F");//from 16ch amp
  tree_mcp->Branch("adc2",LEDsum.adc2,"adc2[32]/F");//from ORTEC amp
  tree_mcp->Branch("I_mcp",LEDsum.Charge,"I_mcp[32]/F");
  tree_mcp->Branch("I_mcp4",LEDsum.Charge4,"I_mcp4[8]/F");
  tree_mcp->Branch("temp",&LEDsum.temp,"temp/F");
  tree_mcp->Branch("humid",&LEDsum.humid,"humid/F");
  tree_mcp->Branch("time",&LEDsum.time,"time/I");
  tree_mcp->Branch("file",&LEDsum.file,"file/I");
  tree_mcp->Branch("LEDrate",&LEDsum.LEDrate,"LEDrate/F");

  tree_charge->Branch("total_charge",LEDsum.TotalCharge,"total_charge[32]/F");
  tree_charge->Branch("total_charge4",LEDsum.TotalCharge4,"total_charge4[8]/F");
  tree_charge->Branch("amp",LEDsum.Amp,"amp[32]/F");
  return;
}

bool LifetimeAna::SetLaserAddress_MCP(TTree *tree){
  /* if(tree->GetNbranches() != 7){//changed # of branch 6 to 7 by muroyama
    cerr << "Nbranches must be 7. (" << tree->GetNbranches() << ")" << endl;
    return false;
    }*/
  tree->SetBranchAddress("adc",GetLaser.adc);//from 16ch amp
  tree->SetBranchAddress("adc2",GetLaser.adc2);//from ORTEC amp
  tree->SetBranchAddress("tdc",GetLaser.tdc);
  tree->SetBranchAddress("adc_ref",&GetLaser.adc_ref);
  tree->SetBranchAddress("tdc_ref",&GetLaser.tdc_ref);
  tree->SetBranchAddress("time",&GetLaser.time);
  tree->SetBranchAddress("event",&GetLaser.event);
  return true;
}

bool LifetimeAna::SetLaserAddress_CONF(TTree *tree){
  if(tree->GetNbranches() != 14){
    cerr << "Nbranches must be 14. (" << tree->GetNbranches() << ")" << endl;
    return false;
  }
  tree->SetBranchAddress("DAQrate",&GetConfig.DAQrate);
  tree->SetBranchAddress("LEDrate",&GetConfig.LEDrate);
  tree->SetBranchAddress("rPMTrate",&GetConfig.rPMTrate);
  tree->SetBranchAddress("temp",&GetConfig.temp);
  tree->SetBranchAddress("humid",&GetConfig.humid);
  return true;
}

void LifetimeAna::MakeHist(TTree *tree){
  int N = tree->GetEntries();
  int Nhit = 0;
  int i = 0;
  int j = 0;
  int k = 0;
  float adc_psbst[4];

  //TCanvas *c = new TCanvas("c1","c1");
  TH1I *hist = new TH1I("Nhit","Nhit",10,0,10);

  GetPedestal(tree);

  for(i = 0;i < N;i++){
    tree->GetEntry(i);
    Nhit = 0;
    if(i == 0)LaserTime = GetLaser.time;
    for(j = 0;j < 32;j++){
      /*if(j < 24){
	if(GetLaser.tdc[j] > TDCregion_MCP[0] && GetLaser.tdc[j] < TDCregion_MCP[1]){
	  h_adc[j]->Fill(GetLaser.adc[j]);
	  h_tdc[j]->Fill(GetLaser.tdc[j]);
	  h_2d[j]->Fill(GetLaser.adc[j],GetLaser.tdc[j]);
	  Nhit++;
	}
      }else{
	if(GetLaser.tdc[j] > TDCregion_MCP[0] - 100 && GetLaser.tdc[j] < TDCregion_MCP[1] - 100){
	  h_adc[j]->Fill(GetLaser.adc[j]);
	  h_tdc[j]->Fill(GetLaser.tdc[j]);
	  h_2d[j]->Fill(GetLaser.adc[j],GetLaser.tdc[j]);
	  Nhit++;
	  }*/
      /* changed TDC window by muroyama since 2017 (life16) */      
      if(j < 16){
	if(j == 0 || j == 4 || j == 10 || j == 12){
	  if(GetLaser.tdc[j] > TDCregion_MCP[2] && GetLaser.tdc[j] < TDCregion_MCP[3]){
	    h_adc[j]->Fill(GetLaser.adc[j]);
	    h_tdc[j]->Fill(GetLaser.tdc[j]);
	    h_2d[j]->Fill(GetLaser.adc[j],GetLaser.tdc[j]);
	    Nhit++;
	  }
	}else{	
	  if(GetLaser.tdc[j] > TDCregion_MCP[0] && GetLaser.tdc[j] < TDCregion_MCP[1]){
	    h_adc[j]->Fill(GetLaser.adc[j]);
	    h_tdc[j]->Fill(GetLaser.tdc[j]);
	    h_2d[j]->Fill(GetLaser.adc[j],GetLaser.tdc[j]);
	    Nhit++;
	  }
	}
      }else if (j < 24){
	if(GetLaser.tdc[j] > TDCregion_MCP[4] && GetLaser.tdc[j] < TDCregion_MCP[5]){
	  h_adc[j]->Fill(GetLaser.adc[j]);
	  h_tdc[j]->Fill(GetLaser.tdc[j]);
	  h_2d[j]->Fill(GetLaser.adc[j],GetLaser.tdc[j]);
	  Nhit++;
	}
      }else{
	if(GetLaser.tdc[j] > TDCregion_MCP[4] - 100 && GetLaser.tdc[j] < TDCregion_MCP[5] - 100){
	  h_adc[j]->Fill(GetLaser.adc[j]);
	  h_tdc[j]->Fill(GetLaser.tdc[j]);
	  h_2d[j]->Fill(GetLaser.adc[j],GetLaser.tdc[j]);
	  Nhit++;
	}
      }
      if(j == 0 && GetLaser.tdc[j] > 0 && GetLaser.tdc[j] < TDCregion_MCP[2])NoiseCount[j]++;
      if(j == 4 && GetLaser.tdc[j] > 0 && GetLaser.tdc[j] < TDCregion_MCP[2])NoiseCount[j]++;
      if(j == 10 && GetLaser.tdc[j] > 0 && GetLaser.tdc[j] < TDCregion_MCP[2])NoiseCount[j]++;
      if(j == 12 && GetLaser.tdc[j] > 0 && GetLaser.tdc[j] < TDCregion_MCP[2])NoiseCount[j]++;
      else if(j < 16 && GetLaser.tdc[j] > 0 && GetLaser.tdc[j] < TDCregion_MCP[0])NoiseCount[j]++;
      else if(j < 24 && GetLaser.tdc[j] > 0 && GetLaser.tdc[j] < TDCregion_MCP[4])NoiseCount[j]++;
      else if(j >= 24 && GetLaser.tdc[j] > 0 && GetLaser.tdc[j] < TDCregion_MCP[4] - 100)NoiseCount[j]++;
    }
    if(GetLaser.tdc_ref > TDCregion_REF[0] && GetLaser.tdc_ref < TDCregion_REF[1]){
      h_adc[32]->Fill(GetLaser.adc_ref);
      h_tdc[32]->Fill(GetLaser.tdc_ref);
      h_2d[32]->Fill(GetLaser.adc_ref,GetLaser.tdc_ref);
    }
    if(GetLaser.tdc_ref > 0 && GetLaser.tdc_ref < TDCregion_REF[0])NoiseCount[32]++;
    if(Nhit){
      for(j = 0;j < 8;j++){
	int add = 0;
	if(j == 7 || j == 6)add = - 100;
	for(k = 0;k < 4;k++)adc_psbst[k] = -100;
	//cout << GetLaser.tdc[4 * j] << " " << GetLaser.tdc[4 * j + 1] << " " << GetLaser.tdc[4 * j + 2] << " " << GetLaser.tdc[4 * j + 3] << endl;
	/*if(GetLaser.tdc[4 * j    ] > TDCregion_MCP[0] + add && GetLaser.tdc[4 * j    ] < TDCregion_MCP[1] + add)
	  adc_psbst[0] = GetLaser.adc[j * 4    ] - Pedestal[j * 4    ];
	if(GetLaser.tdc[4 * j + 1] > TDCregion_MCP[0] + add && GetLaser.tdc[4 * j + 1] < TDCregion_MCP[1] + add)
	  adc_psbst[1] = GetLaser.adc[j * 4 + 1] - Pedestal[j * 4 + 1];
	if(GetLaser.tdc[4 * j + 2] > TDCregion_MCP[0] + add && GetLaser.tdc[4 * j + 2] < TDCregion_MCP[1] + add)
	  adc_psbst[2] = GetLaser.adc[j * 4 + 2] - Pedestal[j * 4 + 2];
	if(GetLaser.tdc[4 * j + 3] > TDCregion_MCP[0] + add && GetLaser.tdc[4 * j + 3] < TDCregion_MCP[1] + add)
	adc_psbst[3] = GetLaser.adc[j * 4 + 3] - Pedestal[j * 4 + 3];*/
	if(j<4){
	  if(j == 2){
	    if(GetLaser.tdc[4 * j    ] > TDCregion_MCP[0] + add && GetLaser.tdc[4 * j    ] < TDCregion_MCP[1] + add)
	      adc_psbst[0] = GetLaser.adc[j * 4    ] - Pedestal[j * 4    ];
	  }else{
	    if(GetLaser.tdc[4 * j    ] > TDCregion_MCP[2] + add && GetLaser.tdc[4 * j    ] < TDCregion_MCP[3] + add)
	      adc_psbst[0] = GetLaser.adc[j * 4    ] - Pedestal[j * 4    ];//4*j == 0, 4, 12
	  }
	  if(GetLaser.tdc[4 * j + 1] > TDCregion_MCP[0] + add && GetLaser.tdc[4 * j + 1] < TDCregion_MCP[1] + add)
	    adc_psbst[1] = GetLaser.adc[j * 4 + 1] - Pedestal[j * 4 + 1];
	  if(j == 2){
	    if(GetLaser.tdc[4 * j + 2] > TDCregion_MCP[2] + add && GetLaser.tdc[4 * j + 2] < TDCregion_MCP[3] + add)
	      adc_psbst[2] = GetLaser.adc[j * 4 + 2] - Pedestal[j * 4 + 2];//4*j == 10
	  }else{
	    if(GetLaser.tdc[4 * j + 2] > TDCregion_MCP[0] + add && GetLaser.tdc[4 * j + 2] < TDCregion_MCP[1] + add)
	      adc_psbst[2] = GetLaser.adc[j * 4 + 2] - Pedestal[j * 4 + 2];
	  }
	  if(GetLaser.tdc[4 * j + 3] > TDCregion_MCP[0] + add && GetLaser.tdc[4 * j + 3] < TDCregion_MCP[1] + add)
	    adc_psbst[3] = GetLaser.adc[j * 4 + 3] - Pedestal[j * 4 + 3];
	}else{
	  if(GetLaser.tdc[4 * j    ] > TDCregion_MCP[4] + add && GetLaser.tdc[4 * j    ] < TDCregion_MCP[5] + add)
	    adc_psbst[0] = GetLaser.adc[j * 4    ] - Pedestal[j * 4    ];
	  if(GetLaser.tdc[4 * j + 1] > TDCregion_MCP[4] + add && GetLaser.tdc[4 * j + 1] < TDCregion_MCP[5] + add)
	    adc_psbst[1] = GetLaser.adc[j * 4 + 1] - Pedestal[j * 4 + 1];
	  if(GetLaser.tdc[4 * j + 2] > TDCregion_MCP[4] + add && GetLaser.tdc[4 * j + 2] < TDCregion_MCP[5] + add)
	    adc_psbst[2] = GetLaser.adc[j * 4 + 2] - Pedestal[j * 4 + 2];
	  if(GetLaser.tdc[4 * j + 3] > TDCregion_MCP[4] + add && GetLaser.tdc[4 * j + 3] < TDCregion_MCP[5] + add)
	    adc_psbst[3] = GetLaser.adc[j * 4 + 3] - Pedestal[j * 4 + 3];
	}
	for(k = 0;k < 4;k++){
	  if(SelectTrueHit(adc_psbst,k))h_cadc[4 * j + k]->Fill(ADCCalibration(GetLaser.adc[4 * j + k],4 * j + k));
	}
      }
    }
    if(i % 100000 == 99999)cout << i + 1 << " events done." << endl;
  }
  for(i = 0;i < 32;i++){
    int add = 0;
    if(i < 24)add = 0;
    else add = -100;
    if(i<16){
      if(i == 0 || i == 4 || i == 10 || i == 12)NoiseCorr[i] = (float)NoiseCount[i] * (TDCregion_MCP[3] - TDCregion_MCP[2]) / (TDCregion_MCP[2] + add);
      else NoiseCorr[i] = (float)NoiseCount[i] * (TDCregion_MCP[1] - TDCregion_MCP[0]) / (TDCregion_MCP[0] + add);

    }else NoiseCorr[i] = (float)NoiseCount[i] * (TDCregion_MCP[5] - TDCregion_MCP[4]) / (TDCregion_MCP[4] + add);
  }
  NoiseCorr[i] = (float)NoiseCount[i] * (TDCregion_REF[1] - TDCregion_REF[0]) / (TDCregion_REF[0]);
  cout << "Noise correction" << endl;
  //for(i = 0;i < 33;i++)cout << i << " " << NoiseCount[i] << endl;
  for(i = 0;i < 33;i++)cout << i << " " << NoiseCorr[i] << endl;
  return;
}

void LifetimeAna::GetPedestal(TTree *tree){
  int N = tree->GetEntries();
  int i = 0;
  int j = 0;

  for(i = 0;i < N;i++){
    tree->GetEntry(i);
    for(j = 0;j < 32;j++){
      if(GetLaser.tdc[j] > 4095){
	h_padc[j]->Fill(GetLaser.adc[j]);
	h_padc2[j]->Fill(GetLaser.adc2[j]);
      }
    }
    if(GetLaser.tdc_ref > 4095)h_padc[32]->Fill(GetLaser.adc_ref);
    if(i % 100000 == 99999)cout << i + 1 << " events done." << endl;
  }
  
  for(j = 0;j < 32;j++){
    Pedestal[j] = h_padc[j]->GetMean();
    Pedestal2[j] = h_padc2[j]->GetMean();
    if(DEBUG)cout << N << " " << h_padc[j]->GetMean() << " (ADC)" << endl;
    if(DEBUG)cout << N << " " << h_padc2[j]->GetMean() << " (ADC2)" << endl;
  }
  Pedestal[32] = h_padc[32]->GetMean();
  if(DEBUG)cout << N << " " << h_padc[32]->GetMean() << " (Ref)" << endl;
      
  return;
}

void LifetimeAna::DrawHist(TCanvas *c,int Nrun,int flag){
  int j = 0;
  char tex_hit[128];
  char tex_gain[128];
  char psname[128];

  for(j = 0;j < 32;j++){
    
    c->Clear();
    c->Divide(2,2);
    
    c->cd(1);
    h_adc[j]->Draw();
    h_padc[j]->SetLineColor(2);
    h_padc[j]->Draw("same");
    
    c->cd(2);
    sprintf(tex_hit,"N_{hit}=%d",(int)h_cadc[j]->GetEntries());
    sprintf(tex_gain,"gain=(%4.2f#pm%4.2f)#times10^{6}",h_cadc[j]->GetMean() / 0.16,h_cadc[j]->GetRMS() / 0.16 / sqrt(h_cadc[j]->GetEntries()));
    /*if(j<16){
      cout << "16ch amp Para" << endl;
      cout << CalibPar2[j][0] << " " << CalibPar2[j][1] << " " << CalibPar2[j][2] << " " << CalibPar2[j][3] << " " << CalibPar2[j][4] << " " << CalibPar2[j][5] << " " <<CalibPar2[j][6] << " " << CalibPar2[j][7] << " "<<endl;
      }*/
    h_cadc[j]->Draw();
    TLatex *hit = new TLatex(0.5,0.6,tex_hit);
    TLatex *gain = new TLatex(0.5,0.5,tex_gain);
    hit->SetNDC();
    hit->Draw();
    gain->SetNDC();
    gain->Draw();
    c->cd(3);
    h_tdc[j]->Draw();
    c->cd(4);
    h_2d[j]->Draw("colz");
    if(j == 0)sprintf(psname,"/home/takt/lifetime/dataset/%s/converted/psfile/kihon/lifetime%d.ps(",DirName,Nrun);
    else if(j == 31)sprintf(psname,"/home/takt/lifetime/dataset/%s/converted/psfile/kihon/lifetime%d.ps)",DirName,Nrun);
    else sprintf(psname,"/home/takt/lifetime/dataset/%s/converted/psfile/kihon/lifetime%d.ps",DirName,Nrun);
    //cout << psname << endl;
    c->Print(psname);
    c->Update();
    if(flag)getchar();
  }
  return;
}

void LifetimeAna::MakeCalibrationConstant(TTree *tree,int Ncalib,int PMT){
  int i = 0;
  int j = 0;
  char fname[128];

  PMT--;

  ofstream fout[4];
  for(j = 0;j < 4;j++){
    sprintf(fname,"textdata/calibration%d_ch%d.dat",Ncalib,PMT * 4 + j + 1);
    fout[j].open(fname);
  }
  for(i = 0;i < tree->GetEntries();i++){
    tree->GetEntry(i);
    if(i % 5000 == 0)for(j = 0;j < 32;j++)h_adc[j]->Reset();
    for(j = 0;j < 32;j++){
      h_adc[j]->Fill(GetLaser.adc[j]);
    }
    if(i % 5000 == 4999){
      for(j = 0;j < 4;j++)fout[j] << h_adc[PMT * 4 + j]->GetMean() << endl;
    }
  }
  for(j = 0;j < 4;j++){
    fout[j].close();
  }
  return ;
}

bool LifetimeAna::SelectTrueHit(float adc_psbst[4],int ch){
  int i = 0;
  int max = 0;
  float max_value = -100;
  //cout << adc_psbst[0] << " " << adc_psbst[1] << " " << adc_psbst[2] << " " << adc_psbst[3] << endl;
  for(i = 0;i < 4;i++){
    if(max_value < adc_psbst[i]){
      max = i;
      max_value = adc_psbst[i];
    }
  }
  if(max_value == -100)return false;
  //cout << max << " " << ch << endl;
  if(max == ch)return true;
  else return false;
}

void LifetimeAna::GetCalibrationFunction(TCanvas *c,int Ncalib){
  int i = 0;
  int j = 0;
  ifstream fin;
  float temp = 0;
  char fname[128];
  char psname[128];
  TGraph *g = new TGraph();
  TF1 *f1 = new TF1("f1","[0]*x*x*x+[1]*x*x+[2]*x+[3]");
  f1->SetParameters(0.000001,-0.0001,0.1,-5);

  sprintf(fname,"textdata/calibration%d_func.dat",Ncalib);
  ofstream fout(fname);
  for(i = 0;i < 32;i++){
    g->Set(0);
    sprintf(fname,"textdata/calibration%d_ch%d.dat",Ncalib,i + 1);
    fin.open(fname);
    if(!fin){
      cerr << "Error: " << fname << " does not exist." << endl;
      return;
    }
    for(j = 0;j < 13;j++){
      fin >> temp;
      g->SetPoint(j,temp,Q[i % 4] * pow(10,-att[j] / 20));
      //cout << i << " " << j << " " << temp << " " << Q[i % 4] * pow(10,-att[j] / 20) << endl;
    }
    fin.close();
    g->Draw("apl*");
    g->Fit("f1");
    c->Update();
    if(i == 0)sprintf(psname,"textdata/calibration%d.ps(",Ncalib);
    else if(i == 31)sprintf(psname,"textdata/calibration%d.ps)",Ncalib);
    else sprintf(psname,"textdata/calibration%d.ps",Ncalib);
    c->Print(psname);
    //getchar();
    fout << f1->GetParameter(0) << " " << f1->GetParameter(1) << " " << f1->GetParameter(2) << " " << f1->GetParameter(3) << endl;
  }
  fout.close();
  return;
}

bool LifetimeAna::GetCalibParameters(int Ncalib){
  char fname[128];
  sprintf(fname,"textdata/calibration%d_func.dat",Ncalib);
  ifstream fin(fname);
  int i = 0;
  int j = 0;
  
  /* for 16ch amp calibration*/
  char fname2[128];
  sprintf(fname2,"textdata/calibration_16champ.dat");
  ifstream fin2(fname2);
  if(i == 0){
    if(!fin2){
      cerr << "Error: " << fname2 << " does not exist." << endl;
      return false;
    }
    for(j = 0;j<16;j++ ){
      fin2 >> CalibPar2[j][0] >> CalibPar2[j][1] >> CalibPar2[j][2] >> CalibPar2[j][3] >> CalibPar2[j][4] >> CalibPar2[j][5] >> CalibPar2[j][6] >> CalibPar2[j][7];
    } 
  }
  if(!fin){
    cerr << "Error: " << fname << " does not exist." << endl;
    return false;
  }
  while(fin >> CalibPar[i][0] >> CalibPar[i][1] >> CalibPar[i][2] >> CalibPar[i][3])i++;
  
  return true;
}

float LifetimeAna::ADCCalibration(int adc,int ch){
  /* for 16ch amp calibration*/
  float cadc = 0;
  if(ch<16){
    cadc = ((adc - Pedestal[ch]) * (adc - Pedestal[ch]) * (adc - Pedestal[ch]) * (adc - Pedestal[ch]) * (adc - Pedestal[ch]) * (adc - Pedestal[ch]) * (adc - Pedestal[ch]) * CalibPar2[ch][0] +  (adc - Pedestal[ch]) * (adc - Pedestal[ch]) * (adc - Pedestal[ch]) * (adc - Pedestal[ch]) * (adc - Pedestal[ch]) * (adc - Pedestal[ch]) * CalibPar2[ch][1] +  (adc - Pedestal[ch]) * (adc - Pedestal[ch]) * (adc - Pedestal[ch]) * (adc - Pedestal[ch]) * (adc - Pedestal[ch]) * CalibPar2[ch][2] +  (adc - Pedestal[ch]) * (adc - Pedestal[ch]) * (adc - Pedestal[ch]) * (adc - Pedestal[ch]) * CalibPar2[ch][3] + (adc - Pedestal[ch]) * (adc - Pedestal[ch]) * (adc - Pedestal[ch]) * CalibPar2[ch][4] +  (adc - Pedestal[ch]) * (adc - Pedestal[ch]) * CalibPar2[ch][5] +  (adc - Pedestal[ch]) * CalibPar2[ch][6] + CalibPar2[ch][7]) * pow(10,12) * 2 * 4;
  }else if(ch == 20 || ch == 24 || ch == 30){    
    cadc = (adc * adc * adc * CalibPar[ch][0] + adc * adc * CalibPar[ch][1] + adc * CalibPar[ch][2] + CalibPar[ch][3]) * 3 / 2;
  }else{
    cadc = adc * adc * adc * CalibPar[ch][0] + adc * adc * CalibPar[ch][1] + adc * CalibPar[ch][2] + CalibPar[ch][3];
  }
  /*if(ch<16)float cadc = (adc -ped_old[ch]) * (adc -ped_old[ch]) * (adc -ped_old[ch]) * CalibPar[ch][0] + (adc -ped_old[ch]) * (adc -ped_old[ch]) * CalibPar[ch][1] + (adc -ped_old[ch]) * CalibPar[ch][2] + CalibPar[ch][3];
    else float cadc = adc * adc * adc * CalibPar[ch][0] + adc * adc * CalibPar[ch][1] + adc * CalibPar[ch][2] + CalibPar[ch][3];
   */
  if(DEBUG){
    if(ch == 15){
    cout << "16ch amp Parameter" << endl;    
    cout << CalibPar2[ch][0] << " " << CalibPar2[ch][1] << " " << CalibPar2[ch][2] << " " << CalibPar2[ch][3] << CalibPar2[ch][4] << " " << CalibPar2[ch][5] << " " << CalibPar2[ch][6] << " " << CalibPar2[ch][7] << endl;
      cout << ch << " " << adc << "->" << cadc << endl;
      }
    if(ch == 16){
      cout << CalibPar[ch][0] << " " << CalibPar[ch][1] << " " << CalibPar[ch][2] << " " << CalibPar[ch][3] << endl;
      cout << ch << " " << adc << "->" << cadc << endl;
    }
  }
  return cadc;
}

void LifetimeAna::SaveLaserData(int Nrun){
  int i = 0;
  char fname[128];
  sprintf(fname,"/home/takt/lifetime/dataset/%s/converted/ascii/lifetime%d.dat",DirName,Nrun);
  ofstream fout(fname);
  for(i = 0;i < 32;i++){
    //Nhit gain gain_err//
    if(h_cadc[i]->GetEntries() != 0)fout << h_cadc[i]->GetEntries() - NoiseCorr[i] << " " << h_cadc[i]->GetMean() / 0.16 << " " <<h_cadc[i]->GetRMS() / 0.16 / sqrt(h_cadc[i]->GetEntries()) << endl;
    else fout << "0 0 0" << endl;
  }
  fout << h_tdc[32]->GetEntries() - NoiseCorr[32] << " 0 0" << endl;
  fout << LaserTime << endl;
  return;
}

void LifetimeAna::EraseTempdata(){
  system("rm -f tempdata.dat");
  return;
}

void LifetimeAna::GetCharge(TTree *tree_mcp,TTree *tree_config,int Nfile){
  int i = 0;
  int j = 0;
  char hname[128];
  ofstream fout52("tempdata.dat",ios::out | ios::app);
  
  tree_config->GetEntry(0);

  LEDsum.temp = GetConfig.temp;
  LEDsum.humid = GetConfig.humid;
  LEDsum.LEDrate = GetConfig.LEDrate;
  LEDsum.file = Nfile;

  fout52 << GetConfig.temp << " " << GetConfig.humid << " " << GetConfig.LEDrate << " " << Nfile << " ";
  // TCanvas *c = new TCanvas("c1","c1");
  TH1I *hist[32];
  TH1I *hist2[32];//add by muroyama to gate Ledsum.adc2 (adc2 data)
  for(i = 0;i < 32;i++){
    sprintf(hname,"hist%d",i + 1);
    hist[i] = new TH1I(hname,hname,1000,0,4000);
    sprintf(hname,"hist_2%d",i + 1);
    hist2[i] = new TH1I(hname,hname,1000,0,4000);
  }

  for(i = 0;i < tree_mcp->GetEntries();i++){
    tree_mcp->GetEntry(i);
    if(i == 0){
      StartTime = GetLaser.time;
      LEDsum.time = StartTime;
    }
    for(j = 0;j < 32;j++){
      hist[j]->Fill(GetLaser.adc[j]);
      hist2[j]->Fill(GetLaser.adc2[j]);
    }  
  }
  //  hist[16]->Draw();
  //c->Update();
  //getchar();
  fout52 << StartTime << " ";
  if(PrevTime == 0)PrevTime = StartTime;
  for(j = 0;j < 32;j++){
    LEDsum.adc[j] = hist[j]->GetMean();
    LEDsum.adc2[j] = hist2[j]->GetMean();
    if(j<16){

    /*~mask PMT~*/  
 //if(j == 12 || j == 13 || j == 14 || j == 15)LEDsum.Charge[j] = (hist2[j]->GetMean() - Pedestal2[j]) / AmpGain[j] * GetConfig.LEDrate / (0.5625 * 2.25 / 2) / 1000000 / 4;//LEDsum.Charge is calculated on LEDsum.adc2 (new set up)
    /*~~~~~~~~~~*/
    //else LEDsum.Charge[j] = (hist2[j]->GetMean() - Pedestal2[j]) / AmpGain[j] * GetConfig.LEDrate / (0.5625 * 2.25) / 1000000 / 4;//LEDsum.Charge is calculated on LEDsum.adc2 (new set up)

      LEDsum.Charge[j] = (hist2[j]->GetMean() - Pedestal2[j]) / AmpGain[j] * GetConfig.LEDrate / (0.5625 * 2.25) / 1000000 / 4;//LEDsum.Charge is calculated on LEDsum.adc2 (new set up)
    }
    else if(j == 20||j == 24 || j == 30) LEDsum.Charge[j] = (hist2[j]->GetMean() - Pedestal2[j]) / AmpGain[j] * GetConfig.LEDrate / (0.5625 * 2.25) / 1000000 *3 / 2;//LEDsum.Charge is calculated on LEDsum.adc2
    /*~mask PMT~*/  
    //else if(j == 20) LEDsum.Charge[j] = (hist2[j]->GetMean() - Pedestal2[j]) / AmpGain[j] * GetConfig.LEDrate / (0.5625 * 2.25) / 1000000 *3 / 2;//LEDsum.Charge is calculated on LEDsum.adc2    
//else if(j == 24 || j == 30) LEDsum.Charge[j] = (hist2[j]->GetMean() - Pedestal2[j]) / AmpGain[j] * GetConfig.LEDrate / (0.5625 * 2.25 / 2) / 1000000 *3 / 2;//LEDsum.Charge is calculated on LEDsum.adc2
    //else if(j == 25 || j == 26 || j == 27 || j == 28 || j == 29 || j == 31)LEDsum.Charge[j] = (hist2[j]->GetMean() - Pedestal2[j]) / AmpGain[j] * GetConfig.LEDrate / (0.5625 * 2.25 / 2) / 1000000;//LEDsum.Charge is calculated on LEDsum.adc2
    /*~~~~~~~~~~*/
    else LEDsum.Charge[j] = (hist2[j]->GetMean() - Pedestal2[j]) / AmpGain[j] * GetConfig.LEDrate / (0.5625 * 2.25) / 1000000;//LEDsum.Charge is calculated on LEDsum.adc2
    fout52 << LEDsum.Charge[j] << " ";
    if(Nfile > 0)LEDsum.TotalCharge[j] += LEDsum.Charge[j] * (StartTime - PrevTime);
  }

  for(j = 0;j < 8;j++){
    LEDsum.Charge4[j] = (LEDsum.Charge[4 * j] + LEDsum.Charge[4 * j + 1] + LEDsum.Charge[4 * j + 2] + LEDsum.Charge[4 * j + 3]) / 4;
    LEDsum.TotalCharge4[j] += LEDsum.Charge4[j] * (StartTime - PrevTime);
    fout52 << LEDsum.Charge4[j] << " ";
    cout << LEDsum.Charge4[j] << " ";
  }
  cout << endl;
  //cout << Nfile << endl;
  fout52 << endl;
  fout52.close();
  //tree_new->Fill();
  PrevTime = StartTime;
  return;
}

void LifetimeAna::SaveLEDsumData(TTree *tree_config){
  int i = 0;
  for(i = 0;i < 32;i++){
    LEDsum.Amp[i] = AmpGain[i];
  }
  tree_config->Fill();
  return;
}

void LifetimeAna::ReadData(TTree *tree_mcp){
  ifstream fin("tempdata.dat");
  int i = 0;
  if(!fin)cout << "Error" << endl;
  while(fin){
    fin >> LEDsum.temp;
    //cout << LEDsum.temp << endl;
    fin >> LEDsum.humid;
    fin >> LEDsum.LEDrate;
    fin >> LEDsum.file;
    fin >> LEDsum.time;
    for(i = 0;i < 32;i++){
      fin >> LEDsum.Charge[i];
      //cout << LEDsum.Charge[i] << endl;
    }
    for(i = 0;i < 8;i++){
      fin >> LEDsum.Charge4[i];
      //cout << LEDsum.Charge4[i] << endl;
    }
    //cout << LEDsum.Charge4[3] << endl;
    if(!fin.eof())tree_mcp->Fill();
  }
  system("rm -f tempdata.dat");
  return;
}
