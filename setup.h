/*
 * Copyright (c) 2009-2015 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file setup.h
 * @brief API for amded's configurable setup
 */

#ifndef INC_SETUP_H
#define INC_SETUP_H

#include <cstdint>
#include <map>
#include <vector>

#include "value.h"

void add_tag(enum tag_id, Value);
void set_opt(uint32_t);
bool get_opt(uint32_t);
void unset_only_tag_delete(void);
bool only_tag_delete(void);

extern std::map< enum file_type, std::vector< enum tag_impl > > read_map;
extern std::map< enum file_type, std::vector< enum tag_impl > > write_map;
extern std::map< enum tag_id, Value > newtags;

#endif /* INC_SETUP_H */
