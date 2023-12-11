/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#ifndef __PHOENIXVIEW__
#define __PHOENIXVIEW__

#include "config.h"
#include "Vector2d.h"
#include "WindowManager.h"

namespace phoenix
{

    //! View class.
    /*!
        Simple view class that simulates a camera looking down on a 2d scene.
        Handles translation, rotation and scaling of the view.
    */
    class View
    {

    public:

        //! Constructor.
        /*!
            \param _p The position of the top-left corner of the view.
			\param _s The size of the view.
        */
        View( const Vector2d& _p = Vector2d(0.0f,0.0f), const Vector2d& _s = Vector2d(-1.0f,-1.0f) )
			: pos(_p), size( _s ), rot(0.0f), scale(1.0f,1.0f)
		{
			setSize( _s );
		}

        //!  The current rotation of the view (in degrees).
        inline float getRotation() const { return rot; }

        //! Get the top-left position of the view.
        inline const Vector2d& getPosition() const { return pos; }

        //! The current scale of the view.
        inline const Vector2d& getScale() const { return scale; }

        //! Sets the top-left corner of the view.
        inline void setPosition(const Vector2d& _p) { pos = _p; }

        //! Sets the rotation of the view (in degrees).
        inline void setRotation(float _r) { rot = _r; }

        //! Sets the scale factor of the view.
        inline void setScale(const Vector2d& _s) { scale = _s; }

        //! Sets the size of the view
		/*!
			By passing in (-1,-1), the view will try to get the size
			from the window manager.
		*/
		inline void setSize( const Vector2d& _sz = Vector2d(-1,-1) ) {
			if( _sz ==  Vector2d(-1,-1) ){
				try{
					size = (WindowManager::Instance())->getWindowSize();
				}catch( WindowManager::BadInstance ){
					size = Vector2d(0,0);
				}
			}else{
				size = _sz; 
			}
		}

        //! Gets the size of the viewport
        inline const Vector2d& getSize() { return size; }

        //! Activate the view.
		/*!
			This performs the needed operations on the OpenGL Modelview matrix in order
			to set it to this view's parameters. It translates, then rotates, then scales.
		*/
        inline void activate()
		{
            // activate the viewport
            glViewport( 0, 0, (GLsizei) size.getX(), (GLsizei) size.getY() );

			glMatrixMode(GL_MODELVIEW);

			glLoadIdentity();

			// Camera offset.
			Vector2d cameraoffset = -pos-( size/2.0f);

			// Translation
			glTranslatef( -pos.getX(), -pos.getY(),0.0f);

			// Rotation
			glTranslatef( -cameraoffset.getX(), -cameraoffset.getY(), 0.0f);
			glRotatef(rot,0.0f,0.0f,1.0f);
			glTranslatef( cameraoffset.getX(), cameraoffset.getY(), 0.0f);

			// Scale
			glScalef(scale.getX(),scale.getY(),1.0f);
		}


    protected:

        //! The top-left position of the view.
        Vector2d pos;


        //! The size of the opengl viewport.
        Vector2d size;

        //! The current rotation of the view (In degrees).
        float rot;

        //! Stores The current scale factor of the scene, (1,1) is default.
        Vector2d scale;
    };

} //namespace phoenix

#endif //__PHOENIXVIEW__
