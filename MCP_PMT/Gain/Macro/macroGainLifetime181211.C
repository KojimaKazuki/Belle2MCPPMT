#include <iostream>
#include <sstream>
#include <stdio.h>
#include "TROOT.h"
#include "TH1.h"
#include "TMath.h"

void macroGainLifetime181211(){

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

    for (Int_t k3Hz = 0; k3Hz < NHz; k3Hz++ ){
        hitRate[k3Hz] = ( k3Hz + 1 )*TMath::Power(10,6) ;
        hitRate0[k3Hz] =( k3Hz + 1 )*TMath::Power(10,6)  / S_eff ;
    }


    // ### Luminosity ### ;
    const Int_t Nterm = 13;
    Double_t LuminosityMax[Nterm];
    Double_t LuminositySlope[Nterm];
    Int_t days[Nterm];

    // # Luminosity Parameter # ;
    LuminosityMax[0] = 0.055 ;
    LuminosityMax[1] = 0.106 ;
    LuminosityMax[2] = 0.208 ;
    LuminosityMax[3] = 0.400 ;
    LuminosityMax[4] = 5.300 ;
    LuminosityMax[5] = 7.600 ;
    for (Int_t k1term = 6; k1term < Nterm; k1term++ ){
        LuminosityMax[k1term] = 8.000 ;
    }

    days[0] = 0 ;
    days[1] = 90 ;
    days[2] = 60 ;
    days[3] = 160 ;
    for (Int_t k1term = 4; k1term < Nterm-1; k1term++ ){
        days[k1term] = 180 ;
    }
    days[Nterm-1] = 180000 ; //(100 years) ;

    // # Luminosity Increase Ratio # ;
    LuminositySlope[0] = 0.;
    for (Int_t k1term = 1; k1term < 7; k1term++ ){
        LuminositySlope[k1term] = ( LuminosityMax[k1term] - LuminosityMax[k1term-1] ) / double(days[k1term]);
        //std::cout << LuminositySlope[k1term] << std::endl;
    }
    for (Int_t k1term = 7; k1term < Nterm; k1term++ ){
        LuminositySlope[k1term] = 0.;
    }

    // # The Total Number of Days of Luminosity run # ; 
    // Dicide the Matrix Size by NdaysMax ;
    Int_t NdaysIn = 0;
    Int_t NtotalDaysIn[Nterm];
    for (Int_t k1term = 0; k1term < Nterm; k1term++ ){
        if (NdaysIn < days[k1term]){
            NdaysIn = days[k1term];
        }
    }
    const Int_t NdaysMax = NdaysIn;
    // Caluculate the total days until days[k1term] ;
    NtotalDaysIn[0] = 0;
    for (Int_t k1term = 1; k1term < Nterm; k1term++ ){
        NtotalDaysIn[k1term] = NtotalDaysIn[k1term-1] + days[k1term];
    }
    const Int_t NtotalDays = NtotalDaysIn[Nterm-1];
    std::cout << NtotalDays << std::endl;


    // # Luminosity & integral Luminocity Calcuration # ;
    const Int_t oneDay = 24*60*60;
    Double_t Luminosity[Nterm][NdaysMax];
    Double_t LuminosityIn;
    Double_t IntegralLumi[NtotalDays];
    Double_t IntegralLumiIn = 0;
    Int_t totalDaysIn = 0;

    for (Int_t k2day = 0; k2day < NdaysMax; k2day++ ){
        Luminosity[0][k2day] = 0.;
    }
    for (Int_t k1term = 1; k1term < Nterm; k1term++ ){
        for (Int_t k2day = 0; k2day < days[k1term]; k2day++ ){
            LuminosityIn = LuminosityMax[k1term-1] + LuminositySlope[k1term] * k2day ;
            Luminosity[k1term][k2day] = LuminosityIn; // Unit: 10^35 cm^(-2)s^(-1) ;
            IntegralLumiIn += Luminosity[k1term][k2day] * oneDay * TMath::Power(10,-7); // 1ab^(-1) = 10^42 cm^(-2) ;
    	    IntegralLumi[totalDaysIn] = IntegralLumiIn;
    	    totalDaysIn += 1;
    	    //std::cout << totalDaysIn << "," << IntegralLumi[totalDaysIn-1] << "; " ;
        }
        for (Int_t k2day = days[k1term]; k2day < NdaysMax; k2day++ ){
    	    Luminosity[k1term][k2day] = 0.;
        }
    }


    // ### Lifetime (Time) at Each Hit Rate (Lifetime of the Each MCP-PMT Type) ### ;
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
    for (Int_t k5gain = 0; k5gain < Ngain; k5gain++ ){
        gain[k5gain] = ( 5.0 + 0.5*k5gain ) * TMath::Power(10,5);
    }

    // # Calculate Lifetime (Time) # ;
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        for (Int_t k5gain = 0; k5gain < Ngain; k5gain++ ){
            for (Int_t k3Hz = 0; k3Hz < NHz; k3Hz++ ){
                time[k4type][k5gain][k3Hz] 
                = lifetime[k4type] / ( hitRate0[k3Hz] * 1.609 * TMath::Power(10,-19) * gain[k5gain] );
            }
        }
    }
    
    // # Lifetime (Time) Difference # ;
    Double_t time_diff[Ntype][NHz];
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        for (Int_t k3Hz = 0; k3Hz < NHz; k3Hz++ ){
            time_diff[k4type][k3Hz] = time[k4type][0][k3Hz] - time[k4type][1][k3Hz];
        }
    }



   
    // ### Lifetime (Time) at Each Hit Rate (Lifetime at Some Lifetime Output Charges) ### ;
    const Int_t Nref = 10; // 9 Lifetime Value for Reference;
    Int_t n6ref;
    Double_t lifetimeRef[Nref]; // [C/cm^2];
    Double_t gain[Ngain];
    Double_t timeRef[Nref][Ngain][NHz];

    // # Parameter for Lifetime&Gain #
    lifetimeRef[0] = 1.;
    lifetimeRef[1] = 3.;
    lifetimeRef[2] = 5.;
    lifetimeRef[3] = 7.;
    lifetimeRef[4] = 9.;
    lifetimeRef[5] = 10.;
    lifetimeRef[6] = 15.;
    lifetimeRef[7] = 20.;
    lifetimeRef[8] = 25.;
    lifetimeRef[9] = 30.;

    // # Calculate Lifetime (Time) # ;
    for (Int_t k6ref = 0; k6ref < Nref; k6ref++ ){
        for (Int_t k5gain = 0; k5gain < Ngain; k5gain++ ){
            for (Int_t k3Hz = 0; k3Hz < NHz; k3Hz++ ){
                timeRef[k6ref][k5gain][k3Hz] 
                = lifetimeRef[k6ref] / ( hitRate0[k3Hz] * 1.609 * TMath::Power(10,-19) * gain[k5gain] );
            }
        }
    }




    // ### Graph ### ;
    TGraph *graph5 = new TGraph(); // --> Time vs. Luminosity ;
    TGraph *graph6 = new TGraph(); // --> Time vs. Integral Luminosity ;   
    TGraph *graph7[Ntype]; // --> Hit Rate vs. Integral Luminosity ;   
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        graph7[k4type] = new TGraph();
    }
    TGraph *graph8[Nref]; // --> Hit Rate vs. Integral Luminosity ;
    for (Int_t k6ref = 0; k6ref < Nref; k6ref++ ){
        graph8[k6ref] = new TGraph();
    }
    TGraph *graph9[Nref]; // --> Hit Rate vs. Integral Luminosity (Reference Only);
    for (Int_t k6ref = 0; k6ref < Nref; k6ref++ ){
        graph9[k6ref] = new TGraph();
    }


    // # Time vs. Luminosity # ;
    // # Time vs. Integral Luminosity # ;
    totalDaysIn = 0;
    Int_t runTime50 = 0;
    for (Int_t k1term = 0; k1term < Nterm-1; k1term++ ){
        for (Int_t k2day = 0; k2day < days[k1term]; k2day++ ){
            graph5 -> SetPoint( totalDaysIn, totalDaysIn, Luminosity[k1term][k2day]);
            graph6 -> SetPoint( totalDaysIn, totalDaysIn, IntegralLumi[totalDaysIn]);
            if ( IntegralLumi[totalDaysIn] < 50.) runTime50 += 1;
            totalDaysIn += 1;
        }
    }
    std::cout << runTime50 << std::endl;

    // # Hit Rate vs. Lifetime (Luminosity) for 5*10^5 Gain # ;
    Int_t timeIntIn;
    Double_t timeLumiIn;

    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        for (Int_t k3Hz = 0; k3Hz < NHz; k3Hz++ ){
            hitRateIn = hitRate[k3Hz] / TMath::Power(10,6);
            timeIntIn = TMath::Nint( time[k4type][0][k3Hz] / oneDay );
            if ( timeIntIn < NtotalDays[Nterm] ){ 
                timeLumiIn = IntegralLumi[timeIntIn];
                //time_diffIn = time_diff[k4type][k3Hz]/oneDay;
                graph7[k4type] ->SetPoint( k3Hz, hitRateIn, timeLumiIn);
            }
            if ( timeIntIn > NtotalDays[Nterm] ){ 
                graph7[k4type] ->SetPoint( k3Hz, 22., 0.);
            }
        }
    }
    // # Hit Rate vs. Lifetime (Luminosity) for 5*10^5 Gain # ;
    for (Int_t k6ref = 0; k6ref < Nref; k6ref++ ){
        for (Int_t k3Hz = 0; k3Hz < NHz; k3Hz++ ){
            hitRateIn = hitRate[k3Hz] / TMath::Power(10,6);
            timeIntIn = TMath::Nint( timeRef[k6ref][0][k3Hz] / oneDay );
            if ( timeIntIn < NtotalDays[Nterm] ){ 
                timeLumiIn = IntegralLumi[timeIntIn];
                //time_diffIn = time_diff[k4type][k3Hz]/oneDay;
                graph8[k6ref] ->SetPoint( k3Hz, hitRateIn, timeLumiIn);
                graph9[k6ref] ->SetPoint( k3Hz, hitRateIn, timeLumiIn);
            }
            if ( timeIntIn > NtotalDays[Nterm] ){ 
                graph8[k6ref] ->SetPoint( k3Hz, 22., 0.);
                graph9[k6ref] ->SetPoint( k3Hz, 22., 0.);
            }
        }
    }





    // ### Function ### ;
    TF1 *func5; // --> Integral Luminosity 50 ab^(-1) ;
    TF1 *func6[Ngain]; // --> Lifetime vs. Hit Rate until 50 ab^(-1) ;
    char func6Name[128];
    char func6In[256];
    Int_t whichGain;


    // # Luminosity 50 ab^(-1) # ;
    func5 = new TF1( "func5", "50.", 0., 5000.);

    // # Lifetime vs. Hit Rate to use until achieving 50 ab^(-1) Integral Luminosity # ;
    for (Int_t k5gain = 0; k5gain < Ngain; k5gain++ ){
        whichGain = k5gain;
        sprintf( func6Name, "func6_%d", whichGain);
        sprintf( func6In, "(x*%f)/(%f*%f*1.609*TMath::Power(10,-19)*%f*TMath::Power(10,6))", S_eff, runTime50, oneDay, gain[k5gain]);
        func6[k5gain] = new TF1( func6Name, func6In, 0., 31.);
    }




    // ### Line ### ;
    // # Days at Integral Luminosity 50 ab^(-1) #
    TLine *line = new TLine( runTime50, 0., runTime50, 10.5);
    // # Graph 5 : Instant Luminosity # --> Canvas 6 ;
    const Double_t LuminosityMin5 = 0. ;
    const Double_t LuminosityMax5 = 10.5;
    TLine *line5_1 = new TLine( NtotalDaysIn[1], LuminosityMin5, NtotalDaysIn[1], LuminosityMax5);
    TLine *line5_2 = new TLine( NtotalDaysIn[2], LuminosityMin5, NtotalDaysIn[2], LuminosityMax5);
    TLine *line5_3 = new TLine( NtotalDaysIn[3], LuminosityMin5, NtotalDaysIn[3], LuminosityMax5);
    TLine *line5_4 = new TLine( NtotalDaysIn[4], LuminosityMin5, NtotalDaysIn[4], LuminosityMax5);
    TLine *line5_5 = new TLine( NtotalDaysIn[5], LuminosityMin5, NtotalDaysIn[5], LuminosityMax5);
    TLine *line5_6 = new TLine( NtotalDaysIn[6], LuminosityMin5, NtotalDaysIn[6], LuminosityMax5);
    TLine *line5_7 = new TLine( NtotalDaysIn[7], LuminosityMin5, NtotalDaysIn[7], LuminosityMax5);
    TLine *line5_8 = new TLine( NtotalDaysIn[8], LuminosityMin5, NtotalDaysIn[8], LuminosityMax5);
    TLine *line5_9 = new TLine( NtotalDaysIn[9], LuminosityMin5, NtotalDaysIn[9], LuminosityMax5);
    TLine *line5_10 = new TLine( NtotalDaysIn[10], LuminosityMin5, NtotalDaysIn[10], LuminosityMax5);

    // # Graph 6 : Integral Luminosity # --> Canvas 6 ;
    const Double_t IntegralLumiMin6 = 0. ;
    const Double_t IntegralLumiMax6 = 105.;
    TLine *line6_1 = new TLine( NtotalDaysIn[1], IntegralLumiMin6, NtotalDaysIn[1], IntegralLumiMax6);
    TLine *line6_2 = new TLine( NtotalDaysIn[2], IntegralLumiMin6, NtotalDaysIn[2], IntegralLumiMax6);
    TLine *line6_3 = new TLine( NtotalDaysIn[3], IntegralLumiMin6, NtotalDaysIn[3], IntegralLumiMax6);
    TLine *line6_4 = new TLine( NtotalDaysIn[4], IntegralLumiMin6, NtotalDaysIn[4], IntegralLumiMax6);
    TLine *line6_5 = new TLine( NtotalDaysIn[5], IntegralLumiMin6, NtotalDaysIn[5], IntegralLumiMax6);
    TLine *line6_6 = new TLine( NtotalDaysIn[6], IntegralLumiMin6, NtotalDaysIn[6], IntegralLumiMax6);
    TLine *line6_7 = new TLine( NtotalDaysIn[7], IntegralLumiMin6, NtotalDaysIn[7], IntegralLumiMax6);
    TLine *line6_8 = new TLine( NtotalDaysIn[8], IntegralLumiMin6, NtotalDaysIn[8], IntegralLumiMax6);
    TLine *line6_9 = new TLine( NtotalDaysIn[9], IntegralLumiMin6, NtotalDaysIn[9], IntegralLumiMax6);
    TLine *line6_10 = new TLine( NtotalDaysIn[10], IntegralLumiMin6, NtotalDaysIn[10], IntegralLumiMax6);




    // ### Make Graphs ### ;
    // ### Canvas ### ;
    TCanvas *c6 = new TCanvas("c6","Luminosity",800,1000);
    TCanvas *c7 = new TCanvas("c7","Lifetime (Luminosity) 5*10^5 Gain",800,1000);
    TCanvas *c8 = new TCanvas("c8","Lifetime (Luminosity) Reference 5*10^5 Gain",800,1000);
    TCanvas *c9 = new TCanvas("c9","Lifetime (Luminosity) Reference Only 5*10^5 Gain",800,1000);
    TCanvas *c10 = new TCanvas("c10","Lifetime (Output Charge) vs. Hit Rate",800,500);


    gStyle ->SetGridStyle(2);
    gStyle ->SetGridColor(kGray+1);

    c6 ->Divide(1,2);
    c7 ->Divide(1,2);
    c8 ->Divide(1,2);
    c9 ->Divide(1,2);



    // ### Frame ### ;
    TH2F *frame7_1 = new TH2F("hist2D7_1","; Hit Rate / MHz ; Lifetime (Integral Luminosity) / ab^{-1} ", 100., 0., 21.1, 100., 0., 105.);
    TH2F *frame7_2 = new TH2F("hist2D7_2","; Hit Rate / MHz ; Lifetime (Integral Luminosity) / ab^{-1} ", 100., 0., 21.1, 100., 0., 3.05);
    TH2F *frame8_1 = new TH2F("hist2D8_1","; Hit Rate / MHz ; Lifetime (Integral Luminosity) / ab^{-1} ", 100., 0., 21.1, 100., 0., 105.);
    TH2F *frame8_2 = new TH2F("hist2D8_2","; Hit Rate / MHz ; Lifetime (Integral Luminosity) / ab^{-1} ", 100., 0., 21.1, 100., 0., 3.05);
    TH2F *frame10_1 = new TH2F("hist2D10_1","; Lifetime (Accumlated Output Charge) / C/cm^{2} ; Average Hit Rate / MHz ", 100., 0., 30.5, 100., 0., 21.1);
    frame7_1 ->SetStats(0);
    frame7_2 ->SetStats(0);
    frame8_1 ->SetStats(0);
    frame8_2 ->SetStats(0);
    frame10_1 ->SetStats(0);
    ((TGaxis*)frame7_1 ->GetYaxis()) ->SetMaxDigits(4);
    ((TGaxis*)frame7_2 ->GetYaxis()) ->SetMaxDigits(4);
    ((TGaxis*)frame8_1 ->GetYaxis()) ->SetMaxDigits(4);
    ((TGaxis*)frame8_2 ->GetYaxis()) ->SetMaxDigits(4);
    ((TGaxis*)frame10_1 ->GetYaxis()) ->SetMaxDigits(4);

    frame7_1 ->GetXaxis() ->SetTitleSize(0.05);
    frame7_2 ->GetXaxis() ->SetTitleSize(0.05);
    frame8_1 ->GetXaxis() ->SetTitleSize(0.05);
    frame8_2 ->GetXaxis() ->SetTitleSize(0.05);
    frame10_1 ->GetXaxis() ->SetTitleSize(0.05);

    frame7_1 ->GetYaxis() ->SetTitleSize(0.05);
    frame7_2 ->GetYaxis() ->SetTitleSize(0.05);
    frame8_1 ->GetYaxis() ->SetTitleSize(0.05);
    frame8_2 ->GetYaxis() ->SetTitleSize(0.05);
    frame10_1 ->GetYaxis() ->SetTitleSize(0.05);

    frame7_1 ->GetYaxis() ->SetTitleOffset(1.);
    frame7_2 ->GetYaxis() ->SetTitleOffset(1.);
    frame8_1 ->GetYaxis() ->SetTitleOffset(1.);
    frame8_2 ->GetYaxis() ->SetTitleOffset(1.);
    frame10_1 ->GetXaxis() ->SetTitleOffset(0.9);
    frame10_1 ->GetYaxis() ->SetTitleOffset(0.8);




    // ### Style ### ;
    // # Style of Axis and Title, Label # ;
    graph5 ->SetTitle("; Run Time / day ; Luminosity / 10^{35} cm^{-2}s^{-1} ");
    graph5 ->GetXaxis() ->SetTitleSize(0.047);
    graph5 ->GetYaxis() ->SetTitleSize(0.047);
    graph5 ->GetXaxis() ->SetTitleOffset(1.);
    graph5 ->GetYaxis() ->SetTitleOffset(0.8);
    graph5 ->GetYaxis() ->SetRangeUser(0., 10.5);
    
    graph6 ->SetTitle("; Run Time / day ; Integral Luminosity / ab^{-1} ");
    graph6 ->GetXaxis() ->SetTitleSize(0.047);
    graph6 ->GetYaxis() ->SetTitleSize(0.047);
    graph6 ->GetXaxis() ->SetTitleOffset(1.);
    graph6 ->GetYaxis() ->SetTitleOffset(1.);
    graph6 ->GetYaxis() ->SetRangeUser(0., 105.);

    for (Int_t k4type = 0; k4type < Ntype; k4type++){
        graph7[k4type] ->SetTitle("; Hit Rate / MHz ; Lifetime (Integral Luminosity) / ab^{-1} ");
        graph7[k4type] ->GetXaxis() ->SetTitleSize(0.047);
        graph7[k4type] ->GetYaxis() ->SetTitleSize(0.047);
        graph7[k4type] ->GetXaxis() ->SetTitleOffset(1.);
        graph7[k4type] ->GetYaxis() ->SetTitleOffset(1.);
        graph7[k4type] ->GetXaxis() ->SetRangeUser(0., 100.1);
    }

    for (Int_t k6ref = 0; k6ref < Nref; k6ref++){
        graph8[k6ref] ->SetTitle("; Hit Rate / MHz ; Lifetime (Integral Luminosity) / ab^{-1} ");
        graph8[k6ref] ->GetXaxis() ->SetTitleSize(0.047);
        graph8[k6ref] ->GetYaxis() ->SetTitleSize(0.047);
        graph8[k6ref] ->GetXaxis() ->SetTitleOffset(1.);
        graph8[k6ref] ->GetYaxis() ->SetTitleOffset(1.);
        graph8[k6ref] ->GetXaxis() ->SetRangeUser(0., 100.1);
    }

    for (Int_t k6ref = 0; k6ref < Nref; k6ref++){
        graph9[k6ref] ->SetTitle("; Hit Rate / MHz ; Lifetime (Integral Luminosity) / ab^{-1} ");
        graph9[k6ref] ->GetXaxis() ->SetTitleSize(0.047);
        graph9[k6ref] ->GetYaxis() ->SetTitleSize(0.047);
        graph9[k6ref] ->GetXaxis() ->SetTitleOffset(1.);
        graph9[k6ref] ->GetYaxis() ->SetTitleOffset(1.);
        graph9[k6ref] ->GetXaxis() ->SetRangeUser(0., 100.1);
    }

    
    // # Marker and Line, Fill Style # ;
    // --> Canvas 6 ;
    // Luminosity ;
    graph5 ->SetMarkerSize(1.);
    graph5 ->SetMarkerStyle(7);
    graph5 ->SetMarkerColor(kBlue);
    // Integral Luminosity ;
    graph6 ->SetMarkerSize(1.);
    graph6 ->SetMarkerStyle(7);
    graph6 ->SetMarkerColor(kBlue);
    // --> Canvas 7 ;
    // Hit Rate vs. Lifetime (Luminosity) for 5*10^5 Gain ;
    for (Int_t k4type = 0; k4type < Ntype; k4type++){
        graph7[k4type] ->SetMarkerSize(1.5);
        //graph7[k4type] ->SetMarkerStyle(8);
        graph7[k4type] ->SetMarkerColor(kBlack);
    }
    graph7[0] ->SetMarkerStyle(20);
    graph7[1] ->SetMarkerStyle(21);
    graph7[2] ->SetMarkerStyle(22);
    graph7[0] ->SetMarkerColor(kRed);
    graph7[1] ->SetMarkerColor(kOrange+1);
    graph7[2] ->SetMarkerColor(kBlue);
    graph7[0] ->SetLineColor(kRed);
    graph7[1] ->SetLineColor(kOrange+1);
    graph7[2] ->SetLineColor(kBlue);
    // --> Canvas 8 ;
    // Hit Rate vs. Lifetime (Luminosity) for 5*10^5 Gain ;
    for (Int_t k6ref = 0; k6ref < Nref; k6ref++){
        graph8[k6ref] ->SetMarkerSize(1.3);
        graph8[k6ref] ->SetMarkerColor(kGray+1);
        graph8[k6ref] ->SetLineColor(kGray+1);
    }
    graph8[0] ->SetMarkerSize(1.1);
    graph8[1] ->SetMarkerSize(1.1);
    graph8[3] ->SetMarkerSize(1.5);
    graph8[7] ->SetMarkerSize(1.1);
    graph8[8] ->SetMarkerSize(1.1);
    graph8[0] ->SetMarkerStyle(20);
    graph8[1] ->SetMarkerStyle(25);
    graph8[2] ->SetMarkerStyle(22);
    graph8[3] ->SetMarkerStyle(27);
    graph8[4] ->SetMarkerStyle(34);
    graph8[5] ->SetMarkerStyle(30);
    graph8[6] ->SetMarkerStyle(23);
    graph8[7] ->SetMarkerStyle(24);
    graph8[8] ->SetMarkerStyle(21);
    graph8[9] ->SetMarkerStyle(26);

    /*
    graph8[0] ->SetMarkerStyle(24);
    graph8[1] ->SetMarkerStyle(25);
    graph8[2] ->SetMarkerStyle(26);
    graph8[3] ->SetMarkerStyle(27);
    graph8[4] ->SetMarkerStyle(28);
    graph8[5] ->SetMarkerStyle(30);
    graph8[6] ->SetMarkerStyle(32);
    */ 
    // --> Canvas 9 ;
    // Hit Rate vs. Lifetime (Luminosity) for 5*10^5 Gain (Reference Only) ;
    for (Int_t k6ref = 0; k6ref < Nref; k6ref++){
        graph9[k6ref] ->SetMarkerSize(1.4);
        graph9[k6ref] ->SetMarkerColor(kBlack);
        graph9[k6ref] ->SetLineColor(kBlack);
    }
    graph9[0] ->SetMarkerSize(1.2);
    graph9[1] ->SetMarkerSize(1.2);
    graph9[3] ->SetMarkerSize(1.7);
    graph9[7] ->SetMarkerSize(1.2);
    graph9[8] ->SetMarkerSize(1.2);
    graph9[0] ->SetMarkerStyle(20);
    graph9[1] ->SetMarkerStyle(25);
    graph9[2] ->SetMarkerStyle(22);
    graph9[3] ->SetMarkerStyle(27);
    graph9[4] ->SetMarkerStyle(34);
    graph9[5] ->SetMarkerStyle(30);
    graph9[6] ->SetMarkerStyle(23);
    graph9[7] ->SetMarkerStyle(24);
    graph9[8] ->SetMarkerStyle(21);
    graph9[9] ->SetMarkerStyle(26);



    // # Function Line Color # ;
    // --> Canvas 6 & Canvas 7 ;
    func5 ->SetLineColor(kMagenta);
    // --> Canvas 9 ;
    func6[0] ->SetLineColor(kBlue);
    func6[1] ->SetLineColor(kRed);


    // # Line Color # ;
    // --> days at the Instant Luminosity 50 ab^(-1) ;
    line ->SetLineColor(kMagenta);
    line ->SetLineStyle(1);
    line ->SetLineWidth(2);
    // --> Graph 5 ;
    line5_1 ->SetLineColor(kGray+1);
    line5_2 ->SetLineColor(kGray+1);
    line5_3 ->SetLineColor(kGray+1);
    line5_4 ->SetLineColor(kGray+1);
    line5_5 ->SetLineColor(kGray+1);
    line5_6 ->SetLineColor(kGray+1);
    line5_7 ->SetLineColor(kGray+1);
    line5_8 ->SetLineColor(kGray+1);
    line5_9 ->SetLineColor(kGray+1);
    line5_10 ->SetLineColor(kGray+1);
    line5_1 ->SetLineStyle(7);
    line5_2 ->SetLineStyle(7);
    line5_3 ->SetLineStyle(7);
    line5_4 ->SetLineStyle(7);
    line5_5 ->SetLineStyle(7);
    line5_6 ->SetLineStyle(7);
    line5_7 ->SetLineStyle(7);
    line5_8 ->SetLineStyle(7);
    line5_9 ->SetLineStyle(7);
    line5_10 ->SetLineStyle(7);
    line5_1 ->SetLineWidth(2);
    line5_2 ->SetLineWidth(2);
    line5_3 ->SetLineWidth(2);
    line5_4 ->SetLineWidth(2);
    line5_5 ->SetLineWidth(2);
    line5_6 ->SetLineWidth(2);
    line5_7 ->SetLineWidth(2);
    line5_8 ->SetLineWidth(2);
    line5_9 ->SetLineWidth(2);
    line5_10 ->SetLineWidth(2);

    // --> Graph 6 ;
    line6_1 ->SetLineColor(kGray+1);
    line6_2 ->SetLineColor(kGray+1);
    line6_3 ->SetLineColor(kGray+1);
    line6_4 ->SetLineColor(kGray+1);
    line6_5 ->SetLineColor(kGray+1);
    line6_6 ->SetLineColor(kGray+1);
    line6_7 ->SetLineColor(kGray+1);
    line6_8 ->SetLineColor(kGray+1);
    line6_9 ->SetLineColor(kGray+1);
    line6_10 ->SetLineColor(kGray+1);
    line6_1 ->SetLineStyle(7);
    line6_2 ->SetLineStyle(7);
    line6_3 ->SetLineStyle(7);
    line6_4 ->SetLineStyle(7);
    line6_5 ->SetLineStyle(7);
    line6_6 ->SetLineStyle(7);
    line6_7 ->SetLineStyle(7);
    line6_8 ->SetLineStyle(7);
    line6_9 ->SetLineStyle(7);
    line6_10 ->SetLineStyle(7);
    line6_1 ->SetLineWidth(2);
    line6_2 ->SetLineWidth(2);
    line6_3 ->SetLineWidth(2);
    line6_4 ->SetLineWidth(2);
    line6_5 ->SetLineWidth(2);
    line6_6 ->SetLineWidth(2);
    line6_7 ->SetLineWidth(2);
    line6_8 ->SetLineWidth(2);
    line6_9 ->SetLineWidth(2);
    line6_10 ->SetLineWidth(2);




    // ### Text ### ;
    // --> Canvas1 & Canvas2 & Canvas3 & Canvas4 ;
    TText *text1_1 = new TText(0.5, 0.5, "");
    TText *text1_2 = new TText(0.5, 0.5, "");
    TText *text1_3 = new TText(0.5, 0.5, "");
    text1_1 ->SetTextColor(kRed);
    text1_2 ->SetTextColor(kOrange+1);
    text1_3 ->SetTextColor(kBlue);




    // ### Draw Graphs ### ;
    // --> Canvas 6 ;
    c6 ->cd(1);
    graph5 ->Draw("AP");
    line5_1 ->Draw("same");
    line5_2 ->Draw("same");
    line5_3 ->Draw("same");
    line5_4 ->Draw("same");
    line5_5 ->Draw("same");
    line5_6 ->Draw("same");
    line5_7 ->Draw("same");
    line5_8 ->Draw("same");
    line5_9 ->Draw("same");
    line5_10 ->Draw("same");
    line ->Draw("same");
    graph5 ->Draw("P");
    gPad ->SetGrid(0,1);
    gPad ->Modified();

    c6 ->cd(2);
    graph6 ->Draw("AP");
    line6_1 ->Draw("same");
    line6_2 ->Draw("same");
    line6_3 ->Draw("same");
    line6_4 ->Draw("same");
    line6_5 ->Draw("same");
    line6_6 ->Draw("same");
    line6_7 ->Draw("same");
    line6_8 ->Draw("same");
    line6_9 ->Draw("same");
    line6_10 ->Draw("same");
    func5 ->Draw("same");
    graph6 ->Draw("P");
    gPad ->SetGrid(0,1);
    gPad ->Modified();


    // --> Canvas 7 ;
    c7 ->cd(1);
    frame7_1 ->Draw();
    func5 ->Draw("same");
    for (Int_t k4type = 0; k4type < Ntype; k4type++){
        graph7[k4type] ->Draw("P");
    }
    gPad ->SetGrid(1,1);
    gPad ->Modified();

    text1_1 ->DrawTextNDC(0.6, 0.84, "Conventional");
    text1_2 ->DrawTextNDC(0.6, 0.785, "ALD");
    text1_3 ->DrawTextNDC(0.6, 0.73, "Life-extended ALD");

    c7 ->cd(2);
    frame7_2 ->Draw();
    for (Int_t k4type = 0; k4type < Ntype; k4type++){
        graph7[k4type] ->Draw("P");
    }
    gPad ->SetGrid(1,1);
    gPad ->Modified();

    text1_1 ->DrawTextNDC(0.6, 0.44, "Conventional");
    text1_2 ->DrawTextNDC(0.6, 0.385, "ALD");
    text1_3 ->DrawTextNDC(0.6, 0.33, "Life-extended ALD");


    // --> Canvas 8 ;
    c8 ->cd(1);
    frame8_1 ->Draw();
    func5 ->Draw("same");
    for (Int_t k6ref = 0; k6ref < Nref; k6ref++ ){
        graph8[k6ref] ->Draw("PL");
    }
    for (Int_t k4type = 0; k4type < Ntype; k4type++){
        graph7[k4type] ->Draw("PL");
    }
    gPad ->SetGrid(1,1);
    gPad ->Modified();

    text1_1 ->DrawTextNDC(0.65, 0.84, "Conventional");
    text1_2 ->DrawTextNDC(0.65, 0.785, "ALD");
    text1_3 ->DrawTextNDC(0.65, 0.73, "Life-extended ALD");

    c8 ->cd(2);
    frame8_2 ->Draw();
    for (Int_t k6ref = 0; k6ref < Nref; k6ref++ ){
        graph8[k6ref] ->Draw("PL");
    }
    for (Int_t k4type = 0; k4type < Ntype; k4type++){
        graph7[k4type] ->Draw("PL");
    }
    gPad ->SetGrid(1,1);
    gPad ->Modified();

    text1_1 ->DrawTextNDC(0.22, 0.69, "Conventional");
    text1_2 ->DrawTextNDC(0.22, 0.635, "ALD");
    text1_3 ->DrawTextNDC(0.22, 0.58, "Life-extended ALD");


    // --> Canvas 9 ;
    c9 ->cd(1);
    frame8_1 ->Draw();
    func5 ->Draw("same");
    for (Int_t k6ref = 0; k6ref < Nref; k6ref++ ){
        graph9[k6ref] ->Draw("PL");
    }
    gPad ->SetGrid(1,1);
    gPad ->Modified();

    c9 ->cd(2);
    frame8_2 ->Draw();
    for (Int_t k6ref = 0; k6ref < Nref; k6ref++ ){
        graph9[k6ref] ->Draw("PL");
    }
    gPad ->SetGrid(1,1);
    gPad ->Modified();


    // --> Canvas 9 ;
    c10 ->cd();
    frame10_1 ->Draw();
    for (Int_t k5gain =0; k5gain < Ngain; k5gain++ ){
        func6[k5gain] ->Draw("same");
    }
    gPad ->SetGrid(1,1);
    gPad ->Modified();

    text1_1 ->DrawTextNDC(0.22, 0.635, "10% Higher Gain");
    text1_3 ->DrawTextNDC(0.22, 0.69, "Nominal Gain");


}






