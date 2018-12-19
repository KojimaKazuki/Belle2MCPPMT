//developed by Maeda Yosuke (10th Jul, 2015)
//copied to dacapo and some modification applied at 4th Aug, 2015
//maeday@hepl.phys.nagoya-u.ac.jp

#include <iostream>
#include <fstream>
#include <ctime>

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

  std::ofstream output( argv[1], std::ios::app );
  if( output.fail() ){
    std::cout << "ERROR : fail to open output file\"" << argv[1] << "\"" << std::endl;
    return 1;
  }
  
  N470* hv = new N470();
  time_t StartTime = time( 0 );
  for( int iCrate=0 ; iCrate<g_nCrate ; iCrate++ ){
    int iCh=0;
    //for( int iCh=0 ; iCh<g_nCh[iCrate] ; iCh++ ){
    while( g_ChID[iCrate][iCh]>=0 ){
      int ChID = g_ChID[iCrate][iCh];
      int status = hv->GetStatus(g_CrateID[iCrate],ChID,false);
      int Vmon = ( status==0 ? hv->GetVmon() : -1 );
      int Imon = ( status==0 ? hv->GetImon() : -1 );
      std::cout << Vmon << "\t" << Imon << "\t";
      output << Vmon << "\t" << Imon << "\t";

      iCh++;
    }
  }
  std::cout << StartTime << std::endl;
  output << StartTime << std::endl;

  delete hv;

  return 1;
}
