void macroHistogram1D_181001(){

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


    TFile *inFile1 = new TFile("/chai/sgt1/bfactory/mcppmt_data/b-field_KEK/data4/test/run686.root ");
    TTree *inTree1 = (TTree*)inFile1 -> Get("mcp");

    inTree1 -> Draw("adc[5] >> hist1 ( 50, 90.0, 140.0)","tdc[5] < 1530 && tdc[5] > 1480");

    TH1F *hist1 = (TH1F*)gROOT -> FindObject("hist1");

    //hist1 -> Fit("gaus","fitting option","graphics option", x_min, x_MAX);



    TFile *inFile2 = new TFile("/chai/sgt1/bfactory/mcppmt_data/b-field_KEK/data4/test/run687.root ");
    TTree *inTree2 = (TTree*)inFile2 -> Get("mcp");

    inTree2 -> Draw("adc[5] >> hist2 ( 50, 90.0, 140.0)","tdc[5] < 1530 && tdc[5] > 1480");

    TH1F *hist2 = (TH1F*)gROOT -> FindObject("hist2");

    //hist2 -> Fit("gaus","fitting option","graphics option", x_min, x_MAX);



    TCanvas *c1 = new TCanvas("c1","");
    hist1 -> SetTitle("MPPC at the PMT position  |  1 Oct. 2018; ADC Count / 0.25 pC ; # of Event"); 
    ((TGaxis*)hist1->GetYaxis())->SetMaxDigits(4);
    //hist2 -> SetTitle("ref. MPPC  |  1 Oct. 2018; TDC Count / 25 ps ; # of Event"); 
    hist1 -> SetLineColor(kBlue);
    hist2 -> SetLineColor(kRed);


    const Float_t xleg = 0.44;

    TLegend *legend = new TLegend( xleg-0.30, 0.77, xleg, 0.88);
    legend -> AddEntry(hist1,"MPPC in 0 T","l"); 
    legend -> AddEntry(hist2,"MPPC in 1.5 T","l"); 
    //legend -> AddEntry(None, "MCP-PMT ID: KT0835;HV = 2,110 V", "");


    hist1 -> Draw();
    hist2 -> Draw("sames");
    legend -> Draw("same");


    gPad->Update();

    const Float_t xstat = 0.34;
    const Float_t stat_Width = 0.2;
    const Float_t stat_Height = 0.15;

    TPaveStats *stat1 = (TPaveStats*)hist1->FindObject("stats");
    stat1 -> SetX1NDC(xstat-stat_Width);
    stat1 -> SetY1NDC(0.76-stat_Height);
    stat1 -> SetX2NDC(xstat);
    stat1 -> SetY2NDC(0.76);
    stat1 -> SetTextColor(kBlue);

    TPaveStats *stat2 = (TPaveStats*)hist2->FindObject("stats");
    stat2 -> SetX1NDC(xstat-stat_Width);
    stat2 -> SetY1NDC(0.76-2*stat_Height-0.01);
    stat2 -> SetX2NDC(xstat);
    stat2 -> SetY2NDC(0.76-stat_Height-0.01);
    stat2 -> SetTextColor(kRed);

    c1 -> Modified(); 
    c1 -> Update(); 

}
