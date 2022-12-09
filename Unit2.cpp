//---------------------------------------------------------------------------


#pragma hdrstop

#include "Unit2.h"
MyPoint::MyPoint()
{
x=0;
y=0;
z=0;
}
MyPoint::MyPoint(char l, double i, double j, double k)
{
let=l;
x=x0=i;
y=y0=j;
z=z0=k;
}
double MyPoint::get_x()
{
return x;
}
double MyPoint::get_y()
{
return y;
}
double MyPoint::get_z()
{
return z;
}
void MyPoint::set_x(double a)
{
x=a;
}
void MyPoint::set_y(double a)
{
y=a;
}
void MyPoint::set_z(double a)
{
z=a;
}
void MyPoint::set_x()
{
x=x0;
}
void MyPoint::set_y()
{
y=y0;
}
void MyPoint::set_z()
{
z=z0;
}
 MyPoint:: ~MyPoint(){}
 char MyPoint::get_let()
 {
 return let;
 }
 //---------------------------------------------------------------------------

#pragma package(smart_init)

