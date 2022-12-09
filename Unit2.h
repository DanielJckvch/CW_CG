//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
class MyPoint
{
private:
char let;

double x;
double y;
double z;

double x0;
double y0;
double z0;

public:

double get_x();
double get_y();
double get_z();
char get_let();
void set_x(double a);
void set_y(double a);
void set_z(double a);
void set_x();
void set_y();
void set_z();
~MyPoint();
MyPoint(char l, double i, double j, double k);
 MyPoint();
} ;
//---------------------------------------------------------------------------
#endif
 