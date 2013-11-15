/*
 * Copyright (c) 2013 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file tag.cpp
 * @brief Tag-writing routines
 */

#include <iostream>
#include <map>
#include <string>

#include <fileref.h>
#include <tpropertymap.h>

#include "file-spec.h"
#include "list-human.h"
#include "setup.h"
#include "tag.h"
#include "amded.h"

/**
 * A mapping of tag-names to internal representation and type.
 *
 * Internally, amded does not process strings each time it has to check which
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
    { "catalog-number", { T_CATALOGNUMBER,  TAG_STRING  } },
    { "comment",        { T_COMMENT,        TAG_STRING  } },
    { "compilation",    { T_COMPILATION,    TAG_STRING  } },
    { "composer",       { T_COMPOSER,       TAG_STRING  } },
    { "conductor",      { T_CONDUCTOR,      TAG_STRING  } },
    { "description",    { T_DESCRIPTION,    TAG_STRING  } },
    { "genre",          { T_GENRE,          TAG_STRING  } },
    { "label",          { T_LABEL,          TAG_STRING  } },
    { "performer",      { T_PERFORMER,      TAG_STRING  } },
    { "publisher",      { T_PUBLISHER,      TAG_STRING  } },
    { "track-number",   { T_TRACKNUMBER,    TAG_INTEGER } },
    { "track-title",    { T_TRACKTITLE,     TAG_STRING  } },
    { "year",           { T_YEAR,           TAG_INTEGER } }
};

static std::map< enum tag_id, std::string > taglib_amded_map = {
    { T_ARTIST,         "ARTIST" },
    { T_ALBUM,          "ALBUM" },
    { T_CATALOGNUMBER,  "CATALOGNUMBER" },
    { T_COMMENT,        "COMMENT" },
    { T_COMPILATION,    "ALBUMARTIST" },
    { T_COMPOSER,       "COMPOSER" },
    { T_CONDUCTOR,      "CONDUCTOR" },
    { T_GENRE,          "GENRE" },
    { T_LABEL,          "LABEL" },
    { T_PERFORMER,      "PERFORMER" },
    { T_TRACKNUMBER,    "TRACKNUMBER" },
    { T_TRACKTITLE,     "TITLE" },
    { T_YEAR,           "DATE" }
};

/** Print a list of supported tags to ‘stdout‘. */
void
list_tags(void)
{
    for (auto &iter : tag_map)
        std::cout << iter.first << std::endl;
}

void
amded_amend_tags(TagLib::PropertyMap &pm)
{
    for (auto &iter : newtags) {
        bool rc = true;

        if (iter.second.get_type() == TAG_INTEGER)
            rc = pm.replace(taglib_amded_map[iter.first],
                            { std::to_string(iter.second.get_int()) });
        else if (iter.second.get_type() == TAG_INVALID)
            pm.erase(taglib_amded_map[iter.first]);
        else
            rc = pm.replace(taglib_amded_map[iter.first],
                            { iter.second.get_str() });

        if (!rc)
            std::cerr << PROJECT << ": Failed to set tag `"
                      << taglib_amded_map[iter.first] << "'!" << std::endl;
    }
}

void
amded_tag(struct amded_file &file)
{
    if (file.fh->readOnly()) {
        std::cerr << PROJECT << ": File is read-only: "
                  << file.name << std::endl;
        return;
    }

    if (file.multi_tag) {
        tag_multitag(file);
        return;
    }

    /* Change current property map to what the user supplied in the cmdline. */
    TagLib::PropertyMap pm = file.fh->properties();
    amded_amend_tags(pm);

    /* Replace current property map with the newly adjusted one. */
    file.fh->setProperties(pm);

    /* Save values to file. */
    if (!(file.fh->save()))
        std::cerr << PROJECT << ": Failed to save file `"
                  << file.name
                  << "'"
                  << std::endl;
}
