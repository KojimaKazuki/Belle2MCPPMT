#define SAVE 1
#define SCP 1
#define DATABASE 0

#include "AnaTTSCh.cpp"
#include "DrawSquareCanvas.cpp"

void macroBasicAnaTTS181023(){

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


  char file_tts[128];
  sprintf(file_tts,"%s",title);
  char id[64];
  sprintf(id,"%s",title);
  strtok(id,"_");
  char date[64];
  sprintf(date,"%s",title);
  strtok(date,"_");
  sprintf(date,"%s",strtok(NULL,".root"));
  if (strlen(date)>8) sprintf(date,"%s",strtok(date,"_"));


  // #Output File Name -> PDF Format# ;
  string fileOutPDF(file_tts);
  fileOutPDF.replace(fileOutPDF.find(".root"),5,"TTS.pdf");

  // #Output File Name -> CSV Format# ;
  string fileOutCSV(file_tts);
  fileOutCSV.replace(fileOutCSV.find(".root"),5,"TTS.csv");


  std::cout << "PMT ID: " << id << std::endl;
  std::cout << "Date: " << date << std::endl;
  std::cout << "Output File Name (PDF): " << fileOutPDF << std::endl;
  std::cout << "Output File Name (CSV): " << fileOutCSV << std::endl;



  // #Output File Open# ;
  std::ofstream ofs(fileOutCSV.c_str());

  if(!ofs){
  std::cout << "It has failed to open the file: " << fileOutCSV << std::endl;
  std::cin.get();
  return;
  }


  // #Get HV Value# #Get TDC->TTS Value#
  double hv[NHV_MAX], err_hv[NHV_MAX];
  double tts[Nch][NHV_MAX], err_tts[Nch][NHV_MAX];
  double val, err;
  int ch = 0, iHV, HV_norm = 0;
  char thumbnail[128];
  TList *hv_set;

  loop_ch:
    iHV=0;

    // #TDC-> TTS Value for Each Channel at a HV Value# ;
    loop_HV:
      // #Get TDC->TTS Value# ;
      cout << AnaTTSCh( fileName, ch, iHV, false, &val, &err);
      cout << " ps TTS" << endl;
      tts[ch][iHV] = val;
      err_tts[ch][iHV] = err;


      // #Go to HV Value Directory# ;
      hv_set = (TList*)gDirectory->Get("hv_set");

      if (!hv_set) {
        TFile *file_tmp = TFile::Open("tmp.root");
        hv_set = (TList*)file_tmp->Get("hv_set");
        file_tmp->Close();
      }


      const int nHV = hv_set->GetEntries();

      if (tts[ch][iHV]==0) goto end_HV;

      // #Get HV Value# ;
      hv[iHV] = (double)((TParameter<int>*)hv_set ->At(iHV))->GetVal();
      err_hv[iHV] = 0;

      // #Get Normal HV Value# ;
      if (!strcmp((char*)((TParameter<int>*)hv_set->At(iHV))->GetName(),"Nominal HV +0 V"))
        HV_norm = hv[iHV];
      // A = B ==> strcmp(A,B) = 0: Comparison of character strings.;

      iHV++;
      if (iHV < nHV) goto loop_HV;
    end_HV:

    if (!HV_norm) HV_norm = hv[1];


    // #TDC->TTS Graph for Each Channel# After Timewalk Correction ;
#if SAVE
    sprintf(thumbnail,"Title:TDC ch%d",ch);

    if (ch == 0) c1->Print((fileOutPDF+string("(")).c_str(),thumbnail);
    else         c1->Print(fileOutPDF.c_str(),thumbnail);
#endif

    std::cout << "Ch" << ch << " done" << std::endl;
    ch ++;
    if (ch < Nch) goto loop_ch;
  end_ch:



  // #Graph of Timewalk Correction# ;
#if SAVE
  c00 ->Print(fileOutPDF.c_str(),"Title:TDC:ADC Ch0-3");
  c01 ->Print(fileOutPDF.c_str(),"Title:TDC:ADC Ch4-7");
  c02 ->Print(fileOutPDF.c_str(),"Title:TDC:ADC Ch8-11");
  c03 ->Print(fileOutPDF.c_str(),"Title:TDC:ADC Ch12-15");
#endif


  // #Check Range of the Low Gain Data# ;
  // #Get "n_lowgain" Value # ;
#if DATABASE
  TMySQLServer *db = new TMySQLServer("mysql://192.168.6.41:3306/top","guest","hogehoge");
  TSQLResult *res;
  TSQLRow *row;
  char sql[256]="";

  sprintf(sql,"SELECT Nlaser FROM laser_test WHERE id='%s'",id);
  res = db->Query(sql);
  int nlaser = res->GetRowCount();
  delete res;
  for (int c=0; c<Nch; c++) {
    sprintf(sql,"SELECT gain_const,gain_slope FROM laser_ana WHERE id='%s' AND Nlaser='%d' AND ch=%d",id,nlaser,c);
    res = db->Query(sql);
    row = res->Next();
    if (!row) {
      delete row; delete res; continue;
    }
    if (!row->GetFieldLength(0)) {
      cout << "No gain data" << endl;
      delete row; delete res; return;
    }
    cnst[c] = atof(row->GetField(0));
    slope[c] = atof(row->GetField(1));
    delete row; delete res;

    n_lowgain[c] = 0;
    tts_max[c] = 0;
    for (int i=0; i<Nhv; i++) {
      gain[c][i] = exp(cnst[c]+slope[c]*hv[i]);
      if (gain[c][i] < gainLow) {
        n_lowgain[c]++;
      } else {
        hv_min[c] = hv[i];
        if (tts[c][i] > tts_max[c]) tts_max[c] = tts[c][i];
      }
    }
  }
#endif
  std::cout << n_lowgain << std::endl;


  // #TTS Analyses for Each Channel# ;
  TCanvas *c2 = new TCanvas("c2","c2");
  c2->Divide(2,2);// Use 1 canvas for 4 channel at the same column;


  TGraphErrors *graph_tts[Nch], *graph_tts_high[Nch];
  TPaveStats *st_tts[Nch];
  // #Fitting Function: Constant Function# ;
  TF1 *func_tts = new TF1("func_tts","[0]");
  func_tts ->SetParNames("Constant");


  gStyle->SetOptFit(0011);


  // #Graph: TTS(Y) vs. HV(X)# ;
  // 1 Graph for 4 Channel: (1,5,9,13),(2,6,10,14),(3,7,11,15),(4,8,12,16);
  double ttsAve[Nch], err_ttsAve[Nch], channel[Nch], err_channel[Nch];
  double sum_ttsAve = 0, sum_err_ttsAve = 0;

  for (int ch = 0; ch < Nch; ch++) {
    c2 ->cd(ch%4+1);

    sprintf(title,"%s Ch%d", id, ch); //JT0100 Ch0;


    // #Graph: TTS(Y) vs. HV(X)# ;
    // (Use the All Data) ;
    graph_tts[ch] = new TGraphErrors( nHV, hv, tts[ch], err_hv, err_tts[ch]);
    graph_tts[ch] ->SetMarkerStyle(24);
    graph_tts[ch] ->SetMinimum(0.0);
    graph_tts[ch] ->SetMaximum(100.0);
    graph_tts[ch] ->Draw("AP");
    graph_tts[ch] ->SetTitle(title);
    graph_tts[ch] ->GetXaxis() ->SetTitle("HV / V");
    graph_tts[ch] ->GetYaxis() ->SetTitle("TTS / ps");


    // #Fitting for Graph: TTS(Y) vs. HV(X)# Constant Function ;
    // (Use the Data above 0.5 x 10^6 Gain) ;
    // TTS is independent from HV Value;
    graph_tts_high[ch] = new TGraphErrors( nHV-n_lowgain[ch], hv, tts[ch], err_hv, err_tts[ch]);
    graph_tts_high[ch]->SetMarkerStyle(20);
    graph_tts_high[ch]->Draw("P");
    graph_tts_high[ch]->Fit( "func_tts","","", hv_min[ch], hv[0]);


    // #Get the Average TTS Sigma Value for Each Channel# ;
    ttsAve[ch] = func_tts->GetParameter(0);
    err_ttsAve[ch] = func_tts->GetParError(0);
    c2->Update();


    // #Statistics Box# ;
    st_tts[ch] = (TPaveStats*)graph_tts_high[ch]->FindObject("stats");
    st_tts[ch]->SetX1NDC(0.55);
    st_tts[ch]->SetX2NDC(0.85);
    st_tts[ch]->SetY1NDC(0.82);
    st_tts[ch]->SetY2NDC(0.88);
    st_tts[ch]->Draw();
    c2->Update();


    channel[ch] = ch;
    err_channel[ch] = 0;

    //Abnormal value; 
    //if (ch == 5 ) err_ttsAve[ch] = 0.;
    //if (ch == 4 ) err_ttsAve[ch] = 0.;
    sum_ttsAve += ttsAve[ch];
    sum_err_ttsAve += pow(err_ttsAve[ch],2.);


#if SAVE
    // #Print Graphs to Output PDF FIle# TTS(Y) vs. HV(X) ;
    if (ch%4==3) {
      sprintf(thumbnail,"Title:TTS(HV) Ch%d-%d",ch-3,ch);
      c2->Print(fileOutPDF.c_str(),thumbnail);
    }
#endif

  }


  // #Get the Average TTS Sigma Value of All channels# ;
  double ttsAveAll, err_ttsAveAll;
  ttsAveAll = sum_ttsAve / (Nch);
  err_ttsAveAll = sqrt(sum_err_ttsAve) / (Nch);


  // #Output(CSV Format) the Average TTS Sigma Value# ;
  ofs << "#MCP-PMT ID" << "," << "Ch No." << "," << "Date" << "," << "TTS Sigma (ps)" << "," << "Error of TTS Sigma (ps)" << std::endl;
  ofs << id << "," << "0" << "," << date << "," << ttsAveAll << "," << err_ttsAveAll << std::endl;
  for (int ch = 0; ch < Nch; ch++){
  ofs << id << "," << channel[ch] + 1 << "," << date << "," << ttsAve[ch] << "," << err_ttsAve[ch] << std::endl;
  }


  // #Make 16 Channels' TTS Plot# TTS(Y) vs. Channel No.(X) ;
  // (Use the Data above 0.5 x 10^6 Gain) ;
  TCanvas *c3 = new TCanvas("c3","c3");

  // #Make the Frame of the Plot# ;
  sprintf(title,"%s, >%.1f#times10^{6}", id, gainLow);
  TH2D *frame_tts_ch = new TH2D("tts_ch",title,1,-1,16,1,0,100);
  tts_ch ->SetStats(kFALSE);
  frame_tts_ch ->GetXaxis() ->SetTitle("Channel");
  frame_tts_ch ->GetYaxis() ->SetTitle("TTS / ps");
  frame_tts_ch ->Draw();

  TGraphErrors *graph_tts_ch = new TGraphErrors( Nch, channel, ttsAve, err_channel, err_ttsAve);
  graph_tts_ch ->SetMarkerStyle(20);
  graph_tts_ch ->Draw("P");
#if SAVE
  c3 ->Print((fileOutPDF+string(")")).c_str(),"Title:TTS(Ch)");
#endif



  string input;
  std::cout << "OK? (yes/no): " << std::flush;
  std::cin >> input;
  if (input!="yes") return;


  std::cout << "Input File: " << fileName << std::endl;
  std::cout << "PMT ID: " << id << std::endl;
  std::cout << "Date: " << date << std::endl;
  std::cout << "Output File Name (PDF): " << fileOutPDF << std::endl;
  std::cout << "Output File Name (CSV): " << fileOutCSV << std::endl;

  return;
}

