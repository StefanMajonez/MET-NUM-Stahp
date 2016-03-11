#include <iostream>
#include <cmath>
#include "gnuplot_i.hpp" // Biblioteka do sciagniecia na https://code.google.com/archive/p/gnuplot-cpp/
// Kompilujemy z warunkiem "-I."
#include <limits>
#include <vector>
#if defined(_WIN32) || defined(WIN32) || defined(__TOS_WIN__) || defined(__WIN32__)
#include <conio.h>
#endif
#include <string>

using namespace std;



long double wielomian(long double x){
	return -3*x*x*x+33*x-8*x*x-20;
	//return x*x+10*x;
}

long double trygonometryczna(long double x){
	return sin(x);
}

long double zlozenie(long double x){
	return 3*pow(x, 2)+sin(5*x+7)-1000;
}



long double falsi(long double left, long double right, int &iterations, long double (*calculate)(long double), long double epsilon){
	
	long double leftV = calculate(left), rightV=calculate(right), center, centerV;
	short repeat=0;
	int i = 0;
	while(i++<iterations || (epsilon < fabs(leftV-rightV)/2 && epsilon>=0) ){
		
		center = ( leftV*right-rightV*left )/( leftV -rightV );
		centerV = calculate(center);
		
		if(centerV*rightV > 0){
			right = center;
			rightV = centerV;
			if(repeat==-1)		// this is improvement to falsi method that make it work even faster
			leftV /= 2;
			repeat = -1;
		}
		else if(centerV * leftV > 0){
			left = center;
			leftV = centerV;
			if(repeat==1)		// same here
			rightV /= 2;
			repeat = 1;
		}
		else{			// would break as next itterations won't do anything
			break;
		}
	}
	if(iterations == 0)
	iterations = i;
	return center;
}


long double bisekcja(long double left, long double right, int &iterations, long double (*calculate)(long double), long double epsilon){
	long double temp, center;
	bool signL = calculate(left)>0?1:0;
	int i = 0;
	while(i++<iterations || ( (epsilon < fabs(calculate(left)-calculate(right))/2) && epsilon >=0) ){
		center = (right+left)/2;
		temp = calculate(center);
		if(temp == 0)
			return center;
		else if(temp<0 ^ signL)
			left = center;
		else
			right = center;
		if(left == right)
		break;
	}
	if(iterations == 0)
	iterations = i;
	return center;
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

int main(){
	long double (*calculate)(long double);
	char temp= 0;
	int iterations = 0;
	long double epsilon = -1;
	long double left = 0, right = 0;
	long double tempX;
	vector<long double> bisectionX, falsiX, zero(1,0);
	string function;
	
	while(temp != '1' && temp != '2' && temp != '3'){
		cout << "choose equation: \n  1) -3*x^3+33*x-8*x^2-20 \n  2) sin(x) \n  3) 3*x^2+sin(5*x+7)-1000 \n";
		cin >> temp;
	}
	
	if(temp == '1'){
		calculate = wielomian;
		function = "-3*x*x*x+33*x-8*x*x-20";
	}
	else if(temp == '2'){
		calculate = trygonometryczna;
		function = "sin(x)";
	}
	else{
		calculate = zlozenie;
		function = "3*x*x+sin(5*x+7)-1000";
	}
	
	temp = 0;
	
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
	
	Gnuplot plot;
	plot.set_style("lines").plot_equation(function).plot_equation("0", "OX");
	wait_for_key();
	do{
		cout << "input left and right points \n";
		cin >> left >> right;
	}
	while(calculate(left)*calculate(right)>=0);
	
	
	tempX = bisekcja(left, right, iterations, calculate, epsilon);
	cout << "bisection: " << tempX << endl;
	bisectionX.push_back(tempX);
	if(epsilon!=-1){
		cout << "iterations: " << iterations << endl;
		iterations = 0;
	}
	else
		cout << "epsilon: " << calculate(tempX) << endl;
	
	
	tempX = falsi(left, right, iterations, calculate, epsilon);
	cout << "falsi: " << tempX << endl;
	falsiX.push_back(tempX);
	if(epsilon!=-1){
		cout << "iterations: " << iterations << endl;
		iterations = 0;
	}
	else
		cout << "epsilon: " << calculate(tempX) << endl;
	
	
	plot.set_style("lines").plot_equation(function).plot_equation("0", "OX").set_style("points").plot_xy(bisectionX, zero, "Bisection").plot_xy(falsiX, zero, "falsi");
	wait_for_key();
}
