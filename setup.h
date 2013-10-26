#ifndef INC_SETUP_H
#define INC_SETUP_H

#include <cstdint>
#include "value.h"

void add_tag(enum tag_id, Value);
void set_opt(uint32_t);
bool get_opt(uint32_t);

#endif /* INC_SETUP_H */
