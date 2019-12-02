#include"CameraManager.h"



Camera::Camera(void)
{
	ID = -1;
	FOV   = 60.0f;
	ZNear = 50.0f;
	ZFar  = 10000.0f;
}
Camera::~Camera(void)
{

}


void Camera::MoveForward(float i_Value)
{
	GLGXVECTOR3 direction = Eye - Loc;
	GLGXVec3Normalize(&direction,&direction);

	Loc = Loc + (direction*i_Value);
	Eye = Eye + (direction*i_Value);
	Update();
};
void Camera::RotatePitch(float i_Value)
{

};
void Camera::RotateYaw(float i_Value)
{

};

void Camera::SetProjection( GLdouble width, GLdouble height)
{
	//gluPerspective( FOV, width / height, ZNear, ZFar); // deprecated

	GLGXMatrixPerspectiveFov(&MatrixProjection,FOV, width / height, ZNear, ZFar);

	

};



int CameraManager::Add(Camera* i_Camera)
{
	bool alreadyPrensent = false;
	for (std::vector<Camera*>::iterator i = m_vCameras.begin(); i != m_vCameras.end(); ++i) 
	{
		if( (*i)->Name.compare(i_Camera->Name) == 0 )
		{
			alreadyPrensent = true;
			return (*i)->ID;
			break;
		}
	}

	m_vCameras.push_back(i_Camera);
	i_Camera->ID = (int) m_vCameras.size()-1;
	i_Camera->Update();

	return i_Camera->ID;
}


int CameraManager::GetID(std::string* i_CameraName)
{
	for (std::vector<Camera*>::iterator i = m_vCameras.begin(); i != m_vCameras.end(); ++i) 
	{
		if( (*i)->Name.compare(*i_CameraName) == 0 )
		{
			return (*i)->ID;
		}
	}
	return 0;
}

Camera* CameraManager::Get(int i_ID)
{
	for (std::vector<Camera*>::iterator i = m_vCameras.begin(); i != m_vCameras.end(); ++i) 
	{
		if( (*i)->ID == i_ID )
		{
			return (*i);
		}
	}
	return NULL;
};

void CameraManager::Release(void)
{
	for (std::vector<Camera*>::iterator i = m_vCameras.begin(); i != m_vCameras.end(); ++i) 
	{
		delete (*i);
		(*i) = NULL;
	}
	m_vCameras.clear();

}




