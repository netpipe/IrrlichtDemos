    /*
    Simple Graph
    (c) Nicolaus Anderson
    Created Jan 7, 2013

    License: Same terms as irrlicht
    */

    #include "SGraph2D.h"

    #ifndef __SIMPLEGRAPH2D_CPP__
    #define __SIMPLEGRAPH2D_CPP__

    void irr::gui::SGraph2D::setGraphSize( irr::core::rectf& size )
    {
        window = size;

        ReallocateGraphSpace();
    }

    void irr::gui::SGraph2D::setGraphSizeX( irr::f32 size, bool isMax )
    {
        if ( isMax )
        {
            window.LowerRightCorner.X = size;
        } else {
            window.UpperLeftCorner.X = size;
        }

        ReallocateGraphSpace();
    }

    void irr::gui::SGraph2D::setGraphSizeY( irr::f32 size, bool isMax )
    {
        if ( isMax )
        {
            window.UpperLeftCorner.Y = size;
        } else {
            window.LowerRightCorner.Y = size;
        }

        ReallocateGraphSpace();
    }

    void irr::gui::SGraph2D::setScale( irr::core::rectf& scale )
    {
        window.UpperLeftCorner.X *= scale.UpperLeftCorner.X;
        window.UpperLeftCorner.Y *= scale.UpperLeftCorner.Y;
        window.LowerRightCorner.X *= scale.LowerRightCorner.X;
        window.LowerRightCorner.Y *= scale.LowerRightCorner.Y;

        window.repair();

        ReallocateGraphSpace();
    }

    void irr::gui::SGraph2D::ReallocateGraphSpace()
    {
        graphImage.reallocate(
            (irr::u32)AbsoluteRect.getWidth(),
            false
            );
    }

    irr::core::rectf irr::gui::SGraph2D::getGraphSize()
    {
        return window;
    }

    Range<irr::f32> irr::gui::SGraph2D::getXAxisRange()
    {
        return Range<irr::f32>(
                    window.UpperLeftCorner.X,
                    window.LowerRightCorner.X
                    );
    }

    Range<irr::f32> irr::gui::SGraph2D::getYAxisRange()
    {
        return Range<irr::f32>(
                    window.UpperLeftCorner.Y,
                    window.LowerRightCorner.Y
                    );
    }

    Inc<irr::f32> irr::gui::SGraph2D::getIterableXRange()
    {
        Inc<irr::f32> inc(CYC_REPEAT);

///        inc.setRange( getXAxisRange() );
        inc.setStep( window.getWidth() / ((irr::f32)AbsoluteRect.getWidth()) );
        inc.restart();

        return inc;
    }

    Inc<irr::f32> irr::gui::SGraph2D::getIterableYRange()
    {
        Inc<irr::f32> inc(CYC_REPEAT);

///        inc.setRange( getYAxisRange() );
        inc.setStep( window.getHeight() / ((irr::f32)AbsoluteRect.getHeight()) );
        inc.restart();

        return inc;
    }

    void irr::gui::SGraph2D::markerSpacingX( irr::f32 gap )
    {
        markXgap = gap;
    }

    void irr::gui::SGraph2D::markerSpacingY( irr::f32 gap )
    {
        markYgap = gap;
    }

    void irr::gui::SGraph2D::draw()
    {
        // Don't bother doing anything if this isn't visible
        if ( !IsVisible || AbsoluteRect.getArea() == 0 || window.getArea() == 0.0f )
            return;

        // variables...

        irr::core::vector2di point; /* point to be drawn on screen representing
                                    a point from the MathFunc */

            /* Marker offset
            - used to ensure the markers are drawn from the center outward. */
        Inc<irr::f32> markerIter;   /* no wrapping because we want
                                    out-of-bounds checking */
            // Marker line - drawn on the GUI graph screen
        irr::core::line2df marker;


        // operations...

        // Draw the background if there is one
        if ( hasBackground )
        {
            viddriver->draw2DRectangle(
                                background_color,
                                AbsoluteRect,
                                &AbsoluteClippingRect
                                );
        }

        // Draw the axis if desired

        if ( showXaxis )
        {
            drawToGUI(
                irr::core::line2df(
                    window.UpperLeftCorner.X,
                    0.0f,
                    window.LowerRightCorner.X,
                    0.0f
                    ),
                xaxis_color
                );
        }

        if ( showYaxis )
        {
            drawToGUI(
                irr::core::line2df(
                    0.0f,
                    window.UpperLeftCorner.Y,
                    0.0f,
                    window.LowerRightCorner.Y
                    ),
                yaxis_color
                );
        }

        // Draw the markers/lines if desired
        if ( UseMarkers )
        {
                // Y-axis
            if ( showYaxisMarks )
            {
                /* Set the offset for making the lines appear to be drawn
                from the center outward */
                markerIter.setMin( window.UpperLeftCorner.Y );
                markerIter.setMax( window.LowerRightCorner.Y );
                markerIter.setStep( markYgap );
                markerIter.setVal( 0.0f ); // start in the center

                // Set up the marker line

                    // Left side of the line
                if ( UseTicks )
                    marker.start.X = -window.getWidth() / 20.0f; // for 10% window width
                else
                    marker.start.X = window.UpperLeftCorner.X;

                    // Right side of the line
                if ( UseTicks )
                    marker.end.X = window.getWidth() / 20.0f; // for 10% window width
                else
                    marker.end.X = window.LowerRightCorner.X;

                // Draw each marker
                while ( !++markerIter ) // go until past the max
                {
                    // Assign the position to the line to draw
                    marker.start.Y = marker.end.Y = markerIter.Val();

                    // Draw the marker to the GUI
                    drawToGUI( marker, ymark_color );
                }

                // Restart at the center
                markerIter = 0.0f;

                // Draw each marker
                while ( !--markerIter ) // go until past the min
                {
                    // Assign the position to the line to draw
                    marker.start.Y = markerIter.Val();
                    marker.end.Y = markerIter.Val();

                    // Draw the marker to the GUI
                    drawToGUI( marker, ymark_color );
                }
            }

                // X-axis
            if ( showXaxisMarks )
            {
                /* Set the offset for making the lines appear to be drawn
                from the center outward */
                markerIter.setMin( window.UpperLeftCorner.X );
                markerIter.setMax( window.LowerRightCorner.X );
                markerIter.setStep( markXgap );
                markerIter.setVal( 0.0f ); // start in the center

                // Set up the marker line

                    // Top of the line - Below the x-axis in terms of irrlicht drawing
                if ( UseTicks )
                    marker.start.Y = window.getHeight() / 20.0f; // for 10% window height
                else
                    marker.start.Y = window.UpperLeftCorner.Y;

                    // Bottom of the line - Above the x-axis in terms of irrlicht drawing
                if ( UseTicks )
                    marker.end.Y = -window.getHeight() / 20.0f; // for 10% window height
                else
                    marker.end.Y = window.LowerRightCorner.Y;

                // Draw each marker
                while ( !++markerIter ) // go until past the max
                {
                    // Assign the position to the line to draw
                    marker.start.X = marker.end.X = markerIter.Val();

                    // Draw the marker to the GUI
                    drawToGUI( marker, xmark_color );
                }

                // Restart at the center
                markerIter = 0.0f;

                // Draw each marker
                while ( !--markerIter ) // go until past the min
                {
                    // Assign the position to the line to draw
                    marker.start.X = markerIter.Val();
                    marker.end.X = markerIter.Val();

                    // Draw the marker to the GUI
                    drawToGUI( marker, xmark_color );
                }
            }
        }

        // Display each points on screen
        for (
            irr::s32 pt = 0;
            pt < (irr::s32)graphImage.size();
            pt++
            )
        {
            /* Note that when drawing the graph in the GUI element,
            we need to shift their stored values because the bounds of the
            graph window are independent of the actual GUI element size. */

            /*
            point.X = (irr::s32)(graphImage[pt].Pos.X - window.UpperLeftCorner.X)
                        * AbsoluteRect.getWidth();
            */

            point.X = (irr::s32)graphImage[pt].Pos.X;

            point.Y = (irr::s32)graphImage[pt].Pos.Y;
                /* Note the y-axis value is negative in order have it drawn
                in the orientation the GUI environment is drawn. */

            drawToGUI( point, graphImage[pt].Color );
        }
    }

    void irr::gui::SGraph2D::clearGraph()
    {
        graphImage.clear();
    }

    void irr::gui::SGraph2D::useBackground( bool use )
    {
        hasBackground = use;
    }

    void irr::gui::SGraph2D::setBackgroundColor( irr::video::SColor color )
    {
        background_color = color;
    }

    void irr::gui::SGraph2D::setPointColor( irr::video::SColor color )
    {
        point_color = color;
    }

    void irr::gui::SGraph2D::setXAxisColor( irr::video::SColor color )
    {
        xaxis_color = color;
    }

    void irr::gui::SGraph2D::setYAxisColor( irr::video::SColor color )
    {
        yaxis_color = color;
    }

    void irr::gui::SGraph2D::setShowXAxis( bool show )
    {
        showXaxis = show;
    }

    void irr::gui::SGraph2D::setShowYAxis( bool show )
    {
        showYaxis = show;
    }

    void irr::gui::SGraph2D::useTicks()
    {
        UseMarkers = true;
        UseTicks = true;

        showXaxisMarks = true;
        showYaxisMarks = true;
    }

    void irr::gui::SGraph2D::useLines()
    {
        UseMarkers = true;
        UseTicks = false;

        showXaxisMarks = true;
        showYaxisMarks = true;
    }

    void irr::gui::SGraph2D::useNoMarkers()
    {
        UseMarkers = false;
    }

    void irr::gui::SGraph2D::showXAxisMarkers( bool show )
    {
        showXaxisMarks = show;
        if (show) UseMarkers = true;
    }

    void irr::gui::SGraph2D::showYAxisMarkers( bool show )
    {
        showYaxisMarks = show;
        if (show) UseMarkers = true;
    }

    void irr::gui::SGraph2D::setXAxisMarkerColor( irr::video::SColor color )
    {
        xmark_color = color;
    }

    void irr::gui::SGraph2D::setYAxisMarkerColor( irr::video::SColor color )
    {
        ymark_color = color;
    }

    void irr::gui::SGraph2D::serializeAttributes(
        irr::io::IAttributes *out,
        irr::io::SAttributeReadWriteOptions *options
        )
    {
        irr::core::recti win;
        win.UpperLeftCorner.X = (irr::s32)(window.UpperLeftCorner.X);
        win.UpperLeftCorner.Y = (irr::s32)(window.UpperLeftCorner.Y);
        win.LowerRightCorner.X = (irr::s32)(window.LowerRightCorner.X);
        win.LowerRightCorner.Y = (irr::s32)(window.LowerRightCorner.Y);

        out->addRect( "Window", win );

        out->addBool( "FillBackground", hasBackground );
        out->addColor( "BGColor", background_color );
        out->addColor( "XAxisColor", xaxis_color );
        out->addColor( "YAxisColor", yaxis_color );
        out->addColor( "PointColor", point_color );
        out->addColor( "XAxisTickColor", xmark_color );
        out->addColor( "YAxisTickColor", ymark_color );

        out->addBool( "UseMarkers", UseMarkers );
        out->addBool( "UseTicks", UseTicks );

        out->addBool( "ShowXAxis", showXaxis );
        out->addBool( "ShowYAxis", showYaxis );
    }

    void irr::gui::SGraph2D::deserializeAttributes(
        irr::io::IAttributes *in,
        irr::io::SAttributeReadWriteOptions *options
        )
    {
        irr::core::recti win = in->getAttributeAsRect( "Window" );
        window.UpperLeftCorner.X = (irr::f32)(win.UpperLeftCorner.X);
        window.UpperLeftCorner.Y = (irr::f32)(win.UpperLeftCorner.Y);
        window.LowerRightCorner.X = (irr::f32)(win.LowerRightCorner.X);
        window.LowerRightCorner.Y = (irr::f32)(win.LowerRightCorner.Y);

        hasBackground       =   in->getAttributeAsBool( "FillBackground" );
        background_color    =   in->getAttributeAsColor( "BGColor" );
        xaxis_color         =   in->getAttributeAsColor( "XAxisColor" );
        yaxis_color         =   in->getAttributeAsColor( "YAxisColor" );
        point_color         =   in->getAttributeAsColor( "PointColor" );
        xmark_color         =   in->getAttributeAsColor( "XAxisTickColor" );
        ymark_color         =   in->getAttributeAsColor( "YAxisTickColor" );

        UseMarkers          =   in->getAttributeAsBool( "UseMarkers" );
        UseTicks            =   in->getAttributeAsBool( "UseTicks" );

        showXaxis           =   in->getAttributeAsBool( "ShowXAxis" );
        showYaxis           =   in->getAttributeAsBool( "ShowYAxis" );
    }

    void irr::gui::SGraph2D::drawOnGraph(
        irr::core::vector2df point, irr::video::SColor color
        )
    {
        // Shift the origin to the corner
        point.X -= window.UpperLeftCorner.X;
        point.Y -= window.UpperLeftCorner.Y;

        // Convert to the actual GUI window's coordinate system
        point.X *= ((irr::f32)AbsoluteRect.getWidth()) / window.getWidth();
        point.Y *= ((irr::f32)AbsoluteRect.getHeight()) / window.getHeight();

        // Save
        graphImage.push_back(
            irr::video::S3DVertex( point.X, point.Y, 0,0,0,0, color, 0,0)
            );
    }

    void irr::gui::SGraph2D::drawRawPoint(
        irr::core::vector2df point,
        irr::video::SColor color
        )
    {
        // Shift the origin to the corner
        point.X -= window.UpperLeftCorner.X
             // conversion of shift to GUI element size
            * ((irr::f32)AbsoluteRect.getWidth()) / window.getWidth();

        point.Y -= window.UpperLeftCorner.Y
            // conversion of shift to GUI element size
            * ((irr::f32)AbsoluteRect.getHeight()) / window.getHeight();

        // Save
        graphImage.push_back(
            irr::video::S3DVertex( point.X, point.Y, 0,0,0,0, color, 0,0)
            );
    }

    void irr::gui::SGraph2D::drawToGUI(
                            irr::core::vector2di point,
                            irr::video::SColor color
                            )
    {
        // Flip for drawing on the GUI
        point.Y = AbsoluteRect.getHeight() - point.Y;

        /* This function has been passed a value that is already prepared to
        be drawn to screen and simply needs to be offset. */
        point.X += AbsoluteRect.UpperLeftCorner.X;
        point.Y += AbsoluteRect.UpperLeftCorner.Y;

        // Do nothing if the window isn't even visible
        if ( window.getArea() == 0.0f )
            return;

        // Draw the point as a pixel - might be very small and hard to see
        /* - Drawn faster than a polygon and may create a smooth line (if MathFunc
        returns values of a smooth function) */
        viddriver->drawPixel( point.X, point.Y, point_color );

        // Attempt to draw the point as a polygon (to make it more visible)

        /*
        viddriver->draw2DPolygon(
                        point,          // position
                        1.5f,           // radius
                        point_color,    // color
                        4               // roundness of the point
                        );
                        */

    }

    void irr::gui::SGraph2D::drawToGUI(
                            irr::core::line2df line,
                            irr::video::SColor color
                            )
    {
        // Do nothing if the window isn't even visible
        // - saves time and prevents divide-by-zero errors
        if ( window.getArea() == 0.0f )
            return;

        /* This function has been passed a value that needs to be prepared
        to fit in the window in addition to it being offset. */

        // Line that will be drawn - initialized to begin within the GUI element
        irr::core::line2di drawline(
            AbsoluteRect.UpperLeftCorner.X,
            AbsoluteRect.UpperLeftCorner.Y,
            AbsoluteRect.UpperLeftCorner.X,
            AbsoluteRect.UpperLeftCorner.Y
            );

        /* Tranform the line to graph coordinates. */
        line.start.X -= window.UpperLeftCorner.X;
        line.start.Y -= window.UpperLeftCorner.Y;
        line.end.X -= window.UpperLeftCorner.X;
        line.end.Y -= window.UpperLeftCorner.Y;

        // starting x
        drawline.start.X += (irr::s32) ( line.start.X
            * AbsoluteRect.getWidth() / window.getWidth() // position conversion
            );

        // starting y
        drawline.start.Y += (irr::s32) ( line.start.Y
            * AbsoluteRect.getHeight() / window.getHeight() // position conversion
            );

        // ending x
        drawline.end.X += (irr::s32) ( line.end.X
            * AbsoluteRect.getWidth() / window.getWidth() // position conversion
            );

        // ending y
        drawline.end.Y += (irr::s32) ( line.end.Y
            * AbsoluteRect.getHeight() / window.getHeight() // position conversion
            );


        // Draw the line
        viddriver->draw2DLine( drawline.start, drawline.end, color );
    }


    #endif // define __SIMPLEGRAPH2D_CPP__
