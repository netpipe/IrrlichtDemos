#include <ImageMixer>

IMAGE_MIXER_START(512,512)

Image a,b,c,d,e,f;
Mask  A,B,C,D,E,F;
Channel Z,X,Y,K,W,J;

a.load("00.bmp");
X= a.getAverage();
A= X>0.5;

b.load("01.bmp");
Y= b.getAverage();
B= Y>0.5;

c.load("02.bmp");
K= c.getAverage();
C= K>0.5;

d.load("03.bmp");
W= d.getAverage();
D= W>0.5;

e.load("04.bmp");
J= e.getAverage();
E= J>0.5;

B= B && A;
C= C && !A;
D= D ^ !A;
E= E ^ !A;

Z=0;
Z.add(1,E);
Z.add(2,D);
Z.add(4,C);
Z.add(8,B);
Z.add(16,A);

Z/=32;

a.setRGB(Z,Z,Z);
a.save("final.bmp");


Z = 16*X + 8*Y + 4*K + 2*W + J;
Z /= 32;


a.setRGB(Z,Z,Z);
a.save("final2.bmp");


IMAGE_MIXER_END()



