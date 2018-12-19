void gain(char fplace[]){
 
   char runnumber[128];
   ifstream fin;
   sprintf(runnumber,"%srunnumber",fplace);
   fin.open(runnumber);
   if(!fin){
     cout << fplace << "; Not runnumber file in this directory." << endl;
     return ;
   }
   
   char buf[20];
   fin.getline(buf,20);
   cout << buf << endl;
   fin.close();
   Int_t fnumber = atoi(buf);//get the runnumber
   const Int_t n = fnumber;
   //const Int_t n = 2;
   TGraph*g[32];   
   TLegend*leg[8];
   char PMT1[32];
   char PMT2[32];
   char PMT3[32];
   char PMT4[32];
   char PMT5[32];
   char PMT6[32];
   char PMT7[32];
   char PMT8[32];
   char pmtch[512];
   char PED[512];
   char ADC[512];
   char GAIN[512];
   char PEDcut[512];
   char ADCcut[512];
   Double_t ped[32];
   Double_t gain[32][n];
   Double_t amp_gain[32]={55.81,55.13,55.46,55.07,56.82,54.67,56.86,54.22,55.58,55.70,59.51,56.87,59.59,58.58,58.13,58.26,55.42,58.41,57.05,57.73,57.91,59.19,53.69,55.63,48.62,49.08,48.9,51.53,48.36,47.92,46.12,47.36};
   Double_t bin[n] ={};
   for(Int_t k=0;k<n;k++){
     bin[k] = k+1;
     char fname[512];
     sprintf(fname,"%slifetime_laser%d.root",fplace,k);
     //cout << fname << endl;
     char title[128];
     sprintf(title,"%s",fname);
     strtok(title,"/");
     while(1){
       sprintf(title,"%s",strtok(NULL,"/"));
       if(strstr(title,".root")) break;
     }
     //cout << title << endl;//get the file title in this directry
     TFile *f = new TFile(fname); 
     TTree *tree1 = (TTree*)f->Get("mcp");
     TTree *tree2 = (TTree*)f->Get("config");
     if(k == 0){
       TCanvas*c1 = new TCanvas("Gain","Gain");
       c1->Divide(8,4);
       TCanvas*c2 = new TCanvas("Gaincheck","Gaincheck");
       c2->Divide(4,2);
     }
       tree2->SetBranchAddress("pmt1",&PMT1);
       tree2->SetBranchAddress("pmt2",&PMT2);
       tree2->SetBranchAddress("pmt3",&PMT3);
       tree2->SetBranchAddress("pmt4",&PMT4);
       tree2->SetBranchAddress("pmt5",&PMT5);
       tree2->SetBranchAddress("pmt6",&PMT6);
       tree2->SetBranchAddress("pmt7",&PMT7);
       tree2->SetBranchAddress("pmt8",&PMT8);
       tree2->GetEntry(0);
       
     
       cout << PMT1 << "|" << PMT2 << "|" << PMT3 << "|" << PMT4 << "|" << PMT5 << "|" << PMT6 << "|" << PMT7 << "|" << PMT8 << endl;
       for(Int_t i =0;i<32;i++){

	 sprintf(PEDcut,"tdc[%d]>4000",i);
	 if(i == 0 || i == 4 || i == 10 || i == 12 || i == 20 || i == 24 || i == 30)sprintf(ADCcut,"2400<tdc[%d]&&tdc[%d]<2700",i,i);
	 else sprintf(ADCcut,"1200<tdc[%d]&&tdc[%d]<1500",i,i);
	 sprintf(PED,"adc[%d]>>ped%d(200,0,200)",i,i);
	 char pedn[512];
	 sprintf(pedn,"ped%d",i);
	 char adcn[512];
	 sprintf(adcn,"adcch%d",i);
	 char gainn[512];
	 sprintf(gainn,"gainch%d",i);
	 
	 c1->cd(i+1);
	 tree1->Draw(PED,PEDcut);
	 TH1D*pedh=(TH1D*)gROOT->FindObject(pedn); 
	 ped[i]=pedh->GetMean(); 
	 //cout << ped[i] << " ; pedestal" << endl;
	 sprintf(ADC,"adc[%d]-%f>>adcch%d(110,-10,100)",i,ped[i],i);
	 tree1->Draw(ADC,ADCcut);
	 TH1D*adch=(TH1D*)gROOT->FindObject(adcn); 
	 adch->SetXTitle("/0.25pC");
	 sprintf(GAIN,"((adc[%d]-%f)*0.25*4*pow(10,1))/(1.6*%f)>>gainch%d(22,-1,11)",i,ped[i],amp_gain[i],i);
	 tree1->Draw(GAIN,ADCcut);
	 TH1D*gainh=(TH1D*)gROOT->FindObject(gainn); 
	 gainh->SetXTitle("gain[*10^6]");
	 gain[i][k] = gainh->GetMean();
	 c1->Update();
	 if(k == n-1){
	 
	   char fname0[512];
	   sprintf(fname0,"%slifetime_laser%d.root",fplace,0);
	   TFile *f0 = new TFile(fname0); 
	   TTree *tree01 = (TTree*)f0->Get("mcp");
	   char PED0[512];
	   sprintf(PED0,"adc[%d]>>ped0%d(200,0,200)",i,i);
	   tree01->Draw(PED0,PEDcut);
	   char ped0n[512];
	   sprintf(ped0n,"ped0%d",i);
	   TH1D*ped0h=(TH1D*)gROOT->FindObject(ped0n); 
	   ped[i]=pedh->GetMean(); 
	   //cout << ped[i] << " ; pedestal" << endl;
	   char ADC0[512];
	   sprintf(ADC0,"adc[%d]-%f>>adc0ch%d(110,-10,100)",i,ped[i],i);
	   tree01->Draw(ADC0,ADCcut);
	   char adc0n[512];
	   sprintf(adc0n,"adc0ch%d",i);;
	   TH1D*adc0h=(TH1D*)gROOT->FindObject(adc0n); 
	   adc0h->SetXTitle("/0.25pC");
	   char GAIN0[512];
	   sprintf(GAIN0,"((adc[%d]-%f)*0.25*4*pow(10,1))/(1.6*%f)>>gain0ch%d(22,-1,11)",i,ped[i],amp_gain[i],i);
	   tree01->Draw(GAIN0,ADCcut);
	   char gain0n[512];
	   sprintf(gain0n,"gain0ch%d",i);
	   TH1D*gain0h=(TH1D*)gROOT->FindObject(gain0n); 
	   gain0h->SetLineColor(2);
	 
	   tree1->Draw(GAIN,ADCcut,"same");
	   TH1D*gainh=(TH1D*)gROOT->FindObject(gainn); 
	   gainh->SetXTitle("gain[*10^6]");
	   gainh->SetLineColor(4);
	 	
	   if(i<4){
	     gain0h->SetTitle(PMT1);       
	     if(i==0)cout << k+1 << "/" << n <<endl;
	     cout << PMT1 << " ; " << "ch" << i << " ; " << gain[i][k] << " ; gain" << endl; 
	   }else if(i<8){
	     gain0h->SetTitle(PMT2);
	     cout << PMT2 << " ; " << "ch" << i << " ; " << gain[i][k] << " ; gain" << endl;
	   }else if(i<12){
	     gain0h->SetTitle(PMT3);
	     cout << PMT3 << " ; " << "ch" << i << " ; " << gain[i][k] << " ; gain" << endl;
	   }else if(i<16){
	     gain0h->SetTitle(PMT4);
	     cout << PMT4 << " ; " << "ch" << i << " ; " << gain[i][k] << " ; gain" << endl;
	   }else if(i<20){
	     gain0h->SetTitle(PMT5);
	     cout << PMT5 << " ; " << "ch" << i << " ; " << gain[i][k] << " ; gain" << endl;
	   }else if(i<24){
	     gain0h->SetTitle(PMT6);
	     cout << PMT6 << " ; " << "ch" << i << " ; " << gain[i][k] << " ; gain" << endl;
	   }else if(i<28){
	     gain0h->SetTitle(PMT7);
	     cout << PMT7 << " ; " << "ch" << i << " ; " << gain[i][k] << " ; gain" << endl;
	   }else if(i<32){
	     gain0h->SetTitle(PMT8);
	     cout << PMT8 << " ; " << "ch" << i << " ; " << gain[i][k] << " ; gain" << endl;
	     if(i == 31)cout << "=====================" << endl; 
	   }
	 }
       }
   }
   
   for(Int_t m = 0;m<8;m++)leg[m] = new TLegend(0.7,0.7,0.9,0.9);
   for(Int_t l=0;l<32;l++){
     Double_t Xmax;
     Double_t Xmin;
     g[l] = new TGraph(n,bin,gain[l]);
     g[l]->SetMarkerSize(2);
     g[l]->SetMarkerStyle(5);
     //g[l]->SetMinimum(0.3);
     //g[l]->SetMaximum(2.5);
     g[l]->GetYaxis()->SetTitle("gain[*10^6]");
     
     if(l<4){
       c2->cd(1);
       g[l]->SetTitle(PMT1);
       sprintf(pmtch,"%sch%d",PMT1,l);
       cout<< pmtch << endl;
       leg[0]->AddEntry(g[l],pmtch,"l");
       
       if(l == 0){
	 Xmax = gain[l][0];
	 Xmin = gain[l][0];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l][m])Xmax = gain[l][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+1][m])Xmax = gain[l+1][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+2][m])Xmax = gain[l+2][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+3][m])Xmax = gain[l+3][m];
	 
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l][m])Xmin = gain[l][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+1][m])Xmin = gain[l+1][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+2][m])Xmin = gain[l+2][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+3][m])Xmin = gain[l+3][m];
	 
	 Xmax = Xmax + 0.3;
	 Xmin = Xmin - 0.3;
	 g[l]->SetMinimum(Xmin);
	 g[l]->SetMaximum(Xmax);
	 g[l]->Draw("apl");
       }else{
	 g[l]->Draw("pl");
	 if(l == 1){
	   g[l]->SetLineColor(2);
	   g[l]->SetMarkerColor(2);
	 }
	 if(l == 2){
	   g[l]->SetLineColor(4);
	   g[l]->SetMarkerColor(4);
	 }
	 if(l == 3){
	   g[l]->SetLineColor(6);
	   g[l]->SetMarkerColor(6);
	   leg[0]->Draw();
	 }
       }
     }else if(l<8){
       c2->cd(2);
       g[l]->SetTitle(PMT2);
       sprintf(pmtch,"%sch%d",PMT2,l-4);
       leg[1]->AddEntry(g[l],pmtch,"l");
       if(l == 4){
	 Xmax = gain[l][0];
	 Xmin = gain[l][0];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l][m])Xmax = gain[l][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+1][m])Xmax = gain[l+1][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+2][m])Xmax = gain[l+2][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+3][m])Xmax = gain[l+3][m];
	 
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l][m])Xmin = gain[l][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+1][m])Xmin = gain[l+1][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+2][m])Xmin = gain[l+2][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+3][m])Xmin = gain[l+3][m];
	
	 Xmax = Xmax + 0.3;
	 Xmin = Xmin - 0.3;
	 g[l]->SetMinimum(Xmin);
	 g[l]->SetMaximum(Xmax);
	 g[l]->Draw("apl");
       }else{
	 g[l]->Draw("pl");
	 if(l == 5){
	   g[l]->SetLineColor(2);
	   g[l]->SetMarkerColor(2);
	 }
	 if(l == 6){
	   g[l]->SetLineColor(4);
	   g[l]->SetMarkerColor(4);
	 }
	 if(l == 7){
	   g[l]->SetLineColor(6);
	   g[l]->SetMarkerColor(6);
	   leg[1]->Draw();
	 }
       }
     }else if(l<12){
       c2->cd(3);	   
       g[l]->SetTitle(PMT3);
       sprintf(pmtch,"%sch%d",PMT3,l-8);
       leg[2]->AddEntry(g[l],pmtch,"l");
       if(l == 8){
	 Xmax = gain[l][0];
	 Xmin = gain[l][0];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l][m])Xmax = gain[l][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+1][m])Xmax = gain[l+1][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+2][m])Xmax = gain[l+2][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+3][m])Xmax = gain[l+3][m];
	 
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l][m])Xmin = gain[l][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+1][m])Xmin = gain[l+1][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+2][m])Xmin = gain[l+2][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+3][m])Xmin = gain[l+3][m];
		
	 Xmax = Xmax + 0.3;
	 Xmin = Xmin - 0.3;
	 g[l]->SetMinimum(Xmin);
	 g[l]->SetMaximum(Xmax);
	 g[l]->Draw("apl");
       }else{
	 g[l]->Draw("pl");
	 if(l == 9){
	   g[l]->SetLineColor(2);
	   g[l]->SetMarkerColor(2);
	 }
	 if(l == 10){
	   g[l]->SetLineColor(4);
	   g[l]->SetMarkerColor(4);
	 }
	 if(l == 11){
	   g[l]->SetLineColor(6);
	   g[l]->SetMarkerColor(6);
	   leg[2]->Draw();	 
	 }
       }
     }else if(l<16){
       c2->cd(4);
       g[l]->SetTitle(PMT4);
       sprintf(pmtch,"%sch%d",PMT4,l-12);
       leg[3]->AddEntry(g[l],pmtch,"l");
       if(l == 12){
	 Xmax = gain[l][0];
	 Xmin = gain[l][0];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l][m])Xmax = gain[l][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+1][m])Xmax = gain[l+1][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+2][m])Xmax = gain[l+2][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+3][m])Xmax = gain[l+3][m];
	 
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l][m])Xmin = gain[l][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+1][m])Xmin = gain[l+1][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+2][m])Xmin = gain[l+2][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+3][m])Xmin = gain[l+3][m];

	 Xmax = Xmax + 0.3;
	 Xmin = Xmin - 0.3;
	 g[l]->SetMinimum(Xmin);
	 g[l]->SetMaximum(Xmax);
	 g[l]->Draw("apl");
       }else{
	 g[l]->Draw("pl");
	 if(l == 13){
	   g[l]->SetLineColor(2);
	   g[l]->SetMarkerColor(2);
	 }
	 if(l == 14){
	   g[l]->SetLineColor(4);
	   g[l]->SetMarkerColor(4);
	 }
	 if(l == 15){
	   g[l]->SetLineColor(6);
	   g[l]->SetMarkerColor(6);
	   leg[3]->Draw();
	 }
       }
     }else if(l<20){
       c2->cd(5);
       g[l]->SetTitle(PMT5);	 
       sprintf(pmtch,"%sch%d",PMT5,l-16);
       leg[4]->AddEntry(g[l],pmtch,"l");
       if(l == 16){
	 Xmax = gain[l][0];
	 Xmin = gain[l][0];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l][m])Xmax = gain[l][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+1][m])Xmax = gain[l+1][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+2][m])Xmax = gain[l+2][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+3][m])Xmax = gain[l+3][m];
	 
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l][m])Xmin = gain[l][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+1][m])Xmin = gain[l+1][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+2][m])Xmin = gain[l+2][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+3][m])Xmin = gain[l+3][m];
	 
	 Xmax = Xmax + 0.3;
	 Xmin = Xmin - 0.3;
	 g[l]->SetMinimum(Xmin);
	 g[l]->SetMaximum(Xmax);
	 g[l]->Draw("apl");
       }else{
	 g[l]->Draw("pl");
	 if(l == 17){
	   g[l]->SetLineColor(2);
	   g[l]->SetMarkerColor(2);
	 }
	 if(l == 18){
	   g[l]->SetLineColor(4);
	   g[l]->SetMarkerColor(4);
	 }
	 if(l == 19){
	   g[l]->SetLineColor(6);
	   g[l]->SetMarkerColor(6);
	   leg[4]->Draw();	 
	 }
       }
     }else if(l<24){
       c2->cd(6);
       g[l]->SetTitle(PMT6);
       sprintf(pmtch,"%sch%d",PMT6,l-20);
       leg[5]->AddEntry(g[l],pmtch,"l");
       if(l == 20){
	 Xmax = gain[l][0];
	 Xmin = gain[l][0];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l][m])Xmax = gain[l][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+1][m])Xmax = gain[l+1][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+2][m])Xmax = gain[l+2][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+3][m])Xmax = gain[l+3][m];
	 
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l][m])Xmin = gain[l][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+1][m])Xmin = gain[l+1][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+2][m])Xmin = gain[l+2][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+3][m])Xmin = gain[l+3][m];

	 Xmax = Xmax + 0.3;
	 Xmin = Xmin - 0.3;
	 g[l]->SetMinimum(Xmin);
	 g[l]->SetMaximum(Xmax);
	 g[l]->Draw("apl");
       }else{
	 g[l]->Draw("pl");
	 if(l == 21){
	   g[l]->SetLineColor(2);
	   g[l]->SetMarkerColor(2);
	 }
	 if(l == 22){
	   g[l]->SetLineColor(4);
	   g[l]->SetMarkerColor(4);
	 }
	 if(l == 23){
	   g[l]->SetLineColor(6);
	   g[l]->SetMarkerColor(6);
	   leg[5]->Draw();
	 }
       }
     }else if(l<28){
       c2->cd(7);
       g[l]->SetTitle(PMT7);	 	 
       sprintf(pmtch,"%sch%d",PMT7,l-24);
       leg[6]->AddEntry(g[l],pmtch,"l"); 
       if(l == 24){
	 Xmax = gain[l][0];
	 Xmin = gain[l][0];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l][m])Xmax = gain[l][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+1][m])Xmax = gain[l+1][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+2][m])Xmax = gain[l+2][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+3][m])Xmax = gain[l+3][m];
	 
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l][m])Xmin = gain[l][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+1][m])Xmin = gain[l+1][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+2][m])Xmin = gain[l+2][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+3][m])Xmin = gain[l+3][m];

	 Xmax = Xmax + 0.3;
	 Xmin = Xmin - 0.3;
	 g[l]->SetMinimum(Xmin);
	 g[l]->SetMaximum(Xmax);
	 g[l]->Draw("apl");
       }else{
	 g[l]->Draw("pl");
	 if(l == 25){
	   g[l]->SetLineColor(2);
	   g[l]->SetMarkerColor(2);
	 }
	 if(l == 26){
	   g[l]->SetLineColor(4);
	   g[l]->SetMarkerColor(4);
	 }
	 if(l == 27){
	   g[l]->SetLineColor(6);
	   g[l]->SetMarkerColor(6);
	   leg[6]->Draw();
	 }
       }
     }else if(l<32){
       c2->cd(8);
       g[l]->SetTitle(PMT8);	 	 
       sprintf(pmtch,"%sch%d",PMT8,l-28);
       leg[7]->AddEntry(g[l],pmtch,"l");
       if(l == 28){
	 g[l]->Draw("apl");
	 Xmax = gain[l][0];
	 Xmin = gain[l][0];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l][m])Xmax = gain[l][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+1][m])Xmax = gain[l+1][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+2][m])Xmax = gain[l+2][m];
	 for(Int_t m=0;m<n;m++)if(Xmax<gain[l+3][m])Xmax = gain[l+3][m];
	 
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l][m])Xmin = gain[l][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+1][m])Xmin = gain[l+1][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+2][m])Xmin = gain[l+2][m];
	 for(Int_t m=0;m<n;m++)if(Xmin>gain[l+3][m])Xmin = gain[l+3][m];

	 Xmax = Xmax + 0.3;
	 Xmin = Xmin - 0.3;
	 g[l]->SetMinimum(Xmin);
	 g[l]->SetMaximum(Xmax);
       }else{
	 g[l]->Draw("pl");
	 if(l == 29){
	   g[l]->SetLineColor(2);
	   g[l]->SetMarkerColor(2);
	 }
	 if(l == 30){
	   g[l]->SetLineColor(4);
	   g[l]->SetMarkerColor(4);
	 }
	 if(l == 31){
	   g[l]->SetLineColor(6);
	   g[l]->SetMarkerColor(6);
	   leg[7]->Draw();
	 }
       }
     }
     c2->Update();
   }
   c1.Print("Gain.ps(");
   c2.Print("Gain.ps)");
}

