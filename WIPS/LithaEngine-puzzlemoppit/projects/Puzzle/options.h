
#ifndef OPTIONS_H
#define OPTIONS_H

#include "Litha.h"

// just some utility stuff

VariantMap get_current_config();
void apply_temp_config(VariantMap config);
void apply_permanent_config(VariantMap config);

#endif
