void macro_reference(TString fname){

  gROOT -> Reset();

  TString fileName = "/home/takt/mcp/data/";
  fileName += fname;

  TFile *inFile = new TFile(fileName);
  TTree *inTree_off = (TTree*)inFile -> Get("mcp_ref");
  TTree *inTree_on = (TTree*)inFile -> Get("mcp");

  inTree_off -> Draw("adc_ref>>h0(300,0,300)","");
  TH1F *h0 = (TH1F*)gROOT -> FindObject("h0");
  inTree_on -> Draw("adc_ref>>h1(300,0,300)","");
  TH1F *h1 = (TH1F*)gROOT -> FindObject("h1");

  h0 -> SetLineColor(kRed);
  h0 -> Draw();
  h1 -> SetLineColor(kBlue);
  h1 -> Draw("same");

}
