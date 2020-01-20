
#ifndef UTILS_VARIANT_MAP_H
#define UTILS_VARIANT_MAP_H

#include "litha_internal.h"
#include <map>

namespace utils
{
class Variant;

typedef std::map<core::stringc, Variant> VariantMap;

// Override original's key values with those in new.
// Unlike using the map assignment operator, non clashing keys in original are retained.
void override_variantmap(VariantMap &originalVM, const VariantMap &newVM);

}

#endif
