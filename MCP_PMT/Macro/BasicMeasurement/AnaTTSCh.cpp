#include "calib_tdc.h"
#include "DrawA4Canvas.cpp"

static int tdc0;
static double tdc_timewalk[100];
static double par[17];
static double diff_par[5], err_par[5];
static double adc_calib[9];

double AnaTTSCh(char fname[], int ch, int iHV, bool calibration=true, double *tts=0, double *err=0)
{
  const double laser = 17.; // laser pulse width (ps)

#define NAMP 2
#define NATT 9
  const int att[NATT] = {10, 8, 6, 5, 4, 3, 2, 1, 0};
  const int amp[2][3] = {{0, 1}, {0, 16, 30}};

#define NCH 16

  const int nbin_tdc = 80;
  const int tdc_min = -20;
  const int tdc_max = 60;

  gStyle->SetNdivisions(507,"XY");
  TFile *file = TFile::Open(fname);

  TList *hv_set = file->Get("hv_set");
  int nhv = hv_set->GetEntries();
  if (iHV>=nhv) {
    cout << "iHV must be less than " << nhv << endl;
    return 0;
  }

  char filename[256];
  sprintf(filename,"%s",fname);
  strtok(filename,"/");
  char dirname[256];
  sprintf(dirname,"%s",filename);
  while (1) {
    sprintf(filename,"%s",strtok(NULL,"/"));
    if (strstr(filename,".root")) break;
    sprintf(dirname,"%s/%s",dirname,filename);
  }
  char *date = strtok(filename,"_");
  date = strtok(NULL,".");
  if (strlen(date)>8) date = strtok(date,"_");

  bool amp_ctrl = true;
  int amp_ver = 0;
  int amp_low;
  if (strncmp(date,"20130501",8)<0) {
    amp_ctrl = false;
    amp_low = 0;
  } else if (strncmp(date,"20140401",8)<0) {
    if (!strncmp(filename,"KT",2) && nhv==7) {
      amp_low = (iHV<=3) ? 1 : 0;
    } else {
      amp_low = (iHV<=1) ? 1 : 0;
    }
  } else {
    amp_ver = 1;
    if (strncmp(date,"20140403",8)==0 && strncmp(filename,"KT0117",6)==0) {
        amp_low = (iHV<=2) ? 2 : 0;
    } else {
      if (!strncmp(filename,"KT",2) && nhv==7) {
        amp_low = (iHV<=1) ? 2 : ((iHV<=3) ? 1 : 0);
      } else {
        amp_low = (iHV<=1) ? 1 : 0;
      }
    }
  }

  int tdc_ver = 0;
  if (strncmp(date,"20140616",8)>=0) tdc_ver = 1;

  double par_time = par_tdc[tdc_ver][ch];
  double par_timewalk[5];
  for (int i=0; i<5; i++) par_timewalk[i] = par_timewalk0[amp_ver][ch][i];

  bool bad_cable_ch2 = false;
  if (strncmp(date,"20130812",8)>=0 && strncmp(date,"20131120",8)<0)
    bad_cable_ch2 = true;

  bool bad_calib_ch3_amp1_10db = false;
  if (strncmp(date,"20130904",8)>=0 && ch==3 && amp_ver==0 && amp_low==1)
    bad_calib_ch3_amp1_10db = true;

  TTree *mcp = (TTree*)file->Get("mcp");
  TTree *calib = (TTree*)file->Get("calib");

  char exp[1024], cut[256], cut0[256];
  char name[64], title[256];

  TCanvas *c1 = (TCanvas*)gROOT->FindObject("c1");
  if (!c1) {
    if (calibration) {
      c1 = new TCanvas("c1","c1");
    } else {
      DrawA4Canvas("c1");
      c1 = (TCanvas*)gROOT->FindObject("c1");
      c1->Divide(2,7);
    }
  } else if (iHV==0) {
    c1->Clear();
    c1->Divide(2,7);
  }

  TCanvas *c0[4];
  for (int i=0; i<4; i++) {
    sprintf(name,"c0%d",i);
    c0[i] = (TCanvas*)gROOT->FindObject(name);
    if (!c0[i]) {
      c0[i] = new TCanvas(name,name);
      c0[i]->Divide(2,2);
    }
  }

if (calibration) {

  c1->cd(1);
  TH1D *h_tdc[2];
  sprintf(exp,"tdc[%d]>>tdc(1200,1400,2600)",ch);
  sprintf(cut,"cycle==0&&att==%d&&amp==0",att[NATT-1]);
  calib->Draw(exp,cut);
  TH1D *h_tdc[0] = (TH1D*)gDirectory->FindObject("tdc");
  h_tdc[0]->GetXaxis()->SetNdivisions(507);
  h_tdc[0]->GetXaxis()->SetTitle("TDC (count)");
  sprintf(exp,"tdc[%d]",ch);
  sprintf(cut,"cycle==1&&att==%d&&amp==0",att[NATT-1]);
  calib->Draw(exp,cut,"same");
  TH1D *h_tdc[1] = (TH1D*)gPad->FindObject("htemp");
  h_tdc[1]->SetLineColor(2);

  c1->cd(2);
  double step = h_tdc[0]->GetRMS()*0.6;
  double start = h_tdc[0]->GetMean()-step*3;
  double dt[2][5], err_dt[2][5], tdc[2][5], err_tdc[2][5], sigma_tdc[2][5];
  TGraphErrors *g_tdc[2];
  for (int i=0; i<2; i++) {
    for (int j=0; j<6; j++) {
      cout << "Fit >>> " << start+j*step << " " << start+(j+1)*step << endl;
      h_tdc[i]->Fit("gaus","0N","",start+(j+1/4.)*step,start+(j+3/4.)*step);
      dt[i][j] = 200/65535.*0x333*j;
      err_dt[i][j] = 0;
      tdc[i][j] = (TF1*)(gROOT->FindObject("gaus"))->GetParameter(1);
      err_tdc[i][j] = (TF1*)(gROOT->FindObject("gaus"))->GetParError(1);
      sigma_tdc[i][j] = (TF1*)(gROOT->FindObject("gaus"))->GetParameter(2);
      cout << endl;
    }
    g_tdc[i] = new TGraphErrors(6,dt[i],tdc[i],err_dt[i],err_tdc[i]);
    g_tdc[i]->GetXaxis()->SetTitle("#Deltat (ns)");
    g_tdc[i]->GetYaxis()->SetTitle("TDC");
    g_tdc[i]->GetYaxis()->SetTitleOffset(1.2);
  }
  g_tdc[0]->SetMarkerStyle(20);
  g_tdc[0]->SetMarkerColor(602);
  g_tdc[0]->SetLineColor(602);
  g_tdc[0]->Draw("AP");
  g_tdc[1]->SetMarkerStyle(25);
  g_tdc[1]->SetMarkerColor(2);
  g_tdc[1]->SetLineColor(2);
  g_tdc[1]->Draw("P");

  TF1 *f_tdc[2];
  double tdc2ps[2];
  char TDCVal[64];
  TText *t_tdc[2];
  gStyle->SetOptFit(1);
  TPaveStats *st_tdc[2];
  for (int i=0; i<2; i++) {
    sprintf(name,"f_tdc%d",i+1);
    f_tdc[i] = new TF1(name,"[0]+[1]*x",0,11);
    f_tdc[i]->SetLineWidth(1);
    if (i==0) f_tdc[i]->SetLineColor(602);
    else      f_tdc[i]->SetLineColor(2);
    g_tdc[i]->Fit(name);
    tdc2ps[i] = 1000./f_tdc[i]->GetParameter(1);
    sprintf(TDCVal,"%.2f ps/count",tdc2ps[i]);
    t_tdc[i] = new TText(0.55,0.3+i*0.08,TDCVal);
    if (i==0) t_tdc[i]->SetTextColor(602);
    else      t_tdc[i]->SetTextColor(2);
    t_tdc[i]->SetTextSize(0.06);
    t_tdc[i]->SetNDC(1);
    t_tdc[i]->Draw();
    c1->Update();
    st_tdc[i] = (TPaveStats*)g_tdc[i]->FindObject("stats");
    st_tdc[i]->SetX1NDC(0.2);
    st_tdc[i]->SetX2NDC(0.55);
    st_tdc[i]->SetY1NDC(0.65);
    st_tdc[i]->SetY2NDC(0.85);
    st_tdc[i]->Draw();
  }

  c1->cd(1);
  char SigmaVal[64];
  sprintf(SigmaVal,"#sigma = %.1f ps",sigma_tdc[1][0]*tdc2ps[1]);
  TLatex *t_sigma = new TLatex(0.26,0.02,SigmaVal);
  t_sigma->SetNDC(1);
  t_sigma->SetTextColor(2);
  t_sigma->SetTextSize(0.06);
  t_sigma->Draw();

  par_time = tdc2ps[0];

} // if (calibration)

  if (!mcp) return 0;

  char *id, *buf = mcp->GetTitle();
  buf = strtok(buf," ");
  while (1) {
    if ((buf = strtok(NULL," ")) == NULL) break;
    id = buf;
  }

  c1->cd(iHV*2+1);
  int hv = ((TParameter<int>*)hv_set->At(iHV))->GetVal();
  sprintf(title,"%s (ch%d, %d V)",id,ch,hv);
  c1->SetTitle(title);

  gStyle->SetOptFit(1);
  gStyle->SetOptStat(1);

  ostringstream alias;
  alias << "((tdc[0]>0&&tdc[0]<4000)";
  for (int c=1; c<16; c++) {
    alias << "+(tdc[" << c << "]>0&&tdc[" << c << "]<4000)";
  }
  alias << ")";
  mcp->SetAlias("nhit",alias.str().c_str());
  sprintf(cut0,"tdc[%d]>0&&tdc[%d]<4000&&nhit==1&&ipos==%d&&hv==%d",
          ch,ch,ch,hv);

  sprintf(exp,"adc[%d]>>ped(200,0,200)",ch);
  sprintf(cut,"tdc[%d]>4000&&ipos==%d&&hv==%d",ch,ch,hv);
  mcp->Draw(exp,cut,"goff");
  TH1D *h_ped = (TH1D*)gROOT->FindObject("ped");
  h_ped->Fit("gaus","NQ");
  double ped = (TF1*)(gROOT->FindObject("gaus"))->GetParameter(1);

  TSpectrum spec;
  if (iHV==0) {
    sprintf(exp,"tdc[%d]>>tdc(4000,0,4000)",ch);
    sprintf(cut,"tdc[%d]>0&&tdc[%d]<4000&&nhit==1&&ipos==%d",ch,ch,ch);
    mcp->Draw(exp,cut,"goff");
    TH1D *h_tdc0 = (TH1D*)gROOT->FindObject("tdc");
    spec.Search(h_tdc0);
    tdc0 = (int)spec.GetPositionX()[0];
  }

  sprintf(name,"ch%d_%dV_TDC",ch,hv);
  sprintf(exp,"tdc[%d]>>%s(%d,%d,%d)",
          ch,name,nbin_tdc,tdc0+tdc_min,tdc0+tdc_max);
  mcp->Draw(exp,cut0);
  TH1D *h_tdc = (TH1D*)gROOT->FindObject(name);
  h_tdc->GetXaxis()->SetTitle("TDC (count)");

if (iHV==0) {
  c0[int(ch/4)]->cd(ch%4+1);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);
/*
  if (amp_ver==0) {
    sprintf(exp,"adc[%d]-%d>>adc(100,0,100)",ch,int(ped));
    sprintf(cut,"tdc[%d]>0&&tdc[%d]<4000&&nhit==1&&ipos==%d",ch,ch,ch);
    mcp->Draw(exp,cut,"goff");
    TH1D *h_adc = (TH1D*)gROOT->FindObject("adc");
    int x = (int)h_adc->GetMean();
    sprintf(exp,"tdc[%d]>>tdc(4000,0,4000)",ch);
    sprintf(cut,"abs(adc[%d]-%d)<2&&%s",ch,int(ped)+x,cut);
    mcp->Draw(exp,cut,"goff");
    TH1D *h_tdc = (TH1D*)gROOT->FindObject("tdc");
    h_tdc->Fit("gaus","QN","",
               h_tdc->GetMaximumBin()-5,h_tdc->GetMaximumBin()+5);
    diff_par[2] = (TF1*)(gROOT->FindObject("gaus"))->GetParameter(1)
                -((par_timewalk[0]/sqrt(x-par_timewalk[1])+par_timewalk[2])
                   *(x>par_timewalk[1])
                  +(par_timewalk[3]*(x-par_timewalk[4]))*(x>par_timewalk[4]));
    err_par[2] = (TF1*)(gROOT->FindObject("gaus"))->GetParError(1);
  }
*/

  TF1 *f_timewalk;
  sprintf(name,"timewalk%02d",ch);
  if (amp_ver==0) {
    f_timewalk = new TF1(name,"([0]/sqrt(x-[1])+[2])*(x>[1])+([3]*(x-[4]))*(x>[4])",0,200);
    f_timewalk->SetParNames("A","x_{0}","TDC_{0}","Slope","x_{1}");
    f_timewalk->FixParameter(0,par_timewalk[0]);
//    f_timewalk->SetParameter(0,par_timewalk[0]);
    f_timewalk->SetParameter(1,par_timewalk[1]);
//    f_timewalk->FixParameter(2,par_timewalk[2]+diff_par[2]);
    f_timewalk->SetParameter(2,par_timewalk[2]);
    f_timewalk->FixParameter(3,par_timewalk[3]);
    f_timewalk->FixParameter(4,par_timewalk[4]);
  } else {
    f_timewalk = new TF1(name,"([0]/sqrt(x-[1])+[2])*(x<=10)+([0]/sqrt(10-[1])+[2]+[3]*(x-10)+[4]*(x-10)**2+[5]*(x-10)**3+[6]*(x-10)**4+[7]*(x-10)**5+[8]*(x-10)**6+[9]*(x-10)**7+[10]*(x-10)**8+[11]*(x-10)**9)*(x>10&&x<=40)+([0]/sqrt(10-[1])+[2]+[3]*(40-10)+[4]*(40-10)**2+[5]*(40-10)**3+[6]*(40-10)**4+[7]*(40-10)**5+[8]*(40-10)**6+[9]*(40-10)**7+[10]*(40-10)**8+[11]*(40-10)**9+[12]*(x-40)+[13]*(x-40)**2+[14]*(x-40)**3+[15]*(x-40)**4+[16]*(x-40)**5)*(x>40)",0,100);
    f_timewalk->SetParNames("A","x_{0}","TDC_{0}");
    f_timewalk->SetParLimits(0,20,60);
    f_timewalk->SetParLimits(1,0,3);
    f_timewalk->SetParameters(40,0,tdc0);
    char pname[32];
    for (int i=0; i<9; i++) {
      sprintf(pname,"p%d",i+1);
      f_timewalk->SetParName(i+3,pname);
      f_timewalk->SetParameter(i+3,0);
    }
    for (int i=0; i<5; i++) {
      sprintf(pname,"q%d",i+1);
      f_timewalk->SetParName(i+12,pname);
      f_timewalk->SetParameter(i+12,0);
    }
//    f_timewalk->SetParLimits(6,0,1);
/*
    f_timewalk = new TF1(name,FuncTimewalk,0,150,4);
//f_timewalk = new TF1("timewalk","([0]/sqrt(x-[1])+[2])*(x>[1])",0,200);
    f_timewalk->SetParNames("A","x_{0}","TDC_{0}","ch");
    f_timewalk->FixParameter(0,par_timewalk[0]);
    f_timewalk->SetParameter(1,par_timewalk[1]);
    f_timewalk->SetParameter(2,par_timewalk[2]);
    f_timewalk->FixParameter(3,ch);
*/
  }
  f_timewalk->SetLineWidth(1);
  f_timewalk->SetNpx(1000);

  sprintf(exp,"tdc[%d]:adc[%d]-%d>>h2(200,0,200,%d,%d,%d)",
          ch,ch,int(ped),nbin_tdc,tdc0+tdc_min,tdc0+tdc_max);
  sprintf(cut,"tdc[%d]>0&&tdc[%d]<4000&&nhit==1&&ipos==%d",ch,ch,ch);
  mcp->Draw(exp,cut,"colz");
  TH2D *h2 = (TH2D*)gROOT->FindObject("h2");
  h2->SetStats(kFALSE);
  h2->GetXaxis()->SetTitle("ADC (count)");
  h2->GetYaxis()->SetTitle("TDC (count)");
  h2->GetYaxis()->SetTitleOffset(1.2);
  h2->GetXaxis()->SetRangeUser(0,100);
  h2->GetYaxis()->SetRangeUser(tdc0+tdc_min+10,tdc0+tdc_max-30);
  TGraphErrors *g_timewalk = new TGraphErrors();
  TH1D *py;
  int npoint=0, bin=1;
  double peak=0, rms;
//  for (int i=0; i<20; i++) {
  for (int i=0; i<100; i++) {
    tdc_timewalk[i] = 0;
    py = h2->ProjectionY("",i+1,i+1);
    rms = py->GetRMS();
    if (py->GetEntries() == 0) continue;
    if (i<10 && py->GetEntries()<10) continue;
//    if (py->GetEntries() < 50) continue;
    if (py->GetEntries() < 100 ||
        !spec.Search(py,rms*0.5,"nobackground goff",0.1)) {
      peak = py->GetMean();
    } else {
      peak = spec.GetPositionX()[0];
    }
    py->Fit("gaus","NQ","",peak-100,peak+3);
    tdc_timewalk[i] = (TF1*)(gROOT->FindObject("gaus"))->GetParameter(1);
//    g_timewalk->SetPoint(npoint,h2->GetBinCenter(i+1),tdc_timewalk[i]);
    g_timewalk->SetPoint(npoint,h2->GetXaxis()->GetBinCenter(i+1),tdc_timewalk[i]);
    g_timewalk->SetPointError(npoint,0,(TF1*)(gROOT->FindObject("gaus"))->GetParError(1));
    npoint++;
  }
  g_timewalk->Fit(name,"0Q","",0,10);
  g_timewalk->Fit(name,"0Q","",10,40);
  g_timewalk->Fit(name,"0Q","",0,100);
  f_timewalk->SetRange(f_timewalk->GetParameter(1),100);
  f_timewalk->Draw("same");
  g_timewalk->Draw("P");
/*
  if (amp_ver==1) {
    TGraph *g_timewalk0 = new TGraph();
    for (int i=10; i<100; i++) {
      g_timewalk0->SetPoint(i-10,i,tdc_timewalk0[ch][i]+diff_par[2]);
    }
//    g_timewalk0->SetMarkerStyle(6);
    g_timewalk0->SetMarkerColor(2);
    g_timewalk0->SetLineColor(2);
    g_timewalk0->Draw("l");
  }
*/
  for (int i=0; i<3; i++) {
    diff_par[i] = f_timewalk->GetParameter(i)-par_timewalk[i];
    err_par[i] = f_timewalk->GetParError(i);
  }
  for (int i=0; i<17; i++) par[i] = f_timewalk->GetParameter(i);

  c0[int(ch/4)]->Update();
  TPaveStats *st_timewalk = (TPaveStats*)g_timewalk->FindObject("stats");
//  st_timewalk->SetX1NDC(0.45);
  st_timewalk->SetX1NDC(0.65);
  st_timewalk->SetX2NDC(0.85);
  if (amp_ver==0) {
    st_timewalk->SetY1NDC(0.55);
  } else {
//    st_timewalk->SetY1NDC(0.616);
    st_timewalk->SetY1NDC(0.35);
  }
  st_timewalk->SetY2NDC(0.88);
  c0[int(ch/4)]->Update();

  gStyle->SetOptStat(1);
}

  c1->cd(iHV*2+2);
  sprintf(name,"ch%d_%dV",ch,hv);
  if (amp_ver==0) {
    sprintf(exp,"(tdc[%d]-((%0.2f/sqrt(adc[%d]-%0.1f)+%0.1f)*(adc[%d]>%0.1f)+(%0.4f*(adc[%d]-%0.1f))*(adc[%d]>%0.1f)))*%0.2f>>%s(%d,%d,%d)",
            ch,par_timewalk[0]+diff_par[0],
            ch,ped+par_timewalk[1]+diff_par[1]-(ped-int(ped))-0.5,
            par_timewalk[2]+diff_par[2],
            ch,ped+par_timewalk[1]+diff_par[1]-(ped-int(ped))-0.5,
            par_timewalk[3],ch,ped+par_timewalk[4]-(ped-int(ped))-0.5,ch,
            ped+par_timewalk[4]-(ped-int(ped))-0.5,par_time,
            name,nbin_tdc,tdc_min*par_time,tdc_max*par_time);
  } else {
    sprintf(exp,"(%0.2f/sqrt(adc[%d]-%0.1f)+%0.1f)*(adc[%d]<=%d)",
            par[0],ch,ped+par[1]-(ped-int(ped))-0.5,par[2],
            ch,ped+10);
    sprintf(exp,"%s+(%0.1f+%0.3f*(adc[%d]-%0.1f)+%0.3e*(adc[%d]-%0.1f)**2+%0.3e*(adc[%d]-%0.1f)**3+%0.3e*(adc[%d]-%0.1f)**4+%0.3e*(adc[%d]-%0.1f)**5+%0.3e*(adc[%d]-%0.1f)**6+%0.3e*(adc[%d]-%0.1f)**7+%0.3e*(adc[%d]-%0.1f)**8+%0.3e*(adc[%d]-%0.1f)**9)*(adc[%d]>%d&&adc[%d]<=%d)",exp,
            par[0]/sqrt(10-par[1])+par[2],
            par[3],ch,ped+10-(ped-int(ped))-0.5,
            par[4],ch,ped+10-(ped-int(ped))-0.5,
            par[5],ch,ped+10-(ped-int(ped))-0.5,
            par[6],ch,ped+10-(ped-int(ped))-0.5,
            par[7],ch,ped+10-(ped-int(ped))-0.5,
            par[8],ch,ped+10-(ped-int(ped))-0.5,
            par[9],ch,ped+10-(ped-int(ped))-0.5,
            par[10],ch,ped+10-(ped-int(ped))-0.5,
            par[11],ch,ped+10-(ped-int(ped))-0.5,
            ch,ped+10,ch,ped+40);
    sprintf(exp,"%s+(%0.1f+%0.3f*(adc[%d]-%0.1f)+%0.2e*(adc[%d]-%0.1f)**2+%0.2e*(adc[%d]-%0.1f)**3+%0.2e*(adc[%d]-%0.1f)**4+%0.2e*(adc[%d]-%0.1f)**5)*(adc[%d]>%d)",exp,
            par[0]/sqrt(10-par[1])+par[2]+par[3]*(40-10)
            +par[4]*pow(40-10,2) +par[5]*pow(40-10,3)
            +par[6]*pow(40-10,4) +par[7]*pow(40-10,5)
            +par[8]*pow(40-10,6) +par[9]*pow(40-10,7)
            +par[10]*pow(40-10,8)+par[11]*pow(40-10,9),
            par[12],ch,ped+40-(ped-int(ped))-0.5,
            par[13],ch,ped+40-(ped-int(ped))-0.5,
            par[14],ch,ped+40-(ped-int(ped))-0.5,
            par[15],ch,ped+40-(ped-int(ped))-0.5,
            par[16],ch,ped+40-(ped-int(ped))-0.5,
            ch,ped+40);
//    sprintf(exp,"(%s)*%0.2f>>%s(%d,%d,%d)",exp,
//            par_time,name,nbin_tdc,tdc_min*par_time,tdc_max*par_time);
    sprintf(exp,"(tdc[%d]-(%s))*%0.2f>>%s(%d,%d,%d)",
            ch,exp,par_time,
            name,nbin_tdc,tdc_min*par_time,tdc_max*par_time);
/*
    alias.str("");
    alias << "((" << par_timewalk[0]+diff_par[0] << "/sqrt(adc[" << ch << "]-"
          << ped+par_timewalk[1]+diff_par[1]-(ped-int(ped))-0.5 << ")+"
          << par_timewalk[2]+diff_par[2] << ")*(adc[" << ch << "]>"
          << ped+par_timewalk[1]+diff_par[1]-(ped-int(ped))-0.5
          << "&&adc[" << ch << "]-" << ped << "<10)";
    Double_t x[1];
    Double_t par[3] = {par_timewalk[0]+diff_par[0],
                       par_timewalk[1]+diff_par[1],
                       par_timewalk[2]+diff_par[2]};
    for (int i=10; i<150; i++) {
//      alias << "+" << tdc_timewalk0[ch][i]+diff_par[2]
      x[0] = i-((ped-int(ped))-0.5);
      alias << "+" << FuncTimewalk(x,par)
            << "*(adc[" << ch << "]==" << int(ped)+i << ")";
    }
    alias << ")";
    mcp->SetAlias("tdc_timewalk",alias.str().c_str());
    sprintf(exp,"(tdc[%d]-tdc_timewalk)*%0.2f>>%s(%d,%d,%d)",
            ch,par_time,
            name,nbin_tdc,tdc_min*par_time,tdc_max*par_time);
*/
  }
  mcp->Draw(exp,cut0);
  TH1D *h_tts = (TH1D*)gROOT->FindObject(name);
  h_tts->GetXaxis()->SetTitle("time (ps)");

  double par2[8];
#if 1 // Fit with a double Gaussian
  TF1 *f_tts = new TF1("f_tts",
    "[0]*exp(-0.5*((x-[1])/[2])^2)+[3]*exp(-0.5*((x-[4])/[5])^2)");
  f_tts->SetParNames("Constant","Mean","Sigma","Constant2","Mean2","Sigma2");
  f_tts->SetParameters(h_tts->GetMaximum(),0,50,
                       h_tts->GetMaximum()*0.1,100.,150);
  f_tts->SetParLimits(1,-100.,100.);
  f_tts->SetParLimits(2,0.,100.);
  f_tts->SetParLimits(4,0.,200.);
  f_tts->SetParLimits(5,100.,200.);
  f_tts->SetNpx(1000);
  f_tts->SetLineWidth(1);
  f_tts->SetLineColor(2);
  h_tts->Fit("f_tts","Q","",-500.,500.);
  f_tts->GetParameters(par2);
  TF1 *gaus2 = new TF1("g","[0]*exp(-0.5*((x-[1])/[2])^2)",-500,500);
  gaus2->SetParameters(par2[3],par2[4],par2[5]);
  gaus2->SetNpx(1000);
  gaus2->SetLineWidth(1);
  gaus2->SetLineColor(2);
  gaus2->SetLineStyle(3);
  gaus2->Draw("same");
#endif
#if 0 // Fit with a Gaussian and a Landau
  TF1 *f_tts = new TF1("f_tts",
    "[0]*(exp(-0.5*((x-[1])/[2])^2)+[3]*exp(-0.5*([4]*(x-[5])+exp(-[6]*(x-[5])))))");
  f_tts->SetParNames("Constant","Mean","Sigma","Fraction","Slope1","Tdiff","Slope2");
  f_tts->SetParameters(h_tts->GetMaximum(),0,h_tts->GetRMS()/2.,0.1,0.5,0,0.3);
//  f_tts->SetParameters(h_tts->GetMaximum(),0,h_tts->GetRMS()/2.,0.1,0.5,2,1.5);
  f_tts->SetParLimits(2,0.,100);
  f_tts->SetParLimits(3,0.,0.3);
  f_tts->SetParLimits(4,0.,1.);
  f_tts->FixParameter(5,0.);
  f_tts->SetParLimits(6,0.,0.5);
//  f_tts->SetParLimits(5,1.,5.);
//  f_tts->SetParLimits(6,1.,2.);
  f_tts->SetNpx(1000);
  f_tts->SetLineColor(2);
  f_tts->SetLineWidth(1);
  h_tts->Fit("f_tts","","",-5.,20.);
  f_tts->GetParameters(par2);
  TF1 *landau = new TF1("l","[0]*exp(-0.5*([1]*(x-[2])+exp(-[3]*(x-[2]))))",-60,60);
  landau->SetParameters(par2[0]*par2[3],par2[4],par2[5],par2[6]);
  landau->SetNpx(1000);
  landau->SetLineWidth(1);
  landau->SetLineColor(602);
  landau->Draw("same");
#endif
#if 0 // Fit with a Gaussian
int bin_peak = h_tts->GetMaximumBin();
peak = h_tts->GetBinCenter(bin_peak);
h_tts->Fit("gaus","NQ","",peak-5,peak+2.);
rms = (TF1*)(gROOT->FindObject("gaus"))->GetParameter(2);
(TF1*)(gROOT->FindObject("gaus"))->SetNpx(1000);
(TF1*)(gROOT->FindObject("gaus"))->SetLineWidth(1);
h_tts->Fit("gaus","","",int(peak-rms*3),int(peak+rms)+1);
val_tts = (TF1*)(gROOT->FindObject("gaus"))->GetParameter(2)*tdc2ps[0];
#endif

  c1->Update();
  TPaveStats *st_tts = (TPaveStats*)h_tts->FindObject("stats");
  st_tts->SetX1NDC(0.74);
  st_tts->SetX2NDC(0.98);
  st_tts->SetY1NDC(0.2);
  st_tts->SetY2NDC(0.935);

  sprintf(exp,"adc[%d]-%d>>adc(200,0,200)",ch,int(ped));
  mcp->Draw(exp,cut0,"goff");
  TH1D *h_adc = (TH1D*)gROOT->FindObject("adc");
//  double adc_mean = h_adc->GetMean();
//
//  double err_timewalk_fit = 0;
//     par_timewalk[0]/sqrt(adc_mean-(par_timewalk[1]+diff_par[1])-err_par[1])
//    -par_timewalk[0]/sqrt(adc_mean-(par_timewalk[1]+diff_par[1]))
//    +err_par[2];

  TH1D *h_diff = new TH1D("h_diff","",100,-50,50);
  double entry_adc, diff_timewalk;
  for (int i=0; i<100; i++) {
    entry_adc = (tdc_timewalk[i]) ? h_adc->GetBinContent(i+1) : 0;
    if (tdc_timewalk[i] && i>par[1]-0.5 && i<=100) {
      if (i<=10) {
        diff_timewalk = tdc_timewalk[i]-(par[0]/sqrt(i-(par[1]-0.5))+par[2]);
      } else if (i<=40) {
        diff_timewalk
          = tdc_timewalk[i]-(
            par[0]/sqrt(10-(par[1]-0.5))+par[2]
            +par[3]*(i-10)     +par[4]*pow(i-10,2)+par[5]*pow(i-10,3)
            +par[6]*pow(i-10,4)+par[7]*pow(i-10,5)+par[8]*pow(i-10,6)
            +par[9]*pow(i-10,7)+par[10]*pow(i-10,8)+par[11]*pow(i-10,9));
      } else {
        diff_timewalk
          = tdc_timewalk[i]-(
            par[0]/sqrt(10-(par[1]-0.5))+par[2]
            +par[3]*(40-10)     +par[4]*pow(40-10,2)+par[5]*pow(40-10,3)
            +par[6]*pow(40-10,4)+par[7]*pow(40-10,5)+par[8]*pow(40-10,6)
            +par[9]*pow(40-10,7)+par[10]*pow(40-10,8)+par[11]*pow(40-10,9)
            +par[12]*(i-40)     +par[13]*pow(i-40,2)+par[14]*pow(i-40,3)
            +par[15]*pow(i-40,4)+par[16]*pow(i-40,5));
      }
/*
    if (tdc_timewalk[i] && i>par_timewalk[1]+diff_par[1]-0.5) {
      diff_timewalk[i] =
        tdc_timewalk[i]-
        (((par_timewalk[0]+diff_par[0])
            /sqrt(i-(par_timewalk[1]+diff_par[1]-0.5))
           +par_timewalk[2]+diff_par[2])
          *(i>par_timewalk[1]+diff_par[1]-0.5)
         +(par_timewalk[3]*(i-(par_timewalk[4]-0.5)))
          *(i>par_timewalk[4]-0.5));
*/
      diff_timewalk *= par_time;
      h_diff->Fill(diff_timewalk,entry_adc);
    }
  }
  double err_timewalk = h_diff->GetRMS();
//  err_timewalk = sqrt(err_timewalk*err_timewalk
//                      +err_timewalk_fit*err_timewalk_fit);

  double dt_calib[2][NATT], adc_mean;
  int bin;
  TH1D *h_calib[2], *h_calib_sum;
  sprintf(name,"ch%d_%dV_calib",ch,hv);
  int i_att=0, n_att=0;
if (iHV==0) {
//  for (int i=0; i<NATT; i++) {
  for (int i=0; i<1; i++) {
    adc_calib[i] = 0;
    if (bad_calib_ch3_amp1_10db && att[i]==10) continue;
    sprintf(exp,"adc[%d]-%d>>adc(200,0,200)",ch,int(ped));
    if (amp_ctrl) {
      sprintf(cut,"dt==0&&att==%d&&amp==%d",
              att[i],amp[amp_ver][amp_low]);
    } else {
      sprintf(cut,"dt==0&&att==%d",att[i]);
    }
    calib->Draw(exp,cut,"goff");
    h_adc = (TH1D*)gROOT->FindObject("adc");
    adc_calib[i] = h_adc->GetMean();
  }
}
//  for (int i=0; i<NATT; i++) {
  for (int i=0; i<1; i++) {
    if (bad_calib_ch3_amp1_10db && att[i]==10) continue;
    sprintf(exp,"adc[%d]-%d>>adc(200,0,200)",ch,int(ped));
    mcp->Draw(exp,cut0,"goff");
    h_adc = (TH1D*)gROOT->FindObject("adc");
    bin = h_adc->GetXaxis()->FindBin(adc_calib[i]);
    entry_adc = h_adc->GetBinContent(bin);
    for (int j=0; j<2; j++) {
      sprintf(exp,"adc[%d]-%d>>adc(200,0,200)",ch,int(ped));
      if (amp_ctrl) {
        sprintf(cut,"dt==0&&att==%d&&amp==%d&&cycle==%d",
                att[i],amp[amp_ver][amp_low],j);
      } else {
        sprintf(cut,"dt==0&&att==%d&&cycle==%d",att[i],j);
      }
      sprintf(exp,"tdc[%d]>>tdc(1400,1300,2700)",ch);
      calib->Draw(exp,cut,"goff");
      h_calib[j] = (TH1D*)gROOT->FindObject("tdc");
      spec.Search(h_calib[j],1,"goff",0.1);
      h_calib[j]->Fit("gaus","QN","",spec.GetPositionX()[0]-5,spec.GetPositionX()[0]+5);
      dt_calib[j][n_att] = (TF1*)(gROOT->FindObject("gaus"))->GetParameter(2)*par_time;
    }
    sprintf(exp,"(tdc[%d]-%0.2f)*%0.2f+%0.2f>>+%s(%d,%d,%d)",
            ch,(TF1*)(gROOT->FindObject("gaus"))->GetParameter(1),par_time,
            h_tts->GetBinLowEdge(h_tts->GetMaximumBin()+1),
            name,nbin_tdc,tdc_min*par_time,tdc_max*par_time);
    if (amp_ctrl) {
      sprintf(cut,"(dt==0&&att==%d&&amp==%d&&cycle==%d)*%d",
              att[i],amp[amp_ver][amp_low],
              (dt_calib[0][n_att]<dt_calib[1][n_att])?0:1,entry_adc);
    } else {
      sprintf(cut,"(dt==0&&att==%d&&cycle==%d)*%d",att[i],
              (dt_calib[0][n_att]<dt_calib[1][n_att])?0:1,entry_adc);
    }
    calib->Draw(exp,cut,"goff");
    n_att++;
//    if (h_adc->GetMean() < adc_mean) i_att = i;
//    if (i>0 && h_adc->GetMean() > adc_mean*2.) break;
  }

/*
  sprintf(exp,"tdc[%d]>>tdc(1400,1300,2700)",ch);
  if (amp_ctrl) {
    sprintf(cut,"dt==0&&att==%d&&amp==%d&&cycle==0",
            att[i_att],amp[amp_ver][amp_low]);
  } else {
    sprintf(cut,"dt==0&&att==%d&&cycle==0",att[i_att]);
  }
  calib->Draw(exp,cut,"goff");
  TH1D *h_tdc = (TH1D*)gROOT->FindObject("tdc");
  spec.Search(h_tdc,1,"goff",0.1);
  h_tdc->Fit("gaus","QN","",spec.GetPositionX()[0]-5,spec.GetPositionX()[0]+5);
  sprintf(name,"ch%d_%dV_calib",ch,hv);
  sprintf(exp,"(tdc[%d]-%0.2f)*%0.2f+%0.2f>>%s",
          ch,(TF1*)(gROOT->FindObject("gaus"))->GetParameter(1),par_time,
          h_tts->GetBinLowEdge(h_tts->GetMaximumBin()+1),name);
  calib->Draw(exp,cut,"same");
  TH1D *h_calib_sum = (TH1D*)gROOT->FindObject(name);
*/

  double jitter = 0;
  double err_jitter = 0;
  h_calib_sum = (TH1D*)gROOT->FindObject(name);
  if (h_calib_sum) {
    h_calib_sum->Scale(h_tts->GetMaximum()/h_calib_sum->GetMaximum());
    h_calib_sum->SetLineColor(11);
    h_calib_sum->SetLineStyle(7);
    h_calib_sum->SetFillColor(11);
    h_calib_sum->SetFillStyle(3004);
    h_calib_sum->Draw("same");
    h_calib_sum->Fit("gaus","NQ");
    jitter = (TF1*)(gROOT->FindObject("gaus"))->GetParameter(2);
//    err_jitter = TMath::RMS(n_att,dt_calib);
    err_jitter = (TF1*)(gROOT->FindObject("gaus"))->GetParError(2);
    jitter = sqrt(jitter*jitter+laser*laser);
  }

  double val_tts = f_tts->GetParameter(2);
  val_tts = (val_tts>jitter) ? sqrt(val_tts*val_tts-jitter*jitter) : 0;
  double err_tts = f_tts->GetParError(2);
  err_tts = sqrt(err_tts*err_tts+err_timewalk*err_timewalk+err_jitter*err_jitter);
  char TTSVal[64];
  sprintf(TTSVal,"#splitline{TTS:}{%0.1f #pm %0.1f ps}",val_tts,err_tts);
  TLatex *t_tts = new TLatex(0.55,0.5,TTSVal);
  t_tts->SetNDC(1);
  t_tts->SetTextSize(0.08);
  t_tts->SetTextColor(2);
  t_tts->Draw();
  if (tts && err) {
    *tts = val_tts;
    *err = err_tts;
  } else {
    cout << TTSVal << endl;
  }

  c1->Update();

  if (tts) {
    return *tts;
  } else {
    return;
  }
}


Double_t FuncTimewalk(Double_t *x, Double_t *par)
{
  double adc = x[0];
  if (adc<=par[1]) return 0;

  int ch = int(par[3]);
  if (ch<0 || ch>15) return 0;

  if (adc<10) {
    return par[0]/sqrt(adc-par[1])+par[2];
  } else {
    int i = int(adc-(par[1]-par_timewalk0[1][ch][1]));
    int i0 = int(10.-(par[1]-par_timewalk0[1][ch][1]));
    if (i<0 || i>=150 || i0<0 || i0>=150) return 0;
    return tdc_timewalk0[ch][i]
           +(par[0]/sqrt(10.-par[1])+par[2]-tdc_timewalk0[ch][i0]);
  }
}
