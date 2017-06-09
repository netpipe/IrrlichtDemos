#include "Flag.h"

#include "vectors.h"
#include <math.h>

#include <GL\glut.h>

#define NULL_VECTOR F3dVector(0.0f,0.0f,0.0f);

CFlag::CFlag()
{
	m_Initialized = false;
}

void CFlag::Initialize(int DetailX, int DetailY, float FlagSize,float spring_const, float damping)
{
	//*********************
	//Init vertex array:
	m_NumVertices = DetailX*DetailY;
	m_Vertices = new SVertex[m_NumVertices];
	m_Velocity = new SF3dVector[m_NumVertices];

	m_DirectDistance = FlagSize/(DetailX-1);
	m_AslantDistance = sqrt(2.0f)*m_DirectDistance;


	for (int yc = 0; yc < DetailY; yc++)
		for (int xc=0; xc < DetailX; xc++)
		{
			int ArrayPos = yc*DetailX+xc;
			

			m_Vertices[ArrayPos].pos = F3dVector(xc*m_DirectDistance,
											     yc*m_DirectDistance,
												 0.0f);

			//only the initialization normal, will be changed later:
			m_Vertices[ArrayPos].normal = F3dVector(0.0f,
												    0.0f,
													-1.0f);

			m_Vertices[ArrayPos].TexCoords.x = xc/float(DetailX-1);
			m_Vertices[ArrayPos].TexCoords.y = yc/float(DetailY-1);

			m_Velocity[ArrayPos] = NULL_VECTOR;
		}

	//*********************
	//Init index array:
	m_NumIndices = 3 * 2*(DetailX-1)*(DetailY-1);
	m_Indices = new GLuint[m_NumIndices];
	for (yc = 0; yc < DetailY-1; yc++)
		for (int xc=0; xc < DetailX-1; xc++)
		{
			//Create two triangles:

			int ArrayPos = 6*(yc*(DetailX-1)+xc);
			m_Indices[ArrayPos+0] = yc*DetailX+xc;
			m_Indices[ArrayPos+1] = yc*DetailX+xc+1;
			m_Indices[ArrayPos+2] = (yc+1)*(DetailX)+xc+1;

			m_Indices[ArrayPos+3] = yc*DetailX+xc;
			m_Indices[ArrayPos+4] = (yc+1)*DetailX+xc+1;
			m_Indices[ArrayPos+5] = (yc+1)*DetailX+xc;

		}


	m_spring_const = spring_const;
	m_Damping = damping;

	m_DetailX = DetailX;
	m_DetailY = DetailY;

	m_Initialized = true;

}

SF3dVector CFlag::GetForce(int xOtherVertex, int yOtherVertex, SF3dVector VertexPos, float StandardDistance)
{

	if ((xOtherVertex >= 0 ) && (xOtherVertex < m_DetailX) && (yOtherVertex >= 0 ) && (yOtherVertex < m_DetailY))
	{

		SF3dVector Distance = m_Vertices[xOtherVertex+m_DetailX*yOtherVertex].pos-VertexPos;

		float len=GetF3dVectorLength(&Distance);
		if (len == 0.0f)  return NULL_VECTOR;  //not correct, but cannot be handled another way..

		SF3dVector res = Distance*((len-StandardDistance)/len);

		res = res * m_spring_const;

		if (GetF3dVectorLength(&res) > 10 )
			res = NULL_VECTOR;

		return res;
	} else return NULL_VECTOR;

}

void CFlag::UpdateNormals()
{
	for (int xc = 0; xc < m_DetailX-1; xc ++)
		for (int yc = 0; yc < m_DetailY-1; yc++)
		{
			int ArrayPos = yc*m_DetailX+xc;

			///
			//Calculating the normal:
			//Take the direction vectors 1.) from the left to the right neighbor 
			// and 2.) from the upper to the lower neighbor.
			//The vector orthogonal to these 

			SF3dVector u,v,p1,p2;	//u and v are direction vectors. p1 / p2: temporary used (storing the points)

			if (xc > 0) p1 = m_Vertices[ArrayPos-1].pos;
			else
						p1 = m_Vertices[ArrayPos].pos;	
			if (xc < m_DetailX-1) 
						p2 = m_Vertices[ArrayPos+1].pos;
			else
						p2 = m_Vertices[ArrayPos].pos;	
			u = p2-p1; //vector from the left neighbor to the right neighbor
			if (yc > 0) p1 = m_Vertices[ArrayPos-m_DetailX].pos;
			else
						p1 = m_Vertices[ArrayPos].pos;	
			if (yc < m_DetailY-1)
						p1 = m_Vertices[ArrayPos+m_DetailX].pos;
			else
						p1 = m_Vertices[ArrayPos].pos;

			v = p1-p2; //vector from the upper neighbor to the lower neighbor
			//calculate the normal:
			m_Vertices[ArrayPos].normal = Normalize3dVector(CrossProduct(&u,&v));

		}
}

void CFlag::UpdateVertices(float dtime, SF3dVector gravity, SF3dVector wind)
{

	
    SF3dVector * a = new SF3dVector [m_DetailX*m_DetailY];  //acceleration array

	for (int xc = 0; xc < m_DetailX; xc ++)
		for (int yc = 0; yc < m_DetailY; yc++)
		if (xc > 0)
		{

			int ArrayPos = yc*m_DetailX+xc;
			SF3dVector VertexPos = m_Vertices[ArrayPos].pos;

			a[ArrayPos] = gravity;

			

			a[ArrayPos]=a[ArrayPos]+GetForce(xc-1,yc, VertexPos, m_DirectDistance);
			a[ArrayPos]=a[ArrayPos]+GetForce(xc+1,yc, VertexPos, m_DirectDistance);
			a[ArrayPos]=a[ArrayPos]+GetForce(xc,  yc-1, VertexPos, m_DirectDistance);
			a[ArrayPos]=a[ArrayPos]+GetForce(xc,  yc+1, VertexPos, m_DirectDistance);

			a[ArrayPos]=a[ArrayPos]+GetForce(xc-1,yc-1,VertexPos, m_AslantDistance);
			a[ArrayPos]=a[ArrayPos]+GetForce(xc-1,yc+1,VertexPos, m_AslantDistance);
			a[ArrayPos]=a[ArrayPos]+GetForce(xc+1,yc-1,VertexPos, m_AslantDistance);
			a[ArrayPos]=a[ArrayPos]+GetForce(xc+1,yc+1,VertexPos, m_AslantDistance);


			//wind:
			float DotProduct = Normalize3dVector(wind)*m_Vertices[ArrayPos].normal;
			if(DotProduct < 0.0f) DotProduct *= -1;
			a[ArrayPos]=a[ArrayPos]+wind*DotProduct;


		} else a[yc*m_DetailX+xc] = NULL_VECTOR;

	for (xc = 0; xc < m_DetailX; xc ++)
		for (int yc = 0; yc < m_DetailY; yc++)
		{
			int ArrayPos = yc*m_DetailX+xc;

			
			m_Velocity[ArrayPos] = m_Velocity[ArrayPos]+a[ArrayPos]*dtime;

			m_Velocity[ArrayPos] = m_Velocity[ArrayPos]*m_Damping;

			m_Vertices[ArrayPos].pos = m_Vertices[ArrayPos].pos+m_Velocity[ArrayPos]*dtime;


		}

	delete[] a;

	UpdateNormals();


}

void CFlag::Render()
{
	if (!m_Initialized) return;

	//Draw the flag drapery:
	glEnable(GL_TEXTURE_2D);
	glVertexPointer(	3,
						GL_FLOAT,
						sizeof(SVertex),
						m_Vertices);
	glNormalPointer(	GL_FLOAT,
						sizeof(SVertex),
						&m_Vertices[0].normal.x);

	glTexCoordPointer ( 2,
						GL_FLOAT,
						sizeof(SVertex),
						&m_Vertices[0].TexCoords.x);

	glDrawElements(	GL_TRIANGLES, //mode
						m_NumIndices,  //count, ie. how many indices
						GL_UNSIGNED_INT, //type of the index array
						m_Indices);;
	
	//draw the flagpole:
	float radius=0.07;

	float maxY = 1.3f;
	float minY = -4.0f;

	float PosArray[] = { -radius, minY , -radius,  //0
						 -radius, minY , +radius,  //1
						 +radius, minY , +radius,  //2
						 +radius, minY , -radius,  //3
						 -radius, maxY , -radius,  //4
						 -radius, maxY , +radius,  //5
						 +radius, maxY , +radius,  //6
						 +radius, maxY , -radius   //7
						};

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
	  glNormal3f(0.0,1.0,0.0);

	  //bottom:
	  glVertex3fv(&PosArray[0*3]);
	  glVertex3fv(&PosArray[1*3]);
	  glVertex3fv(&PosArray[2*3]);
	  
	  glVertex3fv(&PosArray[0*3]);
	  glVertex3fv(&PosArray[2*3]);
	  glVertex3fv(&PosArray[3*3]);

	  //top:
	  glVertex3fv(&PosArray[4*3]);
	  glVertex3fv(&PosArray[5*3]);
	  glVertex3fv(&PosArray[6*3]);
	  
	  glVertex3fv(&PosArray[4*3]);
	  glVertex3fv(&PosArray[6*3]);
	  glVertex3fv(&PosArray[7*3]);


	  glNormal3f(1.0,0.0,0.0);
	  //left:
	  glVertex3fv(&PosArray[0*3]);
	  glVertex3fv(&PosArray[1*3]);
	  glVertex3fv(&PosArray[5*3]);
	  
	  glVertex3fv(&PosArray[0*3]);
	  glVertex3fv(&PosArray[5*3]);
	  glVertex3fv(&PosArray[4*3]);

	  //right:
	  glVertex3fv(&PosArray[3*3]);
	  glVertex3fv(&PosArray[2*3]);
	  glVertex3fv(&PosArray[6*3]);
	  
	  glVertex3fv(&PosArray[3*3]);
	  glVertex3fv(&PosArray[6*3]);
	  glVertex3fv(&PosArray[7*3]);

	  glNormal3f(0.0,0.0,1.0);
	  //front:
	  glVertex3fv(&PosArray[1*3]);
	  glVertex3fv(&PosArray[2*3]);
	  glVertex3fv(&PosArray[5*3]);
	  
	  glVertex3fv(&PosArray[2*3]);
	  glVertex3fv(&PosArray[5*3]);
	  glVertex3fv(&PosArray[6*3]);

	  //back:
	  glVertex3fv(&PosArray[0*3]);
	  glVertex3fv(&PosArray[4*3]);
	  glVertex3fv(&PosArray[3*3]);
	  
	  glVertex3fv(&PosArray[3*3]);
	  glVertex3fv(&PosArray[4*3]);
	  glVertex3fv(&PosArray[7*3]);

	glEnd();
	
	glPushMatrix();
	  glTranslatef(0.0,maxY,0.0);
	  glutSolidSphere(0.15,20,20);
	glPopMatrix();
}
