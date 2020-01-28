// Author : Cobra171
// 15 min whip together - initial test
#include <irrlicht.h>
#include <Box2D/Box2D.h>
#include <Box2D/Common/b2Math.h>
#include <GL/gl.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

   IVideoDriver* driver ;
   ISceneManager* smgr ;
   IGUIEnvironment* guienv ;
   ITimer* timer;
   f32 DeltaTime;
   f32 TimeStamp;
   float32 timeStep;
      int32 velocityIterations;
   int32 positionIterations;


// draw2DImage source from Lonesome Ducky
void draw2DImage(irr::video::IVideoDriver *driver, irr::video::ITexture* texture,
    irr::core::rect<irr::s32> sourceRect, irr::core::position2d<irr::s32> position,
    irr::core::position2d<irr::s32> rotationPoint, irr::f32 rotation, irr::core::vector2df scale,
    bool useAlphaChannel, irr::video::SColor color)
{

   irr::video::SMaterial material;

   // Store and clear the projection matrix
   irr::core::matrix4 oldProjMat = driver->getTransform(irr::video::ETS_PROJECTION);
   driver->setTransform(irr::video::ETS_PROJECTION,irr::core::matrix4());

   // Store and clear the view matrix
   irr::core::matrix4 oldViewMat = driver->getTransform(irr::video::ETS_VIEW);
   driver->setTransform(irr::video::ETS_VIEW,irr::core::matrix4());

   // Find the positions of corners
   irr::core::vector2df corner[4];

   corner[0] = irr::core::vector2df(position.X,position.Y);
   corner[1] = irr::core::vector2df(position.X+sourceRect.getWidth()*scale.X,position.Y);
   corner[2] = irr::core::vector2df(position.X,position.Y+sourceRect.getHeight()*scale.Y);
   corner[3] = irr::core::vector2df(position.X+sourceRect.getWidth()*scale.X,position.Y+sourceRect.getHeight()*scale.Y);

   // Rotate corners
   if (rotation != 0.0f)
      for (int x = 0; x < 4; x++)
         corner[x].rotateBy(rotation,irr::core::vector2df(rotationPoint.X, rotationPoint.Y));


   // Find the uv coordinates of the sourceRect
   irr::core::vector2df uvCorner[4];
   uvCorner[0] = irr::core::vector2df(sourceRect.UpperLeftCorner.X,sourceRect.UpperLeftCorner.Y);
   uvCorner[1] = irr::core::vector2df(sourceRect.LowerRightCorner.X,sourceRect.UpperLeftCorner.Y);
   uvCorner[2] = irr::core::vector2df(sourceRect.UpperLeftCorner.X,sourceRect.LowerRightCorner.Y);
   uvCorner[3] = irr::core::vector2df(sourceRect.LowerRightCorner.X,sourceRect.LowerRightCorner.Y);
   for (int x = 0; x < 4; x++) {
      float uvX = uvCorner[x].X/(float)texture->getSize().Width;
      float uvY = uvCorner[x].Y/(float)texture->getSize().Height;
      uvCorner[x] = irr::core::vector2df(uvX,uvY);
   }

   // Vertices for the image
   irr::video::S3DVertex vertices[4];
   irr::u16 indices[6] = { 0, 1, 2, 3 ,2 ,1 };

   // Convert pixels to world coordinates
   float screenWidth = driver->getScreenSize().Width;
   float screenHeight = driver->getScreenSize().Height;
   for (int x = 0; x < 4; x++) {
      float screenPosX = ((corner[x].X/screenWidth)-0.5f)*2.0f;
      float screenPosY = ((corner[x].Y/screenHeight)-0.5f)*-2.0f;
      vertices[x].Pos = irr::core::vector3df(screenPosX,screenPosY,1);
      vertices[x].TCoords = uvCorner[x];
      vertices[x].Color = color;
   }
   material.Lighting = false;
   material.ZWriteEnable = false;
   material.TextureLayer[0].Texture = texture;
   //material.
   //material.TextureLayer[0].TextureWrapU = irr::video::ETC_CLAMP;
   //material.TextureLayer[0].TextureWrapV = irr::video::ETC_CLAMP;

   if (useAlphaChannel)
      material.MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;
   else
      material.MaterialType = irr::video::EMT_SOLID;

   driver->setMaterial(material);
   driver->drawIndexedTriangleList(&vertices[0],4,&indices[0],2);


   // Restore projection and view matrices
   driver->setTransform(irr::video::ETS_PROJECTION,oldProjMat);
   driver->setTransform(irr::video::ETS_VIEW,oldViewMat);

}


class bwBody
{
    public:
        bwBody(const b2PolygonShape& bShape, b2Body* const bBody, IrrlichtDevice* const bDevice)
        {
            shape = bShape;
            body = bBody;
            device = bDevice;
        }

        void update()
        {
            b2Vec2 position = body->GetPosition();
            float32 angle = body->GetAngle();

            draw2DImage(device->getVideoDriver(), device->getVideoDriver()->getTexture("./media/crate.png"),
                rect<irr::s32>(0,0,120,120), position2d<s32>(position.x-10,position.y-10),
                position2d<s32>(position.x,position.y), angle*RADTODEG, vector2df(0.18,0.18),
                false, SColor(255,255,255,255));

            device->getVideoDriver()->draw2DLine(position2d<s32>(position.x,position.y),
                position2d<s32>(position.x-10,position.y-10), SColor(255, 0, 255, 0));

            ///const b2Mat22 mat(angle);
            for(int i=0; i < shape.GetVertexCount(); i++)
            {
                const b2Vec2 vec = body->GetWorldPoint(shape.GetVertex(i));
                ///const b2Vec2 vec = body->GetWorldPoint(b2Mul(mat,shape.GetVertex(i)));


                // This wireframe drawing is incorrect - Cobra
                /*device->getVideoDriver()->draw2DLine(position2d<s32>(vec.x,vec.y),
                    (i+1 != shape.GetVertexCount()) ?
                    position2d<s32>(body->GetWorldPoint(b2Mul(mat,shape.GetVertex(i+1))).x, body->GetWorldPoint(b2Mul(mat,shape.GetVertex(i+1))).y):
                    position2d<s32>(body->GetWorldPoint(b2Mul(mat,shape.GetVertex(0))).x,body->GetWorldPoint(b2Mul(mat,shape.GetVertex(0))).y),
                    SColor(255, 255, 255, 255));*/

                /*device->getVideoDriver()->draw2DVertexPrimitiveList(const void *vertices, u32 vertexCount,
                    const void *indexList, u32 primCount,
                    E_VERTEX_TYPE vType=EVT_STANDARD, scene::E_PRIMITIVE_TYPE pType=scene::EPT_TRIANGLES, E_INDEX_TYPE iType=EIT_16BIT)*/
            }
        }

    private:
        b2PolygonShape shape;
        b2Body* body;
        IrrlichtDevice* device;
};

irr::core::array<bwBody*> bodies;


void createRigidBox(b2World& world, const vector2d<s32>& pos, IrrlichtDevice* const device)
{
    // Define the dynamic body. We set its position and call the body factory.
   b2BodyDef bodyDef;
   bodyDef.type = b2_dynamicBody;
   bodyDef.position.Set(pos.X, pos.Y);
   b2Body* body = world.CreateBody(&bodyDef);

   // Define another box shape for our dynamic body.
   b2PolygonShape dynamicBox;
   dynamicBox.SetAsBox(10.0f, 10.0f);

   // Define the dynamic body fixture.
   b2FixtureDef fixtureDef;
   fixtureDef.shape = &dynamicBox;

   // Set the box density to be non-zero, so it will be dynamic.
   fixtureDef.density = 1.0f;

   // Override the default friction.
   fixtureDef.friction = 0.3f;

   // Add the shape to the body.
   body->CreateFixture(&fixtureDef);

   bwBody* bww = new bwBody(dynamicBox, body, device);

   bodies.push_back(bww);
}

IrrlichtDevice *device = 0;

b2Vec2 gravity(0.0f, 10.0f);
bool doSleep = true;

//b2World world(gravity,doSleep); //box2d 2.1.2
b2World world(gravity);
   bwBody* bww;

class MyEventReceiver : public IEventReceiver
{
public:
   virtual bool OnEvent(const SEvent& event)
   {
      // Remember the mouse state
      if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
      {
         switch(event.MouseInput.Event)
         {
         case EMIE_LMOUSE_LEFT_UP:
         {
            createRigidBox(world, vector2d<s32>(event.MouseInput.X, event.MouseInput.Y), device);
         }
            break;

         default:
            break;
         }
      }
   }


   MyEventReceiver()
   {
   }
};

void rendermain(){

   device->run();
 //  {
      driver->beginScene(true, true, SColor(255,100,101,140));

      DeltaTime = timer->getTime() - TimeStamp;
        TimeStamp = timer->getTime();

      // Instruct the world to perform a single step of simulation.
      // It is generally best to keep the time step and iterations fixed.
      world.Step(DeltaTime*timeStep, velocityIterations, positionIterations);

      // Clear applied body forces. We didn't apply any forces, but you
      // should know about this function.
      world.ClearForces();

      for(int i=0; i < bodies.size(); i++)
      {
          bodies[i]->update();
      }

      bww->update();

      smgr->drawAll();
      guienv->drawAll();

      driver->endScene();
  // }

   }
#ifdef __EMSCRIPTEN__
   void main_loop(){

   EM_ASM({
      console.log('I received: ' + $0);
   }, 100);

   rendermain();
   }
   #endif
int main()
{
    MyEventReceiver receiver;
   device = createDevice( video::EDT_SOFTWARE, dimension2d<u32>(640, 480), 16, false, false, false, &receiver);

   if (!device)
      return 1;


   device->setWindowCaption(L"Irrlicht/Box2D Sample");


    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();
   timer = device->getTimer();


   guienv->addStaticText(L"Box2D integrated with Irrlicht",
      rect<s32>(10,10,130,22), true);


   smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

   /////////////////
   // Box2D Stuff //
   /////////////////

   // Define the ground body.
   b2BodyDef groundBodyDef;
   groundBodyDef.position.Set(290.0f, 250.0f);

   // Call the body factory which allocates memory for the ground body
   // from a pool and creates the ground box shape (also from a pool).
   // The body is also added to the world.
   b2Body* groundBody = world.CreateBody(&groundBodyDef);

   // Define the ground box shape.
   b2PolygonShape groundBox;

   // The extents are the half-widths of the box.
   groundBox.SetAsBox(290.0f, 10.0f);

   // Add the ground fixture to the ground body.
   groundBody->CreateFixture(&groundBox, 0.0f);

   bww = new bwBody(groundBox, groundBody, device);

   for(int i=0; i < 5; i++)
   {
        for(int j=0; j < 3; j++)
        {
            createRigidBox(world,vector2d<s32>(110+(i*45), 230-(j*20)), device);
        }
   }


   // Prepare for simulation. Typically we use a time step of 1/60 of a
   // second (60Hz) and 10 iterations. This provides a high quality simulation
   // in most game scenarios.
   timeStep = 1.0f / 250.0f;
   velocityIterations = 6;
   positionIterations = 2;


    TimeStamp = timer->getTime();
   DeltaTime = timer->getTime() - TimeStamp;


#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop,0,1);
#else
   while(device->run())
   {
rendermain();
   }
#endif
   for(int i=0; i < bodies.size(); i++)
    {
        delete bodies[i];
    }
    bodies.clear();

   delete bww;

   device->drop();

   return 0;
}
