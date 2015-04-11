/*
 * Copyright (c) 2013-2015 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file cmdline.h
 * @brief API for command line argument processing
 */

#ifndef INC_CMDLINE_H
#define INC_CMDLINE_H

#include <string>

#include "amded.h"
#include "value.h"

std::pair<std::string, std::string> tag_arg_to_pair(std::string);
enum tag_id tag_to_id(std::string);
enum tag_type tag_to_type(std::string);
Value tag_value_from_value(enum tag_type, std::string);
void list_tags(void);
void setup_readmap(std::string);
void setup_writemap(std::string);
void amded_parameters(std::string);

#endif /* INC_CMDLINE_H */
