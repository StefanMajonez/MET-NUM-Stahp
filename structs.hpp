#include <sstream>
#include <iostream>
#include <cmath>
#include "gnuplot_i.hpp" // Biblioteka do sciagniecia na https://code.google.com/archive/p/gnuplot-cpp/
// Kompilujemy z warunkiem "-I."
#include <limits>
#include <vector>
#if defined(_WIN32) || defined(WIN32) || defined(__TOS_WIN__) || defined(__WIN32__)
#include <conio.h>
#endif
using namespace std;
//
// Program korzysta z programu GNUPlot 4.6 
//

enum TrygonType{
	sin
	cos
	tan
	cot
};

void wait_for_key(){
#if defined(_WIN32) || defined(WIN32) || defined(__TOS_WIN__) || defined(__WIN32__)
  cout << endl << "Press any key to continue..." << endl;
  //FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
  _getch();
#elif defined(__unix) || defined(unix) || defined(__APPLE__) || defined(__UNIX__)
  cout << endl << "Press ENTER to continue..." << endl;
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(),'\n');
  cin.get();
#endif
  return;
}

struct Wielomian{
  int stopien;
  double *wartosci;
  stringstream function;
  Wielomian();
  void wczytaj();
  ~Wielomian(){delete[] wartosci;}
  double policz(double);
  void narysuj();
};

struct Trygon{
  double a;
  double policz(){
    return sin(a);
  }
};

struct Wyklad{
  stringstream function;
  double podst, mnoz, dod;
  void wczytaj();
  double policz(double);
  void narysuj();
};


// ################
// ### FUNKCJE  ###
// ################

Wielomian::Wielomian(){
  int st;
  cout<<"Podaj stopieñ wielomianu:";
  cin>>st;
  stopien = st;
  wartosci = new double[st+1];
}

void Wielomian::wczytaj(){
  cout<<"Podaj wartosci dla ka¿dego stopnia zmiennej Dla: \n";
  cout << "wyrazu wolnego: ";
  cin>>wartosci[0];
  function << wartosci[0];
  for(int i=1;i<=stopien;i++){
    cout<<"x^"<<i<<":";
    cin>>wartosci[i];
    function << "+" << wartosci[i];
    for(int j = 0; j <i; j++)
    function << "*x";
  }
  return;
}

double Wielomian::policz(double x){
  double wynik = 0;
  for(int i=0;i<=stopien;i++){
    wynik += pow(x,i)*wartosci[i];
  }
  return wynik;
}

void Wielomian::narysuj(){
  Gnuplot plot;
  plot.set_grid();
  plot.set_style("lines").plot_equation(function.str()).plot_equation("0", "OX");
  wait_for_key();
}

void Wyklad::wczytaj(){
  cout<<"Funkcja postaci a*b^x+c.\nPodaj a:";
  cin>>mnoz;
  function << mnoz << "*";
  cout<<"Podaj b:";
  cin>>podst;
  cout<<"Podaj c:";
  cin>>dod;
  return;
}

double Wyklad::policz(double x){
  double wynik = mnoz * pow(podst,x) + dod;
  return wynik;
}

void Wyklad::narysuj(double x,double y){
  double i = -10;
  vector<double> xtab,ytab,ptx,pty;
  ptx.push_back(x);
  ptx.push_back(y);
  pty.push_back(policz(x));
  pty.push_back(policz(y));
  while((i+=0.0625)<10){
    xtab.push_back(i);
	ytab.push_back(policz(i));
  }
  Gnuplot plot;
  plot.set_grid();
  plot.set_style("points").plot_xy(xtab,ytab,"Wykres funkcji").plot_xy(ptx,pty,"Punkty");
  wait_for_key();
}


