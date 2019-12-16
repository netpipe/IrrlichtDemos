
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

#ifndef _TEST1_PLAYER_H_
#define _TEST1_PLAYER_H_

#include "core/Entity.h"
#include "core/EntityFactory.h"

//tolua_begin
class PlayerEntity : public peak::Entity
{
	public:
		PlayerEntity();
		~PlayerEntity();
		
		virtual bool load(std::string name, peak::Buffer *data = 0);
		virtual void update(float msecs);
		
		void setMovement(int forward, int back, int left, int right);
		void setRotation(float yaw);
		
		//tolua_end
	private:
		int forward, back, left, right;
		float yaw;
		//tolua_begin
};
//tolua_end

class PlayerFactory : public peak::EntityFactory
{
	public:
		virtual peak::Entity *create(std::string name)
		{
			return new PlayerEntity;
		}
		static PlayerFactory factory;
	private:
};

#endif

