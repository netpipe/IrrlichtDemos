
//#include "utils/VariantMap.h"

// UGLY HACK. There are some kind of circular dependencies goin' on.
#include "litha_internal.h"
#include <map>

namespace utils
{

void override_variantmap(VariantMap &originalVM, const VariantMap &newVM)
{
	VariantMap oldVM = originalVM;
	
	// Set new. This completely replaces original.
	originalVM = newVM;
	
	// So we insert old back in.
	// Insert doesn't replace keys that exist, so won't overwrite the new values.
	originalVM.insert( oldVM.begin(), oldVM.end() );
}

}

