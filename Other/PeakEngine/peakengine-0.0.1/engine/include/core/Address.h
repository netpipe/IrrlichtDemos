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

#ifndef _ADDRESS_H_
#define _ADDRESS_H_

#include <string>

//tolua_begin
namespace peak
{
	/**
	 * \brief Class to hold a network address.
	 */
	class Address
	{
		public:
			Address();
			Address(const Address &addr);
			Address(std::string addr);
			Address(const char *addr);
			Address(unsigned char ip1, unsigned char ip2, unsigned char ip3,
					unsigned char ip4, unsigned short port);
			~Address();

			/**
			 * \brief Sets the address to a four byte integer.
			 *
			 * On little endian systems you have to use the reverse byte ordering
			 * for the number so that 0x01000079 results in the network address 127.0.0.1.
			 */
			void setAddress(unsigned int addr);
			/**
			 * \brief Returns the address as a four byte integer.
			 * \return Packed address
			 */
			unsigned int getAddress(void);

			/**
			 * \brief Sets the address data.
			 * \param ip1 First part of IP address
			 * \param ip2 Second part of IP address
			 * \param ip3 Third part of IP address
			 * \param ip4 Fourth part of IP address
			 * \param port Port of the remote address
			 */
			void setAddress(unsigned char ip1, unsigned char ip2,
					unsigned char ip3, unsigned char ip4, unsigned short port);

			/**
			 * \brief Parses address string.
			 *
			 * \param addr Address string ("xxx.xxx.xxx.xxx[:xxxxx]")
			 */
			void setAddress(std::string addr);
			/**
			 * \brief Returns address as string.
			 * \param port If set to true, include the port
			 * \return Address string
			 */
			std::string getAddressString(bool port = true);

			/**
			 * \brief Sets the port of the address.
			 * \param port New port
			 */
			void setPort(unsigned short port);
			/**
			 * \brief Returns the port.
			 * \return Port
			 */
			unsigned short getPort(void);

			//tolua_end
			Address &operator=(Address &addr);
			Address &operator=(const Address &addr);
			bool operator==(Address &addr);
			
		private:
			unsigned char address[4];
			unsigned short port;
			//tolua_begin
	};
}
//tolua_end

#endif

