#include "GLAN8Light.h"




GLAN8Light::GLAN8Light(void)
{
	this->ID = GL_LIGHT0;
	this->Kind = GLAN8LIGHT_LOCAL;
	this->Loc = GLGXVECTOR4(0.0f,0.0f,0.0f,1.0f);
	this->Eye = GLGXVECTOR4(0.0f,0.0f,0.0f,0.0f);
	GLGXMatrixIdentity(&this->Matrix);
	this->InAngle = 30.0f;
	this->OutAngle = 30.0f;
	this->ShaderProgramObject = NULL;
};


GLAN8Light::~GLAN8Light(void)
{

}

void GLAN8Light::Set( void )
{
	//glEnable(GL_LIGHTING);
	//glEnable(GL_COLOR_MATERIAL);

	float lightSetting[4];

	glEnable(this->ID);

	lightSetting[0]=0.5f;
    lightSetting[1]=0.5f;
    lightSetting[2]=0.5f;
    lightSetting[3]=1.0f;
	//glLightfv(this->ID, GL_AMBIENT, lightSetting); // deprecated
 
    lightSetting[0]= 1.0f;	
    lightSetting[1]= 1.0f;
    lightSetting[2]= 1.0f;
    lightSetting[3]= 1.0f;
   // glLightfv(this->ID, GL_DIFFUSE, lightSetting);  // deprecated

   // glLightf(this->ID, GL_SPOT_CUTOFF,180.0f); // deprecated

	if( this->Kind == GLAN8LIGHT_INFINITE ) // Directional
	{
		GLGXVECTOR4 lightDir = (Eye - Loc);
		GLGXVec4Normalize(&lightDir,&lightDir);
		if( ShaderProgramObject != NULL)
			glUniform4fv(glGetUniformLocation(*ShaderProgramObject, "g_LightPosition"),1, (float*)&lightDir);
	}	
	else if( this->Kind == GLAN8LIGHT_LOCAL)	// local
	{
		//glLightfv(this->ID, GL_POSITION,this->Loc);  
		if( ShaderProgramObject != NULL)
			glUniform4fv(glGetUniformLocation(*ShaderProgramObject, "g_LightPosition"),1, (float*)&this->Loc);int errorTest = glGetError();
		int toto = 4;

	}
	else if( this->Kind == GLAN8LIGHT_SPOT )// spot
	{            
		if( ShaderProgramObject != NULL)
		{
			glUniform4fv(glGetUniformLocation(*ShaderProgramObject, "g_LightPosition"),1, (float*)&this->Loc);

			GLGXVECTOR4 lightDir = (Eye - Loc);
			GLGXVec4Normalize(&lightDir,&lightDir);
			glUniform4fv(glGetUniformLocation(*ShaderProgramObject, "g_LightDirection"),1, (float*)&lightDir);

			glUniform1fv(glGetUniformLocation(*ShaderProgramObject, "g_LightSpotCutOff"),1, &this->InAngle);
		}
	}

	if( ShaderProgramObject != NULL)
			glUniform1i(glGetUniformLocation(*ShaderProgramObject, "g_LightType"), this->Kind);
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
}

