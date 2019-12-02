#ifndef _CameraManager_H
#define _CameraManager_H

#include"../GLAN8/GLGX/GLGX.h"
#include<string>
#include<vector>

class Camera
{
private:


public:

	std::string Name;
	int ID;
	
	GLGXVECTOR3 Loc;
	GLGXVECTOR3 Eye;
	GLGXVECTOR3 Up;

	GLfloat		FOV;

	GLGXMATRIX MatrixView;

	// Projection
	GLGXMATRIX MatrixProjection;
	int width;
	int height;
	GLfloat ZNear;
	GLfloat ZFar;
	void SetProjection( GLdouble width, GLdouble height);

	void SetViewMatrix(GLGXMATRIX i_Matrix){MatrixView = i_Matrix;};
	GLGXMATRIX* GetViewMatrix(void){return &MatrixView;};

	void Update(void)
	{
		GLGXMatrixLookAt(&MatrixView,
		&Loc,
		&Eye,
		&Up);
	};

	/*void SetView(void) // deprecated
	{
		glMultMatrixf(MatrixView); 
	}
	*/

	void MoveForward(float i_Value);
	void RotatePitch(float i_Value);
	void RotateYaw(float i_Value);

	Camera(void);
	~Camera(void);
};

class CameraManager
{
private:
	std::vector<Camera*> m_vCameras;

public:
	int Add(Camera* i_Camera);
	int GetID(std::string* i_CameraName);
	Camera* Get(int i_ID);
	void Release(void);
};

#endif	