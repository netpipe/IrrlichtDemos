#include "moonstuff.h"

calendar JDtoCalendar(double jd)
{
	int z;
	double f;
	int alp,a,b,c,d,e;
	double dd;
	int mm,yy;
	double tmpH,tmpM,tmpS;
	calendar cal;


	z=int(jd+0.5);
	f=(jd+0.5)-z;
	if(z<2299161)
	{
		a=z;
	}
	else
	{
		alp=int((z-1867216.25)/36524.25);
		a=z+1+alp-int(alp/4);
	}
	b=a+1524;
	c=int((b-122.1)/365.25);
	d=int(c*365.25);
	e=int((b-d)/30.6001);

	if(e<14)
		mm=e-1;
	else
		mm=e-13;


	if(mm>2)
		yy=c-4716;
	else
		yy=c-4715;

	dd=b-d-int(30.6001*e)+f;

	cal.yy=yy;
	cal.mm=mm;
	cal.dd=int(dd);
	tmpH=(dd-cal.dd)*24;
	cal.hr=int(tmpH);
	tmpM=(tmpH-cal.hr)*60;
	cal.min=int(tmpM);
	tmpS=(tmpM-cal.min)*60;
	cal.sec=int(tmpS);

	return cal;
}

double calendarToJD(calendar cal)
{	
	double JDE;
	int a,b;
	double d=(cal.dd+cal.hr/24);
	
	if(cal.mm<=2)
	{
		cal.yy--;
		cal.mm+=12;
	}
	a=cal.yy/10;
	b=2-a+(a/4);
	
	JDE=(365.25*(cal.yy+4716))+(30.6001*(cal.mm+1))+d+b-1524.5;
	return JDE;
}

void calcNewMoonPhase(const calendar cur, calendar &prev,calendar &next)
{
	double k,t,e;
	double m,md,om,f;
	double JDE,appJDE;
	calendar cal;
	

	if(cur.dd<=6)
		k=floor((cur.yy+((double)(cur.mm-1)/12)-2000)*12.3685);
	else
		k=floor((cur.yy+((double)(cur.mm)/12)-2000)*12.3685);
	
	for(int i=0;i<2;i++)
	{
		t=k/1236.85;
		e=1-(0.002516*t)-(0.0000074*pow(t,2));
		JDE=2451550.09765 + (29.530588853 * k) + (0.0001337 * pow(t,2)) - (0.000000150 * pow(t,3)) + (0.00000000073 * pow(t,4));
		m=2.5534 + (29.10535669 * k) - (0.0000218 * pow(t,2)) - (0.00000011 * pow(t,3));
		m=fmod(m,360);
		if(m<0)
			m+=360;
		else
			m-=360;

		md=201.5643 + (385.81693528 * k) +(0.0107438 * pow(t,2)) + (0.00001239 * pow(t,3)) - (0.000000058 * pow(t,4));
		md=fmod(md,360);
		if(md<0)
			md+=360;
		else
			md-=360;

		f= 160.7108 + (390.67050274 * k)- (0.0016341 * pow(t,2)) - (0.00000227 * pow(t,3)) + (0.000000011 * pow(t,4));
		f=fmod(f,360);
		if(f<0)
			f+=360;
		else
			f-=360;

		om = 124.7746 - (1.56375580 * k) + (0.0020691 * pow(t,2)) + (0.00000215 * pow(t,3));
		om=fmod(om,360);
		if(om<0)
			om+=360;
		else if(om>360)
			om-=360;
				
		m=m*(3.14159/180);
		md=md*(3.14159/180);
		om=om*(3.14159/180);
		f=f*(3.14159/180);

		appJDE=JDE-(0.40720*sin(md))+(.17241*e*sin(m))+(.01608*sin(2*md))+(.01039*sin(2*f))+(.00739*e*sin(md-m))
					-(.00514*e*sin(md+m))+(.00208*pow(e,2)*sin(2*m))-(.00111*sin(md-2*f))-(.00057*sin(md+2*f))
					+(.00056*e*sin(2*md+m))-(.00042*sin(3*m))+(.00042*e*sin(m+2*f))+(.00038*e*sin(m-2*f))
					-(.00024*e*sin(2*md-m))-(.00017*om)-(.00007*sin(md+2*m))+(.00004*sin(2*md-2*f))+(.00004*sin(3*m))
					+(.00003*sin(md+m-2*f))+(.00003*sin(2*md+2*f))-(.00003*sin(md+m+2*f))+(.00003*sin(md-m+2*f))
					-(.00002*sin(md-m-2*f))-(.00002*sin(3*md+m))+(.00002*sin(4*md));

		
		cal=JDtoCalendar(appJDE);
		
		
		if(calendarToJD(cur)-calendarToJD(cal)>0)
		{
			prev=cal;
			k++;
		}
		else
		{
			next=cal;
			k--;
		}
			

	}//end of for loop
	
}//end of function

void calcFullMoonPhase(calendar cur,calendar &prev,calendar &next)
{
	double k,t,e;
	double m,md,om,f;
	double JDE,appJDE;
	calendar cal;
	if(cur.dd<=21)
	k=int((cur.yy+((double)(cur.mm-1)/12)-2000)*12.3685)+0.5;
	else
	k=int((cur.yy+((double)(cur.mm)/12)-2000)*12.3685)+0.5;
	
	for(int i=0;i<2;i++)
	{
	t=k/1236.85;
		e=1-(0.002516*t)-(0.0000074*pow(t,2));
		JDE=2451550.09765 + (29.530588853 * k) + (0.0001337 * pow(t,2)) - (0.000000150 * pow(t,3)) + (0.00000000073 * pow(t,4));
		m=2.5534 + (29.10535669 * k) - (0.0000218 * pow(t,2)) - (0.00000011 * pow(t,3));
		m=fmod(m,360);
		if(m<0)
			m+=360;
		else
			m-=360;

		md=201.5643 + (385.81693528 * k) +(0.0107438 * pow(t,2)) + (0.00001239 * pow(t,3)) - (0.000000058 * pow(t,4));
		md=fmod(md,360);
		if(md<0)
			md+=360;
		else
			md-=360;

		f= 160.7108 + (390.67050274 * k)- (0.0016341 * pow(t,2)) - (0.00000227 * pow(t,3)) + (0.000000011 * pow(t,4));
		f=fmod(f,360);
		if(f<0)
			f+=360;
		else
			f-=360;

		om = 124.7746 - (1.56375580 * k) + (0.0020691 * pow(t,2)) + (0.00000215 * pow(t,3));
		om=fmod(om,360);
		if(om<0)
			om+=360;
		else if(om>360)
			om-=360;
				
		m=m*(3.14159/180);
		md=md*(3.14159/180);
		om=om*(3.14159/180);
		f=f*(3.14159/180);

		appJDE=JDE-0.40614*sin(md) +
          0.17302*e*sin(m) +
          0.01614*sin(2*md) +
          0.01043*sin(2*f) +
          0.00734*e*sin(md - m) +
          -0.00514*e*sin(md + m) +
          0.00209*pow(e,2)*sin(2*m) +
          -0.00111*sin(md - 2*f) +
          -0.00057*sin(md + 2*f) +
          0.00056*e*sin(2*md + m) +
          -0.00042*sin(3*md) +
          0.00042*e*sin(m + 2*f) +
          0.00038*e*sin(m - 2*f) +
          -0.00024*e*sin(2*md - m) +
          -0.00017*sin(om) +
          -0.00007*sin(md + 2*m) +
          0.00004*sin(2*md - 2*f) +
          0.00004*sin(3*m) +
          0.00003*sin(md + m - 2*f) +
          0.00003*sin(2*md + 2*f) +
          -0.00003*sin(md + m + 2*f) +
          0.00003*sin(md - m + 2*f) +
          -0.00002*sin(md - m - 2*f) +
          -0.00002*sin(3*md + m) +
          0.00002*sin(4*md);

		cal=JDtoCalendar(appJDE);
		

		if(calendarToJD(cur)-calendarToJD(cal)>0)
		{
			prev=cal;
			k++;
		}
		else
		{
			next=cal;
			k--;
		}
	}
}


void getCurMoonPhase(calendar &prevN,calendar &nextN ,calendar &prevF, calendar &nextF)
{
	time_t t=0;
	tm* tm=NULL;
	
	time(&t);
	tm=localtime(&t);
	calendar cal={(tm->tm_year+1900),(tm->tm_mon+1),tm->tm_mday,(tm->tm_hour+1),0,0};
	calcNewMoonPhase(cal,prevN,nextN);
	calcFullMoonPhase(cal,prevF,nextF);
}

void getThisMoonPhase(int dd,int mm,int yy,calendar &prevN,calendar &nextN ,calendar &prevF, calendar &nextF)
{
	calendar cal={yy,mm,dd,0,0,0};
	calcNewMoonPhase(cal,prevN,nextN);
	calcFullMoonPhase(cal,prevF,nextF);
}