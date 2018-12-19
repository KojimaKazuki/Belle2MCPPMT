void macroHistogram1D_181012(){

    gStyle -> SetOptStat("irme");
    //gStyle -> SetOptFit(stat);
    gStyle -> SetStatX(0.88); //y at the top-right point of box;
    gStyle -> SetStatY(0.88); //y at the top-right point of box;
    gStyle -> SetStatH(0.18); // hight of box
    gStyle -> SetStatW(0.25); // width of box
    gStyle -> SetLabelSize(0.04,"x");
    gStyle -> SetLabelSize(0.04,"y");
    gStyle -> SetTitleXSize(0.04);
    gStyle -> SetTitleYSize(0.04);
    gStyle->SetTitleYOffset(1.2);


    const Int_t chnum = 0; 
    char cut1[128];
    sprintf(cut1,"ipos == %d",chnum);


    TFile *inFile1 = new TFile("/chai/sgt1/bfactory/mcppmt_data/b-field_KEK/data4/KT0552_20181002.root");
    TTree *inTree1 = (TTree*)inFile1 -> Get("mcp_ref");

    inTree1 -> Draw("adc_ref >> hist1 ( 200, 0.0, 400.0)", cut1);
    inTree1 -> Draw("adc_ref >> hist5 ( 50, 60.0, 110.0)", cut1);

    TH1F *hist1 = (TH1F*)gROOT -> FindObject("hist1");
    TH1F *hist5 = (TH1F*)gROOT -> FindObject("hist5");
    //hist1 -> Fit("gaus","fitting option","graphics option", x_min, x_MAX);



    TTree *inTree2 = (TTree*)inFile1 -> Get("mcp");

    inTree2 -> Draw("adc_ref >> hist2 ( 200, 0.0, 400.0)", cut1);
    inTree2 -> Draw("adc_ref >> hist6 ( 50, 60.0, 110.0)", cut1);
    

    TH1F *hist2 = (TH1F*)gROOT -> FindObject("hist2");
    TH1F *hist6 = (TH1F*)gROOT -> FindObject("hist6");
    //hist2 -> Fit("gaus","fitting option","graphics option", x_min, x_MAX);



    TFile *inFile2 = new TFile("/home/bqt/kojima/MCP_PMT/Data/1810/KT0698_20181012_1.root ");
    TTree *inTree3 = (TTree*)inFile2 -> Get("mcp_ref");

    inTree3 -> Draw("adc_ref >> hist3 ( 200, 0.0, 400.0)", cut1);
    inTree3 -> Draw("adc_ref >> hist7 ( 50, 60.0, 110.0)", cut1);

    TH1F *hist3 = (TH1F*)gROOT -> FindObject("hist3");
    TH1F *hist7 = (TH1F*)gROOT -> FindObject("hist7");
    //hist1 -> Fit("gaus","fitting option","graphics option", x_min, x_MAX);



    TTree *inTree4 = (TTree*)inFile2 -> Get("mcp");

    inTree4 -> Draw("adc_ref >> hist4 ( 200, 0.0, 400.0)", cut1);
    inTree4 -> Draw("adc_ref >> hist8 ( 50, 60.0, 110.0)", cut1);

    TH1F *hist4 = (TH1F*)gROOT -> FindObject("hist4");
    TH1F *hist8 = (TH1F*)gROOT -> FindObject("hist8");
    //hist2 -> Fit("gaus","fitting option","graphics option", x_min, x_MAX);



    char Title1[256];
    sprintf(Title1, "ref. MPPC  |  1 & 12 Oct. 2018 (KT0552 & KT0689); ADC Count / 0.25 pC ; # of Event",);
    char Title2[256];
    sprintf(Title2, "ref. MPPC  |  12 Oct. 2018 (KT0689); ADC Count / 0.25 pC ; # of Event",);


    TCanvas *c1 = new TCanvas("c1","",1300,500);
    c1 -> Divide(2,1);


    c1 -> cd(1);

    hist1 -> SetTitle(Title1); 
    ((TGaxis*)hist1->GetYaxis())->SetMaxDigits(4);
    //hist1 -> SetTitle("ref. MPPC  |  1 Oct. 2018; TDC Count / 25 ps ; # of Event"); 
    hist1 -> SetLineColor(kBlue);
    hist2 -> SetLineColor(kRed);


    TLegend *legend1 = new TLegend(0.63,0.77,0.88,0.88);
    //TLegend *legend1 = new TLegend(0.14,0.77,0.39,0.88);
    legend1 -> AddEntry(hist1,"ref.MPPC (KT0552) in 0 T","l"); 
    legend1 -> AddEntry(hist2,"ref.MPPC (KT0552) in 1.5 T","l"); 
    //legend -> AddEntry(None, "MCP-PMT ID: KT0835;HV = 2,110 V", "");


    hist4 -> Draw();
    hist1 -> Draw("sames");
    hist2 -> Draw("sames");
    legend1 -> Draw("same");


    gPad->Update();

    const Float_t xTR1 = 0.88;
    const Float_t stat_Width = 0.2;
    const Float_t stat_Height = 0.15;

    TPaveStats *stat1 = (TPaveStats*)hist1->FindObject("stats");
    stat1 -> SetX1NDC(xTR1-stat_Width);
    stat1 -> SetY1NDC(0.76-stat_Height);
    stat1 -> SetX2NDC(xTR1);
    stat1 -> SetY2NDC(0.76);
    stat1 -> SetTextColor(kBlue);

    TPaveStats *stat2 = (TPaveStats*)hist2->FindObject("stats");
    stat2 -> SetX1NDC(xTR1-stat_Width);
    stat2 -> SetY1NDC(0.76-2*stat_Height-0.01);
    stat2 -> SetX2NDC(xTR1);
    stat2 -> SetY2NDC(0.76-stat_Height-0.01);
    stat2 -> SetTextColor(kRed);

    c1 -> Modified(); 
    c1 -> Update(); 



    hist4 -> SetTitle(Title1); 
    ((TGaxis*)hist4->GetYaxis())->SetMaxDigits(3);
    //hist1 -> SetTitle("ref. MPPC  |  1 Oct. 2018; TDC Count / 25 ps ; # of Event"); 
    hist3 -> SetLineColor(kGreen);
    hist4 -> SetLineColor(kMagenta);


    //TLegend *legend2 = new TLegend(0.63,0.77,0.88,0.88);
    //TLegend *legend2 = new TLegend(0.14,0.77,0.39,0.88);
    legend1 -> AddEntry(hist3,"ref. MPPC (KT0698) in 0 T","l"); 
    legend1 -> AddEntry(hist4,"ref. MPPC (KT0698) in 1.5 T","l"); 
    //legend -> AddEntry(None, "MCP-PMT ID: KT0835;HV = 2,110 V", "");


    //hist4 -> Draw();
    hist3 -> Draw("sames");
    hist4 -> Draw("same");
    //legend2 -> Draw("same");


    gPad->Update();

    const Float_t xTR2 = 0.88;
    const Float_t stat_Width = 0.2;
    const Float_t stat_Height = 0.15;

    TPaveStats *stat3 = (TPaveStats*)hist3->FindObject("stats");
    stat3 -> SetX1NDC(xTR2-stat_Width);
    stat3 -> SetY1NDC(0.76-3*stat_Height-0.02);
    stat3 -> SetX2NDC(xTR2);
    stat3 -> SetY2NDC(0.76-2*stat_Height-0.02);
    stat3 -> SetTextColor(kGreen);

    TPaveStats *stat4 = (TPaveStats*)hist4->FindObject("stats");
    stat4 -> SetX1NDC(xTR2-stat_Width);
    stat4 -> SetY1NDC(0.76-4*stat_Height-0.03);
    stat4 -> SetX2NDC(xTR2);
    stat4 -> SetY2NDC(0.76-3*stat_Height-0.03);
    stat4 -> SetTextColor(kMagenta);

    c1 -> Modified(); 
    c1 -> Update(); 


    c1 -> cd(2);

    hist5 -> SetTitle(Title1); 
    ((TGaxis*)hist5->GetYaxis())->SetMaxDigits(4);
    //hist6 -> SetTitle("ref. MPPC  |  12 Oct. 2018 (KT0689); TDC Count / 25 ps ; # of Event"); 
    hist5 -> SetLineColor(kBlue);
    hist6 -> SetLineColor(kRed);


    TLegend *legend3 = new TLegend(0.14,0.77,0.39,0.88);
    //TLegend *legend3 = new TLegend(0.14,0.77,0.39,0.88);
    legend3 -> AddEntry(hist5,"ref. MPPC (KT0552) in 0 T","l"); 
    legend3 -> AddEntry(hist6,"ref. MPPC (KT0552) in 1.5 T","l"); 
    //legend -> AddEntry(None, "MCP-PMT ID: KT0835;HV = 2,110 V", "");


    hist8 -> Draw();
    hist5 -> Draw("sames");
    hist6 -> Draw("sames");
    legend3 -> Draw("same");


    gPad->Update();

    const Float_t xTR3 = 0.34;
    const Float_t stat_Width = 0.2;
    const Float_t stat_Height = 0.15;

    TPaveStats *stat5 = (TPaveStats*)hist5->FindObject("stats");
    stat5 -> SetX1NDC(xTR3-stat_Width);
    stat5 -> SetY1NDC(0.76-stat_Height);
    stat5 -> SetX2NDC(xTR3);
    stat5 -> SetY2NDC(0.76);
    stat5 -> SetTextColor(kBlue);

    TPaveStats *stat6 = (TPaveStats*)hist6->FindObject("stats");
    stat6 -> SetX1NDC(xTR3-stat_Width);
    stat6 -> SetY1NDC(0.76-2*stat_Height-0.01);
    stat6 -> SetX2NDC(xTR3);
    stat6 -> SetY2NDC(0.76-stat_Height-0.01);
    stat6 -> SetTextColor(kRed);

    c1 -> Modified(); 
    c1 -> Update(); 


    //hist7 -> SetTitle("KT0552  |  27 Feb. 2016; TDC Count (Ch5) / 25 ps ; # of Event"); 
    ((TGaxis*)hist8->GetYaxis())->SetMaxDigits(4);
    hist8 -> SetTitle(Title1); 
    hist7 -> SetLineColor(kGreen);
    hist8 -> SetLineColor(kMagenta);


    //TLegend *legend4 = new TLegend(0.14,0.77,0.39,0.88);
    //TLegend *legend4 = new TLegend(0.14,0.77,0.39,0.88);
    legend3 -> AddEntry(hist7,"ref. MPPC (KT0698) in 0 T","l"); 
    legend3 -> AddEntry(hist8,"ref. MPPC (KT0698) in 1.5 T","l"); 
    //legend -> AddEntry(None, "MCP-PMT ID: KT0835;HV = 2,110 V", "");


    //hist8 -> Draw();
    hist7 -> Draw("sames");
    hist8 -> Draw("sames");
    //legend4 -> Draw("same");


    gPad->Update();

    const Float_t xTR4 = 0.34;
    const Float_t stat_Width = 0.2;
    const Float_t stat_Height = 0.15;

    TPaveStats *stat7 = (TPaveStats*)hist7->FindObject("stats");
    stat7 -> SetX1NDC(xTR4-stat_Width);
    stat7 -> SetY1NDC(0.76-3*stat_Height-0.01);
    stat7 -> SetX2NDC(xTR4);
    stat7 -> SetY2NDC(0.76-2*stat_Height-0.01);
    stat7 -> SetTextColor(kGreen);

    TPaveStats *stat8 = (TPaveStats*)hist8->FindObject("stats");
    stat8 -> SetX1NDC(xTR4-stat_Width);
    stat8 -> SetY1NDC(0.76-3*stat_Height-0.02);
    stat8 -> SetX2NDC(xTR4);
    stat8 -> SetY2NDC(0.76-4*stat_Height-0.02);
    stat8 -> SetTextColor(kMagenta);

    c1 -> Modified(); 
    c1 -> Update(); 

}
