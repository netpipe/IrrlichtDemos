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

#include <string>
#include <list>

#include "core/ReferenceCounted.h"
#include "graphics/SceneNode.h"

// tolua_begin	
namespace peak
{
	enum EImpostorType
	{
		EIT_Billboard,
		EIT_Internal,
		EIT_External
	};
	
	class Impostor : public ReferenceCounted
	{
		public:
			Impostor();
			~Impostor();
			
			bool create(EImpostorType type, std::string mesh);
			bool destroy(void);
			
			std::string getName(void);
			// tolua_end
			#ifdef _USE_IRRLICHT_
			irr::scene::IMesh *getMesh(void);
			irr::video::ITexture *getTexture(void);
			#endif
		private:
			irr::scene::IMesh *mesh;
			irr::video::ITexture *texture;
			std::string name;
			// tolua_begin	
	};
	
	class ImpostorSceneNode : public SceneNode
	{
		public:
			ImpostorSceneNode(Impostor *impostor, SceneNode *parent = 0);
			ImpostorSceneNode(EImpostorType type, std::string mesh, SceneNode *parent = 0);
			~ImpostorSceneNode();
			// tolua_end
		private:
			Impostor *impostor;
			// tolua_begin
	};
	
	class ImpostorManager
	{
		public:
			/**
			 * \brief Returns a pointer to the impostor manager.
			 */
			static ImpostorManager *get(void);
			~ImpostorManager();
			
			/**
			 * \brief Returns an impostor of the specified type.
			 * \param type Type of the impostor to be created
			 * \param mesh File name of the mesh
			 */
			Impostor *getImpostor(EImpostorType type, std::string mesh);
			/**
			 * \brief Removes an impostor from the impostor list.
			 *
			 * Do not use this function directly as other scene nodes might still
			 * need the impostor! Use Impostor::drop() instead.
			 */
			void deleteImpostor(Impostor *impostor);
			//tolua_end
		private:
			ImpostorManager();
			
			std::list<Impostor*> impostors;
			// tolua_begin	
	};
}
//tolua_end

