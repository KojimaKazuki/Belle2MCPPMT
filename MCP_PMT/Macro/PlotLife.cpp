#include "TMySQLServer.h"

void PlotLife(const char *file1=0, const char *file2=0, const char *file3=0, const char *file4=0, const char *file5=0, Option_t *option="same", int wavelength=400)
{
  if (!file1) {
    cout << "Usage: PlotLife(char *file1, char *file2, char *file3, char *file4, char *file5, Option_t *option)" << endl;
    return;
  }

  const int maxfile = 5;
  const double thr_charge = 0.05;
  const double correction_charge = (22.5*22.5)/(23.0*23.0);

  TH2D *h;
  //cout << strcmp(option,"same") << endl;
  //break;
  if (!strcmp(option,"same")) {
    //if (!strstr(option,"same")) {
    //if (gROOT->FindObject("h")&&h) h->Delete();
    h = new TH2D("h","",550,0,5,120,0.,1.2);
    h->GetXaxis()->SetTitle("accumulated output charge (C/cm^{2})");
    h->GetYaxis()->SetTitle("relative QE");
    h->Draw("same");
  }

  TF1* func = new TF1("f","([0]*(1-0.2*((x-[1])/([2]-[1]))**2))*(x>[1])+[0]*(x<=[1])",0,30);
  func->SetParNames("QE0","Q0","life");
  func->SetParameters(1.,0.,4.);
  func->SetParLimits(1,0,100);
  func->SetParLimits(2,0,100);

  TGraph *g_qe = new TGraph();
  g_qe->GetXaxis()->SetTitle("accumulated output charge (C/cm^{2})");
  g_qe->GetYaxis()->SetTitle("relative QE");
  g_qe->SetMarkerStyle(20);
  g_qe->SetMinimum(0);
  g_qe->SetMaximum(1.2);

  char filename[maxfile][256] = {"","","","",""};
  if (file1) sprintf(filename[0],"%s",file1);
  if (file2) sprintf(filename[1],"%s",file2);
  if (file3) sprintf(filename[2],"%s",file3);
  if (file4) sprintf(filename[3],"%s",file4);
  if (file5) sprintf(filename[4],"%s",file5);
  string dir = "/chai/sgt1/bfactory/mcppmt_data/life/dataset/";
  vector<string> file;
  char date_life[maxfile][32]={0}, date_life0[32]={0};

  if (!strcmp(filename[0],"XM0205")) {
    file.push_back("life13/summary/revised/XM0205.dat");
    for (int i=0; i<1; i++) file[i] = dir+file[i];
  } else if (!strcmp(filename[0],"JT0940")) { //before low gain lifetime test
    file.push_back("life01/summary/revised/JT0940.dat");
    for (int i=0; i<1; i++) file[i] = dir+file[i];
  }else if(!strcmp(filename[0],"JT0940_co")) { //after low gain lifetime test
    file.push_back("life01/summary/revised/JT0940.dat");
    file.push_back("life16/summary/revised/JT0940_co.dat");
    for (int i=0; i<2; i++) file[i] = dir+file[i];
  } else if(!strcmp(filename[0],"JT0898")){
    file.push_back("old_data/lowgain/summary_plots/revised/JT0898.dat");
    //file.push_back("life16/summary/revised/JT0898.dat");
    for (int i=0; i<1; i++) file[i] = dir+file[i];
  } else if(!strcmp(filename[0],"JT0936")){
    file.push_back("old_data/lowgain/summary_plots/revised/JT0936.dat");
    //file.push_back("life17/summary/revised/JT0936.dat");
    for (int i=0; i<1; i++) file[i] = dir+file[i];
  }else if (!strcmp(filename[0],"XM0267")) {
    file.push_back("life1/summary_plots/revised/XM0267.dat");
    for (int i=0; i<1; i++) file[i] = dir+file[i];
  } else if (!strcmp(filename[0],"XM0376")) {
    file.push_back("life1/summary_plots/revised/XM0376.dat");
    file.push_back("life01/summary/revised/XM0376.dat");
    for (int i=0; i<2; i++) file[i] = dir+file[i];
  } else if (!strcmp(filename[0],"JT0453")) {
    g_qe->SetPoint(0,0,1);
    if (wavelength==400) g_qe->SetPoint(1,2.7,19.1/20.0);
    if (wavelength==660) g_qe->SetPoint(1,2.7,0.9/1.2);
    g_qe->Draw("P");
    func->FixParameter(0,1);
    func->FixParameter(1,0);
    g_qe->Fit("f","I");
    return;
  } else if (!strcmp(filename[0],"XM0142")) {
    file.push_back("specialMCP/summary_plots/revised/XM0142.dat");
    file.push_back("ALD2/summary_plots/revised/XM0142.dat");
    sprintf(date_life0,"20130100");
    sprintf(date_life[0],"20130200");
    for (int i=0; i<2; i++) file[i] = dir+file[i];
  } else if (!strcmp(filename[0],"XM0239")) {
    file.push_back("ALD2/summary_plots/revised/XM0239.dat");
    for (int i=0; i<1; i++) file[i] = dir+file[i];
  } else if (!strcmp(filename[0],"XM0240")) {
    file.push_back("specialMCP/summary_plots/revised/XM0240.dat");
    file.push_back("ALD2/summary_plots/revised/XM0240.dat");
    sprintf(date_life0,"20130100");
    sprintf(date_life[0],"20130200");
    for (int i=0; i<2; i++) file[i] = dir+file[i];
  } else if (!strcmp(filename[0],"KT0033")) {
    file.push_back("life13/summary/revised/KT0033.dat");
    //file.push_back("life16/summary/revised/KT0033.dat");
    //file.push_back("life17/summary/revised/KT0033.dat");
    for (int i=0; i<1; i++) file[i] = dir+file[i];
  } else if (!strcmp(filename[0],"KT0028")) {
    file.push_back("life13/summary/revised/KT0028.dat");
    file.push_back("life14/summary/revised/KT0028.dat");
    for (int i=0; i<2; i++) file[i] = dir+file[i];
  } else if (!strcmp(filename[0],"KT0074")) {
    file.push_back("life1/summary_plots/revised/KT0074.dat");
    file.push_back("life01/summary/KT0074.dat");
    file.push_back("life04/summary/KT0074.dat");
    file.push_back("life05/summary/KT0074.dat");
    file.push_back("life06/summary/KT0074.dat");
    //file.push_back("life16/summary/revised/KT0074_co_re.dat");
    //file.push_back("life17/summary/revised/KT0074_re.dat");
    for (int i=0; i<5; i++) file[i] = dir+file[i];
  } else if (!strcmp(filename[0],"KT0139")) {
    file.push_back("life1/summary_plots/revised/KT0139.dat");
    file.push_back("life01/summary/revised/KT0139.dat");
    file.push_back("life04/summary/KT0139.dat");
    file.push_back("life05/summary/KT0139.dat");
    for (int i=0; i<4; i++) file[i] = dir+file[i];
  } else if (!strcmp(filename[0],"YH0203")) {
    file.push_back("life10/summary/revised/YH0203.dat");
    file.push_back("life11/summary/revised/YH0203.dat");
    file.push_back("life12/summary/revised/YH0203.dat");
    file.push_back("life13/summary/revised/YH0203.dat");
    file.push_back("life14/summary/revised/YH0203.dat");
    //file.push_back("life16/summary/revised/YH0203_re.dat");
    //file.push_back("life17/summary/revised/YH0203_re.dat");
    for (int i=0; i<5; i++) file[i] = dir+file[i];
  } else if (!strcmp(filename[0],"YH0205")) {
    file.push_back("life09/summary/revised/YH0205.dat");
    file.push_back("life10/summary/revised/YH0205.dat");
    file.push_back("life11/summary/revised/YH0205.dat");
    file.push_back("life13/summary/revised/YH0205.dat");
    file.push_back("life14/summary/revised/YH0205.dat");
    file.push_back("life21/summary/revised/YH0205.dat");
    for (int i=0; i<6; i++) file[i] = dir+file[i];
  } else if (!strcmp(filename[0],"YH0206")) {
    file.push_back("life10/summary/revised/YH0206.dat");
    file.push_back("life11/summary/revised/YH0206.dat");
    file.push_back("life12/summary/revised/YH0206.dat");
    file.push_back("life13/summary/revised/YH0206.dat");
    file.push_back("life14/summary/revised/YH0206.dat");
    //file.push_back("life16/summary/revised/YH0206_co.dat");
    //file.push_back("life17/summary/revised/YH0206.dat");
    for (int i=0; i<5; i++) file[i] = dir+file[i];
  } else if (!strcmp(filename[0],"YH0208")) {
    file.push_back("life13/summary/revised/YH0208.dat");
    file.push_back("life14/summary/revised/YH0208.dat");
    for (int i=0; i<2; i++) file[i] = dir+file[i];
  } else if (!strcmp(filename[0],"KT0488")) {
    file.push_back("life12/summary/revised/KT0488.dat");
    for (int i=0; i<1; i++) file[i] = dir+file[i];
  } else if (!strcmp(filename[0],"KT0487")) {
    file.push_back("life12/summary/revised/KT0487.dat");
    for (int i=0; i<1; i++) file[i] = dir+file[i];
  } else if (!strcmp(filename[0],"KT0492")) {
    file.push_back("life12/summary/revised/KT0492.dat");
    for (int i=0; i<1; i++) file[i] = dir+file[i];
  } else if (!strcmp(filename[0],"KT0482")) {
    file.push_back("life12/summary/revised/KT0482.dat");
    for (int i=0; i<1; i++) file[i] = dir+file[i];
  } else if (!strcmp(filename[0],"KT0489")) {
    file.push_back("life12/summary/revised/KT0489.dat");
    for (int i=0; i<1; i++) file[i] = dir+file[i];
  } else if (!strcmp(filename[0],"KT0491")) {
    file.push_back("life12/summary/revised/KT0491.dat");
    for (int i=0; i<1; i++) file[i] = dir+file[i];
  }else if(!strcmp(filename[0],"KT0642")){ 
    file.push_back("life16/summary/revised/KT0642.dat");
    for (int i=0; i<1; i++) file[i] = dir+file[i];
  }else if(!strcmp(filename[0],"KT0647")){ 
    file.push_back("life17/summary/revised/KT0647.dat");
    for (int i=0; i<1; i++) file[i] = dir+file[i];
  }else if(!strcmp(filename[0],"YH0274")){
    file.push_back("life19/summary/revised/YH0274.dat");
    for (int i=0; i<1; i++) file[i] = dir+file[i];
  }else if(!strcmp(filename[0],"YH0275")){
    //file.push_back("life19/summary/revised/YH0275_col.dat");
    file.push_back("life19/summary/revised/YH0275.dat");
    for (int i=0; i<1; i++) file[i] = dir+file[i];
  }else if(!strcmp(filename[0],"YH0276")){
    //file.push_back("life19/summary/revised/YH0276_col.dat");
    file.push_back("life19/summary/revised/YH0276.dat");
    file.push_back("life20/summary/revised/YH0276.dat");
    for (int i=0; i<2; i++) file[i] = dir+file[i];
  }else if(!strcmp(filename[0],"YH0284")){
    file.push_back("life20/summary/revised/YH0284.dat");
    for (int i=0; i<1; i++) file[i] = dir+file[i];
  }else if(!strcmp(filename[0],"YH0285")){
    file.push_back("life20/summary/revised/YH0285.dat");
    for (int i=0; i<1; i++) file[i] = dir+file[i];
  }else if(!strcmp(filename[0],"JT0695")){
    file.push_back("life21/summary/revised/JT0695.dat");
    for (int i=0; i<1; i++) file[i] = dir+file[i];
  }else {
    file.push_back(filename[0]);
    if (filename[1]) file.push_back(filename[1]);
    if (filename[2]) file.push_back(filename[2]);
    if (filename[3]) file.push_back(filename[3]);
    if (filename[4]) file.push_back(filename[4]);
}

  char fname[1024], buf[256];
  string str;
  double charge[4095]={0}, qe[4095]={0}, err_charge[4095]={0}, err_qe[4095]={0},
         gain[4095]={0}, err_gain[4095];
  charge[0] = 0; qe[0] = 1; err_charge[0] = 0; err_qe[0] = 0;
  double charge0;
  double range_fit=100;
  double par[maxfile][3];
  double offset[maxfile];
  for (int i=0; i<maxfile; i++) offset[i] = 0;

  sprintf(buf,"test -f %s",file[0].c_str());
  if (gSystem->Exec(buf)) {
    cout << file[0] << " does not exist" << endl;
    return;
  }

  char id[256];
  sprintf(id,"%s",file[0].c_str());
  strtok(id,"/");
  while (1) {
    sprintf(id,"%s",strtok(NULL,"/"));
    if (strstr(id,".dat")) break;
  }
  strtok(id,".");
  h->SetTitle(id);
  g_qe->SetTitle(id);

  if (date_life0[0]==0) {
    str = file[0];
    str = str.substr(0,str.find("/summary"));
    sprintf(buf,"test -d %s/data/laser",str.c_str());
    if (!gSystem->Exec(buf)) str += "/data/laser/lifetime_laser*.root";
    else                     str += "/data/basic/lifetime_basic*.root";
    sprintf(buf,"date +%%Y%%m%%d -r `ls -t %s | tail -1 ` > out.log",str.c_str());
    gSystem->Exec(buf);
    ifstream fin("out.log");
    fin >> date_life0;
    fin.close();
  }

  int np=0;
  int np0[maxfile+1]={0};
  bool nogain;
  int nfile;
  double charge_p=0;
  np0[0] = -1;
  for (nfile=0; nfile<maxfile; nfile++) {
    if (nfile==1 && file.size()<2) break;
    if (nfile==2 && file.size()<3) break;
    if (nfile==3 && file.size()<4) break;
    if (nfile==4 && file.size()<5) break;
    sprintf(fname,"%s",file[nfile].c_str());
//    sprintf(buf,"date +%%Y%%m%%d -r %s > out.log",file[nfile].c_str());
    str = file[nfile];
    sprintf(buf,"test -f %s",fname);
    if (gSystem->Exec(buf)) {
      cout << fname << " does not exist" << endl;
      return;
    }
    if (date_life[nfile][0]==0) {
      str = str.substr(0,str.find("/summary"));
      sprintf(buf,"test -d %s/data/laser",str.c_str());
      if (!gSystem->Exec(buf)) str += "/data/laser/lifetime_laser*.root";
      else                     str += "/data/basic/lifetime_basic*.root";
      sprintf(buf,"date +%%Y%%m%%d -r `ls -rt %s | tail -1 ` > out.log",str.c_str());
      gSystem->Exec(buf);
      ifstream fin("out.log");
      fin >> date_life[nfile];
      fin.close();
    }
    nogain = (strstr(fname,"life01")!=NULL) ||
             (strstr(fname,"life04")!=NULL) ||
             (strstr(fname,"life05")!=NULL) ||
             (strstr(fname,"old_data")!=NULL) ;
    

    ifstream fin(fname);
    int i=0;
    while (!fin.eof()) {
      if (np>=4095) {
        cout << "Reached the maximum number of points 4095" << endl;
        break;
      }
      if (nogain) {
        fin >> charge[np] >> qe[np] >> err_charge[np] >> err_qe[np];
	/*cout << charge[np] << " " << qe[np] << " " << err_charge[np] << " " << err_qe[np] << endl; 
	  cout << np << endl;*/
      } else {
        fin >> charge[np] >> qe[np] >> err_charge[np] >> err_qe[np]
             >> str >> gain[np] >> err_charge[np] >> err_gain[np];
      }
      charge[np] *= 1.e-3*correction_charge;
      err_charge[np] *= 1.e-3*correction_charge;
      
      if (charge[np]>0 && qe[np]>0) {
      	if (i==0) {
	  if (nfile==0) charge0 = charge[np];
          charge_p = charge[np];
	}
        if (charge[np]>charge_p+thr_charge) {
	//if (charge[np]>charge_p) {
          charge_p = charge[np];
          if (np>0) {
            if (qe[np-1]>=0.7 && qe[np]<0.7) range_fit = charge[np];
          }
	  np++;

        } else {
          charge_p = charge[np];
	}
      }
      i++;
    }
    fin.close();
    np0[nfile+1] = np-1;
  }
    gSystem->Exec("rm out.log");

  char date_qe[32];
  double qe_abs[maxfile+1]={0}, qe0_abs, qe_f[maxfile]={0};
  int nqe=0;
  //TMySQLServer *db = new TMySQLServer("mysql://192.168.6.41:3306/top","ana","hogehogehoge");
  TMySQLServer *db = new TMySQLServer("mysql://192.168.6.41:3306/top","guest","hogehoge");
  TSQLResult *res;
  TSQLRow *row;
  char sql[256];
  sprintf(sql,"SELECT DATE_FORMAT(date,'%%Y%%m%%d'),qe_%d FROM qe_test WHERE id='%s'",wavelength,id);
  res = db->Query(sql);
  int nrow = res->GetRowCount();
  row = res->Next();
  if (!row) { delete row; delete res; return;}
  cout << "0: " << row->GetField(0) << endl;
  qe0_abs = atof(row->GetField(1));
//  g_qe->SetPoint(0,0,1); nqe++;
  while (1) {
    row = res->Next();
    if (!row) { delete row; delete res; break;}
    sprintf(date_qe,"%s",row->GetField(0));
    if (strcmp(date_life0,date_qe)>=0 && charge0 > thr_charge) {
      qe_abs[nqe] = atof(row->GetField(1))/qe0_abs;
      g_qe->SetPoint(nqe,charge0,qe_abs[nqe]);
      cout << "0: " << date_qe << endl;
    } else {
      if (g_qe->GetN()) nqe++;
      break;
    }
  }
  for (int i=0; i<nfile; i++) {
    while (1) {
      if (!row) break;
      sprintf(date_qe,"%s",row->GetField(0));
      qe_abs[nqe] = atof(row->GetField(1))/qe0_abs;
      if (strcmp(date_life[i],date_qe)<=0) {
        if (i<nfile-1 && strcmp(date_life[i+1],date_qe)<=0) break;
        g_qe->SetPoint(nqe,charge[np0[i+1]],qe_abs[nqe]);
        qe_f[i] = qe_abs[nqe];
        nqe++;
        break;
      }
      row = res->Next();
    }
    cout << i+1 << ": " << date_qe << "  Q=" << charge[np0[i+1]] << "  QE=";
    if (qe_f[i]) cout << qe_f[i] << endl;
    else         cout << "-" << endl;
  }
  delete db;
  func->FixParameter(0,1);
  int ret_fit = g_qe->Fit("f","IQ0","",0,range_fit);
  if (wavelength==400) {
  g_qe->Fit("f","IQ0","",0,range_fit);
//    g_qe->Draw("P"); g_qe->Fit("f","I","",0,range_fit); c1->Update(); cin >> str;
  } else {
    g_qe->Draw("P");
    func->FixParameter(1,0);
    g_qe->Fit("f","I","",0,range_fit);
    return;
  }
  func->SetParLimits(0,0.8,1.2);
  double q0 = func->GetParameter(1);
  double life0 = func->GetParameter(2);
  func->SetParLimits(2,life0*0.9,life0*1.1);

  TGraphErrors *graph = new TGraphErrors(np,charge,qe,err_charge,err_qe);
  graph->SetMinimum(0);
  graph->SetMaximum(1.2);
  graph->SetTitle(file[0].c_str());
  graph->GetXaxis()->SetTitle("accumulated output charge (C/cm^{2})");
  graph->GetYaxis()->SetTitle("relative QE");
//  graph->Draw("Pl");

  for (int i=0; i<nfile; i++) {
    if (q0 < 0.5) {
      func->FixParameter(1,0);
    } else if (i==0) {
      func->SetParLimits(1,0.,charge[np0[i+1]]);
    } else {
      func->SetParLimits(1,par[i-1][1],charge[np0[i+1]]); // comment out?
    }
    graph->Fit("f","IQ0","",charge[np0[i]+1],charge[np0[i+1]]);
//    graph->Fit("f","I","",charge[np0[i]+1],charge[np0[i+1]]); c1->Update(); cin >> str;
    par[i][0] = func->GetParameter(0);
    par[i][1] = func->GetParameter(1);
    par[i][2] = func->GetParameter(2);
    if (i==0) {
      offset[i] = par[i][0] - 1;
    } else {
      offset[i] = (par[i][1]>charge[np0[i]+1]) ?
        par[i][0]-par[i-1][0] :
        (par[i][0]*(1-0.2*((charge[np0[i]+1]-par[i][1])/(par[i][2]-par[i][1]))*((charge[np0[i]+1]-par[i][1])/(par[i][2]-par[i][1]))))*(charge[np0[i]+1]>par[i][1])+par[i][0]*(charge[np0[i]+1]<=par[i][1])-
        (par[i-1][0]*(1-0.2*((charge[np0[i]+1]-par[i-1][1])/(par[i-1][2]-par[i-1][1]))*((charge[np0[i]+1]-par[i-1][1])/(par[i-1][2]-par[i-1][1]))))*(charge[np0[i]+1]>par[i-1][1])-par[i-1][0]*(charge[np0[i]+1]<=par[i-1][1]);
    }
    for (int n=np0[i]+1; n<=np0[i+1]; n++) {
      for (int j=0; j<i+1; j++) {
        if (ret_fit==0) qe[n] -= offset[j];
      }
    }
  }
  char datname[124];
  sprintf(datname,"../../charge_qe/%s_qe.dat",file1);
 
  ofstream fout_qe(datname);
  for(i=0;i<np;i++){
    fout_qe << charge[i] << " " << qe[i] << " " << err_charge[i] << " " << err_qe[i] << endl;
    cout << charge[i] << " " << qe[i] << " " << err_charge[i] << " " << err_qe[i] << endl;    
  }
  TGraphErrors *g_hit = new TGraphErrors(np,charge,qe,err_charge,err_qe);
  g_hit->SetMinimum(0);
  g_hit->SetMaximum(1.2);
  g_hit->SetTitle(file[0].c_str());
  g_hit->GetXaxis()->SetTitle("accumulated output charge (C/cm^{2})");
  g_hit->GetYaxis()->SetTitle("relative QE");
  g_hit->Draw("Pl");
  func->FixParameter(0,1);
  if (q0 >= 0.5) func->SetParLimits(1,0,100);
  func->SetParLimits(2,0,100);
  g_hit->Fit("f","I","",0,range_fit);
  //  if (!strstr(option,"0")) func->Draw("same");
  g_qe->Draw("P");

  /*func->Draw();
  func->SetMinimum(0);
  func->SetMaximum(1.5);
  */
}
