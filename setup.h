/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file setup.h
 * @brief API for taggit's configurable setup
 */

#ifndef INC_SETUP_H
#define INC_SETUP_H

#include <cstdint>
#include "value.h"

void add_tag(enum tag_id, Value);
void set_opt(uint32_t);
bool get_opt(uint32_t);

#endif /* INC_SETUP_H */
