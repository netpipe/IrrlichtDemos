#include <iostream>
#include <stdlib.h>
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include "ObjLoader.h"


#define TRUE  1
#define FALSE 0



GLfloat yrot;       /* Camera rotation variable */
GLfloat xpos, zpos; /* Camera pos variable */
GLfloat walkbias, walkbiasangle; /* Head-bobbing variables */
GLfloat lookupdown;
GLuint ListID,gravy;
GLfloat jump;
const float Gravity = 9.81; // e= 1/2mv^2
const float piover180 = 0.0174532925f;
int done = FALSE;

	SDL_Surface *surface;
	SDL_Event event;


class vertex{
public:
	float x,y,z;
};

class Triangle{
public:
    vertex triangle[3];
};



inline float GetHeight(int x, int y){

 //file = fopen( FName , "rb");
//return((x%7*y));
return(sin(x*y));
};

void CreateList()
{
	ListID = glGenLists(1);
	glNewList(ListID, GL_COMPILE);		
	for (int z = -3; z < 20; z=z+1){
		for (int x = -3; x < 20; x++){ 
			int zp1= z+1;
			int xp1= x+1;
	float cco = GetHeight(x,z);
		glBegin(GL_TRIANGLE_STRIP); 
			glColor3f(cco, cco, cco);
			glVertex3f(x, GetHeight(x,z) , z ); 
			glVertex3f(x, GetHeight(x,(zp1)), zp1 );
			glVertex3f(xp1, GetHeight((xp1),(zp1)), zp1);
		glEnd();
		glBegin(GL_TRIANGLE_STRIP); 
			glColor3f(cco, cco, cco);
			glVertex3f(x, GetHeight(x,z), z);
			glVertex3f(xp1, GetHeight((xp1),(zp1)), zp1);
			glVertex3f(xp1, GetHeight((xp1),z), z);
		glEnd();
	
		}
	}
glEndList();
}


void DispMods(){
	ObjModel data;
	ObjLoader LoaderClass;

	LoaderClass.LoadObj("sphere.obj");
	data = LoaderClass.ReturnObj();

	gravy = glGenLists(1);
	glNewList(ListID, GL_COMPILE);	

glBegin(GL_TRIANGLES); 
	for(int i = 0; i < data.NumTriangle; i++)  {
glColor3f(1.0f, 1.0f, 1.0f);
cout << i;
glVertex3f(data.VertexArray[data.TriangleArray[i].Vertex[0]].X,data.VertexArray[data.TriangleArray[i].Vertex[0]].Y,data.VertexArray[data.TriangleArray[i].Vertex[0]].Z);
glVertex3f(data.VertexArray[data.TriangleArray[i].Vertex[1]].X,data.VertexArray[data.TriangleArray[i].Vertex[1]].Y,data.VertexArray[data.TriangleArray[i].Vertex[1]].Z);
glVertex3f(data.VertexArray[data.TriangleArray[i].Vertex[2]].X,data.VertexArray[data.TriangleArray[i].Vertex[2]].Y,data.VertexArray[data.TriangleArray[i].Vertex[2]].Z);
	}
glEnd();
glEndList();

cout << "loaderd it fine";

/*

*/

/*
//btexindex number
		data.TriangleArray[i].Vertex[0] ;		data.TriangleArray[i].Vertex[1];		data.TriangleArray[i].Vertex[2];
*/
}
