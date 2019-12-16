#include "optimizer.h"
#include "converter.h"
#include <list>
#include <algorithm>


void OptVertex::updateScore( int cacheIndex )
{
	if( faces.empty() )
	{	
		score = 0;
		return;
	}
	
	// The constants used here are coming from the paper
	if( cacheIndex < 0 ) score = 0;				// Not in cache
	else if( cacheIndex < 3 ) score = 0.75f;	// Among three most recent vertices
	else score = pow( 1.0f - ((cacheIndex - 3) / MeshOptimizer::OptCacheSize), 1.5f );

	score += 2.0f * pow( (float)faces.size(), -0.5f );
}



unsigned int MeshOptimizer::removeDegeneratedTriangles( TriGroup &triGroup, vector< Vertex > &vertices,
														vector< unsigned int > &indices )
{
	unsigned int numDegTris = 0;
	
	for( unsigned int k = triGroup.first; k < triGroup.first + triGroup.count; k += 3 )
	{
		Vec3f &v0 = vertices[indices[k + 0]].pos;
		Vec3f &v1 = vertices[indices[k + 1]].pos;
		Vec3f &v2 = vertices[indices[k + 2]].pos;

		if( (v2 - v0).crossProduct( v1 - v0 ) == Vec3f( 0, 0, 0 ) )
		{
			++numDegTris;
			// Remove triangle indices
			indices.erase( indices.begin() + k + 2 );
			indices.erase( indices.begin() + k + 1 );
			indices.erase( indices.begin() + k + 0 );
			k -= 3;
			triGroup.count -= 3;
		}
	}

	return numDegTris;
}


void MeshOptimizer::optimizeIndexOrder( TriGroup &triGroup, vector< Vertex > &vertices,
										vector< unsigned int > &indices )
{
	// Implementation of Linear-Speed Vertex Cache Optimisation by Tom Forsyth
	// (see http://home.comcast.net/~tom_forsyth/papers/fast_vert_cache_opt.html)
	
	if( triGroup.count == 0 ) return;
	
	vector< OptVertex > verts( triGroup.vertREnd - triGroup.vertRStart + 1 );
	set< OptFace * > faces;
	list< OptVertex * > cache;
	
	// Build vertex and triangle structures
	for( unsigned int i = 0; i < triGroup.count; i += 3 )
	{
		set< OptFace * >::iterator itr1 = faces.insert( faces.begin(), new OptFace() );
		OptFace *face = (*itr1);
		
		face->verts[0] = &verts[indices[triGroup.first + i] - triGroup.vertRStart];
		face->verts[1] = &verts[indices[triGroup.first + i + 1] - triGroup.vertRStart];
		face->verts[2] = &verts[indices[triGroup.first + i + 2] - triGroup.vertRStart];
		face->verts[0]->faces.insert( face );
		face->verts[1]->faces.insert( face );
		face->verts[2]->faces.insert( face );
	}
	for( unsigned int i = 0; i < verts.size(); ++i )
	{
		verts[i].index = triGroup.vertRStart + i;
		verts[i].updateScore( -1 );
	}

	// Main loop of algorithm
	unsigned int curIndex = triGroup.first;
	
	while( !faces.empty() )
	{
		OptFace *bestFace = 0x0;
		float bestScore = -1.0f;
		
		// Try to find best scoring face in cache
		list< OptVertex * >::iterator itr1 = cache.begin();
		while( itr1 != cache.end() )
		{
			set< OptFace * >::iterator itr2 = (*itr1)->faces.begin();
			while( itr2 != (*itr1)->faces.end() )
			{
				if( (*itr2)->getScore() > bestScore )
				{
					bestFace = *itr2;
					bestScore = bestFace->getScore();
				}
				++itr2;
			}
			++itr1;
		}

		// If that didn't work find it in the complete list of triangles
		if( bestFace == 0x0 )
		{
			set< OptFace * >::iterator itr2 = faces.begin();
			while( itr2 != faces.end() )
			{
				if( (*itr2)->getScore() > bestScore )
				{
					bestFace = (*itr2);
					bestScore = bestFace->getScore();
				}
				++itr2;
			}
		}
		
		// Process vertices of best face
		for( unsigned int i = 0; i < 3; ++i )
		{
			// Add vertex to draw list
			indices[curIndex++] = bestFace->verts[i]->index;

			// Move vertex to head of cache
			itr1 = find( cache.begin(), cache.end(), bestFace->verts[i] );
			if( itr1 != cache.end() ) cache.erase( itr1 );
			cache.push_front( bestFace->verts[i] );

			// Remove face from vertex lists
			bestFace->verts[i]->faces.erase( bestFace );
		}

		// Remove best face
		faces.erase( faces.find( bestFace ) );
		delete bestFace;

		// Update scores of vertices in cache
		unsigned int cacheIndex = 0;
		for( itr1 = cache.begin(); itr1 != cache.end(); ++itr1 )
		{
			(*itr1)->updateScore( cacheIndex++ );
		}

		// Trim cache
		for( int i = (int)cache.size(); i > OptCacheSize; --i )
		{
			cache.pop_back();
		}
	}


	// Remap vertices to make access to them as linear as possible
	vector< Vertex > oldVertices( vertices.begin() + triGroup.vertRStart,
								  vertices.begin() + triGroup.vertREnd + 1 );
	map< unsigned int, unsigned int > mapping;
	unsigned int curVertex = triGroup.vertRStart;
	
	for( unsigned int i = triGroup.first; i < triGroup.first + triGroup.count; ++i )
	{
		map< unsigned int, unsigned int >::iterator itr1 = mapping.find( indices[i] );
		
		if( itr1 == mapping.end() )
		{
			mapping[indices[i]] = curVertex;
			indices[i] = curVertex++;
		}
		else
		{
			indices[i] = itr1->second;
		}
	}

	for( map< unsigned int, unsigned int >::iterator itr1 = mapping.begin();
		 itr1 != mapping.end(); ++itr1 )
	{
		vertices[itr1->second] = oldVertices[itr1->first - triGroup.vertRStart];
	}


	/*// Measure cache efficiency
	int misses = 0;
	list< unsigned int > testCache;
	for( unsigned int i = 0; i < triGroup.count; ++i )
	{
		unsigned int index = indices[triGroup.first + i];
		if( find( testCache.begin(), testCache.end(), index ) == testCache.end() )
		{
			testCache.push_back( index );
			if( testCache.size() > 16 ) testCache.erase( testCache.begin() );
			++misses;
		}
	}
	float efficiency = 1.0f - (float)misses / triGroup.count;*/
}
