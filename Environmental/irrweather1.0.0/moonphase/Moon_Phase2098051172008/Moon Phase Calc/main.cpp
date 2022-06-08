
#include<iostream>
#include<ctime>
#include<string>
#include<sstream>
#include <stdlib.h>
using namespace std;
#include"moonstuff.h"

calendar c1,c2,c3,c4;//global variables to hold the calendar dates returned
bool extractDate(string &s,int &dd,int &mm, int &yy);//extracts date from a string
bool convertToInt(const string &s,int &n);//converts string to int
void dispDate();//displays the date calculated
void dispAbout();

int main()
{

	getCurMoonPhase(c1,c2,c3,c4);
	dispDate();
	int dd,mm,yy;
	string input;
	do
	{

		cout<<"enter a date in the format(dd:mm:yyyy or dd/mm/yyyy) or q to quit"<<endl;
		cin>>input;
		if(input=="q")
			break;

		if(extractDate(input,dd,mm,yy)==false)
			cout<<"Please enter valid date format\n";
		else
		{
			//todo
			getThisMoonPhase(dd,mm,yy,c1,c2,c3,c4);
			dispDate();
		}
	}while(1);
	dispAbout();

	exit(0);
	return 0;
}

bool extractDate(string &s,int &dd,int &mm, int &yy)
{
	if(s.length()!=10)
		return false;
	string tmp;
	tmp=s.substr(0,2);

	if(!convertToInt(tmp,dd))
		return false;
	tmp=s.substr(3,2);
	if(!convertToInt(tmp,mm))
		return false;
	tmp=s.substr(6,4);
	if(!convertToInt(tmp,yy))
		return false;
	return true;
}

bool convertToInt(const string &s,int &n)
{
	istringstream ss(s);
	if(ss>>n)
		return true;
	else
		return false;
}

void dispDate()
{
	system("cls");
	cout<<"\t\t\t   MOON PHASE CALCULATOR\n\n";
	cout<<"\t\t\t   \n\n\n";
	cout<<" PREVIOUS (dd:mm:yy hr:min:sec) \t\t   NEXT (dd:mm:yy hr:min:sec)\n\n";
	cout<<"--------------------------------------------------------------------------------\n\n";
	cout<<"New Moon  "<<c1.dd<<"/"<<c1.mm<<"/"<<c1.yy<<"  "<<c1.hr<<":"<<c1.min<<":"<<c1.sec<<"\t\t\t"
					   "New Moon  "<<c2.dd<<"/"<<c2.mm<<"/"<<c2.yy<<"  "<<c2.hr<<":"<<c2.min<<":"<<c2.sec<<"\n\n\n";
	cout<<"Full Moon  "<<c3.dd<<"/"<<c3.mm<<"/"<<c3.yy<<"  "<<c3.hr<<":"<<c3.min<<":"<<c3.sec<<"\t\t\t"
					   "Full Moon  "<<c4.dd<<"/"<<c4.mm<<"/"<<c4.yy<<"  "<<c4.hr<<":"<<c4.min<<":"<<c4.sec<<"\n\n\n";
//current day minus days till fullmoon or new moon to calculate phase and display picture

	cout<<"--------------------------------------------------------------------------------\n\n";
}



void dispAbout()
{
		system("cls");
		cout<<"\t\t\t\t   Moon phase calculator\n";
		cout<<"\t\t\t\t developed by Sourab Bose\n\n\n";
		cout<<"for any suggestions or comments\n";
		cout<<"contact me\n";
		cout<<"sourab_bose2002@yahoo.co.in\n\n";
		cout<<"Quiting...................\n\n";
	for (time_t t = time(0) + 5; time(0) < t; )
	{

	}


}

