/*
 * Copyright (c) 2013-2017 amded workers, All rights reserved.
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
enum tag_id tag_to_id(const std::string&);
enum tag_type tag_to_type(const std::string&);
Value tag_value_from_value(enum tag_type, const std::string&);
void list_tags(void);
void setup_readmap(const std::string&);
void setup_writemap(const std::string&);
void amded_parameters(const std::string&);

#endif /* INC_CMDLINE_H */
