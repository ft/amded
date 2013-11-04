/*
 * Copyright (c) 2009,2010 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file  taggit.h
 * @brief main API for taggits source
 */

#ifndef INC_TAGGIT_H
#define INC_TAGGIT_H

#include <cstdint>

#include <tfile.h>

#include "file-type.h"
#include "tag-implementation.h"

/** the project's (and executable's) name */
#define PROJECT "taggit"

/**
 * Version information
 *
 * +git should be added in untagged commits, to show that a non-release
 * executable is used.
 *
 * If this project is to be distributed with an OS, those distributors may want
 * to add a information here, too (like "(debian 0.2-3)" for a debian package).
 *
 * To get that working, change the VENDOR_VERSION macro in "taggit-vendor.h"
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
 * taggit version 0.5 (debian 0.5-3)
 * \endcode
 */
#define VERSION "0.4+git"

/* Boolean option bits: */

/**
 * Print an empty value list when a file does not contain meta information.
 *
 * Normally, taggit would error our in such situations. But some frontends
 * may want to get an empty list to reduce the number of codepaths in their
 * code.
 */
#define TAGGIT_LIST_ALLOW_EMPTY_TAGS    (1 << 0)

#define TAGGIT_TAG_MAXLENGTH 14

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
    T_GENRE,
    T_LABEL,
    T_PERFORMER,
    T_TRACKTITLE,
    T_TRACKNUMBER,
    T_YEAR
};

struct taggit_file {
    char *name;
    Taggit::FileType type;
    Taggit::TagImplementation tagimpl;
    bool multi_tag;
    TagLib::File *fh;
};

struct taggit_broken_tag_def {};

#endif /* INC_TAGGIT_H */
