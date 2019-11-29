/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/


#include "Phoenix.h"

using namespace phoenix;

class GeometryTest
{
    public:

        GeometryTest() : 
			system()
        {
        }

        virtual ~GeometryTest()
        {
        }

        int run()
        {

            //! First geometry, second geom, combine the second to the first
            BatchGeometryPtr first = system.drawRectangle(phoenix::Rectangle(Vector2d(64,64),Vector2d(64,64)));
            first->colorize( Color(127,255,255,255) );
            first->setImmediate(false);

            BatchGeometryPtr second = system.drawRectangle(phoenix::Rectangle(Vector2d(96,96),Vector2d(64,64)));
            second->colorize( Color(0,0,255,255) );

            first->combine(second, true); //drop the second

            //! Make a composite text, combine it
            BatchGeometryCompositePtr composite = 
                system.getFont()->drawText("ABCDEFGHIJKLMAOPQRSTUVabcdefghijklmnopqrstuv1234567890", Vector2d(16,100))
                ->grab<BatchGeometryComposite>();

            composite->colorize(Color(255,0,0));

            BatchGeometryPtr new_geom = composite->combine();

            //! Draw two texts, combine them.
            BatchGeometryCompositePtr composite1 = 
                system.getFont()->drawText("Text One", Vector2d(16,150))
                ->grab<BatchGeometryComposite>();

            composite1->colorize(Color(0,255,0));

            BatchGeometryCompositePtr composite2 = 
                system.getFont()->drawText("Text Two", Vector2d(64,150))
                ->grab<BatchGeometryComposite>();

            composite2->colorize(Color(0,0,255));
            
            composite1->combine(composite2);
            composite1->setImmediate(false);

            //! Now just draw some stuff.
            while( system.run() )
            {

                //! Draw some info.
                system.drawText( "Geometry Test", Vector2d(16,16) )->colorize( Color(127,127,255,127) );

            }

            return 0;

        }// Run

    protected:
        RenderSystem system;

    private:
};


