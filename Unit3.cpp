//---------------------------------------------------------------------------


#pragma hdrstop

#include "Unit3.h"
#include <math.h>
extern void bresline(TColor*, int, int, int, int, int);

faceCont::faceCont(void)
{
ob=new face[5];
for(int i=0;i<5;i++)
{
 /*for(int j=0;j<3;j++)
 {ob[i].f[j]=0;} */
 ob[i].A=ob[i].B=ob[i].C=ob[i].D=0.0;
 ob[i].toPrint=false;
 ob[i].trian=false;
}
}

void faceCont::init(double** pr)
{  //汤蜒嚷 闲闻手热
for(int k=0;k<2;k++)
{
for(int i=0;i<3;i++)
{
for(int j=0;j<4;j++)
{
  ob[k].f[i][j]=pr[i+k*3][j];
}
}
}

for(int k=2;k<5;k++)
{
 for(int i=0;i<4;i++)
 {
 for(int j=0;j<4;j++)
 {if(k==4)
  {int skip=(i==3||i==0)*((i==0)?-1:1);
  ob[k].f[i][j]=pr[k-3+i+skip][j];}
 else
 {bool skip=(i>=2)?1:0;
 ob[k].f[i][j]=pr[k-2+i+skip][j];}
 }
 }
 ob[k].trian=true;
 }
}

void faceCont::countCoeff(double** p)
{
long int v1[3];
long int v2[3];
long int bariocenter[3];
bariocenter[0]=0;
bariocenter[1]=0;
bariocenter[2]=0;
 for(int j=0;j<6;j++)
 {
 bariocenter[0]=bariocenter[0]+p[j][0];
 }
 bariocenter[0]=bariocenter[0]/6;
 for(int j=0;j<6;j++)
 {
 bariocenter[1]=bariocenter[1]+p[j][1];
 }
 bariocenter[1]=bariocenter[1]/6;
 for(int j=0;j<6;j++)
 {
 bariocenter[2]=bariocenter[2]+p[j][2];
 }
 bariocenter[2]=bariocenter[2]/6;
for(int i=0;i<5;i++)
{

for(int j=0;j<3;j++)
{
v1[j]=ob[i].f[1][j]-ob[i].f[0][j];
v2[j]=ob[i].f[2][j]-ob[i].f[0][j];
}
ob[i].A=v1[1]*v2[2]-v1[2]*v2[1];
ob[i].B=v2[0]*v1[2]-v1[0]*v2[2];
ob[i].C=v1[0]*v2[1]-v1[1]*v2[0];
ob[i].D=(-1*ob[i].A*ob[i].f[0][0]-ob[i].B*ob[i].f[0][1]-ob[i].C*ob[i].f[0][2]);
if(ob[i].A*bariocenter[0]+ob[i].B*bariocenter[1]+ob[i].C*bariocenter[2]+ob[i].D>0)
{
ob[i].A*=-1;
ob[i].B*=-1;
ob[i].C*=-1;
ob[i].D*=-1;
}
}
}

void faceCont::isPrint(double x, double y, double z, double d)
{
for(int i=0;i<5;i++)
{
double a=ob[i].C;
if(a>0)
{ob[i].toPrint=true;}
else
{ob[i].toPrint=false;}
}
}

void faceCont::print(TImage* Image1, MyPoint* pList, int d)
{
 int oi=0;
 for(int i=0;i<5;i++)
 {
 if(ob[i].toPrint)
 {

 if(ob[i].trian)
 {
 Image1->Canvas->MoveTo(ob[i].f[0][0]+d,ob[i].f[0][1]+d);
 Image1->Canvas->LineTo(ob[i].f[1][0]+d,ob[i].f[1][1]+d);
 Image1->Canvas->MoveTo(ob[i].f[2][0]+d,ob[i].f[2][1]+d);
 Image1->Canvas->LineTo(ob[i].f[3][0]+d,ob[i].f[3][1]+d);
 Image1->Canvas->MoveTo(ob[i].f[0][0]+d,ob[i].f[0][1]+d);
 Image1->Canvas->LineTo(ob[i].f[2][0]+d,ob[i].f[2][1]+d);
 Image1->Canvas->MoveTo(ob[i].f[1][0]+d,ob[i].f[1][1]+d);
 Image1->Canvas->LineTo(ob[i].f[3][0]+d,ob[i].f[3][1]+d);

 }
 else
 {
 Image1->Canvas->MoveTo(ob[i].f[0][0]+d,ob[i].f[0][1]+d);
 Image1->Canvas->LineTo(ob[i].f[1][0]+d,ob[i].f[1][1]+d);
 Image1->Canvas->LineTo(ob[i].f[2][0]+d,ob[i].f[2][1]+d);
 Image1->Canvas->LineTo(ob[i].f[0][0]+d,ob[i].f[0][1]+d);
 }
 ob[i].trian=false;
 ob[i].toPrint=false;
 ob[i].A=0.0;
 ob[i].B=0.0;
 ob[i].C=0.0;
 ob[i].D=0.0;
 //抢市姥世
 }
 }

/*
bool out_f=true;
TColor* line;
for(int j=0; j<441; j++)
{
int f_pix=-1;
line=(TColor*)Image1->Picture->Bitmap->ScanLine[j];
for(int i=0; i<537; i++)
{
   if((line[i]==clBlack)&&(f_pix==-1))
   {
    f_pix=i;
   }
   else if((f_pix!=-1)&&(line[i]==clBlack)&&(line[i-1]!=clBlack))
   {
        Image1->Canvas->Pen->Color=clRed;
        Image1->Canvas->MoveTo(i-1,j);
        Image1->Canvas->LineTo(f_pix, j);
        Image1->Canvas->Pen->Color=clBlack;
      break;
   }
}
}*/
}

int faceCont::search(MyPoint* o, face& flat, int d)
{
  for(int i=0;i<6;i++)
  {
  double a=o[i].get_x()*flat.A+o[i].get_y()*flat.B+o[i].get_z()*flat.C+flat.D;
  if((a<0.1&&a>-0.1))
  {return i;}
  }
  return -1;
}


//---------------------------------------------------------------------------

#pragma package(smart_init)


