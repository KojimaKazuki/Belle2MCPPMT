#define SAVE 1
#define SCP 1
#define DATABASE 0

#include "AnaTTSCh.cpp"
#include "DrawSquareCanvas.cpp"

void AnaTTS(char fname[], bool automode=false)
{
#define Nch 16
#define NHV_MAX 7

  const double gain_low = 0.5; // (x10^6)

  char title[128];
  sprintf(title,"%s",fname);
  strtok(title,"/");
  while (1) {
    sprintf(title,"%s",strtok(NULL,"/"));
    if (strstr(title,".root")) break;
  }
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

  string file_out(file_tts);
  file_out.replace(file_out.find(".root"),5,".pdf");

  gStyle->SetOptFit(1);

  double hv[NHV_MAX], err_hv[NHV_MAX];
  double tts[Nch][NHV_MAX], err_tts[Nch][NHV_MAX];
  double val, err;
  int c=0, ihv, hv_norm;
  char thumbnail[128];
  TList *hv_set;
  loop_ch:
    ihv=0;
    loop_hv:
      cout << AnaTTSCh(fname,c,ihv,false,&val,&err);
      cout << " ps TTS" << endl;
      tts[c][ihv] = val;
      err_tts[c][ihv] = err;
      hv_set = (TList*)gDirectory->Get("hv_set");
      if (!hv_set) {
        TFile *file_tmp = TFile::Open("tmp.root");
        hv_set = (TList*)file_tmp->Get("hv_set");
        file_tmp->Close();
      }
      const int Nhv = hv_set->GetEntries();
      if (tts[c][ihv]==0) goto end_hv;
      hv[ihv] = (double)((TParameter<int>*)hv_set->At(ihv))->GetVal();
      err_hv[ihv] = 0;
      if (!strcmp((char*)((TParameter<int>*)hv_set->At(ihv))->GetName(),
          "Nominal HV +0 V"))
        hv_norm = hv[ihv];
      ihv++;
      if (ihv<Nhv) goto loop_hv;
    end_hv:
#if SAVE
    sprintf(thumbnail,"Title:TDC ch%d",c);
    if (c==0) c1->Print((file_out+string("(")).c_str(),thumbnail);
    else      c1->Print(file_out.c_str(),thumbnail);
#endif
    cout << "ch" << c << " done" << endl << endl;
    c++;
    if (c<Nch) goto loop_ch;
  end_ch:

#if SAVE
  c00->Print(file_out.c_str(),"Title:TDC:ADC ch0-3");
  c01->Print(file_out.c_str(),"Title:TDC:ADC ch4-7");
  c02->Print(file_out.c_str(),"Title:TDC:ADC ch8-11");
  c03->Print(file_out.c_str(),"Title:TDC:ADC ch12-15");
#endif

#if DATABASE
  TMySQLServer *db = new TMySQLServer("mysql://192.168.6.41:3306/top","guest","hogehoge");
  TSQLResult *res;
  TSQLRow *row;
  char sql[256]="";

  double cnst[Nch], slope[Nch], gain[Nch][NHV_MAX], hv_min[Nch];
  double tts_max[Nch];
  int n_lowgain[Nch];
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
      if (gain[c][i] < gain_low) {
        n_lowgain[c]++;
      } else {
        hv_min[c] = hv[i];
        if (tts[c][i] > tts_max[c]) tts_max[c] = tts[c][i];
      }
    }
  }
#endif

  TCanvas *c2 = new TCanvas("c2","c2");
  c2->Divide(2,2);
  double tts_av[Nch], err_tts_av[Nch], ch[Nch], err_ch[Nch];
  TGraphErrors *g_tts[Nch], *g_tts_high[Nch];
  TPaveStats *st_tts[Nch];
  TF1 *f_tts = new TF1("f_tts","[0]");
  f_tts->SetParNames("Constant");
  gStyle->SetOptFit(0011);
  for (int c=0; c<Nch; c++) {
    c2->cd(c%4+1);
    sprintf(title,"%s ch%d",id,c);
    g_tts[c] = new TGraphErrors(Nhv,hv,tts[c],err_hv,err_tts[c]);
    g_tts[c]->SetMarkerStyle(24);
    g_tts[c]->SetMinimum(0.0);
    g_tts[c]->SetMaximum(100.0);
    g_tts[c]->Draw("AP");
    g_tts[c]->SetTitle(title);
    g_tts[c]->GetXaxis()->SetTitle("HV (V)");
    g_tts[c]->GetYaxis()->SetTitle("TTS (ps)");
    g_tts_high[c] = new TGraphErrors(Nhv-n_lowgain[c],hv,tts[c],err_hv,err_tts[c]);
    g_tts_high[c]->SetMarkerStyle(20);
    g_tts_high[c]->Draw("P");
    g_tts_high[c]->Fit("f_tts","","",hv_min[c],hv[0]);
    ch[c] = c;
    err_ch[c] = 0;
    tts_av[c] = f_tts->GetParameter(0);
    err_tts_av[c] = f_tts->GetParError(0);
    c2->Update();
    st_tts[c] = (TPaveStats*)g_tts_high[c]->FindObject("stats");
    st_tts[c]->SetX1NDC(0.55);
    st_tts[c]->SetX2NDC(0.85);
    st_tts[c]->SetY1NDC(0.82);
    st_tts[c]->SetY2NDC(0.88);
    st_tts[c]->Draw();
    c2->Update();
#if SAVE
    if (c%4==3) {
      sprintf(thumbnail,"Title:TTS(HV) ch%d-%d",c-3,c);
      c2->Print(file_out.c_str(),thumbnail);
    }
#endif
  }

  TCanvas *c3 = new TCanvas("c3","c3");
  sprintf(title,"%s, >%.1f#times10^{6}",id,gain_low);
  TH2D *frame_tts_ch = new TH2D("tts_ch",title,1,-1,16,1,0,100);
  tts_ch->SetStats(kFALSE);
  frame_tts_ch->GetXaxis()->SetTitle("channel");
  frame_tts_ch->GetYaxis()->SetTitle("TTS (ps)");
  frame_tts_ch->Draw();
  TGraphErrors *g_tts_ch = new TGraphErrors(Nch,ch,tts_av,err_ch,err_tts_av);
  g_tts_ch->SetMarkerStyle(20);
  g_tts_ch->Draw("P");
#if SAVE
  c3->Print((file_out+string(")")).c_str(),"Title:TTS(ch)");
#endif

  string input;
  cout << "OK? (yes/no): " << flush;
  if (automode) {
    input = "yes"; cout << "yes" << endl;
  } else {
    cin >> input;
  }
  if (input!="yes") return;

#if SAVE
  char buf[256];
//  if (gSystem->Exec((string("ls ")+="fig/tts/"+file_out+" > /dev/null").c_str()))
  {
    sprintf(buf,"(mv %s fig/tts/)",file_out.c_str());
    gSystem->Exec(buf);
#if SCP
    string ssh_dir = " matsuoka.nagoya@hepws-9:~/public_html/top/file/tts/";
    gSystem->Exec((string("scp -p fig/tts/")+=file_out+ssh_dir).c_str());
#endif
  }
#endif

  /*#if DATABASE
  sprintf(sql,"SELECT Nlaser FROM laser_test WHERE id='%s'",id);
  res = db->Query(sql);
  int Nlaser = res->GetRowCount();
  delete res;

  bool update = false;
  for (int i=0; i<Nlaser; i++) {
    sprintf(sql,"SELECT root_laser FROM laser_test WHERE id='%s' AND Nlaser='%d'",id,i+1);
    res = db->Query(sql);
    row = res->Next();
    if (!row) { delete row; delete res; delete db; return;}
    if (!row->GetFieldLength(0)) { delete row; delete res; delete db; return;}
    if (!strcmp(row->GetField(0),file_tts)) {
      cout << "Database already exist\nUpdate? (yes/no): " << flush;
      if (automode) {
        input = "yes"; cout << "yes" << endl;
      } else {
        cin >> input;
      }
      if (input=="yes") {
        update = true;
        Nlaser = i;
        delete row; delete res;
        break;
      }
      delete row; delete res; delete db; return;
    }
    delete row; delete res;
  }

  if (update) {
    sprintf(sql,"UPDATE laser_test SET plot_tts='%s', tts='%.1f', tts_max='%.1f' WHERE id='%s' AND Nlaser='%d'",
            file_out.c_str(),TMath::Mean(Nch,tts_av),TMath::MaxElement(Nch,tts_max),
            id,Nlaser+1);
    res = db->Query(sql); delete res;
    sprintf(sql,"SELECT ch FROM laser_ana WHERE id='%s' AND Nlaser='%d'",id,Nlaser+1);
    res = db->Query(sql);
    bool update_tts = res->GetRowCount();
    delete res;
    if (update_tts) {
      for (int c=0; c<Nch; c++) {
        sprintf(sql,"UPDATE laser_ana SET tts='%.1f', tts_max='%.1f' WHERE id='%s' AND Nlaser='%d' AND ch='%d'",
                tts_av[c],tts_max[c],id,Nlaser+1,c);
        res = db->Query(sql); delete res;
        for (int i=0; i<Nhv; i++) {
          sprintf(sql,"UPDATE laser_data SET tts='%.1f' WHERE id='%s' AND Nlaser='%d' AND hv='%d' AND ch='%d'",
                  tts[c][i],id,Nlaser+1,hv[i],c);
//          sprintf(sql,"INSERT INTO laser_data (id,Nlaser,hv,ch,tts) VALUES('%s','%d','%d','%d','%.1f')",
//                  id,Nlaser+1,hv[i],c,tts[c][i]);
          res = db->Query(sql); delete res;
        }
      }
    } else {
      for (int c=0; c<Nch; c++) {
        if (!tts_av[c]) continue;
        sprintf(sql,"INSERT INTO laser_ana (id,Nlaser,ch,tts,tts_max) VALUES('%s','%d','%d','%.1f','%.1f')",
                id,Nlaser+1,c,tts_av[c],tts_max[c]);
        res = db->Query(sql); delete res;
        for (int i=0; i<Nhv; i++) {
          if (!tts[c][i]) continue;
          sprintf(sql,"INSERT INTO laser_data (id,Nlaser,hv,ch,tts) VALUES('%s','%d','%d','%d','%.1f')",
                  id,Nlaser+1,hv[i],c,tts[c][i]);
          res = db->Query(sql); delete res;
        }
      }
    }

  } else {
    sprintf(sql,"INSERT INTO laser_test (id,Nlaser,date,root_laser,plot_tts,tts,tts_max) VALUES('%s','%d','%s','%s','%s','%.1f','%.1f')",
            id,Nlaser+1,date,file_tts,file_out.c_str(),
            TMath::Mean(Nch,tts_av),TMath::MaxElement(Nch,tts_max));
    res = db->Query(sql); delete res;
    for (int c=0; c<Nch; c++) {
      if (!tts_av[c]) continue;
      sprintf(sql,"INSERT INTO laser_ana (id,Nlaser,ch,tts,tts_max) VALUES('%s','%d','%d','%.1f','%.1f')",
              id,Nlaser+1,c,tts_av[c],tts_max[c]);
      res = db->Query(sql); delete res;
      for (int i=0; i<Nhv; i++) {
        if (!tts[c][i]) continue;
        sprintf(sql,"INSERT INTO laser_data (id,Nlaser,hv,ch,tts) VALUES('%s','%d','%d','%d','%.1f')",
                id,Nlaser+1,hv[i],c,tts[c][i]);
        res = db->Query(sql); delete res;
      }
    }
  }

  delete db;
#endif
  */
  cout << file_tts << endl;

//  if (automode) gApplication->Terminate();

  return;
}
