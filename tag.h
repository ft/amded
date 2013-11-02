/*
 * Copyright (c) 2013 taggit workers, All rights reserved.
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

#include "taggit.h"

void taggit_tag(struct taggit_file);
void taggit_amend_tags(TagLib::PropertyMap &);

extern std::map< std::string, std::pair< enum tag_id, enum tag_type > > tag_map;

#endif /* INC_TAG_H */
