/*
 * Copyright (c) 2013 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in doc/LICENCE.
 */

/**
 * @file cmdline.h
 * @brief API for command line argument processing
 */

#ifndef INC_TAG_H
#define INC_TAG_H

#include <string>

#include "taggit.h"
#include "value.h"

std::pair<std::string, std::string> tag_arg_to_pair(std::string);
enum tag_id tag_to_id(std::string);
enum tag_type tag_to_type(std::string);
Value tag_value_from_value(enum tag_type, std::string);
void list_tags(void);

#endif /* INC_TAG_H */
