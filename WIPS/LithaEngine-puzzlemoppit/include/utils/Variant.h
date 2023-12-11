
#ifndef UTILS_VARIANT_H
#define UTILS_VARIANT_H

#include "litha_internal.h"

namespace utils
{

// a variant is a class that can represent any of several types.
// can be assigned and accessed as if it was one of those types.
// this class is inspired by the Variant class from the zlib licensed CRTMPServer,
// http://www.rtmpd.com
class Variant
{
public:
	enum E_VARIANT_TYPE
	{
		EVT_BOOL = 0,
		EVT_U32,
		EVT_S32,
		EVT_F32,
		EVT_F64,
		EVT_STRING,
		EVT_VECTOR // vector is not implemented for a lot of conversions that don't make sense.
	};
	
private:
	E_VARIANT_TYPE variantType;
	
	union
	{
		bool _bool;
		u32 _u32;
		s32 _s32;
		f32 _f32;
		f64 _f64;
	} data;
	
	// classes can't be in a union
	// these are kept in the same struct so copying of Variant is easier
	// (and I don't forget to copy other things if I expand Variant with more class types)
	struct
	{
		// Ideally, string might be dynamically allocated, with the pointer kept
		// in the union above. But this is simpler, and I'm not bothered about performance.
		core::stringc _str;
		
		// Same for this.
		std::vector<Variant> _vec;
	} moredata;
	
public:
	Variant();
	Variant(const Variant &value);
	Variant(bool value);
	Variant(u32 value);
	Variant(s32 value);
	Variant(f32 value);
	Variant(f64 value);
	Variant(const core::stringc &value);
	Variant(const char *value);
	Variant(const std::vector<Variant> &value);
	template<class Type> Variant(const std::vector<Type> &value)
	{
		variantType = EVT_VECTOR;
		
		moredata._vec.clear();
		
		for (u32 i = 0; i < value.size(); i ++)
			moredata._vec.push_back(value[i]);
	}
	
	operator bool() const;
	operator u32() const;
	operator s32() const;
	operator f32() const;
	operator f64() const;
	operator core::stringc() const;
	// Note: it's safer not have a const char * cast operator in case the Variant goes out of scope.
	// (thus making any pointers to the temporary character array invalid)
	operator std::vector<Variant>() const;
	template<class Type> operator std::vector<Type>() const
	{
		switch (variantType)
		{
		case EVT_VECTOR:
			{
				std::vector<Type> ret;
				
				for (u32 i = 0; i < moredata._vec.size(); i ++)
					ret.push_back(moredata._vec[i].To<Type>());
				
				return ret;
			}
		default:
			FAIL << "invalid variant type (" << variantType << ")";
		}
	}
	
	Variant &operator=(const Variant &value);
	Variant &operator=(bool value);
	Variant &operator=(u32 value);
	Variant &operator=(s32 value);
	Variant &operator=(f32 value);
	Variant &operator=(f64 value);
	Variant &operator=(const core::stringc &value);
	Variant &operator=(const char *value);
	Variant &operator=(const std::vector<Variant> &value);
	template<class Type> Variant &operator=(const std::vector<Type> &value)
	{
		variantType = EVT_VECTOR;
		
		moredata._vec.clear();
		
		for (u32 i = 0; i < value.size(); i ++)
			moredata._vec.push_back(value[i]);
		
		return *this;
	}
	
	bool operator==(const Variant &value) const;
	bool operator==(bool value) const;
	bool operator==(u32 value) const;
	bool operator==(s32 value) const;
	bool operator==(f32 value) const;
	bool operator==(f64 value) const;
	bool operator==(const core::stringc &value) const;
	bool operator==(const char *value) const;
	bool operator==(const std::vector<Variant> &value) const;
	template<class Type> bool operator==(const std::vector<Type> &value) const
	{
		// Only do comparison if this is a vector.
		ASSERT(variantType == EVT_VECTOR);
		//return moredata._vec == Variant(value).moredata._vec;
		// Changed to match behaviour of other comparisons.
		// (we cast *this to the other's type, then compare)
		return std::vector<Type>(*this) == value;
	}
	
	// < operators not tested.
	bool operator<(const Variant &value) const;
	bool operator<(bool value) const;
	bool operator<(u32 value) const;
	bool operator<(s32 value) const;
	bool operator<(f32 value) const;
	bool operator<(f64 value) const;
	// string less-than compares string length
	bool operator<(const core::stringc &value) const;
	bool operator<(const char *value) const;
	
	// > operators not tested.
	bool operator>(const Variant &value) const;
	bool operator>(bool value) const;
	bool operator>(u32 value) const;
	bool operator>(s32 value) const;
	bool operator>(f32 value) const;
	bool operator>(f64 value) const;
	// string greater-than compares string length
	bool operator>(const core::stringc &value) const;
	bool operator>(const char *value) const;
	
	Variant &operator+=(const Variant &value);
	Variant &operator+=(bool value);
	Variant &operator+=(u32 value);
	Variant &operator+=(s32 value);
	Variant &operator+=(f32 value);
	Variant &operator+=(f64 value);
	Variant &operator+=(const core::stringc &value);
	Variant &operator+=(const char *value);
	
	const Variant operator+(const Variant &value) const;
	const Variant operator+(bool value) const;
	const Variant operator+(u32 value) const;
	const Variant operator+(s32 value) const;
	const Variant operator+(f32 value) const;
	const Variant operator+(f64 value) const;
	const Variant operator+(const core::stringc &value) const;
	const Variant operator+(const char *value) const;
	
	// This shift operator is only used to build strings.
	// Forces a conversion of the Variant's existing contents to a string.
	// Then appends the new string value on to the end.
	Variant &operator<<(const Variant &value);
	Variant &operator<<(bool value);
	Variant &operator<<(u32 value);
	Variant &operator<<(s32 value);
	Variant &operator<<(f32 value);
	Variant &operator<<(f64 value);
	Variant &operator<<(const core::stringc &value);
	Variant &operator<<(const char *value);
	
	// Explicitly convert this variant to a particular type.
	// Makes use of the overloaded type cast operators.
	// Useful when the compiler complains about things being ambiguous.
	template<class Type>
	Type To() const
	{
		Type t = *this;
		return t;
	}
	
	// Mostly just useful for debugging.
	E_VARIANT_TYPE GetType();
};

}

#endif

