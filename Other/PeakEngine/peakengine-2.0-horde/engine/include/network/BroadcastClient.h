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

#ifndef _BROADCASTCLIENT_H_
#define _BROADCASTCLIENT_H_

#include <enet/enet.h>
#include <string>
#include <vector>

#include "core/Address.h"

//tolua_begin
namespace peak
{
	class Script;
	
	/**
	 * \brief Socket which sends broadcast messages at a fixed interval to get the
	 * address of local network servers.
	 */
	class BroadcastClient
	{
		public:
			BroadcastClient();
			~BroadcastClient();

			/**
			 * \brief Starts broadcasting.
			 * \param port Port to send messages at
			 */
			void start(int port);
			/**
			 * \brief Stops broadcasting
			 */
			void stop(void);

			/**
			 * \brief Clears server list.
			 */
			void clearList(void);

			/**
			 * \brief Returns the number of found servers.
			 */
			int getServerCount(void);
			/**
			 * \brief Returns the server info of one server.
			 * \param index Index of the server, can be in the range 0..getServerCount().
			 * \return Server reply
			 */
			std::string getServerInfo(int index);
			/**
			 * \brief Returns the address of one server.
			 * \param index Index of the server, can be in the range 0..getServerCount().
			 * \return Server address
			 */
			Address getServerAddress(int index);

			/**
			 * \brief Sets a callback function which gets called everytime the
			 * server list changes.
			 * \param script Lua script which defines the function
			 * \param function Function to call
			 */
			void setCallback(Script *script, std::string function);

			/**
			 * \brief Sends a broadcast message if needed, handles incoming responses.
			 * \param msecs Passed time since last call of this function
			 */
			void doWork(float msecs = 0);

			/**
			 * \brief Updates all broadcast clients.
			 * \param msecs Passed time since last call of this function
			 */
			static void doAllWork(float msecs);
		private:
			//tolua_end
			ENetSocket socket;
			int port;

			float updatetime;

			Script *cbscript;
			std::string cbfunc;

			std::vector<std::string> serverinfo;
			std::vector<Address> serveraddr;

			static std::vector<BroadcastClient*> bcclients;
			//tolua_begin
	};
}
//tolua_end

#endif

