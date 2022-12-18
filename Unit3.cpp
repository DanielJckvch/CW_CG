//---------------------------------------------------------------------------


#pragma hdrstop

#include "Unit3.h"
face::face(void)
{
 f=new double*[6];
 for(int i=0;i<6;i++)
 {
  f[i]=new double[4];
 }
 for(int i=0;i<4;i++)
 {
  gab[i]=0;
 }
 trian=false;
}

face::~face()
{
 for(int i=0;i<4;i++)
 {
  delete[] f[i];
 }
 delete[] f;
}

//---------------------------------------------------------------------------

#pragma package(smart_init)
 