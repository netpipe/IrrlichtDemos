// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#include "gfx/Error.h"
#include "CShader.h"
#include "CRoot.h"
#include "CVideoDriver.h"

namespace lew
{
	namespace gfx
	{
		CShader::CShader(IRoot* m_pRoot, const std::string pVertexShader, 
		const std::string pFragmentShader) : IShader()
		{
			pRoot = m_pRoot;

			VertexShaderSource = pVertexShader;
			FragmentShaderSource = pFragmentShader;
			
			compile();
		}
		
		CShader::~CShader()
		{
			((CVideoDriver*)pRoot->getVideoDriver())->removeShader(this);
			
			if(!VertexShaderSource.empty() && !vertexShaderFailed)
				glDeleteShader(VertexShader);	
				
			if(!FragmentShaderSource.empty() && !fragmentShaderFailed)
				glDeleteShader(FragmentShader);		
		}

		void CShader::setUniformf(const int pLocation, int pUniformNum,
		const float u1, const float u2, const float u3,	const float u4)
		{
			if(pUniformNum < 1){pUniformNum = 1;}
			if(pUniformNum > 4){pUniformNum = 4;}

			glUseProgram(ShaderHandle);

			switch (pUniformNum)
			{
				case 1:
					glUniform1f(pLocation, u1);
					break;

				case 2:
					glUniform2f(pLocation, u1, u2);
					break;

				case 3:
					glUniform3f(pLocation, u1, u2, u3);
					break;

				case 4:
					glUniform4f(pLocation, u1, u2, u3, u4);
					break;
			}

			glUseProgram(0);		
		}

		void CShader::setUniformi(const int pLocation, int pUniformNum,
		const int u1, const int u2, const int u3, const int u4)
		{
			if(pUniformNum < 1){pUniformNum = 1;}
			if(pUniformNum > 4){pUniformNum = 4;}

			glUseProgram(ShaderHandle);

			switch (pUniformNum)
			{
				case 1:
					glUniform1i(pLocation, u1);
					break;

				case 2:
					glUniform2i(pLocation, u1, u2);
					break;

				case 3:
					glUniform3i(pLocation, u1, u2, u3);
					break;

				case 4:
					glUniform4i(pLocation, u1, u2, u3, u4);
					break;
			}

			glUseProgram(0);		
		}

		void CShader::setAttributef(const int pLocation, int pAttributeNum,
		const float u1, const float u2, const float u3, const float u4)
		{
			if(pAttributeNum < 1){pAttributeNum = 1;}
			if(pAttributeNum > 4){pAttributeNum = 4;}

			glUseProgram(ShaderHandle);

			switch (pAttributeNum)
			{
				case 1:
					glVertexAttrib1f(pLocation, u1);
					break;

				case 2:
					glVertexAttrib2f(pLocation, u1, u2);
					break;

				case 3:
					glVertexAttrib3f(pLocation, u1, u2, u3);
					break;

				case 4:
					glVertexAttrib4f(pLocation, u1, u2, u3, u4);
					break;
			}

			glUseProgram(0);		
		}

		void CShader::setAttributei(const int pLocation, int pAttributeNum,
		const int u1, const int u2, const int u3, const int u4)
		{
			setAttributef(pLocation, pAttributeNum,
				float(u1), float(u2), float(u3), float(u4));
		}

		int CShader::getUniform(const std::string pName) const
		{
			return glGetUniformLocation(ShaderHandle, pName.c_str());
		}
		
		int CShader::getAttribute(const std::string pName) const
		{
			return glGetAttribLocation(ShaderHandle, pName.c_str());
		}

		std::string CShader::getVertexShaderSource() const 
		{
			return VertexShaderSource;
		}
		
		std::string CShader::getFragmentShaderSource() const
		{
			return FragmentShaderSource;
		}
		
		unsigned long int CShader::getInternalHandle() const
		{
			return ShaderHandle;
		}
		
		void CShader::compile()
		{
			int Success	= 0;
			vertexShaderFailed	= false;
			fragmentShaderFailed = false;

			ShaderHandle = glCreateProgram();

			if(!VertexShaderSource.empty())
			{
				const char* vsSource = VertexShaderSource.c_str();
				VertexShader = glCreateShader(GL_VERTEX_SHADER);

				glShaderSource(VertexShader, 1, &vsSource, NULL);
				glCompileShader(VertexShader);
				
				glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);
				glAttachShader(ShaderHandle, VertexShader);

				if(!Success)
					vertexShaderFailed = true;
			}

			if(!FragmentShaderSource.empty())
			{
				const char* fsSource = FragmentShaderSource.c_str();
				FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
				
				glShaderSource(FragmentShader, 1, &fsSource, NULL);
				glCompileShader(FragmentShader);
				
				glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
				glAttachShader(ShaderHandle, FragmentShader);

				if(!Success)
					fragmentShaderFailed = true;
			}

			if(!vertexShaderFailed || !fragmentShaderFailed)
				glLinkProgramARB(ShaderHandle);

			if(vertexShaderFailed)
			{
				int len = 0;
				glGetObjectParameterivARB(VertexShader, 
					GL_OBJECT_INFO_LOG_LENGTH_ARB, &len);
				
				char* logmsg = new char[len];				
				glGetInfoLogARB(VertexShader, len, 0, logmsg);
				
				std::string output; output += logmsg;
				delete [] logmsg;
				
				throw(err::VertexShader(VertexShaderSource, output));
			}
				
			if(fragmentShaderFailed)
			{
				int len = 0;
				glGetObjectParameterivARB(FragmentShader, 
					GL_OBJECT_INFO_LOG_LENGTH_ARB, &len);
				
				char* logmsg = new char[len];				
				glGetInfoLogARB(FragmentShader, len, 0, logmsg);
				
				std::string output; output += logmsg;
				delete [] logmsg;
				
				throw(err::FragmentShader(FragmentShaderSource, output));
			}
		}
	};
};

