#include<iostream>
using namespace std;


#include <thread>
#include <cstdio>
#include <ctime>

#include "Source.h"
#include "Ray.h"
#include "MyMath.h"
#include "Scene.h"


//#include "Sound.h"
void task1 ( ){
	//Sound e = Sound("acoustics.wav");
	//e.Play();
}
void main (){
	//SoundFile soundfile();
	// //Source source();
	//Ray* raytest = new Ray(new Position(1,1,0), new Position(2,2,0), 2, 4); 
	////Position* t = raytest->Chk_Intersect(new Wall(new Position(5,1,0), new Position(5,4,0)) );
	//Wall* wall1 = new Wall(new Position(5,-4,0), new Position(5,4,0));
	//Wall* wall2 = new Wall(new Position(-5,-4,0), new Position(-5,4,0));
	//Wall* wall3 = new Wall(new Position(-5,4,0), new Position(5,4,0));
	//Wall* wall4 = new Wall(new Position(-5,-4,0), new Position(5,-4,0));
	//Scene* scene = new Scene(wall1,wall2,wall3,wall4);
	//raytest->Propagate(scene,1);
	//Sel_Intersect* t = raytest->Intersect(scene);
	//cout<< t->position->x <<" "<< t->position->y<<endl ;
	//WaveFile Toto("loudness.wav"); 
	//Toto.read("loudness.wav");
//	PlaySound(TEXT("loudness.wav"), NULL, SND_ASYNC);
	thread t1(task1);
	thread t2(task1);
	t2.join();
	std::clock_t start;
	double duration;
	int i = 0 ;	
	start = clock();

	/* Your algorithm here */
	while(true){
		duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
		if(duration==0.6){
			t1.join();
			break;
		}
	}

}