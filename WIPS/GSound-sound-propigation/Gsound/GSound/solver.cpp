#include<iostream>
using namespace std;
#include"MyMath.h"

int maine (){
	float x,y;
	MyMath::commonSolution(1,-1,3,1,1,5,x,y);
	double t = MyMath::calculateM(3,4,-2,1);
	cout << "x: " << x << "\n";
	cout << "y: " << y << "\n";	
	cout << "t: " << t << "\n";	

	return 0;
}
