/*!
\mainpage PhoenixCore

\authors Jonathan Wayne Parrott

\section Introduction
PhoenixCore is a 2d rendering framework built on top of OpenGL. Phoenix provides a very simple abstraction over OpenGL.
Phoenix aims to be fast, flexible, extendable, and easy to use.

\section Quick Example
\code

#include "RenderSystem.h"

int main()
{

    RenderSystem system;

    while( system.run() )
    {

        system.drawText( "Hello, World!", Vector2d(50,50) );

    }

}

\endcode

\section License
\verbinclude source/license.txt
*/