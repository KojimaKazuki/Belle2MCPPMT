//this program is to get informatin from config.txt
#include "Lifetime.h"

#define SLOT_OREG 23
#define CH_LED 0x3
#define CH_LASER 0x18


Lifetime::Lifetime(char *config,char *type){

  TDCafslot[0] = 6;  
  TDCafslot[1] = 6;
  TDCafslot[2] = 6;
  TDCafslot[3] = 6;
  TDCafslot[4] = 6;
  TDCafslot[5] = 6;
  TDCafch[0] = 1;
  TDCafch[1] = 2;
  TDCafch[2] = 3;
  TDCafch[3] = 4;
  TDCafch[4] = 5;
  TDCafch[5] = 6;
  
  char buf[256];
  char hname[128];
  string temp;
  stringstream istr;
  int i = 0;
  flag_CAMAC = false;

  gStyle->SetOptStat(0);

  flag_Canvas = false;
  flag_Complete = false;
  
  for(i = 0;i < 33;i++){
    sprintf(hname,"ADC%d",i + 1);
    h_adc[i] = new TH1I(hname,hname,400,0,4000);
    sprintf(hname,"TDC%d",i + 1);
    h_tdc[i] = new TH1I(hname,hname,400,0,4000);
    PrevCount_ADC[i] = 0;
    PrevCount_TDC[i] = 0;
  }
  for(i = 0;i < 6;i++){
    sprintf(hname,"TDC%d_af",i + 1);
    h_tdc_af[i] = new TH1I(hname,hname,400,0,4000);
    PrevCount_TDC_af[i] = 0;
  }

  sprintf(RunType,"%s",type);

  ifstream fin;
  fin.open(config);//open config.txt
  while(fin.getline(buf,256)){
    //get a line in config.txt in this loop
   
    temp = buf;
    if(temp.find("#") == string::npos){
      if(strcmp(buf,"--- PMT settings ---") == 0){
	//when the gotten line is --- PMT settimgs ---, this program do 
	cout << "Read PMT settings." << endl;
	for(i = 0;i < 3;i++)fin >> buf;
	for(i = 0;i < 8;i++){
	  fin >> buf >> PMTname[i] >> buf;
	  if(strcmp(PMTname[i],"none") == 0){
	    HV[i] = 0;
	  }else{
	    HV[i] = atoi(buf);
	  }
	  cout << "PMT No." << i << ": " << PMTname[i] << " (HV=" << HV[i] << "V)" << endl;  
	}
      }
      
      if(strcmp(buf,"--- Event settings ---") == 0){
	cout << "Read Event settings." << endl;
	while(strcmp(buf,type) != 0)fin >> buf;
	fin >> Nevent;
	cout << type << "=" << Nevent << endl;
      }
            
      if(strcmp(buf,"--- CAMAC settings ---") == 0){
	cout << "Read CAMAC settings." << endl;
	fin.getline(buf,256);
	fin.getline(buf,256);
	fin >> buf >> buf >> buf;
	for(i = 0;i < 33;i++)fin >> TDCslot[i];
	fin >> buf >> buf >> buf;
	for(i = 0;i < 33;i++)fin >> TDCch[i];
	fin >> buf >> buf >> buf;
	for(i = 0;i < 33;i++)fin >> ADCslot[i];
	fin >> buf >> buf >> buf;
	for(i = 0;i < 33;i++)fin >> ADCch[i];
	fin >> buf >> buf >> buf;
	for(i = 0;i < 33;i++)fin >> ADC2slot[i];
	fin >> buf >> buf >> buf;
	for(i = 0;i < 33;i++)fin >> ADC2ch[i];
	cout << "ch  : (TDCsl, TDCch) (ADCsl, ADCch) (ADC2sl, ADC2ch)" << endl;
	for(i = 0;i < 32;i++)printf("ch%2d: (%5d, %5d) (%5d, %5d) (%6d, %6d)\n",i + 1,TDCslot[i],TDCch[i],ADCslot[i],ADCch[i],ADC2slot[i],ADC2ch[i]);
	printf("rPMT: (%5d, %5d) (%5d, %5d) (%6d, %6d)\n",TDCslot[i],TDCch[i],ADCslot[i],ADCch[i],ADC2slot[i],ADC2ch[i]);
	
      }

      if(strcmp(buf,"--- Scaler settings ---") == 0){
	cout << "Read Scaler settings." << endl;
	fin >> buf >> buf >> SCALERslot;
	fin >> buf >> buf >> buf >> DAQrate;
	fin >> buf >> buf >> buf >> LEDrate;
	fin >> buf >> buf >> buf >> rPMTrate;
	cout << "Scaler:" << endl;
	cout << "  slot      = " << SCALERslot << endl;
	cout << "  DAQ rate  = " << DAQrate << endl;
	cout << "  LED rate  = " << LEDrate << endl;
	cout << "  rPMT rate = " << rPMTrate << endl;
      }

      if(strcmp(buf,"--- Amp. gain (only use for LED run) ---") == 0){
	cout << "Read Amp. gain settings." << endl;
	for(i = 0;i < 32;i++){
	  fin >> buf >> AmpGain[i];
	  cout << "ch" << i + 1 << ": " << AmpGain[i] << endl;
	}
      }

      if(strcmp(buf,"--- Miscellaneous ---") == 0){
	cout << "Read other settings." << endl;
	fin.getline(buf,256);
	fin >> buf >> StartQE;
	fin >> buf >> buf >> buf;
	for(i = 0;i < 8;i++)fin >> initQE[i];
	for(i = 0;i < 8;i++)fin >> initQE_err[i];
	fin >> buf >> buf >> buf >> TDCregion_MCP[0] >> buf >> TDCregion_MCP[1];//new amp setup TDC window
	fin >> buf >> buf >> buf >> TDCregion_MCP[2] >> buf >> TDCregion_MCP[3];//new amp setup TDC window for after pulse ch
	fin >> buf >> buf >> buf >> TDCregion_MCP[4] >> buf >> TDCregion_MCP[5];//old setup TDC window
	fin >> buf >> buf >> buf >> TDCregion_REF[0] >> buf >> TDCregion_REF[1];
	fin >> buf >> PreCharge[0] >> PreCharge[1] >> PreCharge[2] >> PreCharge[3] >> PreCharge[4] >> PreCharge[5] >> PreCharge[6] >> PreCharge[7];
	cout << "TDC region:" << endl;
	cout << "  MCP(new): " << TDCregion_MCP[0] << " - " << TDCregion_MCP[1] << endl;
	cout << "  MCP(newaf): " << TDCregion_MCP[2] << " - " << TDCregion_MCP[3] << endl;
	cout << "  MCP(old): " << TDCregion_MCP[4] << " - " << TDCregion_MCP[5] << endl;
	cout << "  REF: " << TDCregion_REF[0] << " - " << TDCregion_REF[1] << endl;
	cout << "PreCharge" << PreCharge[5] << endl;
      }
    }
  }
  fin.close();

  fin.open("../../directory.txt");
  fin >> DirName;
  fin.close();

  //Initialize

  for(i = 0;i < 32;i++){
    TreeContent.adc[i] = 0;
    TreeContent.adc2[i] = 0;
    TreeContent.tdc[i] = 0;
  }
  for(i = 0;i < 6;i++){
    TreeContent.tdc_af[i] = 0;
    TreeContent.tdc_main[i] = 0;
    TreeContent.adc_main[i] = 0;
    TreeContent.adc2_main[i] = 0;
  }
  TreeContent.adc_ref = 0;
  TreeContent.tdc_ref = 0;
  TreeContent.time = 0;
  TreeContent.event = 0;

  cout << "Initiallization done." << endl;
  /* for(int i =0;i<6;i++){
    cout << "afterpulse slot and ch" << endl;
    cout << TDCafslot[i] << endl;
    cout << TDCafch[i] << endl;
    } */ 
}

Lifetime::~Lifetime(){
  char fname[128];
  ofstream fout;
  
  if(flag_Complete){
    if(strcmp(RunType,"laser") == 0 && Nrun != -1){
      sprintf(fname,"/home/takt/lifetime/dataset/%s/data/%s/runnumber",DirName,RunType);
      fout.open(fname);
      fout << Nrun + 1 << endl;
      fout.close();
      sprintf(fname,"/home/takt/lifetime/dataset/%s/data/led/runnumber",DirName);
      fout.open(fname);
      fout << Nrun << endl;
      fout.close();
      sprintf(fname,"/home/takt/lifetime/dataset/%s/data/led/filenumber",DirName);
      fout.open(fname);
      fout << 0 << endl;
      fout.close();
    }else if(strcmp(RunType,"pedestal") == 0 && Nrun != -1){
      sprintf(fname,"/home/takt/lifetime/dataset/%s/data/led/runnumber",DirName);
      fout.open(fname);
      fout << Nrun + 1 << endl;
      fout.close();
      sprintf(fname,"/home/takt/lifetime/dataset/%s/data/pedestal/runnumber",DirName);
      fout.open(fname);
      fout << Nrun + 1 << endl;
      fout.close();
    }else if(strcmp(RunType,"led") == 0 && Nfile != -1){
      sprintf(fname,"/home/takt/lifetime/dataset/%s/data/led/filenumber",DirName);
      fout.open(fname);
      fout << Nfile + 1 << endl;
      fout.close();
    }else if(strcmp(RunType,"test") == 0 && Nrun != -1){
      sprintf(fname,"/home/takt/lifetime/dataset/%s/data/test/runnumber",DirName);
      fout.open(fname);
      fout << Nrun + 1 << endl;
      fout.close();
    }else{
      cout << "Warning: the runnumber or the filenumber is not incremented." << endl;
    }
  }else{
    cout << "Warning: the runnumber or the filenumber is not incremented." << endl;
  }
  cout << endl;
  cout << "***********************************************" << endl;
  cout << RootFileName << " has been completed." << endl;
  cout << "***********************************************" << endl << endl;
}

void Lifetime::InitCamac(){
  cam = new Camac(0);
  flag_CAMAC = true;

  cam->Clear(SLOT_OREG);
  if(!strcmp(RunType,"led")) {
    cam->Write(SLOT_OREG,CH_LED);
  } else {
    cam->Write(SLOT_OREG,CH_LASER);
  }
  sleep(1);
}

void Lifetime::EndCamac(){
  delete cam;
  flag_CAMAC = false;
}

bool Lifetime::SetFileName(char *type,char *rootname){
  char fname[128];
  
  sprintf(fname,"/home/takt/lifetime/dataset/%s/data/%s/runnumber",DirName,type);
  ifstream fin(fname);
  if(!fin){
    cerr << "Error: runnumber file for " << type << " run does not exist." << endl;
    cerr << "Run number is set as -1." << endl;
    Nrun = -1;
    return false;
  }else{
    fin >> Nrun;
  }
  fin.close();
  
  if(strcmp(type,"led") == 0){
    sprintf(fname,"/home/takt/lifetime/dataset/%s/data/led/filenumber",DirName);
    fin.open(fname);
    if(!fin){
      cerr << "Error: filenumber file for LED run does not exist." << endl;
      cerr << "File number is set as -1." << endl;
      Nfile = -1;
      return false;
    }else{
      fin >> Nfile;
    }
    fin.close();
  }
  
  if(strcmp(type,"led") == 0)sprintf(rootname,"/home/takt/lifetime/dataset/%s/data/led/lifetime_LED%d_file%d.root",DirName,Nrun,Nfile);
  else sprintf(rootname,"/home/takt/lifetime/dataset/%s/data/%s/lifetime_%s%d.root",DirName,type,type,Nrun);
  cout << "Created file name: " << rootname << endl;
  sprintf(RootFileName,"%s",rootname);

  if(!CheckRunNumber(type)){
    //cerr << "Runnumber is invalid. Please let an expert know." << endl;
    return false;
  }
  return true;
}

void Lifetime::SetBranch(TTree *tree_main,TTree *tree_config,TTree*tree_af){

  tree_main->Branch("adc",TreeContent.adc,"ADC[32]/s");
  tree_main->Branch("adc2",TreeContent.adc2,"ADC2[32]/s");//add by muroyama
  tree_main->Branch("tdc",TreeContent.tdc,"TDC[32]/s");
  tree_main->Branch("adc_ref",&TreeContent.adc_ref,"ADC_REF/s");
  tree_main->Branch("tdc_ref",&TreeContent.tdc_ref,"TDC_REF/s");
  tree_main->Branch("time",&TreeContent.time,"TIME/I");
  tree_main->Branch("event",&TreeContent.event,"EVENT/I");

  tree_config->Branch("pmt1",TreeConfig.pmt1,"PMT1/C");
  tree_config->Branch("pmt2",TreeConfig.pmt2,"PMT2/C");
  tree_config->Branch("pmt3",TreeConfig.pmt3,"PMT3/C");
  tree_config->Branch("pmt4",TreeConfig.pmt4,"PMT4/C");
  tree_config->Branch("pmt5",TreeConfig.pmt5,"PMT5/C");
  tree_config->Branch("pmt6",TreeConfig.pmt6,"PMT6/C");
  tree_config->Branch("pmt7",TreeConfig.pmt7,"PMT7/C");
  tree_config->Branch("pmt8",TreeConfig.pmt8,"PMT8/C");
  tree_config->Branch("hv",TreeConfig.HV,"HV[8]/I");
  tree_config->Branch("DAQrate",&TreeConfig.DAQrate,"DAQrate/F");
  tree_config->Branch("LEDrate",&TreeConfig.LEDrate,"LEDrate/F");
  tree_config->Branch("rPMTrate",&TreeConfig.rPMTrate,"rPMTrate/F");
  tree_config->Branch("temp",&TreeConfig.temp,"TEMP/F");
  tree_config->Branch("humid",&TreeConfig.humid,"HUMID/F");
  
  tree_af->Branch("tdc_af",TreeContent.tdc_af,"TDC_af[6]/s"); 
  tree_af->Branch("tdc_main",TreeContent.tdc_main,"TDC_main[6]/s"); 
  tree_af->Branch("adc_main",TreeContent.adc_main,"ADC_main[6]/s"); 
  tree_af->Branch("adc2_main",TreeContent.adc2_main,"ADC2_main[6]/s"); 
 return;
}
//type means the kind of run, in this function define where the variable house in Branch
bool Lifetime::MainLoop(char *type,TTree *tree_main,TTree *tree_config,TTree *tree_af){
  int i = 0;
  int j = 0;
  int flag = 0;
  string buf;

  MakeSlotList();
  
  /* for(j = 0;j < SlotList.size();j++){
    cout << "slotlist" << endl;
    cout << SlotList.at(j) << endl;
    } */ 

  if(strcmp(type,"test") == 0)Scaler(1);
  else Scaler(10);
  cout << "Scaler:" << endl;
  cout << "  DAQ  rate = " << TreeConfig.DAQrate << " Hz" << endl;
  cout << "  LED  rate = " << TreeConfig.LEDrate << " Hz" << endl;
  cout << "  rPMT rate = " << TreeConfig.rPMTrate << " Hz" << endl;

  if( TreeConfig.rPMTrate > 20000 ){
    cout << "Error: too much count rate! (" << TreeConfig.rPMTrate << " Hz" << endl;
    return false;
  }

  for(i = 0;i < 8;i++)TreeConfig.HV[i] = HV[i];
  sprintf(TreeConfig.pmt1,"%s",PMTname[0]);
  sprintf(TreeConfig.pmt2,"%s",PMTname[1]);
  sprintf(TreeConfig.pmt3,"%s",PMTname[2]);
  sprintf(TreeConfig.pmt4,"%s",PMTname[3]);
  sprintf(TreeConfig.pmt5,"%s",PMTname[4]);
  sprintf(TreeConfig.pmt6,"%s",PMTname[5]);
  sprintf(TreeConfig.pmt7,"%s",PMTname[6]);
  sprintf(TreeConfig.pmt8,"%s",PMTname[7]);

  ifstream fin("cdata.txt");//cdata.txt is temparture and humid observe
  for(i = 0;i < 7;i++)fin >> buf;
  TreeConfig.temp = atof(buf.substr(14).c_str());
  fin >> buf;
  TreeConfig.humid = atof(buf.substr(10).c_str());
  fin.close();
  tree_config->Fill();

  //ofstream fout_check("/panfs/higgs/bfactory/mcppmt_data/life/dataset/check/check.dat");
  ofstream fout_check("check.dat");
  if(strcmp(type,"led") == 0){
    fout_check << "LED " << time(NULL) << endl;
  }else if(strcmp(type,"laser") == 0){
    fout_check << "Laser " << time(NULL) << endl;
  }
  fout_check.close();
  //system("ssh bfactory@localhost 'cp /home/takt/lifetime/daq/mcp/bin/check.dat /panfs/higgs/bfactory/mcppmt_data/life/dataset/check/'");
  system("ssh bfactory@localhost 'cp /home/takt/lifetime/daq/mcp/bin/check.dat /chai/sgt1/bfactory/mcppmt_data/life/dataset/check/'");
  
  for(i = 0;i < Nevent;i++){
    if(strcmp(type,"led") == 0)DAQ_LED(tree_main,tree_af,i + 1);
    else DAQ(tree_main,tree_af,i + 1);
    if(!Monitor(type,i + 1))cerr << "Error: Canvas is not set." << endl;
  }//in this sentence do DAQ and define "event" 
  return true;
}
//DAQ of laser run, if you add new TTree file, e.g TTree* tree_af, call the variables are two TTree type 
bool Lifetime::DAQ(TTree *tree_main,TTree*tree_af,int event){
  int i = 0;
  if(!flag_CAMAC){
    cerr << "Error: CAMAC is not opened!" << endl;
    return false;
  }
    
  for(i = 0;i < SlotList.size();i++)cam->Init(SlotList.at(i));//SlotList means all TDC and ADC ch
  for(i = 0;i < SlotList.size();i++)cam->LAM(SlotList.at(i));//at these two sentence, initialize and LAM camac at all TDC and ADC ch
  
  for(i = 0;i < 32;i++)if(TDCslot[i] > 0)TreeContent.tdc[i] = cam->Read(TDCslot[i],TDCch[i]);
  if(TDCslot[i] > 0)TreeContent.tdc_ref = cam->Read(TDCslot[i],TDCch[i]);
  for(i = 0;i < 32;i++)if(ADCslot[i] > 0)TreeContent.adc[i] = cam->Read(ADCslot[i],ADCch[i]);
  for(i = 0;i < 32;i++)if(ADC2slot[i] > 0)TreeContent.adc2[i] = cam->Read(ADC2slot[i],ADC2ch[i]);
  if(ADCslot[i] > 0)TreeContent.adc_ref = cam->Read(ADCslot[i],ADCch[i]);
  
  TreeContent.time = time(NULL);//access to variable time defined in TreeContent
  TreeContent.event = event;
  

  for(i=0;i < 6;i++)if(TDCafslot[i] > 0)TreeContent.tdc_af[i] = cam->Read(TDCafslot[i],TDCafch[i]);
  
  if(TDCslot[0] > 0)TreeContent.tdc_main[0] = TreeContent.tdc[0];//PMT1 ch1
  if(ADCslot[0] > 0)TreeContent.adc_main[0] = TreeContent.adc[0];
  if(TDCslot[1] > 0)TreeContent.tdc_main[1] = TreeContent.tdc[4];//PMT2 ch1
  if(ADCslot[1] > 0)TreeContent.adc_main[1] = TreeContent.adc[4];
  if(TDCslot[2] > 0)TreeContent.tdc_main[2] = TreeContent.tdc[12];//PMT4 ch1
  if(ADCslot[2] > 0)TreeContent.adc_main[2] = TreeContent.adc[12];
  if(TDCslot[3] > 0)TreeContent.tdc_main[3] = TreeContent.tdc[20];//PMT6 ch1
  if(ADCslot[3] > 0)TreeContent.adc_main[3] = TreeContent.adc[20];
  if(TDCslot[4] > 0)TreeContent.tdc_main[4] = TreeContent.tdc[24];//PMT7 ch1
  if(ADCslot[4] > 0)TreeContent.adc_main[4] = TreeContent.adc[24];
  if(TDCslot[5] > 0)TreeContent.tdc_main[5] = TreeContent.tdc[30];//PMT8 ch3
  if(ADCslot[5] > 0)TreeContent.adc_main[5] = TreeContent.adc[30];

  if(ADC2slot[0] > 0)TreeContent.adc2_main[0] = TreeContent.adc2[0];//PMT1 ch1
  if(ADC2slot[1] > 0)TreeContent.adc2_main[1] = TreeContent.adc2[4];//PMT2 ch1
  if(ADC2slot[2] > 0)TreeContent.adc2_main[2] = TreeContent.adc2[12];//PMT4 ch1
  if(ADC2slot[3] > 0)TreeContent.adc2_main[3] = TreeContent.adc2[20];//PMT6 ch1
  if(ADC2slot[4] > 0)TreeContent.adc2_main[4] = TreeContent.adc2[24];//PMT7 ch1
  if(ADC2slot[5] > 0)TreeContent.adc2_main[5] = TreeContent.adc2[30];//PMT8 ch1
  // cout << TreeContent.tdc_main[0] << endl;  

  tree_main->Fill();
  tree_af->Fill();
  
  return true;
}
//DAQ of LED run
bool Lifetime::DAQ_LED(TTree *tree_main,TTree *tree_af,int event){
  int i = 0;
  int j = 0;

  if(!flag_CAMAC){
    cerr << "Error: CAMAC is not opened!" << endl;
    return false;
  }

  for(i = 0;i < SlotList.size();i++)cam->Init(SlotList.at(i));
  for(i = 0;i < SlotList.size();i++)cam->LAM(SlotList.at(i));
  
  for(i = 0;i < 32;i++)if(TDCslot[i] > 0)TreeContent.tdc[i] = cam->Read(TDCslot[i],TDCch[i]);
  if(TDCslot[i] > 0)TreeContent.tdc_ref = cam->Read(TDCslot[i],TDCch[i]);
  for(i = 0;i < 32;i++)if(ADCslot[i] > 0)TreeContent.adc[i] = cam->Read(ADCslot[i],ADCch[i]);
  for(i = 0;i < 32;i++)if(ADC2slot[i] > 0)TreeContent.adc2[i] = cam->Read(ADC2slot[i],ADC2ch[i]);
  if(ADCslot[i] > 0)TreeContent.adc_ref = cam->Read(ADCslot[i],ADCch[i]);
  TreeContent.time = time(NULL);
  TreeContent.event = event;
  
  for(i=0;i < 6;i++)if(TDCafslot[i] > 0)TreeContent.tdc_af[i] = cam->Read(TDCafslot[i],TDCafch[i]);
  
  if(TDCslot[0] > 0)TreeContent.tdc_main[0] = TreeContent.tdc[0];//PMT1 ch1
  if(ADCslot[0] > 0)TreeContent.adc_main[0] = TreeContent.adc[0];
  if(TDCslot[1] > 0)TreeContent.tdc_main[1] = TreeContent.tdc[4];//PMT2 ch1
  if(ADCslot[1] > 0)TreeContent.adc_main[1] = TreeContent.adc[4];
  if(TDCslot[2] > 0)TreeContent.tdc_main[2] = TreeContent.tdc[12];//PMT4 ch1
  if(ADCslot[2] > 0)TreeContent.adc_main[2] = TreeContent.adc[12];
  if(TDCslot[3] > 0)TreeContent.tdc_main[3] = TreeContent.tdc[20];//PMT6 ch1
  if(ADCslot[3] > 0)TreeContent.adc_main[3] = TreeContent.adc[20];
  if(TDCslot[4] > 0)TreeContent.tdc_main[4] = TreeContent.tdc[24];//PMT7 ch1
  if(ADCslot[4] > 0)TreeContent.adc_main[4] = TreeContent.adc[24];
  if(TDCslot[5] > 0)TreeContent.tdc_main[5] = TreeContent.tdc[30];//PMT8 ch3
  if(ADCslot[5] > 0)TreeContent.adc_main[5] = TreeContent.adc[30];

  if(ADC2slot[0] > 0)TreeContent.adc2_main[0] = TreeContent.adc2[0];//PMT1 ch1
  if(ADC2slot[1] > 0)TreeContent.adc2_main[1] = TreeContent.adc2[4];//PMT2 ch1
  if(ADC2slot[2] > 0)TreeContent.adc2_main[2] = TreeContent.adc2[12];//PMT4 ch1
  if(ADC2slot[3] > 0)TreeContent.adc2_main[3] = TreeContent.adc2[20];//PMT6 ch1
  if(ADC2slot[4] > 0)TreeContent.adc2_main[4] = TreeContent.adc2[24];//PMT7 ch1
  if(ADC2slot[5] > 0)TreeContent.adc2_main[5] = TreeContent.adc2[30];//PMT8 ch1
  // cout << TreeContent.tdc_main[0] << endl;  

  tree_main->Fill();
  tree_af->Fill();
  return true;
}

bool Lifetime::Scaler(int wait){

  int i = 0;

  if(!flag_CAMAC){
    cerr << "Error: CAMAC is not opened!" << endl;
    return false;
  }

  cam->Clear(SCALERslot);
  cout << "Start scaling. Wait for " << wait << " sec..." << endl;
  sleep(wait);
  TreeConfig.DAQrate = (float)cam->Read(SCALERslot,DAQrate) / wait;
  TreeConfig.LEDrate = (float)cam->Read(SCALERslot,LEDrate) / wait;
  TreeConfig.rPMTrate = (float)cam->Read(SCALERslot,rPMTrate) / wait;

  return true;
}

void Lifetime::GetTempHumid(char *ip){
  char url[128];
  char command[128];
  sprintf(url,"http://%s/B/crrntdata/cdata.txt",ip);
  sprintf(command,"wget -nc %s",url);
  system("rm -f cdata.txt");
  system(command);
  return;
}

bool Lifetime::Monitor(char *type,int event){
  
  if(!flag_Canvas)return false;
  int add = 0;
  int i = 0;
  int af = 0;
  int af_n = 0;
  int af_n2 = 0;
  for(i = 0;i < 33;i++){//set the region of monitor window of TDC
    if(i < 32){
      if(i < 24)add = 0;
      else add = -100;
      if(i == 0)af = 0;
      if(i == 4)af = 1;
      if(i == 12)af = 2;
      if(i == 20)af = 3;
      if(i == 24)af = 4;
      if(i == 30)af = 5;
      if(i<16){
	if(i == 0 || i == 4 || i == 10 || i == 12){
	  if(strcmp(type,"led") == 0){
	    if(TreeContent.tdc[i] > TDCregion_MCP[2] + add && TreeContent.tdc[i] < TDCregion_MCP[3] + add)h_adc[i]->Fill(TreeContent.adc2[i]);
	  }else{
	    if(TreeContent.tdc[i] > TDCregion_MCP[2] + add && TreeContent.tdc[i] < TDCregion_MCP[3] + add)h_adc[i]->Fill(TreeContent.adc[i]);
	  }
	  if(i == 0 || i == 4 || i == 12){//monitor TDC for after pulse
	    if(TreeContent.tdc[i] > TDCregion_MCP[2] + add && TreeContent.tdc[i] < TDCregion_MCP[3] + add && TreeContent.tdc_af[af] < 4000 )h_tdc_af[af]->Fill(TreeContent.tdc_af[af]);
	  }
	}else{
	  if(strcmp(type,"led") == 0){
	    if(TreeContent.tdc[i] > TDCregion_MCP[0] + add && TreeContent.tdc[i] < TDCregion_MCP[1] + add)h_adc[i]->Fill(TreeContent.adc2[i]);
	  }else{
	    if(TreeContent.tdc[i] > TDCregion_MCP[0] + add && TreeContent.tdc[i] < TDCregion_MCP[1] + add)h_adc[i]->Fill(TreeContent.adc[i]);
	  }
	}
      }else{
	if(strcmp(type,"led") == 0){
	  if(TreeContent.tdc[i] > TDCregion_MCP[4] + add && TreeContent.tdc[i] < TDCregion_MCP[5] + add)h_adc[i]->Fill(TreeContent.adc2[i]);
	}else{
	  if(TreeContent.tdc[i] > TDCregion_MCP[4] + add && TreeContent.tdc[i] < TDCregion_MCP[5] + add)h_adc[i]->Fill(TreeContent.adc[i]);
	}
	if(i == 20 || i == 24 || i == 30){//monitor TDC for after pulse
	  if(TreeContent.tdc[i] > TDCregion_MCP[4] + add && TreeContent.tdc[i] < TDCregion_MCP[5] + add && TreeContent.tdc_af[af] < 4000)h_tdc_af[af]->Fill(TreeContent.tdc_af[af]);
	}
      }
      if(TreeContent.tdc[i] > 0)h_tdc[i]->Fill(TreeContent.tdc[i]);
    }else{
      if(TreeContent.tdc_ref > TDCregion_REF[0] && TreeContent.tdc_ref < TDCregion_REF[1])h_adc[i]->Fill(TreeContent.adc_ref);
      if(TreeContent.tdc_ref > 0)h_tdc[i]->Fill(TreeContent.tdc_ref);
    }
  }
  if(event % 1000 == 0){
    cout << endl;
    cout << RootFileName << "; " << event << " events." << endl;
    printf("ChNo:   hit : diff    ChNo:   hit : diff\n");
    for(i = 0;i < 16;i++){
      printf("Ch%2d:%6.2f%%: %4d    Ch%2d:%6.2f%%: %4d\n",i + 1,h_adc[i]->GetEntries() / event * 100,(int)h_adc[i]->GetEntries() - PrevCount_ADC[i],i + 17,h_adc[i + 16]->GetEntries() / event * 100,(int)h_adc[i + 16]->GetEntries() - PrevCount_ADC[i + 16]);
      if(event % 5000 == 0 || event == 1000){
	c_adc1->cd(i + 1);
	h_adc[i]->Draw();
	c_tdc1->cd(i + 1);
	h_tdc[i]->Draw();
	c_adc2->cd(i + 1);
	h_adc[i + 16]->Draw();
	c_tdc2->cd(i + 1);
	h_tdc[i + 16]->Draw();
	c_adc1->Update();
	c_adc2->Update();
	c_tdc1->Update();
	c_tdc2->Update();
	/* after-pulse monitor */
	/*if(i == 15){
	  for(af = 0;af < 6;af++){
	  c_tdc_af->cd(af + 1);
	  h_tdc_af[af]->Draw();
	  c_tdc_af->Update();
	  }
	  }*/	
      }
    }
    printf("rPMT:%6.2f%%: %4d\n",h_adc[32]->GetEntries() / (event + 1) * 100,(int)h_adc[32]->GetEntries() - PrevCount_ADC[32]);
    /* after-pulse monitor */
    printf("PMTNo:   afterpulse-rate:  diff: total-hit\n");
    for(i = 0;i < 6;i++){
      if(i == 0)af_n2 =0;
      if(i == 1)af_n2 =4;
      if(i == 2)af_n2 =12;
      if(i == 3)af_n2 =20;
      if(i == 4)af_n2 =24;
      if(i == 5)af_n2 =30;
      if(i < 2)af_n = i + 1;
      else if(i == 2)af_n = i + 2;
      else if(i > 2)af_n = i + 3;
      if(h_adc[af_n2]->GetEntries() == 0)printf("PMT%2d(af):%14.4s: %4d: %8d\n",af_n,"none",(int)h_tdc_af[i]->GetEntries() - PrevCount_TDC_af[i],(int)h_tdc_af[i]->GetEntries() );//afterpulse rate
      else printf("PMT%2d(af):%13f%%: %4d: %8d\n",af_n,h_tdc_af[i]->GetEntries() / h_adc[af_n2]->GetEntries() * 100,(int)h_tdc_af[i]->GetEntries()-PrevCount_TDC_af[i],(int)h_tdc_af[i]->GetEntries() );//afterpulse rate
    }
    /*  ~~~~~~~~~~~~~~~~  */
    for(i = 0;i < 33;i++){
      PrevCount_ADC[i] = (int)h_adc[i]->GetEntries();
      PrevCount_TDC[i] = (int)h_tdc[i]->GetEntries();
    }
    for(i = 0;i < 6;i++){
      PrevCount_TDC_af[i] = (int)h_tdc_af[i]->GetEntries();
    }
 
  }
  return true;
}

//void Lifetime::ReadCanvas(TCanvas *c1,TCanvas *c2,TCanvas *c3,TCanvas *c4,TCanvas *c5){//for afterpulse monitor
void Lifetime::ReadCanvas(TCanvas *c1,TCanvas *c2,TCanvas *c3,TCanvas *c4){
  c_adc1 = (TCanvas*)gROOT->FindObject(CANVAS1);
  c_adc1->Divide(4,4);
  c_adc2 = (TCanvas*)gROOT->FindObject(CANVAS2);
  c_adc2->Divide(4,4);
  c_tdc1 = (TCanvas*)gROOT->FindObject(CANVAS3);
  c_tdc1->Divide(4,4);
  c_tdc2 = (TCanvas*)gROOT->FindObject(CANVAS4);
  c_tdc2->Divide(4,4);
  //c_tdc_af = (TCanvas*)gROOT->FindObject(CANVAS5);//for afterpulse monitor
  //c_tdc_af->Divide(3,2);
  flag_Canvas = true;
    return;
}

void Lifetime::CompleteFlag(){
  flag_Complete = true;
  return;
}

void Lifetime::Calib(TTree *tree_main,TTree*tree_af){
  int i = 0;
  int j = 0;
  int Nevent = 5000;
  int evn = 0;
  char buf[128];
  char type_calib[512];
  sprintf(type_calib,"calib");
  MakeSlotList();

  for(i = 0;i < 13;i++){
    if(i < 12)cout << "Set " << i << " dB and push ENTER." << endl;
    else cout << "Set pedestal and push ENTER." << endl;
    getchar();
    for(j = 0;j < Nevent;j++){
      evn++;
      DAQ(tree_main,tree_af,evn);
      if(!Monitor(type_calib,evn))cerr << "Error: Canvas is not set." << endl;
    }
  } 
  
  return;
}


bool Lifetime::CheckRunNumber(char *type){
  ifstream fin;
  int runnum = -2;
  char fname[128];

  if(strcmp(type,"laser") == 0){
    sprintf(fname,"/home/takt/lifetime/dataset/%s/data/led/runnumber",DirName);
    fin.open(fname);
    fin >> runnum;
    fin.close();
    if(Nrun != runnum + 1 && Nrun != 0){
      cerr << "Runnumber is invalid. Please let an expert know. (comparison of laser with LED)" << endl;
      cout << "laser=" << Nrun << ", LED=" << runnum << endl;
      return false;
    }
  }
  
  if(strcmp(type,"pedestal") == 0){
    sprintf(fname,"/home/takt/lifetime/dataset/%s/data/laser/runnumber",DirName);
    fin.open(fname);
    fin >> runnum;
    fin.close();
    if(Nrun != runnum - 1){
      cerr << "Runnumber is invalid. Please let an expert know. (comparison of pedestal with laser)" << endl;
      return false;
    }
    sprintf(fname,"/home/takt/lifetime/dataset/%s/data/led/runnumber",DirName);
    fin.open(fname);
    fin >> runnum;
    fin.close();
    if(Nrun != runnum && Nrun != 0){
      cerr << "Runnumber is invalid. Please let an expert know. (comparison of pedestal with LED)" << endl;
      return false;
    }
  }
  
  if(strcmp(type,"led") == 0){
    sprintf(fname,"/home/takt/lifetime/dataset/%s/data/laser/runnumber",DirName);
    fin.open(fname);
    fin >> runnum;
    fin.close();
    if(Nrun != runnum - 1){
      cerr << "Runnumber is invalid. Please let an expert know. (comparison of LED with laser)" << endl;
      return false;
    }
  }

  return true;
}

void Lifetime::MakeSlotList(){
  int i = 0;
  int j = 0;
  int flag = 0;
  
  for(i = 0;i < 33;i++){
    flag = 0;
    for(j = 0;j < SlotList.size();j++){
      if(TDCslot[i] == SlotList.at(j))flag++;
    }//chsck all the elements in vector, check the double count of this list.  
    if(!flag && TDCslot[i] > 0)SlotList.push_back(TDCslot[i]);
  }
  for(i = 0;i < 33;i++){
    flag = 0;
    for(j = 0;j < SlotList.size();j++){
      if(ADCslot[i] == SlotList.at(j))flag++;
    }
    if(!flag && ADCslot[i] > 0)SlotList.push_back(ADCslot[i]);
  }
  for(i = 0;i < 33;i++){
    flag = 0;
    for(j = 0;j < SlotList.size();j++){
      if(ADC2slot[i] == SlotList.at(j))flag++;
    }
    if(!flag && ADC2slot[i] > 0)SlotList.push_back(ADC2slot[i]);
  }
  for(i = 0;i < 6;i++){
    flag = 0;
    for(j = 0;j < SlotList.size();j++){
      if(TDCafslot[i] == SlotList.at(j))flag++;
    }
    if(!flag && TDCafslot[i] > 0)SlotList.push_back(TDCafslot[i]);
  }
  return;
}
