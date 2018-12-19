#define SAVE 1
#define DATABASE 0

#include "AnaGainCh2.cpp"
#include "DrawSquareCanvas.cpp"

void macroBasicAna180919(char fname[], bool automode=false)
{
  const double gain_av[2] = {0.5, 1.}; // (x10^6)
#define Nch 16
#define NHV_MAX 7

  char title[128];
  sprintf(title,"%s",fname);
  strtok(title,"/");
  while (1) {
    sprintf(title,"%s",strtok(NULL,"/"));
    if (strstr(title,".root")) break;
    }
  char file_gain[128];
  sprintf(file_gain,"%s",title);
  char id[64];
  sprintf(id,"%s",title);
  strtok(id,"_");
  char date[64];
  sprintf(date,"%s",title);
  strtok(date,"_");
  sprintf(date,"%s",strtok(NULL,".root"));
  if (strlen(date)>8) sprintf(date,"%s",strtok(date,"_"));
  int amp_ver = 0;
  if (strncmp(date,"20140401",8)>=0) amp_ver = 1;
  bool old_data = false;
  if (strncmp(date,"20120531",8)<=0) old_data = true;

  string file_out(file_gain);
  file_out.replace(file_out.find(".root"),5,".pdf");

  double hv[NHV_MAX], err_hv[2][NHV_MAX];
  double gain[Nch][NHV_MAX], err_gain[2][Nch][NHV_MAX];
  double val, err_l, err_h, rate0;
  int c=0, ihv, hv_norm=0;
  char thumbnail[128];
  TList *hv_set;
  loop_ch:
    ihv=0;
    loop_hv:
//      AnaGainCh(fname,c,ihv,false,&val,&err);
    cout << AnaGainCh2(fname,c,ihv,false,&val,&err_l,&err_h);//,&rate0);
      cout << "e6 gain" << endl;;
      gain[c][ihv] = val;
      err_gain[0][c][ihv] = err_l;
      err_gain[1][c][ihv] = err_h;
      hv_set = (TList*)gDirectory->Get("hv_set");
      if (!hv_set) {
//        TFile *file_tmp = TFile::Open("tmp.root");
//        hv_set = (TList*)file_tmp->Get("hv_set");
//        file_tmp->Close();
        TClonesArray par_hv;
        par_hv.SetClass("TParameter<int>");
        par_hv.SetName("hv_set");
        mcp->Draw("hv>>h_hv(400,0,4000)","","goff");
        TH1D *h_hv = (TH1D*)gROOT->FindObject("h_hv");
        int n=0;
        for (int i=h_hv->GetNbinsX()-1; i>=0; i--) {
          if (h_hv->GetBinContent(i+1)) {
            par_hv[n] = new TParameter<int>("HV",h_hv->GetBinLowEdge(i+1));
            n++;
          }
        }
        hv_set = (TList*)par_hv;
      }
      const int Nhv = hv_set->GetEntries();
      if (gain[c][ihv]==0) goto end_hv;
      hv[ihv] = (double)((TParameter<int>*)hv_set->At(ihv))->GetVal();
      err_hv[0][ihv] = 0;
      err_hv[1][ihv] = 0;
      if (!strcmp((char*)((TParameter<int>*)hv_set->At(ihv))->GetName(),
          "Nominal HV +0 V"))
        hv_norm = hv[ihv];
      ihv++;
      if (ihv<Nhv) goto loop_hv;
    end_hv:
    if (!hv_norm) hv_norm = hv[1];
#if SAVE
    sprintf(thumbnail,"Title:ADC ch%d",c);
 
    if (c==0) c1->Print((file_out+string("(")).c_str(),thumbnail);
    else      c1->Print(file_out.c_str(),thumbnail);
#endif
    cout << "ch" << c << " done" << endl << endl;
    c++;
    if (c<Nch) goto loop_ch;
  end_ch:

  TCanvas *c2 = new TCanvas("c2","c2");
  c2->Divide(2,2);
//  c2->Divide(4,4);
  TGraphAsymmErrors *g_gain_hv[Nch];
  TF1 *f_gain[Nch];
  TPaveStats *st_gain[Nch];
  gStyle->SetOptFit(0011);
  char name[32];
  double hv_max;
  for (c=0; c<Nch; c++) {
    c2->cd(c%4+1);
//    c2->cd(c+1);
    sprintf(name,"graph_ch%d",c);
    sprintf(title,"%s ch%d",id,c);
    g_gain_hv[c] = new TGraphAsymmErrors(ihv,hv,gain[c],err_hv[0],err_hv[1],err_gain[0][c],err_gain[1][c]);
    g_gain_hv[c]->SetMarkerStyle(20);
    g_gain_hv[c]->SetMinimum(0.0);
    g_gain_hv[c]->SetMaximum(4.0);
    g_gain_hv[c]->Draw("AP");
    g_gain_hv[c]->SetTitle(title);
    g_gain_hv[c]->SetName(name);
    g_gain_hv[c]->GetXaxis()->SetTitle("HV (V)");
    g_gain_hv[c]->GetYaxis()->SetTitle("gain (#times10^{6})");
    hv_max = 0;
    if (old_data) hv_max = hv[2];
    for (int i=0; i<ihv; i++) {
      if (amp_ver==0) {
//        if (((ihv-i)<=4 || gain[c][i]<2.5) && hv_max < hv[i]) hv_max = hv[i];
        if (gain[c][i]<3.0 && hv_max < hv[i]) hv_max = hv[i];
      } else {
        if (gain[c][i]<4.0 && hv_max < hv[i]) hv_max = hv[i];
      }
      if (old_data) {
        if (gain[c][i]<2.0 && hv_max < hv[i]) hv_max = hv[i];
      }
    }
    g_gain_hv[c]->Fit("expo","","",hv[ihv-1],hv_max);
    sprintf(name,"ch%d",c);
    f_gain[c] = new TF1(name,"exp([0]+[1]*x)",0,4000);
    f_gain[c]->SetParameters(expo->GetParameter(0),expo->GetParameter(1));
    f_gain[c]->SetLineStyle(9);
    f_gain[c]->Draw("same");

    c2->Update();
    st_gain[c] = (TPaveStats*)g_gain_hv[c]->FindObject("stats");
    st_gain[c]->SetX1NDC(0.2);
    st_gain[c]->SetX2NDC(0.5);
    st_gain[c]->SetY1NDC(0.76);
    st_gain[c]->SetY2NDC(0.88);
    st_gain[c]->Draw();
    c2->Update();
#if SAVE
    if (c%4==3) {
      sprintf(thumbnail,"Title:gain(HV) ch%d-%d",c-3,c);
      c2->Print(file_out.c_str(),thumbnail);
    }
#endif
  }

  TCanvas *c3 = new TCanvas("c3","c3");
  c3->SetLogy(1);
  char buf[256]="";
  for (int c=0; c<Nch; c++) {
    sprintf(name,"ch%d",c);
    if (c==0) sprintf(buf,"%s",name);
    else      sprintf(buf,"%s+%s",buf,name);
  }
  sprintf(buf,"(%s)/%d",buf,Nch);
  TF1 *f_av = new TF1("av",buf,0,4000);
  f_av->SetNpx(1000);
  f_av->Draw();
  f_av->GetXaxis()->SetRangeUser(hv_norm-400,hv_norm+100);
  f_av->GetYaxis()->SetRangeUser(0.1,4);
  f_av->GetXaxis()->SetTitle("HV (V)");
  f_av->GetYaxis()->SetTitle("gain (#times10^{6})");

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0011);
  TH1D *h_av = (TH1D*)f_av->GetHistogram()->Clone();
  h_av->SetName("average");
  h_av->SetStats(1);
  h_av->Fit("expo");
  double par_exp_av[2];
  for (int i=0; i<2; i++) par_exp_av[i] = expo->GetParameter(i);
  c3->Update();
  TPaveStats *st = (TPaveStats*)h_av->FindObject("stats");
  st->SetX1NDC(0.53);
  st->SetX2NDC(0.83);
  st->SetY1NDC(0.275);
  st->SetY2NDC(0.415);
  f_av->Draw();
  st->Draw();
  TText *t_id = new TText(0.18,0.82,id);
  t_id->SetNDC(1);
  t_id->Draw();
#if SAVE
  c3->Print(file_out.c_str(),"Title:gain(HV) average");
#endif
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(1);

  c3->SetLogy(0);
  double ch[Nch], par_exp[Nch][2];
  for (int c=0; c<Nch; c++) {
    ch[c] = c;
    par_exp[c][0] = f_gain[c]->GetParameter(0);
    par_exp[c][1] = f_gain[c]->GetParameter(1);
  }
  double gain0[2][Nch], gain_max, gain_min, gain_ratio[2];
  for (int i=0; i<2; i++) {
    int hv0 = (int)f_av->GetX(gain_av[i]);
    gain_max = 0; gain_min = 1e3;
    for (int c=0; c<Nch; c++) {
      gain0[i][c] = exp(par_exp[c][0]+par_exp[c][1]*hv0);
      if (gain0[i][c] > gain_max) gain_max = gain0[i][c];
      if (gain0[i][c] < gain_min) gain_min = gain0[i][c];
    }
    gain_ratio[i] = gain_max/gain_min;
    TGraph *g_gain0 = new TGraph(Nch,ch,gain0[i]);
    g_gain0->SetMarkerStyle(20);
    g_gain0->SetMinimum(0);
    g_gain0->SetMaximum(gain_av[i]*3);
    g_gain0->Draw("AP");
    g_gain0->GetXaxis()->SetTitle("channel");
    g_gain0->GetYaxis()->SetTitle("gain (#times10^{6})");
    sprintf(title,"%s, %.1f#times10^{6}, %d V",id,gain_av[i],hv0);
    g_gain0->SetTitle(title);
#if SAVE
    sprintf(thumbnail,"Title:gain at %.1fe6",gain_av[i]);
    c3->Print(file_out.c_str(),thumbnail);
#endif
  }

  DrawSquareCanvas("c4");
  for (int i=0; i<2; i++) {
    TH2D *h_gain = new TH2D("gain2d",fname,4,0,4,4,0,4);
    for (int c=0; c<Nch; c++) {
      h_gain->SetBinContent(4-c%4,4-int(c/4),gain0[i][c]);
    }
    for (int b=0; b<(int)sqrt(Nch); b++) {
      sprintf(buf,"%d",sqrt(Nch)-b-1);
      h_gain->GetXaxis()->SetBinLabel(b+1,buf);
      h_gain->GetYaxis()->SetBinLabel(b+1,buf);
    }
    h_gain->GetXaxis()->SetLabelSize(0.09);
    h_gain->GetYaxis()->SetLabelSize(0.09);
    h_gain->SetMinimum(0);
    h_gain->SetMaximum(gain_av[i]*2);
    h_gain->SetContour(99);
    h_gain->SetStats(0);
    int hv0 = (int)f_av->GetX(gain_av[i]);
    sprintf(title,"%s, %.1f#times10^{6}, %d V",id,gain_av[i],hv0);
    h_gain->SetTitle(title);
    h_gain->Draw("colz");
#if SAVE
    sprintf(thumbnail,"Title:gain at %.1fe6 (2D)",gain_av[i]);
    if (i==1) c4->Print((file_out+string(")")).c_str(),thumbnail);
    else      c4->Print(file_out.c_str(),thumbnail);
#endif
  }

  string input;
  cout << "OK? (yes/no): " << flush;
  if (automode) {
    input = "yes"; cout << "yes" << endl;
  } else {
    cin >> input;
  }
  if (input!="yes") return;

  /*#if SAVE
//  if (gSystem->Exec((string("ls ")+="fig/gain/"+file_out+" > /dev/null").c_str()))
  {
    sprintf(buf,"(mv %s fig/gain/)",file_out.c_str());
    gSystem->Exec(buf);
    string ssh_dir = " matsuoka.nagoya@hepws-9:~/public_html/top/file/gain/";
    gSystem->Exec((string("scp -p fig/gain/")+=file_out+ssh_dir).c_str());
  }
#endif

#if DATABASE
  TMySQLServer *db = new TMySQLServer("mysql://192.168.6.41:3306/top","guest","hogehoge");
  TSQLResult *res;
  TSQLRow *row;
  char sql[256]="";

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
    if (!strcmp(row->GetField(0),file_gain)) {
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
    sprintf(sql,"UPDATE laser_test SET gain_const='%.2f', gain_slope='%.5f', gain_range='%.2f' WHERE id='%s' AND Nlaser='%d'",
            par_exp_av[0],par_exp_av[1],gain_ratio[1],id,Nlaser+1);
    res = db->Query(sql); delete res;
    sprintf(sql,"SELECT ch FROM laser_ana WHERE id='%s' AND Nlaser='%d'",id,Nlaser+1);
    res = db->Query(sql);
    bool update_gain = res->GetRowCount();
    delete res;
    if (update_gain) {
      for (int c=0; c<Nch; c++) {
        sprintf(sql,"UPDATE laser_ana SET gain_const='%.2f', gain_slope='%.5f' WHERE id='%s' AND Nlaser='%d' AND ch='%d'",
                par_exp[c][0],par_exp[c][1],id,Nlaser+1,c);
        res = db->Query(sql); delete res;
        for (int i=0; i<Nhv; i++) {
          sprintf(sql,"UPDATE laser_data SET gain='%.2f' WHERE id='%s' AND Nlaser='%d' AND hv='%d' AND ch='%d'",
                  gain[c][i],id,Nlaser+1,hv[i],c);
//          sprintf(sql,"INSERT INTO laser_data (id,Nlaser,hv,ch,gain) VALUES('%s','%d','%d','%d','%.2f')",
//                  id,Nlaser+1,hv[i],c,gain[c][i]);
          res = db->Query(sql); delete res;
        }
      }
    } else {
      for (int c=0; c<Nch; c++) {
        if (!par_exp[c][1]) continue;
        sprintf(sql,"INSERT INTO laser_ana (id,Nlaser,ch,gain_const,gain_slope) VALUES('%s','%d','%d','%.2f','%.5f')",
                id,Nlaser+1,c,par_exp[c][0],par_exp[c][1]);
        res = db->Query(sql); delete res;
        for (int i=0; i<Nhv; i++) {
          if (!gain[c][i]) continue;
          sprintf(sql,"INSERT INTO laser_data (id,Nlaser,hv,ch,gain) VALUES('%s','%d','%d','%d','%.2f')",
                  id,Nlaser+1,hv[i],c,gain[c][i]);
          res = db->Query(sql); delete res;
        }
      }
    }

  } else {
    sprintf(sql,"INSERT INTO laser_test (id,Nlaser,date,root_laser,plot_gain,gain_const,gain_slope,gain_range) VALUES('%s','%d','%s','%s','%s','%.2f','%.5f','%.2f')",
            id,Nlaser+1,date,file_gain,file_out.c_str(),par_exp_av[0],par_exp_av[1],gain_ratio[1]);
    res = db->Query(sql); delete res;
    for (int c=0; c<Nch; c++) {
      if (!par_exp[c][1]) continue;
      sprintf(sql,"INSERT INTO laser_ana (id,Nlaser,ch,gain_const,gain_slope) VALUES('%s','%d','%d','%.2f','%.5f')",
              id,Nlaser+1,c,par_exp[c][0],par_exp[c][1]);
      res = db->Query(sql); delete res;
      for (int i=0; i<Nhv; i++) {
        if (!gain[c][i]) continue;
        sprintf(sql,"INSERT INTO laser_data (id,Nlaser,hv,ch,gain) VALUES('%s','%d','%d','%d','%.2f')",
                id,Nlaser+1,hv[i],c,gain[c][i]);
        res = db->Query(sql); delete res;
      }
    }
  }

  delete db;
#endif
  */
  cout << file_gain << endl;

//  if (automode) gApplication->Terminate();

  return;
}
