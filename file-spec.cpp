/*
 * Copyright (c) 2013-2019 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file file-spec.cpp
 * @brief File specific routines and abstraction
 */

#include <algorithm>
#include <map>
#include <string>

#include <apetag.h>
#include <flacfile.h>
#include <id3v1tag.h>
#include <id3v2tag.h>
#include <mp4file.h>
#include <mpegfile.h>
#include <oggflacfile.h>
#include <opusfile.h>
#include <tfile.h>
#include <tpropertymap.h>
#include <vorbisfile.h>

#include "amded.h"
#include "file-spec.h"
#include "file-type.h"
#include "setup.h"
#include "tag-implementation.h"
#include "tag.h"

/**
 * Map of file types that support multiple tag-types.
 *
 * This is used by both the read-map and the write-map. It also defines default
 * values for those:
 *
 * - The read-map defaults to exactly this map.
 *
 * - The write-map defaults to the first entry in the tag-type vector.
 */
std::map< enum file_type, std::vector< enum tag_impl > > filetag_map {
    { FILE_T_MP3, { TAG_T_ID3V2, TAG_T_APETAG, TAG_T_ID3V1 } }
};

static std::vector< enum tag_impl >
get_vector_from_map(enum file_type type,
                    std::map< enum file_type, std::vector< enum tag_impl > > m)
{
    auto rv = m.find(type);
    if (rv == m.end()) {
        return {};
    }
    return rv->second;
}

static std::vector< enum tag_impl >
get_readmap_vector(enum file_type type)
{
    return get_vector_from_map(type, read_map);
}

static std::vector< enum tag_impl >
get_writemap_vector(enum file_type type)
{
    return get_vector_from_map(type, write_map);
}

static std::vector< enum tag_impl >
get_multitag_vector(enum file_type type)
{
    return get_vector_from_map(type, filetag_map);
}

bool
tag_impl_allowed_for_file_type(enum file_type ft, enum tag_impl ti)
{
    static std::vector<enum tag_impl> ttypes = get_multitag_vector(ft);
    for (auto &iter : ttypes) {
        if (iter == ti) {
            return true;
        }
    }
    return false;
}

static bool
mp3_has_tag_type(TagLib::MPEG::File *fh, enum tag_impl type)
{
    if (type == TAG_T_APETAG && fh->hasAPETag()) {
        return true;
    }
    if (type == TAG_T_ID3V2 && fh->hasID3v2Tag()) {
        return true;
    }
    if (type == TAG_T_ID3V1 && fh->hasID3v1Tag()) {
        return true;
    }
    return false;
}

static bool
has_tag_type(const struct amded_file &file, enum tag_impl type)
{
    switch (file.type.get_id()) {
    case FILE_T_MP3:
        return
            mp3_has_tag_type(
                reinterpret_cast<TagLib::MPEG::File *>(file.fh),
                type);
        break;
    default:
        return false;
    }
}

bool
is_multitag_type(enum file_type type)
{
    return (filetag_map.find(type) != filetag_map.end());
}

static std::map < std::string, enum file_type >
file_ext_map = {
    { "flac", FILE_T_FLAC },
    { "flc",  FILE_T_FLAC },
    { "mp3",  FILE_T_MP3 },
    { "mp4",  FILE_T_M4A },
    { "m4a",  FILE_T_M4A },
    { "ogg",  FILE_T_OGG_VORBIS },
    { "oga",  FILE_T_OGG_VORBIS },
    { "opus", FILE_T_OPUS }
};

void
list_extensions(void)
{
    for (auto &iter : file_ext_map) {
        std::cout << iter.first << std::endl;
    }
}

enum file_type
get_ext_type(const std::string &filename)
{
    int dotidx = filename.rfind('.');
    std::string ext = filename.substr(dotidx + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    auto rv = file_ext_map.find(ext);
    if (rv == file_ext_map.end()) {
        return FILE_T_INVALID;
    }
    return rv->second;
}


static enum tag_impl
get_prefered_tag_impl(const struct amded_file &file)
{
    auto types = get_readmap_vector(file.type.get_id());
    for (auto &iter : types) {
        if (has_tag_type(file, iter)) {
            return iter;
        }
    }
    return TAG_T_NONE;
}

bool
amded_open(struct amded_file &file)
{
    switch (file.type.get_id()) {
    case FILE_T_MP3:
        file.fh = new TagLib::MPEG::File(file.name);
        break;
    case FILE_T_FLAC:
        file.fh = new TagLib::FLAC::File(file.name);
        break;
    case FILE_T_OGG_VORBIS:
        file.fh = new TagLib::Ogg::Vorbis::File(file.name);
        break;
    case FILE_T_M4A:
        file.fh = new TagLib::MP4::File(file.name);
        break;
    case FILE_T_OPUS:
        file.fh = new TagLib::Ogg::Opus::File(file.name);
        break;
    default:
        std::cerr << "BUG: Missing implementation for file type: "
                  << file.type.get_id() << std::endl
                  << "     This should not happen. Please report!"
                  << std::endl;
        return false;
    }

    if (!file.fh->isValid()) {
        std::cerr << PROJECT ": Could not open file: `"
                  << file.name << "'" << std::endl;
        goto error;
    }
    if (file.fh->tag() == nullptr) {
        std::cerr << PROJECT ": No tags in file: `"
                  << file.name << "'" << std::endl;
        goto error;
    }
    if (file.fh->tag() == nullptr) {
        std::cerr << PROJECT ": Could not get audio properties for file: `"
                  << file.name << "'" << std::endl;
        goto error;
    }

    if (is_multitag_type(file.type.get_id())) {
        file.multi_tag = true;
        file.tagimpl = get_prefered_tag_impl(file);
    } else {
        file.multi_tag = false;
    }

    return true;
error:
    delete file.fh;
    return false;
}

std::string
get_tag_types(const struct amded_file &file)
{
    std::string rv {};
    bool first = true;
    auto types = get_multitag_vector(file.type.get_id());
    for (auto &iter : types) {
        if (has_tag_type(file, iter)) {
            Amded::TagImplementation s = iter;
            if (!first) {
                rv += ",";
            } else {
                first = false;
            }
            rv += s.get_label();
        }
    }
    return first ? "none" : rv;
}

TagLib::PropertyMap
get_tags_for_file(const struct amded_file &file)
{
    TagLib::MPEG::File *mp3fh;

    switch (file.type.get_id()) {
    case FILE_T_MP3:
        mp3fh = reinterpret_cast<TagLib::MPEG::File *>(file.fh);
        switch (file.tagimpl.get_id()) {
        case TAG_T_ID3V2:
            return mp3fh->ID3v2Tag()->properties();
        case TAG_T_APETAG:
            return mp3fh->APETag()->properties();
        case TAG_T_ID3V1:
            return mp3fh->ID3v1Tag()->properties();
        default:
            break;
        }
        /* FALL-THROUGH */
    default:
        return file.fh->properties();
    }
}

static bool
amded_tag_mp3(TagLib::MPEG::File *fh,
               const std::vector<enum tag_impl> &wm)
{
    bool want_ape, want_v1, want_v2;

    want_ape = want_v1 = want_v2 = false;
    for (auto &iter : wm) {
        switch (iter) {
        case TAG_T_APETAG:
            if (fh->hasAPETag() || !only_tag_delete()) {
                want_ape = true;
            }
            break;
        case TAG_T_ID3V1:
            if (fh->hasID3v1Tag() || !only_tag_delete()) {
                want_v1 = true;
            }
            break;
        case TAG_T_ID3V2:
            if (fh->hasID3v2Tag() || !only_tag_delete()) {
                want_v2 = true;
            }
            break;
        default:
            break;
        }
    }

    if (!(want_ape || want_v1 || want_v2)) {
        return true;
    }

    int save_tags = TagLib::MPEG::File::NoTags;
    if (want_ape) {
        save_tags |= TagLib::MPEG::File::APE;
        TagLib::APE::Tag *tag = fh->APETag(true);
        TagLib::PropertyMap pm = tag->properties();
        amded_amend_tags(pm);
        tag->setProperties(pm);
    }
    if (want_v2) {
        save_tags |= TagLib::MPEG::File::ID3v2;
        TagLib::ID3v2::Tag *tag = fh->ID3v2Tag(true);
        TagLib::PropertyMap pm = tag->properties();
        amded_amend_tags(pm);
        tag->setProperties(pm);
    }
    if (want_v1) {
        save_tags |= TagLib::MPEG::File::ID3v1;
        TagLib::ID3v1::Tag *tag = fh->ID3v1Tag(true);
        TagLib::PropertyMap pm = tag->properties();
        amded_amend_tags(pm);
        tag->setProperties(pm);
    }

    return fh->save(save_tags, false, 4, false);
}

static bool
amded_strip_mp3(TagLib::MPEG::File *fh,
                 const std::vector<enum tag_impl> &wm)
{
    int save_tags = TagLib::MPEG::File::NoTags;
    for (auto &iter : wm) {
        switch (iter) {
        case TAG_T_APETAG:
            save_tags |= TagLib::MPEG::File::APE;
            break;
        case TAG_T_ID3V1:
            save_tags |= TagLib::MPEG::File::ID3v1;
            break;
        case TAG_T_ID3V2:
            save_tags |= TagLib::MPEG::File::ID3v2;
            break;
        default:
            break;
        }
    }
    if (save_tags == TagLib::MPEG::File::NoTags) {
        return true;
    }
    return fh->strip(save_tags);
}

void
tag_multitag(const struct amded_file &file)
{
    bool rc;
    switch (file.type.get_id()) {
    case FILE_T_MP3:
        rc = amded_tag_mp3(reinterpret_cast<TagLib::MPEG::File *>(file.fh),
                            get_writemap_vector(file.type.get_id()));
        break;
    default:
        return;
    }
    if (!rc) {
        std::cerr << PROJECT << ": Failed to save file `" << file.name << "'"
                  << std::endl;
    }
}

void
strip_multitag(const struct amded_file &file)
{
    bool rc;
    switch (file.type.get_id()) {
    case FILE_T_MP3:
        rc = amded_strip_mp3(reinterpret_cast<TagLib::MPEG::File *>(file.fh),
                              get_writemap_vector(file.type.get_id()));
        break;
    default:
        return;
    }
    if (!rc) {
        std::cerr << PROJECT << ": Failed to save file `" << file.name << "'"
                  << std::endl;
    }
}
