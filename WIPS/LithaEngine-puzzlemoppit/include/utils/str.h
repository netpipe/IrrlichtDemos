
#ifndef UTILS_STR_H
#define UTILS_STR_H

#include "litha_internal.h"
#include <vector>

namespace utils
{
namespace str
{

// trim the specified chars from the end of a string, returning the result
core::stringc rtrim(const core::stringc &str, const core::stringc &chars = " \t\r\n");

// trim from start of string
core::stringc ltrim(const core::stringc &str, const core::stringc &chars = " \t\r\n");

// trim from both start and end of string
core::stringc trim(const core::stringc &str, const core::stringc &chars = " \t\r\n");

// Returns a vector of strings which are the original string split on the boundaries formed
// by the delimiter string.
// If the delimiter is empty, "", or is not found in the string, will return a vector containing
// one element, the unmodified original string.
// If the string is empty, will return an empty vector.
std::vector<core::stringc> explode(const core::stringc &delimiter, const core::stringc &str);

// Like normal explode, except instead of matching an exact string as the delimiter, will match any sequence
// that consists of the given delimiter chars in any order.
std::vector<core::stringc> explode_chars(const core::stringc &delimiterChars, const core::stringc &str);

// Split a string at an assignment (equals) operator.
// e.g. something = another thing
// Will give "something" and "another thing". Trims whitespace from results.
// Always returns a vector with two elements, which can be empty strings.
// If there is no equals character present, returns empty strings.
std::vector<core::stringc> explode_at_assignment(const core::stringc &str);

// Does the string contain a particular character?
bool contains_char(const core::stringc &str, char c);

// boolean returns string "0" or "1"
core::stringc to(bool value);
core::stringc to(u32 value);
core::stringc to(s32 value);
core::stringc to(f32 value);
core::stringc to(f64 value);

// 0 is always returned if the conversion fails.
// Only numerical characters are considered, with the exception being boolean values which can be
// present as the string "true" or "false" (case insensitive)
// The initial part of the string determines the value.
// e.g. "10 cats and 11 dogs" will give the integer 10.
// "true false" will evaluate to TRUE.
bool from_bool(const core::stringc &value);
u32 from_u32(const core::stringc &value);
s32 from_s32(const core::stringc &value);
f32 from_f32(const core::stringc &value);
f64 from_f64(const core::stringc &value);

}
}

#endif
