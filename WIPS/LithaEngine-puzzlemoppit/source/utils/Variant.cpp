
#include "utils/Variant.h"

/*

Note: Some method implementations in here are very similar, and could probably be put into macros.

*/

namespace utils
{

Variant::Variant()
{
	// Default to an empty string
	variantType = EVT_STRING;
	moredata._str = "";
}

Variant::Variant(const Variant &value)
{
	variantType = value.variantType;
	data = value.data;
	moredata = value.moredata;
}

Variant::Variant(bool value)
{
	variantType = EVT_BOOL;
	data._bool = value;
}

Variant::Variant(u32 value)
{
	variantType = EVT_U32;
	data._u32 = value;
}

Variant::Variant(s32 value)
{
	variantType = EVT_S32;
	data._s32 = value;
}

Variant::Variant(f32 value)
{
	variantType = EVT_F32;
	data._f32 = value;
}

Variant::Variant(f64 value)
{
	variantType = EVT_F64;
	data._f64 = value;
}

Variant::Variant(const core::stringc &value)
{
	variantType = EVT_STRING;
	moredata._str = value;
}

Variant::Variant(const char *value)
{
	variantType = EVT_STRING;
	moredata._str = value;
}

Variant::Variant(const std::vector<Variant> &value)
{
	variantType = EVT_VECTOR;
	moredata._vec = value;
}

Variant::operator bool() const
{
	switch (variantType)
	{
	case EVT_BOOL:
		return (bool)data._bool;
	case EVT_U32:
		return (bool)data._u32;
	case EVT_S32:
		return (bool)data._s32;
	case EVT_F32:
		return (bool)data._f32;
	case EVT_F64:
		return (bool)data._f64;
	case EVT_STRING:
		return str::from_bool(moredata._str);
	default:
		FAIL << "invalid variant type (" << variantType << ")";
	}
}

Variant::operator u32() const
{
	switch (variantType)
	{
	case EVT_BOOL:
		return (u32)data._bool;
	case EVT_U32:
		return (u32)data._u32;
	case EVT_S32:
		return (u32)data._s32;
	case EVT_F32:
		return (u32)data._f32;
	case EVT_F64:
		return (u32)data._f64;
	case EVT_STRING:
		return str::from_u32(moredata._str);
	default:
		FAIL << "invalid variant type (" << variantType << ")";
	}
}

Variant::operator s32() const
{
	switch (variantType)
	{
	case EVT_BOOL:
		return (s32)data._bool;
	case EVT_U32:
		return (s32)data._u32;
	case EVT_S32:
		return (s32)data._s32;
	case EVT_F32:
		return (s32)data._f32;
	case EVT_F64:
		return (s32)data._f64;
	case EVT_STRING:
		return str::from_s32(moredata._str);
	default:
		FAIL << "invalid variant type (" << variantType << ")";
	}
}

Variant::operator f32() const
{
	switch (variantType)
	{
	case EVT_BOOL:
		return (f32)data._bool;
	case EVT_U32:
		return (f32)data._u32;
	case EVT_S32:
		return (f32)data._s32;
	case EVT_F32:
		return (f32)data._f32;
	case EVT_F64:
		return (f32)data._f64;
	case EVT_STRING:
		return str::from_f32(moredata._str);
	default:
		FAIL << "invalid variant type (" << variantType << ")";
	}
}

Variant::operator f64() const
{
	switch (variantType)
	{
	case EVT_BOOL:
		return (f64)data._bool;
	case EVT_U32:
		return (f64)data._u32;
	case EVT_S32:
		return (f64)data._s32;
	case EVT_F32:
		return (f64)data._f32;
	case EVT_F64:
		return (f64)data._f64;
	case EVT_STRING:
		return str::from_f64(moredata._str);
	default:
		FAIL << "invalid variant type (" << variantType << ")";
	}
}

Variant::operator core::stringc() const
{
	switch (variantType)
	{
	case EVT_BOOL:
		return str::to(data._bool);
	case EVT_U32:
		return str::to(data._u32);
	case EVT_S32:
		return str::to(data._s32);
	case EVT_F32:
		return str::to(data._f32);
	case EVT_F64:
		return str::to(data._f64);
	case EVT_STRING:
		return moredata._str;
	default:
		FAIL << "invalid variant type (" << variantType << ")";
	}
}

Variant::operator std::vector<Variant>() const
{
	switch (variantType)
	{
	case EVT_VECTOR:
		return moredata._vec;
	default:
		FAIL << "invalid variant type (" << variantType << ")";
	}
}

Variant &Variant::operator=(const Variant &value)
{
	if (this == &value)
		return *this;
	
	variantType = value.variantType;
	data = value.data;
	moredata = value.moredata;
	return *this;
}

Variant &Variant::operator=(bool value)
{
	variantType = EVT_BOOL;
	data._bool = value;
	return *this;
}

Variant &Variant::operator=(u32 value)
{
	variantType = EVT_U32;
	data._u32 = value;
	return *this;
}

Variant &Variant::operator=(s32 value)
{
	variantType = EVT_S32;
	data._s32 = value;
	return *this;
}

Variant &Variant::operator=(f32 value)
{
	variantType = EVT_F32;
	data._f32 = value;
	return *this;
}

Variant &Variant::operator=(f64 value)
{
	variantType = EVT_F64;
	data._f64 = value;
	return *this;
}

Variant &Variant::operator=(const core::stringc &value)
{
	variantType = EVT_STRING;
	moredata._str = value;
	return *this;
}

Variant &Variant::operator=(const char *value)
{
	variantType = EVT_STRING;
	moredata._str = value;
	return *this;
}

Variant &Variant::operator=(const std::vector<Variant> &value)
{
	variantType = EVT_VECTOR;
	moredata._vec = value;
	return *this;
}

bool Variant::operator==(const Variant &value) const
{
	switch (variantType)
	{
	case EVT_BOOL:
		return (bool)(*this) == (bool)value;
	case EVT_U32:
		return (u32)(*this) == (u32)value;
	case EVT_S32:
		return (s32)(*this) == (s32)value;
	case EVT_F32:
		return (f32)(*this) == (f32)value;
	case EVT_F64:
		return (f64)(*this) == (f64)value;
	case EVT_STRING:
		return To<core::stringc>() == value.To<core::stringc>();
	case EVT_VECTOR:
		ASSERT(value.variantType == EVT_VECTOR);
		return moredata._vec == value.moredata._vec;
	default:
		FAIL << "invalid variant type (" << variantType << ")";
	}
}

bool Variant::operator==(bool value) const
{
	return (bool)(*this) == value;
}

bool Variant::operator==(u32 value) const
{
	return (u32)(*this) == value;
}

bool Variant::operator==(s32 value) const
{
	return (s32)(*this) == value;
}

bool Variant::operator==(f32 value) const
{
	return (f32)(*this) == value;
}

bool Variant::operator==(f64 value) const
{
	return (f64)(*this) == value;
}

bool Variant::operator==(const core::stringc &value) const
{
	return To<core::stringc>() == value;
}

bool Variant::operator==(const char *value) const
{
	return To<core::stringc>() == core::stringc(value);
}

bool Variant::operator==(const std::vector<Variant> &value) const
{
	// Only do comparison if this is a vector.
	ASSERT(variantType == EVT_VECTOR);
	return moredata._vec == value;
}

bool Variant::operator<(const Variant &value) const
{
	switch (variantType)
	{
	case EVT_BOOL:
		return (bool)(*this) < (bool)value;
	case EVT_U32:
		return (u32)(*this) < (u32)value;
	case EVT_S32:
		return (s32)(*this) < (s32)value;
	case EVT_F32:
		return (f32)(*this) < (f32)value;
	case EVT_F64:
		return (f64)(*this) < (f64)value;
	case EVT_STRING:
		return To<core::stringc>().size() < value.To<core::stringc>().size();
	default:
		FAIL << "invalid variant type (" << variantType << ")";
	}
}

bool Variant::operator<(bool value) const
{
	return (bool)(*this) < value;
}

bool Variant::operator<(u32 value) const
{
	return (u32)(*this) < value;
}

bool Variant::operator<(s32 value) const
{
	return (s32)(*this) < value;
}

bool Variant::operator<(f32 value) const
{
	return (f32)(*this) < value;
}

bool Variant::operator<(f64 value) const
{
	return (f64)(*this) < value;
}

bool Variant::operator<(const core::stringc &value) const
{
	return To<core::stringc>().size() < value.size();
}

bool Variant::operator<(const char *value) const
{
	return To<core::stringc>().size() < core::stringc(value).size();
}

bool Variant::operator>(const Variant &value) const
{
	switch (variantType)
	{
	case EVT_BOOL:
		return (bool)(*this) > (bool)value;
	case EVT_U32:
		return (u32)(*this) > (u32)value;
	case EVT_S32:
		return (s32)(*this) > (s32)value;
	case EVT_F32:
		return (f32)(*this) > (f32)value;
	case EVT_F64:
		return (f64)(*this) > (f64)value;
	case EVT_STRING:
		return To<core::stringc>().size() > value.To<core::stringc>().size();
	default:
		FAIL << "invalid variant type (" << variantType << ")";
	}
}

bool Variant::operator>(bool value) const
{
	return (bool)(*this) > value;
}

bool Variant::operator>(u32 value) const
{
	return (u32)(*this) > value;
}

bool Variant::operator>(s32 value) const
{
	return (s32)(*this) > value;
}

bool Variant::operator>(f32 value) const
{
	return (f32)(*this) > value;
}

bool Variant::operator>(f64 value) const
{
	return (f64)(*this) > value;
}

bool Variant::operator>(const core::stringc &value) const
{
	return To<core::stringc>().size() > value.size();
}

bool Variant::operator>(const char *value) const
{
	return To<core::stringc>().size() > core::stringc(value).size();
}

Variant &Variant::operator+=(const Variant &value)
{
	switch (variantType)
	{
	case EVT_BOOL:
		data._bool += (bool)value;
		break;
	case EVT_U32:
		data._u32 += (u32)value;
		break;
	case EVT_S32:
		data._s32 += (s32)value;
		break;
	case EVT_F32:
		data._f32 += (f32)value;
		break;
	case EVT_F64:
		data._f64 += (f64)value;
		break;
	case EVT_STRING:
		moredata._str += value.To<core::stringc>();
		break;
	default:
		FAIL << "invalid variant type (" << variantType << ")";
	}
	return *this;
}

Variant &Variant::operator+=(bool value)
{
	switch (variantType)
	{
	case EVT_BOOL:
		data._bool += (bool)value;
		break;
	case EVT_U32:
		data._u32 += (u32)value;
		break;
	case EVT_S32:
		data._s32 += (s32)value;
		break;
	case EVT_F32:
		data._f32 += (f32)value;
		break;
	case EVT_F64:
		data._f64 += (f64)value;
		break;
	case EVT_STRING:
		moredata._str += str::to(value);
		break;
	default:
		FAIL << "invalid variant type (" << variantType << ")";
	}
	return *this;
}

Variant &Variant::operator+=(u32 value)
{
	switch (variantType)
	{
	case EVT_BOOL:
		data._bool += (bool)value;
		break;
	case EVT_U32:
		data._u32 += (u32)value;
		break;
	case EVT_S32:
		data._s32 += (s32)value;
		break;
	case EVT_F32:
		data._f32 += (f32)value;
		break;
	case EVT_F64:
		data._f64 += (f64)value;
		break;
	case EVT_STRING:
		moredata._str += str::to(value);
		break;
	default:
		FAIL << "invalid variant type (" << variantType << ")";
	}
	return *this;
}

Variant &Variant::operator+=(s32 value)
{
	switch (variantType)
	{
	case EVT_BOOL:
		data._bool += (bool)value;
		break;
	case EVT_U32:
		data._u32 += (u32)value;
		break;
	case EVT_S32:
		data._s32 += (s32)value;
		break;
	case EVT_F32:
		data._f32 += (f32)value;
		break;
	case EVT_F64:
		data._f64 += (f64)value;
		break;
	case EVT_STRING:
		moredata._str += str::to(value);
		break;
	default:
		FAIL << "invalid variant type (" << variantType << ")";
	}
	return *this;
}

Variant &Variant::operator+=(f32 value)
{
	switch (variantType)
	{
	case EVT_BOOL:
		data._bool += (bool)value;
		break;
	case EVT_U32:
		data._u32 += (u32)value;
		break;
	case EVT_S32:
		data._s32 += (s32)value;
		break;
	case EVT_F32:
		data._f32 += (f32)value;
		break;
	case EVT_F64:
		data._f64 += (f64)value;
		break;
	case EVT_STRING:
		moredata._str += str::to(value);
		break;
	default:
		FAIL << "invalid variant type (" << variantType << ")";
	}
	return *this;
}

Variant &Variant::operator+=(f64 value)
{
	switch (variantType)
	{
	case EVT_BOOL:
		data._bool += (bool)value;
		break;
	case EVT_U32:
		data._u32 += (u32)value;
		break;
	case EVT_S32:
		data._s32 += (s32)value;
		break;
	case EVT_F32:
		data._f32 += (f32)value;
		break;
	case EVT_F64:
		data._f64 += (f64)value;
		break;
	case EVT_STRING:
		moredata._str += str::to(value);
		break;
	default:
		FAIL << "invalid variant type (" << variantType << ")";
	}
	return *this;
}

Variant &Variant::operator+=(const core::stringc &value)
{
	switch (variantType)
	{
	case EVT_BOOL:
		data._bool += str::from_bool(value);
		break;
	case EVT_U32:
		data._u32 += str::from_u32(value);
		break;
	case EVT_S32:
		data._s32 += str::from_s32(value);
		break;
	case EVT_F32:
		data._f32 += str::from_f32(value);
		break;
	case EVT_F64:
		data._f64 += str::from_f64(value);
		break;
	case EVT_STRING:
		moredata._str += value;
		break;
	default:
		FAIL << "invalid variant type (" << variantType << ")";
	}
	return *this;
}

Variant &Variant::operator+=(const char *value)
{
	switch (variantType)
	{
	case EVT_BOOL:
		data._bool += str::from_bool(value);
		break;
	case EVT_U32:
		data._u32 += str::from_u32(value);
		break;
	case EVT_S32:
		data._s32 += str::from_s32(value);
		break;
	case EVT_F32:
		data._f32 += str::from_f32(value);
		break;
	case EVT_F64:
		data._f64 += str::from_f64(value);
		break;
	case EVT_STRING:
		moredata._str += value;
		break;
	default:
		FAIL << "invalid variant type (" << variantType << ")";
	}
	return *this;
}

const Variant Variant::operator+(const Variant &value) const
{
	Variant result = *this;
	result += value;
	return result;
}

const Variant Variant::operator+(bool value) const
{
	Variant result = *this;
	result += value;
	return result;
}

const Variant Variant::operator+(u32 value) const
{
	Variant result = *this;
	result += value;
	return result;
}

const Variant Variant::operator+(s32 value) const
{
	Variant result = *this;
	result += value;
	return result;
}

const Variant Variant::operator+(f32 value) const
{
	Variant result = *this;
	result += value;
	return result;
}

const Variant Variant::operator+(f64 value) const
{
	Variant result = *this;
	result += value;
	return result;
}

const Variant Variant::operator+(const core::stringc &value) const
{
	Variant result = *this;
	result += value;
	return result;
}

const Variant Variant::operator+(const char *value) const
{
	Variant result = *this;
	result += value;
	return result;
}

Variant &Variant::operator<<(const Variant &value)
{
	return *this << value.To<core::stringc>();
}

Variant &Variant::operator<<(bool value)
{
	return *this << str::to(value);
}

Variant &Variant::operator<<(u32 value)
{
	return *this << str::to(value);
}

Variant &Variant::operator<<(s32 value)
{
	return *this << str::to(value);
}

Variant &Variant::operator<<(f32 value)
{
	return *this << str::to(value);
}

Variant &Variant::operator<<(f64 value)
{
	return *this << str::to(value);
}

Variant &Variant::operator<<(const core::stringc &value)
{
	// Convert this to a string if it isn't one.
	if (variantType != EVT_STRING)
	{
		moredata._str = To<core::stringc>();
		
		// must set new type AFTER the call to To()
		variantType = EVT_STRING;
	}
	
	// Append the new value, as a string.
	moredata._str += value;
	
	return *this;
}

Variant &Variant::operator<<(const char *value)
{
	return *this << core::stringc(value);
}

// Mostly just useful for debugging.
Variant::E_VARIANT_TYPE Variant::GetType()
{
	return variantType;
}

}
