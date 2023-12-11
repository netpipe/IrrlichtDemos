/*! Box2d Example.
	Shows very simple use of phoenix and box2d.
*/

/*!
    Include our header.
*/
#include "Phoenix.h"
#include "Box2d.h"

const float scale_factor = 4.8f;

/*!
    We'll use the phoenix namespace so that we don't
    have to type phoenix:: in front of everything.
*/
using namespace phoenix;

/*! Helper function to draw bodies
    Box2d's internal representation of polygons is slightly different from ours,
    so we'll need to transform it.
*/
void drawBody( RenderSystem& system, b2Body *body, Color color = Color(0,127,255) )
{
    // Iterate through the list of shapes.
    for( b2Shape* shape = body->GetShapeList(); shape != NULL; shape = shape->GetNext() )
    {
        if( shape->GetType() == 1 ) // Polygons only.
        {
            b2PolygonShape* polyshape = (b2PolygonShape*) shape; // Get the shape.
            b2Vec2 polyposition = body->GetPosition() + polyshape->GetCentroid(); //Calcuate our polygon's center.
            Polygon tpoly( Vector2d( polyposition.x, polyposition.y ) * scale_factor ); //Make a new polygon
            for( int i = 0; i < polyshape->GetVertexCount(); i++ ) //Iterate through the vertices.
            {
                // Transform the vertex and add it to the polygon.
                b2Vec2 tv = b2Mul( body->GetXForm(), polyshape->GetVertices()[i]);
                tpoly.addPoint( Vector2d(tv.x, tv.y) * scale_factor );
            }
            // Finally, draw it.
            system.drawPolygon( tpoly, color );
        }
    }
}

/*!
    Your standard main() function. 
*/
int main()
{

    //! We'll need a render system, of course.
    RenderSystem system;

    //! Timer for jumping.
    Timer jumptimer;

    //! Set up a Box2d World.
    b2AABB worldAABB;
    worldAABB.lowerBound.Set(-500.0f, -500.0f);
    worldAABB.upperBound.Set(1500.0f, 500.0f);

    b2Vec2 gravity(0.0f, 20.0f);
    bool doSleep = true;
    b2World world(worldAABB, gravity, doSleep);
    //world.SetPositionCorrection( true );

    /*!
        Let's make some bodies!
    */

    //! The Ground and Walls.

	b2BodyDef bd;
	bd.position.Set(66.5f, 50.0f);
	b2Body* ground = world.CreateBody(&bd);

	b2PolygonDef sd;
	sd.density = 0.0f;
	sd.restitution = 0.2f;

	sd.SetAsBox(1.0f, 50.0f, b2Vec2(-66.5f, 0.0f), 0.0f);
	ground->CreateShape(&sd);

	sd.SetAsBox(1.0f, 50.0f, b2Vec2(500.5f, 0.0f), 0.0f);
	ground->CreateShape(&sd);

	sd.SetAsBox(1.0f, 500.5f, b2Vec2(0.0f, -50.0f), 0.5f * b2_pi);
	ground->CreateShape(&sd);

	sd.SetAsBox(1.0f, 500.5f, b2Vec2(0.0f, 50.0f), -0.5f * b2_pi);
	ground->CreateShape(&sd);

    //! A couple of platforms.

    sd.SetAsBox(1.0f, 10.0f, b2Vec2(0.0f, 0.0f), -0.5f * b2_pi);
	ground->CreateShape(&sd);

    sd.SetAsBox(1.0f, 20.0f, b2Vec2(-20.0f, 30.0f), -0.5f * b2_pi);
	ground->CreateShape(&sd);


    /*!
        Now a body for our little dynamic box.
    */
    b2BodyDef bodyDef;
    bodyDef.position.Set(50.0f, 0.0f);
    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonDef shapeDef;
    shapeDef.SetAsBox(5.0f, 5.0f);
    shapeDef.density = 1.0f;
    shapeDef.friction = 0.03f;
    shapeDef.restitution = 0.4f;
    b2PolygonShape* dynamicShape = (b2PolygonShape*)body->CreateShape(&shapeDef);
    body->SetMassFromShapes();

    // timestep setup
    float timestep = 1.0f/60.0f; //60hz
    int iterations = 20;

    // Timer for the simulation
    Timer physicstimer;
    physicstimer.start();

    while( true )
    {
        if( physicstimer.getTime() > timestep)
        {

            if(! system.run() )
                break;

            //! Do a step of the simulation.
            world.Step( float(physicstimer.getTime())*3.0f, iterations);
            physicstimer.reset();

            //! Platform creation code.
            if( EventReceiver::Instance()->getMouseButtonPressed( PHK_MB_LEFT ) )
            {
                /*!
                    We just get the mouse's position in the world, then create a platform with a random orientation.
                */
                Vector2d absmouseposition = system.getView().getPosition() + EventReceiver::Instance()->getMousePosition();
                b2Vec2 worldpoint( absmouseposition.getX()/scale_factor,  absmouseposition.getY()/scale_factor );
                sd.SetAsBox(1.0f, (float)random(8,15), ground->GetLocalPoint(worldpoint), ( -0.5f + ( random(-8,8)/100.0f ) ) * b2_pi);
	            ground->CreateShape(&sd);
            }

            //! Movement code.
            if( EventReceiver::Instance()->getKey( PHK_LEFT ) )
            {
                body->ApplyForce( b2Vec2( -2000.f, 0 ) , body->GetWorldPoint( b2Vec2( 0,0 ) ) );
            }

            if( EventReceiver::Instance()->getKey( PHK_RIGHT ) )
            {
                body->ApplyForce( b2Vec2( 2000.f, 0 ) , body->GetWorldPoint( b2Vec2( 0,0 ) ) );
            }

            //! Jump code.
            if( EventReceiver::Instance()->getKey( PHK_UP ) && jumptimer.getTime() < 0.01f)
            {
                body->ApplyForce( b2Vec2( 0.f, -100000.f), body->GetWorldPoint( b2Vec2( 0,0 ) ) );
            }

            if( EventReceiver::Instance()->getKeyPressed( PHK_UP ) ) jumptimer.start();
            if( EventReceiver::Instance()->getKeyReleased( PHK_UP ) ) jumptimer.stop();

            /*! Viewport Code
                We just adjust the viewport based on where the dynamic body happens to be.
            */
            Vector2d bodypos = Vector2d( body->GetPosition().x,  body->GetPosition().y ) * scale_factor;
            Vector2d viewcenter = system.getView().getPosition() + ( system.getView().getSize()/2.0f );
            Vector2d difference = bodypos-viewcenter;
            if( difference.getMagnitudeSquared() > 4.0f )
            {
                system.getView().setPosition( system.getView().getPosition() + difference*0.5f );
            }
            
            //! Draw some stuff.
            drawBody( system, body, Color(255,127,0));
            drawBody( system, ground );

			system.getDebugConsole()<<"\nFPS: "<<system.getFPS();
        }
    }

    return 0;

}
