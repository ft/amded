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
 * Please do not do that, just call make "appropriately" (brace yourself, the
 * following quoting is rather ugly):
 *
 * \code
 * % make "ADDTO_CFLAGS=-DVENDOR_VERSION=\"\\\"debian 0.2-3\\\"\""
 * \endcode
 *
 * That will result in a version message such as this:
 *
 * \code
 * taggit v0.2+git (debian 0.2-3)
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

#define TAGGIT_TAG_MAXLENGTH 13

enum tag_type {
    TAG_INVALID = -1,
    TAG_STRING,
    TAG_INTEGER
};

enum tag_id {
    T_UNKNOWN = -1,
    T_ARTIST,
    T_ALBUM,
    T_COMPILATION,
    T_GENRE,
    T_COMMENT,
    T_TRACKTITLE,
    T_TRACKNUMBER,
    T_YEAR
};

enum file_type {
    FILE_T_MP3
};

enum tag_impl {
    TAG_T_APETAG,
    TAG_T_ID3V1,
    TAG_T_ID3V2
};

struct taggit_broken_tag_def {};

#endif /* INC_TAGGIT_H */
