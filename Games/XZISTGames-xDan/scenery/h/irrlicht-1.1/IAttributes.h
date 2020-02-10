// Copyright (C) 2002-2006 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_ATTRIBUTES_H_INCLUDED__
#define __I_ATTRIBUTES_H_INCLUDED__

#include "IUnknown.h"
#include "SColor.h"
#include "vector3d.h"
#include "irrString.h"
#include "irrArray.h"
#include "IXMLReader.h"

namespace irr
{
namespace video
{
	class ITexture;
}
namespace io
{
	class IXMLWriter;

//! Types of attributes available for IAttributes
enum E_ATTRIBUTE_TYPE
{
	// integer attribute
	EAT_INT = 0,

	// float attribute
	EAT_FLOAT,

	// string attribute
	EAT_STRING,

	// boolean attribute
	EAT_BOOL,

	// enumeration attribute
	EAT_ENUM,

	// color attribute
	EAT_COLOR,

	// floating point color attribute
	EAT_COLORF,

	// 3d vector attribute
	EAT_VECTOR3D,

	// binary data attribute
	EAT_BINARY,

	// texture reference attribute
	EAT_TEXTURE,

	// unknown attribute
	EAT_UNKNOWN
};

//! Provides a generic interface for attributes and their values and the possiblity to serialize them
class IAttributes : public virtual IUnknown
{
public:

	//! Removes all attributes
	virtual void clear() = 0;

	//! Adds an attribute as integer
	virtual void addInt(const c8* attributeName, s32 value) = 0;

	//! Adds an attribute as float
	virtual void addFloat(const c8* attributeName, f32 value) = 0;

	//! Adds an attribute as string
	virtual void addString(const c8* attributeName, const char* value) = 0;

	//! Adds an attribute as bool
	virtual void addBool(const c8* attributeName, bool value) = 0;

	//! Adds an attribute as enum
	virtual void addEnum(const c8* attributeName, const char* enumValue, const char* const* enumerationLiterals) = 0;

	//! Adds an attribute as enum
	virtual void addEnum(const c8* attributeName, s32 enumValue, const char* const* enumerationLiterals) = 0;

	//! Adds an attribute as color
	virtual void addColor(const c8* attributeName, video::SColor value) = 0;

	//! Adds an attribute as floating point color
	virtual void addColorf(const c8* attributeName, video::SColorf value) = 0;

	//! Adds an attribute as 3d vector
	virtual void addVector3d(const c8* attributeName, core::vector3df value) = 0;

	//! Adds an attribute as binary data
	virtual void addBinary(const c8* attributeName, void* data, s32 dataSizeInBytes) = 0;

	//! Adds an attribute as texture reference
	virtual void addTexture(const c8* attributeName, video::ITexture* texture) = 0;

	//! Returns amount of attributes in this collection of attributes.
	virtual s32 getAttributeCount() = 0;

	//! Returns attribute name by index. 
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual const c8* getAttributeName(s32 index) = 0;

	//! Returns the type of an attribute
	//! \param attributeName: Name for the attribute
	virtual E_ATTRIBUTE_TYPE getAttributeType(const c8* attributeName) = 0;

	//! Returns attribute type by index. 
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual E_ATTRIBUTE_TYPE getAttributeType(s32 index) = 0;

	//! Sets an attribute value as string. 
	//! \param attributeName: Name for the attribute
	//! \param value: Value for the attribute. Set this to 0 to delete the attribute
	virtual void setAttribute(const c8* attributeName, const c8* value) = 0;

	//! Gets an attribute as string.
	//! \param attributeName: Name of the attribute to get.
	//! \return Returns value of the attribute previously set by setAttribute() 
	//! or 0 if attribute is not set.
	virtual core::stringc getAttributeAsString(const c8* attributeName) = 0;

	//! Gets an attribute as string.
	//! \param attributeName: Name of the attribute to get.
	//! \param target: Buffer where the string is copied to.
	virtual void getAttributeAsString(const c8* attributeName, char* target) = 0;

	//! Returns attribute value as string by index. 
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual core::stringc getAttributeAsString(s32 index) = 0;	

	//! Sets an attribute as boolean value
	virtual void setAttribute(const c8* attributeName, bool value) = 0;

	//! Gets an attribute as boolean value
	//! \param attributeName: Name of the attribute to get.
	//! \return Returns value of the attribute previously set by setAttribute() 
	virtual bool getAttributeAsBool(const c8* attributeName) = 0;

    //! Gets an attribute as boolean value
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual bool getAttributeAsBool(s32 index) = 0;

	//! Sets an attribute as integer value
	virtual void setAttribute(const c8* attributeName, s32 value) = 0;

	//! Gets an attribute as integer value
	//! \param attributeName: Name of the attribute to get.
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual s32 getAttributeAsInt(const c8* attributeName) = 0;

	//! Gets an attribute as integer value
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual s32 getAttributeAsInt(s32 index) = 0;

	//! Sets a attribute as float value
	virtual void setAttribute(const c8* attributeName, f32 value) = 0;

	//! Gets an attribute as float value
	//! \param attributeName: Name of the attribute to get.
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual f32 getAttributeAsFloat(const c8* attributeName) = 0;

	//! Gets an attribute as float value
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual f32 getAttributeAsFloat(s32 index) = 0;

	//! Sets a attribute as color
	virtual void setAttribute(const c8* attributeName, video::SColor color) = 0;

	//! Gets an attribute as color
	//! \param attributeName: Name of the attribute to get.
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual video::SColor getAttributeAsColor(const c8* attributeName) = 0;

	//! Gets an attribute as color
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual video::SColor getAttributeAsColor(s32 index) = 0;

	//! Sets a attribute as floating point color
	virtual void setAttribute(const c8* attributeName, video::SColorf color) = 0;

	//! Gets an attribute as floating point color
	//! \param attributeName: Name of the attribute to get.
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual video::SColorf getAttributeAsColorf(const c8* attributeName) = 0;

	//! Gets an attribute as floating point color
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual video::SColorf getAttributeAsColorf(s32 index) = 0;

	//! Sets a attribute as vector
	virtual void setAttribute(const c8* attributeName, core::vector3df v) = 0;

	//! Gets an attribute as floating point color
	//! \param attributeName: Name of the attribute to get.
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual core::vector3df getAttributeAsVector3d(const c8* attributeName) = 0;

	//! Gets an attribute as floating point color
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual core::vector3df getAttributeAsVector3d(s32 index) = 0;

	//! Sets an attribute as binary data
	virtual void setAttribute(const c8* attributeName, void* data, s32 dataSizeInBytes ) = 0;

	//! Gets an attribute as binary data
	//! \param attributeName: Name of the attribute to get.
	virtual void getAttributeAsBinaryData(const c8* attributeName, void* outData, s32 maxSizeInBytes) = 0;

	//! Gets an attribute as binary data
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual void getAttributeAsBinaryData(s32 index, void* outData, s32 maxSizeInBytes) = 0;

	//! Sets an attribute as enumeration
	virtual void setAttribute(const c8* attributeName, const char* enumValue, const char* const* enumerationLiterals) = 0;

	//! Gets an attribute as enumeration
	//! \param attributeName: Name of the attribute to get.
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual const char* getAttributeAsEnumeration(const c8* attributeName) = 0;

	//! Gets an attribute as enumeration
	//! \param attributeName: Name of the attribute to get.
	//! \param enumerationLiteralsToUse: Use these enumeration literals to get the index value instead of the set ones.
	//! This is useful when the attribute list maybe was read from an xml file, and only contains the enumeration string, but
	//! no information about its index.
	//! \return Returns value of the attribute previously set by setAttribute()
	virtual s32 getAttributeAsEnumeration(const c8* attributeName, const char* const* enumerationLiteralsToUse) = 0;

	//! Gets an attribute as enumeration
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual s32 getAttributeAsEnumeration(s32 index, const char* const* enumerationLiteralsToUse) = 0;

	//! Gets an attribute as enumeration
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual const char* getAttributeAsEnumeration(s32 index) = 0;

	//! Gets the list of enumeration literals of an enumeration attribute 
	//! \param attributeName: Name of the attribute to get.
	virtual void getAttributeEnumerationLiteralsOfEnumeration(const c8* attributeName, core::array<core::stringc>& outLiterals) = 0;

	//! Gets the list of enumeration literals of an enumeration attribute 
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual void getAttributeEnumerationLiteralsOfEnumeration(s32 index, core::array<core::stringc>& outLiterals) = 0;

	//! Sets an attribute as texture reference
	virtual void setAttribute(const c8* attributeName, video::ITexture* texture ) = 0;

	//! Gets an attribute as texture reference
	//! \param attributeName: Name of the attribute to get.
	virtual video::ITexture* getAttributeAsTexture(const c8* attributeName) = 0;

	//! Gets an attribute as texture reference
	//! \param index: Index value, must be between 0 and getAttributeCount()-1.
	virtual video::ITexture* getAttributeAsTexture(s32 index) = 0;

	//! Returns if an attribute with a name exists
	virtual bool existsAttribute(const c8* attributeName) = 0;

	//! Returns attribute index from name, -1 if not found
	virtual s32 findAttribute(const c8* attributeName) = 0;

	//! Sets an attribute value as string. 
	//! \param attributeName: Name for the attribute
	virtual void setAttribute(s32 index, const c8* value) = 0;

	//! Sets an attribute as boolean value
	virtual void setAttribute(s32 index, bool value) = 0;

	//! Sets an attribute as integer value
	virtual void setAttribute(s32 index, s32 value) = 0;

	//! Sets an attribute as float value
	virtual void setAttribute(s32 index, f32 value) = 0;

	//! Sets an attribute as color
	virtual void setAttribute(s32 index, video::SColor color) = 0;

	//! Sets an attribute as floating point color
	virtual void setAttribute(s32 index, video::SColorf color) = 0;

	//! Sets an attribute as vector
	virtual void setAttribute(s32 index, core::vector3df v) = 0;

	//! Sets an attribute as binary data
	virtual void setAttribute(s32 index, void* data, s32 dataSizeInBytes ) = 0;

	//! Sets an attribute as enumeration
	virtual void setAttribute(s32 index, const char* enumValue, const char* const* enumerationLiterals) = 0;

	//! Sets an attribute as texture reference
	virtual void setAttribute(s32 index, video::ITexture* texture) = 0;

	//! Reads attributes from a xml file.
	//! \param readCurrentElementOnly: If set to true, reading only works if current element has the name 'attributes'.
	//! IF set to false, the first appearing list attributes are read.
	virtual bool read(irr::io::IXMLReader* reader, bool readCurrentElementOnly=false) = 0;

	//! Write these attributes into a xml file
	virtual bool write(io::IXMLWriter* writer) = 0;
};

} // end namespace io
} // end namespace irr

#endif



