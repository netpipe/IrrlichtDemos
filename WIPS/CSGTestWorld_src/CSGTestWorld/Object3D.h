
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef OBJECT3D_H
#define OBJECT3D_H

class Vertex;
class Face;
class Bound;
class VertexSet;
class FaceSet;
class Solid;
class mlVector3D;
class gxColor;
class Segment;

class Object3D
{
public:

	VertexSet * vertices;
	FaceSet * faces;

	Bound * bound;
	
	Object3D();
	Object3D(Solid * solid);
	virtual ~Object3D();
	
	//Object3D * clone()
	
	int getNumFaces();
	Face * getFace(int index);

	Bound & getBound();

	void splitFaces(Object3D * pObject);

	static double TOL;

//private:

	Face * addFace(Vertex * v1, Vertex * v2, Vertex * v3);
	Vertex * addVertex(const mlVector3D & pos, const gxColor & color, int status);

	double computeDistance(Vertex & vertex, Face & face);

	void splitFace(int facePos, Segment & segment1, Segment & segment2);
	  
	void breakFaceInTwo(int facePos, const mlVector3D & newPos, int splitEdge);
	
	void breakFaceInTwo(int facePos, const mlVector3D & newPos, Vertex & endVertex);
	
	void breakFaceInThree(int facePos, const mlVector3D & newPos1, const mlVector3D & newPos2, Vertex & startVertex, Vertex & endVertex);
	void breakFaceInThree(int facePos, const mlVector3D & newPos1, const mlVector3D & newPos2, int splitEdge);
	void breakFaceInThree(int facePos, const mlVector3D & newPos, Vertex & endVertex);
	void breakFaceInThree(int facePos, const mlVector3D & newPos);

	void breakFaceInFour(int facePos, const mlVector3D & newPos1, const mlVector3D & newPos2, Vertex & endVertex);
	
	void breakFaceInFive(int facePos, const mlVector3D & newPos1, const mlVector3D & newPos2, int linedVertex);
	
	void classifyFaces(Object3D & pObject);
	
	void invertInsideFaces();
};

#endif // OBJECT3D_H
