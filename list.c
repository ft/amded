/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file  list.c
 * @brief read tag data and prepare it for output
 *
 * This is the backend list_human.c and list_machine.c are using.
 */

#include <stdio.h>
#include <stdlib.h>

#include <taglib/tag_c.h>

#include "taggit.h"
#include "taglib_ext.h"

/** mapping file type numbers to human readable strings */
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

/**
 * Translate an enum file_type into a human readable string
 *
 * Uses file_type_map.
 *
 * @param type      the file_type enum value to translate
 *
 * @return      a pointer to the string in the file_type_map. *Not* a
 *              newly allocated string. NULL is no match was found.
 * @sideeffects none
 */
static char *
get_filetype(enum file_type type)
{
    int i;

    for (i = 0; file_type_map[i].name != NULL; ++i)
        if (type == file_type_map[i].type)
            return file_type_map[i].name;

    return (char *)NULL;
}

/**
 * Handle destruction of file type specific information in list modes
 *
 * @param   type    the type of the currently processed file
 * @param   list    pointer the the taggit_list structure
 *
 * @return      void
 * @sideeffects none
 */
void
taglist_destroy(enum file_type type, struct taggit_list *list)
{
    if (type == FT_MPEG)
        free(list->tagtype);
}

/**
 * Take an file structure; read and prepare its data for later use
 *
 * Most of the data allocated is owned by the taglib code and will be freed
 * when taglib_tag_free_strings() is called. Some of the data in the returned
 * structure is file depended and the aforementioned function will not catch
 * it. That data can be destroyed by calling taglist_destroy(), which should
 * be called before taglib_tag_free_strings().
 *
 * @param   file    the already opened file structure to work on
 *
 * @return      the prepared data; NULL if something goes wrong
 * @sideeffects none
 */
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

    if (file->type == FT_MPEG)
        lst->tagtype = mp3_tagtypes(file->data);
    else
        lst->tagtype = NULL;

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
