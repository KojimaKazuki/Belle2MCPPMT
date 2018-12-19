#include <iostream>
#include <sstream>
#include <stdio.h>
#include "TROOT.h"
#include "TH1.h"
#include "TMath.h"

void macroGainQE181127(){

    // # Title&Label Font #;
    gStyle ->SetOptFit();
    gStyle ->SetLabelSize(0.05,"x");
    gStyle ->SetLabelSize(0.05,"y");
    gStyle ->SetTitleXSize(0.04);
    gStyle ->SetTitleYSize(0.04);


    // ### BG Parameter ###; // Data on Jun 11th/12th 2018;
    const Int_t Nbeam = 2; // LER = 0, HER = 1;
    const Double_t T_LER = 0.0683965;
    const Double_t B_LER = 18093.5;
    const Double_t T_HER = 0.109737;
    const Double_t B_HER = 83267.7;

    Double_t T[Nbeam];
    Double_t B[Nbeam];

    T[0] = T_LER;
    T[1] = T_HER;
    B[0] = B_LER;
    B[1] = B_HER;

    const Double_t P = 13 * TMath::Power(10., -8.);
    //std::cout << P << std::endl;



    // ### Beam Parameter ###;
    const Int_t Ninj = 4;
    Int_t day[Ninj];
    Int_t dayNoBeam[Ninj];
    Int_t sumDayIn = 0;
    Int_t sumDay[2*Ninj];
    const Int_t Ncond = 2; // "conservertive" vs "aggresive";

    // # Beam injection term / days #
    day[0] = 0; // Jun. 2018
    day[1] = 90; // Mar. 2019 - Jun. 2019;
    day[2] = 60; // Oct. 2019 - Dec. 2019;
    day[3] = 160; // Jan. 2020 - Jun. 2020;

    Int_t NdivIn = 0;
    for (Int_t k3inj = 0; k3inj < Ninj; k3inj++){
        if (NdivIn < day[k3inj]){
            NdivIn = day[k3inj];
        }
    }
    const Int_t Ndiv = NdivIn;
    //std::cout << Ndiv << std::endl;

    // # No Beam term / days #
    dayNoBeam[0] = 90;
    dayNoBeam[1] = 110;
    dayNoBeam[2] = 20;
    dayNoBeam[3] = 30;

    for (Int_t k3inj = 0; k3inj < Ninj; k3inj++){
        sumDayIn += day[k3inj];
        sumDay[2*k3inj] = sumDayIn;
        sumDayIn += dayNoBeam[k3inj];
        sumDay[2*k3inj+1] = sumDayIn;
        //std::cout << sumDay[2*k3inj] << " " << sumDay[2*k3inj+1] << std::endl;
    }

    // --> Date Convertion;
    const Int_t Nday = sumDay[2*Ninj-1];
    Int_t t[Nday];

    const Int_t timeOffset = -788918400;
    const Int_t oneDay = 24*60*60;
    TDatime *dateBegin = new TDatime(2019,1,1,0,0,0);
    Int_t dateBeginUnix = dateBegin ->Convert();
    
    for (Int_t k5day = 0; k5day < Nday; k5day++){
        t[k5day] = dateBeginUnix + k5day*oneDay;// + timeOffset;
        //if (k5day%10 == 0) std::cout << t[k5day] << std::endl;
    }

    // # Beam Current / A #;
    Double_t IMAX[Nbeam][Ncond][Ninj];

    Int_t n1beam;
    Int_t n2cond;

    // --> LER beam current (n1beam = 0);
    n1beam = 0;

    // "Conservative" (n2cond = 0);
    n2cond = 0;
    IMAX[n1beam][n2cond][0] = 0.341;
    IMAX[n1beam][n2cond][1] = 1.2;
    IMAX[n1beam][n2cond][2] = 1.4;
    IMAX[n1beam][n2cond][3] = 1.8;

    // "Aggressive" (n2cond = 1);
    n2cond = 1; 
    IMAX[n1beam][n2cond][0] = 0.341;
    IMAX[n1beam][n2cond][1] = 1.4;
    IMAX[n1beam][n2cond][2] = 1.7;
    IMAX[n1beam][n2cond][3] = 2.;


    // --> HER beam current (n1beam = 1);
    n1beam = 1;

    // "Conservative" (n2cond = 0);
    n2cond = 0; 
    IMAX[n1beam][n2cond][0] = 0.287;
    IMAX[n1beam][n2cond][1] = 1.;
    IMAX[n1beam][n2cond][2] = 1.;
    IMAX[n1beam][n2cond][3] = 1.3;

    // "Aggressive" (n2cond = 1);
    n2cond = 1; 
    IMAX[n1beam][n2cond][0] = 0.287;
    IMAX[n1beam][n2cond][1] = 1;
    IMAX[n1beam][n2cond][2] = 1.2;
    IMAX[n1beam][n2cond][3] = 1.4;


    // # Beam Width (sigma_y * = average sigma_y) / nm #;
    Double_t sigma_yMAX[Nbeam][Ncond][Ninj];

    Int_t n1beam;
    Int_t n2cond;

    // --> LER beam current (n1beam = 0);
    n1beam = 0;

    // "Conservative" (n2cond = 0);
    n2cond = 0; 
    sigma_yMAX[n1beam][n2cond][0] = 38000;
    sigma_yMAX[n1beam][n2cond][1] = 693;
    sigma_yMAX[n1beam][n2cond][2] = 529;
    sigma_yMAX[n1beam][n2cond][3] = 442;

    // "Aggressive" (n2cond = 1);
    n2cond = 1; 
    sigma_yMAX[n1beam][n2cond][0] = 38000;
    sigma_yMAX[n1beam][n2cond][1] = 670;
    sigma_yMAX[n1beam][n2cond][2] = 510;
    sigma_yMAX[n1beam][n2cond][3] = 346;


    // --> HER beam current (n1beam = 1);
    n1beam = 1;

    // "Conservative" (n2cond = 0);
    n2cond = 0; 
    sigma_yMAX[n1beam][n2cond][0] = 36000;
    sigma_yMAX[n1beam][n2cond][1] = 1051;
    sigma_yMAX[n1beam][n2cond][2] = 525;
    sigma_yMAX[n1beam][n2cond][3] = 439;

    // "Aggressive" (n2cond = 1);
    n2cond = 1; 
    sigma_yMAX[n1beam][n2cond][0] = 36000;
    sigma_yMAX[n1beam][n2cond][1] = 830;
    sigma_yMAX[n1beam][n2cond][2] = 508;
    sigma_yMAX[n1beam][n2cond][3] = 348;


    // # The number of bunches #;
    Int_t nb[Ninj];
    nb[0] = 789;
    nb[1] = 1576; 
    nb[2] = 1576; 
    nb[3] = 1576; 




    // ### Caluculate BG values ###;
    Double_t I[Nbeam][Ncond][Ninj][Ndiv];
    Double_t sigma_y[Nbeam][Ncond][Ninj][Ndiv];

    // # Variable of Integration  #
    Double_t dI[Nbeam][Ncond][Ninj];
    Double_t dsigma_y[Nbeam][Ncond][Ninj];

    for (Int_t k1beam = 0; k1beam < Nbeam; k1beam++){
        for (Int_t k2cond = 0; k2cond < Ncond; k2cond++){
            dI[k1beam][k2cond][0] = 0;
            for (Int_t k3inj = 1; k3inj < Ninj; k3inj ++){
            dI[k1beam][k2cond][k3inj] = (IMAX[k1beam][k2cond][k3inj]-IMAX[k1beam][k2cond][k3inj-1])/day[k3inj];
            //std::cout << "dI = " << dI[k1beam][k2cond][k3inj] << " , day = " << day[k3inj] << std::endl;
            }
        }
    }

    for (Int_t k1beam = 0; k1beam < Nbeam; k1beam++){
        for (Int_t k2cond = 0; k2cond < Ncond; k2cond++){
            dsigma_y[k1beam][k2cond][0] = 0;
            for (Int_t k3inj = 1; k3inj < Ninj; k3inj ++){
            dsigma_y[k1beam][k2cond][k3inj] = (sigma_yMAX[k1beam][k2cond][k3inj]-sigma_yMAX[k1beam][k2cond][k3inj-1])/day[k3inj];
            //std::cout << "dsigma_y = " << dsigma_y[k1beam][k2cond][k3inj] << std::endl;
            }
        }
    }



    // # I (Current)-value #;
    for (Int_t k1beam = 0; k1beam < Nbeam; k1beam++){
        for (Int_t k2cond = 0; k2cond < Ncond; k2cond++){
            for (Int_t k3inj = 0; k3inj < Ninj; k3inj++){
                if (k3inj == 0){
                    for (Int_t k4div = 0; k4div < Ndiv; k4div++){
                        I[k1beam][k2cond][k3inj][k4div] = 0.;
                    }
                }else{
                    for (Int_t k4div = 0; k4div < day[k3inj]; k4div++){
                        I[k1beam][k2cond][k3inj][k4div] = IMAX[k1beam][k2cond][k3inj-1] + dI[k1beam][k2cond][k3inj] * double(k4div);  //x[i=Ndiv-1]= xMax - dx;
                    }
                    for (Int_t k4div = day[k3inj]; k4div < Ndiv; k4div++){
                        I[k1beam][k2cond][k3inj][k4div] = 0.;
                    }
                }
            }
        }
    }

    // # sigma_y-value #;
    for (Int_t k1beam = 0; k1beam < Nbeam; k1beam++){
        for (Int_t k2cond = 0; k2cond < Ncond; k2cond++){
            for (Int_t k3inj = 0; k3inj < Ninj; k3inj++){
                if (k3inj == 0){
                    for (Int_t k4div = 0; k4div < Ndiv; k4div++){
                        sigma_y[k1beam][k2cond][k3inj][k4div] = 0.;
                    }
                }else{
                    for (Int_t k4div = 0; k4div < day[k3inj]; k4div++){
                        sigma_y[k1beam][k2cond][k3inj][k4div] = sigma_yMAX[k1beam][k2cond][k3inj-1] + dsigma_y[k1beam][k2cond][k3inj] * double(k4div);  //x[i=Ndiv-1]= xMax - dx;
                    }
                    for (Int_t k4div = day[k3inj]; k4div < Ndiv; k4div++){
                        sigma_y[k1beam][k2cond][k3inj][k4div] = 0.;
                    }
                }
            }
        }
    }



    // # Calculate Formula #;
    Double_t BG_Touschek[Nbeam][Ncond][Ninj][Ndiv];
    Double_t BG_BeamGas[Nbeam][Ncond][Ninj][Ndiv];
    Double_t BG[Nbeam][Ncond][Ninj][Ndiv];
    // --> total BG ( LER + HER ); 
    Double_t totalBG[Ncond][Ninj][Ndiv];
    Double_t totalBG_Touschek[Ncond][Ninj][Ndiv];
    Double_t totalBG_BeamGas[Ncond][Ninj][Ndiv];
    // --> Integral BG ;
    Double_t sumBGIn = 0;
    Double_t sumBG[Ncond][Ninj][Ndiv];
    Double_t sumBGNB;
    // --> Accumulated Output Charge / (C/PMT) ;
    const Int_t Ngain = 2;
    Double_t outputCharge[Ncond][Ninj][Ndiv][Ngain];
    // --> Accumulated Output Charge / (C/cm^2) ;
    Double_t outputCharge0In = 0;
    Double_t outputCharge0[Ncond][Ninj][Ndiv][Ngain];
    // --> QE (lifetime = 1.1 C/ cm^2);
    Double_t QE1[Ncond][Ninj][Ndiv][Ngain];
    // --> QE (lifetime = 13.6 C/ cm^2);
    Double_t QE2[Ncond][Ninj][Ndiv][Ngain];

    // --> sigma_y condition (LER: 0, HER: 1);
    Double_t sigma_y0[Nbeam];
    sigma_y0[0] = 38000;
    sigma_y0[1] = 36000;


    for (Int_t k1beam = 0; k1beam < Nbeam; k1beam++){
        for (Int_t k2cond = 0; k2cond < Ncond; k2cond++){
            for (Int_t k3inj = 0; k3inj < Ninj; k3inj++){
                for (Int_t k4div = 0; k4div < day[k3inj]; k4div++){        
                    BG_Touschek[k1beam][k2cond][k3inj][k4div] = T[k1beam]*(TMath::Power( (I[k1beam][k2cond][k3inj][k4div]*1000), 2.))/(sigma_y0[k1beam]*0.001*nb[k3inj]); 
                    BG_BeamGas[k1beam][k2cond][k3inj][k4div] = B[k1beam]*I[k1beam][k2cond][k3inj][k4div]*1000*P;
                    BG[k1beam][k2cond][k3inj][k4div] = BG_Touschek[k1beam][k2cond][k3inj][k4div] + BG_BeamGas[k1beam][k2cond][k3inj][k4div];
                }    
                for (Int_t k4div = day[k3inj]; k4div < Ndiv; k4div++){    
                    BG_Touschek[k1beam][k2cond][k3inj][k4div] = 0.; 
                    BG_BeamGas[k1beam][k2cond][k3inj][k4div] = 0.;
                    BG[k1beam][k2cond][k3inj][k4div] = 0.;
                }
            }
        }
    }

    for (Int_t k2cond = 0; k2cond < Ncond; k2cond++){
        sumBGIn = 0;
        for (Int_t k3inj = 0; k3inj < Ninj; k3inj++){
            for (Int_t k4div = 0; k4div < Ndiv; k4div++){        
                totalBG[k2cond][k3inj][k4div] = BG[0][k2cond][k3inj][k4div] + BG[1][k2cond][k3inj][k4div]; 
                totalBG_Touschek[k2cond][k3inj][k4div] = BG_Touschek[0][k2cond][k3inj][k4div] + BG_Touschek[1][k2cond][k3inj][k4div];
                totalBG_BeamGas[k2cond][k3inj][k4div] = BG_BeamGas[0][k2cond][k3inj][k4div] + BG_BeamGas[1][k2cond][k3inj][k4div];
                if ( k4div < day[k3inj]){
                sumBGIn += totalBG[k2cond][k3inj][k4div]*double(oneDay);
                }else{
                sumBGIn += 0;
                }
                sumBG[k2cond][k3inj][k4div] = sumBGIn;
                outputCharge[k2cond][k3inj][k4div][0] = sumBGIn * TMath::Power(10,6) * 1.609 * TMath::Power(10,-19) * 5.0 * TMath::Power(10,5);
                outputCharge[k2cond][k3inj][k4div][1] = sumBGIn * TMath::Power(10,6) * 1.609 * TMath::Power(10,-19) * 5.5 * TMath::Power(10,5);

                outputCharge0In = sumBGIn * TMath::Power(10,6) * 1.609 * TMath::Power(10,-19) * 5.0 * TMath::Power(10,5) / (2.3*2.3);
                outputCharge0[k2cond][k3inj][k4div][0] = outputCharge0In;
                QE1[k2cond][k3inj][k4div][0] = 1 - 0.2 * TMath::Power((outputCharge0In/1.1),2);
                QE2[k2cond][k3inj][k4div][0] = 1 - 0.2 * TMath::Power((outputCharge0In/13.6),2);
                outputCharge0In = sumBGIn * TMath::Power(10,6) * 1.609 * TMath::Power(10,-19) * 5.5 * TMath::Power(10,5) / (2.3*2.3);
                outputCharge0[k2cond][k3inj][k4div][1] = outputCharge0In;
                QE1[k2cond][k3inj][k4div][1] = 1 - 0.2 * TMath::Power((outputCharge0In/1.1),2);
                QE2[k2cond][k3inj][k4div][1] = 1 - 0.2 * TMath::Power((outputCharge0In/13.6),2);
                /* 
                if ( k4div%10 == 0){
                std::cout << "total BG = " << totalBG_Touschek[k2cond][k3inj][k4div] << " , day = " << k4div << std::endl;
                }
                */
                
            }
        }
    }

    
    // ### Graph ### ;
    // # Background Rate Graph # ;
    Int_t dayStart[Ninj];
    Int_t dayEnd[Ninj];
    Int_t dayNBStart[Ninj];
    Int_t dayNBEnd[Ninj];
    Int_t n1beam;
    Int_t n4div;
    const Int_t Ngraph = 9;
    Int_t n6graph;

    TGraph *graph1[Ngraph]; // --> Background ("Conservative");
    TGraph *graph2[Ngraph]; // --> Background ("Aggressive");
    TGraph *graph3 = new TGraph(); // --> Ancillary Graph ;
    TGraph *graph4[Ncond]; // --> Integral Background ;
    TGraph *graph5[Ncond][Ngain]; // --> MCP-PMT Accumulated Output Charge (C/PMT) ;
    TGraph *graph6[Ncond][Ngain]; // --> MCP-PMT Accumulated Output Charge (C/cm^2) ;
    TGraph *graph7[Ncond][Ngain]; // --> MCP-PMT QE 1 (Conventional) ;
    TGraph *graph8[Ncond][Ngain]; // --> MCP-PMT QE 2 (Life-extended-ALD);


    for (Int_t k6graph = 0; k6graph < Ngraph; k6graph++){
        graph1[k6graph] = new TGraph();
        graph2[k6graph] = new TGraph();
    }
    for (Int_t k2cond = 0; k2cond < Ncond; k2cond++){
        graph4[k2cond] = new TGraph();
        for (Int_t k7gain = 0; k7gain < Ngain; k7gain++){
            graph5[k2cond][k7gain] = new TGraph();
            graph6[k2cond][k7gain] = new TGraph();
            graph7[k2cond][k7gain] = new TGraph();
            graph8[k2cond][k7gain] = new TGraph();
        }
    }

    // # "Conservative(graph1)" & "Aggresive(graph2)" condition # ;
    // --> calculation of days for Setpoint ;
    for (Int_t k3inj = 0; k3inj < Ninj; k3inj++){
        if (k3inj == 0){
            dayStart[k3inj] = 0;
        }else{
            dayStart[k3inj] = sumDay[2*k3inj-1];
        }
        dayEnd[k3inj] = sumDay[2*k3inj];
        dayNBStart[k3inj] = sumDay[2*k3inj];
        dayNBEnd[k3inj] = sumDay[2*k3inj+1];
    }

    // --> Total Background Rate (n6graph = 0) ;
    n6graph = 0;
    for (Int_t k3inj = 0; k3inj < Ninj; k3inj++){
        for (Int_t k5day = dayStart[k3inj]; k5day < dayEnd[k3inj]; k5day++){
            n4div = k5day - dayStart[k3inj];
            graph1[n6graph] ->SetPoint( k5day, t[k5day], totalBG[0][k3inj][n4div]);
            graph2[n6graph] ->SetPoint( k5day, t[k5day], totalBG[1][k3inj][n4div]);
        }
        for (Int_t k5day = dayNBStart[k3inj]; k5day < dayNBEnd[k3inj]; k5day++){
            graph1[n6graph] ->SetPoint( k5day, t[k5day], -1.);
            graph2[n6graph] ->SetPoint( k5day, t[k5day], -1.);
        }
    }
    // --> Total Touscheck Background Rate (n6graph = 1);
    n6graph = 1;
    for (Int_t k3inj = 0; k3inj < Ninj; k3inj++){            
        for (Int_t k5day = dayStart[k3inj]; k5day < dayEnd[k3inj]; k5day++){
            n4div = k5day - dayStart[k3inj];
            graph1[n6graph] ->SetPoint( k5day, t[k5day], totalBG_Touschek[0][k3inj][n4div]);
            graph2[n6graph] ->SetPoint( k5day, t[k5day], totalBG_Touschek[1][k3inj][n4div]);
        }
        for (Int_t k5day = dayNBStart[k3inj]; k5day < dayNBEnd[k3inj]; k5day++){
            graph1[n6graph] ->SetPoint( k5day, t[k5day], -1.);
            graph2[n6graph] ->SetPoint( k5day, t[k5day], -1.);
        }
    }
    // --> Total Beam Gas Background Rate (n6graph = 2);
    n6graph = 2;
    for (Int_t k3inj = 0; k3inj < Ninj; k3inj++){            
        for (Int_t k5day = dayStart[k3inj]; k5day < dayEnd[k3inj]; k5day++){
            n4div = k5day - dayStart[k3inj];
            graph1[n6graph] ->SetPoint( k5day, t[k5day], totalBG_BeamGas[0][k3inj][n4div]);
            graph2[n6graph] ->SetPoint( k5day, t[k5day], totalBG_BeamGas[1][k3inj][n4div]);
        }
        for (Int_t k5day = dayNBStart[k3inj]; k5day < dayNBEnd[k3inj]; k5day++){
            graph1[n6graph] ->SetPoint( k5day, t[k5day], -1.);
            graph2[n6graph] ->SetPoint( k5day, t[k5day], -1.);
        }
    }
    // --> Bockground Rate for Each Beam (LER: n6graph = 1; HER: n6graph = 2);
    for (Int_t k6graph = 3; k6graph < 5; k6graph++){
        n1beam = k6graph -3;
        for (Int_t k3inj = 0; k3inj < Ninj; k3inj++){            
            for (Int_t k5day = dayStart[k3inj]; k5day < dayEnd[k3inj]; k5day++){
                n4div = k5day - dayStart[k3inj];
                graph1[k6graph] ->SetPoint( k5day, t[k5day], BG[n1beam][0][k3inj][n4div]);
                graph2[k6graph] ->SetPoint( k5day, t[k5day], BG[n1beam][1][k3inj][n4div]);
            }
            for (Int_t k5day = dayNBStart[k3inj]; k5day < dayNBEnd[k3inj]; k5day++){
                graph1[k6graph] ->SetPoint( k5day, t[k5day], -1.);
                graph2[k6graph] ->SetPoint( k5day, t[k5day], -1.);
            }
        }
    }
    // --> Touschek Bockground Rate (LER: n6graph = 3; HER: n6graph = 4);
    for (Int_t k6graph = 5; k6graph < 7; k6graph++){
        n1beam = k6graph -5;
        for (Int_t k3inj = 0; k3inj < Ninj; k3inj++){            
            for (Int_t k5day = dayStart[k3inj]; k5day < dayEnd[k3inj]; k5day++){
                n4div = k5day - dayStart[k3inj];
                graph1[k6graph] ->SetPoint( k5day, t[k5day], BG_Touschek[n1beam][0][k3inj][n4div]);
                graph2[k6graph] ->SetPoint( k5day, t[k5day], BG_Touschek[n1beam][1][k3inj][n4div]);
            }
            for (Int_t k5day = dayNBStart[k3inj]; k5day < dayNBEnd[k3inj]; k5day++){
                graph1[k6graph] ->SetPoint( k5day, t[k5day], -1.);
                graph2[k6graph] ->SetPoint( k5day, t[k5day], -1.);
            }
        }
    }
    // --> Beam Gas Background Rate (LER: n6graph = 5; HER: n6graph = 6);
    for (Int_t k6graph = 7; k6graph < Ngraph; k6graph++){
        n1beam = k6graph -7;
        for (Int_t k3inj = 0; k3inj < Ninj; k3inj++){            
            for (Int_t k5day = dayStart[k3inj]; k5day < dayEnd[k3inj]; k5day++){
                n4div = k5day - dayStart[k3inj];
                graph1[k6graph] ->SetPoint( k5day, t[k5day], BG_BeamGas[n1beam][0][k3inj][n4div]);
                graph2[k6graph] ->SetPoint( k5day, t[k5day], BG_BeamGas[n1beam][1][k3inj][n4div]);
            }
            for (Int_t k5day = dayNBStart[k3inj]; k5day < dayNBEnd[k3inj]; k5day++){
                graph1[k6graph] ->SetPoint( k5day, t[k5day], -1.);
                graph2[k6graph] ->SetPoint( k5day, t[k5day], -1.);
            }
        }
    }
    
    // # Ancillary Graph # ;
    for (Int_t k3inj = 0; k3inj < Ninj; k3inj++){
        for (Int_t k5day = dayStart[k3inj]; k5day < dayEnd[k3inj]; k5day++){
            n4div = k5day - dayStart[k3inj];
            graph3 ->SetPoint( k5day, t[k5day], totalBG[1][k3inj][n4div]);
        }
        for (Int_t k5day = dayNBStart[k3inj]; k5day < dayNBEnd[k3inj]; k5day++){
            graph3 ->SetPoint( k5day, t[k5day], 0.);
        }
    }

    // --> Integral Background ;
    for (Int_t k2cond = 0; k2cond < Ncond; k2cond++){
        sumBGNB = 0;
        for (Int_t k3inj = 0; k3inj < Ninj; k3inj++){
            for (Int_t k5day = dayStart[k3inj]; k5day < dayEnd[k3inj]; k5day++){
                n4div = k5day - dayStart[k3inj];
                graph4[k2cond] ->SetPoint( k5day, t[k5day], sumBG[k2cond][k3inj][n4div]);
                sumBGNB = sumBG[k2cond][k3inj][n4div];
            }
            for (Int_t k5day = dayNBStart[k3inj]; k5day < dayNBEnd[k3inj]; k5day++){
                graph4[k2cond] ->SetPoint( k5day, t[k5day], sumBGNB);
            }
        }
    }

    // --> MCP-PMT Integral Output Charge (C/PMT) ;
    Double_t outputChargeNB;
    for (Int_t k2cond = 0; k2cond < Ncond; k2cond++){
        outputChargeNB = 0;
        for (Int_t k3inj = 0; k3inj < Ninj; k3inj++){
            for (Int_t k7gain = 0; k7gain < Ngain; k7gain++){
                for (Int_t k5day = dayStart[k3inj]; k5day < dayEnd[k3inj]; k5day++){
                    n4div = k5day - dayStart[k3inj];
                    graph5[k2cond][k7gain] ->SetPoint( k5day, t[k5day], outputCharge[k2cond][k3inj][n4div][k7gain]);
                    outputChargeNB = outputCharge[k2cond][k3inj][n4div][k7gain];
                }
                for (Int_t k5day = dayNBStart[k3inj]; k5day < dayNBEnd[k3inj]; k5day++){
                    graph5[k2cond][k7gain] ->SetPoint( k5day, t[k5day], outputChargeNB);
                }
            }
        }
    }

    // --> MCP-PMT Integral Output Charge (C/cm^2) ;
    Double_t outputCharge0NB;
    for (Int_t k2cond = 0; k2cond < Ncond; k2cond++){
        outputCharge0NB = 0;
        for (Int_t k3inj = 0; k3inj < Ninj; k3inj++){
            for (Int_t k7gain = 0; k7gain < Ngain; k7gain++){
                for (Int_t k5day = dayStart[k3inj]; k5day < dayEnd[k3inj]; k5day++){
                    n4div = k5day - dayStart[k3inj];
                    graph6[k2cond][k7gain] ->SetPoint( k5day, t[k5day], outputCharge0[k2cond][k3inj][n4div][k7gain]);
                    outputCharge0NB = outputCharge0[k2cond][k3inj][n4div][k7gain];
                }
                for (Int_t k5day = dayNBStart[k3inj]; k5day < dayNBEnd[k3inj]; k5day++){
                    graph6[k2cond][k7gain] ->SetPoint( k5day, t[k5day], outputCharge0NB);
                }
            }
        }
    }

    // --> MCP-PMT QE ;
    // "Lifetime = 1.1 C/cm^2" ;
    Double_t QE1NB;
    for (Int_t k2cond = 0; k2cond < Ncond; k2cond++){
        QE1NB = 1.;
        for (Int_t k3inj = 0; k3inj < Ninj; k3inj++){
            for (Int_t k7gain = 0; k7gain < Ngain; k7gain++){
                for (Int_t k5day = dayStart[k3inj]; k5day < dayEnd[k3inj]; k5day++){
                    n4div = k5day - dayStart[k3inj];
                    graph7[k2cond][k7gain] ->SetPoint( k5day, t[k5day], QE1[k2cond][k3inj][n4div][k7gain]);
                    QE1NB = QE1[k2cond][k3inj][n4div][k7gain];
                }
                for (Int_t k5day = dayNBStart[k3inj]; k5day < dayNBEnd[k3inj]; k5day++){
                    graph7[k2cond][k7gain] ->SetPoint( k5day, t[k5day], QE1NB);
                }
            }
        }
    }
    // "Lifetime = 13.6 C/cm^2" ;
    Double_t QE2NB;
    for (Int_t k2cond = 0; k2cond < Ncond; k2cond++){
        QE2NB = 1.;
        for (Int_t k3inj = 0; k3inj < Ninj; k3inj++){
            for (Int_t k7gain = 0; k7gain < Ngain; k7gain++){
                for (Int_t k5day = dayStart[k3inj]; k5day < dayEnd[k3inj]; k5day++){
                    n4div = k5day - dayStart[k3inj];
                    graph8[k2cond][k7gain] ->SetPoint( k5day, t[k5day], QE2[k2cond][k3inj][n4div][k7gain]);
                    QE2NB = QE2[k2cond][k3inj][n4div][k7gain];
                }
                for (Int_t k5day = dayNBStart[k3inj]; k5day < dayNBEnd[k3inj]; k5day++){
                    graph8[k2cond][k7gain] ->SetPoint( k5day, t[k5day], QE2NB);
                }
            }
        }
    }



    // # Canvas # ;    
    TCanvas *c1 = new TCanvas("c1","Background Rate",800,500);
    TCanvas *c2 = new TCanvas("c2","MCP-PMT Integral Background",800,500);
    TCanvas *c3 = new TCanvas("c3","MCP-PMT Output Charge (C/PMT)",800,500);
    TCanvas *c4 = new TCanvas("c4","MCP-PMT Output Charge (C/cm^2)",800,500);
    TCanvas *c5 = new TCanvas("c5","MCP-PMT QE (1.1 C/cm^2)",800,500);
    TCanvas *c6 = new TCanvas("c6","MCP-PMT QE (13.6 C/cm^2)",800,500);

    gStyle ->SetGridStyle(2);
    gStyle ->SetGridColor(kGray+1);

    // # Style of Axis and Title, Label # ;
    for(Int_t k6graph = 0; k6graph < Ngraph; k6graph++){
        graph1[k6graph] ->GetXaxis() ->SetTimeDisplay(1);
        graph1[k6graph] ->GetXaxis() ->SetNdivisions(9,5,0,kTRUE);
        graph1[k6graph] ->GetXaxis() ->SetTimeFormat("%y-%m\/%d");
        graph1[k6graph] ->GetXaxis() ->SetTimeOffset(0,"GMT");
        //graph1[k6graph] ->GetXaxis() ->SetLabelOffset(0.03);
        graph1[k6graph] ->SetTitle(";Date&Time ;BG Rate / ( MHz / PMT )");
        graph1[k6graph] ->GetXaxis() ->SetTitleSize(0.047);
        graph1[k6graph] ->GetYaxis() ->SetTitleSize(0.047);
        graph1[k6graph] ->GetXaxis() ->SetTitleOffset(1.);
        graph1[k6graph] ->GetYaxis() ->SetTitleOffset(0.7);
        graph1[k6graph] ->GetXaxis() ->SetRangeUser(t[0],t[Nday-1]);
        graph1[k6graph] ->GetYaxis() ->SetRangeUser(0.,40.);

        graph2[k6graph] ->GetXaxis() ->SetTimeDisplay(1);
        graph2[k6graph] ->GetXaxis() ->SetNdivisions(9,5,0,kTRUE);
        graph2[k6graph] ->GetXaxis() ->SetTimeFormat("%y-%m\/%d");
        graph2[k6graph] ->GetXaxis() ->SetTimeOffset(0,"GMT");
        //graph2[k6graph] ->GetXaxis() ->SetLabelOffset(0.03);
        graph2[k6graph] ->SetTitle(";Date&Time ;BG Rate [ MHz / PMT ]");
        graph2[k6graph] ->GetXaxis() ->SetTitleSize(0.047);
        graph2[k6graph] ->GetYaxis() ->SetTitleSize(0.047);
        graph2[k6graph] ->GetXaxis() ->SetTitleOffset(1.);
        graph2[k6graph] ->GetYaxis() ->SetTitleOffset(0.7);
        graph2[k6graph] ->GetXaxis() ->SetRangeUser(t[0],t[Nday-1]);
        graph2[k6graph] ->GetYaxis() ->SetRangeUser(0.,30.2);
    }
    for(Int_t k2cond = 0; k2cond < Ncond; k2cond++){
        graph4[k2cond] ->GetXaxis() ->SetTimeDisplay(1);
        graph4[k2cond] ->GetXaxis() ->SetNdivisions(9,5,0,kTRUE);
        graph4[k2cond] ->GetXaxis() ->SetTimeFormat("%y-%m\/%d");
        graph4[k2cond] ->GetXaxis() ->SetTimeOffset(0,"GMT");
        //graph4[k2cond] ->GetXaxis() ->SetLabelOffset(0.03);
        graph4[k2cond] ->SetTitle(";Date&Time ;Integral BG hit / PMT");
        graph4[k2cond] ->GetXaxis() ->SetTitleSize(0.047);
        graph4[k2cond] ->GetYaxis() ->SetTitleSize(0.047);
        graph4[k2cond] ->GetXaxis() ->SetTitleOffset(1.);
        graph4[k2cond] ->GetYaxis() ->SetTitleOffset(1.);
        graph4[k2cond] ->GetXaxis() ->SetRangeUser(t[0],t[Nday-1]);
        graph4[k2cond] ->GetYaxis() ->SetRangeUser(0.,704000000.);
    }
    for(Int_t k2cond = 0; k2cond < Ncond; k2cond++){
        for (Int_t k7gain = 0; k7gain < Ngain; k7gain++){
            graph5[k2cond][k7gain] ->GetXaxis() ->SetTimeDisplay(1);
            graph5[k2cond][k7gain] ->GetXaxis() ->SetNdivisions(9,5,0,kTRUE);
            graph5[k2cond][k7gain] ->GetXaxis() ->SetTimeFormat("%y-%m\/%d");
            graph5[k2cond][k7gain] ->GetXaxis() ->SetTimeOffset(0,"GMT");
            //graph5[k2cond][k7gain] ->GetXaxis() ->SetLabelOffset(0.03);
            graph5[k2cond][k7gain] ->SetTitle(";Date&Time ;Accumulated Output Charge / ( C / PMT )");
            graph5[k2cond][k7gain] ->GetXaxis() ->SetTitleSize(0.047);
            graph5[k2cond][k7gain] ->GetYaxis() ->SetTitleSize(0.047);
            graph5[k2cond][k7gain] ->GetXaxis() ->SetTitleOffset(1.);
            graph5[k2cond][k7gain] ->GetYaxis() ->SetTitleOffset(0.8);
            graph5[k2cond][k7gain] ->GetXaxis() ->SetRangeUser(t[0],t[Nday-1]);
            //graph5[k2cond][k7gain] ->GetYaxis() ->SetRangeUser(0.,704000000.);

            graph6[k2cond][k7gain] ->GetXaxis() ->SetTimeDisplay(1);
            graph6[k2cond][k7gain] ->GetXaxis() ->SetNdivisions(9,5,0,kTRUE);
            graph6[k2cond][k7gain] ->GetXaxis() ->SetTimeFormat("%y-%m\/%d");
            graph6[k2cond][k7gain] ->GetXaxis() ->SetTimeOffset(0,"GMT");
            //graph6[k2cond][k7gain] ->GetXaxis() ->SetLabelOffset(0.03);
            graph6[k2cond][k7gain] ->SetTitle(";Date&Time ; Accumulated Output Charge / ( C / cm^2 )");
            graph6[k2cond][k7gain] ->GetXaxis() ->SetTitleSize(0.047);
            graph6[k2cond][k7gain] ->GetYaxis() ->SetTitleSize(0.047);
            graph6[k2cond][k7gain] ->GetXaxis() ->SetTitleOffset(1.);
            graph6[k2cond][k7gain] ->GetYaxis() ->SetTitleOffset(0.8);
            graph6[k2cond][k7gain] ->GetXaxis() ->SetRangeUser(t[0],t[Nday-1]);
            //graph6[k2cond][k7gain] ->GetYaxis() ->SetRangeUser(0.,1.25);

            graph7[k2cond][k7gain] ->GetXaxis() ->SetTimeDisplay(1);
            graph7[k2cond][k7gain] ->GetXaxis() ->SetNdivisions(9,5,0,kTRUE);
            graph7[k2cond][k7gain] ->GetXaxis() ->SetTimeFormat("%y-%m\/%d");
            graph7[k2cond][k7gain] ->GetXaxis() ->SetTimeOffset(0,"GMT");
            //graph7[k2cond][k7gain] ->GetXaxis() ->SetLabelOffset(0.03);
            graph7[k2cond][k7gain] ->SetTitle(";Date&Time ; Relative QE");
            graph7[k2cond][k7gain] ->GetXaxis() ->SetTitleSize(0.047);
            graph7[k2cond][k7gain] ->GetYaxis() ->SetTitleSize(0.047);
            graph7[k2cond][k7gain] ->GetXaxis() ->SetTitleOffset(1.);
            graph7[k2cond][k7gain] ->GetYaxis() ->SetTitleOffset(0.8);
            graph7[k2cond][k7gain] ->GetXaxis() ->SetRangeUser(t[0],t[Nday-1]);
            graph7[k2cond][k7gain] ->GetYaxis() ->SetRangeUser(0.,1.25);

            graph8[k2cond][k7gain] ->GetXaxis() ->SetTimeDisplay(1);
            graph8[k2cond][k7gain] ->GetXaxis() ->SetNdivisions(9,5,0,kTRUE);
            graph8[k2cond][k7gain] ->GetXaxis() ->SetTimeFormat("%y-%m\/%d");
            graph8[k2cond][k7gain] ->GetXaxis() ->SetTimeOffset(0,"GMT");
            //graph8[k2cond][k7gain] ->GetXaxis() ->SetLabelOffset(0.03);
            graph8[k2cond][k7gain] ->SetTitle(";Date&Time ; Relative QE");
            graph8[k2cond][k7gain] ->GetXaxis() ->SetTitleSize(0.047);
            graph8[k2cond][k7gain] ->GetYaxis() ->SetTitleSize(0.047);
            graph8[k2cond][k7gain] ->GetXaxis() ->SetTitleOffset(1.);
            graph8[k2cond][k7gain] ->GetYaxis() ->SetTitleOffset(0.8);
            graph8[k2cond][k7gain] ->GetXaxis() ->SetRangeUser(t[0],t[Nday-1]);
            graph8[k2cond][k7gain] ->GetYaxis() ->SetRangeUser(0.,1.25);
        }
    }


    // # Marker and Line, Fill Style # ;
    // --> Canvas1;
    // Total Background Rate Graph (n6graph = 0) ;
    n6graph = 0;
    graph1[n6graph] ->SetLineWidth(2);
    graph1[n6graph] ->SetMarkerStyle(8);
    graph1[n6graph] ->SetMarkerSize(1.);
    graph1[n6graph] ->SetMarkerColor(kBlue);
    graph1[n6graph] ->SetLineColor(kBlue);
    graph1[n6graph] ->SetFillColor(kCyan-7);
    graph1[n6graph] ->SetFillStyle(3013);
    
    graph2[n6graph] ->SetLineWidth(2);
    graph2[n6graph] ->SetMarkerStyle(8);
    graph2[n6graph] ->SetMarkerSize(1.);
    graph2[n6graph] ->SetMarkerColor(kRed);
    graph2[n6graph] ->SetLineColor(kRed);
    graph2[n6graph] ->SetFillColor(kOrange);
    graph2[n6graph] ->SetFillStyle(3013);
    // Touschek Background Rate Graph (n6graph = 1) ;
    n6graph = 1;
    graph1[n6graph] ->SetMarkerStyle(6);
    graph1[n6graph] ->SetMarkerSize(1);
    graph1[n6graph] ->SetMarkerColor(kAzure+3);
    
    graph2[n6graph] ->SetMarkerStyle(6);
    graph2[n6graph] ->SetMarkerSize(1);
    graph2[n6graph] ->SetMarkerColor(kOrange+1);

    // Beam Gas Background Rate Graph (n6graph = 2) ;
    n6graph = 2;
    graph1[n6graph] ->SetMarkerStyle(6);
    graph1[n6graph] ->SetMarkerSize(1);
    graph1[n6graph] ->SetMarkerColor(kGreen+2);
    
    graph2[n6graph] ->SetMarkerStyle(6);
    graph2[n6graph] ->SetMarkerSize(1);
    graph2[n6graph] ->SetMarkerColor(kMagenta);

    // --> Canvas2;
    // Integral Background ;
    // "Conservative" ;
    n2cond = 0;
    graph4[n2cond] ->SetLineWidth(2);
    graph4[n2cond] ->SetMarkerStyle(7);
    graph4[n2cond] ->SetMarkerSize(1.);
    graph4[n2cond] ->SetMarkerColor(kBlue);
    graph4[n2cond] ->SetLineColor(kBlue);
    graph4[n2cond] ->SetFillColor(kCyan-7);
    graph4[n2cond] ->SetFillStyle(3015);
    // "Aggressive" ;
    n2cond = 1;
    graph4[n2cond] ->SetLineWidth(2);
    graph4[n2cond] ->SetMarkerStyle(7);
    graph4[n2cond] ->SetMarkerSize(1.);
    graph4[n2cond] ->SetMarkerColor(kRed);
    graph4[n2cond] ->SetLineColor(kRed);
    graph4[n2cond] ->SetFillColor(kOrange);
    graph4[n2cond] ->SetFillStyle(3015);

    // --> Canvas3;
    // MCP-PMT Accumulated Output Charge (C/PMT) ;
    Int_t n7gain;
    for ( k2cond = 0; k2cond < Ncond; k2cond++){
        for ( k7gain = 0; k7gain < Ngain; k7gain++){
            graph5[k2cond][k7gain] ->SetLineWidth(2);
            graph5[k2cond][k7gain] ->SetMarkerStyle(7);
            graph5[k2cond][k7gain] ->SetMarkerSize(1.);
        }
    }
    // "Conservative" ;
    graph5[0][0] ->SetMarkerColor(kBlue);
    graph5[0][0] ->SetLineColor(kBlue);
    graph5[0][1] ->SetMarkerColor(kOrange-3);
    graph5[0][1] ->SetLineColor(kOrange-3);
    // "Aggressive" ;
    graph5[1][0] ->SetMarkerColor(kRed);
    graph5[1][0] ->SetLineColor(kRed);
    graph5[1][1] ->SetMarkerColor(kSpring+3);
    graph5[1][1] ->SetLineColor(kSpring+3);

    // --> Canvas4;
    // MCP-PMT Accumulated Output Charge (C/cm^2) ;
    Int_t n7gain;
    for ( k2cond = 0; k2cond < Ncond; k2cond++){
        for ( k7gain = 0; k7gain < Ngain; k7gain++){
            graph6[k2cond][k7gain] ->SetLineWidth(2);
            graph6[k2cond][k7gain] ->SetMarkerStyle(7);
            graph6[k2cond][k7gain] ->SetMarkerSize(1.);
        }
    }
    // "Conservative" ;
    graph6[0][0] ->SetMarkerColor(kBlue);
    graph6[0][0] ->SetLineColor(kBlue);
    graph6[0][1] ->SetMarkerColor(kOrange-3);
    graph6[0][1] ->SetLineColor(kOrange-3);
    // "Aggressive" ;
    graph6[1][0] ->SetMarkerColor(kRed);
    graph6[1][0] ->SetLineColor(kRed);
    graph6[1][1] ->SetMarkerColor(kSpring+3);
    graph6[1][1] ->SetLineColor(kSpring+3);

    // --> Canvas5;
    // MCP-PMT QE ;
    Int_t n7gain;
    for ( k2cond = 0; k2cond < Ncond; k2cond++){
        for ( k7gain = 0; k7gain < Ngain; k7gain++){
            graph7[k2cond][k7gain] ->SetLineWidth(2);
            graph7[k2cond][k7gain] ->SetMarkerStyle(7);
            graph7[k2cond][k7gain] ->SetMarkerSize(1.);
        }
    }
    // "Conservative" ;
    graph7[0][0] ->SetMarkerColor(kBlue);
    graph7[0][0] ->SetLineColor(kBlue);
    graph7[0][1] ->SetMarkerColor(kOrange-3);
    graph7[0][1] ->SetLineColor(kOrange-3);
    // "Aggressive" ;
    graph7[1][0] ->SetMarkerColor(kRed);
    graph7[1][0] ->SetLineColor(kRed);
    graph7[1][1] ->SetMarkerColor(kSpring+3);
    graph7[1][1] ->SetLineColor(kSpring+3);

    // --> Canvas6;
    // MCP-PMT QE ;
    Int_t n7gain;
    for ( k2cond = 0; k2cond < Ncond; k2cond++){
        for ( k7gain = 0; k7gain < Ngain; k7gain++){
            graph8[k2cond][k7gain] ->SetLineWidth(2);
            graph8[k2cond][k7gain] ->SetMarkerStyle(7);
            graph8[k2cond][k7gain] ->SetMarkerSize(1.);
        }
    }
    // "Conservative" ;
    graph8[0][0] ->SetMarkerColor(kBlue);
    graph8[0][0] ->SetLineColor(kBlue);
    graph8[0][1] ->SetMarkerColor(kOrange-3);
    graph8[0][1] ->SetLineColor(kOrange-3);
    // "Aggressive" ;
    graph8[1][0] ->SetMarkerColor(kRed);
    graph8[1][0] ->SetLineColor(kRed);
    graph8[1][1] ->SetMarkerColor(kSpring+3);
    graph8[1][1] ->SetLineColor(kSpring+3);



    // # Text # ;
    // --> Canvas1 ;
    TText *text1_1 = new TText(0.5, 0.5, "");
    TText *text1_2 = new TText(0.5, 0.5, "");
    TText *text1_3 = new TText(0.5, 0.5, "");
    TText *text1_4 = new TText(0.5, 0.5, "");
    TText *text1_5 = new TText(0.5, 0.5, "");
    TText *text1_6 = new TText(0.5, 0.5, "");
    text1_1 ->SetTextColor(kBlue);
    text1_2 ->SetTextColor(kRed);
    text1_3 ->SetTextColor(kGreen+2);
    text1_4 ->SetTextColor(kMagenta);
    text1_5 ->SetTextColor(kAzure+3);
    text1_6 ->SetTextColor(kOrange+2);
    text1_3 ->SetTextFont(42);
    text1_4 ->SetTextFont(42);
    text1_5 ->SetTextFont(42);
    text1_6 ->SetTextFont(42);
    text1_1 ->SetTextSize(0.04);
    text1_2 ->SetTextSize(0.04);
    text1_3 ->SetTextSize(0.04);
    text1_4 ->SetTextSize(0.04);
    text1_5 ->SetTextSize(0.04);
    text1_6 ->SetTextSize(0.04);
    // --> Canvas2 ;
    TText *text2_1 = new TText(0.5, 0.5, "");
    TText *text2_2 = new TText(0.5, 0.5, "");
    text2_1 ->SetTextColor(kBlue);
    text2_2 ->SetTextColor(kRed);
    // --> Canvas3 ;
    TText *text3_1 = new TText(0.5, 0.5, "");
    TText *text3_2 = new TText(0.5, 0.5, "");
    TText *text3_3 = new TText(0.5, 0.5, "");
    TText *text3_4 = new TText(0.5, 0.5, "");
    text3_1 ->SetTextColor(kBlue);
    text3_2 ->SetTextColor(kRed);
    text3_3 ->SetTextColor(kOrange-3);
    text3_4 ->SetTextColor(kSpring+3);
    // --> Canvas4 ;
    TText *text4_1 = new TText(0.5, 0.5, "");
    TText *text4_2 = new TText(0.5, 0.5, "");
    TText *text4_3 = new TText(0.5, 0.5, "");
    TText *text4_4 = new TText(0.5, 0.5, "");
    text4_1 ->SetTextColor(kBlue);
    text4_2 ->SetTextColor(kRed);
    text4_3 ->SetTextColor(kOrange-3);
    text4_4 ->SetTextColor(kSpring+3);
    // --> Canvas5 ;
    TText *text5_1 = new TText(0.5, 0.5, "");
    TText *text5_2 = new TText(0.5, 0.5, "");
    TText *text5_3 = new TText(0.5, 0.5, "");
    TText *text5_4 = new TText(0.5, 0.5, "");
    text5_1 ->SetTextColor(kBlue);
    text5_2 ->SetTextColor(kRed);
    text5_3 ->SetTextColor(kOrange-3);
    text5_4 ->SetTextColor(kSpring+3);
    // --> Canvas6 ;
    TText *text6_1 = new TText(0.5, 0.5, "");
    TText *text6_2 = new TText(0.5, 0.5, "");
    TText *text6_3 = new TText(0.5, 0.5, "");
    TText *text6_4 = new TText(0.5, 0.5, "");
    text6_1 ->SetTextColor(kBlue);
    text6_2 ->SetTextColor(kRed);
    text6_3 ->SetTextColor(kOrange-3);
    text6_4 ->SetTextColor(kSpring+3);
    


    // # Draw Graphs #
    // --> Canvas1 ;
    c1 ->cd();
    graph2[0] ->Draw("AFP");
    graph1[0] ->Draw("FP");
    //graph3 ->Draw("PL");
    for (Int_t k6graph = 1; k6graph < Ngraph-6; k6graph++){
        graph1[k6graph] ->Draw("P");
        graph2[k6graph] ->Draw("P");
    }
    gPad ->SetGrid(1,1); 
    gPad ->Modified();

    text1_1 ->DrawTextNDC(0.15, 0.85, "Total BG (Conservative)");
    text1_2 ->DrawTextNDC(0.15, 0.805, "Total BG (Aggressive)");
    text1_3 ->DrawTextNDC(0.15, 0.755, "Beam Gas BG (Conservative)");
    text1_4 ->DrawTextNDC(0.15, 0.715, "Beam Gas BG (Aggressive)");
    text1_5 ->DrawTextNDC(0.15, 0.665, "Touschek BG (Conservative)");
    text1_6 ->DrawTextNDC(0.15, 0.62, "Touschek BG (Aggressive)");

    // --> Canvas 2;
    c2 ->cd();
    graph4[1] ->Draw("AP");
    graph4[0] ->Draw("P");
    gPad ->SetGrid(1,1); 
    gPad ->Modified();

    text2_1 ->DrawTextNDC(0.15, 0.84, "Integral Background (Conservative)");
    text2_2 ->DrawTextNDC(0.15, 0.785, "Integral Background (Aggressive)");

    // --> Canvas 3;
    c3 ->cd();
    graph5[1][1] ->Draw("AP");
    for (Int_t k2cond = 0; k2cond < Ncond; k2cond++){
        for (Int_t k7gain = 0; k7gain < Ngain; k7gain++){
            graph5[k2cond][k7gain] ->Draw("P");
        }
    }
    gPad ->SetGrid(1,1); 
    gPad ->Modified();

    text3_1 ->DrawTextNDC(0.15, 0.84, "Nominal Gain (Conservative)");
    text3_2 ->DrawTextNDC(0.15, 0.785, "Nominal Gain (Aggressive)");
    text3_3 ->DrawTextNDC(0.15, 0.73, "10% Upper Gain (Conservative)");
    text3_4 ->DrawTextNDC(0.15, 0.675, "10% Upper Gain (Aggressive)");

    // --> Canvas 4;
    c4 ->cd();
    graph6[1][1] ->Draw("AP");
    for (Int_t k2cond = 0; k2cond < Ncond; k2cond++){
        for (Int_t k7gain = 0; k7gain < Ngain; k7gain++){
            graph6[k2cond][k7gain] ->Draw("P");
        }
    }
    gPad ->SetGrid(1,1); 
    gPad ->Modified();

    text4_1 ->DrawTextNDC(0.15, 0.84, "Nominal Gain (Conservative)");
    text4_2 ->DrawTextNDC(0.15, 0.785, "Nominal Gain (Aggressive)");
    text4_3 ->DrawTextNDC(0.15, 0.73, "10% Upper Gain (Conservative)");
    text4_4 ->DrawTextNDC(0.15, 0.675, "10% Upper Gain (Aggressive)");

    // --> Canvas 5;
    c5 ->cd();
    graph7[1][1] ->Draw("AP");
    for (Int_t k2cond = 0; k2cond < Ncond; k2cond++){
        for (Int_t k7gain = 0; k7gain < Ngain; k7gain++){
            graph7[k2cond][k7gain] ->Draw("P");
        }
    }
    gPad ->SetGrid(1,1); 
    gPad ->Modified();

    text5_1 ->DrawTextNDC(0.45, 0.84, "Nominal Gain (Conservative)");
    text5_2 ->DrawTextNDC(0.45, 0.785, "Nominal Gain (Aggressive)");
    text5_3 ->DrawTextNDC(0.45, 0.73, "10% Upper Gain (Conservative)");
    text5_4 ->DrawTextNDC(0.45, 0.675, "10% Upper Gain (Aggressive)");

    // --> Canvas 6;
    c6 ->cd();
    graph8[1][1] ->Draw("AP");
    for (Int_t k2cond = 0; k2cond < Ncond; k2cond++){
        for (Int_t k7gain = 0; k7gain < Ngain; k7gain++){
            graph8[k2cond][k7gain] ->Draw("P");
        }
    }
    gPad ->SetGrid(1,1); 
    gPad ->Modified();

    text6_1 ->DrawTextNDC(0.15, 0.44, "Nominal Gain (Conservative)");
    text6_2 ->DrawTextNDC(0.15, 0.385, "Nominal Gain (Aggressive)");
    text6_3 ->DrawTextNDC(0.15, 0.33, "10% Upper Gain (Conservative)");
    text6_4 ->DrawTextNDC(0.15, 0.275, "10% Upper Gain (Aggressive)");


}
