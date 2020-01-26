#ifndef __BALL_H__
#define __BALL_H__
#include <string>
#include "newton.h"
//#include "game.h"
#define NEWTON_GRAVITY -159.6f
using namespace std;

class Ball {
public:
    Ball();

    Ball(int ball_kind, vector3df loc,ISceneManager* smgr,IVideoDriver* driver,NewtonWorld* nWorld);
  //  int ball_kind();
   Ball& get();
     Ball& operator=(const Ball& b);
   void DropBall(NewtonWorld* nWorld);
   void PositionBall(vector3df loc);
  //   vector3df getRelativeCameraPosition(vector3df vPosition,double rotationToCamera);
   vector3df getRelativeCameraPosition(double rotationToCamera);
//	IAnimatedMesh *mesh;
//	ISceneNode *node;
	NewtonBody *body;
	NewtonCollision *collision;
   IAnimatedMeshSceneNode* node;
//    ISceneNode* node;
//	ISceneNode* node;
	//////////////////////////////////////////////////////////////////////////
	// Newton Callbacks
	static void _cdecl Ball::SetMeshTransformEvent(const NewtonBody* , const float*);
	static void _cdecl Ball::ApplyForceAndTorqueEvent (const NewtonBody* body);
private: 
   int ball_kind;
   ISceneManager* smgr;
   IVideoDriver* driver;
   NewtonWorld* nWorld;
    unsigned ball_type;
}; 
#endif

