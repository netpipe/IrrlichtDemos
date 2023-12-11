
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "Object3D.h"

#include "Solid.h"
#include "VectorSet.h"
#include "IntSet.h"
#include "ColorSet.h"
#include "Vertex.h"
#include "VertexSet.h"
#include "Face.h"
#include "FaceSet.h"
#include "Bound.h"

#include "Line.h"
#include "Segment.h"

#include <math.h>

#include "DiagnosticTool.h"
#include "DiagnosticEvent.h"

extern DiagnosticTool * g_pTool;

//double Object3D::TOL = 1e-10f;
double Object3D::TOL = 0.5e-5f;

Object3D::Object3D()
{
	vertices = 0;
	faces = 0;
	bound = 0;
}

Object3D::Object3D(Solid * solid)
{
	Vertex * v1 = 0;
	Vertex * v2 = 0;
	Vertex * v3 = 0;
	Vertex * vertex = 0;

	VectorSet & verticesPoints = *solid->getVertices();
	IntSet & indices = *solid->getIndices();
	ColorSet & colors = *solid->getColors();
	//VertexPointerSet & verticesTemp = *(new VertexPointerSet());
	VertexPointerSet * verticesTemp = new VertexPointerSet();
	
	//create vertices
	vertices = new VertexSet();
	for(int i=0;i<verticesPoints.length();i++)
	{
		vertex = addVertex(verticesPoints[i], colors[i], Vertex::UNKNOWN);
		verticesTemp->add(vertex); 
		//verticesTemp.add(vertex); 
	}

	//create faces
	faces = new FaceSet();
	for(int i=0; i<indices.length(); i=i+3)
	{
		//v1 = verticesTemp.GetVertexPtr(indices[i]);
		//v2 = verticesTemp.GetVertexPtr(indices[i+1]);
		//v3 = verticesTemp.GetVertexPtr(indices[i+2]);
		v1 = verticesTemp->GetVertexPtr(indices[i]);
		v2 = verticesTemp->GetVertexPtr(indices[i+1]);
		v3 = verticesTemp->GetVertexPtr(indices[i+2]);
		addFace(v1, v2, v3);
	}
	
	//create bound
	bound = new Bound(&verticesPoints);

	delete verticesTemp;
}

Object3D::~Object3D()
{
	printf("Deleting vertex set\n");
	delete vertices;
	printf("Deleting face set\n");
	delete faces;
	printf("Deleting bound\n");
	delete bound;
}

//--------------------------------------GETS------------------------------------//

/**
 * Gets the number of faces
 * 
 * @return number of faces
 */
int Object3D::getNumFaces()
{
	return faces->GetSize();
}

/**
 * Gets a face reference for a given position
 * 
 * @param index required face position
 * @return face reference , null if the position is invalid
 */
Face * Object3D::getFace(int index)
{
	if(index<0 || index>=faces->GetSize())
	{
		return 0;
	}
	else
	{
		return faces->GetFace(index);
	}
}

/**
 * Gets the solid bound
 * 
 * @return solid bound
 */
Bound & Object3D::getBound()
{
	return *bound;
}

//------------------------------------ADDS----------------------------------------//

// There may be missing functions in the container classes.
	
/**
 * Method used to add a face properly for internal methods
 * 
 * @param v1 a face vertex
 * @param v2 a face vertex
 * @param v3 a face vertex
 */
Face * Object3D::addFace(Vertex * v1, Vertex * v2, Vertex * v3)
{
	if(!(v1->equals(v2)||v1->equals(v3)||v2->equals(v3)))
	{
		Face face(v1, v2, v3);
		//if(face.getArea()>TOL)
		// Tried removing this check, just for fun.
		// I should probably use double precision for everything.
		if(true)
		{
			Face * pAddedFace = (*faces).AddFace(face);
			return pAddedFace;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

/**
 * Method used to add a vertex properly for internal methods
 * 
 * @param pos vertex position
 * @param color vertex color
 * @param status vertex status
 * @return the vertex inserted (if a similar vertex already exists, this is returned)
 */
Vertex * Object3D::addVertex(const mlVector3D & pos, const gxColor & color, int status)
{
	int i;
	//if already there is an equal vertex, it is not inserted
	Vertex vertex(pos, color, status);

	//for(i=0;i<vertices.size();i++)
	for(i=0;i<vertices->length();i++)
	{
		if(vertex.equals(vertices->GetVertex(i))) break;			
	}

	if(i==vertices->length())
	{
		Vertex * pVertexInList = 0;

		pVertexInList = vertices->AddVertex(vertex);
		return pVertexInList;

		//return vertex;
	}
	else
	{
		Vertex * pVertexInList = 0;

		pVertexInList = vertices->GetVertex(i);
		pVertexInList->setStatus(status);

		return pVertexInList;
	}

	return 0;
	
}

//-------------------------FACES_SPLITTING_METHODS------------------------------//

/**
 * Split faces so that none face is intercepted by a face of other object
 * 
 * @param object the other object 3d used to make the split 
 */
void Object3D::splitFaces(Object3D * object)
{
	Line line;
	Face * face1;
	Face * face2;

	//Segment segments[], segment1, segment2;

	Segment segment1, segment2;

	double distFace1Vert1, distFace1Vert2, distFace1Vert3, distFace2Vert1, distFace2Vert2, distFace2Vert3;
	int signFace1Vert1, signFace1Vert2, signFace1Vert3, signFace2Vert1, signFace2Vert2, signFace2Vert3;
	int numFacesBefore = getNumFaces();
	int numFacesStart = getNumFaces();
	int facesIgnored = 0;
					
	//if the objects bounds overlap...								
	if(getBound().overlap(object->getBound()))
	{			
		//for each object1 face...
		for(int i=0;i<getNumFaces();i++)
		{
			//if object1 face bound and object2 bound overlap ...
			face1 = getFace(i);
			Face face1Orig; face1Orig.CloneFrom(face1);
			
			if(face1->getBound().overlap(object->getBound()))
			{
				//for each object2 face...
				for(int j=0;j<object->getNumFaces();j++)
				{
					//if object1 face bound and object2 face bound overlap...  
					face2 = object->getFace(j);
					Face face2Orig; face2Orig.CloneFrom(face2);

					if(false)
					{
						DiagnosticEvent * pEvent = g_pTool->AddEvent();
						if(pEvent)
						{
							pEvent->AddTriangle(face1->v1->getPosition(), face1->v2->getPosition(), face1->v3->getPosition(), gxColorWhite);
							pEvent->AddTriangle(face2->v1->getPosition(), face2->v2->getPosition(), face2->v3->getPosition(), gxColorBlue);
							pEvent->SetObjects(this, object);
							pEvent->SetText("Before cut Face %d x %d", i,j);
						}
					}

					if(face1->getBound().overlap(face2->getBound()))
					{
						//PART I - DO TWO POLIGONS INTERSECT?
						//POSSIBLE RESULTS: INTERSECT, NOT_INTERSECT, COPLANAR
						
						//distance from the face1 vertices to the face2 plane
						distFace1Vert1 = computeDistance(*(face1->v1), *face2);
						distFace1Vert2 = computeDistance(*(face1->v2), *face2);
						distFace1Vert3 = computeDistance(*(face1->v3), *face2);
						
						//distances signs from the face1 vertices to the face2 plane 
						signFace1Vert1 = (distFace1Vert1>TOL? 1 :(distFace1Vert1<-TOL? -1 : 0)); 
						signFace1Vert2 = (distFace1Vert2>TOL? 1 :(distFace1Vert2<-TOL? -1 : 0));
						signFace1Vert3 = (distFace1Vert3>TOL? 1 :(distFace1Vert3<-TOL? -1 : 0));
						
						//if all the signs are zero, the planes are coplanar
						//if all the signs are positive or negative, the planes do not intersect
						//if the signs are not equal...
						if (!(signFace1Vert1==signFace1Vert2 && signFace1Vert2==signFace1Vert3))
						{
							//distance from the face2 vertices to the face1 plane
							distFace2Vert1 = computeDistance(*(face2->v1), *face1);
							distFace2Vert2 = computeDistance(*(face2->v2), *face1);
							distFace2Vert3 = computeDistance(*(face2->v3), *face1);
							
							//distances signs from the face2 vertices to the face1 plane
							signFace2Vert1 = (distFace2Vert1>TOL? 1 :(distFace2Vert1<-TOL? -1 : 0)); 
							signFace2Vert2 = (distFace2Vert2>TOL? 1 :(distFace2Vert2<-TOL? -1 : 0));
							signFace2Vert3 = (distFace2Vert3>TOL? 1 :(distFace2Vert3<-TOL? -1 : 0));
						
							//if the signs are not equal...
							if (!(signFace2Vert1==signFace2Vert2 && signFace2Vert2==signFace2Vert3))
							{
								line = Line(face1, face2);
						
								//intersection of the face1 and the plane of face2
								segment1 = Segment(line, *face1, signFace1Vert1, signFace1Vert2, signFace1Vert3);
																
								//intersection of the face2 and the plane of face1
								segment2 = Segment(line, *face2, signFace2Vert1, signFace2Vert2, signFace2Vert3);
															
								//if the two segments intersect...
								if(segment1.intersect(segment2))
								{
									//PART II - SUBDIVIDING NON-COPLANAR POLYGONS
									int lastNumFaces = getNumFaces();
									this->splitFace(i, segment1, segment2);
																		
									//prevent from infinite loop (with a loss of faces...)
									if(numFacesStart*20<getNumFaces())
									{
										//System.out.println("possible infinite loop situation: terminating faces split");
										//return;
									}
							
									// "if the face in the position isn't the same, there was a break"

									//if(face1!=getFace(i)) 
									if(!(face1->equals_pointersmatch(&face1Orig)))
									{
										if(true)
										{
											DiagnosticEvent * pEvent = g_pTool->AddEvent();
											if(pEvent)
											{
												//pEvent->AddTriangle(face1->v1->getPosition(), face1->v2->getPosition(), face1->v3->getPosition(), gxColorWhite);
												//pEvent->AddTriangle(face2->v1->getPosition(), face2->v2->getPosition(), face2->v3->getPosition(), gxColorBlue);
												//pEvent->SetObjects(this, object);
												pEvent->SetObjects(this, 0);
												//pEvent->SetObjectWithHighlightedTriangle(this, i);
												pEvent->SetText("Face %d was cut with %d", i,j);
											}
										}

										//if the generated solid is equal the origin...
										//if(face1->equals(getFace(getNumFaces()-1)))
										if(face1Orig.equals(getFace(getNumFaces()-1)))
										{
											//return it to its position and jump it
											if(i!=(getNumFaces()-1))
											{
												// Add a before and after diagnostic event.

												faces->RemoveFace(getNumFaces()-1);
												//faces->InsertFace(i, *face1);
												faces->InsertFace(i, face1Orig);
											}
											else
											{
												continue;
											}
										}
										//else: test next face
										// This is because the whole face list was shunted down when the current face was removed, so
										// the next face is the face at the current position in the list.
										else
										{
											i--;
											break;
										}
									}
								}
							}
						}
					}

					if(false)
					{
						DiagnosticEvent * pEvent = g_pTool->AddEvent();
						if(pEvent)
						{
							//pEvent->AddTriangle(face1->v1->getPosition(), face1->v2->getPosition(), face1->v3->getPosition(), gxColorWhite);
							//pEvent->AddTriangle(face2->v1->getPosition(), face2->v2->getPosition(), face2->v3->getPosition(), gxColorBlue);
							//pEvent->SetObjects(this, object);
							pEvent->SetObjects(this, 0);
							//pEvent->SetObjectWithHighlightedTriangle(this, i);
							pEvent->SetText("After cut Face %d x %d", i,j);
						}
					}
				}
			}
		}
	}

	extern int g_CSGDiagToolNumEventAdds; 

	printf("Num faces at start = %d\n", numFacesStart);
	printf("Num faces at end   = %d\n", getNumFaces());

	if(true)
	{
		for(int i = 0; i < getNumFaces(); i++)
		{
			DiagnosticEvent * pEvent = g_pTool->AddEvent();
			if(pEvent)
			{
				pEvent->SetObjectWithHighlightedTriangle(this, i);
				pEvent->SetText("After splitFaces - Face %d of %d", i+1, getNumFaces());
			}
		}
	}

	printf("\nNum events = %d\n", g_CSGDiagToolNumEventAdds);
}

/**
 * Computes closest distance from a vertex to a plane
 * 
 * @param vertex vertex used to compute the distance
 * @param face face representing the plane where it is contained
 * @return the closest distance from the vertex to the plane
 */
double Object3D::computeDistance(Vertex & vertex, Face & face)
{
	mlVector3D normal = face.getNormal();
	double a = normal.x;
	double b = normal.y;
	double c = normal.z;
	double d = -(a*face.v1->x + b*face.v1->y + c*face.v1->z);
	return a*vertex.x + b*vertex.y + c*vertex.z + d;
}

/**
 * Split an individual face
 * 
 * @param facePos face position on the array of faces
 * @param segment1 segment representing the intersection of the face with the plane
 * of another face
 * @return segment2 segment representing the intersection of other face with the
 * plane of the current face plane
 */	  
void Object3D::splitFace(int facePos, Segment & segment1, Segment & segment2)
{
	Vertex startPosVertex, endPosVertex;
	mlVector3D startPos, endPos;
	int startType, endType, middleType;
	double startDist, endDist;
	
	Face & face = *getFace(facePos);
	Vertex * startVertex = segment1.getStartVertex();
	Vertex * endVertex = segment1.getEndVertex();
	
	//starting point: deeper starting point 		
	if (segment2.getStartDistance() > segment1.getStartDistance()+TOL)
	{
		startDist = segment2.getStartDistance();
		startType = segment1.getIntermediateType();
		startPos = segment2.getStartPosition();
	}
	else
	{
		startDist = segment1.getStartDistance();
		startType = segment1.getStartType();
		startPos = segment1.getStartPosition();
	}
	
	//ending point: deepest ending point
	if (segment2.getEndDistance() < segment1.getEndDistance()-TOL)
	{
		endDist = segment2.getEndDistance();
		endType = segment1.getIntermediateType();
		endPos = segment2.getEndPosition();
	}
	else
	{
		endDist = segment1.getEndDistance();
		endType = segment1.getEndType();
		endPos = segment1.getEndPosition();
	}		
	middleType = segment1.getIntermediateType();
	
	//set vertex to BOUNDARY if it is start type		
	if (startType == Segment::VERTEX)
	{
		startVertex->setStatus(Vertex::BOUNDARY);
	}
			
	//set vertex to BOUNDARY if it is end type
	if (endType == Segment::VERTEX)
	{
		endVertex->setStatus(Vertex::BOUNDARY);
	}
	
	//VERTEX-_______-VERTEX 
	if (startType == Segment::VERTEX && endType == Segment::VERTEX)
	{
		return;
	}
	
	//______-EDGE-______
	else if (middleType == Segment::EDGE)
	{
		//gets the edge 
		int splitEdge;
		if ((startVertex == face.v1 && endVertex == face.v2) || (startVertex == face.v2 && endVertex == face.v1))
		{
			splitEdge = 1;
		}
		else if ((startVertex == face.v2 && endVertex == face.v3) || (startVertex == face.v3 && endVertex == face.v2))
		{	  
			splitEdge = 2; 
		} 
		else
		{
			splitEdge = 3;
		} 
		
		//VERTEX-EDGE-EDGE
		if (startType == Segment::VERTEX)
		{
			breakFaceInTwo(facePos, endPos, splitEdge);
			return;
		}
		
		//EDGE-EDGE-VERTEX
		else if (endType == Segment::VERTEX)
		{
			breakFaceInTwo(facePos, startPos, splitEdge);
			return;
		}
    
		// EDGE-EDGE-EDGE
		else if (startDist == endDist)
		{
			breakFaceInTwo(facePos, endPos, splitEdge);
		}
		else
		{
			if((startVertex == face.v1 && endVertex == face.v2) || (startVertex == face.v2 && endVertex == face.v3) || (startVertex == face.v3 && endVertex == face.v1))
			{
				breakFaceInThree(facePos, startPos, endPos, splitEdge);
			}
			else
			{
				breakFaceInThree(facePos, endPos, startPos, splitEdge);
			}
		}
		return;
	}
	
	//______-FACE-______
	
	//VERTEX-FACE-EDGE
	else if (startType == Segment::VERTEX && endType == Segment::EDGE)
	{
		breakFaceInTwo(facePos, endPos, *endVertex);
	}
	//EDGE-FACE-VERTEX
	else if (startType == Segment::EDGE && endType == Segment::VERTEX)
	{
		breakFaceInTwo(facePos, startPos, *startVertex);
	}
	//VERTEX-FACE-FACE
	else if (startType == Segment::VERTEX && endType == Segment::FACE)
	{
		breakFaceInThree(facePos, endPos, *startVertex);
	}
	//FACE-FACE-VERTEX
	else if (startType == Segment::FACE && endType == Segment::VERTEX)
	{
		breakFaceInThree(facePos, startPos, *endVertex);
	}
	//EDGE-FACE-EDGE
	else if (startType == Segment::EDGE && endType == Segment::EDGE)
	{
		breakFaceInThree(facePos, startPos, endPos, *startVertex, *endVertex);
	}
	//EDGE-FACE-FACE
	else if (startType == Segment::EDGE && endType == Segment::FACE)
	{
		breakFaceInFour(facePos, startPos, endPos, *startVertex);
	}
	//FACE-FACE-EDGE
	else if (startType == Segment::FACE && endType == Segment::EDGE)
	{
		breakFaceInFour(facePos, endPos, startPos, *endVertex);
	}
	//FACE-FACE-FACE
	else if (startType == Segment::FACE && endType == Segment::FACE)
	{
		mlVector3D segmentVector(startPos.x-endPos.x, startPos.y-endPos.y, startPos.z-endPos.z);
					
		//if the intersection segment is a point only...
		if (fabs(segmentVector.x)<TOL && fabs(segmentVector.y)<TOL && fabs(segmentVector.z)<TOL)
		{
			breakFaceInThree(facePos, startPos);
			return;
		}
		
		//gets the vertex more lined with the intersection segment
		int linedVertex;
		mlVector3D linedVertexPos;
		mlVector3D vertexVector(endPos.x-face.v1->x, endPos.y-face.v1->y, endPos.z-face.v1->z);
		vertexVector.Normalise();
		//double dot1 = fabs(segmentVector.dot(vertexVector));
		double dot1 = fabs(segmentVector * vertexVector);
		vertexVector = mlVector3D(endPos.x-face.v2->x, endPos.y-face.v2->y, endPos.z-face.v2->z);
		vertexVector.Normalise();
		//double dot2 = fabs(segmentVector.dot(vertexVector));
		double dot2 = fabs(segmentVector * vertexVector);
		vertexVector = mlVector3D(endPos.x-face.v3->x, endPos.y-face.v3->y, endPos.z-face.v3->z);
		vertexVector.Normalise();
		//double dot3 = fabs(segmentVector.dot(vertexVector));
		double dot3 = fabs(segmentVector * vertexVector);
		if (dot1 > dot2 && dot1 > dot3)
		{
		 	linedVertex = 1;
			linedVertexPos = face.v1->getPosition();
		}
		else if (dot2 > dot3 && dot2 > dot1)
		{
			linedVertex = 2;
			linedVertexPos = face.v2->getPosition();
		}
		else
		{
			linedVertex = 3;
			linedVertexPos = face.v3->getPosition();
		}
    
		// Now find which of the intersection endpoints is nearest to that vertex.
		if ((linedVertexPos - startPos).Magnitude() > (linedVertexPos - endPos).Magnitude())
		{
			breakFaceInFive(facePos, startPos, endPos, linedVertex);
		}
		else
		{
			breakFaceInFive(facePos, endPos, startPos, linedVertex);
		}
	}
}

/**
 * Face breaker for VERTEX-EDGE-EDGE / EDGE-EDGE-VERTEX
 * 
 * @param facePos face position on the faces array
 * @param newPos new vertex position
 * @param edge that will be split 
 */		
void Object3D::breakFaceInTwo(int facePos, const mlVector3D & newPos, int splitEdge)
{
	Face & face = *faces->GetFace(facePos);
	//faces->RemoveFace(facePos);
	
	Vertex * vertex = addVertex(newPos, face.v1->getColor(), Vertex::BOUNDARY);
					
	if (splitEdge == 1)
	{
		addFace(face.v1, vertex, face.v3);
		addFace(vertex, face.v2, face.v3);
	}
	else if (splitEdge == 2)
	{
		addFace(face.v2, vertex, face.v1);
		addFace(vertex, face.v3, face.v1);
	}
	else
	{
		addFace(face.v3, vertex, face.v2);
		addFace(vertex, face.v1, face.v2);
	}

	faces->RemoveFace(facePos);
}

/**
 * Face breaker for VERTEX-FACE-EDGE / EDGE-FACE-VERTEX
 * 
 * @param facePos face position on the faces array
 * @param newPos new vertex position
 * @param endVertex vertex used for splitting 
 */		
void Object3D::breakFaceInTwo(int facePos, const mlVector3D & newPos, Vertex & endVertex)
{
	Face & face = *faces->GetFace(facePos);
	// should I try putting it back???

	//faces->RemoveFace(facePos);

	
	Vertex * vertex = addVertex(newPos, face.v1->getColor(), Vertex::BOUNDARY);
				
	if (endVertex.equals(face.v1))
	{
		addFace(face.v1, vertex, face.v3);
		addFace(vertex, face.v2, face.v3);
	}
	else if (endVertex.equals(face.v2))
	{
		addFace(face.v2, vertex, face.v1);
		addFace(vertex, face.v3, face.v1);
	}
	else
	{
		addFace(face.v3, vertex, face.v2);
		addFace(vertex, face.v1, face.v2);
	}

	faces->RemoveFace(facePos);
}

/**
 * Face breaker for EDGE-EDGE-EDGE
 * 
 * @param facePos face position on the faces array
 * @param newPos1 new vertex position
 * @param newPos2 new vertex position 
 * @param splitEdge edge that will be split
 */
void Object3D::breakFaceInThree(int facePos, const mlVector3D & newPos1, const mlVector3D & newPos2, int splitEdge)
{
	Face & face = *(faces->get(facePos));
	// Moved remove to the end
	//faces->remove(facePos);
	
	Vertex * vertex1 = addVertex(newPos1, face.v1->getColor(), Vertex::BOUNDARY);	
	Vertex * vertex2 = addVertex(newPos2, face.v1->getColor(), Vertex::BOUNDARY);
					
	if (splitEdge == 1)
	{
		addFace(face.v1, vertex1, face.v3);
		addFace(vertex1, vertex2, face.v3);
		addFace(vertex2, face.v2, face.v3);
	}
	else if (splitEdge == 2)
	{
		addFace(face.v2, vertex1, face.v1);
		addFace(vertex1, vertex2, face.v1);
		addFace(vertex2, face.v3, face.v1);
	}
	else
	{
		addFace(face.v3, vertex1, face.v2);
		addFace(vertex1, vertex2, face.v2);
		addFace(vertex2, face.v1, face.v2);
	}

	// Moved this
	faces->remove(facePos);
}

/**
 * Face breaker for VERTEX-FACE-FACE / FACE-FACE-VERTEX
 * 
 * @param facePos face position on the faces array
 * @param newPos new vertex position
 * @param endVertex vertex used for the split
 */
void Object3D::breakFaceInThree(int facePos, const mlVector3D & newPos, Vertex & endVertex)
{
	Face & face = *(faces->get(facePos));
	// Moved remove to the end
	//faces->remove(facePos);
	
	Vertex * vertex = addVertex(newPos, face.v1->getColor(), Vertex::BOUNDARY);
					
	if (endVertex.equals(face.v1))
	{
		addFace(face.v1, face.v2, vertex);
		addFace(face.v2, face.v3, vertex);
		addFace(face.v3, face.v1, vertex);
	}
	else if (endVertex.equals(face.v2))
	{
		addFace(face.v2, face.v3, vertex);
		addFace(face.v3, face.v1, vertex);
		addFace(face.v1, face.v2, vertex);
	}
	else
	{
		addFace(face.v3, face.v1, vertex);
		addFace(face.v1, face.v2, vertex);
		addFace(face.v2, face.v3, vertex);
	}

	// Moved this
	faces->remove(facePos);
}

// General translation rules:
// Use references in function parameters as opposed to pointers
// Would use pointers everywhere, but access is through dot operator
// 

/**
 * Face breaker for EDGE-FACE-EDGE
 * 
 * @param facePos face position on the faces array
 * @param newPos1 new vertex position
 * @param newPos2 new vertex position 
 * @param startVertex vertex used the new faces creation
 * @param endVertex vertex used for the new faces creation
 */
void Object3D::breakFaceInThree(int facePos, const mlVector3D & newPos1, const mlVector3D & newPos2, Vertex & startVertex, Vertex & endVertex)
{
	Face & face = *(faces->get(facePos));
	// Moved remove to the end
	//faces->remove(facePos);
	
	Vertex * vertex1 = addVertex(newPos1, face.v1->getColor(), Vertex::BOUNDARY);
	Vertex * vertex2 = addVertex(newPos2, face.v1->getColor(), Vertex::BOUNDARY);
					
	if (startVertex.equals(face.v1) && endVertex.equals(face.v2))
	{
		addFace(face.v1, vertex1, vertex2);
		addFace(face.v1, vertex2, face.v3);
		addFace(vertex1, face.v2, vertex2);
	}
	else if (startVertex.equals(face.v2) && endVertex.equals(face.v1))
	{
		addFace(face.v1, vertex2, vertex1);
		addFace(face.v1, vertex1, face.v3);
		addFace(vertex2, face.v2, vertex1);
	}
	else if (startVertex.equals(face.v2) && endVertex.equals(face.v3))
	{
		addFace(face.v2, vertex1, vertex2);
		addFace(face.v2, vertex2, face.v1);
		addFace(vertex1, face.v3, vertex2);
	}
	else if (startVertex.equals(face.v3) && endVertex.equals(face.v2))
	{
		addFace(face.v2, vertex2, vertex1);
		addFace(face.v2, vertex1, face.v1);
		addFace(vertex2, face.v3, vertex1);
	}
	else if (startVertex.equals(face.v3) && endVertex.equals(face.v1))
	{
		addFace(face.v3, vertex1, vertex2);
		addFace(face.v3, vertex2, face.v2);
		addFace(vertex1, face.v1, vertex2);
	}
	else
	{
		addFace(face.v3, vertex2, vertex1);
		addFace(face.v3, vertex1, face.v2);
		addFace(vertex2, face.v1, vertex1);
	}

	// Moved this
	faces->remove(facePos);
}

/**
 * Face breaker for FACE-FACE-FACE (a point only)
 * 
 * @param facePos face position on the faces array
 * @param newPos new vertex position
 */
void Object3D::breakFaceInThree(int facePos, const mlVector3D & newPos)
{
	Face & face = *(faces->get(facePos));
	// Moved remove to the end
	//faces->remove(facePos);
	
	Vertex * vertex = addVertex(newPos, face.v1->getColor(), Vertex::BOUNDARY);
			
	addFace(face.v1, face.v2, vertex);
	addFace(face.v2, face.v3, vertex);
	addFace(face.v3, face.v1, vertex);

	// Moved this
	faces->remove(facePos);
}

/**
 * Face breaker for EDGE-FACE-FACE / FACE-FACE-EDGE
 * 
 * @param facePos face position on the faces array
 * @param newPos1 new vertex position
 * @param newPos2 new vertex position 
 * @param endVertex vertex used for the split
 */	
void Object3D::breakFaceInFour(int facePos, const mlVector3D & newPos1, const mlVector3D & newPos2, Vertex & endVertex)
{
	Face & face = *(faces->get(facePos));
	// Moved remove to the end
	//faces->remove(facePos);
	
	Vertex * vertex1 = addVertex(newPos1, face.v1->getColor(), Vertex::BOUNDARY);
	Vertex * vertex2 = addVertex(newPos2, face.v1->getColor(), Vertex::BOUNDARY);
	
	if (endVertex.equals(face.v1))
	{
		addFace(face.v1, vertex1, vertex2);
		addFace(vertex1, face.v2, vertex2);
		addFace(face.v2, face.v3, vertex2);
		addFace(face.v3, face.v1, vertex2);
	}
	else if (endVertex.equals(face.v2))
	{
		addFace(face.v2, vertex1, vertex2);
		addFace(vertex1, face.v3, vertex2);
		addFace(face.v3, face.v1, vertex2);
		addFace(face.v1, face.v2, vertex2);
	}
	else
	{
		addFace(face.v3, vertex1, vertex2);
		addFace(vertex1, face.v1, vertex2);
		addFace(face.v1, face.v2, vertex2);
		addFace(face.v2, face.v3, vertex2);
	}

	// Moved this
	faces->remove(facePos);
}

/**
 * Face breaker for FACE-FACE-FACE
 * 
 * @param facePos face position on the faces array
 * @param newPos1 new vertex position
 * @param newPos2 new vertex position 
 * @param linedVertex what vertex is more lined with the interersection found
 */		
void Object3D::breakFaceInFive(int facePos, const mlVector3D & newPos1, const mlVector3D & newPos2, int linedVertex)
{
	Face & face = *(faces->get(facePos));
	// In the original java code, remove was being called here, it has been moved to the end of the function.
	//faces->remove(facePos);
	
	Vertex * vertex1 = addVertex(newPos1, face.v1->getColor(), Vertex::BOUNDARY);
	Vertex * vertex2 = addVertex(newPos2, face.v1->getColor(), Vertex::BOUNDARY);
	
	double cont = 0;
	if (linedVertex == 1)
	{
		addFace(face.v2, face.v3, vertex1);
		addFace(face.v2, vertex1, vertex2);
		addFace(face.v3, vertex2, vertex1);
		addFace(face.v2, vertex2, face.v1);
		addFace(face.v3, face.v1, vertex2);
	}
	else if(linedVertex == 2)
	{
		addFace(face.v3, face.v1, vertex1);
		addFace(face.v3, vertex1, vertex2);
		addFace(face.v1, vertex2, vertex1);
		addFace(face.v3, vertex2, face.v2);
		addFace(face.v1, face.v2, vertex2);
	}
	else
	{
		addFace(face.v1, face.v2, vertex1);
		addFace(face.v1, vertex1, vertex2);
		addFace(face.v2, vertex2, vertex1);
		addFace(face.v1, vertex2, face.v3);
		addFace(face.v2, face.v3, vertex2);
	}

	// Calling it at the end, because the java garbage collection won't destroy it until after the end of the function,
	// (when there is nothing referencing it anymore)
	faces->remove(facePos);
}

//-----------------------------------OTHERS-------------------------------------//

/**
 * Classify faces as being inside, outside or on boundary of other object
 * 
 * @param object object 3d used for the comparison
 */
void Object3D::classifyFaces(Object3D & object)
{
	//calculate adjacency information
	//Face face;
	for(int i=0;i<getNumFaces();i++)
	{
		Face & face = *(getFace(i));
		face.v1->addAdjacentVertex(face.v2);
		face.v1->addAdjacentVertex(face.v3);
		face.v2->addAdjacentVertex(face.v1);
		face.v2->addAdjacentVertex(face.v3);
		face.v3->addAdjacentVertex(face.v1);
		face.v3->addAdjacentVertex(face.v2);
	}
	
	//for each face
	for(int i=0;i<getNumFaces();i++)
	{
		Face & face = *(getFace(i));
		
		//if the face vertices aren't classified to make the simple classify
		if(face.simpleClassify()==false)
		{
			//makes the ray trace classification
			face.rayTraceClassify(object);
			
			//mark the vertices
			if(face.v1->getStatus()==Vertex::UNKNOWN) 
			{
				face.v1->mark(face.getStatus());
			}
			if(face.v2->getStatus()==Vertex::UNKNOWN) 
			{
				face.v2->mark(face.getStatus());
			}
			if(face.v3->getStatus()==Vertex::UNKNOWN) 
			{
				face.v3->mark(face.getStatus());
			}
		}
	}
}

/** Inverts faces classified as INSIDE, making its normals point outside. Usually
 *  used into the second solid when the difference is applied. */
void Object3D::invertInsideFaces()
{
	Face * face = 0;
	for(int i=0;i<getNumFaces();i++)
	{
		face = getFace(i);
		if(face->getStatus()==Face::INSIDE)
		{
			face->invert();
		}
	}
}
