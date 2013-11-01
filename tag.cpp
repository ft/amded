/*
 * Copyright (c) 2013 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file tag.cpp
 * @brief Tag-writing routines
 */

#include <map>
#include <string>

#include "tag.h"
#include "taggit.h"

/**
 * A mapping of tag-names to internal representation and type.
 *
 * Internally, taggit does not process strings each time it has to check which
 * tag it is working on. Instead an internal identifier of type "enum tag_id"
 * is used.
 *
 * Since tags can contain different values, this also maps to an "enum
 * tag_type" value, that identifies the type of value a tag normally contains.
 */
std::map< std::string,
          std::pair< enum tag_id,
                     enum tag_type > >
tag_map = {
    { "artist",         { T_ARTIST,         TAG_STRING  } },
    { "album",          { T_ALBUM,          TAG_STRING  } },
    { "catalognumber",  { T_CATALOGNUMBER,  TAG_STRING  } },
    { "comment",        { T_COMMENT,        TAG_STRING  } },
    { "compilation",    { T_COMPILATION,    TAG_STRING  } },
    { "composer",       { T_COMPOSER,       TAG_STRING  } },
    { "conductor",      { T_CONDUCTOR,      TAG_STRING  } },
    { "genre",          { T_GENRE,          TAG_STRING  } },
    { "label",          { T_LABEL,          TAG_STRING  } },
    { "performer",      { T_PERFORMER,      TAG_STRING  } },
    { "tracknumber",    { T_TRACKNUMBER,    TAG_INTEGER } },
    { "tracktitle",     { T_TRACKTITLE,     TAG_STRING  } },
    { "year",           { T_YEAR,           TAG_INTEGER } }
};

/** Print a list of supported tags to ‘stdout‘. */
void
list_tags(void)
{
    for (auto &iter : tag_map)
        std::cout << iter.first << std::endl;
}

void
taggit_tag(struct taggit_file file)
{
}
