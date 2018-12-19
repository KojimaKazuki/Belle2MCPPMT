int monitor(){

  int i = 0;
  int j = 0;
  int k = 0;
  int N = 0;
  int Nrun = 0;
  int Nfile = 0;
  int temp_Nrun = 0;
  char dir[64];
  char buf[64];
  char fname[128];
  char fileNameOutput[128];
  char hname[128];
  int init_time = 0;
  float charge[8];
  float amp[32];
  float max_charge = 0;

  unsigned short int get_adc[32];
  unsigned short int get_tdc[32];
  int get_time = 0;
  float get_daq = 0;
  float get_led = 0;
  float get_temp = 0;
  float get_humid = 0;
  float get_rPMT = 0;

  float pedestal[32];

  gStyle->SetLabelSize(0.06,"XY");
  gStyle->SetTitleXSize(0.06);
  gStyle->SetTitleYSize(0.06);
  gStyle->SetPadLeftMargin(0.14);
  gStyle->SetPadBottomMargin(0.14);

  TH1I *hist[32];
  for(i = 0;i < 32;i++){
    sprintf(hname,"hist%d",i + 1);
    hist[i] = new TH1I(hname,hname,400,0,4000);
  }

  ifstream fin;
  fin.open("../../directory.txt");
  fin >> dir;
  fin.close();

  fin.open("../config.txt");
  for(i = 0;i < 325;i++)fin >> buf;
  for(i = 0;i < 32;i++){
    fin >> buf >> amp[i];
    //cout << amp[i] << endl;
  }
  fin.close();
  //return;
  sprintf(fname,"/home/takt/lifetime/dataset/%s/data/led/runnumber",dir);
  fin.open(fname);
  fin >> Nrun;
  fin.close();

  sprintf(fname,"/home/takt/lifetime/dataset/%s/data/led/filenumber",dir);
  fin.open(fname);
  fin >> Nfile;
  fin.close();

  fin.open("runnumber");
  fin >> temp_Nrun;
  fin.close();

  if(temp_Nrun != Nrun){
    system("rm -f temp.dat");
    system("rm -f pedestal.dat");
  }

  fin.open("pedestal.dat");
  if(fin)for(i = 0;i < 32;i++)fin >> pedestal[i];
  else{
    sprintf(fname,"/home/takt/lifetime/dataset/%s/data/laser/lifetime_laser%d.root",dir,Nrun);
    TFile *f_laser = new TFile(fname);
    TTree *tree_laser = (TTree*)f_laser->Get("mcp");
    //tree_laser->SetBranchAddress("adc",get_adc);
    tree_laser->SetBranchAddress("adc2",get_adc);//change by muroyama 2017 Oct from life 16
    tree_laser->SetBranchAddress("tdc",get_tdc);
    for(k = 0;k < 10000;k++){
      tree_laser->GetEntry(k);
      for(j = 0;j < 32;j++)if(get_tdc[j] > 4095)hist[j]->Fill(get_adc[j]);
    }
    f_laser->Close();
    ofstream fout("pedestal.dat");
    for(j = 0;j < 32;j++){
      fout << hist[j]->GetMean() << endl;
      pedestal[i] =  hist[j]->GetMean();
      hist[j]->Reset();
    }
  }
  fin.close();

  ofstream fout("runnumber");
  fout << Nrun << endl;
  fout.close();

  //cout << dir << " " << Nrun << " " << Nfile << endl;

  TGraph *g[15];
  for(i = 0;i < 15;i++)g[i] = new TGraph();
  int volume = 0;

  i = 0;
  fin.open("temp.dat");
  while(fin >> get_time >> get_daq >> get_led >> get_rPMT >> get_temp >> get_humid >> charge[0] >> charge[1] >> charge[2] >> charge[3] >> charge[4] >> charge[5] >> charge[6] >> charge[7] >> volume){
    if(i == 0)init_time = get_time;
    //cout << get_time << " " << init_time << endl;
    for(j = 0;j < 8;j++){
      
      if(max_charge < charge[j])max_charge = charge[j];
      g[j]->SetPoint(i,(float)(get_time - init_time) / 3600,charge[j]);

    }
    g[8]->SetPoint(i,(float)(get_time - init_time) / 3600,get_daq);
    g[9]->SetPoint(i,(float)(get_time - init_time) / 3600,get_led);
    g[10]->SetPoint(i,(float)(get_time - init_time) / 3600,get_rPMT);
    g[11]->SetPoint(i,(float)(get_time - init_time) / 3600,get_temp);
    g[12]->SetPoint(i,(float)(get_time - init_time) / 3600,get_humid);
    g[13]->SetPoint(i,(float)(get_time - init_time) / 3600,volume);
    //cout << "from temp.dat; " << i << " "  << get_time << " " << get_led << " " << charge[0] << endl;
    i++;
  }
  N = i;
  fin.close();

  ofstream fout("temp.dat",ios::out | ios::app);
  sprintf(fileNameOutput, "%sLEDRunData.dat",dir)
  ofstream fileOut2( fileNameOutput,ios::out | ios::app);
  fileOut2 << "# Lifetime;" << " " << "Run No.;" << " " << "Unix Time;" << " " << "DAQ Rate;" << " " << "LED Rate;" << " " << "ref.PMT Rate" << " " << "Temperature;" << " " << "Humidity;";
  for(j = 0;j < 8;j++)fileOut2 << " " << "PMT" << j << " Charge;";
  fileOut2 << " "<< endl;


  string volume_temp = "";
  system("df | grep -A 1 'mapper' | tail -1 > .vol");
  ifstream fin_vol(".vol");
  fin_vol >> buf >> buf >> buf >> volume_temp;
  //cout << volume_temp << endl;
  volume = atof(volume_temp.substr(0,2).c_str());
  /*
  system("free | tail -1 > .mem");  
  ifstream fin_mem(".mem");
  fin_vol >> buf >> buf >> mem_temp;
  memory = atof(volume_temp.substr(0,2).c_str());
  */

  for(i = N;i < Nfile;i++){
    sprintf(fname,"/home/takt/lifetime/dataset/%s/data/led/lifetime_LED%d_file%d.root",dir,Nrun,i);
    TFile *f = new TFile(fname);
    for(j = 0;j < 8;j++)hist[j]->Reset();
    if(!f->IsZombie()){
      TTree *tree2 = (TTree*)f->Get("config");
      tree2->SetBranchAddress("DAQrate",&get_daq);
      tree2->SetBranchAddress("LEDrate",&get_led);
      tree2->SetBranchAddress("rPMTrate",&get_rPMT);
      tree2->SetBranchAddress("temp",&get_temp);
      tree2->SetBranchAddress("humid",&get_humid);
      tree2->GetEntry(0);
      get_led = get_led / 1000;
      get_daq = get_daq / 1000;
      get_rPMT = get_rPMT / 1000;
      
      TTree *tree1 = (TTree*)f->Get("mcp");
      tree1->SetBranchAddress("time",&get_time);
      //tree1->SetBranchAddress("adc",get_adc);
      tree1->SetBranchAddress("adc2",get_adc);//change by muroyama 2017 Oct from life 16
      for(j = 0;j < tree1->GetEntries();j++){
	tree1->GetEntry(j);
	for(k = 0;k < 32;k++){
	  hist[k]->Fill(get_adc[k]);
	}
      }
      if(i == 0)init_time = get_time;
      for(k = 0;k < 8;k++){
	if(k<4){
	  charge[k] = ((hist[4 * k]->GetMean() - pedestal[4 * k]) / amp[4 * k] + (hist[4 * k + 1]->GetMean() - pedestal[4 * k + 1])  / amp[4 * k + 1] + (hist[4 * k + 2]->GetMean() - pedestal[4 * k + 2]) / amp[4 * k + 2] + (hist[4 * k + 3]->GetMean() - pedestal[4 * k + 3]) / amp[4 * k + 3]);
	}else if (k==5||k==6){
	  charge[k] = ((hist[4 * k]->GetMean() - pedestal[4 * k]) / amp[4 * k]* 3/2 + (hist[4 * k + 1]->GetMean() - pedestal[4 * k + 1])  / amp[4 * k + 1] + (hist[4 * k + 2]->GetMean() - pedestal[4 * k + 2]) / amp[4 * k + 2] + (hist[4 * k + 3]->GetMean() - pedestal[4 * k + 3]) / amp[4 * k + 3]);
	}else if(k==7){
	  charge[k] = ((hist[4 * k]->GetMean() - pedestal[4 * k]) / amp[4 * k] + (hist[4 * k + 1]->GetMean() - pedestal[4 * k + 1])  / amp[4 * k + 1] + (hist[4 * k + 2]->GetMean() - pedestal[4 * k + 2]) / amp[4 * k + 2]*3/2 + (hist[4 * k + 3]->GetMean() - pedestal[4 * k + 3]) / amp[4 * k + 3]);
	}else{
	  charge[k] = ((hist[4 * k]->GetMean() - pedestal[4 * k]) / amp[4 * k] + (hist[4 * k + 1]->GetMean() - pedestal[4 * k + 1])  / amp[4 * k + 1] + (hist[4 * k + 2]->GetMean() - pedestal[4 * k + 2]) / amp[4 * k + 2] + (hist[4 * k + 3]->GetMean() - pedestal[4 * k + 3]) / amp[4 * k + 3]);
	}
	 
	//cout << pedestal[k] << endl;// " " << charge[k] << " " << amp[k] << " ";
	charge[k] = charge[k] / 4 / (0.5625 * 2.25) / 1000000 * get_led * 1000;
	//cout << charge[k] << " " << get_led << endl;
      }

      
      for(j = 0;j < 8;j++){
	if(max_charge < charge[j])max_charge = charge[j];
	g[j]->SetPoint(i,(float)(get_time - init_time) / 3600,charge[j]);
      }
      g[8]->SetPoint(i,(float)(get_time - init_time) / 3600,get_daq);
      g[9]->SetPoint(i,(float)(get_time - init_time) / 3600,get_led);
      g[10]->SetPoint(i,(float)(get_time - init_time) / 3600,get_rPMT);
      g[11]->SetPoint(i,(float)(get_time - init_time) / 3600,get_temp);
      g[12]->SetPoint(i,(float)(get_time - init_time) / 3600,get_humid);
      g[13]->SetPoint(i,(float)(get_time - init_time) / 3600,volume);
      fout << get_time << " " << get_daq << " " << get_led << " " << get_rPMT << " " << get_temp << " " << get_humid;
      for(j = 0;j < 8;j++)fout << " " << charge[j];
      fout << " " << volume << endl;
      //cout << "from root file; " << i << " " << get_time << " " << charge[5] << " " << get_led << " " << volume << "%" << endl; 
      fileOut2 << dir << " " << Nrun << " " << get_time << " " << get_daq << " " << get_led << " " << get_rPMT << " " << get_temp << " " << get_humid;
      for(j = 0;j < 8;j++)fileOut2 << " " << charge[j];
      fileOut2 << " " << endl;
    }
    f->Close();
  }

  fout.close();

  char title[64];
  double x, y;

  TText *alert1[14];

  TCanvas *c1 = new TCanvas("c1","c1",1000,800);
  c1->Divide(2,3);
  c1->cd(1);
  //gPad->DrawFrame(0,-0.0001,(float)(get_time - init_time) / 3600 + 0.0001,max_charge * 1.1,";time (hours);current (#mu A/cm^{2})");
  TLatex *tex1[8];
  for(j = 0;j < 4;j++){
    g[j]->SetLineColor(j + 1);
    g[j]->SetMarkerColor(j + 1);
    g[j]->Draw("pl");
    g[j]->GetPoint(g[j]->GetN() - 1,x,y);
    sprintf(title,"PMT%d=%6.4f #mu A/cm^{2}",j + 1,y);
    tex1[j] = new TLatex(0.3,0.5 - j * 0.05,title);
    tex1[j]->SetNDC();
    tex1[j]->SetTextColor(j + 1);
    tex1[j]->SetTextSize(0.06);
    tex1[j]->Draw();
    alert1[j] = new TText(0.6,0.5 - j * 0.05,"No current!");
    alert1[j]->SetTextColor(2);
    alert1[j]->SetNDC();
    //if(y < 0.0001)alert1[j]->Draw();
  }
  gPad->SetGridx();
  gPad->SetGridy();
  

  c1->cd(2);
  gPad->DrawFrame(0,0,(float)(get_time - init_time) / 3600,max_charge * 1.1,";time (hours);current (#mu A/cm^{2})");
  for(j = 4;j < 8;j++){
    g[j]->SetLineColor(j - 3);
    g[j]->SetMarkerColor(j - 3);
    g[j]->Draw("pl");
    g[j]->GetPoint(g[j]->GetN() - 1,x,y);
    sprintf(title,"PMT%d=%6.4f #mu A/cm^{2}",j + 1,y);
    tex1[j] = new TLatex(0.3,0.5 - (j - 4) * 0.05,title);
    tex1[j]->SetNDC();
    tex1[j]->SetTextColor(j - 3);
    tex1[j]->SetTextSize(0.06);
    tex1[j]->Draw();
    alert1[j] = new TText(0.6,0.5 - (j - 4) * 0.05,"No current!");
    alert1[j]->SetTextColor(2);
    alert1[j]->SetNDC();
    if(y < 0.0001)alert1[j]->Draw();  
  }

  gPad->SetGridx();
  gPad->SetGridy();

  c1->cd(3);
  gPad->DrawFrame(0,0,(float)(get_time - init_time) / 3600,get_led * 1.1,";time (hours);rate (kHz)");
  TLatex *ratemon[3];
  char alert_cont[64];
  for(j = 8;j < 11;j++){
    g[j]->SetLineColor(j - 7);
    g[j]->SetMarkerColor(j - 7);
    g[j]->Draw("pl");
    g[j]->GetPoint(g[j]->GetN() - 1,x,y);
    if(j == 8){
      sprintf(title,"DAQ rate =%9.2f kHz",y);
    }
    if(j == 9){
      sprintf(title,"LED rate =%9.2f kHz",y);
    }
    if(j == 10){
      sprintf(title,"rPMT rate=%9.2f kHz",y);
    }
    ratemon[j - 8] = new TLatex(0.3,0.5 - (j - 8) * 0.05,title);
    ratemon[j - 8]->SetNDC();
    ratemon[j - 8]->SetTextColor(j - 7);
    ratemon[j - 8]->SetTextSize(0.06);
    ratemon[j - 8]->Draw();
  }
  
  gPad->SetGridx();
  gPad->SetGridy();
  
  c1->cd(4);
  gPad->DrawFrame(0,0,(float)(get_time - init_time) / 3600,100,";time (hours);T (#circC) / Humid. (%)");
  TLatex *tempmon[2];
  for(j = 11;j < 13;j++){
    g[j]->SetLineColor(j - 10);
    g[j]->SetMarkerColor(j - 10);
    g[j]->Draw("pl");
    g[j]->GetPoint(g[j]->GetN() - 1,x,y);
    if(j == 11)sprintf(title,"Temp.=%4.1f#circC",y);
    alert1[j] = new TText(0.6,0.5 - (j - 11) * 0.05,"Humidity too high!");
    alert1[j]->SetNDC();
    alert1[j]->SetTextColor(2);
    if(j == 12)sprintf(title,"Humid.=%2.0f%%",y);      
    
    tempmon[j - 11] = new TLatex(0.3,0.5 - (j - 11) * 0.05,title);
    tempmon[j - 11]->SetNDC();
    tempmon[j - 11]->SetTextColor(j - 10);
    tempmon[j - 11]->SetTextSize(0.06);
    tempmon[j - 11]->Draw();
    if(j == 12 && y > 60)alert1[j]->Draw();
  }
  gPad->SetGridx();
  gPad->SetGridy();
  c1->Update();

  c1->cd(5);
  gPad->DrawFrame(0,0,(float)(get_time - init_time) / 3600,100,";time (hours);Used local HDD space (%)");
  TLatex *HDDmon;
  //g[12]->SetLineColor(j - 10);
  //g[12]->SetMarkerColor(j - 10);
  g[13]->Draw("pl");
  g[13]->GetPoint(g[12]->GetN() - 1,x,y);
  sprintf(title,"HDD used = %2.0f%%",y);
  HDDmon = new TLatex(0.3,0.5 - (j - 11) * 0.05,title);
  HDDmon->SetNDC();
  HDDmon->SetTextColor(2);
  HDDmon->SetTextSize(0.06);
  HDDmon->Draw();

  TLine *line_v = new TLine(0,80,(float)(get_time - init_time) / 3600,80);
  line_v->SetLineWidth(3);
  line_v->SetLineColor(2);
  line_v->Draw();

  gPad->SetGridx();
  gPad->SetGridy();
  c1->Update();


  cout << "ENTER to stop LED run" << endl;
  for(i = 0;i < 12;i++){
    c1->Update();
//    gSystem->Sleep(10000);
    if (system("read -t 10") == 0) {
      cout << "Check the online monitor" << endl;
      cout << "1. All the values are stable? (RETURN to continue)" << flush;
      system("read");
      cout << "2. All the MCP-PMT currents are NOT 0? (RETURN to continue)" << flush;
      system("read");
      cout << "3. DAQ rate is 0.5 kHz? (RETURN to continue)" << flush;
      system("read");
      cout << "4. LED rate is consistent with the pulser setting? (RETURN to continue)" << flush;
      system("read");
      cout << "5. Hit rate of reference PMT (rPMT rate) is 0 kHz? (RETURN to continue)" << flush;
      system("read");
      cout << "6. Humidity is below 60%? (RETURN to continue)" << flush;
      system("read");
      return 1;
    }
  }

  return 0;
}
