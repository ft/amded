/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

#include <stdio.h>
#include <stdlib.h>

#include <taglib/tag_c.h>

#include "taggit.h"
#include "taglib_ext.h"

#define ASCII_STX (char)0x02
#define ASCII_ETX (char)0x03

static inline void
key_val_str(const char *key, char *val)
{
    printf("%s%c%s", key, ASCII_STX, val);
}

static inline void
key_val_int(const char *key, int val)
{
    printf("%s%c%d", key, ASCII_STX, val);
}

static inline void
key_val_dbl(const char *key, double val)
{
    printf("%s%c%g", key, ASCII_STX, val);
}

static inline void
etx(void)
{
    printf("%c", ASCII_ETX);
}

void
taggit_list_machine(const char *file)
{
    struct taggit_file f;
    struct taggit_list *lst;

    f = taggit_file_open(file);
    if (f.data == NULL) {
        fprintf(stderr, "Cannot handle file: \"%s\" - skipping.\n", file);
        return;
    }

    lst = list(&f);
    if (lst == NULL) {
        fprintf(stderr, "File does not contain valid data: \"%s\"\n", file);
        return;
    }

    /*
     * output format:
     *  <key><STX><value><ETX><key><STX><value><ETX><key>...<value>
     */
    key_val_str("filename", (char*)file);
    etx();
    key_val_str("filetype", lst->filetype);
    etx();
    key_val_str("artist", lst->artist);
    etx();
    key_val_str("album", lst->album);
    etx();
    key_val_int("tracknumber", lst->tracknumber);
    etx();
    key_val_str("tracktitle", lst->tracktitle);
    etx();
    key_val_int("year", lst->year);
    etx();
    key_val_str("genre", lst->genre);
    etx();
    key_val_int("bitrate", lst->bitrate);
    etx();
    key_val_int("kbitrate", lst->kbitrate);
    etx();
    key_val_int("samplerate", lst->samplerate);
    etx();
    key_val_dbl("ksamplerate", lst->ksamplerate);
    etx();
    key_val_int("channels", lst->channels);
    etx();
    key_val_int("length", lst->length);
    etx();
    key_val_int("mm:ss", lst->minutes);
    printf(":%d", lst->seconds);

    taglib_tag_free_strings();
    taggit_file_destroy(&f);
    free(lst);
}
