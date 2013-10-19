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

#endif /* INC_TAGGIT_H */
