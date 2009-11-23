/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file  taggit.h
 * @brief main API for taggits source
 */

#ifndef INC_TAGGIT_H
#define INC_TAGGIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>

#include <taglib/tag_c.h>

/** the project's (and executable's) name */
#define PROJECT "taggit"
/**
 * Version information
 *
 * +git should be added in untagged commits, to show that a non-release
 * executable is used. If this project is to be distributed with an OS,
 * those distributors may want to add a string here, too (like
 * "0.2 (debian 0.2-3)" for a debian package).
 */
#define VERSION "0.2+git"

/** information returned by the list() function */
struct taggit_list {
    char *filetype;
    char *tagtype;
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
};

/** identification numbers for tags supported by taggit */
enum tag_id {
    T_UNKNOWN = 0,
    T_ARTIST,
    T_ALBUM,
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
    };

    struct taglist *next;
};

/** Checking two strings for equality */
static inline int
streq(char *s, char *t)
{
    return !strcmp(s, t) ? 1 : 0;
}

/** Case insensitively checking two strings for equality */
static inline int
strcaseeq(char *s, char *t)
{
    return !strcasecmp(s, t) ? 1 : 0;
}

/** malloc() wrapper that checks for errors */
static inline void *
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

void taggit_version(void);
void taggit_usage(void);
void taggit_licence(void);

void taglist_destroy(enum file_type, struct taggit_list *);
void taggit_list_human(const char *);
void taggit_list_machine(const char *);
void taggit_tag(const char *);

struct taggit_list *list(struct taggit_file *);
struct t_tag next_tag(const char *);
void add_tag(struct t_tag *);
void list_tags(void);

int str2uint(char *, unsigned int *);

#ifdef __cplusplus
}
#endif

#endif /* INC_TAGGIT_H */
