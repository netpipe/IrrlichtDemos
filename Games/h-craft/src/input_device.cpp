// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "input_device.h"
#include <cassert>
#include <cstring>

namespace
{
	const char *g_InputDeviceTypeXmlNames[IDP_MAX] =
	{
		"NO",
		"KB",
		"MS",
		"JS",
		"TO"
	};
}

const char* IInputDevice::GetXmlNameForDeviceType(InputDeviceType type_)
{
    assert( type_ < IDP_MAX );
    if ( type_ < IDP_MAX )
        return g_InputDeviceTypeXmlNames[type_];
    return NULL;
}

InputDeviceType IInputDevice::GetDeviceTypeForXmlName(const char *name_)
{
    if ( !name_ || !strlen(name_) )
    {
        assert(!"IInputDevice::GetDeviceTypeForXmlName - no device name given");
        return IDP_MAX;
    }
    for ( int i=0; i<IDP_MAX; ++i )
    {
        if ( 0 == strcmp(name_, g_InputDeviceTypeXmlNames[i]) )
        {
            return (InputDeviceType)i;
        }
    }

    assert(!"IInputDevice::GetDeviceTypeForXmlName - unknown device name");
    return IDP_MAX;
}

