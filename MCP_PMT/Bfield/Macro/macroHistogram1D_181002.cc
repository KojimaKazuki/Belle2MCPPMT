void macroHistogram1D_181002(){

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


    TFile *inFile1 = new TFile("/chai/sgt1/bfactory/mcppmt_data/b-field_KEK/data2/KT0552_20160227.root ");
    TTree *inTree1 = (TTree*)inFile1 -> Get("mcp_ref");

    inTree1 -> Draw("adc[0] >> hist1 ( 200, 0.0, 400.0)","ipos == 0 && tdc[0] > 1500 && tdc[0] < 1600");

    TH1F *hist1 = (TH1F*)gROOT -> FindObject("hist1");
    //hist1 -> Fit("gaus","fitting option","graphics option", x_min, x_MAX);



    TTree *inTree2 = (TTree*)inFile1 -> Get("mcp");

    inTree2 -> Draw("adc[0] >> hist2 ( 200, 0.0, 400.0)","ipos == 0 && tdc[0] > 1500 && tdc[0] < 1600");

    TH1F *hist2 = (TH1F*)gROOT -> FindObject("hist2");
    //hist2 -> Fit("gaus","fitting option","graphics option", x_min, x_MAX);



    TCanvas *c1 = new TCanvas("c1","");
    hist2 -> SetTitle("KT0552  |  27 Feb. 2016; ADC Count (Ch1) / 0.25 pC ; # of Event"); 
    ((TGaxis*)hist2->GetYaxis())->SetMaxDigits(4);
    //hist1 -> SetTitle("ref. MPPC  |  1 Oct. 2018; TDC Count / 25 ps ; # of Event"); 
    hist1 -> SetLineColor(kBlue);
    hist2 -> SetLineColor(kRed);


    TLegend *legend = new TLegend(0.63,0.77,0.88,0.88);
    legend -> AddEntry(hist1,"KT0552 in 0 T","l"); 
    legend -> AddEntry(hist2,"KT0552 in 1.5 T","l"); 
    //legend -> AddEntry(None, "MCP-PMT ID: KT0835;HV = 2,110 V", "");


    hist2 -> Draw();
    hist1 -> Draw("sames");
    hist2 -> Draw("same");
    legend -> Draw("same");


    gPad->Update();

    const Float_t stat_Width = 0.2;
    const Float_t stat_Height = 0.15;

    TPaveStats *stat1 = (TPaveStats*)hist1->FindObject("stats");
    stat1 -> SetX1NDC(0.88-stat_Width);
    stat1 -> SetY1NDC(0.76-stat_Height);
    stat1 -> SetX2NDC(0.88);
    stat1 -> SetY2NDC(0.76);
    stat1 -> SetTextColor(kBlue);

    TPaveStats *stat2 = (TPaveStats*)hist2->FindObject("stats");
    stat2 -> SetX1NDC(0.88-stat_Width);
    stat2 -> SetY1NDC(0.76-2*stat_Height-0.01);
    stat2 -> SetX2NDC(0.88);
    stat2 -> SetY2NDC(0.76-stat_Height-0.01);
    stat2 -> SetTextColor(kRed);

    c1 -> Modified(); 
    c1 -> Update(); 

}
