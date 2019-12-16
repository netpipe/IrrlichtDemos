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

#include "core/Address.h"
#include <cstring>
#include <cstdlib>
#include <stdio.h>
namespace peak
{
	Address::Address()
	{
		address[0] = 0;
		address[1] = 0;
		address[2] = 0;
		address[3] = 0;
		port = 0;
	}
	Address::Address(const Address &addr)
	{
		address[0] = addr.address[0];
		address[1] = addr.address[1];
		address[2] = addr.address[2];
		address[3] = addr.address[3];
		port = addr.port;
	}
	Address::Address(std::string addr)
	{
		address[0] = 0;
		address[1] = 0;
		address[2] = 0;
		address[3] = 0;
		port = 0;
		setAddress(addr);
	}
	Address::Address(const char *addr)
	{
		address[0] = 0;
		address[1] = 0;
		address[2] = 0;
		address[3] = 0;
		port = 0;
		setAddress(std::string(addr));
	}
	Address::Address(unsigned char ip1, unsigned char ip2, unsigned char ip3,
			unsigned char ip4, unsigned short port)
	{
		address[0] = ip1;
		address[1] = ip2;
		address[2] = ip3;
		address[3] = ip4;
		this->port = port;
	}
	Address::~Address()
	{
	}

	void Address::setAddress(unsigned int addr)
	{
		char *addrptr = (char*)&addr;
		address[0] = addrptr[0];
		address[1] = addrptr[1];
		address[2] = addrptr[2];
		address[3] = addrptr[3];
	}
	unsigned int Address::getAddress(void)
	{
		unsigned int addr;
		char *addrptr = (char*)&addr;
		addrptr[0] = address[0];
		addrptr[1] = address[1];
		addrptr[2] = address[2];
		addrptr[3] = address[3];
		return addr;
	}

	void Address::setAddress(unsigned char ip1, unsigned char ip2,
			unsigned char ip3, unsigned char ip4, unsigned short port)
	{
		address[0] = ip1;
		address[1] = ip2;
		address[2] = ip3;
		address[3] = ip4;
		this->port = port;
	}

	void Address::setAddress(std::string addr)
	{
		char str[30];
		strcpy(str, addr.c_str());
		char *pos = str;
		char *end = pos;
		address[0] = strtol(pos, &end, 10);
		pos = end + 1;
		address[1] = strtol(pos, &end, 10);
		pos = end + 1;
		address[2] = strtol(pos, &end, 10);
		pos = end + 1;
		address[3] = strtol(pos, &end, 10);
		if (*end == 0)
			return;
		pos = end + 1;
		port = strtol(pos, &end, 10);
	}
	std::string Address::getAddressString(bool port)
	{
		char tmp[21];
		if (port)
		{
			sprintf(tmp, "%d.%d.%d.%d:%d", address[0], address[1], address[2],
					address[3], this->port);
		}
		else
		{
			sprintf(tmp, "%d.%d.%d.%d", address[0], address[1], address[2],
					address[3]);
		}
		return tmp;
	}

	void Address::setPort(unsigned short port)
	{
		this->port = port;
	}
	unsigned short Address::getPort(void)
	{
		return port;
	}

	Address &Address::operator=(Address &addr)
	{
		address[0] = addr.address[0];
		address[1] = addr.address[1];
		address[2] = addr.address[2];
		address[3] = addr.address[3];
		port = addr.port;
		return *this;
	}
	Address &Address::operator=(const Address &addr)
	{
		address[0] = addr.address[0];
		address[1] = addr.address[1];
		address[2] = addr.address[2];
		address[3] = addr.address[3];
		port = addr.port;
		return *this;
	}
	bool Address::operator==(Address &addr)
	{
		return ((address[0] == addr.address[0])
				&& (address[1] == addr.address[1])
				&& (address[2] == addr.address[2])
				&& (address[3] == addr.address[3])
				&& (port == addr.port));

	}
}

