/*
 * Copyright (c) 2013 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file list.cpp
 * @brief Tag reader backend
 *
 * Frontends are in ‘list-human.cpp‘ and ‘list-machine.cpp‘.
 *
 * There are three categories of data, that the listing backend needs to return
 * seperately. First there is amded-specific information about the currently
 * processed file: For example which file-type are we dealing with? And which
 * tag-types are we looking at?
 *
 * Then there is the actual meta data: Album, artist, title and all that.
 *
 * Finally, there is information about the encoded audio data: Bit-rate,
 * sampling rate, number of channels, duration etc.
 *
 * All three sources need to check whether or not the user wants to see
 * non-existent and empty tags listed.
 */

#include <map>
#include <string>

#include <fileref.h>
#include <tpropertymap.h>

#include "file-spec.h"
#include "list.h"
#include "setup.h"
#include "tag.h"
#include "amded.h"
#include "value.h"

static void
tagtomap(std::map< std::string, Value > &m,
         const TagLib::PropertyMap &tags,
         const char *tagname,
         const char *propname,
         const bool wantempty,
         const bool isint)
{
    bool didnothing = false;
    if (tags.contains(propname)) {
        const TagLib::StringList values = tags.find(propname)->second;
        size_t size = values.size();
        if (size > 0) {
            if (isint)
                m[tagname] = values[0].toInt();
            else if (values[0].length() > 0)
                m[tagname] = values[0];
            else
                didnothing = true;
        } else
            didnothing = true;
    } else
        didnothing = true;

    if (didnothing && wantempty) {
        if (isint)
            m[tagname] = (int)0;
        else
            m[tagname] = (std::string)"";
    }
}

std::map< std::string, Value >
amded_list_tags(const struct amded_file &file)
{
    const TagLib::PropertyMap &tags = get_tags_for_file(file);
    std::map< std::string, Value > retval;
    bool wantempty = get_opt(AMDED_LIST_ALLOW_EMPTY_TAGS);

    if (file.tagimpl.get_id() == TAG_T_NONE) {
        if (wantempty) {
            for (auto &iter : tag_map) {
                if (iter.second.second == TAG_STRING) {
                    retval[iter.first] = (std::string)"";
                } else {
                    retval[iter.first] = 0;
                }
            }
            retval["is-va"] = false;
            return retval;
        } else
            return { };
    }

    tagtomap(retval, tags, "album", "ALBUM", wantempty, false);
    tagtomap(retval, tags, "artist", "ARTIST", wantempty, false);
    tagtomap(retval, tags, "catalog-number", "CATALOGNUMBER", wantempty, false);
    tagtomap(retval, tags, "comment", "COMMENT", wantempty, false);
    tagtomap(retval, tags, "compilation", "ALBUMARTIST", wantempty, false);
    if (tags.contains("ALBUMARTIST"))
        retval["is-va"] = true;
    else
        retval["is-va"] = false;
    tagtomap(retval, tags, "composer", "COMPOSER", wantempty, false);
    tagtomap(retval, tags, "conductor", "CONDUCTOR", wantempty, false);
    tagtomap(retval, tags, "description", "DESCRIPTION", wantempty, false);
    tagtomap(retval, tags, "genre", "GENRE", wantempty, false);
    tagtomap(retval, tags, "label", "LABEL", wantempty, false);
    tagtomap(retval, tags, "performer", "PERFORMER", wantempty, false);
    tagtomap(retval, tags, "publisher", "PUBLISHER", wantempty, false);
    tagtomap(retval, tags, "track-number", "TRACKNUMBER", wantempty, true);
    tagtomap(retval, tags, "track-title", "TITLE", wantempty, false);
    tagtomap(retval, tags, "url", "URL", wantempty, false);
    tagtomap(retval, tags, "year", "DATE", wantempty, true);
    tagtomap(retval, tags, "mb-album-id", "MUSICBRAINZ_ALBUMID", wantempty, false);
    tagtomap(retval, tags, "mb-artist-id", "MUSICBRAINZ_ARTISTID", wantempty, false);
    tagtomap(retval, tags, "mb-track-id", "MUSICBRAINZ_TRACKID", wantempty, false);
    return retval;
}

std::map< std::string, Value >
amded_list_audioprops(TagLib::AudioProperties *p)
{
    std::map< std::string, Value > retval;
    /* bitrate() actually returns kilo-bitrate */
    retval["bit-rate"] = p->bitrate() * 1000;
    retval["channels"] = p->channels();
    retval["length"] = p->length();
    retval["sample-rate"] = p->sampleRate();
    return retval;
}

std::map< std::string, Value >
amded_list_amded(const struct amded_file &file)
{
    bool wantempty = get_opt(AMDED_LIST_ALLOW_EMPTY_TAGS);
    std::map< std::string, Value > retval;
    retval["file-type"] = file.type.get_label();
    if (file.multi_tag) {
        retval["tag-type"] = file.tagimpl.get_label();
        retval["tag-types"] = get_tag_types(file);
    } else if (wantempty) {
        retval["tag-type"] = (TagLib::String)"";
        retval["tag-types"] = (TagLib::String)"";
    }
    return retval;
}
