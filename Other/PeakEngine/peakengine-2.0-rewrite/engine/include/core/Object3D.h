
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "core/Vector3D.h"
#include "core/Quaternion.h"

namespace peak
{
	class Object3D
	{
		public:
			Object3D();
			~Object3D();
			
		protected:
			Vector3D position;
			Quaternion rotation;
			Vector3D linearvel;
			Vector3D angularvel;
	};
}

#endif

