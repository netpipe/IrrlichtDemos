#ifndef UTIL_H
#define UTIL_H

#include <irrlicht.h>

inline bool interpolatePosition(irr::core::vector2df &from, irr::core::vector2df to, float speed, irr::u32 t)
{
    if (from == to)
        return false;

    irr::core::vector2df dir = to - from;
    float length = dir.getLength();
    float speedT = (speed * t);
    if (length >= speedT) {
        dir.normalize();
        dir *= speedT;
        from += dir;
    } else {
        from = to;
        return false;
    }
    return true;
}

inline bool interpolatePosition(float &fromX, float &fromY, float toX, float toY, float speed, irr::u32 t)
{
    if (fromX == toX && fromY == toY)
        return false;
        
    irr::core::vector2df from(fromX, fromY);
    irr::core::vector2df to(toX, toY);
    irr::core::vector2df dir = to - from;
    float length = dir.getLength();
    float speedT = (speed * t);
    if (length >= speedT) {
        dir.normalize();
        dir *= speedT;
        from += dir;
        fromX = from.X;
        fromY = from.Y;
    } else {
        fromX = toX;
        fromY = toY;
    }
    return true;
}

inline int readLine(irr::io::IReadFile *file, irr::c8* line)
{
    irr::c8 temp;
    irr::u32 lineIndex = 0;

    while ( file->read( &temp, 1 ) == 1 )
    {
        // Windows line endings use '\r\n' while Unix line endings use simply '\n'
        if ( temp == '\r' )
        {
            // read the '\n' after the '\r' to get to the beginning of the next line.
            temp = file->read( &temp, 1 );

            break;
        }
        else if ( temp == '\n' )
        {
            // nothing to do, if '\n' comes first, then it's Unix line endings and
            // the file position is already at the beginning of the next line.
            break;
        }

        line[lineIndex++] = temp;
    }

    line[lineIndex] = '\0';

    return lineIndex;
}


#endif
