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

extern uint32_t taggit_options;

/**
 * Check whether an option is set.
 *
 * This macro works on `#taggit_options'.
 *
 * @code
 *    if (IS_SET(FOO)) {
 *        // FOO is set
 *    } else if (IS_SET(FOO | BAR)) {
 *        // FOO *and* BAR are set
 *    } else if (IS_SET(FOO) || IS_SET(BAR)) {
 *        // FOO *or* bar are set
 *    }
 * @endcode
 *
 * @param  OPT  A bitmask (32-bit max) to compare against `#taggit_options'.
 *
 * @return (macro)
 * @sideeffects none
 */
#define IS_SET(OPT) ((taggit_options & (OPT)) == OPT)

/**
 * Set an option bit in `#taggit_options'.
 *
 * @param  OPT  A bitmask to set in `#taggit_options'.
 *
 * @return (macro)
 * @sideeffects none
 */
#define SET_OPT(OPT) (taggit_options |= OPT)

/**
 * Unset an option bit in `#taggit_options'.
 *
 * @param  OPT  A bitmask to unset in `#taggit_options'.
 *
 * @return (macro)
 * @sideeffects none
 */
#define UNSET_OPT(OPT) (taggit_options &= ~(OPT))

/**
 * Toggle an option bit in `#taggit_options'.
 *
 * @param  OPT  A bitmask to toggle in `#taggit_options'.
 *
 * @return (macro)
 * @sideeffects none
 */
#define TOGGLE_OPT(OPT) (taggit_options ^= (OPT))

/*
 * Bit value macros for `#taggit_options'.
 */

/**
 * Print an empty value list when a file does not contain meta information.
 *
 * Normally, taggit would error our in such situations. But some frontends
 * may want to get an empty list to reduce the number of codepaths in their
 * code.
 */
#define TAGGIT_LIST_ALLOW_EMPTY_TAGS    (1 << 0)

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

#endif /* INC_TAGGIT_H */
