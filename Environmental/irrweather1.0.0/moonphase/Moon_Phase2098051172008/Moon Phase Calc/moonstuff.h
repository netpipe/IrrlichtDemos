#ifndef CALENDAR_H
#define CALENDAR_H

#include<ctime>
#include<cmath>

using namespace std;

struct calendar 
{
	int yy;
	int mm;
	int dd;
	int hr;
	int min;
	int sec;
};

calendar JDtoCalendar(double jd); //converts julian date to calendar date 
double calendarToJD(calendar cal);//converts calendar to julian date
void calcNewMoonPhase(const calendar cur, calendar &prev,calendar &next);
void calcFullMoonPhase(calendar cur,calendar &prev,calendar &next);
void getCurMoonPhase(calendar &prevN,calendar &nextN ,calendar &prevF, calendar &nextF);
void getThisMoonPhase(int dd,int mm,int yy,calendar &prevN,calendar &nextN ,calendar &prevF, calendar &nextF);

#endif