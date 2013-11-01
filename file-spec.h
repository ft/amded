/*
 * Copyright (c) 2013 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file file-spec.cpp
 * @brief API for File specific routines and abstraction
 */

#ifndef INC_FILE_SPEC_H
#define INC_FILE_SPEC_H

#include <map>
#include <string>

#include "taggit.h"

enum file_type get_ext_type(std::string);
enum file_type get_file_type(std::string);
std::string get_file_type_reverse(enum file_type);
std::vector< enum tag_impl > get_multitag_vector(enum file_type);
bool is_multitag_type(enum file_type);
bool taggit_open(struct taggit_file &);

std::string get_tag_types(const struct taggit_file &);
std::string tag_impl_to_string(enum tag_impl);
TagLib::PropertyMap get_tags_for_file(const struct taggit_file &);

extern std::map< enum file_type, std::vector< enum tag_impl > > filetag_map;

#endif /* INC_FILE_SPEC_H */
