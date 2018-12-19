#include "Camac.h"
#include "../camdrv/camlib.c"
#include <stdio.h>

Camac::Camac(int crate)
{
  err=0;

  //CAM_Close();
  err += CAMOPN();
  err += CSETCR(crate);
  err += CSETI();
  err += CGENZ();
  err += CGENC();

  if (err) {
    cout << "CAMAC open error" << endl;
  } else {
    cout << "CAMAC opened" << endl;
  }
  
}


Camac::~Camac()
{
  CAM_Close();
}


void Camac::Init(int slot)
{
  CAMAC(NAF(slot,0,CAM_ENLAM),&dummy,&q,&x);
  CAMAC(NAF(slot,0,CAM_CLR),&dummy,&q,&x);
}


void Camac::LAM(int slot)
{
  int i = 0;
  q=0;
  x = 0;
  while (q==0){
    CAMAC(NAF(slot,0,CAM_LAM),&dummy,&q,&x);
    i++;
    if(i % 1000 == 0 && q == 0){
      i = 0;
      cerr << "No resonse from slot " << slot << "; (q, x) = (" << q << ", " << x << ")" << endl;
    }
    //usleep(1);
  }
}


void Camac::Clear(int slot)
{
  CAMAC(NAF(slot,0,CAM_CLR),&dummy,&q,&x);
}


int Camac::Read(int slot, int ch)
{
  CAMAC(NAF(slot,ch,CAM_READ),&val,&q,&x);

  if (q==0) {
    cout << "CAMAC read error" << endl;
    err=1;
  }

  return val;
}


void Camac::Write(int slot, int data)
{
  CAMAC(NAF(slot,0,CAM_WRITE),&data,&q,&x);

  if (x!=1) {
    cout << "CAMAC write error" << endl;
    err=1;
  }
}


int Camac::GetErrFlag()
{
  return err;
}


void Camac::ClearErrFlag()
{
  err=0;
}
