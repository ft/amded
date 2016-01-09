/*
 * Copyright (c) 2013-2016 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file setup.cpp
 * @brief Configurable setup
 *
 * Amded does not read a configuration file. Instead, its configurable
 * parameters are changed via command line options. This file implements
 * parameter storage and retrieval. It does not take care of parsing command
 * line arguments.
 *
 * The trail of action looks like this (for the ‘-t’ command line option):
 *
 * - The parse_options() function in amded.cpp calls bsd_getopt() from
 *   bsdgetopt.c; when bsd_getopt() processes the argument that contains th
 *   ‘-t’ option, parse_options() will see the option's "foo=bar" argument.
 *
 * - Then, parse_options() uses functionality from tag.cpp to split the
 *   argument into "foo" and "bar" (ie. tag-name and value). It also takes care
 *   of verifying that "foo" is a supported tag-name, and that "bar" is a
 *   reasonable value for "foo" (for example: the "year" tag is supported, and
 *   needs an integer value).
 *
 * - At this point, part of amded's setup can be changed. Thus, ‘add_tag()’
 *   from this file is called to put the gived value for the supported ‘tag_id’
 *   is stored in the ‘newtags’ map.
 *
 * Setup details:
 *
 *   Defining values for tags:
 *
 *     Amded is able to change tags in audio-files. The ‘-t’ option is the
 *     driver for that. The tag-name is turned into a numeric value (enum
 *     tag_id). The value is an instance of the ‘Value’ class (defined in
 *     value.h and * value.cpp). The values are put into a map in which the
 *     tag_id value is the key. Thus "-t foo=bar -t foo=baz" sets the value of
 *     the "foo" that to "baz".
 *
 *   Read-Map:
 *
 *     Some file types support multiple types of tags: For example mp3 files
 *     may contain ID3V1 (*yuck*), ID3V2 and apetag tags at the same time.
 *     Values for the same tags in different tag types may differ. Amded
 *     supports this while reading tags by providing a list of tag types that
 *     defines the read-priority of tags for a given file.
 *
 *     For example, if the read-map entry for mp3 files looks like this:
 *
 *       "apetag,id3v2"
 *
 *     ...and you try to look up the "artist" tag, which exists in both tags,
 *     the value from the ‘apetag’ tag-type. If the "album" tag on the other
 *     hand, only exists in the ‘id3v2’ tag-type, but not in the ‘apetag’
 *     tag-type, the value is read from ‘id3v2’. Values from the ‘id3v1’
 *     tag-type would be ignored entirely.
 *
 *     To support this, amded needs two maps: One map defines which tag-types
 *     are supported by a file-type. And the other one maps file types to a
 *     vector of tag-types.
 *
 *     The tag-reading procedure (as started by the ‘-m’ and ‘-l’ options) has
 *     to obey the vector in the second mapping.
 *
 *   Write-Map:
 *
 *     The write-map addresses the same problem as the read-map, but not while
 *     reading tags, but while writing them to the file. It defines lists of
 *     file-types to write to a file, that supports more than one tag-type.
 *
 *     To support this, amded needs the map that defines which tag-types are
 *     supported in a file-type (the same as with the read-map). In addition to
 *     that, a mapping of file type to a vector of tag-types is needed again.
 *
 *     The tag-writing procedure then writes the tags to _all_ tag-types listed
 *     in that vector.
 *
 *   Boolean flags:
 *
 *     Amded's behaviour can also be altered by a set of boolean flags (such
 *     as ‘-E’). The implementation works by setting and reading bits in a
 *     large integer word.
 */

#include <cstdint>
#include <map>
#include <vector>

#include "setup.h"
#include "amded.h"
#include "value.h"

/*
 * Tag value settings:
 */

std::map< enum tag_id, Value > newtags;

void
add_tag(enum tag_id id, Value v)
{
    newtags[id] = v;
}

/*
 * Read-map:
 */

std::map< enum file_type, std::vector< enum tag_impl > > read_map;

/*
 * Write-map:
 */

std::map< enum file_type, std::vector< enum tag_impl > > write_map;

/*
 * Boolean option implementation:
 */

/**
 * Check whether an option is set.
 *
 * @code
 *    if (IS_SET(FOO, datastore)) {
 *        // FOO is set
 *    } else if (IS_SET(FOO | BAR, datastore)) {
 *        // FOO *and* BAR are set
 *    } else if (IS_SET(FOO, datastore) || IS_SET(BAR, datastore)) {
 *        // FOO *or* bar are set
 *    }
 * @endcode
 *
 * @param  OPT    A bitmask to compare against STORE.
 * @param  STORE  The data store to match OPT against.
 *
 * @return (macro)
 */
#define IS_SET(OPT, STORE) ((STORE & (OPT)) == OPT)

/**
 * Set an option bit in `STORE'.
 *
 * @param  OPT    A bitmask to set in `STORE'.
 * @param  STORE  The data store to set OPT in.
 *
 * @return (macro)
 */
#define SET_OPT(OPT, STORE) (STORE |= OPT)

/**
 * Unset an option bit in `STORE'.
 *
 * @param  OPT    A bitmask to unset in STORE.
 * @param  STORE  The data store to unset OPT in.
 *
 * @return (macro)
 */
#define UNSET_OPT(OPT, STORE) (STORE &= ~(OPT))

/**
 * Toggle an option bit in `STORE'.
 *
 * @param  OPT    A bitmask to toggle in STORE.
 * @param  STORE  The data store to toggle OPT in.
 *
 * @return (macro)
 */
#define TOGGLE_OPT(OPT, STORE) (STORE ^= (OPT))

static uint32_t amded_options;

void
set_opt(uint32_t optmask)
{
    SET_OPT(optmask, amded_options);
}

bool
get_opt(uint32_t optmask)
{
    return (bool)IS_SET(optmask, amded_options);
}

/*
 * Tell -d and -t apart. We need to change the behaviour a little if only -d
 * options were given.
 */

static bool otd = true;

bool
only_tag_delete(void)
{
    return otd;
}

void
unset_only_tag_delete(void)
{
    otd = false;
}
