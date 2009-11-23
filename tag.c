/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "taggit.h"
#include "taglib_ext.h"

extern struct taglist *tags_head;

struct {
    char *name;
    enum tag_id id;
    enum tag_type type;
} tag_type_map[] = {
    { "artist",         T_ARTIST,           TAG_STRING },
    { "album",          T_ALBUM,            TAG_STRING },
    { "tracktitle",     T_TRACKTITLE,       TAG_STRING },
    { "genre",          T_GENRE,            TAG_STRING },
    { "year",           T_YEAR,             TAG_INT },
    { "tracknumber",    T_TRACKNUMBER,      TAG_INT },
    { (char *)NULL,     T_UNKNOWN,          TAG_INVALID }
};

void
list_tags(void)
{
    int i;
    for (i = 0; tag_type_map[i].name != NULL; ++i)
        printf("%s\n", tag_type_map[i].name);
}

static enum tag_type
tag_to_type(char *tag)
{
    int i;

    for (i = 0; tag_type_map[i].name != NULL; ++i)
        if (streq(tag_type_map[i].name, tag))
            return tag_type_map[i].type;

    return TAG_INVALID;
}

static enum tag_id
tag_to_id(char *tag)
{
    int i;

    for (i = 0; tag_type_map[i].name != NULL; ++i)
        if (streq(tag_type_map[i].name, tag))
            return tag_type_map[i].id;

    return T_UNKNOWN;
}

int
str2uint(char *str, unsigned int *retval)
{
    unsigned long int rc;

    errno = 0;
    rc = strtoul((const char *)str, (char **) NULL, 10);
    if ((errno == ERANGE && (rc == ULONG_MAX)))
        return 0;

    /* okay, so far it's a valid long uint. */
    if (rc > UINT_MAX)
        return 0;

    *retval = (unsigned int)rc;
    return 1;
}

struct t_tag
next_tag(const char *arg)
{
    struct t_tag t;
    char *ptr;

    ptr = strchr(arg, (int)'=');
    if (ptr == NULL) {
        t.type = TAG_INVALID;
        t.name = t.value = (char *)NULL;
        return t;
    }

    t.name = (char *)arg;
    t.value = ptr + 1;
    *ptr = '\0';
    if (*(t.value) == '\0') {
        t.type = TAG_NUKE;
        return t;
    }
    t.type = tag_to_type(t.name);
    return t;
}

void
add_tag(struct t_tag *tag)
{
    struct taglist *ptr;
    unsigned int integer;
    enum tag_id id;

    if ((id = tag_to_id(tag->name)) == T_UNKNOWN) {
        fprintf(stderr, "Unknown tag: \"%s\"\n", tag->name);
        return;
    }
    if (tag->type == TAG_INT && str2uint(tag->value, &integer) == 0) {
        fprintf(stderr, "Invalid uint: \"%s\", not adding %s\n",
                tag->value, tag->name);
        return;
    }

    if (tags_head == NULL) {
        tags_head = MALLOC_OR_DIE(1, struct taglist);
        ptr = tags_head;
    } else {
        ptr = tags_head;
        while (ptr->next != NULL)
            ptr = ptr->next;
        ptr->next = MALLOC_OR_DIE(1, struct taglist);
        ptr = ptr->next;
    }

    ptr->id = id;
    ptr->name = xstrdup(tag->name);
    ptr->type = tag->type;
    if (ptr->type == TAG_INT)
        ptr->val.integer = integer;
    else
        ptr->val.string = xstrdup(tag->value);
    ptr->next = NULL;
}

void
taggit_tag(const char *file)
{
    TagLib_Tag *tag;
    struct taggit_file f;
    struct taglist *ptr;

    f = taggit_file_open(file);
    if (f.type == FT_INVALID)
        return;

    tag = taglib_file_tag(f.data);
    if (f.type == FT_MPEG)
        mp3_dotheape(f.data);

    ptr = tags_head;
    while (ptr != NULL) {
        switch (ptr->id) {
        case T_ARTIST:
            taglib_tag_set_artist(tag, ptr->val.string);
            break;
        case T_ALBUM:
            taglib_tag_set_album(tag, ptr->val.string);
            break;
        case T_TRACKNUMBER:
            taglib_tag_set_track(tag, ptr->val.integer);
            break;
        case T_TRACKTITLE:
            taglib_tag_set_title(tag, ptr->val.string);
            break;
        case T_GENRE:
            taglib_tag_set_genre(tag, ptr->val.string);
            break;
        case T_YEAR:
            taglib_tag_set_year(tag, ptr->val.integer);
            break;
        default:
            fprintf(stderr, "Whoops, unknown tag-id (name: \"%s\").\n",
                    ptr->name);
            break;
        }

        ptr = ptr->next;
    }

    taggit_file_save(&f);
    taglib_tag_free_strings();
    taggit_file_destroy(&f);
}
