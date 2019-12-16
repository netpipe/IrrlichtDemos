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

#ifndef _optimizer_H_
#define _optimizer_H_

#include <vector>
#include <set>
using namespace std;

struct TriGroup;
struct Vertex;
struct OptFace;


struct OptVertex
{
	unsigned int		index;		// Index in vertex array
	float				score;
	set< OptFace * >	faces;		// Faces that are using this vertex

	void updateScore( int cacheIndex );
};

struct OptFace
{
	OptVertex	*verts[3];
	
	float getScore()  { return verts[0]->score + verts[1]->score + verts[2]->score; }
};

class MeshOptimizer
{
public:

	static const int OptCacheSize = 32;
	
	static unsigned int removeDegeneratedTriangles( TriGroup &triGroup, vector< Vertex > &vertices,
													vector< unsigned int > &indices );
	static void optimizeIndexOrderNV( TriGroup &triGroup, vector< Vertex > &vertices,
									  vector< unsigned int > &indices );
	static void optimizeIndexOrder( TriGroup &triGroup, vector< Vertex > &vertices,
									 vector< unsigned int > &indices );
};

#endif	// _optimizer_H_
