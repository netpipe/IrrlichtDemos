/*
Coded by Ahmet YILDIRIM
mclightning & whizgeek .com
Release Date: 02.06.11
*/
#include <irrlicht.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <iostream>


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class irFinder
{
	private:
		CvCapture* capture;
		IplImage* img;
		IplImage* im;
		CvMemStorage* storage;
		CvSeq* c;
		const char* pencere;
		bool gosterPencere;
		int tre;
		int max;
		CvPoint temp;
	public:
		irFinder(const char*,bool,int,int);
		~irFinder();
		CvPoint yenile();
		void ayar(int treshold,int max);
		void kapat();
};
irFinder::irFinder(const char* pencere,bool gosterPencere,int tre,int max)
{
	this->capture = cvCreateCameraCapture(0);
	this->storage = cvCreateMemStorage();	
	this->c=NULL;
	this->pencere=pencere;
	this->gosterPencere=gosterPencere;
	this->tre=tre;
	this->max=max;
	if(this->gosterPencere)
	{
		cvNamedWindow( pencere, CV_WINDOW_AUTOSIZE );
	}
}
irFinder::~irFinder()
{
	cvReleaseCapture( &(this->capture) );
	cvReleaseImage( &this->img );
	if(this->gosterPencere)
	{
	cvDestroyWindow( this->pencere );
	}
}
void irFinder::kapat()
{
	cvReleaseCapture( &(this->capture) );
	cvReleaseImage( &this->img );
	if(this->gosterPencere)
	{
	cvDestroyWindow( this->pencere );
	}
}
void irFinder::ayar(int threshold,int max)
{
	this->tre=threshold;
	this->max=max;
}
CvPoint irFinder::yenile()
{
	int Nc=0;
	this->img = cvQueryFrame(this->capture);
	this->im= cvCreateImage( cvGetSize(this->img),IPL_DEPTH_8U,1);
	cvThreshold(this->img,this->img,this->tre,this->max,CV_THRESH_BINARY);
	cvCvtColor(this->img,this->im,CV_BGR2GRAY);
	Nc = cvFindContours(this->im,this->storage,&this->c,sizeof(CvContour),CV_RETR_LIST );	
	CvPoint nokta;
	if(Nc>0) 
	{	
		CvPoint* k=CV_GET_SEQ_ELEM(CvPoint,this->c,0);
		nokta.x=k->x;
		nokta.y=k->y;
		this->temp=nokta;
	}
	else
	{
		nokta=this->temp;
	}
	if(this->gosterPencere)
	{
		cvCircle(this->img,nokta,4,cvScalar(255,255,255));
		cvShowImage(this->pencere,this->img);
	}
	return nokta;
}