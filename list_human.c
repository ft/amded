/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

#include <stdio.h>
#include <stdlib.h>

#include <taglib/tag_c.h>

#include "taggit.h"

void
taggit_list_human(const char *file)
{
    TagLib_File *tl;
    struct taggit_list *lst;

    tl = taglib_file_new(file);
    if (tl == NULL) {
        fprintf(stderr, "Cannot handle file: \"%s\" - skipping.\n", file);
        return;
    }

    lst = list(tl);
    if (lst == NULL) {
        fprintf(stderr, "File does not contain valid data: \"%s\"\n", file);
        return;
    }

    printf("<%s>\n", file);
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

    taglib_tag_free_strings();
    taglib_file_free(tl);
    free(lst);
}
