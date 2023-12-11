/*! Resource example.
    In this example we will explore phoenix's resources. It
    will show how to define custom resources and manipulate
    them.
*/

#include "Phoenix.h"

using namespace phoenix;
using namespace boost;

/*!
    The first thing we'll do is define our resource. We'll
    simply make a class and derive from Resource. 
*/
class ExampleResource
    : public Resource
{
public:

    /*!
        Here we have a constructor, you'll have to pass the resource manager in
        so that Resource can be constructed.
    */
    ExampleResource( ResourceManager& r )
        : Resource( r ), position(), color()
    {
        /*!
            Here we'll set up some stuff about our resource. We'll give it a
            random color and position.
        */
        color = Color( random(127,255), random(127,255), random(127,255) );
        position = Vector2d( (float)random(0,620), (float)random(0,440) );
    }

    /*!
        Now for an interesting function. This is our draw funcion which we
        will call for every resource.
    */
    void draw( RenderSystem& system )
    {
        /*!
            Just like in the Hello World example, we'll draw some text.
            This time we'll draw it with our position and color.
        */
        system.drawText( "Resource" , position, color );

        /*!
            What if we'd like for these resources to go away? Well,
            in order to get rid of a resource, we drop it. Here we
            check if the user has pressed the spacebar, and if our
            random function landed on 1. If so, we'll drop this
            resource.
        */
        if( EventReceiver::Instance()->getKeyPressed( PHK_SPACE ) && random(0,10) == 1 )
            drop();
    }

    /*!
        Also note that it is possible (and sometimes desired) to
        overload the drop() function. You can use drop as a moment
        to clear up or drop() and other resources. However, you
        should usually wait until the Garbage Collector calls the
        destructor before actually deleting any internal data.
    */

private:

    /*!
        These are just our color and position members.
    */
    Vector2d position;
    Color color;

};

int main()
{

    RenderSystem system;

    /*!
        We need a resource manager to keep track of all of
        our resources. We could use the system's internal
        resource manager, but we'd have to check for types
        in that case, because it also stores all the textures
        and fonts.
    */
    ResourceManager examples;

    //! We'll also want to start its garbage collector.
    examples.start();

    /*!
        Now let's fill the resource manager with 10 resources.
    */
    for( int i = 0; i < 10; ++i )
    {
        new ExampleResource( examples );
    }

    while( system.run() )
    {

        /*!
            Now we'll iterate through each resource and call draw().
            Because garbage collection occurs in another thread, we
            must lock the resource manager before doing so.
        */
        examples.lock();

        /*!
            Now the iteration. The loop looks more complicated than it actually is. 
            It's just standard list iteration. sometimes it's more asthetically 
            pleasing to use this:
                BOOST_FOREACH( ResourcePtr& r, examples.getResourceList() )
            but for this example I wanted to show iterating without foreach.
        */
        for( std::list< ResourcePtr >::iterator i = examples.getList().begin(); i != examples.getList().end(); ++i )
        {
            /*!
                First, we make sure the resource hasn't been dropped. Usually it is
                still safe to use dropped resources, but it is undesirable to do so.
            */
            if( ! (*i)->dropped() )
            {
                /*!
                    Now we'll use Resource's templated grab<>() function to automatically
                    cast to an ExampleResource and then we'll call our draw function.
                */
                (*i)->grab<ExampleResource>()->draw( system );
            }
        }

        /*!
            Finally, we can unlock our resource manager and let the garbage collector
            work.
        */
        examples.unlock();

    }

    return 0;

}
