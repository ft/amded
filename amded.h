/*
 * Copyright (c) 2009-2017 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file  amded.h
 * @brief main API for amdeds source
 */

#ifndef INC_AMDED_H
#define INC_AMDED_H

#include <cstdint>

#include <tfile.h>

#include "file-type.h"
#include "tag-implementation.h"

/** the project's (and executable's) name */
#define PROJECT "amded"

/**
 * Version information
 *
 * +git should be added in untagged commits, to show that a non-release
 * executable is used.
 *
 * If this project is to be distributed with an OS, those distributors may want
 * to add a information here, too (like "(debian 0.2-3)" for a debian package).
 *
 * To get that working, change the VENDOR_VERSION macro in "amded-vendor.h"
 * and run the build like this:
 *
 * \code
 * % make ADDTOCXXFLAGS=-DVENDOR_BUILD
 * \endcode
 *
 * If VENDOR_VERSION is set to "debian 0.5-3", that will result in a version
 * message such as this:
 *
 * \code
 * amded version 0.5 (debian 0.5-3)
 * \endcode
 */
#define VERSION "0.8+git"

/* Boolean option bits: */

/**
 * Print an empty value list when a file does not contain meta information.
 *
 * Normally, amded would error our in such situations. But some frontends
 * may want to get an empty list to reduce the number of codepaths in their
 * code.
 */
#define AMDED_LIST_ALLOW_EMPTY_TAGS    (1 << 0)
#define AMDED_KEEP_UNSUPPORTED_TAGS    (1 << 1)
#define AMDED_JSON_DONT_USE_BASE64     (1 << 2)

#define AMDED_TAG_MAXLENGTH 14

enum tag_type {
    TAG_INVALID = -1,
    TAG_STRING,
    TAG_INTEGER,
    TAG_BOOLEAN
};

enum tag_id {
    T_UNKNOWN = -1,
    T_ARTIST,
    T_ALBUM,
    T_CATALOGNUMBER,
    T_COMMENT,
    T_COMPILATION,
    T_COMPOSER,
    T_CONDUCTOR,
    T_DESCRIPTION,
    T_GENRE,
    T_LABEL,
    T_PERFORMER,
    T_PUBLISHER,
    T_TRACKTITLE,
    T_TRACKNUMBER,
    T_URL,
    T_YEAR,
    T_MB_ALBUM_ID,
    T_MB_ARTIST_ID,
    T_MB_TRACK_ID
};

struct amded_file {
    char *name;
    Amded::FileType type;
    Amded::TagImplementation tagimpl;
    bool multi_tag;
    TagLib::File *fh;
};

struct amded_broken_tag_def {};

#endif /* INC_AMDED_H */
