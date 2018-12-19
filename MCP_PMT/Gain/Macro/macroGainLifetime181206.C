#include <iostream>
#include <sstream>
#include <stdio.h>
#include "TROOT.h"
#include "TH1.h"
#include "TMath.h"

void macroGainLifetime181206(){

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
    const Int_t Nterm = 11;
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
    days[Nterm-1] = 180 ; //(1 years) ;

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


    /*// ### Output Charge ### ;
    const Int_t Ngain = 2; // Nominal gain = 0, 10% higher gain = 1;
    const Int_t Nday = 1024;
    const Int_t oneDay = 24*60*60;
    Double_t outputCharge[Ngain][Nday];
    for ( k5gain = 0; k5gain < Ngain; k5gain++){
        for ( k2day = 0; k2day < Nday; k2day++){
            outputCharge[k5gain][k2day] 
            = ( hitRate[k3Hz]*oneDay ) * k2day * 1.609 * TMath::Power(10,-19) * ( 5.0 + 0.5*k5gain )*TMath::Power(10,5) ;
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



    // ### Graph ### ;
    TGraph *graph1[Ntype]; // --> Hit Rate vs. Lifetime (Time) for 5*10^5 Gain ;
    TGraph *graph2[Ntype]; // --> Hit Rate vs. Lifetime (Time) Difference ;
    TGraph *graph3[Ntype]; // --> Lifetime (Time) for 5*10^5 Gain vs. Lifetime (Time) Difference ;
    TGraph *graph4[Ntype]; // --> Lifetime (Time) for 5*10^5 Gain vs. Lifetime (Time) Difference ;
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        graph1[k4type] = new TGraph();
        graph2[k4type] = new TGraph();
        graph3[k4type] = new TGraph();
        graph4[k4type] = new TGraph();
    }
    TGraph *graph5 = new TGraph(); // --> Time vs. Luminosity ;
    TGraph *graph6 = new TGraph(); // --> Time vs. Integral Luminosity ;   
    TGraph *graph7[Ntype]; // --> Hit Rate vs. Integral Luminosity ;   
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        graph7[k4type] = new TGraph();
    }


    // # Hit Rate vs. Lifetime (Time) for 5*10^5 Gain # ;
    // # Hit Rate vs. Lifetime (Time) Difference # ;
    // # Lifetime (Time) for 5*10^5 Gain vs. Lifetime (Time) Difference # ;
    Double_t hitRateIn;
    Double_t timeIn;
    Double_t time_diffIn;
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        for (Int_t k3Hz = 0; k3Hz < NHz; k3Hz++ ){
            hitRateIn = hitRate[k3Hz] / TMath::Power(10,6);
            timeIn = time[k4type][0][k3Hz] / oneDay;
            time_diffIn = time_diff[k4type][k3Hz] / oneDay;
            graph1[k4type] ->SetPoint( k3Hz, hitRateIn, timeIn);
            graph2[k4type] ->SetPoint( k3Hz, hitRateIn, time_diffIn);
            graph3[k4type] ->SetPoint( k3Hz, timeIn, time_diffIn);
            graph4[k4type] ->SetPoint( k3Hz, timeIn, time_diffIn);
        }
    }

    // # Time vs. Luminosity # ;
    // # Time vs. Integral Luminosity # ;
    totalDaysIn = 0;
    Int_t runTime50 = 0;
    for (Int_t k1term = 0; k1term < Nterm; k1term++ ){
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




    // ### Function ### ;
    TF1 *func1[Ntype][Ngain]; // --> Hit Rate vs. Lifetime (Time) for 5*10^5 Gain ;
    TF1 *func2[Ntype]; // --> Hit Rate vs. Lifetime (Time) Difference ;
    TF1 *func3; // --> Lifetime (Time) for 5*10^5 Gain vs. Lifetime (Time) Difference ;
    TF1 *func4[Ntype]; // --> Lifetime (Time) for 5*10^5 Gain vs. Lifetime (Time) Difference ;
    TF1 *func5; // --> Integral Luminosity 50 ab^(-1)
    char func1Name[128];
    char func2Name[128];
    char func3Name[128];
    char func4Name[128];
    char func1In[256];
    char func2In[256];
    char func3In[256];
    Int_t whichGain;


    // # Hit Rate vs. Lifetime (Time) for 5*10^5 Gain # ;
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        for (Int_t k5gain = 0; k5gain < Ngain; k5gain++){
            //if ( k5gain == 0 ) sprintf( whichGain, "N");
            //if ( k5gain == 1 ) sprintf( whichGain, "H");
            whichGain = k5gain;
            sprintf( func1Name, "func1_%d%d", k4type, whichGain);
            sprintf( func1In, "(%f*%f)/(x*TMath::Power(10,6)*1.609*TMath::Power(10,-19)*%f*%f)",
                    lifetime[k4type], S_eff, gain[k5gain], oneDay);
            func1[k4type][k5gain] = new TF1( func1Name, func1In, 0., 22.);
        }
    }


    // # Hit Rate vs. Lifetime (Time) Difference # ;
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        sprintf( func2Name, "func2_%d", k4type);
        sprintf( func2In, "func1_%d0 - func1_%d1", k4type, k4type);
        func2[k4type] = new TF1( func2Name, func2In, 0., 22.);
    }
    // # Lifetime (Time) for 5*10^5 Gain vs. Lifetime (Time) Difference # ;
    sprintf( func3In, "(1-%f/%f)*x", gain[0],gain[1]);
    func3 = new TF1( "func3", func3In, 0., 11000.);
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        sprintf( func4Name, "func4_%d", k4type);
        func4[k4type] = new TF1( func4Name, func3In, 0., 11000.);
    }

    // # Luminosity 50 ab^(-1) # ;
    func5 = new TF1( "func5", "50.", 0., 5000.);




    // ### 2D Histogram ### ;
    // # Hit Rate vs. Lifetime (Time) / Lifetime (Time) Difference # ;
    TH2F *hist2D[Ntype];
    char histName[128];
    std::string whichType;
    n2type = 0;
    whichType = "Conventional";
    sprintf( histName, "hist2D_%s", whichType.c_str());
    hist2D[n2type] = new TH2F( histName, "; Hit Rate / MHz     ; Lifetime / day ; Lifetime Difference / day ", 22, 0., 22., 100, 0., 1100.);
    for (Int_t k4type = 1 ; k4type < Ntype; k4type++ ){
        if ( k4type == 1 ) whichType = "ALD";
        if ( k4type == 2 ) whichType = "Life-extended_ALD";
        sprintf( histName, "hist2D_%s", whichType.c_str());
        hist2D[k4type] = new TH2F( histName, "; Hit Rate / MHz      ; Lifetime / day ; Lifetime Difference / day ", 22, 0., 22., 100, 0., 11000.);
    }

    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        for (Int_t k3Hz = 0; k3Hz < NHz; k3Hz++ ){
            hitRateIn = hitRate[k3Hz] / TMath::Power(10,6);
            timeIn = time[k4type][0][k3Hz]/oneDay;
            time_diffIn = time_diff[k4type][k3Hz]/oneDay;
            //std::cout << hitRateIn << ", " << timeIn << ", " << time_diffIn << std::endl;
            hist2D[k4type] -> Fill(hitRateIn,timeIn,time_diffIn);
        }
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
    TCanvas *c1 = new TCanvas("c1","Lifetime (Time) 5*10^5 Gain",800,1000);
    TCanvas *c2 = new TCanvas("c2","Lifetime (Time) Difference",800,1000);
    TCanvas *c3 = new TCanvas("c3","Lifetime (Time) vs. Difference",1600,500);
    TCanvas *c4 = new TCanvas("c4","Lifetime (Time) vs. Difference",1600,1000);
    TCanvas *c5[Ntype];
    char canvasName[128];
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        sprintf( canvasName, "c5_%d", k4type);
        c5[k4type] = new TCanvas( canvasName,"Hit Rate vs. Lifetime (Time) / Difference",800,1000);
    }
    TCanvas *c6 = new TCanvas("c6","Luminosity",800,1000);
    TCanvas *c7 = new TCanvas("c7","Lifetime (Luminosity) 5*10^5 Gain",800,1000);


    gStyle ->SetGridStyle(2);
    gStyle ->SetGridColor(kGray+1);

    c1 ->Divide(1,2);
    c2 ->Divide(1,2);
    c3 ->Divide(2,1);
    c4 ->Divide(3,2);
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        c5[k4type] ->Divide(1,2);
    }
    c6 ->Divide(1,2);
    c7 ->Divide(1,2);




    // ### Frame ### ;
    TH2F *frame1_1 = new TH2F("hist2D1_1","; Hit Rate / MHz ; Lifetime / day ", 100., 0., 21.1, 100., 0., 11000.);
    TH2F *frame1_2 = new TH2F("hist2D1_2","; Hit Rate / MHz ; Lifetime / day ", 100., 0., 21.1, 100., 0., 1000.);
    TH2F *frame2_1 = new TH2F("hist2D2_1","; Hit Rate / MHz ; Lifetime Difference / day ", 100., 0., 21.1, 100., 0., 1000.);
    TH2F *frame2_2 = new TH2F("hist2D2_2","; Hit Rate / MHz ; Lifetime Difference / day ", 100., 0., 21.1, 100., 0., 100.);
    TH2F *frame3_1 = new TH2F("hist2D3_1","; Lifetime / day ; Lifetime Difference / day ", 100., 10., 11000., 100., 0., 1000.);
    TH2F *frame4_1 = new TH2F("hist2D4_1","; Lifetime / day ; Lifetime Difference / day ", 100., 10., 1100., 100., 0., 100.);
    TH2F *frame4_2 = new TH2F("hist2D4_2","; Lifetime / day ; Lifetime Difference / day ", 100., 10., 11000., 100., 0., 1000.);
    TH2F *frame4_3 = new TH2F("hist2D4_3","; Lifetime / day ; Lifetime Difference / day ", 100., 10., 11000., 100., 0., 1000.);
    TH2F *frame7_1 = new TH2F("hist2D7_1","; Hit Rate / MHz ; Lifetime (Integral Luminosity) / ab^{-1} ", 100., 0., 21.1, 100., 0., 105.);
    TH2F *frame7_2 = new TH2F("hist2D7_2","; Hit Rate / MHz ; Lifetime (Integral Luminosity) / ab^{-1} ", 100., 0., 21.1, 100., 0., 3.05);
    frame1_1 ->SetStats(0);
    frame1_2 ->SetStats(0);
    frame2_1 ->SetStats(0);
    frame2_2 ->SetStats(0);
    frame3_1 ->SetStats(0);
    frame4_1 ->SetStats(0);
    frame4_2 ->SetStats(0);
    frame4_3 ->SetStats(0);
    frame7_1 ->SetStats(0);
    frame7_2 ->SetStats(0);
    ((TGaxis*)frame1_1 ->GetYaxis()) ->SetMaxDigits(4);
    ((TGaxis*)frame1_2 ->GetYaxis()) ->SetMaxDigits(4);
    ((TGaxis*)frame2_1 ->GetYaxis()) ->SetMaxDigits(4);
    ((TGaxis*)frame2_2 ->GetYaxis()) ->SetMaxDigits(4);
    ((TGaxis*)frame3_1 ->GetYaxis()) ->SetMaxDigits(4);
    ((TGaxis*)frame4_1 ->GetYaxis()) ->SetMaxDigits(4);
    ((TGaxis*)frame4_2 ->GetYaxis()) ->SetMaxDigits(4);
    ((TGaxis*)frame4_3 ->GetYaxis()) ->SetMaxDigits(4);
    ((TGaxis*)frame7_1 ->GetYaxis()) ->SetMaxDigits(4);
    ((TGaxis*)frame7_2 ->GetYaxis()) ->SetMaxDigits(4);

    frame1_1 ->GetXaxis() ->SetTitleSize(0.05);
    frame1_2 ->GetXaxis() ->SetTitleSize(0.05);
    frame2_1 ->GetXaxis() ->SetTitleSize(0.05);
    frame2_2 ->GetXaxis() ->SetTitleSize(0.05);
    frame3_1 ->GetXaxis() ->SetTitleSize(0.05);
    frame3_1 ->GetXaxis() ->SetTitleSize(0.05);
    frame4_1 ->GetXaxis() ->SetTitleSize(0.05);
    frame4_2 ->GetXaxis() ->SetTitleSize(0.05);
    frame4_3 ->GetXaxis() ->SetTitleSize(0.05);
    frame7_1 ->GetXaxis() ->SetTitleSize(0.05);
    frame7_2 ->GetXaxis() ->SetTitleSize(0.05);

    frame1_1 ->GetYaxis() ->SetTitleSize(0.05);
    frame1_2 ->GetYaxis() ->SetTitleSize(0.05);
    frame2_1 ->GetYaxis() ->SetTitleSize(0.05);
    frame2_2 ->GetYaxis() ->SetTitleSize(0.05);
    frame3_1 ->GetXaxis() ->SetTitleSize(0.05);
    frame3_1 ->GetYaxis() ->SetTitleSize(0.05);
    frame4_1 ->GetYaxis() ->SetTitleSize(0.05);
    frame4_2 ->GetYaxis() ->SetTitleSize(0.05);
    frame4_3 ->GetYaxis() ->SetTitleSize(0.05);
    frame7_1 ->GetYaxis() ->SetTitleSize(0.05);
    frame7_2 ->GetYaxis() ->SetTitleSize(0.05);

    frame1_1 ->GetYaxis() ->SetTitleOffset(1.);
    frame1_2 ->GetYaxis() ->SetTitleOffset(1.);
    frame2_1 ->GetYaxis() ->SetTitleOffset(1.);
    frame2_2 ->GetYaxis() ->SetTitleOffset(1.);
    frame3_1 ->GetXaxis() ->SetTitleOffset(1.);
    frame3_1 ->GetYaxis() ->SetTitleOffset(1.);
    frame3_1 ->GetXaxis() ->SetMoreLogLabels();
    frame4_1 ->GetYaxis() ->SetTitleOffset(1.);
    frame4_2 ->GetYaxis() ->SetTitleOffset(1.);
    frame4_3 ->GetYaxis() ->SetTitleOffset(1.);
    frame7_1 ->GetYaxis() ->SetTitleOffset(1.);
    frame7_2 ->GetYaxis() ->SetTitleOffset(1.);




    // ### Style ### ;
    // # Style of Axis and Title, Label # ;
    for(Int_t k4type = 0; k4type < Ntype; k4type++){
        graph1[k4type] ->SetTitle("; Hit Rate / MHz ; Lifetime / day ");
        graph1[k4type] ->GetXaxis() ->SetTitleSize(0.047);
        graph1[k4type] ->GetYaxis() ->SetTitleSize(0.047);
        graph1[k4type] ->GetXaxis() ->SetTitleOffset(1.);
        graph1[k4type] ->GetYaxis() ->SetTitleOffset(0.7);
        graph1[k4type] ->GetXaxis() ->SetRangeUser(0., 21.1);

        graph2[k4type] ->SetTitle("; Hit Rate / MHz ; Lifetime Difference / day ");
        graph2[k4type] ->GetXaxis() ->SetTitleSize(0.047);
        graph2[k4type] ->GetYaxis() ->SetTitleSize(0.047);
        graph2[k4type] ->GetXaxis() ->SetTitleOffset(1.);
        graph2[k4type] ->GetYaxis() ->SetTitleOffset(0.7);
        graph2[k4type] ->GetXaxis() ->SetRangeUser(0., 21.1);

        graph3[k4type] ->SetTitle("; Lifetime / day ; Lifetime Difference / day ");
        graph3[k4type] ->GetXaxis() ->SetTitleSize(0.047);
        graph3[k4type] ->GetYaxis() ->SetTitleSize(0.047);
        graph3[k4type] ->GetXaxis() ->SetTitleOffset(1.);
        graph3[k4type] ->GetYaxis() ->SetTitleOffset(0.7);

        graph4[k4type] ->SetTitle("; Lifetime / day ; Lifetime Difference / day ");
        graph4[k4type] ->GetXaxis() ->SetTitleSize(0.047);
        graph4[k4type] ->GetYaxis() ->SetTitleSize(0.047);
        graph4[k4type] ->GetXaxis() ->SetTitleOffset(1.);
        graph4[k4type] ->GetYaxis() ->SetTitleOffset(0.7);

        hist2D[k4type] ->GetXaxis() ->SetTitleOffset(1.8);
        hist2D[k4type] ->GetYaxis() ->SetTitleOffset(2.);
        hist2D[k4type] ->GetZaxis() ->SetTitleOffset(1.2);

    } 

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
        graph1[k4type] ->GetXaxis() ->SetRangeUser(0., 100.1);
    }

    
    // # Marker and Line, Fill Style # ;
    // --> Canvas 1 ;
    // Hit Rate vs. Lifetime (Time) for 5*10^5 Gain ;
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        graph1[k4type] ->SetMarkerStyle(8);
        graph1[k4type] ->SetMarkerSize(1.);
        graph1[k4type] ->SetMarkerColor(kBlack);
    }
    // --> Canvas 2 ;
    // Hit Rate vs. Lifetime (Time) Difference ;
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        graph2[k4type] ->SetMarkerStyle(8);
        graph2[k4type] ->SetMarkerSize(1.);
        graph2[k4type] ->SetMarkerColor(kBlack);
    }
    // --> Canvas 3 ;
    // Lifetime (Time) for 5*10^5 Gain vs. Lifetime (Time) Difference ;
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        graph3[k4type] ->SetMarkerSize(1.1);
    }
    graph3[2] ->SetMarkerSize(1.2);
    graph3[0] ->SetMarkerStyle(20);
    graph3[1] ->SetMarkerStyle(21);
    graph3[2] ->SetMarkerStyle(22);
    graph3[0] ->SetMarkerColor(kRed);
    graph3[1] ->SetMarkerColor(kOrange+1);
    graph3[2] ->SetMarkerColor(kBlue);
    // --> Canvas 4 ;
    // Lifetime (Time) for 5*10^5 Gain vs. Lifetime (Time) Difference ; 
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        graph4[k4type] ->SetMarkerSize(1.);
        graph4[k4type] ->SetMarkerStyle(8);
        graph4[k4type] ->SetMarkerColor(kBlack);
    }
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
        graph7[k4type] ->SetMarkerStyle(8);
        graph7[k4type] ->SetMarkerColor(kBlack);
    }
    graph7[0] ->SetMarkerStyle(20);
    graph7[1] ->SetMarkerStyle(21);
    graph7[2] ->SetMarkerStyle(22);
    graph7[0] ->SetMarkerColor(kRed);
    graph7[1] ->SetMarkerColor(kOrange+1);
    graph7[2] ->SetMarkerColor(kBlue);


    // # Function Line Color # ;
    // --> Canvas 1 & Canvas 2 ;
    // "Conventional" ;
    func1[0][0] ->SetLineColor(kRed);
    func2[0] ->SetLineColor(kRed);
    // "ALD" ;
    func1[1][0] ->SetLineColor(kOrange+1);
    func2[1] ->SetLineColor(kOrange+1);
    // "Life-extended ALD" ;
    func1[2][0] ->SetLineColor(kBlue);
    func2[2] ->SetLineColor(kBlue);
    // --> Canvas 3 ;
    func3 ->SetLineColor(kBlack);
    func3 ->SetLineStyle(2);
    // --> Canvas 4 ;
    // "Conventional" ;
    func4[0] ->SetLineColor(kRed);
    // "ALD" ;
    func4[1] ->SetLineColor(kOrange+1);
    // "Life-extended ALD" ;
    func4[2] ->SetLineColor(kBlue);
    // --> Canvas 6 & Canvas 7 ;
    func5 ->SetLineColor(kMagenta);


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




    // ### Draw Graphs ### ;
    // --> Canvas 1 ;
    c1 ->cd(1);
    frame1_1 -> Draw();
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        graph1[k4type] ->Draw("P");
        func1[k4type][0] ->Draw("same");
        graph1[k4type] ->Draw("P");
    }
    gPad ->SetGrid(1,1);
    gPad ->Modified();

    text1_1 ->DrawTextNDC(0.6, 0.84, "Conventional");
    text1_2 ->DrawTextNDC(0.6, 0.785, "ALD");
    text1_3 ->DrawTextNDC(0.6, 0.73, "Life-extended ALD");

    c1 ->cd(2);
    frame1_2 -> Draw();
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        graph1[k4type] ->Draw("P");
        func1[k4type][0] ->Draw("same");
        graph1[k4type] ->Draw("P");
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
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        graph1[k4type] ->Draw("P");
        func1[k4type][0] ->Draw("same");
        graph1[k4type] ->Draw("P");
    }
    gPad ->SetGrid(1,1);
    gPad ->Modified();

    text1_1 ->DrawTextNDC(0.6, 0.84, "Conventional");
    text1_2 ->DrawTextNDC(0.6, 0.785, "ALD");
    text1_3 ->DrawTextNDC(0.6, 0.73, "Life-extended ALD");

    c1 ->cd(2);
    frame1_2 -> Draw();
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        graph1[k4type] ->Draw("P");
        func1[k4type][0] ->Draw("same");
        graph1[k4type] ->Draw("P");
    }
    gPad ->SetGrid(1,1);
    gPad ->Modified();

    text1_1 ->DrawTextNDC(0.6, 0.84, "Conventional");
    text1_2 ->DrawTextNDC(0.6, 0.785, "ALD");
    text1_3 ->DrawTextNDC(0.6, 0.73, "Life-extended ALD");


    // --> Canvas 2 ;
    c2 ->cd(1);
    frame2_1 -> Draw();
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        graph2[k4type] ->Draw("P");
        func2[k4type] ->Draw("same");
        graph2[k4type] ->Draw("P");
    }
    gPad ->SetGrid(1,1);
    gPad ->Modified();

    text1_1 ->DrawTextNDC(0.6, 0.84, "Conventional");
    text1_2 ->DrawTextNDC(0.6, 0.785, "ALD");
    text1_3 ->DrawTextNDC(0.6, 0.73, "Life-extended ALD");

    c2 ->cd(2);
    frame2_2 -> Draw();
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        graph2[k4type] ->Draw("P");
        func2[k4type] ->Draw("same");
        graph2[k4type] ->Draw("P");
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
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        graph3[k4type] ->Draw("P");
        graph3[k4type] ->Draw("P");
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
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        graph3[k4type] ->Draw("P");
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

    // --> Canvas 5 ;
    for (Int_t k4type = 0; k4type < Ntype; k4type++ ){
        c5[k4type] ->cd(1);
        hist2D[k4type] ->Draw("colz");
        
        text5_1 ->DrawTextNDC(0.7, 0.01, "Hit Rate / MHz");
        text5_2 ->DrawTextNDC(0.03, 0.65, "Lifetime / day ");
        //text5_3 ->DrawTextNDC(0.6, 0.73, "Lifetime Difference / day ");


        c5[k4type] ->cd(2);
        hist2D[k4type] ->Draw("lego2z");
    }
        /*
        c5[0] ->cd();    
        TPaveStats *stat5 = (TPaveStats*)hist2D_Conventional -> FindObject("stats");

        stat5 ->SetX1NDC(0.64); //xStart
        stat5 ->SetX2NDC(0.84); //xEnd
        stat5 ->SetY1NDC(0.68); //yStart
	stat5 ->SetY2NDC(0.84); //yEnd
	c5[0] ->Modified();
	*/

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


}






