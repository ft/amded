/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file  list_machine.c
 * @brief print meta information from audio files in a machine readable form
 */

#include <stdio.h>
#include <stdlib.h>

#include <taglib/tag_c.h>

#include "taggit.h"
#include "taglib_ext.h"

/** ascii start-of-text character code */
#define ASCII_STX (char)0x02
/** ascii end-of-text character code */
#define ASCII_ETX (char)0x03

/**
 * Print a key-value pair, with the value being a string
 *
 * @param   key     name of the key
 * @param   val     value string
 *
 * @return void
 * @sideeffects none
 */
static inline void
key_val_str(const char *key, char *val)
{
    printf("%s%c%s", key, ASCII_STX, val);
}

/**
 * Print a key-value pair, with the value being an integer
 *
 * @param   key     name of the key
 * @param   val     the integer value
 *
 * @return void
 * @sideeffects none
 */
static inline void
key_val_int(const char *key, int val)
{
    printf("%s%c%d", key, ASCII_STX, val);
}

/**
 * Print a key-value pair, with the value being floating point data
 *
 * @param   key     name of the key
 * @param   val     the double value
 *
 * @return void
 * @sideeffects none
 */
static inline void
key_val_dbl(const char *key, double val)
{
    printf("%s%c%g", key, ASCII_STX, val);
}

/** print a single ETX character */
static inline void
etx(void)
{
    printf("%c", ASCII_ETX);
}

/**
 * Print meta information of a file seperated by ASCII control characters
 *
 * output format:\n
 * <code>
 *  key\<STX\>value\<ETX\>key\<STX\>value\<ETX\>key...value
 * </code>
 *
 * @param   file    file name of the file to process
 *
 * @return      void
 * @sideeffects none
 */
void
taggit_list_machine(const char *file)
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

    key_val_str("filename", (char*)file);
    etx();
    key_val_str("filetype", lst->filetype);
    etx();
    if (f.type == FT_MPEG) {
        key_val_str("tagtypes", lst->tagtypes);
        etx();
        key_val_str("tagtype", (char*)mp3_id_to_string(lst->tagtype));
        etx();
    }
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

    taglist_destroy(f.type, lst);
    taglib_tag_free_strings();
    taggit_file_destroy(&f);
    free(lst);
}
