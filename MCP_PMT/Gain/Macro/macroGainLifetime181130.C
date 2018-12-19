#include <iostream>
#include <sstream>
#include <stdio.h>
#include "TROOT.h"
#include "TH1.h"
#include "TMath.h"

void macroGainLifetime181130(){

    // ### Title&Label Font ### ;
    gStyle ->SetOptFit();
    gStyle ->SetLabelSize(0.05,"x");
    gStyle ->SetLabelSize(0.05,"y");
    gStyle ->SetLabelSize(0.05,"z");
    gStyle ->SetTitleXSize(0.04);
    gStyle ->SetTitleYSize(0.04);
    gStyle ->SetTitleSize(0.04,"z");


    // ### Hit Rate ### ;
    const Int_t NHz = 20;
    Int_t hitRate[NHz]; // --> Hit Rate / PMT ;
    Double_t hitRate0[NHz]; // --> Hit Rate / cm^2 ;
    const Double_t S_eff = 2.3*2.3; 

    for (Int_t k1Hz = 0; k1Hz < NHz; k1Hz++ ){
        hitRate[k1Hz] = ( k1Hz + 1 )*TMath::Power(10,6) ;
        hitRate0[k1Hz] =( k1Hz + 1 )*TMath::Power(10,6)  / S_eff ;
    }

    /*// ### Output Charge ### ;
    const Int_t Ngain = 2; // Nominal gain = 0, 10% higher gain = 1;
    const Int_t Nday = 1024;
    const Int_t oneDay = 24*60*60;
    Double_t outputCharge[Ngain][Nday];
    for ( k2gain = 0; k2gain < Ngain; k2gain++){
        for ( k3day = 0; k3day < Nday; k3day++){
            outputCharge[k2gain][k3day] 
            = ( hitRate[k1Hz]*oneDay ) * k3day * 1.609 * TMath::Power(10,-19) * ( 5.0 + 0.5*k2gain )*TMath::Power(10,5) ;
        }
    }*/


    // ### Lifetime (Time) at Each Hit Rate ### ;
    const Int_t Ntype = 3; // Conventional = 0, ALD =1; Life-extended ALD = 2;
    const Int_t Ngain = 2;
    Int_t n2type;
    Double_t lifetime[Ntype]; // [C/cm^2];
    Double_t gain[Ngain];
    Double_t time[Ntype][Ngain][NHz];

    // # Parameter for Lifetime&Gain #
    lifetime[0] = 1.1;
    lifetime[1] = 10.4;
    lifetime[2] = 13.6;
    for (Int_t k3gain = 0; k3gain < Ngain; k3gain++ ){
        gain[k3gain] = ( 5.0 + 0.5*k3gain ) * TMath::Power(10,5);
    }

    // # Calculate Lifetime (Time) # ;
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        for (Int_t k3gain = 0; k3gain < Ngain; k3gain++ ){
            for (Int_t k1Hz = 0; k1Hz < NHz; k1Hz++ ){
                time[k2type][k3gain][k1Hz] 
                = lifetime[k2type] / ( hitRate0[k1Hz] * 1.609 * TMath::Power(10,-19) * gain[k3gain] );
            }
        }
    }
    
    // # Lifetime (Time) Difference # ;
    Double_t time_diff[Ntype][NHz];
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        for (Int_t k1Hz = 0; k1Hz < NHz; k1Hz++ ){
            time_diff[k2type][k1Hz] = time[k2type][0][k1Hz] - time[k2type][1][k1Hz];
        }
    }



    // ### Graph ### ;
    TGraph *graph1[Ntype]; // --> Hit Rate vs. Lifetime (Time) for 5*10^5 Gain ;
    TGraph *graph2[Ntype]; // --> Hit Rate vs. Lifetime (Time) Difference ;
    TGraph *graph3[Ntype]; // --> Lifetime (Time) for 5*10^5 Gain vs. Lifetime (Time) Difference ;
    TGraph *graph4[Ntype]; // --> Lifetime (Time) for 5*10^5 Gain vs. Lifetime (Time) Difference ;
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        graph1[k2type] = new TGraph();
        graph2[k2type] = new TGraph();
        graph3[k2type] = new TGraph();
        graph4[k2type] = new TGraph();
    }

    // # Hit Rate vs. Lifetime (Time) for 5*10^5 Gain # ;
    // # Hit Rate vs. Lifetime (Time) Difference # ;
    // # Lifetime (Time) for 5*10^5 Gain vs. Lifetime (Time) Difference # ;
    const Int_t oneDay = 24*60*60;
    Double_t hitRateIn;
    Double_t timeIn;
    Double_t time_diffIn;
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        for (Int_t k1Hz = 0; k1Hz < NHz; k1Hz++ ){
            hitRateIn = hitRate[k1Hz] / TMath::Power(10,6);
            timeIn = time[k2type][0][k1Hz]/oneDay;
            time_diffIn = time_diff[k2type][k1Hz]/oneDay;
            graph1[k2type] ->SetPoint( k1Hz, hitRateIn, timeIn);
            graph2[k2type] ->SetPoint( k1Hz, hitRateIn, time_diffIn);
            graph3[k2type] ->SetPoint( k1Hz, timeIn, time_diffIn);
            graph4[k2type] ->SetPoint( k1Hz, timeIn, time_diffIn);
        }
    }



    // ### Function ### ;
    TF1 *func1[Ntype][Ngain]; // --> Hit Rate vs. Lifetime (Time) for 5*10^5 Gain ;
    TF1 *func2[Ntype]; // --> Hit Rate vs. Lifetime (Time) Difference ;
    TF1 *func3; // --> Lifetime (Time) for 5*10^5 Gain vs. Lifetime (Time) Difference ;
    TF1 *func4[Ntype]; // --> Lifetime (Time) for 5*10^5 Gain vs. Lifetime (Time) Difference ;
    char func1Name[128];
    char func2Name[128];
    char func3Name[128];
    char func4Name[128];
    char func1In[256];
    char func2In[256];
    char func3In[256];
    Int_t whichGain;


    // # Hit Rate vs. Lifetime (Time) for 5*10^5 Gain # ;
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        for (Int_t k3gain = 0; k3gain < Ngain; k3gain++){
            //if ( k3gain == 0 ) sprintf( whichGain, "N");
            //if ( k3gain == 1 ) sprintf( whichGain, "H");
            whichGain = k3gain;
            sprintf( func1Name, "func1_%d%d", k2type, whichGain);
            sprintf( func1In, "(%f*%f)/(x*TMath::Power(10,6)*1.609*TMath::Power(10,-19)*%f*%f)",
                    lifetime[k2type], S_eff, gain[k3gain], oneDay);
            func1[k2type][k3gain] = new TF1( func1Name, func1In, 0., 22.);
        }
    }


    // # Hit Rate vs. Lifetime (Time) Difference # ;
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        sprintf( func2Name, "func2_%d", k2type);
        sprintf( func2In, "func1_%d0 - func1_%d1", k2type, k2type);
        func2[k2type] = new TF1( func2Name, func2In, 0., 22.);
    }
    // # Lifetime (Time) for 5*10^5 Gain vs. Lifetime (Time) Difference # ;
    sprintf( func3In, "(1-%f/%f)*x", gain[0],gain[1]);
    func3 = new TF1( "func3", func3In, 0., 11000.);
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        sprintf( func4Name, "func4_%d", k2type);
        func4[k2type] = new TF1( func4Name, func3In, 0., 11000.);
    }



    // ### 2D Histogram ### ;
    // # Hit Rate vs. Lifetime (Time) / Lifetime (Time) Difference # ;
    TH2F *hist2D[Ntype];
    char histName[128];
    std::string whichType;
    n2type = 0;
    whichType = "Conventional";
    sprintf( histName, "hist2D_%s", whichType.c_str());
    hist2D[n2type] = new TH2F( histName, "; Hit Rate / MHz     ; Lifetime / day ; Lifetime Difference / day ", 22, 0., 22., 100, 0., 1100.);
    for (Int_t k2type = 1 ; k2type < Ntype; k2type++ ){
        if ( k2type == 1 ) whichType = "ALD";
        if ( k2type == 2 ) whichType = "Life-extended_ALD";
        sprintf( histName, "hist2D_%s", whichType.c_str());
        hist2D[k2type] = new TH2F( histName, "; Hit Rate / MHz      ; Lifetime / day ; Lifetime Difference / day ", 22, 0., 22., 100, 0., 11000.);
    }

    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        for (Int_t k1Hz = 0; k1Hz < NHz; k1Hz++ ){
            hitRateIn = hitRate[k1Hz] / TMath::Power(10,6);
            timeIn = time[k2type][0][k1Hz]/oneDay;
            time_diffIn = time_diff[k2type][k1Hz]/oneDay;
            //std::cout << hitRateIn << ", " << timeIn << ", " << time_diffIn << std::endl;
            hist2D[k2type] -> Fill(hitRateIn,timeIn,time_diffIn);
        }
    }




    // ### Make Graphs ### ;
    // # Canvas # ;
    TCanvas *c1 = new TCanvas("c1","Lifetime (Time) 5*10^5 Gain",800,1000);
    TCanvas *c2 = new TCanvas("c2","Lifetime (Time) Difference",800,1000);
    TCanvas *c3 = new TCanvas("c3","Lifetime (Time) vs. Difference",1600,500);
    TCanvas *c4 = new TCanvas("c4","Lifetime (Time) vs. Difference",1600,1000);
    TCanvas *c5[Ntype];
    char canvasName[128];
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        sprintf( canvasName, "c5_%d", k2type);
        c5[k2type] = new TCanvas( canvasName,"Hit Rate vs. Lifetime (Time) / Difference",800,1000);
    }

    gStyle ->SetGridStyle(2);
    gStyle ->SetGridColor(kGray+1);

    c1 ->Divide(1,2);
    c2 ->Divide(1,2);
    c3 ->Divide(2,1);
    c4 ->Divide(3,2);
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        c5[k2type] ->Divide(1,2);
    }


    // # Frame # ;
    TH2F *frame1_1 = new TH2F("hist2D1_1","; Hit Rate / MHz ; Lifetime / day ", 100., 0., 21.1, 100., 0., 11000.);
    TH2F *frame1_2 = new TH2F("hist2D1_2","; Hit Rate / MHz ; Lifetime / day ", 100., 0., 21.1, 100., 0., 1000.);
    TH2F *frame2_1 = new TH2F("hist2D2_1","; Hit Rate / MHz ; Lifetime Difference / day ", 100., 0., 21.1, 100., 0., 1000.);
    TH2F *frame2_2 = new TH2F("hist2D2_2","; Hit Rate / MHz ; Lifetime Difference / day ", 100., 0., 21.1, 100., 0., 100.);
    TH2F *frame3_1 = new TH2F("hist2D3_1","; Lifetime / day ; Lifetime Difference / day ", 100., 10., 11000., 100., 0., 1000.);
    TH2F *frame4_1 = new TH2F("hist2D4_1","; Lifetime / day ; Lifetime Difference / day ", 100., 10., 1100., 100., 0., 100.);
    TH2F *frame4_2 = new TH2F("hist2D4_2","; Lifetime / day ; Lifetime Difference / day ", 100., 10., 11000., 100., 0., 1000.);
    TH2F *frame4_3 = new TH2F("hist2D4_3","; Lifetime / day ; Lifetime Difference / day ", 100., 10., 11000., 100., 0., 1000.);
    frame1_1 ->SetStats(0);
    frame1_2 ->SetStats(0);
    frame2_1 ->SetStats(0);
    frame2_2 ->SetStats(0);
    frame3_1 ->SetStats(0);
    frame4_1 ->SetStats(0);
    frame4_2 ->SetStats(0);
    frame4_3 ->SetStats(0);
    ((TGaxis*)frame1_1 ->GetYaxis()) ->SetMaxDigits(4);
    ((TGaxis*)frame1_2 ->GetYaxis()) ->SetMaxDigits(4);
    ((TGaxis*)frame2_1 ->GetYaxis()) ->SetMaxDigits(4);
    ((TGaxis*)frame2_2 ->GetYaxis()) ->SetMaxDigits(4);
    ((TGaxis*)frame3_1 ->GetYaxis()) ->SetMaxDigits(4);
    ((TGaxis*)frame4_1 ->GetYaxis()) ->SetMaxDigits(4);
    ((TGaxis*)frame4_2 ->GetYaxis()) ->SetMaxDigits(4);
    ((TGaxis*)frame4_3 ->GetYaxis()) ->SetMaxDigits(4);
    frame1_1 ->GetYaxis() ->SetTitleOffset(0.8);
    frame1_2 ->GetYaxis() ->SetTitleOffset(1.2);
    frame2_1 ->GetYaxis() ->SetTitleOffset(1.2);
    frame2_2 ->GetYaxis() ->SetTitleOffset(1.0);
    frame3_1 ->GetXaxis() ->SetTitleOffset(1.2);
    frame3_1 ->GetYaxis() ->SetTitleOffset(1.2);
    frame3_1 ->GetXaxis() ->SetMoreLogLabels();
    frame4_1 ->GetYaxis() ->SetTitleOffset(1.3);
    frame4_2 ->GetYaxis() ->SetTitleOffset(1.3);
    frame4_3 ->GetYaxis() ->SetTitleOffset(1.3);



    // # Style of Axis and Title, Label # ;
    for(Int_t k2type = 0; k2type < Ntype; k2type++){
        graph1[k2type] ->SetTitle("; Hit Rate / MHz ; Lifetime / day ");
        graph1[k2type] ->GetXaxis() ->SetTitleSize(0.047);
        graph1[k2type] ->GetYaxis() ->SetTitleSize(0.047);
        graph1[k2type] ->GetXaxis() ->SetTitleOffset(1.);
        graph1[k2type] ->GetYaxis() ->SetTitleOffset(0.7);
        graph1[k2type] ->GetXaxis() ->SetRangeUser(0., 21.1);

        graph2[k2type] ->SetTitle("; Hit Rate / MHz ; Lifetime Difference / day ");
        graph2[k2type] ->GetXaxis() ->SetTitleSize(0.047);
        graph2[k2type] ->GetYaxis() ->SetTitleSize(0.047);
        graph2[k2type] ->GetXaxis() ->SetTitleOffset(1.);
        graph2[k2type] ->GetYaxis() ->SetTitleOffset(0.7);
        graph2[k2type] ->GetXaxis() ->SetRangeUser(0., 21.1);

        graph3[k2type] ->SetTitle("; Lifetime / day ; Lifetime Difference / day ");
        graph3[k2type] ->GetXaxis() ->SetTitleSize(0.047);
        graph3[k2type] ->GetYaxis() ->SetTitleSize(0.047);
        graph3[k2type] ->GetXaxis() ->SetTitleOffset(1.);
        graph3[k2type] ->GetYaxis() ->SetTitleOffset(0.7);

        graph4[k2type] ->SetTitle("; Lifetime / day ; Lifetime Difference / day ");
        graph4[k2type] ->GetXaxis() ->SetTitleSize(0.047);
        graph4[k2type] ->GetYaxis() ->SetTitleSize(0.047);
        graph4[k2type] ->GetXaxis() ->SetTitleOffset(1.);
        graph4[k2type] ->GetYaxis() ->SetTitleOffset(0.7);

        hist2D[k2type] ->GetXaxis() ->SetTitleOffset(1.8);
        hist2D[k2type] ->GetYaxis() ->SetTitleOffset(2.);
        hist2D[k2type] ->GetZaxis() ->SetTitleOffset(1.2);
    } 




    // # Marker and Line, Fill Style # ;
    // --> Canvas 1;
    // Hit Rate vs. Lifetime (Time) for 5*10^5 Gain ;
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        graph1[k2type] ->SetMarkerStyle(8);
        graph1[k2type] ->SetMarkerSize(1.);
        graph1[k2type] ->SetMarkerColor(kBlack);
    }
    // --> Canvas 2;
    // Hit Rate vs. Lifetime (Time) Difference ;
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        graph2[k2type] ->SetMarkerStyle(8);
        graph2[k2type] ->SetMarkerSize(1.);
        graph2[k2type] ->SetMarkerColor(kBlack);
    }
    // --> Canvas 3;
    // Lifetime (Time) for 5*10^5 Gain vs. Lifetime (Time) Difference ;
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        graph3[k2type] ->SetMarkerSize(1.1);
    }
    graph3[2] ->SetMarkerSize(1.2);
    graph3[0] ->SetMarkerStyle(20);
    graph3[1] ->SetMarkerStyle(21);
    graph3[2] ->SetMarkerStyle(22);
    graph3[0] ->SetMarkerColor(kRed);
    graph3[1] ->SetMarkerColor(kOrange+1);
    graph3[2] ->SetMarkerColor(kBlue);
    // --> Canvas 3;
    // Lifetime (Time) for 5*10^5 Gain vs. Lifetime (Time) Difference ; 
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        graph4[k2type] ->SetMarkerSize(1.);
        graph4[k2type] ->SetMarkerStyle(8);
        graph4[k2type] ->SetMarkerColor(kBlack);
    }


    // # Function Line Color # ;
    // --> Canvas1 & Canvas2 ;
    // "Conventional" ;
    func1[0][0] ->SetLineColor(kRed);
    func2[0] ->SetLineColor(kRed);
    // "ALD" ;
    func1[1][0] ->SetLineColor(kOrange+1);
    func2[1] ->SetLineColor(kOrange+1);
    // "Life-extended ALD" ;
    func1[2][0] ->SetLineColor(kBlue);
    func2[2] ->SetLineColor(kBlue);
    // --> Canvas3 ;
    func3 ->SetLineColor(kBlack);
    func3 ->SetLineStyle(2);
    // --> Canvas4 ;
    // "Conventional" ;
    func4[0] ->SetLineColor(kRed);
    // "ALD" ;
    func4[1] ->SetLineColor(kOrange+1);
    // "Life-extended ALD" ;
    func4[2] ->SetLineColor(kBlue);


    // # Text # ;
    // --> Canvas1 & Canvas2 & Canvas3 & Canvas4 ;
    TText *text1_1 = new TText(0.5, 0.5, "");
    TText *text1_2 = new TText(0.5, 0.5, "");
    TText *text1_3 = new TText(0.5, 0.5, "");
    text1_1 ->SetTextColor(kRed);
    text1_2 ->SetTextColor(kOrange+1);
    text1_3 ->SetTextColor(kBlue);
    // --> Canvas5 ;
    TText *text5_1 = new TText(0.5, 0.5, "");
    TText *text5_2 = new TText(0.5, 0.5, "");
    TText *text5_3 = new TText(0.5, 0.5, "");
    text5_1 ->SetTextColor(kBlack);
    text5_2 ->SetTextColor(kBlack);
    text5_3 ->SetTextColor(kBlack);
    text5_1 ->SetTextFont(42);
    text5_2 ->SetTextFont(42);
    text5_3 ->SetTextFont(42);
    text5_2 ->SetTextAngle(90);



    // # Draw Graphs # ;
    // --> Canvas 1 ;
    c1 ->cd(1);
    frame1_1 -> Draw();
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        graph1[k2type] ->Draw("P");
        func1[k2type][0] ->Draw("same");
        graph1[k2type] ->Draw("P");
    }
    gPad ->SetGrid(1,1);
    gPad ->Modified();

    text1_1 ->DrawTextNDC(0.6, 0.84, "Conventional");
    text1_2 ->DrawTextNDC(0.6, 0.785, "ALD");
    text1_3 ->DrawTextNDC(0.6, 0.73, "Life-extended ALD");

    c1 ->cd(2);
    frame1_2 -> Draw();
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        graph1[k2type] ->Draw("P");
        func1[k2type][0] ->Draw("same");
        graph1[k2type] ->Draw("P");
    }
    gPad ->SetGrid(1,1);
    gPad ->Modified();

    text1_1 ->DrawTextNDC(0.6, 0.84, "Conventional");
    text1_2 ->DrawTextNDC(0.6, 0.785, "ALD");
    text1_3 ->DrawTextNDC(0.6, 0.73, "Life-extended ALD");



    // # Draw Graphs # ;
    // --> Canvas 1 ;
    c1 ->cd(1);
    frame1_1 -> Draw();
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        graph1[k2type] ->Draw("P");
        func1[k2type][0] ->Draw("same");
        graph1[k2type] ->Draw("P");
    }
    gPad ->SetGrid(1,1);
    gPad ->Modified();

    text1_1 ->DrawTextNDC(0.6, 0.84, "Conventional");
    text1_2 ->DrawTextNDC(0.6, 0.785, "ALD");
    text1_3 ->DrawTextNDC(0.6, 0.73, "Life-extended ALD");

    c1 ->cd(2);
    frame1_2 -> Draw();
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        graph1[k2type] ->Draw("P");
        func1[k2type][0] ->Draw("same");
        graph1[k2type] ->Draw("P");
    }
    gPad ->SetGrid(1,1);
    gPad ->Modified();

    text1_1 ->DrawTextNDC(0.6, 0.84, "Conventional");
    text1_2 ->DrawTextNDC(0.6, 0.785, "ALD");
    text1_3 ->DrawTextNDC(0.6, 0.73, "Life-extended ALD");


    // --> Canvas 2 ;
    c2 ->cd(1);
    frame2_1 -> Draw();
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        graph2[k2type] ->Draw("P");
        func2[k2type] ->Draw("same");
        graph2[k2type] ->Draw("P");
    }
    gPad ->SetGrid(1,1);
    gPad ->Modified();

    text1_1 ->DrawTextNDC(0.6, 0.84, "Conventional");
    text1_2 ->DrawTextNDC(0.6, 0.785, "ALD");
    text1_3 ->DrawTextNDC(0.6, 0.73, "Life-extended ALD");

    c2 ->cd(2);
    frame2_2 -> Draw();
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        graph2[k2type] ->Draw("P");
        func2[k2type] ->Draw("same");
        graph2[k2type] ->Draw("P");
    }
    gPad ->SetGrid(1,1);
    gPad ->Modified();

    text1_1 ->DrawTextNDC(0.6, 0.84, "Conventional");
    text1_2 ->DrawTextNDC(0.6, 0.785, "ALD");
    text1_3 ->DrawTextNDC(0.6, 0.73, "Life-extended ALD");


    // --> Canvas 3 ;
    c3 ->cd(1);
    frame3_1 ->Draw();
    graph3[2] -> Draw("P");
    func3 ->Draw("same");
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        graph3[k2type] ->Draw("P");
        graph3[k2type] ->Draw("P");
    }
    gPad ->SetGrid(1,1);
    gPad ->Modified();
 
    text1_1 ->DrawTextNDC(0.15, 0.84, "Conventional");
    text1_2 ->DrawTextNDC(0.15, 0.785, "ALD");
    text1_3 ->DrawTextNDC(0.15, 0.73, "Life-extended ALD");

    c3 ->cd(2);
    c3 ->cd(2) ->SetLogx();
    frame3_1 ->Draw();
    func3 ->Draw("same");
    graph3[2] -> Draw("P");
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        graph3[k2type] ->Draw("P");
    }
    gPad ->SetGrid(1,1);
    gPad ->Modified();

    text1_1 ->DrawTextNDC(0.15, 0.84, "Conventional");
    text1_2 ->DrawTextNDC(0.15, 0.785, "ALD");
    text1_3 ->DrawTextNDC(0.15, 0.73, "Life-extended ALD");



    // --> Canvas 4 ;
    // Normal X Axis ;
    c4 ->cd(1);
    frame4_1 ->Draw();
    graph4[0] -> Draw("P");
    func4[0] ->Draw("same");
    graph4[0] -> Draw("P");
    gPad ->SetGrid(1,1);
    gPad ->Modified();
    text1_1 ->DrawTextNDC(0.15, 0.785, "Conventional");

    c4 ->cd(2);
    frame4_2 ->Draw();
    graph4[1] -> Draw("P");
    func4[1] ->Draw("same");
    graph4[1] -> Draw("P");
    gPad ->SetGrid(1,1);
    gPad ->Modified();
    text1_2 ->DrawTextNDC(0.15, 0.785, "ALD");

    c4 ->cd(3);
    frame4_3 ->Draw();
    graph4[2] -> Draw("P");
    func4[2] ->Draw("same");
    graph4[2] -> Draw("P");
    gPad ->SetGrid(1,1);
    gPad ->Modified();
    text1_3 ->DrawTextNDC(0.15, 0.785, "Life-extended ALD");

    // Log Scale X Axis ;
    c4 ->cd(4);
    c4 ->cd(4) ->SetLogx();
    frame4_1 ->Draw();
    graph4[0] -> Draw("P");
    func4[0] ->Draw("same");
    graph4[0] -> Draw("P");
    gPad ->SetGrid(1,1);
    gPad ->Modified();
    text1_1 ->DrawTextNDC(0.15, 0.785, "Conventional");

    c4 ->cd(5);
    c4 ->cd(5) ->SetLogx();
    frame4_2 ->Draw();
    graph4[1] -> Draw("P");
    func4[1] ->Draw("same");
    graph4[1] -> Draw("P");
    gPad ->SetGrid(1,1);
    gPad ->Modified();
    text1_2 ->DrawTextNDC(0.15, 0.785, "ALD");

    c4 ->cd(6);
    c4 ->cd(6) ->SetLogx();
    frame4_3 ->Draw();
    graph4[2] -> Draw("P");
    func4[2] ->Draw("same");
    graph4[2] -> Draw("P");
    gPad ->SetGrid(1,1);
    gPad ->Modified();
    text1_3 ->DrawTextNDC(0.15, 0.785, "Life-extended ALD");

    // --> Canvas5 ;
    for (Int_t k2type = 0; k2type < Ntype; k2type++ ){
        c5[k2type] ->cd(1);
        hist2D[k2type] ->Draw("colz");
        
        text5_1 ->DrawTextNDC(0.7, 0.01, "Hit Rate / MHz");
        text5_2 ->DrawTextNDC(0.03, 0.65, "Lifetime / day ");
        //text5_3 ->DrawTextNDC(0.6, 0.73, "Lifetime Difference / day ");


        c5[k2type] ->cd(2);
        hist2D[k2type] ->Draw("lego2z");
    }
        /*
        c5[0] ->cd();    
        TPaveStats *stat5 = (TPaveStats*)hist2D_Conventional -> FindObject("stats");

        stat5 ->SetX1NDC(0.64); //x座標の始点、４行あるときの初期位置は0.78?
        stat5 ->SetX2NDC(0.84); //x座標の終点、４行あるときの初期位置は0.98?
        stat5 ->SetY1NDC(0.68); //y座標の始点、４行あるときの初期位置は0.775?
        stat5 ->SetY2NDC(0.84); //y座標の終点、４行あるときの初期位置は0.935?
        c5[0] ->Modified();
        */
}






