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

#include "core/Vector3D.h"

//tolua_begin
namespace peak
{
	class SceneNode;
	class SoundSource;
	
	/**
	 * \brief Class which holds a 2D/3D sound
	 *
	 * Sounds are created via SoundEngine::play2D() or SoundEngine::play3D().
	 */
	class Sound
	{
		public:
			Sound();
			~Sound();
			
			/**
			 * \brief Initialized the sound from a sound source
			 */
			bool init(SoundSource *source);
			/**
			 * \brief Destroys the sound
			 */
			bool destroy(void);
			
			/**
			 * \brief Returns the sound source
			 */
			SoundSource *getSource(void);
			
			/**
			 * \brief Sets whether the sound is looped
			 */
			void setLooped(bool looped);
			/**
			 * \brief Returns whether the sound is looped
			 */
			bool getLooped(void);
			
			/**
			 * \brief Sets whether the sound is deleted when it stops
			 */
			void setCleanUp(bool cleanup);
			/**
			 * \brief Returns whether the sound is deleted when it stops
			 */
			bool getCleanUp(void);
			
			/**
			 * \brief Sets whether the sound is paused
			 */
			void setPaused(bool paused);
			/**
			 * \brief Returns whether the sound is paused
			 */
			bool isPaused(void);
			
			/**
			 * \brief Sets whether the sound is playing
			 */
			void setPlaying(bool playing);
			/**
			 * \brief Returns whether the sound is playing
			 */
			bool isPlaying(void);
			
			/**
			 * \brief Sets the volume of the sound
			 */
			void setVolume(float volume);
			/**
			 * \brief Returns the volume of the sound
			 */
			float getVolume(void);
			
			/**
			 * \brief Sets the position
			 */
			void setPosition(Vector3D pos);
			/**
			 * \brief Returns the position
			 */
			Vector3D getPosition(void);
			/**
			 * \brief Sets the velocity
			 */
			void setVelocity(Vector3D vel);
			/**
			 * \brief Returns the velocity
			 */
			Vector3D getVelocity(void);
			
			/**
			 * \brief Sets whether the sound is positioned relatively to the
			 * listener
			 */
			void setRelativeToListener(bool rel);
			/**
			 * \brief Returns whether the sound is positioned relatively to the
			 * listener
			 */
			bool isRelativeToListener(void);
			
			/**
			 * \brief Attaches the sound to a scene node
			 * \param node Node to attache the sound to
			 * \param adjustspeed If set to true, the speed of the sound is
			 * automatically updated when the scene node moves
			 */
			void attachToSceneNode(SceneNode *node, bool adjustspeed = false);
			
			/**
			 * \brief Updates the sound
			 */
			void doWork(float msecs);
		private:
			//tolua_end
			SoundSource *source;
			unsigned int oalsource;
			unsigned int buffers[3];
			bool cleanup;
			
			SceneNode *node;
			bool adjustspeed;
			//tolua_begin
	};
}
//tolua_end

