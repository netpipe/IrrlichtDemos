#include "stdio.h"
//#include "tchar.h"
#include "iostream"
#include "math.h"
#include "irrlicht.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace io;
using namespace video;
using namespace scene;

namespace Noise
{

   inline double FindNoise(double x,double y);
   inline double Interpolate(double a,double b,double x);
   ITexture* Render_Clouds(bool colored,int w,int h,double zoom,int oct,double p,int r,int g,int b,IVideoDriver* driver);
   int Live(bool colored,int w,int h,double zoom, int octaves ,double p, int r, int g, int b,IVideoDriver* driver);
   double SmoothNoise(double x,double y);
   double InterpolateNoise(double x, double y);
   double DNoise(double x);

};

inline double Noise::DNoise(double x)
{
   int n = (int(x)<<13) ^ int(x);

   //return (double)( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & Ox7fffffff) / 1073741824.0);
   int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
   return 1.0-((double)nn/1073741824.0);
};


inline double Noise::FindNoise(double x,double y)
{
   int n=(int)x+(int)y*57;
   n=(n<<13)^n;
   int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
   return 1.0-((double)nn/1073741824.0);
};

inline double Noise::Interpolate(double a,double b,double x)
{
   double ft = x * 3.1415927;
   double f = (1.0-cos(ft)) * 0.5;
   return a * (1.0-f) + b * f;
};

double Noise::SmoothNoise(double x,double y)
{
   int smooth = 1;
   int cr = 16;
   int si = 4;
   int cn = 2;
   double corners = ( FindNoise(x-smooth, y-smooth)+FindNoise(x+smooth, y-smooth)+FindNoise(x-smooth, y+smooth)+FindNoise(x+smooth, y+smooth) ) / cr;
    double sides   = ( FindNoise(x-smooth, y) +FindNoise(x+smooth, y) +FindNoise(x, y-smooth) +FindNoise(x, y+smooth) ) / si;
    double center  =  FindNoise(x, y) / cn;

    return corners + sides + center;
};

double Noise::InterpolateNoise(double x, double y)
{
   double integer_X    = int(x);
    double fractional_X = x - integer_X;

    double integer_Y    = int(y);
    double fractional_Y = y - integer_Y;

    double v1 = SmoothNoise(integer_X,     integer_Y);
    double v2 = SmoothNoise(integer_X + 1, integer_Y);
    double v3 = SmoothNoise(integer_X,     integer_Y + 1);
    double v4 = SmoothNoise(integer_X + 1, integer_Y + 1);

    double i1 = Interpolate(v1 , v2 , fractional_X);
    double i2 = Interpolate(v3 , v4 , fractional_X);

    return Interpolate(i1 , i2 , fractional_Y);
};

int r, g, b;

ITexture* Noise::Render_Clouds(bool colored,int w,int h,double zoom, int octaves ,double p, int r, int g, int b,IVideoDriver* driver)
{

   double total = 0;//FindNoise(rand(),rand());
   ITexture* File = 0;
   bool water = false;

   IImage* NoiseMap =
      driver->createImage(ECF_R8G8B8,dimension2d<u32>(h,w));
   u8 * ff=(u8 *)NoiseMap->lock();

   for(int y=0;y<h;y++)
   {
      for(int x=0;x<w;x++)
      {
         total = 0;

         for(int a=0;a<octaves-1;a++)//This loops trough the octaves.
         {
            double frequency = pow(2.0,a);//This increases the frequency with every loop of the octave.
            double amplitude = pow(p,a);//This decreases the amplitude with every loop of the octave.

            total = total + InterpolateNoise(x * frequency / zoom, y / zoom * frequency) * amplitude;
         }

         if (colored == 1)
         {
            bool water = false;

            int height = (int)((total*128.0)+128.0);//Convert to 0-256 values.
            if (height<=0)
            {
               height=0;
               water = true;

               r = 0;
               g = 150;
               b = 195;
            }
            if (height>=50)
            {
               water = true;

               r = 0;
               g = 155;
               b = 200;
            }
            if (height>=145)
            {
               r = 232;
               g = 223;
               b = 230;
            }
            if (height>=150)
            {
               r = 114;
               g = 193;
               b = 101;
            }
            if (height>=200)
            {
               r = 201;
               g = 193;
               b = 101;
            }
            if (height>=250)
            {
               r = 217;
               g = 227;
               b = 237;
            }
            if (height>=255)
            {
               height=255;

               r = 227;
               g = 237;
               b = 247;
            }

            double random = DNoise(int(rand()));

            if (random >= .25)
            {
               //if (water == false)
               //{
                  int smooth = (int)((random)*2+total);

                  r = r + smooth;
                  g = g + smooth;
                  b = b + smooth;
               //}
            }

            if (random >= .50)
            {
               //if (water == false)
               //{
                  int smooth = (int)((random)*5+total);

                  r = r + smooth;
                  g = g + smooth;
                  b = b + smooth;
               //}
            }

            NoiseMap->
               setPixel(x,y,SColor(255,(int)r,(int)g,(int)b),false);
         }
         else
         {
            int height = (int)((total*128.0)+128.0);//Convert to 0-256 values.
            if(height>255)
               height=255;
            if(height<0)
               height=0;

            NoiseMap->
               setPixel(x,y,SColor(255,(int)height,(int)height,(int)height),false);
         }
      }
   }

   if (NoiseMap)
   {
      cout << "Image created.\n";
   }
   else
   {
      cout << "Image failed.\n";
   }

   bool i =
      driver->writeImageToFile(NoiseMap,"NoiseMaps/noise.bmp",0);
   //NoiseMap->drop();
   //Tex->drop();

   if (i)
   {
      cout << "Image exported.\n";
      ITexture* File =
         driver->getTexture("NoiseMaps/noise.bmp");
   }
   else
   {
      cout << "Image failed to export.\n";
   }

   return File;
};
