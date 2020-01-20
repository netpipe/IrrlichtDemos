// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _CShaderGfxHeader_
#define _CShaderGfxHeader_

#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>

#include "gfx/IShader.h"

namespace lew
{
	namespace gfx
	{
		class IRoot;
		class CShader : public IShader
		{
			public:			
				CShader(IRoot* m_pRoot, const std::string pVertexShader, 
					const std::string pFragmentShader);
				
				virtual ~CShader();

				void setUniformf(const int pLocation, int pUniformNum,
					const float u1, const float u2 = 0.0f, const float u3 = 0.0f, 
					const float u4 = 0.0f);

				void setUniformi(const int pLocation, int pUniformNum,
					const int u1, const int u2 = 0, const int u3 = 0, 
					const int u4 = 0);

				void setAttributef(const int pLocation, int pAttributeNum,
					const float u1, const float u2 = 0.0f, const float u3 = 0.0f, 
					const float u4 = 0.0f);

				void setAttributei(const int pLocation, int pAttributeNum,
					const int u1, const int u2 = 0, const int u3 = 0, 
					const int u4 = 0);

				int getUniform(const std::string pName) const;
				int getAttribute(const std::string pName) const;

				std::string getVertexShaderSource() const;
				std::string getFragmentShaderSource() const;
				
				unsigned long int getInternalHandle() const;
				
				
			private:
				void compile();
				
				IRoot* pRoot;
				GLuint ShaderHandle;
				GLuint VertexShader;
				GLuint FragmentShader;
				
				std::string	VertexShaderSource;
				std::string FragmentShaderSource;
				
				bool vertexShaderFailed;
				bool fragmentShaderFailed;
		};
	};
};

#endif

