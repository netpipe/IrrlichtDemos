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
		/*
        btVector3 localscale = btVector3(scale.x, scale.y, scale.z);


        ResHandle geomhandle = Horde3D::addResource(ResourceTypes::SceneGraph, modelfile.c_str(), 0);
		Horde3DUtils::loadResourcesFromDisk(GameEngine::get()->getRootDirectory().c_str());


		unsigned int vertexcount = Horde3D::getResourceParami(geomhandle, GeometryResParams::VertexCount);
		float *vertices = new float[vertexcount * 3];
		vertices = (float*)Horde3D::getResourceData(geomhandle, GeometryResParams::VertexData);

		unsigned int indexcount = Horde3D::getResourceParami(geomhandle, GeometryResParams::IndexCount);
		int *indices = new int[indexcount];
		memcpy(indices, Horde3D::getResourceData(geomhandle, GeometryResParams::IndexData), sizeof(int) * indexcount);

		unsigned int tricount = vertexcount * 3;

		for (int i = 0; i < tricount; i++)
		{
			int index0 = indices[i*3];
			int index1 = indices[i*3+1];
			int index2 = indices[i*3+2];

			btVector3 vertex0(vertices[index0*3], vertices[index0*3+1], vertices[index0*3+2]);
 			btVector3 vertex1(vertices[index1*3], vertices[index1*3+1], vertices[index1*3+2]);
 			btVector3 vertex2(vertices[index2*3], vertices[index2*3+1], vertices[index2*3+2]);

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
                convexshape->addPoint(btPoint3(hull->getVertexPointer()[j]));
           	}

            delete tmpshape;
            delete hull;
        }
        else
        {
            shape = new btBvhTriangleMeshShape(trimesh, true);
        }*/





        /// WAVEFRONT ONLY ( NO IRRLICHT) ( WORKING )
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
