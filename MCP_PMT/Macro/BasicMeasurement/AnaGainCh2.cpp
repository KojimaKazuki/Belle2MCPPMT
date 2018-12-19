#define CALIB_CH 0

#include "calib_gain.h"
#include "DrawA4Canvas.cpp"

static int tdc0;
static double rate[7];

double AnaGainCh2(char fname[], int ch, int iHV, bool calibration=true, double *gain=0, double *error_low=0, double *error_high=0)//, double *rate0=0)
{
#define NAMP 2
#define NATT 9
  const int att[NATT] = {10, 8, 6, 5, 4, 3, 2, 1, 0};

  // Calibration date
#define NCalib 2
  char date_calib[NCalib][64] = {"20130204","20121110"};

#define NCH 16
  double q0[NCH];

  gStyle->SetNdivisions(507,"XY");
  TFile *file = TFile::Open(fname);

  TTree *mcp = (TTree*)file->Get("mcp");
  TTree *calib = (TTree*)file->Get("calib");
  TList *hv_set = file->Get("hv_set");
  int nhv=0;
  if (hv_set) {
    nhv = hv_set->GetEntries();
  } else {
    TClonesArray par_hv;
    par_hv.SetClass("TParameter<int>");
    par_hv.SetName("hv_set");
    mcp->Draw("hv>>h_hv(400,0,4000)","","goff");
    TH1D *h_hv = (TH1D*)gROOT->FindObject("h_hv");
    for (int i=h_hv->GetNbinsX()-1; i>=0; i--) {
      if (h_hv->GetBinContent(i+1)) {
        par_hv[nhv] = new TParameter<int>("HV",h_hv->GetBinLowEdge(i+1));
        nhv++;
      }
    }
    hv_set = (TList*)par_hv;
  }
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

  bool old_data = false;
  bool amp_ctrl = true;
  int amp_ver = 0;
  int amp_low;
  if (strncmp(date,"20120531",8)<=0) {
    old_data = true;
    amp_ctrl = false;
    amp_low = 0;
  } else if (strncmp(date,"20121211",8)<0) {
    cout << "Unavailable" << endl;
    return 0;
  } else if (strncmp(date,"20130501",8)<0) {
    amp_ctrl = false;
    amp_low = 0;
  } else if (strncmp(date,"20140401",8)<0) {
    if (!strncmp(filename,"KT",2) && nhv==7) {
      amp_low = (iHV<=3) ? 1 : 0;
    } else {
      amp_low = (iHV<=1) ? 1 : 0;
    }
#if CALIB_CH
    amp_low = 1-iHV;
#endif
  } else {
    amp_ver = 1;
    if (strncmp(date,"20140403",8)==0 && strncmp(filename,"KT0117",6)==0) {
//      if (!strncmp(filename,"KT",2) && nhv==7) {
        amp_low = (iHV<=2) ? 2 : 0;
//      } else {
//        amp_low = (iHV==0) ? 2 : 0;
//      }
    } else {
      if ((!strncmp(filename,"KT",2) || !strncmp(filename,"YH",2)) && nhv==7) {
        amp_low = (iHV<=1) ? 2 : ((iHV<=3) ? 1 : 0);
      } else {
        amp_low = (iHV<=1) ? 1 : 0;
      }
    }
#if CALIB_CH
    amp_low = 2-iHV;
#endif
  }

  double correction = correction_ch[amp_ver][ch] * correction_gain;
#if CALIB_CH
  correction = 1.;
#endif
  double err_cor = 0;
  if (amp_ver==0 && amp_low==1) {
    correction *= correction_amplv[ch];
    err_cor = err_correction_amplv;
  }
  if (amp_ver==0) {
    correction *= correction_amp[ch];
  }
  if (old_data) {
    correction *= correction_old[ch];
  }

  double par_gain[3];
  for (int i=0; i<3; i++) par_gain[i] = par_amp[amp_ver][ch][amp_low][i];

  bool bad_ref_hitrate = false;
  if (strncmp(date,"20130122",8)<0) {
    bad_ref_hitrate = true;
  }

  bool bad_cable_ch2 = false;
  if (strncmp(date,"20130812",8)>=0 && strncmp(date,"20131120",8)<0) {
    bad_cable_ch2 = true;
  }

if (calibration) {
  int iCalib = 0;
  for (iCalib=0; iCalib<NCalib; iCalib++) {
    if (strncmp(date,date_calib[iCalib],8)>=0) break;
  }
  char fname_calib[128];
  sprintf(fname_calib,"%s/q0_%s.dat",dirname,date_calib[iCalib]);
  ifstream file_calib(fname_calib);
  string str_q0;
  for (int c=0; c<NCH; c++) {
    getline(file_calib,str_q0);
    if (str_q0[0]=='#') {
      c--;
      continue;
    }
    q0[c] = atof(str_q0.c_str())/51.;
//    q0[c] = 0.88;
  }
  file_calib.close();
}

  char exp[256], exp0[256], cut[256], cut0[256], cut1[256];
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
  }

if (calibration) {

  c1->cd(1);
  sprintf(exp,"adc[%d]>>adc_calib(250,50,300)",ch);
  calib->Draw(exp,"cycle==0&&dt==50");
  TH1D *h_adc_calib = (TH1D*)gROOT->FindObject("adc_calib");
  h_adc_calib->GetXaxis()->SetTitle("ADC (count)");
  h_adc_calib->SetLineColor(11);
  h_adc_calib->Fit("gaus","N","",50,150);
  double ped_calib = (TF1*)(gROOT->FindObject("gaus"))->GetParameter(1);
  sprintf(exp,"adc[%d]",ch);
  if (amp_ctrl) {
    calib->Draw(exp,"cycle==0&&amp==1&&dt==0","same");
    calib->Draw(exp,"cycle==1&&amp==1&&dt==0","same");
  } else {
    calib->Draw(exp,"cycle==0&&dt==0","same");
    calib->Draw(exp,"cycle==1&&dt==0","same");
  }
  TH1D *h_adc_calib2 = (TH1D*)gPad->FindObject("htemp");
  h_adc_calib2->SetLineColor(2);

  c1->cd(2);
  double input[2][NAMP][NATT+1], err_input[2][NAMP][NATT+1],
         adc[2][NAMP][NATT+1], err_adc[2][NAMP][NATT+1];
  double mean, rms;
  TGraphErrors *g_adc[2][NAMP];
  TH1D *h_temp = new TH1D("h_temp","",300,0,300);
  for (int i=0; i<2; i++) {
    for (int j=0; j<NAMP; j++) {
      if (!amp_ctrl && j>0) break;
      for (int k=0; k<NATT; k++) {
        sprintf(exp,"adc[%d]-%.2f>>h_temp",ch,ped_calib);
        if (amp_ctrl) {
          sprintf(cut,"cycle==%d&&amp==%d&&att==%d&&dt==0",i,j,att[k]);
        } else {
          sprintf(cut,"cycle==%d&&att==%d&&dt==0",i,att[k]);
        }
        calib->Draw(exp,cut,"goff");
        mean = h_temp->GetMean();
        rms = h_temp->GetRMS();
        h_temp->Fit("gaus","N","",mean-rms*5,mean+rms*5);
        input[i][j][k] = q0[ch]/pow(10,att[k]/20.);
        err_input[i][j][k] = 0;
        adc[i][j][k] = (TF1*)(gROOT->FindObject("gaus"))->GetParameter(1);
        err_adc[i][j][k] = (TF1*)(gROOT->FindObject("gaus"))->GetParError(1);
      }
      input[i][j][NATT] = 0;
      err_input[i][j][NATT] = 0;
      adc[i][j][NATT] = 0;
      err_adc[i][j][NATT] = 0;
      g_adc[i][j] = new TGraphErrors(NATT+1,adc[i][j],input[i][j],err_adc[i][j],err_input[i][j]);
      g_adc[i][j]->GetXaxis()->SetTitle("ADC");
      g_adc[i][j]->GetYaxis()->SetTitle("Charge (pC)");
    }
  }

  for (int j=0; j<NAMP; j++) {
    if (!amp_ctrl && j>0) break;
    g_adc[0][j]->SetMarkerStyle(20);
    g_adc[0][j]->SetMarkerColor(602);
    g_adc[0][j]->SetLineColor(602);
    g_adc[1][j]->SetMarkerStyle(25);
    g_adc[1][j]->SetMarkerColor(2);
    g_adc[1][j]->SetLineColor(2);
    if (j==0) g_adc[0][j]->Draw("AP");
    else      g_adc[0][j]->Draw("P");
    g_adc[1][j]->Draw("P");
  }
  g_adc[0][0]->SetMinimum(0);

  TF1 *adc2pc[2][NAMP];
  gStyle->SetOptFit(1);
  TPaveStats *st_adc[2][NAMP];
  for (int i=0; i<2; i++) {
    for (int j=0; j<NAMP; j++) {
      if (!amp_ctrl && j>0) break;
      sprintf(name,"adc2pc%d",i);
      adc2pc[i][j] = new TF1(name,"[0]*x**[1]+[2]*x**5",0,200);
      adc2pc[i][j]->SetParameters(0.1,0.5,1e-12);
      adc2pc[i][j]->SetParLimits(1,0,1);
      adc2pc[i][j]->SetParLimits(2,0,1e-10);
      adc2pc[i][j]->SetLineWidth(1);
      if (i==0) adc2pc[i][j]->SetLineColor(602);
      else      adc2pc[i][j]->SetLineColor(2);
      g_adc[i][j]->Fit(name,"","",0,adc[i][j][NATT-1]);
      c1->Update();
      st_adc[i][j] = (TPaveStats*)g_adc[i][j]->FindObject("stats");
      st_adc[i][j]->SetX1NDC(0.2+0.3*i);
      st_adc[i][j]->SetX2NDC(0.6+0.3*i);
      st_adc[i][j]->SetY1NDC(0.65-0.45*i);
      st_adc[i][j]->SetY2NDC(0.88-0.45*i);
      st_adc[i][j]->Draw();
    }
  }

  for (int i=0; i<3; i++) par_gain[i] = adc2pc[0][amp_low]->GetParameter(i);

} // if (calibration)

  if (!mcp) return 0;
 
  char *id, *buf = mcp->GetTitle();
  // char *id, *buf = fname;
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

/*
  ostringstream alias;
  alias << "((tdc[0]>0&&tdc[0]<4000)";
  for (int c=1; c<16; c++) {
    alias << "+(tdc[" << c << "]>0&&tdc[" << c << "]<4000)";
  }
  alias << ")";
  mcp->SetAlias("nhit",alias.str().c_str());
*/

//  sprintf(cut0,"nhit==1&&ipos==%d&&hv==%d",ch+old_data,hv);
  sprintf(cut0,"ipos==%d&&hv==%d",ch+old_data,hv);
#if CALIB_CH
  if (ch==0) {
    sprintf(cut0,"ipos==0&&hv==%d",hv);
  } else {
    sprintf(cut0,"ipos==0&&hv==%d&&tdc[0]>4000",hv);
  }
#endif

  if (iHV==0 || CALIB_CH) {
    if (!bad_ref_hitrate) {
      sprintf(cut,"tdc_ref<4000&&ipos==%d",((CALIB_CH)?0:ch)+old_data);
      mcp->Draw("hv>>h_ref(4000,0,4000)",cut,"goff");
    }
    sprintf(exp,"tdc[%d]>>tdc(4000,0,4000)",ch);
    sprintf(cut,"tdc[%d]<4000&&ipos==%d",ch,((CALIB_CH)?0:ch)+old_data);
    mcp->Draw(exp,cut,"goff");
    TH1D *h_tdc = (TH1D*)gROOT->FindObject("tdc");
    TSpectrum spec;
    spec.Search(h_tdc);
    tdc0 = (int)spec.GetPositionX()[0];
    sprintf(cut,"tdc[%d]>%d&&tdc[%d]<%d&&ipos==%d",
            ch,tdc0-20,ch,tdc0+120,((CALIB_CH)?0:ch)+old_data);
    mcp->Draw("hv>>h_hit(4000,0,4000)",cut,"goff");
    TH1D *h_hit = (TH1D*)gROOT->FindObject("h_hit");
    if (!bad_ref_hitrate) {
      TH1D *h_ref = (TH1D*)gROOT->FindObject("h_ref");
      h_hit->Divide(h_ref);
    }
    for (int i=0; i<nhv; i++) {
      rate[i] = h_hit->GetBinContent(((TParameter<int>*)hv_set->At(i))->GetVal()+1);
      rate[i] /= (double)h_hit->GetMaximum();
    }
  }
  sprintf(cut,"tdc[%d]>%d&&tdc[%d]<%d&&%s",ch,tdc0-20,ch,tdc0+120,cut0);
  int n_hit = mcp->GetEntries(cut);

  sprintf(exp,"adc[%d]>>ped(200,0,200)",ch);
  sprintf(cut,"tdc[%d]>4000&&%s",ch,cut0);
  mcp->Draw(exp,cut);
  TH1D *h_ped = (TH1D*)gROOT->FindObject("ped");
  if (!h_ped->GetEntries()) {
    cout << "************************" << endl;
    cout << "Pedestal error (ch" << ch << ", " << hv << " V)" << endl;
    cout << "************************" << endl;
    for (int i=iHV; i<7; i++) {
      c1->GetPad(i*2+1)->Clear();
      c1->GetPad(i*2+2)->Clear();
    }
//    c1->Close();
    return 0;
  }
  h_ped->Fit("gaus","Q","",50,150);
  double ped = (TF1*)(gROOT->FindObject("gaus"))->GetParameter(1);
  sprintf(name,"ch%d_%dV_ADC",ch,hv);
  sprintf(exp,"adc[%d]-%d>>%s(200,0,200)",ch,int(ped),name);
  if (bad_cable_ch2 && ch==2) {
    sprintf(cut1,"tdc[2]>1400&&tdc[2]<1540&&%s",cut0);
  } else {
    sprintf(cut1,"tdc[%d]>%d&&tdc[%d]<%d&&%s",ch,tdc0-20,ch,tdc0+120,cut0);
  }
  sprintf(cut,"%s&&adc[%d]-%d>=0",cut1,ch,int(ped));
  mcp->Draw(exp,cut);
  TH1D *h_adc = (TH1D*)gROOT->FindObject(name);
  h_adc->GetXaxis()->SetTitle("ADC (count)");

  c1->cd(iHV*2+2);
  double bins[1024] = {0};
  int nbin=200;
  for (int i=1; i<nbin; i++) {
    bins[i] = (par_gain[0]*pow(i-(ped-int(ped))+0.001,par_gain[1])
              +par_gain[2]*pow(i-(ped-int(ped))+0.001,5))
              / correction;
    if (i>=199 && bins[i]<2) nbin++;
    if (bins[i-1]<2 && bins[i]>2) bins[i]=2;
  }
  nbin--;
  sprintf(exp0,
     "(%.3e*(adc[%d]-%.2f)**%.3f+%.3e*(adc[%d]-%.2f)**5)/%.2f",
     par_gain[0],ch,ped,par_gain[1],par_gain[2],ch,ped,
     correction);
  sprintf(cut,"%s&&adc[%d]-%d>0",cut1,ch,int(ped));
  sprintf(title,"%s {%s}",exp0,cut);
  sprintf(name,"ch%d_%dV",ch,hv);
  TH1D *h_gain = new TH1D(name,title,nbin,bins);
  sprintf(exp,"%s>>%s",exp0,name);
  mcp->Draw(exp,cut,"goff");
  sprintf(exp,"0>>+%s",name);
  sprintf(cut,"%s&&adc[%d]-%d==0",cut1,ch,int(ped));
  mcp->Draw(exp,cut);
  h_gain->GetXaxis()->SetTitle("charge (pC)");
  h_gain->GetXaxis()->SetRangeUser(0,2.);

  sprintf(name,"ch%d_%dV_ped",ch,hv);
  TH1D *h_gain_ped = new TH1D(name,"",nbin,bins);
  h_gain_ped->SetLineColor(11);
  h_gain_ped->SetLineStyle(7);
  h_gain_ped->SetFillColor(11);
  h_gain_ped->SetFillStyle(3004);
  sprintf(exp,"%s>>%s",exp0,name);
  sprintf(cut,"tdc[%d]>4000&&%s&&adc[%d]-%d>0",ch,cut0,ch,int(ped));
  int n_ped = mcp->Draw(exp,cut,"goff");
  sprintf(exp,"0>>+%s",name);
  sprintf(cut,"tdc[%d]>4000&&%s&&adc[%d]-%d==0",ch,cut0,ch,int(ped));
  n_ped += mcp->Draw(exp,cut,"goff");
  h_gain_ped->Scale(n_hit/double(n_ped));
  h_gain_ped->Draw("same");

  int bin_threshold = 0;
  for (int i=0; i<nbin; i++) {
    if (h_gain_ped->GetBinContent(i+1) < h_gain->GetBinContent(i+1)) {
      bin_threshold = i;
      break;
    }
  }
  if (!bin_threshold) bin_threshold++;

  sprintf(name,"ch%d_%dV_cor",ch,hv);
  TH1D *h_gain_cor = (TH1D*)h_gain->Clone(name);
//  double rate = (bad_ref_hitrate) ? n_hit : n_hit/double(n_ref);
//  if (rate0 && amp_ver==0) {
  if (amp_ver==0) {
//    if (iHV==0) *rate0 = rate;
//    if ((*rate0)>rate) {
//      int n_add = int((1.-rate/(*rate0))*n_ref);
      int n_add = int((1./rate[iHV]-1)*n_hit);
      int cont1, cont2;
      for (int i=bin_threshold; i>=0; i--) {
        cont1 = h_gain_cor->GetBinContent(i+1);
        cont2 = h_gain_cor->GetBinContent(i+2);
        if (cont1+n_add > cont2) {
          h_gain_cor->SetBinContent(i+1,cont2);
          n_add -= cont2-cont1;
        } else {
          h_gain_cor->SetBinContent(i+1,cont1+n_add);
          n_add = 0;
          break;
        }
        if (i==0 && n_add) {
          for (int j=bin_threshold; j>=0; j--) {
            h_gain_cor->SetBinContent(j+1,
                h_gain_cor->GetBinContent(j+1)+int(n_add/(bin_threshold+1)));
          }
        }
      }
//    }
  }
#if 0
  char RateVal[64];
  sprintf(RateVal,"Relative hit rate: %0.2f%%",
          ((bad_ref_hitrate) ? rate/(*rate0) : rate));
  TText *t_rate = new TText(0.5,0.4,RateVal);
  t_rate->SetNDC(1);
  t_rate->Draw();
#endif

  sprintf(name,"ch%d_%dV_norm",ch,hv);
  TH1D *h_gain_norm = (TH1D*)h_gain_cor->Clone(name);
  for (int i=0; i<nbin; i++) {
    h_gain_norm->SetBinContent(i+1,h_gain_norm->GetBinContent(i+1)/(bins[i+1]-bins[i]));
  }
  h_gain_norm->Scale(h_gain->Integral()/h_gain_norm->Integral());
  h_gain_norm->SetLineColor(2);
  h_gain_norm->Draw("same");

  char GainVal[64];
  if (gain) {
    *gain = (h_gain_cor->GetMean())*1.e-12/1.6e-19/1.e6;
  } else {
    printf("Gain: (%0.2f #pm ",(h_gain_cor->GetMean())*1.e-12/1.6e-19/1.e6);
  }
  double gain_high, gain_low, dgain_high=0, dgain_low=0;
  if (gain) {
    gain_high = (h_gain->GetMean())*1.e-12/1.6e-19/1.e6;
//    gain_low = (rate<(*rate0)) ? gain_high * rate/(*rate0) : (*gain);
    gain_low = gain_high * rate[iHV];
    dgain_high = gain_high-(*gain);
    dgain_low = ((*gain)>gain_low) ? ((*gain)-gain_low) : 0.;
  }
  double e1 = h_gain_cor->GetMeanError();
  double e2 = h_gain_ped->GetMeanError();
  double e3 = 0;
  for (int i=1; i<nbin; i++) {
    if (bins[i]>h_gain_cor->GetMean()) {
      e3 = bins[i]-bins[i-1];
      break;
    }
  }
  double err_stat = sqrt(e1*e1+e2*e2+e3*e3)*1.e-12/1.6e-19/1.e6;

  if (gain && error_low && error_high) {
    *error_high = sqrt(err_stat*err_stat+dgain_high*dgain_high
                       +err_cor*err_cor*(*gain)*(*gain));
    *error_low  = sqrt(err_stat*err_stat+dgain_low*dgain_low
                       +err_cor*err_cor*(*gain)*(*gain));
  } else {
    printf("%0.2f) e6\n",err_stat);
  }
  if (gain) {
    sprintf(GainVal,"#splitline{Gain:}{(%0.2f #pm %0.2f) e6}",*gain,err_stat);
    TLatex *t_gain = new TLatex(0.65,0.5,GainVal);
    t_gain->SetNDC(1);
    t_gain->SetTextSize(0.08);
    t_gain->SetTextColor(2);
    t_gain->Draw();
  }
  c1->Update();

  if (gain) {
    return *gain;
  } else {
//    return (h_gain_cor->GetMean())*1.e-12/1.6e-19/1.e6;
    return;
  }
}
