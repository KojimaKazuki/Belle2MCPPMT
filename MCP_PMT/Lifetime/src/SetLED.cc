#include <iostream>
#include <signal.h>
#include <sys/time.h>

#include "utility.h"

#define SLOT_OREG 23
#define CH_LED 0x3
#define CH_LASER 0x18

using namespace std;

int main(int argc, char **argv)
{
  char* mygetchar(int timeout);

  bool prompt=false;
  bool check=false;
  int c;
  while ((c = getopt(argc,argv,"ich")) != -1) {
    switch (c) {
      case 'i':
        prompt = true;
        break;
      case 'c':
        check = true;
        break;
      case 'h':
        cout << "Usage ./SetLED [-i] [-c] [on/off]" << endl;
        cout << "   -i: prompt before turn on/off LED" << endl;
        cout << "   -c: check LED on/off" << endl;
        cout << "   -h: print this message" << endl;
        return 0;
      default:
        break;
    }
  }

  if (argc-optind != 1) {
    fprintf(stdout, "Usage: %s [-i] [-c] [on/off]\n", argv[0]);
    return -1;
  }

  int LEDon;
  string buf;
  if (!strcmp(argv[optind],"on")) {
    LEDon = 1;
  } else if (!strcmp(argv[optind],"off")) {
    LEDon = 0;
  } else {
    cout << "ERROR: argument " << argv[optind] << " is not defined" << endl;
    return -1;
  }

  if (prompt) {
    cout << "Turn LED " << argv[optind] << "? (y/n): " << flush;
    if (strncmp(mygetchar(10),"y",1)) return 1;
  }

  Camac *camac = new Camac(0);

  int LEDrate;

  if (!check) {
    camac->Clear(SLOT_OREG);
    if (LEDon) camac->Write(SLOT_OREG,CH_LED);
    else       camac->Write(SLOT_OREG,CH_LASER);
    sleep(1);
    cout << "LED turned " << ((LEDon) ? "on" : "off") << endl;
  }

  int ScalerSlot, ScalerCh;
  ifstream fin("../config.txt");
  while (getline(fin,buf)){
    if(buf.find("#") == string::npos){
      if(buf == "--- Scaler settings ---"){
	fin >> buf >> buf >> ScalerSlot;
	fin >> buf >> buf >> buf >> buf;
	fin >> buf >> buf >> buf >> ScalerCh;
        break;
      }
    }
  }

  camac->Clear(ScalerSlot);
  sleep(1);
  LEDrate = camac->Read(ScalerSlot,ScalerCh);
  cout << "LED rate: " << LEDrate << " Hz" << endl;

  delete camac;

  if (LEDon && !LEDrate) {
    cout << "ERROR: LED is off" << endl;
    return -1;
  } else if (!LEDon && LEDrate>1) {
    cout << "ERROR: LED is on" << endl;
    return -1;
  }

  return 0;
}

void mywait(int no) { printf("timeout.\n"); }
	 
char* mygetchar(int timeout)
{
  static char buf[256];
  struct sigaction sa;
  struct itimerval itimer;

  memset(&sa,0,sizeof(struct sigaction));
  sa.sa_handler = mywait;
  //sa.sa_flags   = SA_RESTART; // if restart input
  if ( sigaction(SIGALRM,&sa,NULL) != 0 ){
      perror("sigaction");
      return 0;
  }
  itimer.it_value.tv_sec  = itimer.it_interval.tv_sec  = timeout; // sec
  itimer.it_value.tv_usec = itimer.it_interval.tv_usec = 0; // micro sec
  if ( setitimer(ITIMER_REAL,&itimer,NULL) < 0 ) {
      perror("setitimer");
      return 0;
  }
  fgets(buf,256,stdin);
  return buf;
}
