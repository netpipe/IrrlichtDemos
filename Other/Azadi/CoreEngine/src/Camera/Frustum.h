#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <cmath>

class Frustum
{
	public:
		void ExtractFrustum();
		bool PointCheck(float, float, float);
		bool SphereCheck(float, float, float, float);
		float frustum[6][4];
};

//extern Frustum frustum;
