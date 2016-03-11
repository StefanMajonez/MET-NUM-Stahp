#include <iostream>
#include <cmath>
#include <gnuplot_i.hpp> // Biblioteka do sciagniecia na https://code.google.com/archive/p/gnuplot-cpp/
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

struct Wielomian{
  int stopien;
  double *wartosci;
  Wielomian();
  void wczytaj();
  ~Wielomian(){delete[] wartosci;}
  double policz(double);
  void narysuj(double,double);
};

struct Trygon{
  
};

struct Wyklad{
  double podst, mnoz, dod;
  void wczytaj();
  double policz(double);
  void narysuj(double,double);
};

void wait_for_key();
template <class FUNKCJA> double falsi(double,double,bool,double,FUNKCJA);

// ################
// ##### MAIN #####
// ################

int main(){
  // 1. Wczytaj funkcję
  // 2. Narysuj tą funkcję dla użytkownika
  // 3. wczytaj left, right
  // 4. FALSI i bitwise
  return 0;
}

// ################
// ### FUNKCJIE ###
// ################

Wielomian::Wielomian(){
  int st;
  cout<<"Podaj stopień wielomianu:";
  cin>>st;
  stopien = st;
  wartosci = new double[st+1];
}

void Wielomian::wczytaj(){
  cout<<"Podaj wartosci dla każdego stopnia zmiennej\n Dla:";
  for(int i=0;i<=stopien;i++){
    cout<<"x^"<<i<<":";
    cin>>wartosci[i];
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

void Wielomian::narysuj(double x,double y){
  double i = -10;
  vector<double> xtab,ytab,ptx,pty;
  ptx.push_back(x).push_back(y);
  pty.push_back(policz(x)).push_back(policz(y));
  while((i+=0.0625)<10){
    xtab.push_back(i);
	ytab.push_back(policz(i));
  }
  Gnuplot plot;
  plot.set_grid().set_yrange(-10,10);
  plot.set_style("points").plot_xy(xtab,ytab,"Wykres funkcji").plot_xy(ptx,pty,"Punkty");
  wait_for_key();
}

void Wyklad::wczytaj(){
  cout<<"Funkcja postaci a*b^x+c.\nPodaj a:";
  cin>>mnoz;
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
  ptx.push_back(x).push_back(y);
  pty.push_back(policz(x)).push_back(policz(y));
  while((i+=0.0625)<10){
    xtab.push_back(i);
	ytab.push_back(policz(i));
  }
  Gnuplot plot;
  plot.set_grid().set_yrange(-10,10);
  plot.set_style("points").plot_xy(xtab,ytab,"Wykres funkcji").plot_xy(ptx,pty,"Punkty");
  wait_for_key();
}

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

template<class FUNKCJA> double falsi(double left,double right,int iterations,double eps,FUNKCJA func){
  double leftV = func.policz(left),rightV = func.policz(right),center,centerV;
  short repeat=0;
  while((iterations-->0) || (eps < (fabs(leftV - rightV)/2 && eps >=0))){
    center = ( leftV*right-rightV*left )/( leftV -rightV );
    centerV = func.policz(center);

    if(centerV*rightV > 0){
      right = center;
      rightV = centerV;
      if(repeat==-1)
      leftV /= 2;
      repeat = -1;
    }
    else if(centerV * leftV > 0){
      left = center;
      leftV = centerV;
      if(repeat==1)
      rightV /= 2;
      repeat = 1;
    }
    else{
      break;
    }
  }
  func.narysuj(left,right);
  return center;
}

