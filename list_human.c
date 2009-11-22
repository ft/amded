/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

#include <stdio.h>
#include <stdlib.h>

#include <taglib/tag_c.h>

#include "taggit.h"
#include "taglib_ext.h"

void
taggit_list_human(const char *file)
{
    struct taggit_file f;
    struct taggit_list *lst;

    f = taggit_file_open(file);
    if (f.type == FT_INVALID)
        return;

    lst = list(&f);
    if (lst == NULL) {
        fprintf(stderr, "File does not contain valid data: \"%s\"\n", file);
        return;
    }

    printf("<%s>\n", file);
    printf("filetype    | %s\n", lst->filetype);
    if (f.type == FT_MPEG)
        printf("tagtypes    | %s\n", lst->tagtype);
    printf("artist      | \"%s\"\n", lst->artist);
    printf("album       | \"%s\"\n", lst->album);
    printf("tracknumber | \"%d\"\n", lst->tracknumber);
    printf("tracktitle  | \"%s\"\n", lst->tracktitle);
    printf("year        | \"%d\"\n", lst->year);
    printf("genre       | \"%s\"\n", lst->genre);
    printf("bitrate     | %d\n", lst->bitrate);
    printf("kbitrate    | %d\n", lst->kbitrate);
    printf("samplerate  | %d\n", lst->samplerate);
    printf("ksamplerate | %g\n", lst->ksamplerate);
    printf("channels    | %d\n", lst->channels);
    printf("length      | %d\n", lst->length);
    printf("mm:ss       | %d:%02d\n", lst->minutes, lst->seconds);

    taglist_destroy(f.type, lst);
    taglib_tag_free_strings();
    taggit_file_destroy(&f);
    free(lst);
}
