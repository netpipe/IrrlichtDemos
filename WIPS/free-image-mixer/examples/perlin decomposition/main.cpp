#include <ImageMixer>

IMAGE_MIXER_START(512,512)

Image a;
Channel Z,R;
Mask m;

a.load("00.bmp");
Z=a.getAverage();
Z*=512;

R=0;
m= Z>=256;
Z.sub(256,m);
R.set(1,m);
a.setRGB(R,R,R);
a.save("01.bmp");

R=0;
m= Z>=128;
Z.sub(128,m);
R.set(1,m);
a.setRGB(R,R,R);
a.save("02.bmp");

R=0;
m= Z>=64;
Z=Z.sub(64,m);
R.set(1,m);
a.setRGB(R,R,R);
a.save("03.bmp");

R=0;
m= Z>=32;
Z.sub(32,m);
R.set(1,m);
a.setRGB(R,R,R);
a.save("04.bmp");

R=0;
m= Z>=16;
Z.sub(16,m);
R.set(1,m);
a.setRGB(R,R,R);
a.save("05.bmp");

R=0;
m= Z>=8;
Z.sub(8,m);
R.set(1,m);
a.setRGB(R,R,R);
a.save("06.bmp");

R=0;
m= Z>=4;
Z.sub(4,m);
R.set(1,m);
a.setRGB(R,R,R);
a.save("07.bmp");

R=0;
m= Z>=2;
Z.sub(2,m);
R.set(1,m);
a.setRGB(R,R,R);
a.save("08.bmp");


R=0;
m= Z>=1;
Z.sub(1,m);
R.set(1,m);
a.setRGB(R,R,R);
a.save("09.bmp");



IMAGE_MIXER_END()



