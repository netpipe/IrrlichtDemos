/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#include "Font.h"

using namespace phoenix;
using namespace std;
using namespace boost;

Font::Font( RenderSystem& _r, int _t )
	: Resource( _r.getResourceManager(), _t ), renderer( &(_r.getBatchRenderer()) ), color(), scale( Vector2d( 1, 1 ) )
{

}