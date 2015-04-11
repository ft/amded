/*
 * Copyright (c) 2013-2015 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file list.h
 * @brief API for tag reader backend
 *
 * Frontends are in ‘list-human.h‘ and ‘list-machine.h‘.
 */

#ifndef INC_LIST_H
#define INC_LIST_H

#include <map>
#include <string>

#include <fileref.h>

#include "amded.h"
#include "value.h"

std::map< std::string, Value > amded_list_tags(const struct amded_file &);
std::map< std::string, Value > amded_list_audioprops(TagLib::AudioProperties*);
std::map< std::string, Value > amded_list_amded(const struct amded_file &);

#endif /* INC_LIST_H */
