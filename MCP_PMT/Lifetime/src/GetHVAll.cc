//developed by Maeda Yosuke (10th Jul, 2015)
//copied to dacapo and some modification applied at 4th Aug, 2015
//maeday@hepl.phys.nagoya-u.ac.jp

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <stdio.h>
#include <time.h>
#include "utility.h"

const int g_nCrate = 3;
const int g_CrateID[g_nCrate] = { 1, 2, 10 };
const int g_ChID[g_nCrate][10] = { { 0, 1, 2, 3, -1 },
				   { 0, 1, 2, 3, -1 },
				   { 0, -1 } };

int main( int argc, char *argv[] ){

  if( argc<2 ){
    std::cout << "usage:" << std::endl;
    std::cout << argv[0] << " (outputfilename)" << std::endl;
    return 1;
  }

  std::ofstream fileOut( argv[1], std::ios::app );
  if( fileOut.fail() ){
    std::cout << "ERROR : fail to open output file\"" << argv[1] << "\"" << std::endl;
    return 1;
  }
  
  N470* hv = new N470();
  time_t startTime = time( 0 );
  char dateMonitor[64];
  strftime(dateMonitor, sizeof(dateMonitor), "%Y/%m/%d %a %H:%M:%S", localtime(&startTime));

  int VmonArray[g_nCrate][10] = {};
  int ImonArray[g_nCrate][10] = {};
  for(int iCrate = 0 ; iCrate < g_nCrate ; iCrate++ ){
    int iCh = 0;
    //for( int iCh=0 ; iCh<g_nCh[iCrate] ; iCh++ ){
    while( g_ChID[iCrate][iCh]>=0 ){
      int ChID = g_ChID[iCrate][iCh];
      int status = hv->GetStatus(g_CrateID[iCrate],ChID,false);
      int Vmon = ( status==0 ? hv->GetVmon() : -1 );
      int Imon = ( status==0 ? hv->GetImon() : -1 );
      VmonArray[iCrate][iCh] = Vmon;
      ImonArray[iCrate][iCh] = Imon;
      std::cout << "Crate" << iCrate << "Ch No." << iCh << "Vmon: " << Vmon << "\t" << "Imon" << Imon <<  "\n";
      std::cout << "Crate" << iCrate << "Ch No." << iCh << "Vmon2: " << VmonArray[iCrate][iCh] << "\t" << "Imon2" << ImonArray[iCrate][iCh] <<  "\n";
      fileOut << Vmon << "\t" << Imon << "\t";

      iCh++;
    }
  }  

  const int NCh = 4; 
  // ### Output Monitor Values of Voltage ### ;
  std::cout << "Vmon:" << "\t";
  for (int kCrate = 0; kCrate < g_nCrate - 1; kCrate++ ){
    for (int kCh = 0; kCh < NCh; kCh++ ){
      std::cout << VmonArray[kCrate][kCh] << "\t";
    }
  }
  std::cout << std::endl;
  // ### Output Monitor Values of Current ### ;
  std::cout << "Imon:" << "\t";
  for (int kCrate = 0; kCrate < g_nCrate -1; kCrate++ ){
    for (int kCh = 0; kCh < NCh; kCh++ ){
      std::cout << ImonArray[kCrate][kCh] << "\t";
    }
  }
  std::cout << std::endl;
  // ### Output the Volatge & Current of the Reference PMT ###
  std::cout << "Vmon(ref): " << VmonArray[g_nCrate-1][0] << std::endl;
  std::cout << "Imon(ref): " << ImonArray[g_nCrate-1][0] << std::endl;
  // ### Output Start Time of Monitoring ### ;
  std::cout << "The date to start monitoring: " << dateMonitor << std::endl;
  std::cout << "The date to start monitoring: " << time << std::endl;
  fileOut << startTime << std::endl;

  delete hv;

  return 1;
}
