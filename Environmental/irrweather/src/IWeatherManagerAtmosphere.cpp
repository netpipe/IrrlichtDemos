// Copyright (C) 2009-2010 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrWeatherManager" weather management library for the Irrlicht rendering engine.
// For conditions of distribution and use, see copyright notice in irrWeatherManager.h

#include "IWeatherManagerAtmosphere.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

//float round( float d )
//{
//   return floor( d + 0.5 );
//}

IWeatherManagerAtmosphere::IWeatherManagerAtmosphere(irr::IrrlichtDevice* const irrDevice,irr::scene::ISceneNode* const parent, irr::s32 id)
{
    //default values
    kin=0;
    ilgis=0;
    uvX=0.0f;
    i=0;
    rad=0.017453292519943295769236907684886f;//degree to radian (PI/180);
    /* prspalva[0]=0;//tamsi
    prspalva[1]=128;
    prspalva[2]=255;
    pbspalva[0]=128;//sviesi
    pbspalva[1]=255;
    pbspalva[2]=255;
    vspalva[0]=113;//vidurys
    vspalva[1]=184;
    vspalva[2]=255;*/
    J=DateToJulian(2010,3,26,16,50);//start time
    dayspeed=60.0f;
    time_int_step=0.0f;//start sun pos interpolation
    sun_interpolation_speed=30.0f;//make sun pos interpolation every 30 virtual min
    J1minute=1.0f/1440.0f;//one minute in Julian time

    // Start the sky
    skyid=id;
    device=irrDevice;
    driver=device->getVideoDriver();
    smgr=device->getSceneManager();
    setSkyImage("sky2.tga");
    CreateSkyPallete();
    startTimer();
    setAmbientLight2(SColor(255,255,255,255));//bug fix

    UpdateFog = false;
}

void IWeatherManagerAtmosphere::setDate(u32 year, u32 month, u32 day, u32 hour, u32 minute)
{
    J = DateToJulian(year,month,day,hour,minute);
}

//###rounds angle to fit 360 degrees
f32 IWeatherManagerAtmosphere::round360(f32 angle)
{
    if (angle>360)
    {
        while (angle>360)
        {
            angle-=360;
        }
    }
    return angle;
}

vector3df IWeatherManagerAtmosphere::getInterpolated3df(vector3df from,vector3df to, f32 d)
{
        f32 inv = 1.0f - d;
        vector3df rez;
        rez.X=from.X *inv + to.X*d;
        rez.Y=from.Y*inv + to.Y*d;
        rez.Z=from.Z*inv + to.Z*d;
        return  rez;
}

//prepare sun position interpolation (find start and end positions)
void IWeatherManagerAtmosphere::prep_interpolation(f64 Jdate, f64 time)//time-time from 1st sun pos to 2nd
{
        core::matrix4 mat;
        core::vector3df kampas;
        saule(52.0f,-5.0f,Jdate);//52.0 -5.0 kaunas 54.54 -23.54
        kampas.X=-sun_angle[1];//heigh
        kampas.Y=sun_angle[0];//0.0f;-
        kampas.Z=0.0f;
        mat.setRotationDegrees(kampas);
        f32 vieta[4];
        vieta[0]=0.0f;
        vieta[1]=0.0f;
        vieta[2]=1000.0f;
        vieta[3]=0.0f;
        mat.multiplyWith1x4Matrix(vieta);
        sun_pos_from.X=vieta[0];
        sun_pos_from.Y=vieta[1];
        sun_pos_from.Z=vieta[2];
        sun_angle_from=sun_angle[1];
        saule(52.0f,-5.0f,Jdate+time);//52.0 -5.0 kaunas 54.54 -23.54
        kampas.X=-sun_angle[1];//heigh
        kampas.Y=sun_angle[0];//0.0f;-
        kampas.Z=0.0f;
        core::matrix4 mat2;
        mat2.setRotationDegrees(kampas);
        vieta[0]=0.0f;
        vieta[1]=0.0f;
        vieta[2]=1000.0f;
        vieta[3]=0.0f;
        sun_angle_to=sun_angle[1];
        mat2.multiplyWith1x4Matrix(vieta);
        sun_pos_to.X=vieta[0];
        sun_pos_to.Y=vieta[1];
        sun_pos_to.Z=vieta[2];
}

//calculate sun position
void IWeatherManagerAtmosphere::saule(f64 pl,f64 lw,f64 J)
{
        //lw - longitude
        //pl - latitude
        //double J=2453097;
        f64 M = 357.5291f + 0.98560028*(J - 2451545);//degree
        M=round360(M);//degree
        f64 Mrad=M*rad;//radian
        f64 C = 1.9148f* sin(Mrad) + 0.02f* sin(2*Mrad) + 0.0003f*sin(3* Mrad);//degree
        //printf("C %3.4f\n",C);
        C=round360(C);//degree
        //f64 Crad=C*rad;//radian
        f64 lemda = M + 102.9372f + C + 180.0f;//degree
        lemda=round360(lemda);//degree
        f64 lemdarad=lemda*rad;//radian
        f64 alfa =lemda - 2.468f *sin(2* lemdarad) + 0.053f* sin(4* lemdarad)-0.0014f *sin(6 *lemdarad);//degree
        alfa=round360(alfa);//degree
        f64 sigma=22.8008f* sin(lemdarad) + 0.5999f* sin(lemdarad)*sin(lemdarad)*sin(lemdarad)
        + 0.0493f* sin(lemdarad)*sin(lemdarad)*sin(lemdarad)*sin(lemdarad)*sin(lemdarad);//degree
        sigma=round360(sigma);//degree
        f64 sigmarad=sigma*rad;//radian
        f64 zv=280.16f+360.9856235f*(J-2451545.0f)-lw;//degree
        zv=round360(zv);//degree
        f64 H = zv - alfa;//degree
        H=round360(H);//degree
        f64 Hrad=H*rad;//radian
        f64 A = atan2(sin(Hrad), cos(Hrad)* sin(pl*rad) - tan(sigmarad)*cos(pl*rad))/rad;
        f64 h = asin(sin(pl*rad)*sin(sigmarad) + cos(pl*rad)*cos(sigmarad)*cos(Hrad))/rad;
        //A from 0..180,-180..0
        //printf("M %3.4f C %3.4f lemda %3.4f alfa %3.4f sigma %3.4f\n",M,C,lemda,alfa,sigma);
        //printf("zv %3.4f H %3.4f A %3.4f h %3.15f\n",zv,H,A,h);
        sun_angle[0]=A;
        sun_angle[1]=h;//height
}


void IWeatherManagerAtmosphere::setSkyImage(const char *filename)
{
        skyimage=driver->createImageFromFile(filename);
}

void IWeatherManagerAtmosphere::CreateSkyPallete()
{
        //Psize-paletes dydis
        if (dangus!=NULL)
        {
            driver->removeTexture(dangus);
        }

        dangus=driver->getTexture("sky2.tga");

        //stars box
        ISceneNode* skybox = smgr->addSkyBoxSceneNode(
            driver->getTexture("stars_top.png"),
            driver->getTexture("stars_bottom.png"),
            driver->getTexture("stars.png"),
            driver->getTexture("stars.png"),
            driver->getTexture("stars.png"),
            driver->getTexture("stars.png"));

        skybox->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);

        Sky = new IAtmosphereSkySceneNode(dangus,smgr->getRootSceneNode(), smgr, 80, skyid);

        CloudCone = new ICloudLayerCone(0,smgr->getRootSceneNode(), smgr, 80, -1);
        CloudCone->setMaterialFlag(EMF_LIGHTING, true);
        CloudCone->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);

        //sun billboard
        bill=new ICompoundCelestialBodySceneNode(smgr->getRootSceneNode(),smgr,0, core::vector3df(0,0,0),core::dimension2d<f32>(150,150));
        bill->setMaterialFlag(video::EMF_LIGHTING, false);
        bill->setMaterialTexture(0, driver->getTexture("sun.tga"));
        bill->getMaterial(0).MaterialTypeParam = 0.01f;
        bill->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

        Sky->getMaterial(0).MaterialTypeParam = 0.01f;
        Sky->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);


        sunlight = smgr->addLightSceneNode(bill, core::vector3df(0,100,0),
        video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 10000.0f);

}

//###Starts ATMOsphere and prepares for work
/*void IWeatherManagerAtmosphere::start(IrrlichtDevice* const irrDevice,scene::ISceneNode* const parent, s32 id)
{
        skyid=id;
        device=irrDevice;
        driver=device->getVideoDriver();
        smgr=device->getSceneManager();
        setSkyImage("sky2.tga");
        CreateSkyPallete();
        startTimer();
        setAmbientLight2(SColor(255,255,255,255));//bug fix
}*/

//###starts timer
void IWeatherManagerAtmosphere::startTimer()
{
        // Atimer=new ITimer();
        Atimer=device->getTimer();
        // Atimer->start();
        //Atimer->setTime(0);
        currentTime=Atimer->getRealTime();
        startTime=Atimer->getRealTime();
        dTime=0.0f;
        J1=J;//force update sun first time
        ptime=Atimer->getRealTime();
        gtime=Atimer->getRealTime();
        JulianToDate(J);
}

//###Calculates delta time (time from last frame) for timer
void IWeatherManagerAtmosphere::updateTimer()
{
        currentTime =Atimer->getRealTime();
        dTime=currentTime-startTime;
}

//###returns sun rotation about y axis
f64 IWeatherManagerAtmosphere::getSunXAngle()
{
        return sun_angle[0]*rad;//angle in radians
}

//###returns sun rotation about x axis (sun height above horizont)
f64 IWeatherManagerAtmosphere::getSunYAngle()
{
        return sun_angle[1]*rad;// angle in radians
}

void IWeatherManagerAtmosphere::setDaysPerDay(f64 days)
{
        dayspeed=days;
}

f64 IWeatherManagerAtmosphere::getDayspeed()
{
        return dayspeed;
}

void IWeatherManagerAtmosphere::update()
{
        updateTimer();
        SColor sp;
        J=J+(((double)dayspeed/86400)/1000.0f)*dTime;

        //if interpolation is finished then start again
        if(time_int_step==0.0f)
        {
            //calculate sun interpolation positions
            prep_interpolation(J,sun_interpolation_speed*J1minute);
        JulianToDate(J);
        counter_time=0.0f;
        }//1440

        //printf("%8.4f %4.8f\n",J,time_int_step);
        //---move sun billboard to sun place
        counter_time+=J-J1;//1440
        time_int_step=counter_time/(sun_interpolation_speed*J1minute);//(1.0f/(sun_interpolation_speed*(1.0f/1440.0f)))*dTime;
        vector3df sun_place=getInterpolated3df(sun_pos_from,sun_pos_to, time_int_step);
        J1=J;
        ICameraSceneNode *cam=smgr->getActiveCamera();
        core::vector3df cameraPos = cam->getAbsolutePosition();
        core::vector3df vt = sun_place;
        bill->setPosition(vt);

        // sunlight->setPosition(vt);
        //---sun movement end
        f32 inv = 1.0f - time_int_step;
        uvX=((sun_angle_from *inv + sun_angle_to*time_int_step)+90.0f)/180;

        if(time_int_step>=1.0f)
            time_int_step=0.0f;

        sp=skyimage->getPixel((int)round(128*uvX),116); // Y = 123
        //driver->setAmbientLight(SColor(255,sp.getRed(),sp.getGreen(),sp.getBlue()));
        //printf("vt %3.4f",getSunYAngle());

        ClearColor = SColor(255,sp.getRed(),sp.getGreen(),sp.getBlue());

        if(UpdateFog)
        {
            SColor fogColor; E_FOG_TYPE fogType; f32 fogStart;
            f32 fogEnd; f32 fogDensity; bool pixelFog; bool rangeFog;

            driver->getFog(fogColor,fogType,fogStart,fogEnd,fogDensity,pixelFog,rangeFog);
            driver->setFog(ClearColor,fogType,fogStart,fogEnd,fogDensity,pixelFog,rangeFog);
        }

        if (getSunYAngle()<0.0042)
        {//isjungti lenpa kai naktis
            sunlight->setVisible(false);
            setAmbientLight2(SColor(255,sp.getRed(),sp.getGreen(),sp.getBlue()));
        }

        else
        {
            sunlight->setVisible(true);
            setAmbientLight2(SColor(255,sp.getRed(),sp.getGreen(),sp.getBlue()));//bug fix
        }
        // smgr->setShadowColor(SColor(50,sp.getRed(),sp.getGreen(),sp.getBlue()));
        //sunlight->getLightData().DiffuseColor=SColor(255,sp.getRed(),sp.getGreen(),sp.getBlue());
        Sky->setuvX(uvX);

        startTime = currentTime;
}

wchar_t IWeatherManagerAtmosphere::getTextDate()
{
        JulianToDate(J);
        return 1;
}

//###Converts normal date tu Julian calendar date
f64 IWeatherManagerAtmosphere::DateToJulian(u16 y,u16 m,u16 d,u16 h,u16 min)
{
        //http://www.phys.uu.nl/~strous/AA/en//reken/juliaansedag.html
        f64 hh=h*60+min;             //to minutes
        f64 dd=d+(hh/1440.0f);
        printf("dd= %8.8f %8.8f\n",dd,hh);
        if (m<3)
        {
            m=m+12;
            y=y-1;
        }
        f64 c=2-floor(float(y/100))+floor(float(y/400));
        f64 dt=floor(float(1461.0f*(y+4716.0f)/4))+floor(float(153*(m+1)/5))+dd+c-1524.5f;
        return dt;
}

    //###Converts Julian calendar date to normal calendar date
void IWeatherManagerAtmosphere::JulianToDate(f64 x)
{
        //http://www.phys.uu.nl/~strous/AA/en//reken/juliaansedag.html
        f64 p = floor(x + 0.5);
        f64 s1 = p + 68569;
        f64 n = floor(4*s1/146097);
        f64 s2 = s1 - floor((146097*n + 3)/4);
        f64 i = floor(4000*(s2 + 1)/1461001);
        f64 s3 = s2 - floor(1461*i/4) + 31;
        f64 q = floor(80*s3/2447);
        f64 e = s3 - floor(2447*q/80);
        f64 s4 = floor(q/11);
        f64 m = q + 2 - 12*s4;
        f64 y = 100*(n - 49) + i + s4;
        f64 d = e + x - p + 0.5;
        double rr;
        f64 h = ((modf(d,&rr)*1440)/60);
        d=floor(d);
        f64 min=floor(modf(h,&rr)*60);
        h=floor(h);
        printf("update time:%4.0f %2.0f %2.0f %2.0f %2.0f\n",y,m,d,h,min);
        Ndate[0]=(u16)y;
        Ndate[1]=(u16)m;
        Ndate[2]=(u16)d;
        Ndate[3]=(u16)h;
        Ndate[4]=(u16)min;
}

void IWeatherManagerAtmosphere::setAmbientLight2(video::SColor color)
{
        io::IFileSystem* files=device->getFileSystem();
        io::IAttributes* a = files->createEmptyAttributes();

        // get the current attributes
        scene::ISceneNode* self = smgr->getRootSceneNode();

        self->serializeAttributes(a);

        // set the color attribute
        a->setAttribute("AmbientLight", color);

        self->deserializeAttributes(a);

        // destroy attributes
        a->drop();
}

IWeatherManagerAtmosphere::~IWeatherManagerAtmosphere()
{
    if(bill)
        bill->remove();

    if(CloudCone)
        CloudCone->remove();

    if(Sky)
        Sky->remove();
}
