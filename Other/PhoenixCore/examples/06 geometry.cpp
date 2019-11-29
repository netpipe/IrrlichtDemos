/*! Geometry Example.
    This example shows advanced usage of geometry objects as well
    as grouping geometry.
*/

#include "Phoenix.h"

using namespace phoenix;
using namespace boost;

/*!
    This class is used to setup and restore
    the blending modes for our geometry. It simply
    enables additive blending and then restores the
    blending state.
*/
class AdditiveState
	: public GroupState
{
public:
	AdditiveState():GroupState(){}
	virtual ~AdditiveState(){};

	void begin( BatchRenderer& r )
	{
		RenderSystem::setBlendMode( GL_SRC_ALPHA, GL_ONE );
	}

	void end( BatchRenderer& r )
	{
		RenderSystem::setBlendMode();
	}
};

int main()
{

    RenderSystem system;

    /*!
        Let's load up a texture to play with.
    */
    TexturePtr feather = system.loadTexture( std::string(PHOENIXCORE_DATA_DIR) + std::string("feather.png") );

    /*!
        let's make our group of geometry, with this
        we'll demonstrate grouping and group states.
        Grouped objects are drawn together and usually share
        some sort of state that's controlled by the group state
		object.

        The first thing we need to do is manipulate our factory:
        The RenderSystem. We can make it so that the next geometries
        that are made have specific properties. We'll set the group
        id and add the group state object.
    */
	system.getBatchRenderer().addGroupState( 1, GroupStatePtr( new AdditiveState() ) );
    system.getGraphicsFactory().setGroup( 1 );

    /*!
        Now we can draw some things, and they'll all
        be additively blended.
    */
    BatchGeometryPtr tgeom; // a temporary pointer for geometry.

    //! Here we just draw a basic scaled texture.
    tgeom = system.drawTexture( feather, Vector2d( 240,320 ), 0, Vector2d( 3.0f, 3.0f ) );
    tgeom->setImmediate( false );
    
    /*!
        Here we'll draw a rectangle centered on the origin, but we'll rotate and
        scale it then translate it to it's final position.
    */
    system.setDepth( 1.0f );
    tgeom = system.drawRectangle( Rectangle( -150,-150, 150,150 ) );
    tgeom->rotate( DegreesToRadians( 45.0f ) );
    tgeom->scale( Vector2d( 2.0f, 1.3f ) );
    tgeom->translate( Vector2d( 300, 400 ) );
    tgeom->colorize( Color( 127,200,255 ) );
    tgeom->setImmediate( false );

    /*!
        Lastly, we'll make some completely custom geometry. He'll we'll make
        a sort of circular geometry with a texture and custom tcoords

        We'll first use BatchGeometry's create function.
    */
    system.setDepth( 2.0f );
    tgeom = new BatchGeometry( system.getBatchRenderer(), GL_TRIANGLES, feather, system.getGraphicsFactory().getGroup(), system.getDepth() );
    tgeom->setImmediate( false );

    /*!
        Now we'll generate a simple circular peice of geometry. This is a 
        rather simple operation. You'll notice that we used  a different
        texture coordinate for the end, this creates a rather neat effect.
    */
    Vector2d center( 400, 200 );
    Vector2d ray( 250, 0 );
    for( int i = 0; i < 360; i+=10 )
    {
        tgeom->addVertex( Vertex( center, Color(), TextureCoords( 0.0f, 1.0f ) ) );
        tgeom->addVertex( Vertex( center + ( ray * RotationMatrix( DegreesToRadians( (float)i ) ) ), Color(), TextureCoords( 1.0f, 0.0f ) ) );
        tgeom->addVertex( Vertex( center + ( ray * RotationMatrix( DegreesToRadians( (float)i + 10.0f ) ) ), Color(), TextureCoords( 1.0f, 0.0f ) ) );
    }

    /*!
        Finally, we need to restore the properties we set. This is important
        if we draw any more geometry after this, because if we don't restore
        the properties all the rest of the geometry would have these properties
        too!
    */
    system.getGraphicsFactory().setDepth();
    system.getGraphicsFactory().setGroup();

    while( system.run() )
    {
        /*!
            Just for kicks, let make it where we can move the last geometry we
            created around with the arrow keys. This is easy achieved using
            translate.
        */
        if( EventReceiver::Instance()->getKeyPressed( PHK_UP ) ) tgeom->translate( Vector2d( 0.0f, -5.0f ) ); 
        if( EventReceiver::Instance()->getKeyPressed( PHK_DOWN ) ) tgeom->translate( Vector2d( 0.0f, 5.0f ) ); 
        if( EventReceiver::Instance()->getKeyPressed( PHK_RIGHT ) ) tgeom->translate( Vector2d( 5.0f, 0.0f ) ); 
        if( EventReceiver::Instance()->getKeyPressed( PHK_LEFT ) ) tgeom->translate( Vector2d( -5.0f, 0.0f ) ); 
    }
	
    return 0;

}
