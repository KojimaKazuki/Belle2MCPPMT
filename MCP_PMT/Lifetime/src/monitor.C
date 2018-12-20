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
  char fileName[128];
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

  ifstream fileIn;
  fileIn.open("../../directory.txt");
  fileIn >> dir;
  fileIn.close();

  fileIn.open("../config.txt");
  for(i = 0;i < 325;i++)fileIn >> buf;
  for(i = 0;i < 32;i++){
    fileIn >> buf >> amp[i];
    //cout << amp[i] << endl;
  }
  fileIn.close();
  //return;
  sprintf(fileName,"/home/takt/lifetime/dataset/%s/data/led/runnumber",dir);
  fileIn.open(fileName);
  fileIn >> Nrun;
  fileIn.close();

  sprintf(fileName,"/home/takt/lifetime/dataset/%s/data/led/filenumber",dir);
  fileIn.open(fileName);
  fileIn >> Nfile;
  fileIn.close();

  fileIn.open("runnumber");
  fileIn >> temp_Nrun;
  fileIn.close();

  if(temp_Nrun != Nrun){
    system("rm -f temp.dat");
    system("rm -f pedestal.dat");
  }

  fileIn.open("pedestal.dat");
  if(fileIn)for(i = 0;i < 32;i++)fileIn >> pedestal[i];
  else{
    sprintf(fileName,"/home/takt/lifetime/dataset/%s/data/laser/lifetime_laser%d.root",dir,Nrun);
    TFile *fileLaser = new TFile(fileName);
    TTree *treeLaser = (TTree*)fileLaser->Get("mcp");
    //treeLaser->SetBranchAddress("adc",get_adc);
    treeLaser->SetBranchAddress("adc2",get_adc);//change by muroyama 2017 Oct from life 16
    treeLaser->SetBranchAddress("tdc",get_tdc);
    for(k = 0;k < 10000;k++){
      treeLaser->GetEntry(k);
      for(j = 0;j < 32;j++)if(get_tdc[j] > 4095)hist[j]->Fill(get_adc[j]);
    }
    fileLaser->Close();
    ofstream fileOut("pedestal.dat");
    for(j = 0;j < 32;j++){
      fileOut << hist[j]->GetMean() << endl;
      pedestal[i] =  hist[j]->GetMean();
      hist[j]->Reset();
    }
  }
  fileIn.close();

  ofstream fileOut("runnumber");
  fileOut << Nrun << endl;
  fileOut.close();

  //cout << dir << " " << Nrun << " " << Nfile << endl;

  TGraph *graph1[15];
  for(i = 0;i < 15;i++)graph1[i] = new TGraph();
  int volume = 0;

  i = 0;
  fileIn.open("temp.dat");
  while(fileIn >> get_time >> get_daq >> get_led >> get_rPMT >> get_temp >> get_humid >> charge[0] >> charge[1] >> charge[2] >> charge[3] >> charge[4] >> charge[5] >> charge[6] >> charge[7] >> volume){
    if(i == 0)init_time = get_time;
    //cout << get_time << " " << init_time << endl;
    for(j = 0;j < 8;j++){
      
      if(max_charge < charge[j])max_charge = charge[j];
      graph1[j]->SetPoint(i,(float)(get_time - init_time) / 3600,charge[j]);

    }
    graph1[8]->SetPoint(i,(float)(get_time - init_time) / 3600,get_daq);
    graph1[9]->SetPoint(i,(float)(get_time - init_time) / 3600,get_led);
    graph1[10]->SetPoint(i,(float)(get_time - init_time) / 3600,get_rPMT);
    graph1[11]->SetPoint(i,(float)(get_time - init_time) / 3600,get_temp);
    graph1[12]->SetPoint(i,(float)(get_time - init_time) / 3600,get_humid);
    graph1[13]->SetPoint(i,(float)(get_time - init_time) / 3600,volume);
    //cout << "from temp.dat; " << i << " "  << get_time << " " << get_led << " " << charge[0] << endl;
    i++;
  }
  N = i;
  fileIn.close();

  ofstream fileOut1("temp.dat",ios::out | ios::app);
  sprintf(fileNameOutput, "%sLEDRunData.dat",dir)
  ofstream fileOut2( fileNameOutput,ios::out | ios::app);
  fileOut2 << "# Lifetime;" << " " << "Run No.;" << " " << "Unix Time;" << " " << "DAQ Rate;" << " " << "LED Rate;" << " " << "ref.PMT Rate" << " " << "Temperature;" << " " << "Humidity;";
  for(j = 0;j < 8;j++)fileOut2 << " " << "PMT" << j << " Charge;";
  fileOut2 << " "<< endl;


  string volume_temp = "";
  system("df | grep -A 1 'mapper' | tail -1 > .vol");
  ifstream fileIn_vol(".vol");
  fileIn_vol >> buf >> buf >> buf >> volume_temp;
  //cout << volume_temp << endl;
  volume = atof(volume_temp.substr(0,2).c_str());
  /*
  system("free | tail -1 > .mem");  
  ifstream fileIn_mem(".mem");
  fileIn_vol >> buf >> buf >> mem_temp;
  memory = atof(volume_temp.substr(0,2).c_str());
  */

  for(i = N;i < Nfile;i++){
    sprintf(fileName,"/home/takt/lifetime/dataset/%s/data/led/lifetime_LED%d_file%d.root",dir,Nrun,i);
    TFile *file1 = new TFile(fileName);
    for(j = 0;j < 8;j++)hist[j]->Reset();
    if(!file1->IsZombie()){
      TTree *tree2 = (TTree*)file1->Get("config");
      tree2->SetBranchAddress("DAQrate",&get_daq);
      tree2->SetBranchAddress("LEDrate",&get_led);
      tree2->SetBranchAddress("rPMTrate",&get_rPMT);
      tree2->SetBranchAddress("temp",&get_temp);
      tree2->SetBranchAddress("humid",&get_humid);
      tree2->GetEntry(0);
      get_led = get_led / 1000;
      get_daq = get_daq / 1000;
      get_rPMT = get_rPMT / 1000;
      
      TTree *tree1 = (TTree*)file1->Get("mcp");
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
	graph1[j]->SetPoint(i,(float)(get_time - init_time) / 3600,charge[j]);
      }
      garph1[8]->SetPoint(i,(float)(get_time - init_time) / 3600,get_daq);
      graph1[9]->SetPoint(i,(float)(get_time - init_time) / 3600,get_led);
      graph1[10]->SetPoint(i,(float)(get_time - init_time) / 3600,get_rPMT);
      graph1[11]->SetPoint(i,(float)(get_time - init_time) / 3600,get_temp);
      graph1[12]->SetPoint(i,(float)(get_time - init_time) / 3600,get_humid);
      graph1[13]->SetPoint(i,(float)(get_time - init_time) / 3600,volume);
      fileOut1 << get_time << " " << get_daq << " " << get_led << " " << get_rPMT << " " << get_temp << " " << get_humid;
      for(j = 0;j < 8;j++)fileOut1 << " " << charge[j];
      fileOut1 << " " << volume << endl;
      //cout << "from root file; " << i << " " << get_time << " " << charge[5] << " " << get_led << " " << volume << "%" << endl; 
      fileOut2 << dir << " " << Nrun << " " << get_time << " " << get_daq << " " << get_led << " " << get_rPMT << " " << get_temp << " " << get_humid;
      for(j = 0;j < 8;j++)fileOut2 << " " << charge[j];
      fileOut2 << " " << endl;
    }
    file1->Close();
  }

  fileOut1.close();
  fileOut2.close();

  char title[64];
  double x, y;

  TText *alert1[14];

  TCanvas *c1 = new TCanvas("c1","c1",1000,800);
  c1->Divide(2,3);
  c1->cd(1);
  //gPad->DrawFrame(0,-0.0001,(float)(get_time - init_time) / 3600 + 0.0001,max_charge * 1.1,";time (hours);current (#mu A/cm^{2})");
  TLatex *tex1[8];
  for(j = 0;j < 4;j++){
    graph1[j]->SetLineColor(j + 1);
    graph1[j]->SetMarkerColor(j + 1);
    graph1[j]->Draw("pl");
    graph1[j]->GetPoint(graph1[j]->GetN() - 1,x,y);
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
    graph1[j]->SetLineColor(j - 3);
    graph1[j]->SetMarkerColor(j - 3);
    graph1[j]->Draw("pl");
    graph1[j]->GetPoint(graph1[j]->GetN() - 1,x,y);
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
    graph1[j]->SetLineColor(j - 7);
    graph1[j]->SetMarkerColor(j - 7);
    graph1[j]->Draw("pl");
    graph1[j]->GetPoint(graph1[j]->GetN() - 1,x,y);
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
    graph1[j]->SetLineColor(j - 10);
    graph1[j]->SetMarkerColor(j - 10);
    graph1[j]->Draw("pl");
    graph1[j]->GetPoint(graph1[j]->GetN() - 1,x,y);
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
  //graph1[12]->SetLineColor(j - 10);
  //graph1[12]->SetMarkerColor(j - 10);
  graph1[13]->Draw("pl");
  graph1[13]->GetPoint(graph1[12]->GetN() - 1,x,y);
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
