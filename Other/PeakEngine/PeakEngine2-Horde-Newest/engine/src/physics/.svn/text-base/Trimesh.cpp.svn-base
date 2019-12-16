/*
Copyright (C) 2008   Christian Reiser, Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "physics/Trimesh.h"
#include "core/FileSystem.h"
#include "core/Logger.h"
#include "core/GameEngine.h"
#include "graphics/GraphicsEngine.h"


#include "support/cd_wavefront.h"
#include <Horde3D.h>
#include <Horde3DUtils.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>

#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;
typedef unsigned int uint32;

namespace peak
{

    Trimesh::Trimesh()
    {
    }

    Trimesh::~Trimesh()
    {
    }

    void Trimesh::create(std::string modelfile, float mass, Vector3D scale)
    {
        trimesh = new btTriangleMesh();
        std::string extension = modelfile.c_str() + modelfile.size() - 3;
        if (extension == std::string("geo"))
        {
        
		    // MANUAL .GEO PARSING
		    char* buffer;
			unsigned int length;
			std::string filename = FileSystem::get()->getFileExists(std::string("models/") + modelfile);
			// Open File
			ifstream is(filename.c_str() , ios::binary);
			if (!is)
			{
				cerr << "File is not existing.." << endl;
				return;
			}
			// Get length of file
			is.seekg (0, ios::end);
			length = is.tellg();
			is.seekg (0, ios::beg);
			// Allocate memory
			buffer = new char [length];
			// Read data as a block
			is.read (buffer,length);
			is.close();
			// Write filesize
			cout << "Filesize: " << length * 0.0009765625 << " Kilobyte" << endl;

			char* cur = (char*)buffer;
	
			// Parse file
			// Check header and version
			char id[4];
			memcpy( &id, cur, 4 ); cur += 4;
			if( id[0] != 'H' || id[1] != '3' || id[2] != 'D' || id[3] != 'G' )
			{
				cout << "Couldn't find header!" << endl;
				cout << "Header: ";
				cout.write(id, 4);
				cout << endl;
				return;
			}
			cout << "Header: ";
			cout.write(id, 4);
			cout << endl;
		
			uint32 version;
			memcpy( &version, cur, sizeof( uint32 ) ); cur += sizeof( uint32 );
			if( version != 5 )
			{
				cout << "Couldn't determine version!" << endl;
				cout << "Version: " << version << endl;
				return;
			}
			cout << "Version: " << version << endl;
	
			cout << "Parsed header and version." << endl;
	
			// Skip joints
			cout << "Parsing joints.." << endl;
			uint32 count;
			memcpy( &count, cur, sizeof( uint32 ) ); cur += sizeof( uint32 );
			cout << "Joint Count: " << count << endl;

			for( uint32 i = 0; i < count; ++i )
			{
				// Inverse bind matrix
				for( uint32 j = 0; j < 16; ++j )
				{

					cur += sizeof( float );
				}
			}
			cout << "Parsed joints." << endl;
	
			cout << "Parsing vertices.." << endl;
			Vector3D* vertexdata;
			int vertexcount;
	
	
			// Load vertex stream data
			uint32 streamsize;
			memcpy( &count, cur, sizeof( uint32 ) ); cur += sizeof( uint32 );			// Number of streams
			memcpy( &streamsize, cur, sizeof( uint32 ) ); cur += sizeof( uint32 );		// Number of vertices
	
			cout << "Streamcount: " << count << endl << "Vertexcount: " << streamsize << endl;

			vertexcount = streamsize;
			vertexdata = new Vector3D[streamsize];
	
			for (uint32 i = 0; i < count; ++i)
			{
				uint32 streamid, streamelementsize;
				memcpy( &streamid, cur, sizeof( uint32 ) ); cur += sizeof( uint32 );
				memcpy( &streamelementsize, cur, sizeof( uint32 ) ); cur += sizeof( uint32 );
		
		
				switch( streamid )
				{
				case 0:		// Position
					if( streamelementsize != 12 )  cout << "Invalid position base stream" << endl;

					for (uint32 j = 0 ; j < streamsize; ++j)
					{
						// float temp = ((float)j / (float)vertexcount) * 100.0f;
						// cout << "Parsing vertices... " << (float)temp << "%" << endl;
						memcpy( &vertexdata[j].x, cur, sizeof( float ) ); cur += sizeof( float );
						memcpy( &vertexdata[j].y, cur, sizeof( float ) ); cur += sizeof( float );
						memcpy( &vertexdata[j].z, cur, sizeof( float ) ); cur += sizeof( float );
					}
					break;
				case 1:		// Normal
					if( streamelementsize != 6 )  cout << "Invalid normal base stream" << endl;
					for( uint32 j = 0; j < streamsize; ++j )
					{
						cur += sizeof( short ) * 3;
					}
					break;
				case 2:		// Tangent
					if( streamelementsize != 6 ) cout << "Invalid tangent base stream" << endl;
					for( uint32 j = 0; j < streamsize; ++j )
					{
						cur += sizeof( short ) * 3;
					}
					break;
				case 3:		// Bitangent
					if( streamelementsize != 6 ) cout << "Invalid bitangent base stream" << endl;
					for( uint32 j = 0; j < streamsize; ++j )
					{
						cur += sizeof( short ) * 3;
					}
					break;
				case 4:		// Joint indices
					if( streamelementsize != 4 ) cout << "Invalid joint stream" << endl;
					for( uint32 j = 0; j < streamsize; ++j )
					{
						cur += sizeof( char ) * 4;
					}
					break;
				case 5:		// Weights
					if( streamelementsize != 4 ) cout << "Invalid weight stream" << endl;
					for( uint32 j = 0; j < streamsize; ++j )
					{
						cur += sizeof( char ) * 4;
					}
					break;
				case 6:		// Texture Coord Set 1
					if( streamelementsize != 8 ) cout << "Invalid texCoord1 stream" << endl;
					for( uint32 j = 0; j < streamsize; ++j )
					{
						cur += sizeof( float ) * 2;
					}
					break;
				case 7:		// Texture Coord Set 2
					if( streamelementsize != 8 ) cout << "Invalid texCoord2 stream" << endl;
					for( uint32 j = 0; j < streamsize; ++j )
					{
						cur += sizeof( float ) * 2;
					}
					break;
				default:
					cur += streamelementsize * streamsize;
					continue;
				}
			}
	
			cout << "Parsed vertices." << endl;
	
			cout << "Parsing indices.." << endl;
			// Load triangle indices
			memcpy( &count, cur, sizeof( uint32 ) ); cur += sizeof( uint32 );
	
			int indexcount = count;
			cout << "Index count: " << indexcount << endl;
			cout << "Triangle count: " << indexcount / 3 << endl;
			int *indexdata = new int[indexcount];

			for( uint32 i = 0; i < count; ++i )
			{
				memcpy( &indexdata[i], cur, sizeof( uint32 ) ); cur += sizeof( uint32 );
			}
	
			cout << "Parsed indices." << endl;
		
			delete[] buffer;
		
			btVector3 localscale = btVector3(scale.x, scale.y, scale.z);
		
			// Bullet needs Wavefront coordinate systems
			for (int i = 0; i < vertexcount; i++)
			{
				vertexdata[i].rotateYZ(3.14159265 / 2);
			}
		
			for (int i = 0; i < indexcount / 3; i++)
			{
				int index0 = indexdata[i*3];
				int index1 = indexdata[i*3+1];
				int index2 = indexdata[i*3+2];
			
				btVector3 vertex0(vertexdata[index0*3].x, vertexdata[index0*3].y, vertexdata[index0*3].z);
				btVector3 vertex1(vertexdata[index1*3].x, vertexdata[index1*3].y, vertexdata[index1*3].z);
				btVector3 vertex2(vertexdata[index2*3].x, vertexdata[index2*3].y, vertexdata[index2*3].z);
			
				vertex0 *= localscale;
				vertex1 *= localscale;
				vertex2 *= localscale;
			
				trimesh->addTriangle(vertex0,vertex1,vertex2);
			}
			delete[] indexdata;
			delete[] vertexdata;
			if (mass != 0.0f)
			{
				btConvexShape *tmpshape = new btConvexTriangleMeshShape(trimesh);
				btShapeHull *hull = new btShapeHull(tmpshape);
				btScalar margin = tmpshape->getMargin();
				hull->buildHull(margin);
				tmpshape->setUserPointer(hull);
			
				shape = new btConvexHullShape();
				btConvexHullShape *convexshape = (btConvexHullShape*)(shape);
				for (int j = 0; j < hull->numVertices() ; j++)
				{
					convexshape->addPoint(btVector3(hull->getVertexPointer()[j]));
				}
			
				LDEBUG("Dynamic convex shape was created.\n");
			
				delete tmpshape;
				delete hull;
			}	
			else
			{
				shape = new btBvhTriangleMeshShape(trimesh, true);
				LDEBUG("Static concave shape was created.\n");
			}
		}
		else if(extension == std::string("obj"))
		{
		    /// WAVEFRONT ONLY ( NO HORDE3D) ( WORKING )
		    btVector3 localscale = btVector3(scale.x, scale.y, scale.z);
		    // Is the file existing?
		    std::string filename = FileSystem::get()->getFileExists(std::string("models/") + modelfile);

		    /// Load the wavefront file
		    ConvexDecomposition::WavefrontObj file;
		    file.loadObj(filename.c_str());


		    // Store the triangle data from the modelfile in the triangle shape
		    if ( file.mTriCount )
		    {
		        for (int i = 0; i < file.mTriCount; i++)
		        {
		            int index0 = file.mIndices[i*3];
		            int index1 = file.mIndices[i*3+1];
		            int index2 = file.mIndices[i*3+2];

		            btVector3 vertex0(file.mVertices[index0*3], file.mVertices[index0*3+1],file.mVertices[index0*3+2]);
		            btVector3 vertex1(file.mVertices[index1*3], file.mVertices[index1*3+1],file.mVertices[index1*3+2]);
		            btVector3 vertex2(file.mVertices[index2*3], file.mVertices[index2*3+1],file.mVertices[index2*3+2]);

		            vertex0 *= localscale;
		            vertex1 *= localscale;
		            vertex2 *= localscale;

		            trimesh->addTriangle(vertex0,vertex1,vertex2);
		        }
				if (mass != 0.0f)
				{
		        	btConvexShape *tmpshape = new btConvexTriangleMeshShape(trimesh);
		        	btShapeHull *hull = new btShapeHull(tmpshape);
		        	btScalar margin = tmpshape->getMargin();
		        	hull->buildHull(margin);
		        	tmpshape->setUserPointer(hull);

		        	shape = new btConvexHullShape();
		        	btConvexHullShape *convexshape = (btConvexHullShape*)(shape);
		        	for (int j = 0; j < hull->numVertices() ; j++)
		        	{
		            	convexshape->addPoint(btVector3(hull->getVertexPointer()[j]));
		       		}

		       		LDEBUG("Dynamic convex shape was created.\n");

		        	delete tmpshape;
		        	delete hull;
		        }
		        else
		        {
		        	shape = new btBvhTriangleMeshShape(trimesh, true);
		        	LDEBUG("Static concave shape was created.\n");
		        }
		    }
		}
		else
		{
			LERROR("Incorrect file ending.");
		}



        /// This piece of code is correct, no question...
        transform = new btTransform();
        transform->setIdentity();
        transform->setOrigin(btVector3(0, 0, 0));


        inertia = Vector3D(0, 0, 0);
        if( mass != 0.0f)
        {
            btVector3 in;
            shape->calculateLocalInertia(mass, in);
            inertia = Vector3D(in.x(), in.y(), in.z());
        }
        this->mass = mass;



    }

    bool Trimesh::destroy()
    {
        delete trimesh;
        return true;
    }


}
