#include <ImageMixer>

//THIS IS A GENERAL PURPOSE INTERPOLATION CODE:
// YOU CAN TRY TO SEE WICH IS THE EFFECT ON DIFFERENT IMAGES.
// TRY TO CHANGE IT AND PLAY WITH THE CODE TO SEE WHAT'S HAPPENS! :)
IMAGE_MIXER_START(1024,768);

Image X,Y,Z,W;

X.load("texture01.jpg");
Y.load("texture02.jpg");
Z.load("texture03.jpg");
W = X;

Channel T,U,V;

T = X.getLuminance();
U = Y.getLightness();
V = Z.getAverage();

X.mixCubic(Y,Z,U,V);
X.mixCubic(Z,Y,V,V);

X.clamp();
X.save("mixage.bmp"); //first image to be saved

X = W;
X.mixQuadratic(Y,Z,U);
X.mixQuadratic(Y,V,T);
X.clamp();
X.save("mixage2.bmp"); //second immage to be save

IMAGE_MIXER_END();
