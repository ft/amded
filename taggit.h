/*
 * Copyright (c) 2009,2010 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file  taggit.h
 * @brief main API for taggits source
 *
 * The static functions, defined within this file are prefixed with the
 * ATunusedAT lint annotation. This is purely to shut up lint in cases a
 * function is not used within a source file that includes this file
 * (which is quite common).
 *
 * To enable debugging output to stderr, call make like this:
 *
 * \code
 * % make ADDTO_CFLAGS=-DTAGGIT_DEBUG
 * \endcode
 *
 * To add more than one flag to that variable, do:
 *
 * \code
 * % make "ADDTO_CFLAGS=-DTAGGIT_DEBUG -DWHATEVER"
 * \endcode
 */

#ifndef INC_TAGGIT_H
#define INC_TAGGIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>

typedef int boolean;
#define true ((boolean)1)
#define false ((boolean)0)

#if defined S_SPLINT_S

/*
 * Okay, so if we're running "make lint", splint doesn't seem to realize
 * that strings.h defines strcasecmp(). That in turn will make it spit out
 * warnings each time this file is included.
 *
 * So, for splint - and *only* splint - make strcasecmp() synonymous to
 * strcmp(). In real code, that would be a rather bad idea, so force throwing
 * an error, too. (We're using -preproc for running splint, so #error won't
 * be fatal for the actual splint run.)
 */
#ifdef strcasecmp
#undef strcasecmp
#endif /* strcasecmp */
#define strcasecmp(x,y) strcmp(x,y)

#error "S_SPLINT_S macro defined, but not on a splint run!"
#error "In real code, this point may never ever be reached!"

#endif /* S_SPLINT_S */

#include <tag_c.h>

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
#define VERSION "0.3+git"

/**
 * Define a maximum set of tags in read and write maps
 *
 * We are currently only doing such quirks for mp3, where we
 * need to configure how to read and write different types of
 * tags (id3v1, v2 and ape). So for now three is enough.
 *
 * This could be done dynamically, but I don't think it's worth it.
 */
#define TAGGIT_MAP_MAX 3

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

/** information returned by the list() function */
struct taggit_list {
    char *filetype;
    char *tagtypes;
    int tagtype;
    char *artist;
    char *album;
    int tracknumber;
    char *tracktitle;
    int year;
    char *genre;
    int bitrate;
    int kbitrate;
    int samplerate;
    double ksamplerate;
    int channels;
    int length;
    int seconds;
    int minutes;
    int is_va;
    const char *va;
};

/** identification numbers for tags supported by taggit */
enum tag_id {
    T_UNKNOWN = 0,
    T_ARTIST,
    T_ALBUM,
    T_COMPILATION,
    T_GENRE,
    T_TRACKTITLE,
    T_TRACKNUMBER,
    T_YEAR
};

/** identification numbers for types of tag values */
enum tag_type {
    TAG_INVALID = 0,
    TAG_STRING,
    TAG_INT,
    TAG_NUKE
};

/** identification numbers for file types */
enum file_type {
    FT_INVALID = 0,
    FT_MPEG,
    FT_OGGVORBIS,
    FT_OGGFLAC,
    FT_FLAC,
    FT_UNKNOWN = 255
};

/**
 * TabLib makes it rather hard for C to check of which file type
 * the opened file is.
 *
 * This structure adds a type identifier to simplify that.
 */
struct taggit_file {
    TagLib_File *data;
    enum file_type type;
};

/**
 * structure keeping tagnames, tagvalues and the tagname's type.
 *
 * When reading command line options, -t accepts name=value pairs.
 * This is a structure that makes accessing names and values simple.
 * Also, this sticks a type identifier that describes what the tag's
 * value's type should be.
 */
struct t_tag {
    char *name;
    char *value;
    enum tag_type type;
};

/**
 * Linking tag names to values, including tag_ids and tag_types
 *
 * This is the second stage of interpreting options into data structures.
 * add_tag() uses this to create its linked list of tags to write into
 * files.
 */
struct taglist {
    char *name;
    enum tag_id id;
    enum tag_type type;
    union {
        char *string;
        int integer;
    } val;

    struct taglist *next;
};

/** Checking two strings for equality */
/*@unused@*/
static inline boolean
streq(char *s, char *t)
{
    return strcmp(s, t) == 0 ? true : false;
}

/** Case insensitively checking two strings for equality */
/*@unused@*/
static inline boolean
strcaseeq(char *s, char *t)
{
    return strcasecmp(s, t) == 0 ? true : false;
}

/** malloc() wrapper that checks for errors */
/*@unused@*/
/*@out@*/static inline void *
xmalloc_or_die(size_t size)
{
    void *buf;

    if ((buf = malloc(size)) == NULL) {
        fprintf(stderr, "malloc() failed. Abort.");
        exit(EXIT_FAILURE);
    }
    return buf;
}

/** malloc()ing multiple spaces */
#define MALLOC_OR_DIE(number,type)  \
    (type *)xmalloc_or_die(number * sizeof(type))

/** duplicate a string */
/*@unused@*/
/*@out@*/static inline char *
xstrdup(const char *s)
{
    char *ptr;
    size_t len;

    len = strlen(s) + 1;
    ptr = MALLOC_OR_DIE(len + 1, char);

    memcpy((void*)ptr, (const void*)s, len);
    ptr[len] = '\0';
    return ptr;
}

void taggit_version(void);
void taggit_usage(void);
void taggit_licence(void);

const char *mp3_id_to_string(int);
int setup_get_write_mask(enum file_type);
int setup_please_read(char *, int);
void setup_create_write_masks(void);
void setup_readmap(const char *);
void setup_writemap(const char *);
void taglist_destroy(enum file_type, struct taggit_list *);
void taggit_list_human(const char *);
void taggit_list_machine(const char *);
void taggit_tag(const char *);

struct taggit_list *list(struct taggit_file *);
struct t_tag next_tag(const char *);
void add_tag(struct t_tag *);
void list_tags(void);
enum file_type get_file_type_id(const char *);

int str2uint(char *, unsigned int *);


#ifdef TAGGIT_DEBUG
void dump_readmap(void);
void dump_writemap(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* INC_TAGGIT_H */
