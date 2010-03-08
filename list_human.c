/*
 * Copyright (c) 2009,2010 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file  list_human.c
 * @brief print meta information from audio files in a human readable form
 */

#include <stdio.h>
#include <stdlib.h>

#include <tag_c.h>

#include "taggit.h"
#include "taglib_ext.h"

/**
 * Print meta information of a file in a nicely formated table
 *
 * @param   file    file name of the file to process
 *
 * @return      void
 * @sideeffects none
 */
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
    if (f.type == FT_MPEG) {
        printf("tagtypes    | %s\n", lst->tagtypes);
        printf("tagtype     | %s\n", mp3_id_to_string(lst->tagtype));
    }
    printf("artist      | \"%s\"\n", lst->artist);
    printf("album       | \"%s\"\n", lst->album);
    printf("tracknumber | \"%d\"\n", lst->tracknumber);
    printf("tracktitle  | \"%s\"\n", lst->tracktitle);
    printf("year        | \"%d\"\n", lst->year);
    printf("genre       | \"%s\"\n", lst->genre);
    if (lst->is_va) {
        printf("is_va       | true\n");
        printf("compilation | \"%s\"\n", lst->va);
    } else
        printf("is_va       | false\n");
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
