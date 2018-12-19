void macroHistogram1D(){

    gStyle ->SetOptFit(stat);
    gStyle ->SetStatX(x = 0.88); //y at the top-right point of box;
    gStyle ->SetStatY(y = 0.88); //y at the top-right point of box;
    gStyle ->SetLabelSize(0.04,"x");
    gStyle ->SetLabelSize(0.04,"y");
    gStyle ->SetTitleXSize(0.04);
    gStyle ->SetTitleYSize(0.04);

    TFile *inFile = new TFile("/home/bqt/kojima/file_name.root");
    TTree *inTree = (TTree*)inFile -> Get("Tree_name");

    inTree -> Draw("brunch_name  >> hist1 (nbins_x, x_min, x_MAX)","option_(example)abs(DST_mcPDG) == 413");

    TH1F *hist1 = (TH1F*)gROOT -> FindObject("hist1");

    hist1 -> Fit("gaus","fitting option","graphics option", x_min, x_MAX);


    Tcanvas *c1 = new TCanvas("c1","");
    hist1 -> SetTitle("title; x_Title ; Y_title"); 

    Tlegend *legend = new TLegend(0.3,0.73,0.9,0.9)
    legend -> AddEntry(hist1,"Ch1 on the side of PMT","lf") 
    legend -> AddEntry(None, "MCP-PMT ID: KT0835;HV = 2,110 V", "")


    hist1 -> Draw();
    legend -> Draw("same");
