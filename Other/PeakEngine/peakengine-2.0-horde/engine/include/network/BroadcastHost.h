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

#ifndef _BROADCASTHOST_H_
#define _BROADCASTHOST_H_

#include <enet/enet.h>
#include <string>

//tolua_begin
namespace peak
{
	/**
	 * \brief Socket which automatically replies to broadcast messages.
	 */
	class BroadcastHost
	{
		public:
			BroadcastHost();
			~BroadcastHost();

			/**
			 * \brief Initializes the broadcast host.
			 * \param port Port on which the socket listens
			 */
			bool init(int port);
			/**
			 * \brief Destroys the broadcast host.
			 */
			bool shutdown(void);

			/**
			 * \brief Sets the reply of the broadcast host.
			 * \param info Reply string
			 */
			void setInfo(std::string info);
			/**
			 * \brief Returns the used reply string.
			 */
			std::string getInfo(void);

			/**
			 * \brief Listens for incoming data and sends replies.
			 */
			void doWork(void);
		private:
			//tolua_end
			ENetSocket bcastsocket;

			std::string info;
			//tolua_begin
	};
}
//tolua_end

#endif

