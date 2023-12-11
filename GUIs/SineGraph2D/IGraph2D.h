    /*
    Simple Graph
    (c) Nicolaus Anderson
    Created Jan 7, 2013

    License: Same terms as irrlicht
    */

    #include <irrlicht.h>

    #ifndef _IGRAPH2D_
    #define _IGRAPH2D_

    namespace irr
    {
    namespace gui
    {

    //! Class Graph 2D
    /*
    Purpose: To plot 2D math functions.
    */
    class IGraph2D : public IGUIElement
    {
    public:

        IGraph2D( IGUIEnvironment* envir, IGUIElement* parent, irr::s32 id, irr::core::recti& rectangle )
            : IGUIElement( EGUIET_ELEMENT, envir, parent, id, rectangle )
        {
        }

        ~IGraph2D()
        {
        }

            //! Set graph size
        /* NOT the same as scaling.
        This function changes the ranges of values in the graph without changing
        the size of the graph as a GUI element.
        */
        virtual void setGraphSize( irr::core::rectf& size )=0;

            //! Set graph size along one dimension
        /* NOT the same as scaling.
        This function changes the ranges of values of ONE AXIS in the graph without
        changing the size of the graph as a GUI element.
        \param size - New axis min or max
        \param isMax - If the value given is for the maximum
        */
        virtual void setGraphSizeX( irr::f32 size, bool isMax=true )=0;
        virtual void setGraphSizeY( irr::f32 size, bool isMax=true )=0;

            //! Set scale
        /* Changes the scale of the ranges of values in the graph without changing
        the size of the graph itself.
        NOTE: Since this is a scaling, the rectangle being passed in should be the
        percentage change (i.e. multiplier for the current values). */
        virtual void setScale( irr::core::rectf& scale )=0;

            //! Get graph size
        /* Returns the size of the graph. */
        virtual irr::core::rectf getGraphSize()=0;

            //! Draw
        /* Draws the GUI element. */
        virtual void draw()=0;

            //! Clear graph
        /* Erases everything in the graph. */
        virtual void clearGraph()=0;

        //-------------------------------
        // Drawing area functions / Spaz

            //! Set point color
        /* Sets the color of the points to be displayed. */
        virtual void setPointColor( irr::video::SColor color )=0;

            //! Set x-axis color
        /* Sets the color that will be used in drawing the line representing
        the x-axis. */
        virtual void setXAxisColor( irr::video::SColor color )=0;

            //! Set y-axis color
        /* Sets the color that will be used in drawing the line representing
        the y-axis. */
        virtual void setYAxisColor( irr::video::SColor color )=0;

        //-----------------------------

        //! Serialize attributes
        virtual void serializeAttributes(
            irr::io::IAttributes* out,
            irr::io::SAttributeReadWriteOptions* options=0
            )=0;

        //! Deserialize attributes
        virtual void deserializeAttributes(
            irr::io::IAttributes* in,
            irr::io::SAttributeReadWriteOptions* options=0
            )=0;
    };

    } // end namespace gui
    } // end namespace irr

    #endif // define _IGRAPH2D_
