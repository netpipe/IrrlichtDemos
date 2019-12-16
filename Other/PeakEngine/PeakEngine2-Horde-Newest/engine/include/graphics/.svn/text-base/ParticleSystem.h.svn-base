/*
Copyright (C) 2008  Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

#include "graphics/SceneNode.h"
#include "core/Vector2D.h"

// tolua_begin	
namespace peak
{
	/**
	 * \brief Class which holds a particle system to create special effects like
	 * flames
	 */
	class ParticleSystem : public SceneNode
	{
		public:
			ParticleSystem(std::string filename, SceneNode *parent = 0);
			~ParticleSystem();
			
			// tolua_end
			void doWork(float msecs);
			static void doAllWork(float msecs);
		private:
			int resource;
			
			static std::list<ParticleSystem*> particlesystems;
			// tolua_begin
	};
}
// tolua_end
	
#endif

