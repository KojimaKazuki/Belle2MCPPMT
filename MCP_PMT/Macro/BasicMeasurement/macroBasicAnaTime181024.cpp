#define SAVE 1
#define SCP 1
#define DATABASE 0

#include "AnaTTSCh.cpp"
#include "DrawSquareCanvas.cpp"

void macroBasicAnaTime181024(){

#define Nch 16
#define NHV_MAX 7
gStyle->SetOptFit(1);
const double gainLow = 0.5; // (x10^6);

double cnst[Nch], slope[Nch], gain[Nch][NHV_MAX], hv_min[Nch];
double tts_max[Nch];
int n_lowgain[Nch];


  // #Input File: PMT-ID, Date# ;
  char fileNameIn[128];

  while (true){
    std::cout << "Please tell me the name of input file." << std::endl;
    std::cin >> fileNameIn;

      if (std::cin.fail()){
        std::cout << "Input Error! Please check it!\n" << std::endl;
        std::cin.clear();
        std::cin.ignore(1024, '\n');
        continue;
      }
      break;
  }
  std::cout << fileNameIn << std::endl;


  //char fileName0[128] = "/home/bqt/kojima/MCP_PMT/Data/1809/";
  //char fileName0[128] = "/home/bqt/kojima/MCP_PMT/Data/1810/";
  char fileName0[128] = "/home/bqt/kojima/MCP_PMT/Data/1811/";
  //char fileName0[128] = "/chai/sgt1/bfactory/mcppmt_data/kihon/03/";
  char fileName[128];
  sprintf( fileName, "%s%s" ,fileName0 ,fileNameIn);
  std::cout << "Input File: " << fileName << std::endl;


  char title[128];
  sprintf(title,"%s",fileNameIn);
  strtok(title,"/");
  std::cout << "Input File Name: " << title << std::endl;

  /*
  while (true) {
    sprintf(title,"%s",strtok(NULL,"/"));
    if (strstr(title,".root")) break;
  }
  */


  char file_time[128];
  sprintf(file_time,"%s",title);
  char id[64];
  sprintf(id,"%s",title);
  strtok(id,"_");
  char date[64];
  sprintf(date,"%s",title);
  strtok(date,"_");
  sprintf(date,"%s",strtok(NULL,".root"));
  if (strlen(date)>8) sprintf(date,"%s",strtok(date,"_"));


//  // #Output File Name -> PDF Format# ;
//  string fileOutPDF(file_time);
//  fileOutPDF.replace(fileOutPDF.find(".root"),5,"Time.pdf");

  // #Output File Name -> CSV Format# ;
  string fileOutCSV(file_time);
  fileOutCSV.replace(fileOutCSV.find(".root"),5,"Time.csv");


  std::cout << "PMT ID: " << id << std::endl;
  std::cout << "Date: " << date << std::endl;
//  std::cout << "Output File Name (PDF): " << fileOutPDF << std::endl;
  std::cout << "Output File Name (CSV): " << fileOutCSV << std::endl;


  // #Input File Open# ;
  TFile *file = TFile::Open(fileName);
  TTree *mcp = (TTree*)file ->Get("mcp");
  int nAllEntry = mcp ->GetEntries();


  // #Output File Open# ;
  std::ofstream ofs(fileOutCSV.c_str());

  if(!ofs){
  std::cout << "It has failed to open the file: " << fileOutCSV << std::endl;
  std::cin.get();
  return;
  }


  // #Get HV Value#
  // #Get the Time for a Measurement of Each Channel at a HV Value# ;
  double hv0[NHV_MAX], err_hv0[NHV_MAX];
  double tts[Nch][NHV_MAX], err_tts[Nch][NHV_MAX];
  double val, err;
  int channel[Nch];
  int ch = 0, iHV, HV_norm = 0;
  char thumbnail[128];
  TList *hv_set;


  UShort_t hv = 0;
  int time = 0;
  int nEntry[Nch][NHV_MAX];
  int nTotalEntry[Nch] = {};
  int timeStart[Nch][NHV_MAX], timeEnd[Nch][NHV_MAX];
  int hvStart[Nch][NHV_MAX], hvEnd[Nch][NHV_MAX];
  int hv0Out[Nch][NHV_MAX];


  loop_ch:
    iHV = 0;
    channel[ch] = ch;


    loop_HV:

      // #Go to HV Value Directory# ;
      hv_set = (TList*)gDirectory ->Get("hv_set");

      if (!hv_set) {
        TFile *file_hv = TFile::Open(fileName);
        hv_set = (TList*)file_hv ->Get("hv_set");
        file_hv ->Close();
      }
      const int nHV = hv_set ->GetEntries();


      // #Get HV Value# ;
      hv0[iHV] = (double)((TParameter<int>*)hv_set ->At(iHV)) ->GetVal();
      err_hv0[iHV] = 0;

      // #Get Normal HV Value# ;
      if (!strcmp((char*)((TParameter<int>*)hv_set->At(iHV))->GetName(),"Nominal HV +0 V"))
        HV_norm = hv0[iHV];
      // A = B ==> strcmp(A,B) = 0: Comparison of character strings.;



      // #Get Time for Each Measurement# 
      // #Set Branch Address#
      mcp ->SetBranchAddress("hv", &hv[ch][iHV]);
      mcp ->SetBranchAddress("time", &time[ch][iHV]);

      // #Event Selection#
      char cut[128];
      sprintf( cut, "hv == %d && ipos == %d", (int)hv0[iHV], channel[ch]);
      mcp ->Draw(">>eventList", cut);

      TEventList *eventList = (TEventList*)gROOT ->FindObject("eventList");
//      mcp ->GetEventList()
      nEntry[ch][iHV] = eventList ->GetN();

      // #Get the Entry# the i-th Event Number Satisfying the cut: eventList ->GetEntry(i);
      // #Get the Start Time & the End Time# ; 
      for(int i = 0; i < nEntry[ch][iHV]; i++){
        mcp ->GetEntry(eventList ->GetEntry(i));

        if (i == 0){
          timeStart[ch][iHV] = time;
          hvStart[ch][iHV] = (int)hv;
        }
        if (i == nEntry[ch][iHV] -1){
          timeEnd[ch][iHV] = time;
          hvEnd[ch][iHV] = (int)hv;
        }
      }

      nTotalEntry[ch] += nEntry[ch][iHV];
      hv0Out[ch][iHV] = hv0[iHV];



      iHV++;
      if (iHV < nHV) goto loop_HV;
    end_HV:

    if (!HV_norm) HV_norm = hv[1];




    std::cout << "Ch" << ch << " done" << std::endl;
    ch ++;
    if (ch < Nch) goto loop_ch;
  end_ch:


  // #Output(CSV Format) the Average TTS Sigma Value# ;
  ofs << "#MCP-PMT ID" << "," << "Ch No." << "," << "Date" << "," << "# of Entries" << "," << "HV Set Value / V" << "," << "HV Start / V" << "," << "HV End / V" << "," << "Start Time" << "," << "End Time" << std::endl;

  ofs << id << "," << "0" << "," << date << "," << nAllEntry  << "," << 0 << "," << hvStart[0][0] << "," << hvEnd[Nch-1][nHV-1] << "," << timeStart[0][0] << "," << timeEnd[Nch-1][nHV-1] << std::endl;

  for (int ch = 0; ch < Nch; ch++){
      ofs << id << "," << channel[ch] + 1 << "," << date << "," << nTotalEntry[ch] << "," << 0 << "," << hvStart[ch][0] << "," << hvEnd[ch][nHV-1] << "," << timeStart[ch][0] << "," << timeEnd[ch][nHV-1] << std::endl;
    for (int iHV = 0; iHV < nHV; iHV++){
      ofs << id << "," << channel[ch] + 1 << "," << date << "," << nEntry[ch][iHV] << "," << hv0Out[ch][iHV] << "," << hvStart[ch][iHV] << "," << hvEnd[ch][iHV] << "," << timeStart[ch][iHV] << "," << timeEnd[ch][iHV] << std::endl;
    }
  }


  string input;
  std::cout << "OK? (yes/no): " << std::flush;
  std::cin >> input;
  if (input!="yes") return;


  std::cout << "Input File: " << fileName << std::endl;
  std::cout << "PMT ID: " << id << std::endl;
  std::cout << "Date: " << date << std::endl;
//  std::cout << "Output File Name (PDF): " << fileOutPDF << std::endl;
  std::cout << "Output File Name (CSV): " << fileOutCSV << std::endl;

  return;
}
