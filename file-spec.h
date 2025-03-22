/*
 * Copyright (c) 2013-2025 amded workers, All rights reserved.
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

#include "amded.h"

enum file_type get_ext_type(const std::string&);
bool is_multitag_type(enum file_type);
bool amded_open(struct amded_file &);

std::string get_tag_types(const struct amded_file &);
TagLib::PropertyMap get_tags_for_file(const struct amded_file &);
bool tag_impl_allowed_for_file_type(enum file_type, enum tag_impl);
void tag_multitag(const struct amded_file &);
void strip_multitag(const struct amded_file &);
void list_extensions(void);

extern std::map< enum file_type, std::vector< enum tag_impl > > filetag_map;

#endif /* INC_FILE_SPEC_H */
