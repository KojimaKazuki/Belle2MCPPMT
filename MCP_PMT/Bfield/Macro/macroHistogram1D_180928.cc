void macroHistogram1D_180928(){

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


    TFile *inFile = new TFile("/chai/sgt1/bfactory/mcppmt_data/b-field_KEK/data4/test/run682.root ");
    TTree *inTree = (TTree*)inFile -> Get("mcp");

    inTree -> Draw("tdc[5] >> hist1 (200, 0.0, 4000.0)","tdc[5] > 0");

    TH1F *hist1 = (TH1F*)gROOT -> FindObject("hist1");

    //hist1 -> Fit("gaus","fitting option","graphics option", x_min, x_MAX);


    TCanvas *c1 = new TCanvas("c1","");
    hist1 -> SetTitle("MPPC at the PMT position  |  26 Sep. 2018; TDC Count / 25 ps ; # of Event"); 
    ((TGaxis*)hist1->GetYaxis())->SetMaxDigits(4);

    //Tlegend *legend = new TLegend(0.3,0.73,0.9,0.9)
    //legend -> AddEntry(hist1,"Ch1 on the side of PMT","lf") 
    //legend -> AddEntry(None, "MCP-PMT ID: KT0835;HV = 2,110 V", "")


    hist1 -> Draw();
    //legend -> Draw("same");

}
