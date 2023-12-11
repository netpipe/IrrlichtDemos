#ifndef MATRICES_H
#define MATRICES_H
#include <iostream>
#include <GL/glew.h>
#include <vector>
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtx/transform.hpp"


enum matrixModes{
	MODEL_MATRIX=0,
	VIEW_MATRIX,
	PROJECTION_MATRIX
};

class matrices{
	std::vector<glm::mat4> modelMatrix; //object position
	std::vector<glm::mat4> viewMatrix; //by the camera
	//modelViewMatrix=viewMatrix*modelMatrix
	std::vector<glm::mat4> projectionMatrix;
//modelViewProjectionMatrix=ProjectionMatrix*viewMatrix*modelMatrix

	//glMatrixMode
	int currentMatrix;
	
	bool matricesReady;
	glm::mat4 modelViewMatrix;
	glm::mat4 modelViewProjectionMatrix;
	glm::mat3 normalMatrix;
	
	public:
		matrices();
		
		void loadIdentity();	//glLoadIdentity()==no transformastion
		void matrixMode(int i); //glMatrixMode
		
		//transformation
		void translate(float x,float y,float z); //==glTranslatef
		void scale(float x,float y,float z); //glScalef(x,y,z)
		void scale(float x); //glScalef(x,x,x);
		void rotateX(float angle);//glRotatef(angle,1,0,0);
		void rotateY(float angle);//glRotatef(angle,0,1,0);
		void rotateZ(float angle);//glRotatef(angle,0,0,1);
		
		//projection
		void perspective(float angle,float aRatio,float near,float far); //==gluPerspective
		void ortho(float left,float right,float bottom,float top,float near,float far);	//==glOrtho
		
		void pushMatrix(); //glPushMatrix()
		void popMatrix(); //glPopMatrix()
		
		glm::mat4& getModelMatrix();
		glm::mat4& getViewMatrix();
		glm::mat4& getModelViewMatrix();
		glm::mat4& getModelViewProjectionMatrix();
		glm::mat4& getProjectionMatrix();
		glm::mat3& getNormalMatrix();
		
		//GLSL
		void updateMatrices(unsigned int programId);
		
	
};


#endif
