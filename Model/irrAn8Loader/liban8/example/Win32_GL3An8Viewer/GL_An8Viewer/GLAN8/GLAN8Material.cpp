#include "GLAN8Material.h"

GLAN8Material::GLAN8Material(void)
{
	ShaderProgramObject = NULL;
}

GLAN8Material::~GLAN8Material(void)
{
}

void GLAN8Material::Set( void )
{
	//glColor3f(DiffuseRGB.r,DiffuseRGB.g,DiffuseRGB.b); // deprecated

	if( DiffuseIsTexture == GL_TRUE && DiffuseTextureID != -1 )
	{
		// glEnable(GL_TEXTURE_2D); // deprecated
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,DiffuseTexture);

		if( ShaderProgramObject != NULL)
		{
			glUniform1i(glGetUniformLocation(*ShaderProgramObject, "g_HasDiffuseTexture"), 1);
			glUniform1i(glGetUniformLocation(*ShaderProgramObject, "g_DiffuseTexture"), 0);
			glUniform4fv(glGetUniformLocation(*ShaderProgramObject, "g_DiffuseColor"), 1,DiffuseRGB);
		}
	}
	else
	{
		//glDisable(GL_TEXTURE_2D); // deprecated
		if( ShaderProgramObject != NULL)
		{
			glUniform1i(glGetUniformLocation(*ShaderProgramObject, "g_HasDiffuseTexture"), 0);
			glUniform4fv(glGetUniformLocation(*ShaderProgramObject, "g_DiffuseColor"), 1,DiffuseRGB);
			
		}
			
	}
}

