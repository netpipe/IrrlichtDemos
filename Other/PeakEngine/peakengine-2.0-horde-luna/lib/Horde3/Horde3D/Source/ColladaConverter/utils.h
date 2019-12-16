// *************************************************************************************************
//
// Horde3D
//   Next-Generation Graphics Engine
// --------------------------------------
// Copyright (C) 2006-2008 Nicolas Schulz
//
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// *************************************************************************************************

#ifndef _utils_H_
#define _utils_H_

#include "utMath.h"
#include <cstring>
#include <cstdlib>
#include <string>
using namespace std;


bool parseString( char *s, unsigned int &pos, string &token );
bool parseFloat( char *s, unsigned int &pos, float &value );
bool parseUInt( char *s, unsigned int &pos, unsigned int &value );
void removeGate( string &s );
string decodeURL( const string &url );
string extractFileName( const string &fullPath, bool extension );
string extractFilePath( const string &fullPath );

void log( const string &msg );

Matrix4f makeMatrix4f( float *floatArray16, bool y_up );

#endif // _utils_H_
