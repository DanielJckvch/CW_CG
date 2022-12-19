//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
#include <math.h>
#include <vector>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;
TForm1 *Form1;
//Призма. Поворот и перенос вокруг всех осей, масштабирование, закраска и удаление невидимых поверхностей
//Определить поведение при выходе за гран экрана, вынести функции в файл,
void print(TImage* Image1, int mode=0);//Печатание призмы
void rotandscale(MyPoint* o, int pointNum, bool sw, bool sign);//Поворот и масштабирование
void prisminit(MyPoint* o);//Инициализация призмы
void bresline(TColor*, int, int, int, int, int);//Прочерчивание линии алгоритмом Брезенхэиа
void fillFaceList(int fig);//Инициализация списка граней
void countCoeffs(face* verges, int pointNum,int vergesNum);//Расчёт уравнений нормали граней
void openBuffer(TImage* Image1);//Открытие буфера для двойной буферизации
void closeBuffer(void);//Закрытие буфера
void surfaceFill(TColor* Image, unsigned int fillColor, unsigned int brColor, int w, int x, int y);//Заливка грани
int isOuter(int* w, int i);
void getGab(int i);
int getFaceNum(int* w, double* zMaxIn=0);
int isVisible(double x, double y, double p1x, double p1y, double p2x, double p2y);
double getZ(int* w, int i);
void copy(double* vp, double* p);
void pyrinit(MyPoint* o);
void getProjection(TImage* Image1,MyPoint* o,double** projObj, int pointNum);
void makeShadow(TImage* Image1,MyPoint* o,MyPoint* sdwOb,double** projObj, int pointNum);

//Контейнеры объектов
MyPoint* prism;
MyPoint* pyr;
MyPoint* prismSdw;
MyPoint* pyrSdw;
face* prismVerges;
face* pyrVerges;
double** prismProj;
double** pyrProj;
int light[3]={100,100,2147483648};//200 is d

//Для двойной буферизации
TColor* buff=0;
BITMAPINFO info;

//Размеры объектов
double h=30.0;
double prismEdge=50;
double pyrEdge=50;
double pyrH=pyrEdge*0.866;
int d=200;
double z_plane=50.0;
double z=500.0;
int pyrUp=20;
int prismUp=50;
int pointNum=4;
int vergesNum=4;

//Угол поворота, шаг сдвига и коэффициент масштабирования
double a_step=10;
int sc_step=2;
int mov_step=10;

//Режимы обработки объектов
int axis_mode=0;
int proj_mode=0;
int fig_proc_mode=0;



//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Form1Create(TObject *Sender)
{
//Создание контейнеров значений точек
prism=new MyPoint[6];
pyr=new MyPoint[4];
prismSdw=new MyPoint[6];
pyrSdw=new MyPoint[4];
prismVerges=new face[6];
pyrVerges=new face[4];
prismProj=new double*[6];
for(int i=0;i<6;i++)
{
 prismProj[i]=new double[4];
}
pyrProj=new double*[4];
for(int i=0;i<4;i++)
{
 pyrProj[i]=new double[4];
}
int a=sizeof(int);
//Инициализация и вывод фигур
//Подготовка bitmap
pyrinit(pyr);
prisminit(prism);
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
//Создание тени
makeShadow(Image1,pyr,pyrSdw,pyrProj,4);
makeShadow(Image1,prism,prismSdw,prismProj,6);
//Расчёт проекций и граней для тени
getProjection(Image1,pyrSdw,pyrProj,4);
getProjection(Image1,prismSdw,prismProj,6);
fillFaceList(0);
fillFaceList(1);
getGab(0);
getGab(1);
countCoeffs(pyrVerges, 4,4);
countCoeffs(prismVerges, 6,5);
//Вывод тени
openBuffer(Image1);
print(Image1);
//Расчёт проекций и граней для фигур
getProjection(Image1,pyr,pyrProj,4);
getProjection(Image1,prism,prismProj,6);
fillFaceList(0);
fillFaceList(1);
getGab(0);
getGab(1);
countCoeffs(pyrVerges, 4,4);
countCoeffs(prismVerges, 6,5);
//Вывод фигур
print(Image1, 1);
Edit1->Text="X";
Edit2->Text="Isometric";
Edit3->Text="Pyramid";
closeBuffer();

}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
//Поворот против часовой стрелки
MyPoint* ob;
if(fig_proc_mode==0)
{
 ob=pyr;
}
else
{
 ob=prism;
}
rotandscale(ob,pointNum, 0, 1);
//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
//Создание тени
makeShadow(Image1,pyr,pyrSdw,pyrProj,4);
makeShadow(Image1,prism,prismSdw,prismProj,6);
//Расчёт проекций и граней для тени
getProjection(Image1,pyrSdw,pyrProj,4);
getProjection(Image1,prismSdw,prismProj,6);
fillFaceList(0);
fillFaceList(1);
getGab(0);
getGab(1);
countCoeffs(pyrVerges, 4,4);
countCoeffs(prismVerges, 6,5);
//Вывод тени
openBuffer(Image1);
print(Image1);
//Расчёт проекций и граней для фигур
getProjection(Image1,pyr,pyrProj,4);
getProjection(Image1,prism,prismProj,6);
fillFaceList(0);
fillFaceList(1);
getGab(0);
getGab(1);
countCoeffs(pyrVerges, 4,4);
countCoeffs(prismVerges, 6,5);
//Вывод фигур
print(Image1, 1);
closeBuffer();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
//Поворот по часовой стрелке
MyPoint* ob;
if(fig_proc_mode==0)
{
 ob=pyr;
}
else
{
 ob=prism;
}
rotandscale(ob,pointNum, 0, 0);
//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
//Создание тени
makeShadow(Image1,pyr,pyrSdw,pyrProj,4);
makeShadow(Image1,prism,prismSdw,prismProj,6);
//Расчёт проекций и граней для тени
getProjection(Image1,pyrSdw,pyrProj,4);
getProjection(Image1,prismSdw,prismProj,6);
fillFaceList(0);
fillFaceList(1);
getGab(0);
getGab(1);
countCoeffs(pyrVerges, 4,4);
countCoeffs(prismVerges, 6,5);
//Вывод тени
openBuffer(Image1);
print(Image1);
//Расчёт проекций и граней для фигур
getProjection(Image1,pyr,pyrProj,4);
getProjection(Image1,prism,prismProj,6);
fillFaceList(0);
fillFaceList(1);
getGab(0);
getGab(1);
countCoeffs(pyrVerges, 4,4);
countCoeffs(prismVerges, 6,5);
//Вывод фигур
print(Image1, 1);
closeBuffer();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
//Масштаб -
MyPoint* ob;
if(fig_proc_mode==0)
{
 ob=pyr;
}
else
{
 ob=prism;
}
rotandscale(ob,pointNum, 1, 1);
//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
//Создание тени
makeShadow(Image1,pyr,pyrSdw,pyrProj,4);
makeShadow(Image1,prism,prismSdw,prismProj,6);
//Расчёт проекций и граней для тени
getProjection(Image1,pyrSdw,pyrProj,4);
getProjection(Image1,prismSdw,prismProj,6);
fillFaceList(0);
fillFaceList(1);
getGab(0);
getGab(1);
countCoeffs(pyrVerges, 4,4);
countCoeffs(prismVerges, 6,5);
//Вывод тени
openBuffer(Image1);
print(Image1);
//Расчёт проекций и граней для фигур
getProjection(Image1,pyr,pyrProj,4);
getProjection(Image1,prism,prismProj,6);
fillFaceList(0);
fillFaceList(1);
getGab(0);
getGab(1);
countCoeffs(pyrVerges, 4,4);
countCoeffs(prismVerges, 6,5);
//Вывод фигур
print(Image1, 1);
closeBuffer();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
//Масштаб +
MyPoint* ob;
if(fig_proc_mode==0)
{
 ob=pyr;
}
else
{
 ob=prism;
}
rotandscale(ob,pointNum, 1, 0);
//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
//Создание тени
makeShadow(Image1,pyr,pyrSdw,pyrProj,4);
makeShadow(Image1,prism,prismSdw,prismProj,6);
//Расчёт проекций и граней для тени
getProjection(Image1,pyrSdw,pyrProj,4);
getProjection(Image1,prismSdw,prismProj,6);
fillFaceList(0);
fillFaceList(1);
getGab(0);
getGab(1);
countCoeffs(pyrVerges, 4,4);
countCoeffs(prismVerges, 6,5);
//Вывод тени
openBuffer(Image1);
print(Image1);
//Расчёт проекций и граней для фигур
getProjection(Image1,pyr,pyrProj,4);
getProjection(Image1,prism,prismProj,6);
fillFaceList(0);
fillFaceList(1);
getGab(0);
getGab(1);
countCoeffs(pyrVerges, 4,4);
countCoeffs(prismVerges, 6,5);
//Вывод фигур
print(Image1, 1);
closeBuffer();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
//Сдвиг против направления оси
MyPoint* ob;
if(fig_proc_mode==0)
{
 ob=pyr;
}
else
{
 ob=prism;
}
switch(axis_mode)
{
 case 0:
  for(int i=0; i<pointNum; i++)
  {
   ob[i].set_x(ob[i].get_x()-mov_step);
  }
  break;
 case 1:
  for(int i=0; i<pointNum; i++)
  {
   ob[i].set_y(ob[i].get_y()-mov_step);
  }
  break;
 case 2:
  for(int i=0; i<pointNum; i++)
  {
   ob[i].set_z(ob[i].get_z()-mov_step);
  }
  break;
}

//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
//Создание тени
makeShadow(Image1,pyr,pyrSdw,pyrProj,4);
makeShadow(Image1,prism,prismSdw,prismProj,6);
//Расчёт проекций и граней для тени
getProjection(Image1,pyrSdw,pyrProj,4);
getProjection(Image1,prismSdw,prismProj,6);
fillFaceList(0);
fillFaceList(1);
getGab(0);
getGab(1);
countCoeffs(pyrVerges, 4,4);
countCoeffs(prismVerges, 6,5);
//Вывод тени
openBuffer(Image1);
print(Image1);
//Расчёт проекций и граней для фигур
getProjection(Image1,pyr,pyrProj,4);
getProjection(Image1,prism,prismProj,6);
fillFaceList(0);
fillFaceList(1);
getGab(0);
getGab(1);
countCoeffs(pyrVerges, 4,4);
countCoeffs(prismVerges, 6,5);
//Вывод фигур
print(Image1, 1);
closeBuffer();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
//Сдвиг по направлению оси
MyPoint* ob;
if(fig_proc_mode==0)
{
 ob=pyr;
}
else
{
 ob=prism;
}
switch(axis_mode)
{
 case 0:
  for(int i=0; i<pointNum; i++)
  {
   ob[i].set_x(ob[i].get_x()+mov_step);
  }
  break;
 case 1:
  for(int i=0; i<pointNum; i++)
  {
   ob[i].set_y(ob[i].get_y()+mov_step);
  }
  break;
 case 2:
  for(int i=0; i<pointNum; i++)
  {
   ob[i].set_z(ob[i].get_z()+mov_step);
  }
  break;
}
//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
//Создание тени
makeShadow(Image1,pyr,pyrSdw,pyrProj,4);
makeShadow(Image1,prism,prismSdw,prismProj,6);
//Расчёт проекций и граней для тени
getProjection(Image1,pyrSdw,pyrProj,4);
getProjection(Image1,prismSdw,prismProj,6);
fillFaceList(0);
fillFaceList(1);
getGab(0);
getGab(1);
countCoeffs(pyrVerges, 4,4);
countCoeffs(prismVerges, 6,5);
//Вывод тени
openBuffer(Image1);
print(Image1);
//Расчёт проекций и граней для фигур
getProjection(Image1,pyr,pyrProj,4);
getProjection(Image1,prism,prismProj,6);
fillFaceList(0);
fillFaceList(1);
getGab(0);
getGab(1);
countCoeffs(pyrVerges, 4,4);
countCoeffs(prismVerges, 6,5);
//Вывод фигур
print(Image1, 1);
closeBuffer();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
//Выбор оси
if(++axis_mode==3)
{axis_mode=0;}
switch(axis_mode)
{
case 0:
Edit1->Text="X";
break;
case 1:
Edit1->Text="Y";
break;
case 2:
Edit1->Text="Z";
break;
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender)
{
//Переключение режима проекции
MyPoint* ob;
if(fig_proc_mode==0)
{
 ob=pyr;
}
else
{
 ob=prism;
}
if(++proj_mode==3)
{proj_mode=0;}
switch(proj_mode)
{
case 0:
Edit2->Text="Isometric";
break;
case 1:
Edit2->Text="Cavalier";
break;
case 2:
Edit2->Text="Perspective";
break;
}
//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
//Создание тени
makeShadow(Image1,pyr,pyrSdw,pyrProj,4);
makeShadow(Image1,prism,prismSdw,prismProj,6);
//Расчёт проекций и граней для тени
getProjection(Image1,pyrSdw,pyrProj,4);
getProjection(Image1,prismSdw,prismProj,6);
fillFaceList(0);
fillFaceList(1);
getGab(0);
getGab(1);
countCoeffs(pyrVerges, 4,4);
countCoeffs(prismVerges, 6,5);
//Вывод тени
openBuffer(Image1);
print(Image1);
//Расчёт проекций и граней для фигур
getProjection(Image1,pyr,pyrProj,4);
getProjection(Image1,prism,prismProj,6);
fillFaceList(0);
fillFaceList(1);
getGab(0);
getGab(1);
countCoeffs(pyrVerges, 4,4);
countCoeffs(prismVerges, 6,5);
//Вывод фигур
print(Image1, 1);
closeBuffer();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button9Click(TObject *Sender)
{
 //Выбор обрабатываемой фигуры
 if(--fig_proc_mode==-1)
 {
  fig_proc_mode=1;
 }
 switch(fig_proc_mode)
 {
  case 0:
   Edit3->Text="Pyramid";
   pointNum=4;
   vergesNum=4;
   break;
  case 1:
   Edit3->Text="Prism";
   pointNum=6;
   vergesNum=5;
   break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Form1Close(TObject *Sender, TCloseAction &Action)
{
    if(buff)
    {
     delete buff;
    }
    delete[] prism;
    delete[] prismVerges;
 delete[] pyrVerges;
 delete[] pyr;
 delete[] prismSdw;
 delete[] pyrSdw;
 for(int i=0;i<6;i++)
 {
  delete [] prismProj[i];
 }
 delete [] prismProj;
 for(int i=0;i<4;i++)
 {
  delete[] pyrProj[i];
 }
 delete[] pyrProj;
}
//---------------------------------------------------------------------------

void print(TImage* Image1, int mode)
{
vector<int> stack;
int N=9;
int out=0;
int faceNum=0;
int imSize=Image1->Width;
int window[] = {0,0,0};
window[2]=imSize;
stack.push_back(window[0]);
stack.push_back(window[1]);
stack.push_back(window[2]);
while(!stack.empty())
{
  window[2]=stack.back();
  stack.pop_back();
  window[1]=stack.back();
  stack.pop_back();
  window[0]=stack.back();
  stack.pop_back();
  imSize=window[2];
  out=0;
  for(int i=0;i<N && out==0;i++)
  {
   out = isOuter(window,i);
  }
  if(out!=0)
  {
   if(imSize>1)
   {
    imSize/=2;
    //
    stack.push_back(window[0]+imSize);
    stack.push_back(window[1]+imSize);
    stack.push_back(imSize);
    //
    stack.push_back(window[0]);
    stack.push_back(window[1]+imSize);
    stack.push_back(imSize);
    //
    stack.push_back(window[0]+imSize);
    stack.push_back(window[1]);
    stack.push_back(imSize);
    //
    stack.push_back(window[0]);
    stack.push_back(window[1]);
    stack.push_back(imSize);
    //
   }
   else
   {
    faceNum=getFaceNum(window);
    if(faceNum!=-1)
    {
     face* verges;
     if(faceNum>3)
     {
      faceNum-=4;
      verges=prismVerges;
     }
     else
     {
      verges=pyrVerges;
     }
     if(mode==0)
     {
      buff[window[0]+(window[1])*Image1->Width]=0;
     }
     else
     {
      buff[window[0]+(window[1])*Image1->Width]=verges[faceNum].color;
     }
    }
   }
  }
}

 //Вывод буфера на экран
 SetDIBits(Image1->Picture->Bitmap->Canvas->Handle,Image1->Picture->Bitmap->Handle,0,Image1->Height,buff,&info,DIB_RGB_COLORS);
 //Обозначение вершин буквами
if(mode==1)
{
 face* verges=pyrVerges;
 MyPoint* ob=pyr;
 int letPoint[3]={0,0,1};
 double zMax=0;
 for(int i=0;i<5;i++)
 {
  if(i>3&&verges==pyrVerges)
  {
   i-=4;
   verges=prismVerges;
   ob=prism;
  }
  for(int j=0;j<4-verges[i].trian;j++)
  {
   letPoint[0]=verges[i].f[j][0];
   letPoint[1]=verges[i].f[j][1];
   getFaceNum(letPoint, &zMax);
   //Если буква не перекрывается какой-либо гранью, она должна быть выведена
   if(verges[i].f[j][2]>=zMax-2)
   {
    Image1->Canvas->TextOutA(verges[i].f[j][0],verges[i].f[j][1],ob[verges[i].index[j]].get_let());
   }
  }
 }
 }

}
//---------------------------------------------------------------------------

void rotandscale(MyPoint* o, int pointNum, bool sw, bool sign)
{
 double s=sc_step;
 double a=a_step;
 if(sign)
 {
  a=a*(-1);
  s=1/s;
 }
 double r[4][4];
 // Выбор способа преобразования
 if(sw)
 {
  r[0][0]=s; r[0][1]=0; r[0][2]=0; r[0][3]=0;
  r[1][0]=0; r[1][1]=s; r[1][2]=0; r[1][3]=0;
  r[2][0]=0; r[2][1]=0; r[2][2]=s; r[2][3]=0;
  r[3][0]=0; r[3][1]=0; r[3][2]=0; r[3][3]=1;
 }
 else
 {
  switch(axis_mode)
  {
   case 0:
    r[0][0]=1; r[0][1]=0; r[0][2]=0; r[0][3]=0;
    r[1][0]=0; r[1][1]=cos(a*3.14/180); r[1][2]=sin(a*3.14/180); r[1][3]=0;
    r[2][0]=0; r[2][1]=-sin(a*3.14/180); r[2][2]=cos(a*3.14/180); r[3][3]=0;
    r[3][0]=0; r[3][1]=0; r[3][2]=0; r[3][3]=1;
    break;
   case 1:
    r[0][0]=cos(a*3.14/180); r[0][1]=0; r[0][2]=-sin(a*3.14/180); r[0][3]=0;
    r[1][0]=0; r[1][1]=1; r[1][2]=0; r[1][3]=0;
    r[2][0]=sin(a*3.14/180); r[2][1]=0; r[2][2]=cos(a*3.14/180); r[3][3]=0;
    r[3][0]=0; r[3][1]=0; r[3][2]=0; r[3][3]=1;
    break;
   case 2:
    r[0][0]=cos(a*3.14/180); r[0][1]=sin(a*3.14/180); r[0][2]=0; r[0][3]=0;
    r[1][0]=-sin(a*3.14/180); r[1][1]=cos(a*3.14/180); r[1][2]=0; r[1][3]=0;
    r[2][0]=0; r[2][1]=0; r[2][2]=1; r[3][3]=0;
    r[3][0]=0; r[3][1]=0; r[3][2]=0; r[3][3]=1;
    break;
  }
 }
 int k=0;
 // Преобразование точек
 for(k=0;k<pointNum;k++)
 {
  double v1[4]={o[k].get_x(),o[k].get_y(),o[k].get_z(),1};
  double v2[4]={0, 0, 0, 1};
  int i, j;
  for (i = 0;i < 4;i++)
  {
        double sum = 0;
        for (j = 0;j < 4;j++)
        {
         sum= sum+ (r[j][i] * v1[j]);
        }
        v2[i] = sum;
  }
  o[k].set_x(v2[0]);
  o[k].set_y(v2[1]);
  o[k].set_z(v2[2]);
 }
}
//----------------------------------------------------------------------------
void prisminit(MyPoint* o)
{
  o[0]=MyPoint('A',0.0,prismEdge,h/2+prismUp);
  o[1]=MyPoint(char('A'+1),0.0,0.0,h/2+prismUp);
  o[2]=MyPoint(char('A'+2),prismEdge,0.0,h/2+prismUp);
 for(int i=3; i<6; i++)
 {
 o[i]=MyPoint(char('A'+i),o[i-3].get_x(),o[i-3].get_y(),o[i-3].get_z()+h+prismUp);
 }
}
//---------------------------------------------------------------------------
void pyrinit(MyPoint* o)
{
  o[0]=MyPoint(char('A'),0.0,pyrEdge/1.73205,pyrUp);
  o[1]=MyPoint(char('A'+1),(pyrEdge/1.73205)*0.86603,(pyrEdge/1.73205)*-0.5,pyrUp);
  o[2]=MyPoint(char('A'+2),(pyrEdge/1.73205)*-0.86603,(pyrEdge/1.73205)*-0.5,pyrUp);
  o[3]=MyPoint(char('A'+3),0.0,0.0,pyrH+pyrUp);
}
//---------------------------------------------------------------------------

void bresline(TColor* Image, int w, int x0, int y0, int x1, int y1)
{
// Отрисовка линии алгоритмом Брезенхэма
  int sx=x0<x1?1:-1;
  int dx=(x1-x0)*sx;
  int sy=y0<y1?1:-1;
  int dy=(y1-y0)*sy;
  int err1=(dx>dy?dx:-dy)/2;
  int err2=err1;
  for(;;)
  {
  Image[x0+y0*w]=clBlack;
  if(x0==x1&&y0==y1)
  {
  break;
  }
  err2=err1;
  if(err2>-dx)
  {
  err1-=dy;
  x0+=sx;
  }
  if(err2<dy)
  {
  err1+=dx;
  y0+=sy;
  }
  }
}
//---------------------------------------------------------------------------

void fillFaceList(int fig)
{
if(fig==1)
{
//Заполнение списка граней призмы
 face* verges=prismVerges;
 for(int k=0;k<2;k++)
 {
  for(int i=0;i<3;i++)
  {
   //Внесение в список треугольных граней(оснований)
   copy(verges[k].f[i], prismProj[i+k*3]);
   verges[k].index[i]=i+k*3;
   verges[k].trian=true;
  }
 }
//Внесение в список треугольных граней(оснований)
/*Нумерация вершин(с внешней строны грани ACDF): 1-левая нижняя,
2-правая нижняя, 3-левая верхняя, 4-правая верхняя*/
for(int k=2;k<5;k++)
{
 for(int i=0;i<2;i++)
 {
  if(k==4)
  { //Грань ACFD-замыкающая
   if(i==0)
   {
    copy(verges[k].f[i], prismProj[2]);
    verges[k].index[i]=2;
   }
   else
   {
    copy(verges[k].f[i], prismProj[5]);
    verges[k].index[i]=5;
   }
  }
  else
  {//Грани BADE и CBEF
   int skip=(i>0)?2:0;
   copy(verges[k].f[i], prismProj[i+skip+k-2]);
   verges[k].index[i]=i+skip+k-2;
  }
 }

 for(int i=2;i<4;i++)
 {
  if(k==4)
  {
   if(i==3)
   {
    copy(verges[k].f[i], prismProj[0]);
    verges[k].index[i]=0;
   }
   else
   {
    copy(verges[k].f[i], prismProj[3]);
    verges[k].index[i]=3;
   }
  }
  else
  {//Грани BADE и CBEF
   int skip=(i==2)?2:-2;
   copy(verges[k].f[i], prismProj[i+skip+k-2]);
   verges[k].index[i]=i+skip+k-2;
  }
 }
}
 //Инициализация цвета поверхности
 verges[0].color=0x00FF00FF;//Фиолетовый
 verges[1].color=0x00FF0000;//Красный
 verges[2].color=0x0000FF00;//Зелёный
 verges[3].color=0x000000FF;//Синий
 verges[4].color=0x00FFFF00;//Жёлтый
}
else
{
//Заполнение списка граней пирамиды
face* verges=pyrVerges;
for(int i=0;i<3;i++)
{
  //Внесение в список основания
  copy(verges[0].f[i], pyrProj[i]);
  verges[0].index[i]=i;
  verges[0].trian=true;
}

//Внесение в список боковых сторон
for(int k=1;k<3;k++)
{
 for(int i=0;i<3;i++)
 {

  if(i==1)
  {
   copy(verges[k].f[i], pyrProj[3]);
   verges[k].index[i]=3;
  }
  else if(i==0)
  {
   copy(verges[k].f[i], pyrProj[i+k-1]);
   verges[k].index[i]=i+k-1;
  }
  else
  {
   copy(verges[k].f[i], pyrProj[i+k-2]);
   verges[k].index[i]=i+k-2;
  }
 }
 verges[k].trian=true;
}
 //Грань замыкающая
 copy(verges[3].f[0], pyrProj[2]);
 verges[3].index[0]=2;
 copy(verges[3].f[1], pyrProj[3]);
 verges[3].index[1]=3;
 copy(verges[3].f[2], pyrProj[0]);
 verges[3].index[2]=0;
 verges[3].trian=true;
 //Инициализация цвета поверхности
 verges[0].color=0x00FF0000;//Красный
 verges[1].color=0x00FF00FF;//Фиолетовый
 verges[2].color=0x0000FF00;//Зелёный
 verges[3].color=0x000000FF;//Синий
 //verges[4].color=0x00FFFF00;//Жёлтый
}
}
//---------------------------------------------------------------------------

void countCoeffs(face* verges, int pointNum,int vergesNum)
{
//Расчёт коэффициентов уравнения плоскостей
double v1[3];
double v2[3];
 //Вычисление векторов нормали
 for(int i=0;i<vergesNum;i++)
 {
  for(int j=0;j<3;j++)
  {
   v1[j]=verges[i].f[0][j]-verges[i].f[2][j];
   v2[j]=verges[i].f[1][j]-verges[i].f[2][j];
  }
verges[i].A=v1[1]*v2[2]-v1[2]*v2[1];
verges[i].B=v2[0]*v1[2]-v1[0]*v2[2];
verges[i].C=v1[0]*v2[1]-v1[1]*v2[0];
verges[i].D=-1*(verges[i].A*verges[i].f[0][0]+verges[i].B*verges[i].f[0][1]+verges[i].C*verges[i].f[0][2]);

}
}
//---------------------------------------------------------------------------

void openBuffer(TImage* Image1)
{//Заполнение информационного заголовка о bitmap
info.bmiHeader.biSize=sizeof(BITMAPINFOHEADER );
info.bmiHeader.biBitCount=0;
info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
GetDIBits(Image1->Picture->Bitmap->Canvas->Handle,Image1->Picture->Bitmap->Handle,0,Image1->Picture->Bitmap->Height,NULL,(BITMAPINFO*)&info,DIB_RGB_COLORS);
//Открытие буфера и его заполнение
buff=new TColor[info.bmiHeader.biHeight*info.bmiHeader.biWidth];
info.bmiHeader.biCompression=BI_RGB;
info.bmiHeader.biHeight*=-1;
GetDIBits(Image1->Picture->Bitmap->Canvas->Handle,Image1->Picture->Bitmap->Handle,0,Image1->Picture->Bitmap->Height,buff,(BITMAPINFO*)&info,DIB_RGB_COLORS);
}
//---------------------------------------------------------------------------

void closeBuffer(void)
{
//Очистка буфера
delete buff;
buff=0;
}
//---------------------------------------------------------------------------

void surfaceFill(TColor* Image, unsigned int fillColor, unsigned int brColor, int w, int x, int y)
{
 if((Image[x+y*w]!=brColor)&&(Image[x+y*w]!=fillColor)&&(y*w<512*512)&&x>=0&&y>=0)
 {
 Image[x+y*w]=fillColor;
 surfaceFill(Image, fillColor, brColor, w, x+1, y);
 surfaceFill(Image, fillColor, brColor, w, x, y+1);
 surfaceFill(Image, fillColor, brColor, w, x-1, y);
 surfaceFill(Image, fillColor, brColor, w, x, y-1);
 }
}
//--------------------------------------------------------------------------
int isOuter(int* w, int i)
{
  face* verges;
  if(i>3)
  {
   i-=4;
   verges=prismVerges;
  }
  else
  {
   verges=pyrVerges;
  }
  int xl = w[0];
  int xr = w[0]+w[2]-1;
  int yd = w[1];
  int yu = w[1]+w[2]-1;
  int out=1;
  if(verges[i].gab[0]>xr){out=0;}
  if(verges[i].gab[1]<xl){out=0;}
  if(verges[i].gab[2]>yu){out=0;}
  if(verges[i].gab[3]<yd){out=0;}
  return out;
}
//---------------------------------------------------------------------------
void getGab(int fig)
{
  face* verges;
  int N;
  if(fig==1)
  {
   verges=prismVerges;
   N=6;
  }
  else
  {
   verges=pyrVerges;
   N=4;
  }
  for(int i=0;i<N;i++)
  {
    verges[i].gab[0]=verges[i].f[0][0];
    verges[i].gab[1]=verges[i].f[0][0];
    verges[i].gab[2]=verges[i].f[0][1];
    verges[i].gab[3]=verges[i].f[0][1];
    for(int j=0;j<4-verges[i].trian;j++)
    {
     if(verges[i].gab[0]>verges[i].f[j][0])
     {verges[i].gab[0]=verges[i].f[j][0];}
     if(verges[i].gab[1]<verges[i].f[j][0])
     {verges[i].gab[1]=verges[i].f[j][0];}
     if(verges[i].gab[2]>verges[i].f[j][1])
     {verges[i].gab[2]=verges[i].f[j][1];}
     if(verges[i].gab[3]<verges[i].f[j][1])
     {verges[i].gab[3]=verges[i].f[j][1];}
    }
  }
}
/*Нумерация вершин(с внешней строны грани ACDF): 1-левая нижняя,
2-правая нижняя, 3-левая верхняя, 4-правая верхняя*/
int getFaceNum(int* w, double* zMaxIn)
{
 face* verges=pyrVerges;
 int faceNum=-1;
 int vis=-2;
 double zMax=-100000.0;
 double p1x, p1y, p2x, p2y;
 double xCen=w[0]+w[2]/2;
 double yCen=w[1]+w[2]/2;
 double z=0;
 int N=9;
 int k=0;
 for(int i=0;i<5;i++,k++)
 { if(i>3&&verges==pyrVerges)
   {
    i-=4;
    verges=prismVerges;
   }
   int j=0;
   while(j<3-verges[i].trian)
   {
    p1x=verges[i].f[j][0];
    p1y=verges[i].f[j][1];
    p2x=verges[i].f[j+1][0];
    p2y=verges[i].f[j+1][1];
    if(vis==-2)
    {
     vis=isVisible(xCen, yCen, p1x, p1y, p2x, p2y);
    }
    else if(vis!=isVisible(xCen, yCen, p1x, p1y, p2x, p2y))
    {
     vis=-2;
     break;
    }
    j++;
   }
   if(vis==-2)
   {continue;}
   p1x=verges[i].f[j][0];
   p1y=verges[i].f[j][1];
   p2x=verges[i].f[0][0];
   p2y=verges[i].f[0][1];
   if(vis==isVisible(xCen, yCen, p1x, p1y, p2x, p2y))
   {
    z=getZ(w,k);
    if(z>zMax)
    {
     zMax=z;
     faceNum=k;
     //zBuff[w[0]+w[1]*Image1->Width]=1;
    }
   }
   vis=-2;
 }
 if(zMaxIn)
 {
  *zMaxIn=zMax;
 }
 return faceNum;
}


//-----------------------------------------------------------------------------
int isVisible(double x, double y, double p1x, double p1y, double p2x, double p2y)
{
 int vis=0;
 double prod=0;
 prod=(x-p1x)*(p2y-p1y)-(y-p1y)*(p2x-p1x);
 if(prod!=0)
 {
  vis=(prod<0)?-1:1;
 }
 return vis;
}
//----------------------------------------------------------------------------
double getZ(int* w, int i)
{
  face* verges;
  int N;
  if(i>3)
  {
   i-=4;
   verges=prismVerges;
   N=6;
  }
  else
  {
   verges=pyrVerges;
   N=4;
  }
 double zp;
 double xCen=w[0]+w[2]/2;
 double yCen=w[1]+w[2]/2;
 if(verges[i].C==0.0)
 {
  for(int j=1;j<N;j++)
  {
   if(verges[i].f[j][2]<verges[i].f[j-1][2])
   {zp=verges[i].f[j-1][2];}
   else
   {zp=verges[i].f[j][2];}
  }
 }
 else
 {
  zp=-1*(verges[i].A*xCen+verges[i].B*yCen+verges[i].D)/verges[i].C;
 }
 return zp;
}
//---------------------------------------------------------------------------
void copy(double* vp, double* p)
{
 for(int j=0;j<4;j++)
 {
   vp[j]=p[j]+d;
 }
}

//---------------------------------------------------------------------------
void getProjection(TImage* Image1,MyPoint* o,double** projObj, int pointNum)
{
 int k;
 double pi = 3.1415926;
 double a; //x
 double b; //y
 double proj[4][4];//Матрица проекции
 double per[4][4]={0};//Матрица перспективы
//Выбор проекционного преобразования
 switch(proj_mode)
 {
  case 0:
   a=45.0*pi/180; //x
   b=35.26*pi/180; //y
   proj[0][0]=cos(a);proj[0][1]=sin(a)*cos(b);proj[0][2]=sin(a)*sin(b);proj[0][3]=0;
   proj[1][0]=-sin(a);proj[1][1]=cos(a)*cos(b);proj[1][2]=sin(b)*cos(a);proj[1][3]=0;
   proj[2][0]=0;proj[2][1]=-sin(b);proj[2][2]=cos(b);proj[2][3]=0;
   proj[3][0]=0;proj[3][1]=0;proj[3][2]=0;proj[3][3]=1;
   break;
  case 1:
   a=45.0*pi/180; //x
//b=35.26*pi/180; //y
   proj[0][0]=1;proj[0][1]=0;proj[0][2]=0;proj[0][3]=0;
   proj[1][0]=0;proj[1][1]=1;proj[1][2]=0;proj[1][3]=0;
   proj[2][0]=-cos(a);proj[2][1]=-sin(a);proj[2][2]=1;proj[2][3]=0;
   proj[3][0]=0;proj[3][1]=0;proj[3][2]=0;proj[3][3]=1;
   break;
  case 2:
   a=45.0*pi/180; //x
   b=35.26*pi/180; //y
   proj[0][0]=cos(a);proj[0][1]=sin(a)*cos(b);proj[0][2]=sin(a)*sin(b);proj[0][3]=0;
   proj[1][0]=-sin(a);proj[1][1]=cos(a)*cos(b);proj[1][2]=cos(a)*sin(b);proj[1][3]=0;
   proj[2][0]=0;proj[2][1]=-sin(b);proj[2][2]=cos(b);proj[2][3]=-z_plane;
   proj[3][0]=0;proj[3][1]=0;proj[3][2]=0;proj[3][3]=1;
   break;
 }
 for(k=0;k<pointNum;k++)
 {
  double v1[4]={o[k].get_x(),o[k].get_y(),o[k].get_z(),1};
  double v2[4]={0, 0, 0, 1};

  for (int i = 0;i < 4;i++)
  {
        double sum = 0;
        for (int j = 0;j < 4;j++)
        {
                sum= sum+ (proj[j][i] * v1[j]);
        }

        v2[i] = sum;
  }
  if(proj_mode==2)
  {
   int i, j;
   //Перспективное преобразование
   for(int i=0;i<4;i++)
   {
    per[i][i]=(z-z_plane)/(z-o[k].get_z());
   }
   for (int i = 0;i < 4;i++)
   {
        double sum = 0;
        for (int j = 0;j < 4;j++)
        {
                sum= sum+ (per[j][i] * v2[j]);
        }

        v2[i] = sum;
   }
  }
  projObj[k][0]= v2[0];
  projObj[k][1]= v2[1];
  projObj[k][2]= v2[2];
  projObj[k][3]= v2[3];
  //Проверка на выход точки за холст
  if(v2[0]+d<0||v2[0]+d>Image1->Width||v2[1]+d<0||v2[1]+d>Image1->Height)
  {
   if(o==prismSdw||o==pyrSdw)
   {
    for(int j=0;j<4;j++)
    {
     pyr[j].set_x();
     pyr[j].set_y();
     pyr[j].set_z();
    }
    for(int j=0;j<6;j++)
    {
     prism[j].set_x();
     prism[j].set_y();
     prism[j].set_z();
    }
    makeShadow(Image1,pyr,pyrSdw,pyrProj,4);
    makeShadow(Image1,prism,prismSdw,prismProj,6);
    k=-1;
    continue;
   }
   for(int j=0;j<pointNum;j++)
   {
    o[j].set_x();
    o[j].set_y();
    o[j].set_z();
   }
   k=-1;
   continue;
  }
 }
}
//---------------------------------------------------------------------------

void makeShadow(TImage* Image1,MyPoint* o,MyPoint* sdwOb,double** projObj, int pointNum)
{
 double shadProj[4][4]={{1,0,-1*light[0]/light[2],0},{0,1,-1*light[1]/light[2],0},{0,0,0,0},{0,0,0,1}};
 // Преобразование точек
  int i=0;
  for(int i=0;i<pointNum;i++)
  {
   double v1[4]={o[i].get_x(),o[i].get_y(),o[i].get_z(),1};
   double v2[4]={0, 0, 0, 1};
   for (int j = 0;j < 4;j++)
   {
        double sum = 0;
        for (int l = 0;l < 4;l++)
        {
         sum= sum+ (shadProj[j][l] * v1[l]);
        }
        v2[j] = sum;
   }
   sdwOb[i].set_x(v2[0]);
   sdwOb[i].set_y(v2[1]);
   sdwOb[i].set_z(v2[2]);
 }
}
