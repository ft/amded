/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

#include <stdio.h>
#include <stdlib.h>

#include <taglib/tag_c.h>

#include "taggit.h"

struct {
    enum file_type type;
    char *name;
} file_type_map[] = {
    { FT_MPEG,      "mp3" },
    { FT_OGGFLAC,   "oggflac" },
    { FT_OGGVORBIS, "oggvorbis" },
    { FT_FLAC,      "flac" },
    { FT_UNKNOWN,   "unknown" },
    { FT_INVALID,   "invalid" },
    { FT_INVALID,   NULL }
};

static char *
get_filetype(enum file_type type)
{
    int i;

    for (i = 0; file_type_map[i].name != NULL; ++i)
        if (type == file_type_map[i].type)
            return file_type_map[i].name;

    return (char *)NULL;
}

struct taggit_list *
list(struct taggit_file *file)
{
    TagLib_Tag *tag;
    const TagLib_AudioProperties *properties;
    struct taggit_list *lst;

    lst = MALLOC_OR_DIE(1, struct taggit_list);
    tag = taglib_file_tag(file->data);
    if (tag == NULL)
        return NULL;
    properties = taglib_file_audioproperties(file->data);
    if (properties == NULL)
        return NULL;

    lst->filetype = get_filetype(file->type);
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
