#include <iostream>
#include <cmath>
#include "gnuplot_i.hpp" // Biblioteka do sciagniecia na https://code.google.com/archive/p/gnuplot-cpp/
						 // Kompilujemy z warunkiem "-I."
#include <limits>
#include <vector>
#include <conio.h>

//
// Program korzysta z programu GNUPlot 4.6 
//

using namespace std;
long double wielomian(long double);
long double trygonometryczna(long double);
long double zlozenie(long double);
void wait_for_key();
long double falsi(long double left, long double right, int iterations, long double (*calculate)(long double), long double epsilon);
long double bisekcja(long double left, long double right, int iterations, long double (*calculate)(long double), long double epsilon);
struct Wielomian;
struct Trygonometryczna;

// ########################
// ######### MAIN #########
// ########################
int main(){
	long double (*calculate)(long double) = wielomian;
	int iterations;
	cin>>iterations;
	long double epsilon = -1;
	cout << bisekcja(-10, 0, iterations, calculate, epsilon) << endl;
	cout << falsi(-10, 0, iterations, calculate, epsilon) << endl << endl;
}

// ###########################
// ######### FUNKCJE #########
// ###########################
long double zlozenie(long double x){
	return 3*pow(x, 2)+sin(5*x+M_PI)+10;
}

void wait_for_key(){
#if defined(_WIN32) || defined(WIN32) || defined(__TOS_WIN__) || defined(__WIN32__)  // every keypress registered, also arrow keys
    cout << endl << "Press any key to continue..." << endl;
    //FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();
#elif defined(__unix) || defined(unix) || defined(__APPLE__) || defined(__UNIX__)
    cout << endl << "Press ENTER to continue..." << endl;
    cin.clear();
	cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    cin.get();
#endif
    return;
}

long double wielomian(long double x){
	return -3*pow(x, 3)+33*x-8*pow(x,2)-20;
	//return x*x+10*x;
}

long double trygonometryczna(long double x){
	return sin(5*x+M_PI)+3;
}
/*
struct Wioelomian{
	int stopien;
	Wielomian(int st=2){
		stopien = st;
		double *wartosci = new double[st+1];
	}
	~Wielomian(){delete[] wartosci;}

	void wprowadz(){
		cout<<"Podaj wartosci:\n";
		for(int i=0;i<=stopien;i++){
			cout<<"Przy x^"<<i<<":";
			cin>>wartosci[i];
		}
	}

	double policz(double x){
		double wynik = 0;
		for(int i=0;i<=stopien;i++){
			wynik += pow(x,i)*wartosci[i];
		}
		return wynik;
	}

};

struct Trygonometryczna{
	double 
};
*/
long double falsi(long double left, long double right, int iterations, long double (*calculate)(long double), long double epsilon){
	
	long double leftV = calculate(left), rightV=calculate(right), center, centerV;
	short repeat=0;
	
	while(iterations-->0 || (epsilon < (leftV-rightV)/2 && epsilon >=0)){
		
		center = ( leftV*right-rightV*left )/( leftV -rightV );
		centerV = calculate(center);
		
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
	Gnuplot plot;
	vector<long double> x;
	vector<long double> y;
	x.push_back(left);
	x.push_back(right);
	y.push_back(leftV);
	y.push_back(rightV);
	plot.set_grid();
	if(x[0]==x[1])plot.set_xrange(-5.08679, -5.08677);
	plot.set_style("linespoints").plot_xy(x,y,"Wynik").set_style("lines").plot_equation("-3*x*x*x-8*x*x+33*x-20").set_style("impulses").plot_xy(x,y,"user-defined doubles").set_style("lines").plot_equation("0", "OX");
	wait_for_key();
	return center;
}

long double bisekcja(long double left, long double right, int iterations, long double (*calculate)(long double), long double epsilon){
	long double temp, center;
	bool signL = calculate(left)>0?1:0;
	while(iterations-->0 || (epsilon < (calculate(left)-calculate(right))/2 && epsilon >= 0)){
		center = (right+left)/2;
		temp = calculate(center);
		if(temp == 0)
			return center;
		else if(temp<0 ^ signL)
			left = center;
		else
			right = center;
	}
	return center;
}
