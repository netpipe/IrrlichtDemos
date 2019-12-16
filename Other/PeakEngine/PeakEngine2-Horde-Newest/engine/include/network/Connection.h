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

#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "core/Buffer.h"

#include <enet/enet.h>
#include <queue>

//tolua_begin
namespace peak
{
	class Timer;
	
	/**
	 * \brief Base class for any client/server connection
	 */
	class Connection
	{
		public:
			Connection(ENetPeer *peer);
			virtual ~Connection();

			/**
			 * \brief Destroys the connection and frees all data.
			 */
			virtual void destroy(void);

			/**
			 * \brief Closes the connection.
			 */
			virtual void disconnect(void);
			/**
			 * \brief Reads whether the connection is established or not.
			 * \return Returns false if the connection was closed.
			 */
			virtual bool isConnected(void);

			/**
			 * \brief Returns whether there is incoming data waiting to be read.
			 * \return Returns true if there is incoming data.
			 */
			virtual bool hasNewData(void);
			/**
			 * \brief Reads incoming data.
			 *
			 * This function returns a buffer which has to be deleted by the user.
			 * \return Incoming data
			 */
			virtual Buffer *readData(void);
			/**
			 * \brief Sends data.
			 *
			 * You must not delete the buffer for the connection takes care of it.
			 * \param data Pointer to a Buffer created via new
			 * \param reliable If set to true, a reliable connection is used
			 * \return Returns false if the data could not be sent.
			 */
			virtual bool sendData(Buffer *data, bool reliable = true);

			/**
			 * \brief Injects incoming data.
			 *
			 * Because of the design of enet, connections do not receive the data
			 * themselves.
			 *
			 * For internal use only.
			 * \param data Incoming data
			 */
			virtual void injectData(Buffer *data);
			/**
			 * \brief Returns the peer structure used by enet.
			 *
			 * For internal use only.
			 * \return Peer structure
			 */
			virtual ENetPeer *getPeer(void);
		private:
			//tolua_end
			ENetPeer *peer;

			std::queue<Buffer*> received;
			
			Timer *speedtimer;
			int sent;
			int relsent;
			//tolua_begin
	};
}
//tolua_end

#endif

