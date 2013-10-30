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

#include <string>

#include "taggit.h"

enum file_type get_file_type(std::string);
std::string get_file_type_reverse(enum file_type);

#endif /* INC_FILE_SPEC_H */
