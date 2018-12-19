#include <unistd.h>

#include "utility.h"

#define CRATE 10
#define CH 0
#define HV 2550

using namespace std;

int main(int argc,char *argv[]){

  bool check=false;
  bool force=false;
  int c;
  while ((c = getopt(argc,argv,"cfh")) != -1) {
    switch (c) {
      case 'c':
        check = true;
        break;
      case 'f':
        force = true;
        break;
      case 'h':
        cout << "Usage ./HVset [-c] [-f] [on/off]" << endl;
        cout << "   -c: check HV status" << endl;
        cout << "   -f: never prompt" << endl;
        cout << "   -h: print this message" << endl;
        return 0;
      default:
        break;
    }
  }

  if(argc-optind != 1){
    cerr << "Usage ./HVset [-c] [on/off]" << endl;
    return -1;
  }

  bool HVon;
  if (!strcmp(argv[optind],"on")) {
    HVon = true;
  } else if (!strcmp(argv[optind],"off")) {
    HVon = false;
  } else {
    cout << "ERROR: argument " << argv[optind] << " is not defined" << endl;
    return -1;
  }

  N470 *hv = new N470();
  if (hv->GetStatus(CRATE,CH,true)) {
    delete hv;
    return 0;
  }

  string buf;
  if (check) {
    delete hv;
    cout << "HV is " << argv[optind] << "? (y/n): " << flush;
    cin >> buf;
    if (buf=="y") {
      if (HVon && !hv->GetHVon()) {
        cout << "You are wrong. HV is not on" << endl;
        return -1;
      }
      if (!HVon && hv->GetHVon()) {
        cout << "You are wrong. HV is not off" << endl;
        return -1;
      }
      return 0;
    } else {
      if (HVon && hv->GetHVon()) {
        cout << "You are wrong. HV is on" << endl;
        return -1;
      }
      if (!HVon && !hv->GetHVon()) {
        cout << "You are wrong. HV is off" << endl;
        return -1;
      }
      return 1;
    }
  }

  if (HVon) {
    if (hv->GetHVon()) {
      cout << "HV is already on" << endl;
      delete hv;
      return 0;
    }
    if (!force) {
      cout << "Turn on HV? (y/n): " << flush;
      cin >> buf;
    } else {
      buf = "y";
    }
    if (buf=="y") {
      hv->SetV(0,CRATE,CH,HV);
      hv->HVOn(CRATE,CH);
    } else {
      delete hv;
      return 1;
    }

  } else {
    if (!hv->GetHVon()) {
      cout << "HV is already off" << endl;
      delete hv;
      return 0;
    }
    if (!force) {
      cout << "Turn off HV? (y/n): " << flush;
      cin >> buf;
    } else {
      buf = "y";
    }
    if (buf=="y") {
      hv->HVOff(CRATE,CH);
    } else {
      delete hv;
      return 1;
    }
  }

  int Vmon;
  int Vset = (HVon) ? HV : 0;
  while (1) {
    sleep(3);
    if (hv->GetStatus(CRATE,CH,false)) {
      delete hv;
      return -1;
    }
    Vmon = hv->GetVmon();
    cout << "Vmon = " << Vmon << endl;
    if (abs(Vmon-Vset) < 10) break;
  }

  delete hv;

  return 0;
}
