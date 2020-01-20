// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _IShaderGfxHeader_
#define _IShaderGfxHeader_

#include <string>
#include "gfx/Reference.h"

namespace lew
{
	namespace gfx
	{
		class IShader : public Reference
		{
			public:			
				IShader() : Reference() {} 

				virtual void setUniformf(const int pLocation, int pUniformNum,
					const float u1, const float u2 = 0.0f, const float u3 = 0.0f, 
					const float u4 = 0.0f) = 0;

				virtual void setUniformi(const int pLocation, int pUniformNum,
					const int u1, const int u2 = 0, const int u3 = 0, 
					const int u4 = 0) = 0;

				virtual void setAttributef(const int pLocation, int pAttributeNum,
					const float u1, const float u2 = 0.0f, const float u3 = 0.0f, 
					const float u4 = 0.0f) = 0;

				virtual void setAttributei(const int pLocation, int pAttributeNum,
					const int u1, const int u2 = 0, const int u3 = 0, 
					const int u4 = 0) = 0;

				virtual int getUniform(const std::string pName) const = 0;
				virtual int getAttribute(const std::string pName) const = 0;

				virtual std::string getVertexShaderSource() const = 0;
				virtual std::string getFragmentShaderSource() const = 0;
		};
	};
};

#endif

