
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <string>

//tolua_begin
namespace peak
{
	class Terrain
	{
		public:
			Terrain();
			~Terrain();
			
			bool create(std::string heightmap, bool physics, std::string colormap, std::string detail0, std::string detail1, std::string detail2, std::string detail3);
			bool destroy(void);
			
			//tolua_end
		private:
			//tolua_begin
	};
}
//tolua_end

#endif

