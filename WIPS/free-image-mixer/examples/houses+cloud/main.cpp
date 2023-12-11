#include <ImageMixer>


IMAGE_MIXER_START(1200, 800);

Image X,Y;
ColorMatrixChannel M;
Channel L1,L2;
X.load("nebbia.jpg");
Y.load("sky01.jpg");

L1 = X.getLuminance();
M.setCrossRotation(acos(L1*L1)*30,asin(L1)*40,L1);
M.applyTo(X);

L2 = X.getLuminance();
M.setCrossRotation(acos(L1*L1)*30,asin(L1)*40,L1);
M.applyTo(Y);

X.clamp();
X.save("neb.bmp");

Y.clamp();
Y.save("sky.bmp");

IMAGE_MIXER_END();

