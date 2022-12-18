//---------------------------------------------------------------------------

#ifndef Unit3H
#define Unit3H
class face
{
  public:
  double** f;
  double A;
  double B;
  double C;
  double D;
  int index[4];
  double gab[4];
  unsigned int color;
  bool trian;
  face(void);
  ~face();
};
//---------------------------------------------------------------------------
#endif
 