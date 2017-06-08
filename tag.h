/*
 * Copyright (c) 2013-2017 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file tag.h
 * @brief API for tag-writing routines
 */

#ifndef INC_TAG_H
#define INC_TAG_H

#include <map>
#include <string>

#include <tpropertymap.h>

#include "amded.h"

void amded_tag(struct amded_file &);
void amded_amend_tags(TagLib::PropertyMap &);
void list_tags(void);

extern std::map< std::string, std::pair< enum tag_id, enum tag_type > > tag_map;

#endif /* INC_TAG_H */
