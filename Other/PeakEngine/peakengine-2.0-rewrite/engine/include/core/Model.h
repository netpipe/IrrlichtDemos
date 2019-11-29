
#ifndef _MODEL_H_
#define _MODEL_H_

#include "core/Object3D.h"

#include <string>

namespace peak
{
	class Model : public Object3D
	{
		public:
			Model();
			~Model();
			
			bool load(std::string name, bool graphics = true, bool physics = true);
			bool destroy(void);
			
		private:
	};
}

#endif

