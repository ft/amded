/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

#ifndef INC_TAGGIT_H
#define INC_TAGGIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>

#include <taglib/tag_c.h>

#define PROJECT "taggit"
#define VERSION "0.2+git"

#define TAGGIT_USAGE \
"usage: taggit OPTION(s) FILE(s)\n\n" \
"  informational options:\n" \
"    -h                display this help text\n" \
"    -L                show taggit's licence information\n" \
"    -v                print version information\n" \
"  action options:\n" \
"    -l                list tags in human readable form\n" \
"    -m                list tags in machine readable form\n" \
"    -t <tag>=<value>  set a tag to a value\n" \
"\n"

#define TAGGIT_LICENCE \
" Copyright 2009 taggit workers, All rights reserved.\n" \
"\n" \
" Redistribution and use in source and binary forms, with or without\n" \
" modification, are permitted provided that the following conditions\n" \
" are met:\n" \
"\n" \
" 1. Redistributions of source code must retain the above copyright\n" \
"    notice, this list of conditions and the following disclaimer.\n" \
" 2. Redistributions in binary form must reproduce the above copyright\n" \
"    notice, this list of conditions and the following disclaimer in the\n" \
"    documentation and/or other materials provided with the distribution.\n" \
"\n" \
" THIS SOFTWARE IS PROVIDED \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES,\n"\
" INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY\n" \
" AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL\n" \
" THE AUTHOR OR CONTRIBUTORS OF THE PROJECT BE LIABLE FOR ANY DIRECT,\n" \
" INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES\n" \
" (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR\n" \
" SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)\n" \
" HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,\n" \
" STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING\n" \
" IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE\n" \
" POSSIBILITY OF SUCH DAMAGE.\n"

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

enum tag_id {
    T_UNKNOWN = 0,
    T_ARTIST,
    T_ALBUM,
    T_GENRE,
    T_TRACKTITLE,
    T_TRACKNUMBER,
    T_YEAR
};

enum tag_type {
    TAG_INVALID = 0,
    TAG_STRING,
    TAG_INT,
    TAG_NUKE
};

enum file_type {
    FT_INVALID = 0,
    FT_MPEG,
    FT_OGGVORBIS,
    FT_OGGFLAC,
    FT_FLAC,
    FT_UNKNOWN = 255
};

struct taggit_file {
    TagLib_File *data;
    enum file_type type;
};

struct t_tag {
    char *name;
    char *value;
    enum tag_type type;
};

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

static inline int
streq(char *s, char *t)
{
    return !strcmp(s, t) ? 1 : 0;
}

static inline int
strcaseeq(char *s, char *t)
{
    return !strcasecmp(s, t) ? 1 : 0;
}

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

#define MALLOC_OR_DIE(number,type)  \
    (type *)xmalloc_or_die(number * sizeof(type))

void taglist_destroy(enum file_type, struct taggit_list *);
void taggit_list_human(const char *);
void taggit_list_machine(const char *);
void taggit_tag(const char *);

struct taggit_list *list(struct taggit_file *);
struct t_tag next_tag(const char *);
void add_tag(struct t_tag *);
void list_tags(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_TAGGIT_H */
