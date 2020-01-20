
#include "utils/str.h"

namespace utils
{
namespace str
{

// returns true if the given character occurs in the charList string
bool char_occurs(char c, const core::stringc &charList)
{
	for (u32 i = 0; i < charList.size(); i ++)
	{
		if (charList[i] == c)
			return true;
	}
	return false;
}

core::stringc rtrim(const core::stringc &str, const core::stringc &chars)
{
	if ( !char_occurs(str.lastChar(), chars) )
		return str;
	
	if (str.size() == 0)
		return str;
	
	u32 i = str.size();
	
	while ( i > 0 && char_occurs(str[i-1], chars) )
		i --;
	
	if (i == 0)
		return core::stringc();
	
	return str.subString(0, i);
}

core::stringc ltrim(const core::stringc &str, const core::stringc &chars)
{
	u32 i = 0;
	
	while ( i < str.size() && char_occurs(str[i], chars) )
		i ++;
	
	return str.subString(i, str.size()-i);
}

core::stringc trim(const core::stringc &str, const core::stringc &chars)
{
	return ltrim( rtrim(str, chars), chars );
}

std::vector<core::stringc> explode(const core::stringc &delimiter, const core::stringc &str)
{
	std::vector<core::stringc> results;
	
	// Empty string?
	if (str.size() == 0)
	{
		return results;
	}
	
	// Empty delimiter?
	if (delimiter.size() == 0)
	{
		results.push_back(str);
		return results;
	}
	
	u32 i = 0;
	
	while (1)
	{
		// Eat up delimiters
		while (str.subString(i,delimiter.size()) == delimiter && i < str.size())
			i += delimiter.size();
		
		// Done?
		if (i >= str.size())
			break;
		
		// No delimiter? Start new string.
		u32 start = i;
		
		// Find next delimiter
		while (str.subString(i,delimiter.size()) != delimiter && i < str.size())
			i ++;
		
		// Found a string!?
		results.push_back(str.subString(start, i-start));
		
		// Done?
		if (i == str.size())
			break;
	}
	
	return results;
}

std::vector<core::stringc> explode_chars(const core::stringc &delimiterChars, const core::stringc &str)
{
	std::vector<core::stringc> results;
	
	// Empty string?
	if (str.size() == 0)
	{
		return results;
	}
	
	// Empty delimiter?
	if (delimiterChars.size() == 0)
	{
		results.push_back(str);
		return results;
	}
	
	u32 i = 0;
	
	while (1)
	{
		// Eat up delimiters
		while (char_occurs(str[i], delimiterChars) && i < str.size())
			i ++;
		
		// Done?
		if (i >= str.size())
			break;
		
		// No delimiter? Start new string.
		u32 start = i;
		
		// Find next delimiter
		while (!char_occurs(str[i], delimiterChars) && i < str.size())
			i ++;
		
		// Found a string!?
		results.push_back(str.subString(start, i-start));
		
		// Done?
		if (i == str.size())
			break;
	}
	
	return results;
}

bool contains_char(const core::stringc &str, char c)
{
	return char_occurs(c, str);
}

std::vector<core::stringc> explode_at_assignment(const core::stringc &str)
{
	std::vector<core::stringc> result;
	result.push_back("");
	result.push_back("");
	
	ASSERT(result.size() == 2);
	
	std::vector<core::stringc> parts = str::explode("=", str);
	
	if (parts.size() >= 2)
	{
		// Remove any whitespace
		result[0] = str::trim(parts[0]);
		result[1] = str::trim(parts[1]);
		
		return result;
	}
	
	return result;
}

core::stringc to(bool value)
{
	return to((u32)value);
}

core::stringc to(u32 value)
{
	core::stringc str;
	str += value;
	return str;
}

core::stringc to(s32 value)
{
	core::stringc str;
	str += value;
	return str;
}

core::stringc to(f32 value)
{
	core::stringc str;
	str += value;
	
	// Must contain a decimal point...
	ASSERT( char_occurs('.', str) );
	
	// If there is at least one trailing zero...
	if (str.lastChar() == '0')
	{
		// Trim any trailing zeroes
		str = rtrim(str, "0");
		
		// If the last char is a decimal point, add back a single zero.
		if (str.lastChar() == '.')
			str += '0';
	}
	
	return str;
}

core::stringc to(f64 value)
{
	core::stringc str;
	str += value;
	
	// Must contain a decimal point...
	ASSERT( char_occurs('.', str) );
	
	// If there is at least one trailing zero...
	if (str.lastChar() == '0')
	{
		// Trim any trailing zeroes
		str = rtrim(str, "0");
		
		// If the last char is a decimal point, add back a single zero.
		if (str.lastChar() == '.')
			str += '0';
	}
	
	return str;
}

bool from_bool(const core::stringc &value)
{
	if (value.subString(0,4).equals_ignore_case("true"))
		return true;
	
	return (bool)strtol(value.c_str(), NULL, 0);
}

u32 from_u32(const core::stringc &value)
{
	return (u32)strtol(value.c_str(), NULL, 0);
}

s32 from_s32(const core::stringc &value)
{
	return (s32)strtol(value.c_str(), NULL, 0);
}

f32 from_f32(const core::stringc &value)
{
	return (f32)strtod(value.c_str(), NULL);
}

f64 from_f64(const core::stringc &value)
{
	return strtod(value.c_str(), NULL);
}

}
}

