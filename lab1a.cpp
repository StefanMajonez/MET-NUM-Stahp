#include <iostream>
#include <cmath>
#include "gnuplot_i.hpp" // Biblioteka do sciagniecia na https://code.google.com/archive/p/gnuplot-cpp/
// Kompilujemy z warunkiem "-I."
#include <limits>
#include <vector>
#if defined(_WIN32) || defined(WIN32) || defined(__TOS_WIN__) || defined(__WIN32__)
#include <conio.h>
#endif
#include "structs.hpp"

using namespace std;
//
// Program korzysta z programu GNUPlot 4.6 
//



template<class FUNKCJA> double falsi(double left,double right,int iterations,double eps, FUNKCJA *func);





int main(){
  long double left, right, epsilon=-1;
  Wielomian funk;
  Gnuplot plot;
  vector<double> falsiX, biectionX, zero(1, 0);
  int iterations=0;
  char temp;
  
  plot.set_grid();
  funk.wczytaj();
  funk.narysuj();
  
  do{
    cout << "input left and right points \n";
    cin >> left >> right;
  }
  while(funk.policz(left)*funk.policz(right)>=0);
  
  while(temp != '1' && temp != '2'){
    cout << "calculating by: \n  1) number of itterations \n  2) to accuracy \n";
    cin >> temp;
  }
  if(temp=='1'){
    cout << "iteratins: ";
    cin >> iterations;
  }
  else{
    cout << "epsilon: ";
    cin >> epsilon;
  }
  
  falsiX.push_back( falsi(left, right, iterations, epsilon, &funk) );
  plot.set_grid();
  plot.set_style("points").plot_xy(falsiX, zero, "falsi").set_style("lines").plot_equation("0", "OX").plot_equation(funk.function.str());
  wait_for_key();
  
}

template<class FUNKCJA> double falsi(double left,double right,int iterations,double eps, FUNKCJA *func){
  double leftV = (*func).policz(left),rightV = (*func).policz(right),center,centerV;
  short repeat=0;
  while((iterations-->0) || (eps < (fabs(leftV - rightV)/2 && eps >=0))){
    center = ( leftV*right-rightV*left )/( leftV -rightV );
    centerV = (*func).policz(center);

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
      return center;
    }
  }
  return center;
}
