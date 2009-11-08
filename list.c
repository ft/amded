/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

#include <stdio.h>
#include <stdlib.h>

#include <taglib/tag_c.h>

#include "taggit.h"

struct taggit_list *
list(TagLib_File *file)
{
    TagLib_Tag *tag;
    const TagLib_AudioProperties *properties;
    struct taggit_list *lst;

    lst = malloc(sizeof(struct taggit_list));
    if (lst == NULL) {
        fprintf(stderr, "malloc() failed. Out of memory?\n");
        fprintf(stderr, "Cannot continue.\n");
        exit(EXIT_FAILURE);
    }

    tag = taglib_file_tag(file);
    if (tag == NULL)
        return NULL;
    properties = taglib_file_audioproperties(file);
    if (properties == NULL)
        return NULL;

    lst->artist = taglib_tag_artist(tag);
    lst->album = taglib_tag_album(tag);
    lst->tracktitle = taglib_tag_title(tag);
    lst->tracknumber = taglib_tag_track(tag);
    lst->year = taglib_tag_year(tag);
    lst->genre = taglib_tag_genre(tag);

    lst->samplerate = taglib_audioproperties_samplerate(properties);
    lst->kbitrate = taglib_audioproperties_bitrate(properties);
    lst->length = taglib_audioproperties_length(properties);
    lst->channels = taglib_audioproperties_channels(properties);

    lst->bitrate = lst->kbitrate * 1000;
    lst->ksamplerate = (double)lst->samplerate / (double)1000;
    lst->seconds = lst->length % 60;
    lst->minutes = (lst->length - lst->seconds) / 60;

    return lst;
}
