#ifndef CAMAC_H_INCLUDED
#define CAMAC_H_INCLUDED

#include <iostream>

#define CAM_READ 0   /* read channel A */
#define CAM_LAM 8    /* look at me */
#define CAM_CLR 9    /* clear */
#define CAM_WRITE 16 /* write */
#define CAM_ENLAM 26 /* enable LAM */

using namespace std;

class Camac
{
  public:

  Camac(int create=0);
  ~Camac();

  void Init(int slot);
  void LAM(int slot);
  void Clear(int slot);
  int Read(int slot, int ch);
  void Write(int slot, int data);
  int GetErrFlag();
  void ClearErrFlag();

  private:

  int q, x, val, dummy;
  int err;
};

#endif //CAMAC_H_INCLUDED
